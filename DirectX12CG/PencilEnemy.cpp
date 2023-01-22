#include "PencilEnemy.h"
#include "Player.h"
#include "Handwriting.h"
using namespace MCB;
using namespace std;

void PencilEnemy::UniqueInitialize()
{
	ADXCollider tempAttackCol(this);
	tempAttackCol.radius_ = 10;
	tempAttackCol.isTrigger = true;
	attackCol.push_back(tempAttackCol);

}

void PencilEnemy::UniqueUpdate()
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
		else if (attack)
		{
			position.x += attackVec.vec.x * speed * 2;
			position.y += attackVec.vec.y * speed * 2;
			position.z += attackVec.vec.z * speed * 2;

			for (auto& itr : movePoint)
			{
				itr += (attackVec * speed * 2);
			}
			Movement += speed * 2;
		}

		if (Movement > WRITING_RADIUS)
		{
			unique_ptr<Handwriting> temp = make_unique<Handwriting>();
			temp->Initialize({ position.x,position.y,position.z }, handwritingModel);
			handwriting.push_back(move(temp));
			Movement = 0;
			writingCount++;
		}

		if (writingCount > 20)
		{
			writingCount = 0;
			velocity *= -1;
		}


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
			if (num == 1)
			{
				itr.isTrigger = true;
				itr.radius_ = 3;
			}
			num++;
		}
	}

	allObjPtr.push_back(&attackObj);
	if (capture == nullptr)
	{
		AttackCheck();
		AttackTimerUpdate();
	}
	else
	{
		attack = false;
	}
	for (auto& itr : attackCol)
	{
		itr.Update(this);
	}
	if (capture == nullptr)
	{
		AttackHit();
	}
}

void PencilEnemy::Draw()
{
	Object3d::Draw();

}

void PencilEnemy::UpdateMatrix(MCB::ICamera* camera)
{
	for (auto& itr : handwriting)
	{
		itr->Object3d::Update(*camera->GetView(),*camera->GetProjection());
	}
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
}

void PencilEnemy::AttackCheck()
{
	if (Player::GetPlayer() == nullptr || attack)return;
	int num = 0;
	for (auto& itr : attackCol)
	{
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				AttackStart();
				attackVec = attackVec.V3Get(position, Player::GetPlayer()->position);
				attackVec.V3Norm();

				//attackObj.position = { position.x + vec.vec.x * 2,position.y + vec.vec.y * 2,position.z + vec.vec.z * 2 };
			}
		}
		num++;
	}
}

void PencilEnemy::AttackHit()
{
	if (!attack)return;
	int num = 0;
	for (auto& itr : colliders)
	{
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				Player::GetPlayer()->Damage(damage);
			}
		}
		num++;
	}
}