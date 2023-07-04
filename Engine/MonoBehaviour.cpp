#include "pch.h"
#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{
}

MonoBehaviour::~MonoBehaviour()
{
}

/*
	{
		static TransformMat t;
		float speed = 1.f;

		if (GINPUT->GetButton(KEY_TYPE::W))
			t.offset.y += speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::S))
			t.offset.y -= speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::D))
			t.offset.x += speed * DELTA_TIME;
		if (GINPUT->GetButton(KEY_TYPE::A))
			t.offset.x -= speed * DELTA_TIME;

		mesh->SetTransform(t);

		mesh->Render();
	}
*/