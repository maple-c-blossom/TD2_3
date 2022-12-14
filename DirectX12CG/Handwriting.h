#pragma once
#include "Object3d.h"
#include "Util.h"
#include "Timer.h"
class Handwriting:public MCB::Object3d
{
private:
	Timer lifeTimer;
	MCB::ObjectMaterial material;

public:
	void Initialize(MCB::Float3 position,MCB::Model* model);
	void Update();
	void Draw();
	bool GetLifeTimeOver();
};

