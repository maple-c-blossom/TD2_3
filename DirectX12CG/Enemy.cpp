#include "Enemy.h"
std::list<Enemy*> Enemy::allEnemyPtr{};
std::list<Enemy*> Enemy::enemies{};
void Enemy::StaticUpdate()
{
	enemies = allEnemyPtr;
	allEnemyPtr.clear();
}

void Enemy::Update()
{
	UniqueUpdate();
	allEnemyPtr.push_back(this);
	allObjPtr.push_back(this);
}

std::list<Enemy*> Enemy::GetAllEnemies()
{
	return enemies;
}
