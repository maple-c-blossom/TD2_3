#include "BossDamageEffect.h"

void BossDamageEffect::Initialize(MCB::Model* model, MCB::Vector3D Velocity, MCB::Float3 position, MCB::Float3 scale, MCB::Float4 color, float speed, int lifeTime)
{
	this->model = model;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->defaultScale.x = scale.x;
	this->defaultScale.y = scale.y;
	this->defaultScale.z = scale.z;
	defaultSpeed = speed;
	lifeTimer.Set(lifeTime);
	Object3d::Init();
	nowFrontVec.vec.x = Velocity.vec.x;
	nowFrontVec.vec.y = Velocity.vec.y;
	nowFrontVec.vec.z = Velocity.vec.z;
	nowFrontVec.V3Norm();
	material.Init();
	material.constMapMaterial->color = color;
}

void BossDamageEffect::Update()
{
	lifeTimer.SafeUpdate();
	speed = InQuad(defaultSpeed, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale.x = InQuad(defaultScale.x, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale.y = InQuad(defaultScale.y, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale.z = InQuad(defaultScale.z, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	material.constMapMaterial->color.w = InQuad(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	position.x += nowFrontVec.vec.x * speed;
	position.y += nowFrontVec.vec.y * speed;
	position.z += nowFrontVec.vec.z * speed;
	//rotation.x += ConvertRadius(speed);
	if (lifeTimer.IsEnd())
	{
		deleteFlag = true;
	}
}

void BossDamageEffect::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection(),true);
}

void BossDamageEffect::Draw()
{
	Object3d::Draw(&material);
}
