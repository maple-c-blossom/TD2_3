#include "Handwriting.h"
#include "Status.h"
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
}

void Handwriting::Update()
{
	lifeTimer.SafeUpdate();
	material.constMapMaterial->alpha = MCB::Lerp(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
}

void Handwriting::Draw()
{
	Object3d::Draw(&material);
}

bool Handwriting::GetLifeTimeOver()
{
	return lifeTimer.IsEnd();
}
