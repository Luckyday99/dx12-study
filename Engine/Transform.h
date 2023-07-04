#pragma once
#include "Component.h"

struct TransformMat
{
	Vec4 offset;
};


class Transform : public Component
{
public:

	Transform();
	virtual ~Transform();

private:

};

