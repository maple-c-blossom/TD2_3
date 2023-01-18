#include "Player.h"
#include "ADXUtility.h"

using namespace MCB;

Player* Player::playerPtr = nullptr;
std::list<MCB::Object3d*> Player::captureList = {};
int Player::captureCount = 0;
std::list<KneadedEraser> Player::GetKneadedErasers()
{
	return kneadedErasers;
}

std::list<KneadedEraser>* Player::GetKneadedErasersPtr()
{
	return &kneadedErasers;
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

	bool trueMakingKneadedEraser = makingKneadedEraser && kneadedErasers.size() <= maxKneadedErasers;

	velocity = position - prevPos;
	prevPos = position;

	velocity /= 0.8f;

	if (rotateMode)
	{
		weight = (1 + kneadedErasers.size()) * 10;
	}
	else
	{
		weight = 10;
	}

	if (moving)
	{
		moveSpeedPercentage += 1 / weight;

		Vector3D walkVec = { 0,0,0 };

		if (MoveUp)
		{
			walkVec.vec.z += 1;
		}
		if (MoveDown)
		{
			walkVec.vec.z -= 1;
		}
		if (MoveRight)
		{
			walkVec.vec.x += 1;
		}
		if (MoveLeft)
		{
			walkVec.vec.x -= 1;
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

		float prevDirectionAngle = directionAngle;
		directionAngle += ADXUtility::AngleDiff(directionAngle, atan2(walkVec.vec.x, walkVec.vec.z)) / (1 + (weight) / 5);

		if (!trueMakingKneadedEraser)
		{
			if (!rotateMode)
			{
				shard += velocity.V3Len() * 0.01;
			}
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
		moveSpeedPercentage -= 1 / weight;

		rotateTapped = false;
		rotateCanceled = false;
		rotateModeCount *= 0.9;
		if (abs(rotateModeCount) < 1)
		{
			rotateMode = false;
		}
	}

	moveSpeedPercentage = max(0,min(moveSpeedPercentage,1));

	velocity = Vector3D{sin(directionAngle),0,cos(directionAngle)} * moveSpeedPercentage * maxMoveSpeed;

	position.x += velocity.ConvertXMFloat3().x;
	position.y += velocity.ConvertXMFloat3().y;
	position.z += velocity.ConvertXMFloat3().z;

	if (shard <= 0)
	{
		rotateCanceled = true;
	}

	rotateModeCount = max(-maxRotateSpeed, min(rotateModeCount, maxRotateSpeed));

	if (rotateMode)
	{
		shard -= abs(rotateModeCount * 0.001);
		rotation.y += rotateModeCount * 0.03;
	}
	else if(!trueMakingKneadedEraser)
	{
		rotation.y += ADXUtility::AngleDiff(rotation.y, directionAngle) / (kneadedErasers.size() / 10.0 + 1);
	}

	shard = min(max(0, shard), maxShard);

	if (trueMakingKneadedEraser)
	{
		shard -= velocity.V3Len();
		if (kneadedErasers.empty()
			|| Vector3D{ kneadedErasers.back().position.x,kneadedErasers.back().position.y,kneadedErasers.back().position.z }.V3Len() > kneadedEraserDistance)
		{
			kneadedErasers.push_back(KneadedEraser{});
			kneadedErasers.back().parent = this;
			kneadedErasers.back().model = KneadedEraserModel;
			kneadedErasers.back().colliders.push_back(ADXCollider(&kneadedErasers.back()));
			kneadedErasers.back().colliders.back().isTrigger = true;
			kneadedErasers.back().colliders.back().collideLayer = 1;
		}

		for (auto& itr : kneadedErasers)
		{
			Vector3D rotatedVel = MCB::MCBMatrix::transform(velocity, MCB::MCBMatrix::MCBMatrixConvertXMMatrix(matWorld.matRot).Inverse());

			itr.position.x -= rotatedVel.ConvertXMFloat3().x;
			itr.position.y -= rotatedVel.ConvertXMFloat3().y;
			itr.position.z -= rotatedVel.ConvertXMFloat3().z;

		
		}
	}

	if (kneadedErasers.size() > maxKneadedErasers)
	{
		kneadedErasers.erase(kneadedErasers.end());
	}

	velocity.vec = { 0,0,0 };

	for (auto& itr : kneadedErasers)
	{
		itr.UniqueUpdate();
	}

	allObjPtr.push_back(this);
	for (auto& colItr : colliders)
	{
		colItr.Update(this);
	}

	invincible--;
	invincible = max(invincible, 0);

	visible = invincible % 8 < 4;
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

bool Player::IsInvincible()
{
	return invincible > 0;
}

void Player::Erase()
{
	shard += 1;
}

void Player::Damage(int damage)
{
	if(invincible <= 0)
	{
		hp -= damage;
		invincible = 70;
	}
}