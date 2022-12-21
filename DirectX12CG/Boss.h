#pragma once
#include "Object3d.h"
#include "EraserEnemy.h"
#include "PencilEnemy.h"
class Boss :public MCB::Object3d
{
private:
	std::list<std::unique_ptr<Enemy>>enemys;
	int hp;
	float speed;
	MCB::Vector3D velocity;
	bool attack = false;
	bool beforeAttack = false;
	bool prevBeforeAttack;
	Timer beforeAttackTimer;
	Timer AttackTimer;
	Model* enemyModel;
	Model* handwrModel;
	void EnemyPop(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed,int popNum = 1);
public:

};

