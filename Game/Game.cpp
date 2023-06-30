#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& window)
{
	GEngine->Init(window);

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

	mesh->Init(vertex, index);
	texture->Init(L"..\\Resources\\Texture\\Kitty.jpg");

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	// 렌더링 할 내용

	shader->Update();

	{
		Transform t;
		t.offset = Vec4(0.25f, 0.25f, 0.1f, 0.f);
		mesh->SetTransform(t);

		mesh->SetTexture(texture);

		mesh->Render();
	}
	{
		Transform t;
		t.offset = Vec4(-0.25f, -0.25f, 0.f, 0.f);
		mesh->SetTransform(t);

		mesh->SetTexture(texture);

		mesh->Render();
	}

	GEngine->RenderEnd();
}
