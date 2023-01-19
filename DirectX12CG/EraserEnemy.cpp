#include "EraserEnemy.h"

void EraserEnemy::UniqueInitialize()
{
	for (auto& itr : attackObj.colliders)
	{
		itr.isTrigger = true;
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
