#include "Handwriting.h"
#include "Status.h"
#include "Player.h"
#include "Enemy.h"

void Handwriting::Initialize(MCB::Float3 position, MCB::Model* model)
{
	lifeTimer.Set(MAX_LIFE_TIME_HAND_WRITING);
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	Init();
	color = { 0.1f,0.1f,0.1f,1.0f };
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().collideLayer = 2;
}

void Handwriting::Update()
{
	lifeTimer.SafeUpdate();
	color.w = MCB::Lerp(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	if (lifeTimer.IsEnd())
	{
		deleteFlag = true;
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

void Handwriting::UniqueOnColliderHit(ADXCollider* myCol, ADXCollider* col)
{
	if (col->gameObject == Player::GetPlayer())
	{
		Player::GetPlayer()->Erase();
		this->deleteFlag = true;
	}
	else //上でcontinueしてるから本来いらないけど一応
	{
		for (auto& itr2 : Player::GetPlayer()->kneadedErasers)//ねり消しに当たっても筆跡は消す
		{
			if (col->gameObject == itr2.get())
			{
				Player::GetPlayer()->Erase();
				this->deleteFlag = true;
			}
		}
	}
}
