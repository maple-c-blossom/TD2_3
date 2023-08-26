#pragma once
#include "Object3d.h"
#include "Timer.h"
class Boss;
class Cleaner :public MCB::Object3d
{
	std::array<MCB::Vector3D, 6>movePoint;
	int moveindex = 2;
	int moveEndindex = 3;
	Timer moveTime;
public:
	Boss* boss = nullptr;
	void Inilialize(MCB::Model* model);
	void Update();
};
