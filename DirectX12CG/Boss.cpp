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

void Boss::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, MCB::Model* star, MCB::Model* ball, MCB::Model* cover, float speed, Player* playerPtr)
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
	colliders.back().scale_ = { 1.2f,1.7f,0.7f };
	colliders.back().pos_.y = 0.85f;
	colliders.back().colType_ = box;
	colliders.back().pushBackPriority = 1;
	attackCol.push_back(tempCol);
	attackCol.back().pos_.y = 0.85f;
	attackCol.back().radius_ = 3.f;

	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}
	Object3d::Init();
	this->cover = std::make_unique<Object3d>();
	this->cover->model = cover;
	this->cover->parent = this;
	this->cover->position.x = 1.2f;
	this->cover->position.z = -0.10f;
	q.SetRota({ 0,1,0 }, 0);
	gaugeTexCells = { loader->LoadTexture(L"Resources\\gauge\\bossHpGauge.png"), loader->LoadTexture(L"Resources\\gauge\\bossHpGaugeFill.png") };
	gaugeTexs = { gaugeTexCells[0]->texture.get(), gaugeTexCells[1]->texture.get() };
	for (auto& itr : gauges)
	{
		itr = itr.CreateSprite();
		itr.anchorPoint = { 0,0 };
	};

	soundEffect[0] = soundmanager->LoadWaveSound("Resources\\sound\\se\\bossAttack.wav");
	soundEffect[1] = soundmanager->LoadWaveSound("Resources\\sound\\se\\damage.wav");
	soundmanager->SetVolume(50, soundEffect[0]);
	soundmanager->SetVolume(50, soundEffect[1]);

	this->quaternionPtr = &quaternion;
}

void Boss::Update(bool moveLimit)
{
	for (auto& itr : colliders)
	{
		itr.collideLayer = 3;
	}

	if (moveLimit)
	{
		AttackTimerUpdate();
		AttackCheck();
		AttackHit();
	}

	if (!attack && !beforeAttack && !afterAttack)
	{
		velocity = velocity.V3Get(position, playerPtr->position);
		position.x += velocity.vec.x * moveSpeed;
		position.z += velocity.vec.z * moveSpeed;
		for (auto& itr : colliders)
		{
			itr.pushable_ = true;
		}
	}
	if (attack || beforeAttack || afterAttack)
	{
		for (auto& itr : colliders)
		{
			itr.pushable_ = false;
		}
	}
	Quaternion hogeQ;
	if (!attack && !beforeAttack && !afterAttack)
	{
		q = q.DirToDir({ 0,0,-1 }, Vector3D({ position.x,0,position.z }, playerPtr->position));
		hogeQ = q;
	}
	else if(afterAttack)
	{
		Quaternion tempQ = q.DirToDir({ 0,0,-1 }, Vector3D(velocity.vec, { playerPtr->position.x, playerPtr->position.y, playerPtr->position.z }));
		//q = tempQ.Slerp(hogeQ, tempQ, afterAttackTimer.GetEndTime(), afterAttackTimer.NowTime());
	}

	if (beforeAttack)
	{
		if (angle > 0)
		{
			angle = Lerp(45,0, beforeAttackTimer.GetEndTime(), beforeAttackTimer.NowTime());
			downAngle = Lerp(0, -25, beforeAttackTimer.GetEndTime(), beforeAttackTimer.NowTime());
		}
	}
	else if (attack)
	{
		if (AttackTimer.GetEndTime() - 20 >= AttackTimer.NowTime())
		{
			downAngle = Lerp(-25,90, AttackTimer.GetEndTime() - 20, AttackTimer.NowTime());
		}
		if (AttackTimer.GetEndTime() - 20 == AttackTimer.NowTime())
		{
			shake->Setshake(10, 20, 3);
			soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::FallAttack]);
		}

	}
	else if (afterAttack)
	{
		if (downAngle > 0)
		{
			downAngle = Lerp(90, 0, afterAttackTimer.GetEndTime() - afterAttackTimer.GetEndTime() / 2, afterAttackTimer.NowTime());
		}
		else angle = Lerp(0, 45, afterAttackTimer.GetEndTime() - afterAttackTimer.GetEndTime() / 2, afterAttackTimer.NowTime() - afterAttackTimer.GetEndTime() / 2);
	}
	else
	{
		downAngle = 0;
	}

	if (afterAttack || attack || beforeAttack)
	{
		Quaternion temp;
		Vector3D vec = -(velocity.GetV3Cross({ 0,1,0 }));
		float tempangle = ConvertRadius(downAngle);
		temp.SetRota(vec, tempangle);
		quaternion = quaternion.GetDirectProduct(temp,q);
		quaternion.Normalize();
	}
	else
	{
		quaternion = q;
	}

	this->cover->rotation.y = -ConvertRadius(angle);





	if (!attack && !beforeAttack && !afterAttack)
	{
		if (hp <= MAX_HP_BOSS / 10)
		{
			gravity = 0.001f * 20;
		}
		else
		{
			gravity = 0.001f;
		}
		if (!isUp && !isDown)
		{
			jumpSpeed = 0.25f;
			isUp = true;
		}
		else if (isUp)
		{
			jumpSpeed -= gravity;
			if (jumpSpeed < 0)
			{
				isDown = true;
				isUp = false;
				downSpeed = gravity;
			}
			position.y += jumpSpeed;

			if (position.y > 2.f)
			{
				isUp = false;
				isDown = true;
				downSpeed = gravity;
				position.y = 2.f;
			}

		}
		else if (isDown)
		{
			downSpeed -= gravity;
			if (downSpeed < -0.5f)
			{
				downSpeed = -0.5f;
			}
			position.y += downSpeed;
			if (position.y < 0)
			{
				position.y = 0;
				isDown = false;
			}
		}
	}
	//Float2 temp;
//temp.x = MCB::Lerp(0, 85,(position.z + 30) / 85);
//temp.x = (position.z + 30) / 85;
//Float2 Vartical;
//Vartical.x = MCB::Lerp(-40, -80, temp.x);
//Vartical.y = MCB::Lerp(40, 80, temp.x);
	if (moveLimit)
	{
		if (position.x < -50)
		{
			position.x = -50;
		}
		if (position.x > 50)
		{
			position.x = 50;
		}

		if (position.z < -40)
		{
			position.z = -40;
		}
		if (position.z > 40)
		{
			position.z = 40;
		}

	}
	else
	{
		quaternion = { 0,0,0,1 };
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.y > 2.f)
	{
		position.y = 2.f;
	}

	velocity.V3Norm();

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
	if (beforeAttack)
	{
		if (beforeAttackTimer.NowTime() % 3 == 0)
		{
			color = { 1.f,0.f,0.f,1.f };
		}
		else
		{
			color = { 1.f,1.f,1.f,1.f };
		}
	}
	else if (attack)
	{
		color = { 1.f,0.f,0.f,1.f };
	}
	else
	{
		color = { 1.f,1.f,1.f,1.f };
	}

	cover->color = color;

	if (hp <= 0)
	{
		beforedethDown = true;
		beforedethDownTimer.Set(30);
	}

}

void Boss::DethUpdate()
{

	color = { 1.f,1.f,1.f,1.f };
	cover->color = color;
	DethTimerUpdate();
	if (beforedethDown)
	{
		if (beforedethDownTimer.GetEndTime() - 20 >= beforedethDownTimer.NowTime())
		{
			downAngle = Lerp(0, -25, beforedethDownTimer.GetEndTime() - 20, beforedethDownTimer.NowTime());
		}
	}
	else if (dethDown)
	{
		if (dethDownTimer.GetEndTime() - 20 >= dethDownTimer.NowTime())
		{
			downAngle = Lerp(-25, 90, dethDownTimer.GetEndTime() - 20, dethDownTimer.NowTime());
		}
		if (dethDownTimer.GetEndTime() - 20 == dethDownTimer.NowTime())
		{
			shake->Setshake(10, 20, 3);
			soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::FallAttack]);
		}

	}

	if (dethDown || beforedethDown)
	{
		Quaternion temp;
		Vector3D vec = -(velocity.GetV3Cross({ 0,1,0 }));
		float tempangle = ConvertRadius(downAngle);
		temp.SetRota(vec, tempangle);
		quaternion = quaternion.GetDirectProduct(temp, q);
		quaternion.Normalize();
	}

	for (auto& itr : effects)
	{
		itr->Update();
	}


}

void Boss::Draw()
{
	
	if (!imotalFlag || imotalTimer.NowTime() % 3 == 0)
	{
		if (!afterdethDown)
		{
			Object3d::Draw();
			cover->Draw();
		}
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

void Boss::StatusDraw()
{
	float edgeSpace = 20;

	float gaugeSizeX = 640;
	float gaugeSizeY = 64;
	float edgeLength = 3;
	float upperEdgeLength = 16;

	float gaugeRange = gaugeSizeX - edgeLength * 2 - upperEdgeLength;
	float fillAmount = hp / MAX_HP_BOSS;
	float gaugeAmount = gaugeRange * fillAmount;

	float heartScale = 38;


	gauges[0].SpriteDraw(*gaugeTexs[0], edgeSpace + 240, edgeSpace);
	gauges[1].size = { gaugeAmount + edgeLength + upperEdgeLength ,gaugeSizeY};
	gauges[1].SpriteCuttingDraw(*gaugeTexs[1], edgeSpace + gaugeRange - gaugeAmount + 240, edgeSpace , { (gaugeAmount * gaugeSizeX),gaugeSizeY }, { 0, 0 });

}

void Boss::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
	cover->Update(*camera->GetView(), *camera->GetProjection());
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
						if (shake)shake->Setshake(20, 30, damage + 2 > 5 ? 5 : damage + 2);
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

					itr3.gameObject->deleteFlag = true;



					for (auto& itr : *Player::GetCaptureList())//すでに練りけしについている敵のデリートフラグをOnに
					{
						itr->deleteFlag = true;
					}
					imotalTimer.Set(60);
					imotalFlag = true;
					soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::Damage]);
				}
			}
		}

	}
}

void Boss::AttackCheck()
{
	if (Player::GetPlayer() == nullptr || attack || beforeAttack || afterAttack)return;
	int num = 0;
	for (auto& itr : attackCol)
	{
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				AttackStart();

				//attackObj.position = { position.x + vec.vec.x * 2,position.y + vec.vec.y * 2,position.z + vec.vec.z * 2 };
			}
		}
		num++;
	}
}

void Boss::AttackHit()
{
	if (!attack)return;
	int num = 0;
	for (auto& itr : colliders)
	{
		for (auto& itr2 : Player::GetPlayer()->colliders)
		{
			if (itr.IsHit(itr2))
			{
				Player::GetPlayer()->Damage(2);
			}
		}
		num++;
	}
}


void Boss::AttackStart()
{
	if (!beforeAttack &&!attack && !afterAttack)
	{
		beforeAttackTimer.Set(ENEMY_BEFORE_ATTACK_TIME);
	}
	beforeAttack = true;
}

void Boss::AttackTimerUpdate()
{
	if (beforeAttack)
	{
		beforeAttackTimer.SafeUpdate();
		if (beforeAttackTimer.IsEnd())
		{
			beforeAttack = false;
			attack = true;
			AttackTimer.Set(ENEMY_ATTACK_TIME / 2);
		}
	}
	else if (attack)
	{
		AttackTimer.SafeUpdate();
		if (AttackTimer.IsEnd())
		{
			attack = false;
			afterAttack = true;
			afterAttackTimer.Set(ENEMY_BEFORE_ATTACK_TIME * 2);
		}
	}
	else if (afterAttack)
	{
		afterAttackTimer.SafeUpdate();
		if (afterAttackTimer.IsEnd())
		{
			afterAttack = false;
		}
	}
}


void Boss::DethTimerUpdate()
{
	if (beforedethDown)
	{
		beforedethDownTimer.SafeUpdate();
		if (beforedethDownTimer.IsEnd())
		{
			beforedethDown = false;
			dethDown = true;
			dethDownTimer.Set(ENEMY_ATTACK_TIME);
		}
	}
	else if (dethDown)
	{
		dethDownTimer.SafeUpdate();
		if (dethDownTimer.IsEnd())
		{
			dethDown = false;
			afterdethDown = true;
			for (int i = 0; i < 30; i++)
			{
				unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
				effect->Initialize(sphereModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
					{ position.x + GetRand(0,200) / 100,position.y + GetRand(0,200) / 100,position.z + GetRand(0,200) / 100 }, { (float)25 / 15 + 1,(float)25 / 15 + 1,(float)25 / 15 + 1 }, { ((float)25 / 20),0,1 - ((float)25 / 20),1 }, 0.75f, 30);
				effect->color = { 0.76078431f,0.30588235,0.51372549f,1.0f };
				effects.push_back(std::move(effect));
			}
		}
	}
}