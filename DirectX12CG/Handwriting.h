#pragma once
#include "Object3d.h"
#include "Util.h"
#include "Timer.h"

	enum TypeName
	{
		Write,
		Bonus
	};
class Enemy;
class Handwriting:public MCB::Object3d
{
private:
	Timer lifeTimer;
	MCB::ObjectMaterial material;
public:
	int generatorType = Write;
	void Initialize(MCB::Float3 position,MCB::Model* model);
	void Update();
	void Draw();
	bool GetLifeTimeOver();
	void SetDelete(bool flag) { deleteFlag = flag; };
};

