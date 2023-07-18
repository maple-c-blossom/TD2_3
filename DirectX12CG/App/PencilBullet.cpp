#include "PencilBullet.h"
#include "Util.h"
void PencilBullet::Initialize(MCB::Model* model, float speed, MCB::Vector3D startPos, MCB::Vector3D targetPos)
{
	nowFrontVec_.vec_ = model;
	this->speed = speed;
	this->startPos = startPos;
	this->endPos = targetPos;
	lifeTime.Set(speed);
	this->position = { startPos.vec.x,startPos.vec.y,startPos.vec.z };
	Init();
}

void PencilBullet::Update()
{
	if (lifeTime.IsEnd()) return;
	lifeTime.Update();
	MCB::InQuad(startPos.vec.x, endPos.vec.x, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
	MCB::InQuad(startPos.vec.y, endPos.vec.y, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
	MCB::InQuad(startPos.vec.z, endPos.vec.z, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
}