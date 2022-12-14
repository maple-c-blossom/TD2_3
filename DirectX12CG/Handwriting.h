#pragma once
#include "Object3d.h"
#include "Util.h"
#include "Timer.h"
class Handwriting:public MCB::Object3d
{
private:
	int hp;
	static const int MAX_LIFE_TIME = 300;
	Timer lifeTimer;
public:
	void Initialize(MCB::Float3 position,MCB::Model* model);
	void Update();
	void Draw();
};

