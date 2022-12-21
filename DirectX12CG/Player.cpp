#include "Player.h"

using namespace MCB;

void Player::Initialize()
{
	prevPos = position;
	velocity = Vector3D{ 0,0,0 };
}

void Player::Update()
{
	velocity = position - prevPos;
	prevPos = position;

	velocity *= 0.8;
	//velocity.vec.y /= 0.8;
	//velocity.vec.y -= 0.015;

	if (input->IsKeyDown(keyConfig[0]) || input->IsKeyDown(keyConfig[1])
		|| input->IsKeyDown(keyConfig[2]) || input->IsKeyDown(keyConfig[3]))
	{
		shard += 0.1;
		if (input->IsKeyDown(keyConfig[0]))
		{
			velocity.vec.z += 0.05;
		}
		if (input->IsKeyDown(keyConfig[1]))
		{
			velocity.vec.z -= 0.05;
		}
		if (input->IsKeyDown(keyConfig[2]))
		{
			velocity.vec.x += 0.05;
		}
		if (input->IsKeyDown(keyConfig[3]))
		{
			velocity.vec.x -= 0.05;
		}
	}

	position.x += velocity.ConvertXMFloat3().x;
	position.y += velocity.ConvertXMFloat3().y;
	position.z += velocity.ConvertXMFloat3().z;

	if (input->IsKeyDown(keyConfig[4]) && shard > 0)
	{
		shard--;
		kneadedErasers.push_back(KneadedEraser{});
		kneadedErasers.back().parent = this;
		kneadedErasers.back().model = model;

		for (auto& itr : kneadedErasers)
		{
			itr.position.x -= velocity.ConvertXMFloat3().x;
			itr.position.y -= velocity.ConvertXMFloat3().y;
			itr.position.z -= velocity.ConvertXMFloat3().z;
		}
	}

	for (auto& itr : kneadedErasers)
	{
		allObjPtr.push_back(&itr);
	}
	allObjPtr.push_back(this);
}

void Player::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
	for (auto& itr : kneadedErasers)
	{
		itr.Object3d::Update(*camera->GetView(), *camera->GetProjection());
	}
}

void Player::Draw()
{
	Object3d::Draw();
	for (auto& itr : kneadedErasers)
	{
		itr.Draw();
	}
}