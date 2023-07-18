#include "PencilBullet.h"
#include "Util.h"
void PencilBullet::Initialize(MCB::Model* model, float speed, MCB::Vector3D startPos, MCB::Vector3D targetPos)
{
	this->model_ = model;
	this->speed = speed;
	this->startPos = startPos;
	this->endPos = targetPos;
	lifeTime.Set(speed);
	this->position_ = { startPos.vec_.x_,startPos.vec_.y_,startPos.vec_.z_ };
	Init();
}

void PencilBullet::Update()
{
	if (lifeTime.IsEnd()) return;
	lifeTime.Update();
	MCB::InQuad(startPos.vec_.x_, endPos.vec_.x_, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
	MCB::InQuad(startPos.vec_.y_, endPos.vec_.y_, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
	MCB::InQuad(startPos.vec_.z_, endPos.vec_.z_, lifeTime.GetEndTime() - 5, lifeTime.NowTime());
}