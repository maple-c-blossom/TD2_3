#include "PencilEnemy.h"
#include "Player.h"

using namespace MCB;
using namespace std;

void PencilEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;

	ADXCollider tempAttackCol(this);
	colliders.push_back(tempAttackCol);
	ADXCollider tempAttackObjCol(&attackObj);
	attackObj.colliders.push_back(tempAttackObjCol);
	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}
	for (auto& itr : attackObj.colliders)
	{
		itr.isTrigger = true;
	}
	Object3d::Init();
	attackObj.model = model;
	attackObj.Init();
}

void PencilEnemy::UniqueUpdate()
{
	velocity.V3Norm();
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
	for (auto& itr : attackObj.colliders)
	{
		itr.Update(&attackObj);
	}
	AttackHit();
	handwriting.remove_if([](auto& itr) {return itr->GetLifeTimeOver(); });

	allEnemyPtr.push_back(this);

}

void PencilEnemy::Draw()
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

void PencilEnemy::UpdateMatrix(MCB::ICamera* camera)
{
	for (auto& itr : handwriting)
	{
		itr->Object3d::Update(*camera->GetView(),*camera->GetProjection());
	}
	attackObj.Update(*camera->GetView(), *camera->GetProjection());
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
}

void PencilEnemy::AttackCheck()
{
	if (playerPtr == nullptr)return;
	int num = 0;
	for (auto& itr : colliders)
	{
		if (num != 0)
		{
			break;
		}
		for (auto& itr2 : playerPtr->colliders)
		{
			if (itr.IsHit(itr2))
			{
				AttackStart();
				Vector3D vec;
				vec.V3Get({ position.x,position.y ,position.z }, { playerPtr->position.x,playerPtr->position.y,playerPtr->position.z });
				vec.V3Norm();
				attackObj.position = { position.x + vec.vec.x * 2,position.y + vec.vec.y * 2,position.z + vec.vec.z * 2 };
			}
		}
		num++;
	}
}

void PencilEnemy::AttackHit()
{
	if (!attack)return;
	for (auto& itr : attackObj.colliders)
	{
		for (auto& itr2 : playerPtr->colliders)
		{
			if (itr.IsHit(itr2))
			{
				playerPtr->SetHp(playerPtr->GetHp() - damage);
			}
		}
	}
}



void PencilEnemy::Turn()
{
	velocity *= -1;
}
