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
	if (this->deleteFlag)return;
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
	for (auto& itr : colliders)
	{
		
		for (auto& itr2 : enemies)
		{
			if (itr2->enemyType == Erase || itr2->deleteFlag || !itr2->imotalTimer.IsEnd())continue;
			for (auto& itr3 : itr2->colliders)
			{
				if (itr3.gameObject->deleteFlag)continue;
				if (itr3.IsHit(itr))
				{
					bool flag = itr2->deleteFlag;
					itr2->IsDamage(1);
					if (flag != itr2->deleteFlag)
					{
						for (int i = 0; i < 10; i++)
						{
							std::unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
							float angle = ConvertRadius((i * 32.f));
							temp->Initialize({ position.x + sinf(angle) * 2,position.y,position.z + cosf(angle) * 2 }, handwritingModel);
							WritingEnemy::GetHandWrite()->push_back(move(temp));
						}
					}
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
