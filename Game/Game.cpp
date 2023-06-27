#include "pch.h"
#include "Game.h"
#include "Engine.h"


void Game::Init(const WindowInfo& window)
{
	GEngine->Init(window);

	vector<Vertex> vertex(4);

	vertex[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vertex[0].color = Vec4(1.f, 0.f, 0.f, 1.f);

	vertex[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vertex[1].color = Vec4(0.f, 1.f, 0.f, 1.f);

	vertex[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vertex[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	vertex[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vertex[3].color = Vec4(0.f, 1.f, 0.f, 1.f);

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

	MESH->Init(vertex, index);

	// SHADER->Init(L"..\\Resources\\Shader\\default.hlsli");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->Render();
}
