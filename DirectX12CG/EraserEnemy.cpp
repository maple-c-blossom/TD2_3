#include "EraserEnemy.h"
#include "WritingEnemy.h"
void EraserEnemy::UniqueInitialize()
{
	for (auto& itr : attackObj.colliders)
	{
		itr.isTrigger = true;
	}
	enemyType = Erase;

}

void EraserEnemy::UniqueUpdate()
{
	velocity.V3Norm();
	float tempNorm = 1000;
	velocity = Float3(0.f,0.f,0.f);
	for (auto& itr : *WritingEnemy::GetHandWrite())
	{
		MCB::Vector3D temp(position, itr->position);
		float tempLen = temp.V3Len();
		if (tempLen < tempNorm)
		{
			temp.V3Norm();
			velocity = temp;
			tempNorm = tempLen;
		}
	}

	for (auto& itr : enemies)
	{
		if (itr->enemyType == Erase)continue;
		MCB::Vector3D temp(position, itr->position);
		float tempLen = temp.V3Len();
		if (tempLen < tempNorm)
		{
			temp.V3Norm();
			velocity = temp;
			tempNorm = tempLen;
		}
	}
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;
	for (auto& itr : *WritingEnemy::GetHandWrite())
	{
		for (auto& itr2 : itr->colliders)
		{
			for (auto& itr3 : colliders)
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
