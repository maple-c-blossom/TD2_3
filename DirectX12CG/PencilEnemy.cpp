#include "PencilEnemy.h"
#include "Player.h"

using namespace MCB;
using namespace std;

void PencilEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed)
{
	//this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;

	ADXCollider tempAttackCol(this);
	colliders.push_back(tempAttackCol);
	colliders.push_back(this);
	ADXCollider tempAttackObjCol(&attackObj);
	tempAttackObjCol.isTrigger = true;
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
	attackObj.scale = { 2,2,2 };
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

	handwriting.remove_if([](auto& itr) {return itr->GetLifeTimeOver() || itr->deleteFlag; });

	if (Movement > WRITING_RADIUS)
	{
		unique_ptr<Handwriting> temp = make_unique<Handwriting>();
		temp->Initialize({ position.x,position.y,position.z }, handwritingModel);
		handwriting.push_back(move(temp));
		Movement = 0;
	}
	for (auto& itr : handwriting)
	{
		itr->Update();
	}

	if (movePoint.size() > 0)
	{
		movePointIndex = movePointIndex % movePoint.size();

		Vector3D positionVec = Vector3D(position.x,position.y,position.z);

		velocity = Vector3D::normal(movePoint[movePointIndex] - positionVec);

		float movePointDistance = (movePoint[movePointIndex] - positionVec).V3Len();
		if (movePointDistance >= nearestMovePointDistance)
		{
			notApproachingCount++;
			if (notApproachingCount > 10)
			{
				movePointIndex++;
				movePointIndex = movePointIndex % movePoint.size();
				nearestMovePointDistance = (movePoint[movePointIndex] - positionVec).V3Len();
				notApproachingCount = 0;
			}
		}
		else
		{
			nearestMovePointDistance = movePointDistance;
		}
	}

	allEnemyPtr.push_back(this);
	{
		int num = 0;
		for (auto& itr : colliders)
		{
			itr.Update(this);
			if (num == 0)
			{
				itr.isTrigger = true;
				itr.radius_ = 3;
			}
			num++;
		}
	}

	AttackCheck();
	AttackTimerUpdate();
	allObjPtr.push_back(&attackObj);
	for (auto& itr : attackObj.colliders)
	{
		itr.Update(&attackObj);
	}
	AttackHit();
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
	if (Player::GetPlayer() == nullptr)return;
	int num = 0;
	for (auto& itr : colliders)
	{
		if (num != 0)
		{
			break;
		}
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				AttackStart();
				Vector3D vec;
				vec.V3Get({ position.x,position.y ,position.z }, { Player::GetPlayer()->position.x,Player::GetPlayer()->position.y,Player::GetPlayer()->position.z });
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
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				Player::GetPlayer()->Damage(damage);
			}
		}
	}
}