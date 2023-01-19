#include "Enemy.h"
#include "Status.h"
#include "Player.h"

std::list<Enemy*> Enemy::allEnemyPtr{};
std::list<Enemy*> Enemy::enemies{};
std::list<Handwriting*> Enemy::handwriting{};
std::list<Handwriting*> Enemy::allHandwritingPtr{};

using namespace MCB;

void Enemy::StaticUpdate()
{
	enemies = allEnemyPtr;
	allEnemyPtr.clear();

	handwriting = allHandwritingPtr;
	allHandwritingPtr.clear();
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

void Enemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;

	colliders.push_back(this);
	colliders.back().pushable_ = true;

	UniqueInitialize();
}

void Enemy::Update()
{
	for(auto& itr : colliders)
	{
		itr.collideLayer = 3;
	}

	UniqueUpdate();

	if (capture == nullptr)
	{
		for (auto& itr : colliders)
		{
			if (!itr.isTrigger)
			{
				for (auto& colListItr : itr.collideList)
				{
					for (auto& colListItr2 : KneadedEraser::GetAllKneadedEraser())
					{
						if (colListItr->gameObject == colListItr2)
						{
							if (capture == nullptr)Player::GetCaptureList()->push_back(this);//プレイヤーがもっているキャプチャしている敵のリストに格納(攻撃当てた時に敵を消すためのリスト）
							Player::SetCaptureCount(Player::GetCaptureCount() + 1);
							capture = colListItr2;
						}
					}
				}
			}
			itr.Update(this);
		}
	}

	if (Player::GetPlayer()->IsInvincible())
	{
		capture = nullptr;
		Player::SetCaptureCount(0);
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

void Enemy::PushBackAllHandWriting(Handwriting* ptr)
{
	allHandwritingPtr.push_back(ptr);
}
