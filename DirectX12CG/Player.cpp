#include "Player.h"
#include "ADXUtility.h"

using namespace MCB;

void Player::Initialize()
{
	prevPos = position;
	velocity = Vector3D{ 0,0,0 };
}

void Player::Update()
{
	bool makingKneadedEraser =
		input->IsKeyDown(keyConfig[4]) && shard > 0;

	velocity = position - prevPos;
	prevPos = position;

	velocity *= 0.8;
	//velocity.vec.y /= 0.8;
	//velocity.vec.y -= 0.015;

	if (input->IsKeyDown(keyConfig[0]) || input->IsKeyDown(keyConfig[1])
		|| input->IsKeyDown(keyConfig[2]) || input->IsKeyDown(keyConfig[3]))
	{
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
		shard += velocity.V3Len() * 0.3;
		if (!makingKneadedEraser)
		{
			directionAngle = atan2(velocity.vec.x, velocity.vec.z);
		}
	}

	position.x += velocity.ConvertXMFloat3().x;
	position.y += velocity.ConvertXMFloat3().y;
	position.z += velocity.ConvertXMFloat3().z;

	rotation.y += ADXUtility::AngleDiff(rotation.y, directionAngle) / (kneadedErasers.size() / 10.0 + 1);

	shard = min(max(0, shard), maxShard);

	if (makingKneadedEraser)
	{
		shard -= velocity.V3Len();
		kneadedErasers.push_back(KneadedEraser{});
		kneadedErasers.back().parent = this;
		kneadedErasers.back().model = model;

		for (auto& itr : kneadedErasers)
		{
			Vector3D rotatedVel = MCB::MCBMatrix::transform(velocity, MCB::MCBMatrix::MCBMatrixConvertXMMatrix(matWorld.matRot).Inverse());

			itr.position.x -= rotatedVel.ConvertXMFloat3().x;
			itr.position.y -= rotatedVel.ConvertXMFloat3().y;
			itr.position.z -= rotatedVel.ConvertXMFloat3().z;
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