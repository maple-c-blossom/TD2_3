#include "Handwriting.h"
#include "Status.h"
#include "Player.h"

void Handwriting::Initialize(MCB::Float3 position, MCB::Model* model)
{
	lifeTimer.Set(MAX_LIFE_TIME_HAND_WRITING);
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	material.Init();
	material.material = model->material.material;
	Init();
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
}

void Handwriting::Update()
{

}

void Handwriting::Draw()
{
	Object3d::Draw(&material);
}

bool Handwriting::GetLifeTimeOver()
{
	return lifeTimer.IsEnd();
}
