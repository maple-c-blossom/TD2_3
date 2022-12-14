#include "EraserEnemy.h"

void EraserEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}
