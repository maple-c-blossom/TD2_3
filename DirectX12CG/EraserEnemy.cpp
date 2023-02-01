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
	if (!Object3d::IsValid(this))return;
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
			for (auto& itr3 : colliders)
			{
				if (itr3.IsHit(itr2))
				{
					itr->SetDelete(true);

				}
			}
		}
	}
	//for (auto& itr : colliders)
	//{
	//	
	//	for (auto& itr2 : enemies)
	//	{
	//		if (itr2->enemyType == Erase || !Object3d::IsValid(itr2) || !itr2->imotalTimer.IsEnd())continue;
	//		for (auto& itr3 : itr2->colliders)
	//		{
	//			if (!Object3d::IsValid(itr3.gameObject))continue;
	//			if (itr3.IsHit(itr))
	//			{
	//				bool flag = itr2->deleteFlag;
	//				itr2->IsDamage(1);
	//				if (flag != itr2->deleteFlag)
	//				{
	//					for (int i = 0; i < 10; i++)
	//					{
	//						std::unique_ptr<Handwriting> temp = std::make_unique<Handwriting>();
	//						float angle = ConvertRadius((i * 32.f));
	//						temp->Initialize({ position.x + sinf(angle) * 2,position.y,position.z + cosf(angle) * 2 }, handwritingModel);
	//						temp->generatorType = TypeName::Bonus;
	//						WritingEnemy::GetHandWrite()->push_back(move(temp));
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	Float2 temp;
	//temp.x = MCB::Lerp(0, 85, (position.z + 30) / 85);
	temp.x = (position.z + 30) / 85;
	Float2 Vartical;
	Vartical.x = MCB::Lerp(-40, -80, temp.x);
	Vartical.y = MCB::Lerp(40, 80, temp.x);
	if (position.x < Vartical.x)
	{
		position.x = Vartical.x;
	}
	if (position.x > Vartical.y)
	{
		position.x = Vartical.y;
	}

	if (position.z < -30)
	{
		position.z = -30;
	}
	if (position.z > 55)
	{
		position.z = 55;
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
