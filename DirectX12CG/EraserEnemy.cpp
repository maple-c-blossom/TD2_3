#include "EraserEnemy.h"

void EraserEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model,float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;
	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}

	Object3d::Init();

}

void EraserEnemy::UniqueUpdate()
{
	velocity.V3Norm();
	float tempNorm = 1000;
	for (auto& itr : this->handwritingPtr)
	{
		MCB::Vector3D temp;
		temp.V3Get(position, itr->position);
		float tempLen = temp.V3Len();
		if (tempLen < tempNorm)
		{
			velocity = temp;
			tempNorm = tempLen;
		}
	}
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;
	for (auto& itr : this->handwritingPtr)
	{
		for (auto& itr2 : itr->colliders)
		{
			for (auto& itr3 : this->colliders)
			{
				if (itr3.IsHit(itr2))
				{
					itr->SetDelete(true);
				}
			}
		}
	}



}

void EraserEnemy::Draw()
{
	Object3d::Draw();
}

void EraserEnemy::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
}

void EraserEnemy::Turn()
{
	velocity *= -1;

}
