#pragma once


enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	/* ... */
	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1,
};

class GameObject;
class Transform;

class Component
{
public:

	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:

	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }

public:

	COMPONENT_TYPE GetType() { return _type; }
	bool isValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject>	GetGameObject();
	shared_ptr<Transform>	GetTransform();


private:
	friend class GameObject;

	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

private:

	COMPONENT_TYPE			_type;
	weak_ptr<GameObject>	_gameObject; // 서로가 서로를 알아야 하므로 순환구조 발생


};

