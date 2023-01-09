#pragma once
#include "Object3d.h"
#include "Timer.h"
#include "PencilBullet.h"
class PencilBullet :public MCB::Object3d
{
private:
	MCB::Vector3D startPos;
	MCB::Vector3D endPos;
	float speed;
	Timer lifeTime;
public:
	void Initialize(MCB::Model* model, float speed, MCB::Vector3D startPos, MCB::Vector3D targetPos);
	void Update();
	bool GetDelete() { return lifeTime.IsEnd(); }
};

