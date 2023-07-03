#include "pch.h"
#include "Game.h"
#include "Engine.h"



void Game::Init(const WindowInfo& window)
{
	GENGINE->Init(window);
}

void Game::Update()
{
	GENGINE->Update();

	GENGINE->Render();
}
