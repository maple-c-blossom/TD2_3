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

void PencilEnemy::UniqueUpdate(bool movelimit)
{
	if (movelimit)
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
				position.x += attackVec.vec.x * speed;
				position.y += attackVec.vec.y * speed;
				position.z += attackVec.vec.z * speed;

				std::array<Vector3D, 3> PrevmovePoint = {};
				bool flag = false;
				int num = 0;
				for (auto& itr : movePoint)
				{
					PrevmovePoint[num] = itr;
					itr += (attackVec * speed * 2);
					if (itr.vec.x < -50)
					{
						 flag = true;
					}
					if (itr.vec.x > 50)
					{
						flag = true;
					}

					if (itr.vec.z < -40)
					{
						flag = true;
					}
					if (itr.vec.z > 40)
					{
						flag = true;
					}

					itr = PrevmovePoint[num];
					num++;
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
			Vector3D PrevmovePoint = movePoint[movePointIndex];

			Vector3D positionVec = Vector3D(position.x, position.y, position.z);

			Float2 temp;
			temp.x = MCB::Lerp(0, 85, (movePoint[movePointIndex].vec.z + 30) / 85);
			temp.x /= 85;
			Float2 Vartical;
			Vartical.x = MCB::Lerp(-40, -80, temp.x);
			Vartical.y = MCB::Lerp(40, 80, temp.x);
			if (movePoint[movePointIndex].vec.x < -50)
			{
				movePoint[movePointIndex].vec.x = PrevmovePoint.vec.x;
			}
			if (movePoint[movePointIndex].vec.x > 50)
			{
				movePoint[movePointIndex].vec.x = PrevmovePoint.vec.x;
			}

			if (movePoint[movePointIndex].vec.z < -40)
			{
				movePoint[movePointIndex].vec.z = PrevmovePoint.vec.z;
			}
			if (movePoint[movePointIndex].vec.z > 40)
			{
				movePoint[movePointIndex].vec.z = PrevmovePoint.vec.z;
			}
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
	}
	allEnemyPtr.push_back(this);
	{
		int num = 0;
		for (auto& itr : colliders)
		{
			if (num == 1)
			{
				itr.isTrigger = true;
				itr.radius_ = 3;
			}
			num++;
		}
	}

	if (capture == nullptr && movelimit)
	{
		AttackCheck();
		AttackTimerUpdate();
	}
	else
	{
		attack = false;
		beforeAttack = false;
		color = { 1.f,1.f,1.f,1.f }; 
	}

	bodySlam = attack;

	for (auto& itr : attackCol)
	{
		itr.Update(this);
	}
	if (capture == nullptr)
	{
		AttackHit();
	}
	//Float2 temp;
//temp.x = MCB::Lerp(0, 85,(position.z + 30) / 85);
//temp.x = (position.z + 30) / 85;
//Float2 Vartical;
//Vartical.x = MCB::Lerp(-40, -80, temp.x);
//Vartical.y = MCB::Lerp(40, 80, temp.x);
	if (movelimit)
	{


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