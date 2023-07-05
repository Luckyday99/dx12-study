#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"


void Game::Init(const WindowInfo& window)
{
	GENGINE->Init(window);

	GET_SINGLE(SceneManager)->LoadScene(L"TestScene");
}

void Game::Update()
{
	GENGINE->Update();
}
