#pragma once

class GameObject;

class Scene
{
public:

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

private:

	vector<shared_ptr<GameObject>> _gameObjects;
	// gameObject를 여러개의 벡터로 분할 (배경, 플레이어, 카메라 등) -> 필요한거 골라쓰기

};