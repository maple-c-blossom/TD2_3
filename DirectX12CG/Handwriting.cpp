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
	material.Init();
	material.material = model->material.material;
	Init();
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().collideLayer = 2;
}

void Handwriting::Update()
{
	lifeTimer.SafeUpdate();
	material.constMapMaterial->alpha = MCB::Lerp(1, 0, lifeTimer.GetEndTime(), lifeTimer.NowTime());
	for (auto& itr : colliders)
	{
		for (auto& colListItr : itr.collideList)
		{
			if (colListItr->gameObject == Player::GetPlayer())
			{
				Player::GetPlayer()->Erase();
				this->deleteFlag = true;
				continue;
			}
			else //上でcontinueしてるから本来いらないけど一応
			{
				for (auto& itr2 : *Player::GetPlayer()->GetKneadedErasersPtr())//ねり消しに当たっても筆跡は消す
				{
					if(colListItr->gameObject == &itr2)
					{
						Player::GetPlayer()->Erase();
						this->deleteFlag = true;
					}
				}
			}
		}
		itr.Update(this);
	}
	if (lifeTimer.IsEnd())
	{
		deleteFlag = true;
	}
	if (IsValid())
	{
		allObjPtr.push_back(this);
	}
}

void Handwriting::Draw()
{
	Object3d::Draw(&material);
}

bool Handwriting::GetLifeTimeOver()
{
	return lifeTimer.IsEnd();
}
