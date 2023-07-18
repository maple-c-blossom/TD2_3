#include "mechanicalPencil.h"
#include "Player.h"
#include "Status.h"


using namespace MCB;
using namespace std;

void mechanicalPencil::UniqueInitialize()
{
	ADXCollider tempAttackCol(this);
	attackCol.push_back(tempAttackCol);
	ADXCollider tempAttackObjCol(&attackObj);
	attackObj.colliders.push_back(tempAttackObjCol);
	for (auto& itr : attackObj.colliders)
	{
		itr.isTrigger = true;
	}
	attackObj.model = model;
	attackObj.scale = { 2,2,2 };
	attackObj.Init();
}

void mechanicalPencil::UniqueUpdate(bool flag)
{
	velocity.V3Norm();
	if (capture == nullptr)
	{
		if (!attack && !beforeAttack)
		{
			position.x += velocity.vec.x * speed;
			position.y += velocity.vec.y * speed;
			position.z += velocity.vec.z * speed;
			Movement += speed;
		}
		if (Movement > WRITING_RADIUS)
		{
			unique_ptr<Handwriting> temp = make_unique<Handwriting>();
			temp->Initialize({ position.x,position.y,position.z }, handwritingModel);
			handwriting.push_back(move(temp));
		}
	}
	for (auto& itr : handwriting)
	{
		itr->Update();
	}

	if (position.x < -20 || position.x > 20)
	{
		velocity.vec.x *= -1;
	}

	if (position.y < -20 || position.y > 20)
	{
		velocity.vec.y *= -1;
	}

	if (position.z < -20 || position.z > 20)
	{
		velocity.vec.z *= -1;
	}

	allEnemyPtr.push_back(this);
	{
		int num = 0;
		for (auto& itr : colliders)
		{
			itr.Update(this);
			if (num == 0)
			{
				itr.radius_ = 3;
			}
			num++;
		}
	}

	AttackCheck();
	AttackTimerUpdate();
	attackObj.UpdateData();
	AttackHit();
}

void mechanicalPencil::Draw()
{
	Object3d::Draw();
	if (attack)
	{
		attackObj.Draw();
	}
	for (auto& itr : handwriting)
	{
		itr->Draw();
	}
}

void mechanicalPencil::UpdateMatrix(MCB::ICamera* camera)
{
	for (auto& itr : handwriting)
	{
		itr->Object3d::Update(*camera->GetView(), *camera->GetProjection());
	}
	attackObj.Update(*camera->GetView(), *camera->GetProjection());
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
}

void mechanicalPencil::AttackCheck()
{
	if (attackTimer.IsEnd())
	{
		attackTimer.Set(30);
		beforeAttack = true;
	}
}

void mechanicalPencil::AttackHit()
{
	if (!attack)return;
	std::unique_ptr<PencilBullet> temp = std::make_unique<PencilBullet>();
	temp->Initialize(bulletModel, 1, nowFrontVec_, { position.x,position.y,position.z });
	bullets.push_back(std::move(temp));
	attack = false;
}



void mechanicalPencil::Turn()
{
	velocity *= -1;
}
