#pragma once
#include "Object3d.h"
#include "EraserEnemy.h"
#include "PencilEnemy.h"
#include "Util.h"
#include "BossDamageEffect.h"
#include "Sprite.h"
#include "Sound.h"


class Player;

class Boss :public MCB::Object3d
{
private:
	enum class SoundEffect
	{
		FallAttack,
		Damage,

	};
	std::vector<std::unique_ptr<Enemy>>enemys;
	std::list<std::unique_ptr<BossDamageEffect>>effects;
	std::vector<ADXCollider> attackCol;
	int hp;
	std::array<int, 2> soundEffect;

	float speed;
	MCB::Vector3D velocity;
	Quaternion q;
	bool attack = false;
	bool beforeAttack = false;
	bool afterAttack = false;


	bool dethDown = false;
	bool beforedethDown = false;
	bool afterdethDown = false;
	Timer beforedethDownTimer;
	Timer dethDownTimer;

	bool prevBeforeAttack;
	bool imotalFlag = false;
	Timer beforeAttackTimer;
	Timer AttackTimer;
	Timer afterAttackTimer;
	Timer imotalTimer;//���G����
	float moveSpeed = 0.005f;
	float jumpSpeed = 0.025f;
	float downSpeed = -0.025f;
	float gravity = 0.001f;
	bool isUp;
	bool isDown;
	Model* enemyModel;
	Model* handwrModel;
	Model* starModel;
	Model* sphereModel;
	Player* playerPtr;
	float downAngle = 0;
	TextureManager* loader = TextureManager::GetInstance();

	std::array<MCB::Sprite, 2> gauges;
	std::array<MCB::TextureCell*, 2> gaugeTexCells;
	std::array<MCB::Texture*, 2> gaugeTexs;
	std::unique_ptr<Object3d> cover;
	float angle = 25.f;

	void EnemyPop(MCB::Vector3D velocity, MCB::Float3 position,  float speed,int popNum = 1);
public:
	SoundManager* soundmanager;
	bool IsImotal() { return imotalFlag; }
	Shake* shake;
	Quaternion GetQ() { return q; }
	int GetHp() { return hp; };
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, MCB::Model* star, MCB::Model* ball, MCB::Model* cover, float speed, Player* playerPtr);
	void Update(bool moveLimit = true);
	void DethUpdate();
	void Draw();
	void StatusDraw();
	void UpdateMatrix(MCB::ICamera* camera);
	void Damage(int damage);
	void AttackCheck();
	void AttackHit();
	void AttackStart();
	void AttackTimerUpdate();
	void DethTimerUpdate();
};

