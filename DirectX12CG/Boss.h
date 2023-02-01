#pragma once
#include "Object3d.h"
#include "EraserEnemy.h"
#include "PencilEnemy.h"
#include "Util.h"

class Player;

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
	bool imotalFlag = false;
	Timer beforeAttackTimer;
	Timer AttackTimer;
	Timer imotalTimer;//–³“GŽžŠÔ
	Model* enemyModel;
	Model* handwrModel;
	Player* playerPtr;
	void EnemyPop(MCB::Vector3D velocity, MCB::Float3 position,  float speed,int popNum = 1);
public:
	Shake* shake;
	int GetHp() { return hp; };
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, float speed, Player* playerPtr);
	void Update();
	void Draw();
	void UpdateMatrix(MCB::ICamera* camera);
	void Damage(int damage);
};

