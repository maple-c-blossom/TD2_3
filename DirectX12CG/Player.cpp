#include "Player.h"
#include "ADXUtility.h"

using namespace MCB;

Player* Player::playerPtr = nullptr;

std::list<KneadedEraser> Player::GetKneadedErasers()
{
	return kneadedErasers;
}

void Player::Initialize()
{
	Init();
	prevPos = position;
	velocity = Vector3D{ 0,0,0 };
	colliders.push_back(ADXCollider(this));
	playerPtr = this;
}

void Player::Update()
{
	bool MoveUp =
		input->IsKeyDown(keyConfig[0]) || input->IsKeyDown(keyConfig[4]);
	bool MoveDown =
		input->IsKeyDown(keyConfig[1]) || input->IsKeyDown(keyConfig[5]);
	bool MoveRight =
		input->IsKeyDown(keyConfig[2]) || input->IsKeyDown(keyConfig[6]);
	bool MoveLeft =
		input->IsKeyDown(keyConfig[3]) || input->IsKeyDown(keyConfig[7]);

	bool moving = MoveUp || MoveDown || MoveRight || MoveLeft;

	bool makingKneadedEraser =
		input->IsKeyDown(keyConfig[8]) && shard > 0;

	velocity = position - prevPos;
	prevPos = position;

	velocity *= 0.8;
	//velocity.vec.y /= 0.8;
	//velocity.vec.y -= 0.015;

	if (moving)
	{
		if (MoveUp)
		{
			velocity.vec.z += 0.05;
		}
		if (MoveDown)
		{
			velocity.vec.z -= 0.05;
		}
		if (MoveRight)
		{
			velocity.vec.x += 0.05;
		}
		if (MoveLeft)
		{
			velocity.vec.x -= 0.05;
		}

		if (!rotateTapped)
		{
			if (MoveUp && MoveDown)
			{
				rotateModeCount += 11;
				rotateTapped = true;
			}

			if (MoveRight && MoveLeft)
			{
				rotateModeCount -= 11;
				rotateTapped = true;
			}
		}

		if (!makingKneadedEraser)
		{
			if (!rotateMode)
			{
				shard += velocity.V3Len() * 0.3;
			}
			float prevDirectionAngle = directionAngle;
			directionAngle = atan2(velocity.vec.x, velocity.vec.z);
			rotateModeCount += ADXUtility::AngleDiff(prevDirectionAngle, directionAngle);
			if(rotateCanceled)
			{ 
				rotateMode = false;
			}
			else if (abs(rotateModeCount) > 12.56)
			{
				rotateMode = true;
			}
		}
	}
	else
	{
		rotateTapped = false;
		rotateCanceled = false;
		rotateModeCount *= 0.9;
		if (abs(rotateModeCount) < 1)
		{
			rotateMode = false;
		}
	}

	position.x += velocity.ConvertXMFloat3().x;
	position.y += velocity.ConvertXMFloat3().y;
	position.z += velocity.ConvertXMFloat3().z;

	if (shard <= 0)
	{
		rotateCanceled = true;
	}

	if (rotateMode)
	{
		shard -= abs(rotateModeCount * 0.001);
		rotation.y += rotateModeCount * 0.03;
	}
	else
	{
		rotation.y += ADXUtility::AngleDiff(rotation.y, directionAngle) / (kneadedErasers.size() / 10.0 + 1);
	}

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

			itr.colliders.push_back(ADXCollider(&itr));
			itr.colliders.back().isTrigger = true;
		}
	}

	for (auto& itr : kneadedErasers)
	{
		allObjPtr.push_back(&itr);
		KneadedEraser::allKneadedEraser.push_back(&itr);
		for (auto& colItr : itr.colliders)
		{
			colItr.Update(&itr);
		}
	}

	allObjPtr.push_back(this);
	for (auto& colItr : colliders)
	{
		colItr.Update(this);
	}
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

void Player::Erase()
{
	shard += 1;
}