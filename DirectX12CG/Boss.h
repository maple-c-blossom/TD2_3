#pragma once
#include "Object3d.h"
#include "EraserEnemy.h"
#include "PencilEnemy.h"
#include "Util.h"
#include "BossDamageEffect.h"
#include "Sprite.h"

class Player;

class Boss :public MCB::Object3d
{
private:
	std::vector<std::unique_ptr<Enemy>>enemys;
	std::list<std::unique_ptr<BossDamageEffect>>effects;
	int hp;
	float speed;
	MCB::Vector3D velocity;
	bool attack = false;
	bool beforeAttack = false;
	bool prevBeforeAttack;
	bool imotalFlag = false;
	Timer beforeAttackTimer;
	Timer AttackTimer;
	Timer imotalTimer;//���G����
	float moveSpeed = 0.005f;
	float jumpSpeed = 0.025f;
	float downSpeed = -0.025f;
	bool isUp;
	bool isDown;
	Model* enemyModel;
	Model* handwrModel;
	Model* starModel;
	Model* sphereModel;
	Player* playerPtr;

	TextureManager* loader = TextureManager::GetInstance();

	std::array<MCB::Sprite, 2> gauges;
	std::array<MCB::TextureCell*, 2> gaugeTexCells;
	std::array<MCB::Texture*, 2> gaugeTexs;

	void EnemyPop(MCB::Vector3D velocity, MCB::Float3 position,  float speed,int popNum = 1);
public:
	bool IsImotal() { return imotalFlag; }
	Shake* shake;
	int GetHp() { return hp; };
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, MCB::Model* star, MCB::Model* ball, float speed, Player* playerPtr);
	void Update(bool moveLimit = true);
	void Draw();
	void UpdateMatrix(MCB::ICamera* camera);
	void Damage(int damage);
};

