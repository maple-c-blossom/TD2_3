#include "Enemy.h"
#include "Status.h"
#include "Player.h"
#include "Handwriting.h"
#include "WritingEnemy.h"
std::vector<Enemy*> Enemy::allEnemyPtr{};
std::vector<Enemy*> Enemy::enemies{};


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

void Enemy::IsDamage(int damage)
{
	hp -= damage;
	imotalTimer.Set(10);
	if (hp <= 0)
	{
		deleteFlag = true;
	}
}

void Enemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;
	this->hp = 5;
	Object3d::Init();

	colliders.push_back(this);
	colliders.back().pushable_ = true;
	imotalTimer.Set(1);
	imotalTimer.Update();
	imotalTimer.Update();
	UniqueInitialize();
}

void Enemy::Update()
{
	for(auto& itr : colliders)
	{
		itr.collideLayer = 3;
		itr.isTrigger = false;
	}
	if (!imotalTimer.IsEnd())
	{
		imotalTimer.Update();
	}
	UniqueUpdate();

	for (auto& itr : colliders)
	{
		itr.Update(this);
	}

	if (Player::GetPlayer()->IsInvincible() || (capture != nullptr && !Object3d::IsValid(capture)))
	{
		capture = nullptr;
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
		Player::GetCaptureList()->push_back(this);//プレイヤーがもっているキャプチャしている敵のリストに格納(攻撃当てた時に敵を消すためのリスト）
	}

	position.y = 0;
	
	allEnemyPtr.push_back(this);
	if (hp <= 0)
	{
		deleteFlag = true;
	}
	UpdateData();
}

std::vector<Enemy*> Enemy::GetAllEnemies()
{
	return enemies;
}

void Enemy::UniqueOnColliderHit(ADXCollider* myCol, ADXCollider* col)
{
	if (capture == nullptr && !myCol->isTrigger)
	{
		for (auto& colListItr : KneadedEraser::GetAllKneadedEraser())
		{
			if (col->gameObject == colListItr)
			{
				capture = colListItr;
			}
		}
	}
}


