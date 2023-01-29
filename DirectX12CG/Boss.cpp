#include "Boss.h"
#include "Player.h"
using namespace MCB;
using namespace std;

void Boss::EnemyPop(MCB::Vector3D velocity, MCB::Float3 position, float speed, int popNum)
{
	for (int i = 0; i < popNum; i++)
	{
		unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
		temp->Enemy::Initialize({ 1,0,0 }, { 0,0,0 }, enemyModel, 0.5f);
		temp->SetHandwritingModel(handwrModel);
		enemys.push_back(move(temp));
	}
}

void Boss::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, MCB::Model* star, MCB::Model* ball, float speed, Player* playerPtr)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;
	this->enemyModel = enemyModel;
	this->handwrModel = handwrModel;
	this->playerPtr = playerPtr;
	this->scale = { 4,4,4 };
	starModel = star;
	sphereModel = ball;
	hp = MAX_HP_BOSS;
	ADXCollider tempCol(this);
	colliders.push_back(tempCol);
	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}
	Object3d::Init();

}

void Boss::Update()
{
	for (auto& itr : colliders)
	{
		itr.collideLayer = 3;
	}

	position.y = 0;

	velocity.V3Norm();
	//position.x += velocity.vec.x * speed;
	//position.y += velocity.vec.y * speed;
	//position.z += velocity.vec.z * speed;
	if (position.x < -20 || position.x > 20)
	{
		velocity.vec.x *= -1;
	}

	if (position.y < -20 || position.y > 20)
	{
		velocity.vec.y *= -1;
	}

	if (position.z < -20 || position.z > 20)
	{
		velocity.vec.z *= -1;
	}
	for (auto& itr : enemys)
	{
		itr->Update();
	}
	if (imotalFlag)
	{
		imotalTimer.Update();
		if (imotalTimer.IsEnd())
		{
			imotalFlag = false;
		}
	}
	for (auto& itr : effects)
	{
		itr->Update();
	}
	Damage(1);
	UpdateData();
}

void Boss::Draw()
{
	if (!imotalFlag || imotalTimer.NowTime() % 3 == 0)
	{
		Object3d::Draw();
	}
	for (auto& itr : enemys)
	{
		itr->Draw();
	}
	for (auto& itr : effects)
	{
		itr->Draw();
	}
}

void Boss::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
	for (auto& itr : enemys)
	{
		itr->UpdateMatrix(camera);
	}
	for (auto& itr : effects)
	{
		itr->UpdateMatrix(camera);
	}
}

void Boss::Damage(int damage)
{
	if (playerPtr == nullptr)return;
	for (auto& itr : colliders)
	{
		for (auto& itr2 : playerPtr->GetKneadedErasers())
		{
			for (auto& itr3 : itr2.colliders)
			{
				if (itr.IsHit(itr3) && !imotalFlag)
				{
					int damage = (int)(1.5f * (powf((float)Player::GetCaptureList()->size(),1.6f)) + 1);
					if (!imotalFlag)
					{
						hp -= damage;
						if (Player::GetCaptureList()->size() >= 5)
						{
							for (int i = 0; i < damage; i++)
							{
								unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
								effect->Initialize(starModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
									{ position.x + GetRand(0,200) / 100,position.y + GetRand(0,200) / 100,position.z + GetRand(0,200) / 100 }, { (float)damage / 10 + 1,(float)damage / 10 + 1,(float)damage / 10 + 1 }, { 1,1,0,1 }, 0.75f, 30);
								effects.push_back(std::move(effect));
							}
						}
						else
						{
							for (int i = 0; i < damage + 5; i++)
							{
								unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
								effect->Initialize(sphereModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360)))},
									{ position.x + GetRand(0,200) / 100,position.y + GetRand(0,200) / 100,position.z + GetRand(0,200) / 100 }, { (float)damage / 15 + 1,(float)damage / 15 + 1,(float)damage / 15 + 1 }, { ((float)damage / 20),0,1 - ((float)damage / 20),1 }, 0.75f, 30);
								effects.push_back(std::move(effect));
							}
						}
					}
					for (auto& itr : *Player::GetCaptureList())//すでに練りけしについている敵のデリートフラグをOnに
					{
						itr->deleteFlag = true;
					}
					imotalTimer.Set(60);
					imotalFlag = true;
				}
			}
		}

	}
}
