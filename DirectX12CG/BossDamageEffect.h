#pragma once
#include "Object3d.h"
#include "Timer.h"
#include "Util.h"
#include "ICamera.h"
class BossDamageEffect:public MCB::Object3d
{
	Timer lifeTimer;
	float defaultSpeed;
	float speed;
	Float3 defaultScale;
	MCB::ObjectMaterial material;
public:
	void Initialize(MCB::Model* model, MCB::Vector3D Velocity, MCB::Float3 position,MCB::Float3 scale, MCB::Float4 color,float speed,int lifeTime);
	void Update();
	void UpdateMatrix(MCB::ICamera* camera);
	void Draw();
};

