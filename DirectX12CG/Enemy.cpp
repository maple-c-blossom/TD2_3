#include "Enemy.h"
#include "Status.h"
std::list<Enemy*> Enemy::allEnemyPtr{};
std::list<Enemy*> Enemy::enemies{};
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
	
	allEnemyPtr.push_back(this);
	allObjPtr.push_back(this);
	allObjPtr.push_back(&attackObj);
	for (auto& itr : colliders)
	{
		itr.Update(this);
	}
	for (auto& itr : attackObj.colliders)
	{
		itr.Update(&attackObj);
	}

}

std::list<Enemy*> Enemy::GetAllEnemies()
{
	return enemies;
}
