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
bool Enemy::IsAttack()
{
	return attack;
}
void Enemy::Update()
{
	UniqueUpdate();
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
	allEnemyPtr.push_back(this);
	allObjPtr.push_back(this);
}

std::list<Enemy*> Enemy::GetAllEnemies()
{
	return enemies;
}
