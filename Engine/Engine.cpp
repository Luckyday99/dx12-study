#include "pch.h"
#include "Engine.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

void Engine::Init(const WindowInfo& window)
{
	_window = window;

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(window, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_dsBuffer->Init(window);
	_tableDescHeap->Init(256);

	GET_SINGLE(Input)->Init(window.hwnd);
	GET_SINGLE(Timer)->Init();

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformMat), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);

	GENGINE->GetCmdQueue()->WaitSync();

	ResizeWindow(window.width, window.height);
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();

	Render();

	ShowFps();
}

void Engine::Render()
{
	RenderBegin();

	// 렌더링 할 내용
	GET_SINGLE(SceneManager)->Update();

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
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

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeint = static_cast<REGISTER_TYPE>(reg);
	assert(_cstBuffers.size() == typeint);

	shared_ptr<ConstantBuffer> cstBuffer = make_shared<ConstantBuffer>();
	cstBuffer->Init(reg, bufferSize, count);
	_cstBuffers.push_back(cstBuffer);
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);
}