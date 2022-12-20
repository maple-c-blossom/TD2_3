#include "EraserEnemy.h"

void EraserEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model,float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;

}

void EraserEnemy::UniqueUpdate()
{
	velocity.V3Norm();
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;

}

void EraserEnemy::Draw()
{
	Object3d::Draw();
}

void EraserEnemy::Turn()
{
	velocity *= -1;

}
