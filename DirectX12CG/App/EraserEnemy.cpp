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

void EraserEnemy::UniqueUpdate(bool limitMove)
{
	if (!Object3d::IsValid(this))return;
	if (limitMove)
	{
		velocity.V3Norm();
		float tempNorm = 1000;
		velocity = Float3(0.f,0.f,0.f);
		for (auto& itr : *WritingEnemy::GetHandWrite())
		{
			if (itr->generatorType == TypeName::Bonus || !Object3d::IsValid(&*itr)) continue;
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
			if (itr->generatorType == TypeName::Bonus) continue;
			for (auto& itr2 : itr->colliders)
			{
				if (&itr2 == nullptr) continue;
				for (auto& itr3 : colliders)
				{
					if (&itr3 == nullptr) continue;
					if (itr3.IsHit(itr2))
					{
						itr->SetDelete(true);
	
					}
				}
			}
		}
	
		if (position.x < -50)
		{
			position.x = -50;
		}
		if (position.x > 50)
		{
			position.x = 50;
		}
	
		if (position.z < -40)
		{
			position.z = -40;
		}
		if (position.z > 40)
		{
			position.z = 40;
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
