#include "Handwriting.h"

void Handwriting::Initialize(MCB::Float3 position, MCB::Model* model)
{
	lifeTimer.Set(MAX_LIFE_TIME);
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	Init();
}

void Handwriting::Update()
{
	
}
