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

void Boss::GaugeUpdate()
{
	if (imotalFlag && imotalTimer.NowTime() < 15)
	{
		gaugeShake = { 0,cos((float)imotalTimer.NowTime()) * (15 - imotalTimer.NowTime()) * 2.0f };
	}
	else if (damageAmount > hp)
	{
		gaugeShake = { 0,cos((float)clock()) * 2.0f };
	}
	else
	{
		gaugeShake = { 0,0 };
	}

	if (imotalTimer.NowTime() >= 30)
	{
		damageAmount -= MAX_HP_BOSS * 0.002f;
	}

	damageAmount = max(hp, damageAmount);
}

void Boss::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, MCB::Model* star, MCB::Model* ball, MCB::Model* cover, float speed, Player* playerPtr)
{
	this->velocity = velocity;
	this->position_.x = position.x_;
	this->position_.y = position.y_;
	this->position_.z = position.z_;
	this->model_ = model;;
	this->speed = speed;
	this->enemyModel = enemyModel;
	this->handwrModel = handwrModel;
	this->playerPtr = playerPtr;
	this->scale_ = { 4,4,4 };
	starModel = star;
	sphereModel = ball;
	hp = MAX_HP_BOSS;
	ADXCollider tempCol(this);
	colliders.push_back(tempCol);
	colliders.back().scale_ = { 1.2f,2.125f,0.7f };
	colliders.back().pos_.y = 1.275f;
	colliders.back().pos_.z = 0.5f;
	colliders.back().colType_ = box;
	colliders.back().pushBackPriority = 1;
	attackCol.push_back(tempCol);
	attackCol.back().pos_.y = 0.85f;
	attackCol.back().radius_ = 3.f;
	attackCol.back().isTrigger = true;

	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}
	Object3d::Init();
	this->cover = std::make_unique<Object3d>();
	this->cover->model_ = cover;
	this->cover->parent_ = this;
	this->cover->position_.x = 1.2f;
	this->cover->position_.z = -0.10f;
	q.SetRota({ 0,1,0 }, 0);
	gaugeTexCells = { loader->LoadTexture(L"Resources\\gauge\\bossHpGauge.png"), loader->LoadTexture(L"Resources\\gauge\\bossHpGaugeFill.png"),loader->LoadTexture(L"Resources\\gauge\\bossHpGaugeDecrease.png") };
	gaugeTexs = { gaugeTexCells[0]->texture.get(), gaugeTexCells[1]->texture.get(), gaugeTexCells[2]->texture.get() };
	for (auto& itr : gauges)
	{
		itr.CreateSprite();
		itr.anchorPoint_ = { 0,0 };
	};

	soundEffect[(unsigned int)SoundEffect::FallAttack] = soundmanager->LoadWaveSound("Resources\\sound\\se\\bossAttack.wav");
	soundEffect[(unsigned int)SoundEffect::Hit] = soundmanager->LoadWaveSound("Resources\\sound\\se\\slam.wav");
	soundEffect[(unsigned int)SoundEffect::HeavyHit] = soundmanager->LoadWaveSound("Resources\\sound\\se\\slap.wav");
	dieSound = soundmanager->LoadWaveSound("Resources\\sound\\se\\pop.wav");
	soundmanager->SetVolume(50, soundEffect[(unsigned int)SoundEffect::FallAttack]);
	soundmanager->SetVolume(6, soundEffect[(unsigned int)SoundEffect::Hit]);
	soundmanager->SetVolume(6, soundEffect[(unsigned int)SoundEffect::HeavyHit]);
	soundmanager->SetVolume(50, dieSound);

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
		velocity = velocity.V3Get(position_, playerPtr->position_);
		nowFrontVec_ = velocity;
		velocity.V3Norm();
		position_.x += velocity.vec_.x_ * moveSpeed;
		position_.z += velocity.vec_.z_ * moveSpeed;
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
		q = q.DirToDir({ 0,0,-1 }, Vector3D({ position_.x,0,position_.z }, playerPtr->position_));
		hogeQ = q;
	}
	else if(afterAttack)
	{
		//Quaternion tempQ = q.DirToDir({ 0,0,-1 }, Vector3D(velocity.vec_, { playerPtr->position_.x, playerPtr->position_.y, playerPtr->position_.z }));
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
			soundmanager->SetVolume(60,soundEffect[(unsigned int)SoundEffect::FallAttack]);
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

	this->cover->rotation_.y = -ConvertRadius(angle);





	if (!attack && !beforeAttack && !afterAttack)
	{
		if (hp <= MAX_HP_BOSS / 4)
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
			position_.y += jumpSpeed;

			if (position_.y > 2.f)
			{
				isUp = false;
				isDown = true;
				downSpeed = gravity;
				//position_.y = 2.f;
			}

		}
		else if (isDown)
		{
			downSpeed -= gravity;
			if (downSpeed < -0.5f)
			{
				downSpeed = -0.5f;
			}
			position_.y += downSpeed;
			if (position_.y < 0)
			{
				//position_.y = 0;
				isDown = false;
			}
		}
	}
	//Float2 temp;
//temp.x = MCB::Lerp(0, 85,(position_.z + 30) / 85);
//temp.x = (position_.z + 30) / 85;
//Float2 Vartical;
//Vartical.x = MCB::Lerp(-40, -80, temp.x);
//Vartical.y = MCB::Lerp(40, 80, temp.x);
	if (moveLimit)
	{
		if (position_.x < -50)
		{
			position_.x = -50;
		}
		if (position_.x > 50)
		{
			position_.x = 50;
		}

		if (position_.z < -40)
		{
			position_.z = -40;
		}
		if (position_.z > 40)
		{
			position_.z = 40;
		}

	}
	else
	{
		quaternion = { 0,0,0,1 };
	}
	if (position_.y < 0)
	{
		//position_.y = 0;
	}
	if (position_.y > 2.f)
	{
		//position_.y = 2.f;
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
		if (beforeAttackTimer.NowTime() % 5 == 0)
		{
			color_ = { 1.f,0.f,0.f,1.f };
		}
		else
		{
			color_ = { 1.f,1.f,1.f,1.f };
		}
	}
	else if (attack)
	{
		color_ = { 1.f,0.f,0.f,1.f };
	}
	else
	{
		color_ = { 1.f,1.f,1.f,1.f };
	}

	if (!imotalFlag)
	{
		color_.w_ = 1.f;
	}
	else
	{
		color_.w_ = 0.15f;
	}
	cover->color_ = color_;

	if (hp <= 0)
	{
		hp = 0;

		beforedethDown = true;
		beforedeathDownTimer.Set(30);
	}

	if (heavyHitNum > 0)
	{
		heavyHitInterval--;
		if (heavyHitInterval <= 0)
		{
			soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::HeavyHit]);
			soundmanager->SetVolume(10,soundEffect[(unsigned int)SoundEffect::HeavyHit]);
			heavyHitNum--;
			heavyHitInterval = 5;
		}
	}

	GaugeUpdate();
}

void Boss::DethUpdate()
{

	color_ = { 1.f,1.f,1.f,1.f };
	cover->color_ = color_;
	DethTimerUpdate();
	if (beforedethDown)
	{
		if (beforedeathDownTimer.GetEndTime() - 20 >= beforedeathDownTimer.NowTime())
		{
			downAngle = Lerp(0, -25, beforedeathDownTimer.GetEndTime() - 20, beforedeathDownTimer.NowTime());
		}
	}
	else if (dethDown)
	{
		if (deathDownTimer.GetEndTime() - 20 >= deathDownTimer.NowTime())
		{
			downAngle = InQuad(-25, 90, deathDownTimer.GetEndTime() - 20, deathDownTimer.NowTime());
		}
		if (deathDownTimer.GetEndTime() - 20 == deathDownTimer.NowTime())
		{
			shake->Setshake(10, 20, 3);
			soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::FallAttack]);
			soundmanager->SetVolume(90,soundEffect[(unsigned int)SoundEffect::FallAttack]);
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

	GaugeUpdate();
}

void Boss::Draw()
{
	

	if (!afterdethDown)
	{
		Object3d::Draw();
		cover->Draw();
	}
	if (!afterdethDown)
	{
		for (auto& itr : enemys)
		{
			itr->Draw();
		}
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
	float edgeLength = 8;


	float gaugeRange = gaugeSizeX - edgeLength * 2;
	float fillAmount = (float)hp / MAX_HP_BOSS;
	float gaugeAmount = gaugeRange * fillAmount;

	float damageFillAmount = damageAmount / MAX_HP_BOSS;
	float damageGaugeAmount = gaugeRange * damageFillAmount;


	gauges[0].SpriteDraw(*gaugeTexs[0], DxWindow::GetInstance()->sWINDOW_WIDTH_ - gaugeSizeX - edgeSpace, edgeSpace + gaugeShake.y_);
	gauges[1].size_ = { gaugeAmount + edgeLength ,gaugeSizeY };
	gauges[2].size_ = { damageGaugeAmount + edgeLength ,gaugeSizeY };
	gauges[2].SpriteCuttingDraw(*gaugeTexs[2], DxWindow::GetInstance()->sWINDOW_WIDTH_ - gaugeSizeX - edgeSpace + gaugeRange - damageGaugeAmount, edgeSpace + gaugeShake.y_, { (damageGaugeAmount + edgeLength),gaugeSizeY }, { 0, 0 });
	gauges[1].SpriteCuttingDraw(*gaugeTexs[1], DxWindow::GetInstance()->sWINDOW_WIDTH_ - gaugeSizeX - edgeSpace + gaugeRange - gaugeAmount, edgeSpace + gaugeShake.y_, { (gaugeAmount + edgeLength),gaugeSizeY }, { 0, 0 });

}

void Boss::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(quaternion);
	cover->Update();
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
	bool isHit = false;
	for (auto& itr : colliders)
	{
		for (auto& itr2 : playerPtr->kneadedErasers)
		{
			for (auto& itr3 : itr2->colliders)
			{
				if (itr.IsHit(itr3) && !imotalFlag)
				{
					int damage = (int)(1.5f * (powf((float)Player::GetCaptureList()->size(),1.6f)) + 1);
					if (!imotalFlag)
					{
						hp -= damage;
						isHit = true;
						if (shake)shake->Setshake(20, 30, damage + 2 > 5 ? 5 : damage + 2);
						if (Player::GetCaptureList()->size() >= 5)
						{
							for (int i = 0; i < damage; i++)
							{
								unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
								effect->Initialize(starModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
									{ position_.x + GetRand(0,200) / 100,position_.y + GetRand(0,200) / 100,position_.z + GetRand(0,200) / 100 }, { (float)damage / 10 + 1,(float)damage / 10 + 1,(float)damage / 10 + 1 }, { 1,1,0,1 }, 0.75f, 30);
								effects.push_back(std::move(effect));
							}
						}
						else
						{
							for (int i = 0; i < damage + 5; i++)
							{
								unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
								effect->Initialize(sphereModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360)))},
									{ position_.x + GetRand(0,200) / 100,position_.y + GetRand(0,200) / 100,position_.z + GetRand(0,200) / 100 }, { (float)damage / 15 + 1,(float)damage / 15 + 1,(float)damage / 15 + 1 }, { ((float)damage / 20),0,1 - ((float)damage / 20),1 }, 0.75f, 30);
								effects.push_back(std::move(effect));
							}
						}
					}

					itr3.gameObject->deleteFlag_ = true;

					heavyHitNum = 0;

					for (auto& itr : *Player::GetCaptureList())//すでに練りけしについている敵のデリートフラグをOnに
					{
						itr->deleteFlag_ = true;
						heavyHitNum++;
					}
					imotalTimer.Set(60);
					imotalFlag = true;

					heavyHitNum = min(heavyHitNum, 6);
				}
			}
		}

	}
		if(isHit) soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::Hit]);
		if(isHit) soundmanager->SetVolume(10,soundEffect[(unsigned int)SoundEffect::Hit]);
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

				//attackObj.position_ = { position_.x + vec.vec_.x_ * 2,position_.y + vec.vec_.y_ * 2,position_.z + vec.vec_.z_ * 2 };
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
		beforedeathDownTimer.SafeUpdate();
		if (beforedeathDownTimer.IsEnd())
		{
			beforedethDown = false;
			dethDown = true;
			deathDownTimer.Set(ENEMY_ATTACK_TIME);
		}
	}
	else if (dethDown)
	{
		deathDownTimer.SafeUpdate();
		if (deathDownTimer.IsEnd())
		{
			dethDown = false;
			afterdethDown = true;
			for (int i = 0; i < 40; i++)
			{
				unique_ptr<BossDamageEffect> effect = make_unique<BossDamageEffect>();
				effect->Initialize(starModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
					{ position_.x + GetRand(0,200) / 100,position_.y + GetRand(0,200) / 100,position_.z + GetRand(0,200) / 100 }, { (float)25 / 15 + 1,(float)25 / 15 + 1,(float)25 / 15 + 1 }, { ((float)25 / 20),0,1 - ((float)25 / 20),1 }, 0.75f, 60);
				effect->color_ = { 0.54901961f,0.15294118,0.51764706f,1.0f };
				effects.push_back(std::move(effect));
			}
			soundmanager->PlaySoundWave(dieSound);
			soundmanager->SetVolume(80,dieSound);
		}
	}
}

Boss::~Boss()
{
	for (auto& itr : gaugeTexCells)
	{
		itr->free = true;
	}
}
