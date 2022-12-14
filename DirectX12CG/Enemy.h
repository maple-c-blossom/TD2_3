#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
#include "Handwriting.h"
class Enemy:public MCB::Object3d
{
protected:
	int hp;
	float speed;
	MCB::Vector3D velocity;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

