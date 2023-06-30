#include "pch.h"
#include "Engine.h"


void Engine::Init(const WindowInfo& window)
{
	_window = window;

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_cstBuffer = make_shared<ConstantBuffer>();
	_dsBuffer = make_shared<DepthStencilBuffer>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(window, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_cstBuffer->Init(sizeof(Transform), 256);
	_dsBuffer->Init(window);
	_tableDescHeap->Init(256);

	ResizeWindow(window.width, window.height);
}

void Engine::Render()
{
	RenderBegin();

	RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	RECT rect = { 0, 0, width, height };
	_window.width = width;
	_window.height = height;

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	_dsBuffer->Init(_window);
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}   