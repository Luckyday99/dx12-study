#include "pch.h"
#include "Engine.h"


void Engine::Init(const WindowInfo& window)
{
	_window = window;
	GINPUT->Init(window.hwnd);

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(window.width), static_cast<FLOAT>(window.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(window, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_cstBuffer->Init(sizeof(Transform), 256);
	_dsBuffer->Init(window);
	_tableDescHeap->Init(256);

	_timer->Init();

	// Create Mesh
	vector<Vertex> vertex(4);
	{
		vertex[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
		vertex[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
		vertex[0].uv = Vec2(0.f, 0.f);
	}
	{
		vertex[1].pos = Vec3(0.5f, 0.5f, 0.5f);
		vertex[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
		vertex[1].uv = Vec2(1.f, 0.f);
	}
	{
		vertex[2].pos = Vec3(0.5f, -0.5f, 0.5f);
		vertex[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
		vertex[2].uv = Vec2(1.f, 1.f);
	}
	{
		vertex[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
		vertex[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
		vertex[3].uv = Vec2(0.f, 1.f);
	}

	vector<uint32> index;
	{
		index.push_back(0);
		index.push_back(1);
		index.push_back(2);
	}
	{
		index.push_back(0);
		index.push_back(2);
		index.push_back(3);
	}

	_mesh->Init(vertex, index);
	_texture->Init(L"..\\Resources\\Texture\\Kitty.jpg");

	_shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	GENGINE->GetCmdQueue()->WaitSync();

	ResizeWindow(window.width, window.height);
}

void Engine::Render()
{
	RenderBegin();

	// 렌더링 할 내용

	_shader->Update();

	{
		static Transform t;
		float speed = 1.f;

		if (GINPUT->GetButton(KEY_TYPE::W))
			t.offset.y += speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::S))
			t.offset.y -= speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::D))
			t.offset.x += speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::A))
			t.offset.x -= speed * DELTA_TIME;

		_mesh->SetTransform(t);

		_mesh->SetTexture(_texture);

		_mesh->Render();
	}

	RenderEnd();
}

void Engine::Update()
{
	GINPUT->Update();
	_timer->Update();

	ShowFps();
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

void Engine::ShowFps()
{
	uint32 fps = _timer->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);
}