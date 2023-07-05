#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Engine.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// 기존 Scene 정리
	// 파일에서 Scene 정보 Load

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

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

	gameObject->Init();
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertex, index);
	meshRenderer->SetMesh(mesh);

	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Init(L"..\\Resources\\Texture\\Kitty.jpg");

	shared_ptr<Material> material = make_shared<Material>();
	material->SetShader(shader);
	material->SetFloat(0, 0.3f);
	material->SetFloat(1, 0.4f);
	material->SetFloat(2, 0.3f);
	material->SetTexture(0, texture);
	meshRenderer->SetMaterial(material);

	gameObject->AddComponent(meshRenderer);

	scene->AddGameObject(gameObject);

	return scene;
}
