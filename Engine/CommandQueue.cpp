#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator : 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	_cmdList->Close();

	// 리소스 작업용 Command List
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));


	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

// CommandList에 실행할 정보를 밀어넣는 과정
void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// 외주 결과물

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE->GetSignature().Get());
	GENGINE->GetCstBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GENGINE->GetCstBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	GENGINE->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GENGINE->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// 출력 병합기 (OutPut Merge)

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStrencilView = GENGINE->GetDSBuffer()->GetDsvCpuHandle();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);

	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStrencilView);

	_cmdList->ClearDepthStencilView(depthStrencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
}

void CommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();

	_swapChain->SwapIndex();
}

void CommandQueue::FlushResourceCommandQueue()
{
	_resCmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* resCmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(resCmdListArr), resCmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}
