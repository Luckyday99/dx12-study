#pragma once

class SwapChain;

// CommandQueue : DX12에 등장
// 요청을 CMD LIST에 모아서 한번에 GPU로 전송
class CommandQueue
{
public:

	~CommandQueue();
	
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResCmdList() { return _resCmdList; }

private:

	ComPtr<ID3D12CommandQueue>			_cmdQueue;

	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;
	
	// 리소스 작업용 커멘드 리스트(렌더링할때 매번 열고 닫을 필요가 없음)
	// Command List는 꼭 하나일 필요가 없다!
	// ==> Cmd List를 객체로 따로 묶는게 더 낫지 않음?
	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;

	// 멀티쓰레드를 위해 fence를 이용
	// CPU / GPU 동기화를 위한 간단한 도구
	// GPU가 일을 끝낼때까지 CPU가 대기(WaitSync)하는 방식으로 구현(효율 나쁨)
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};