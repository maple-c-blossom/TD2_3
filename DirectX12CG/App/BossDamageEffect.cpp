#include "BossDamageEffect.h"

void BossDamageEffect::Initialize(MCB::Model* model, MCB::Vector3D Velocity, MCB::Float3 position, MCB::Float3 scale, MCB::Float4 color, float speed, int lifeTime)
{
	this->model_ = model;
	this->position_.x = position.x_;
	this->position_.y = position.y_;
	this->position_.z = position.z_;
	this->defaultScale.x_ = scale.x_;
	this->defaultScale.y_ = scale.y_;
	this->defaultScale.z_ = scale.z_;
	defaultSpeed = speed;
	lifeTimer.Set(lifeTime);
	Object3d::Init();
	nowFrontVec_.vec_.x_ = Velocity.vec_.x_;
	nowFrontVec_.vec_.y_ = Velocity.vec_.y_;
	nowFrontVec_.vec_.z_ = Velocity.vec_.z_;
	nowFrontVec_.V3Norm();
	this->color_ = color;
}

void BossDamageEffect::Update()
{
	lifeTimer.SafeUpdate();
	speed = InQuad(defaultSpeed, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale_.x = InQuad(defaultScale.x_, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale_.y = InQuad(defaultScale.y_, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	scale_.z = InQuad(defaultScale.z_, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	color_.w_ = InQuad(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	position_.x += nowFrontVec_.vec_.x_ * speed;
	position_.y += nowFrontVec_.vec_.y_ * speed;
	position_.z += nowFrontVec_.vec_.z_ * speed;
	//rotation.x_ += ConvertRadius(speed);
	if (lifeTimer.IsEnd())
	{
		deleteFlag_ = true;
	}
}

void BossDamageEffect::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::camera_ = camera;
	Object3d::Update();
}

void BossDamageEffect::Draw()
{
	Object3d::Draw();
}
