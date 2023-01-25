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
	colliders.back().pushable_ = true;
	colliders.back().collideLayer = -1;
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
		input->IsKeyDown(keyConfig[8]);

	bool makingKneadedEraserTrigger = input->IsKeyTrigger(keyConfig[8]);

	bool trueMakingKneadedEraser = makingKneadedEraser && shard > 0 && kneadedErasers.size() <= maxKneadedErasers && !rotateMode;

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

	kneadedErasers.remove_if([](auto& itr) {return !Object3d::IsValid(&itr); });

	if (kneadedErasers.size() <= 0)
	{
		rotateModeCount = 0;
		rotateMode = false;
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
		if (moving && abs(rotateModeCount) < 12.56)
		{
			if (rotateModeCount > 0)
			{
				rotateModeCount += 0.3;
			}
			else
			{
				rotateModeCount -= 0.3;
			}
		}

		shard -= abs(rotateModeCount * 0.001);
		rotation.y += rotateModeCount * 0.03;
	}
	else if(!trueMakingKneadedEraser)
	{
		rotation.y += ADXUtility::AngleDiff(rotation.y, directionAngle) / (kneadedErasers.size() / 10.0 + 1);
	}

	shard = min(max(0, shard), maxShard);

	if (makingKneadedEraserTrigger && kneadedErasers.size() > 0)
	{
		for (auto& itr : kneadedErasers)
		{
			itr.deleteFlag = true;
		}
	}

	if (trueMakingKneadedEraser)
	{
		shard -= velocity.V3Len() * 2;
		if (kneadedErasers.empty()
			|| Vector3D{ kneadedErasers.front().position.x,kneadedErasers.front().position.y,kneadedErasers.front().position.z }.V3Len() > kneadedEraserDistance)
		{
			kneadedErasers.push_front(KneadedEraser{});
			kneadedErasers.front().parent = this;
			kneadedErasers.front().model = KneadedEraserModel;
			kneadedErasers.front().matWorld.matWorld *= matWorld.matWorld;
			kneadedErasers.front().colliders.push_back(ADXCollider(&kneadedErasers.front()));
			kneadedErasers.front().colliders.back().isTrigger = true;
			kneadedErasers.front().colliders.back().collideLayer = 1;
		}

		for (auto& itr : kneadedErasers)
		{
			Vector3D rotatedVel = MCB::MCBMatrix::transform(velocity, MCB::MCBMatrix::MCBMatrixConvertXMMatrix(matWorld.matRot).Inverse());

			itr.position.x -= rotatedVel.ConvertXMFloat3().x;
			itr.position.y -= rotatedVel.ConvertXMFloat3().y;
			itr.position.z -= rotatedVel.ConvertXMFloat3().z;
		}
	}

	captureCount = captureList.size();
	captureList = {};

	velocity.vec = { 0,0,0 };

	{
		bool connectedFlag = true;
		for (auto& itr : kneadedErasers)
		{
			if (connectedFlag)
			{
				itr.UniqueUpdate();
			}
			else
			{
				itr.deleteFlag = true;
			}
			connectedFlag = IsValid(&itr);
		}
	}

	allObjPtr.push_back(this);
	for (auto& colItr : colliders)
	{
		colItr.Update(this);
	}

	invincible--;
	invincible = max(invincible, 0);

	visible = invincible % 8 < 4;
	animationTime.Update();
	if (animationTime.IsEnd())
	{
		animationTime.Set(10);
		animeNum++;
		if (animeNum >= tutorialTexs.size())
		{
			animeNum = 2;
		}
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

void Player::TutorialDraw()
{
	if (shard > 15)
	{
		if (kneadedErasers.size() > 0)
		{
			tutorials[2].SpriteDraw(*tutorialTexs[animeNum], DxWindow::GetInstance()->window_width - 128, 0, 126, 126);//ここで「ctrlGuide2」という名前の画像を表示//ここで「ctrlGuide3」「ctrlGuide4」「ctrlGuide5」「ctrlGuide6」という名前の画像をアニメーションさせて表示
		}
		else
		{
			animationTime.Set(10);
			animeNum = 2;
			tutorials[1].SpriteDraw(*tutorialTexs[1], DxWindow::GetInstance()->window_width - 128, 0,126,126);//ここで「ctrlGuide2」という名前の画像を表示
		}
	}
	else
	{
		tutorials[0].SpriteDraw(*tutorialTexs[0], DxWindow::GetInstance()->window_width - 128, 0,126,126);//ここで「ctrlGuide1」という名前の画像を表示
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

void Player::TutorialInitialize(MCB::Texture* tutorial1, MCB::Texture* tutorial2, MCB::Texture* tutorial3, MCB::Texture* tutorial4, MCB::Texture* tutorial5, MCB::Texture* tutorial6)
{
	tutorialTexs[0] = tutorial1;//ctrlGuide1
	tutorialTexs[1] = tutorial2;//ctrlGuide2
	tutorialTexs[2] = tutorial3;//ctrlGuide3
	tutorialTexs[3] = tutorial4;//ctrlGuide4
	tutorialTexs[4] = tutorial5;//ctrlGuide5
	tutorialTexs[5] = tutorial6;//ctrlGuide6

	for(auto&itr:tutorials)
	{
		itr = itr.CreateSprite();
		itr.anchorPoint = { 0,0 };
	};
}

void Player::Damage(int damage)
{
	if(invincible <= 0)
	{
		hp -= damage;
		invincible = 70;
	}
}