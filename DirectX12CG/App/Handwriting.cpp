#include "Handwriting.h"
#include "Status.h"
#include "Player.h"
#include "Enemy.h"

void Handwriting::Initialize(MCB::Float3 position, MCB::Model* model)
{
	lifeTimer.Set(MAX_LIFE_TIME_HAND_WRITING);
	this->position_.x = position.x_;
	this->position_.y = position.y_;
	this->position_.z = position.z_;
	this->model_ = model;
	Init();
	color_ = { 0.1f,0.1f,0.1f,1.0f };
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().collideLayer = 2;
}

void Handwriting::Update()
{
	lifeTimer.SafeUpdate();
	color_.w_ = MCB::Lerp(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	if (lifeTimer.IsEnd())
	{
		deleteFlag_ = true;
	}
	UpdateData();
}

void Handwriting::Draw()
{
	Object3d::Draw();
}

bool Handwriting::GetLifeTimeOver()
{
	return lifeTimer.IsEnd();
}

void Handwriting::OnColliderHit(ADXCollider* myCol, ADXCollider* col)
{
	if (col->gameObject == Player::GetPlayer())
	{
		Player::GetPlayer()->Erase();
		this->deleteFlag_ = true;
	}
	else //ã‚Åcontinue‚µ‚Ä‚é‚©‚ç–{—ˆ‚¢‚ç‚È‚¢‚¯‚Çˆê‰ž
	{
		for (auto& itr2 : Player::GetPlayer()->kneadedErasers)//‚Ë‚èÁ‚µ‚É“–‚½‚Á‚Ä‚à•MÕ‚ÍÁ‚·
		{
			if (col->gameObject == itr2.get())
			{
				Player::GetPlayer()->Erase();
				this->deleteFlag_ = true;
			}
		}
	}
}
