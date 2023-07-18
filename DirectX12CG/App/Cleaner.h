#pragma once
#include "Object3d.h"
#include "Timer.h"
class Cleaner :public MCB::Object3d
{
	std::array<MCB::Vector3D, 4>movePoint;
	int moveindex = 0;
	int moveEndindex = 1;
	Timer moveTime;
public:
	void Inilialize(MCB::Model* model);
	void Update();
};
