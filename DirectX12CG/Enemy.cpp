#include "Enemy.h"
#include "Status.h"
#include "Player.h"

std::list<Enemy*> Enemy::allEnemyPtr{};
std::list<Enemy*> Enemy::enemies{};

using namespace MCB;

void Enemy::StaticUpdate()
{
	enemies = allEnemyPtr;
	allEnemyPtr.clear();
}

void Enemy::AttackStart()
{
	if (!beforeAttack)
	{
		beforeAttackTimer.Set(ENEMY_BEFORE_ATTACK_TIME);
	}
	beforeAttack = true;
}

void Enemy::AttackTimerUpdate()
{
	if (beforeAttack)
	{
		beforeAttackTimer.SafeUpdate();
		if (beforeAttackTimer.IsEnd())
		{
			beforeAttack = false;
			attack = true;
			AttackTimer.Set(ENEMY_ATTACK_TIME);
		}
	}
	else if (attack)
	{
		AttackTimer.SafeUpdate();
		if (AttackTimer.IsEnd())
		{
			attack = false;
		}
	}
}

bool Enemy::IsAttack()
{
	return attack;
}

void Enemy::Update()
{

	UniqueUpdate();

	if (capture == nullptr)
	{
		for (auto& itr : colliders)
		{
			for (auto& colListItr : itr.collideList)
			{
				for (auto& colListItr2 : KneadedEraser::GetAllKneadedEraser())
				{
					if (colListItr->gameObject == colListItr2)
					{
						capture = colListItr2;
					}
				}
			}
			itr.Update(this);
		}
	}

	if (capture != nullptr && prevCapture == nullptr)
	{
		Vector3D positionVec = Vector3D(position.x, position.y, position.z);
		captureLocalPos = MCBMatrix::transform(positionVec, MCB::MCBMatrix::MCBMatrixConvertXMMatrix(capture->matWorld.matWorld).Inverse());
	}
	prevCapture = capture;

	if (capture != nullptr)
	{
		Vector3D positionVec = MCBMatrix::transform(captureLocalPos, MCB::MCBMatrix::MCBMatrixConvertXMMatrix(capture->matWorld.matWorld));
		position = positionVec.ConvertXMFloat3();
	}
	
	allEnemyPtr.push_back(this);
	allObjPtr.push_back(this);

	for (auto& itr : colliders)
	{
		itr.Update(this);
	}
}

std::list<Enemy*> Enemy::GetAllEnemies()
{
	return enemies;
}