#include "Player.h"
#include "ADXUtility.h"

using namespace MCB;

Player* Player::playerPtr = nullptr;
std::list<MCB::Object3d*> Player::captureList = {};
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

	kneadedEraserGaugeTexCells = { loader->LoadTexture(L"Resources\\gauge\\nerikeshiGauge.png"), loader->LoadTexture(L"Resources\\gauge\\nerikeshiGaugeFill.png") };
	kneadedEraserGaugeTexs = { kneadedEraserGaugeTexCells[0]->texture.get(), kneadedEraserGaugeTexCells[1]->texture.get() };
	for (auto& itr : kneadedEraserGauges)
	{
		itr = itr.CreateSprite();
		itr.anchorPoint = { 0,0 };
	};

	heartTexCell = loader->LoadTexture(L"Resources\\gauge\\playerHp.png");
	heartTex = heartTexCell->texture.get();
	heartBlankTexCell = loader->LoadTexture(L"Resources\\gauge\\playerHpBlank.png");
	heartBlankTex = heartBlankTexCell->texture.get();
	for (auto& itr : hearts)
	{
		itr = itr.CreateSprite();
		itr.anchorPoint = { 0,0 };
	};

	shardEmptyTexCell = { loader->LoadTexture(L"Resources\\gauge\\shardEmpty_0.png"),loader->LoadTexture(L"Resources\\gauge\\shardEmpty_1.png") };
	shardEmptyTex = { shardEmptyTexCell[0]->texture.get(),shardEmptyTexCell[1]->texture.get() };
	shardEmpty = shardEmpty.CreateSprite();
	shardEmpty.anchorPoint = { 0,1 };

	soundEffect[(unsigned int)SoundEffect::Damage] = soundmanager->LoadWaveSound("Resources\\sound\\se\\damage.wav");
	rotateSound = soundmanager->LoadWaveSound("Resources\\sound\\se\\kaiten.wav");

	soundmanager->SetVolume(13, soundEffect[(unsigned int)SoundEffect::Damage]);
}

void Player::Update(bool flag)
{

	bool MoveUp =
		input->IsKeyDown(keyConfig[0]) || input->IsKeyDown(keyConfig[4]);
	bool MoveDown =
		input->IsKeyDown(keyConfig[1]) || input->IsKeyDown(keyConfig[5]);
	bool MoveRight =
		input->IsKeyDown(keyConfig[2]) || input->IsKeyDown(keyConfig[6]);
	bool MoveLeft =
		input->IsKeyDown(keyConfig[3]) || input->IsKeyDown(keyConfig[7]);

	bool moving = MoveUp || MoveDown || MoveRight || MoveLeft || Vector3D(*gamePadAxisConfig).V3Len() > 0.2f;

	bool makingKneadedEraser =
		input->IsKeyDown(keyConfig[8]) || input->gamePad->IsButtonDown(gamePadConfig[0] | gamePadConfig[1] | gamePadConfig[2] | gamePadConfig[3]);

	trueMakingKneadedEraser = makingKneadedEraser && makingKneadedEraserAllow;

	velocity = position - prevPos;
	prevPos = position;

	velocity /= 0.8f;

	shardEmptyDisplay--;
	shardEmptyDisplay = max(shardEmptyDisplay, 0);

	if (rotateMode)
	{
		weight = 300;
		if (!rotateSoundPlay)
		{
			rotateSoundPlay = true;
			soundmanager->PlaySoundWave(rotateSound, true);
		}
	}
	else
	{
		weight = 10;
		rotateSoundPlay = false;
		soundmanager->StopSoundWave(rotateSound);
	}

	if (trueMakingKneadedEraser && !prevTrueMakingKneadedEraser)
	{
		holdDirectionAngle = directionAngle;
	}
	if (!trueMakingKneadedEraser && prevTrueMakingKneadedEraser)
	{
		directionAngle = holdDirectionAngle;
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

		walkVec += {gamePadAxisConfig->x,0,gamePadAxisConfig->y};
		rotateMoveRecord[rotateMoveRecordNum] = { walkVec.vec.x,walkVec.vec.z };
		rotateMoveRecordNum++;
		if(rotateMoveRecordNum >= rotateMoveRecord.size())
		{
			rotateMoveRecordNum = 0;
			if ((rotateMoveRecord[0] == rotateMoveRecord[4]) && (rotateMoveRecord[1] == rotateMoveRecord[5])
				&& (rotateMoveRecord[2] == rotateMoveRecord[6]) && (rotateMoveRecord[3] == rotateMoveRecord[7]))
			{
				if (rotateMoveRecord[0] != rotateMoveRecord[1] && rotateMoveRecord[0] != rotateMoveRecord[2] && rotateMoveRecord[0] != rotateMoveRecord[3] &&
					rotateMoveRecord[1] != rotateMoveRecord[2] && rotateMoveRecord[1] != rotateMoveRecord[3] && rotateMoveRecord[2] != rotateMoveRecord[3])
				{
					rotateModeCount = 12.6f;
				}

			}
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

			if (rotateTapped)
			{
				rotateTapTime = 30;
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
			else if (abs(rotateModeCount) > 12.56f)
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
		if (rotateTapTime <= 0 && rotateMode)
		{
			postRotateCount = rotateModeCount;
			rotateModeCount = 0;

			rotateMode = false;
		}
	}
	postRotateCount *= 0.9;

	rotateTapTime--;
	rotateTapTime = max(0, rotateTapTime);

	kneadedErasers.remove_if([](auto& itr) {return Object3d::DeleteAllowed(&itr); });

	moveSpeedPercentage = max(0, min(moveSpeedPercentage, 1));

	velocity = Vector3D{ sin(directionAngle),0,cos(directionAngle) } *moveSpeedPercentage * maxMoveSpeed;

	if (shard <= 0 && trueMakingKneadedEraser)
	{
		velocity *= 0.1f;
		shardEmptyDisplay = 1;
	}

	position.x += velocity.ConvertXMFloat3().x;
	position.y += velocity.ConvertXMFloat3().y;
	position.z += velocity.ConvertXMFloat3().z;
	//ˆÚ“®§ŒÀ‹æ‰æ--
	//Float2 temp;
	//temp.x = MCB::Lerp(0, 85,(position.z + 30) / 85);
	//temp.x = (position.z + 30) / 85;
	//Float2 Vartical;
	//Vartical.x = MCB::Lerp(-40, -80, temp.x);
	//Vartical.y = MCB::Lerp(40, 80, temp.x);
	if (flag)
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

		if (position.x < -250)
		{
			position.x = -250;
		}
		if (position.x > 190)
		{
			position.x = 190;
		}

		if (position.z < -20)
		{
			position.z = -20;
		}
		if (position.z > 20)
		{
			position.z = 20;
		}
	}

	position.y = 0;
	//--

	if (shard <= 0)
	{
		rotateCanceled = true;
		if (rotateMode)
		{
			shardEmptyDisplay = 60;
		}
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

		shard -= abs(rotateModeCount * 0.01);
		rotation.y += rotateModeCount * 0.03;
	}
	else if (abs(postRotateCount) > 1.0f)
	{
		rotation.y += postRotateCount * 0.03;
	}
	else if (!trueMakingKneadedEraser)
	{
		rotation.y += ADXUtility::AngleDiff(rotation.y, directionAngle) / 2;
	}

	shard = min(max(0, shard), maxShard);

	if (trueMakingKneadedEraser)
	{
		rotateMode = false;
		rotateModeCount = 0;
		postRotateCount = 0;
		shard -= velocity.V3Len() * 1;
		if (shard > velocity.V3Len() * 1)
		{
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
	}

	if (kneadedErasers.size() > maxKneadedErasers)
	{
		int index = 0;
		for (auto& itr : kneadedErasers)
		{
			if (index > maxKneadedErasers)
			{
				itr.deleteFlag = true;
			}
			index++;
		}
	}

	prevTrueMakingKneadedEraser = trueMakingKneadedEraser;

	captureList = {};


	{
		bool connectedFlag = true;
		for (auto& itr : kneadedErasers)
		{
			if (connectedFlag)
			{
				itr.UniqueUpdate();
				connectedFlag = IsValid(&itr);
			}
			else
			{
				itr.deleteFlag = true;
			}
		}
	}

	UpdateData();

	invincible--;
	invincible = max(invincible, 0);

	visible = invincible % 8 < 4;
	animationTime.Update();
	if (animationTime.IsEnd())
	{
		animationTime.Set(10);
		animeNum++;
		if (animeNum >= tutorialTexs.size() || animeNum >= 7)
		{
			animeNum = 3;
		}
	}

	if(invincible > 60)
	{
		int index = 0;
		for (auto& itr : heartShake)
		{
			itr = { sin((float)invincible + index * 2) * (invincible - 60) * 1.3f,cos((float)invincible + index * 2) * (invincible - 60) * 1.3f };
			index++;
		}
	}
	else if (hp <= 3)
	{
		int index = 0;
		for (auto& itr : heartShake)
		{
			itr = { sin((float)clock() / 50 + index * 2) * 1.5f,cos((float)clock() / 50 + index * 2) * 1.5f};
			index++;
		}
	}
	else
	{
		for (auto& itr : heartShake)
		{
			itr = { 0,0 };
		}
	}




}

void Player::DethUpdate()
{
	if (!deth)
	{
		for (int i = 0; i < 40; i++)
		{
			std::unique_ptr<BossDamageEffect> effect = std::make_unique<BossDamageEffect>();
			effect->Initialize(sphereModel, { sinf(ConvertRadius((float)GetRand(0,360))) * cosf(ConvertRadius((float)GetRand(0,360))),sinf(ConvertRadius((float)GetRand(0,360))) * sinf(ConvertRadius((float)GetRand(0,360))),cosf(ConvertRadius((float)GetRand(0,360))) },
				{ position.x + GetRand(0,200) / 100,position.y + GetRand(0,200) / 100,position.z + GetRand(0,200) / 100 }, { (float)25 / 15 + 1,(float)25 / 15 + 1,(float)25 / 15 + 1 }, { ((float)25 / 20),0,1 - ((float)25 / 20),1 }, 0.75f, 60);
			effect->color = { 0.4f,0.3f,0.8f,1.0f };
			effects.push_back(std::move(effect));
		}
		deth = true;
	}
	for (auto& itr : effects)
	{
		itr->Update();
	}
}

void Player::UpdateMatrix(MCB::ICamera* camera)
{

	if (!deth)
	{
		Object3d::Update(*camera->GetView(), *camera->GetProjection());
		for (auto& itr : kneadedErasers)
		{
			itr.Object3d::Update(*camera->GetView(), *camera->GetProjection());
		}
	}
	else
	{
		for (auto& itr : effects)
		{
			itr->UpdateMatrix(camera);
		}
	}
}

void Player::Draw()
{
	if (!deth)
	{
		Object3d::Draw();
		for (auto& itr : kneadedErasers)
		{
			itr.Draw();
		}
	}
	else
	{

		for (auto& itr : effects)
		{
			itr->Draw();
		}
	}
}

void Player::TutorialDraw()
{
	float spriteSize = 126.0f;
	float spriteExtend = 3.0f;
	float edgeSpace = 20;
	float totalSpriteSize = spriteSize * spriteExtend;
	float edgedHalfSpriteSize = totalSpriteSize * 0.625;


	Texture* tutorialTexL = tutorialTexs[0];
	Texture* tutorialTexR = tutorialTexs[2];

	if (shard > 0.5f)
	{
		if (kneadedErasers.size() > 0 && !rotateMode)
		{
			tutorialTexL = tutorialTexs[animeNum];
		}
		else
		{
			animationTime.Set(10);
			animeNum = 3;
		}
	}

	tutorials[0].SpriteDraw(*tutorialTexL, edgeSpace, DxWindow::GetInstance()->window_height - edgedHalfSpriteSize - edgeSpace, totalSpriteSize, totalSpriteSize);
	tutorials[1].SpriteDraw(*tutorialTexR, DxWindow::GetInstance()->window_width - totalSpriteSize - edgeSpace, DxWindow::GetInstance()->window_height - edgedHalfSpriteSize - edgeSpace, totalSpriteSize, totalSpriteSize);
}

void Player::StatusDraw()
{
	float edgeSpace = 20;

	float gaugeSizeX = 114;
	float gaugeSizeY = 496;
	float edgeLength = 3;
	float upperEdgeLength = 16;

	float gaugeRange = gaugeSizeY - edgeLength * 2 - upperEdgeLength;
	float fillAmount = shard / maxShard;
	float gaugeAmount = gaugeRange * fillAmount;

	float heartScale = 38;


	kneadedEraserGauges[0].SpriteDraw(*kneadedEraserGaugeTexs[0], edgeSpace, edgeSpace);
	kneadedEraserGauges[1].size = { gaugeSizeX,gaugeAmount + edgeLength + upperEdgeLength };
	kneadedEraserGauges[1].SpriteCuttingDraw(*kneadedEraserGaugeTexs[1], edgeSpace, edgeSpace + gaugeRange - gaugeAmount, { gaugeSizeX,(gaugeAmount + edgeLength + upperEdgeLength) }, { 0, 0 });

	Texture* nowShardEmptyTex = shardEmptyTex[0];

	if (clock() % 400 > 200)
	{
		nowShardEmptyTex = shardEmptyTex[1];
	}
	if (shardEmptyDisplay > 0)
	{
		shardEmpty.SpriteDraw(*nowShardEmptyTex, edgeSpace + gaugeSizeX, edgeSpace + gaugeSizeY - edgeLength);
	}

	int index = 0;
	for (auto& itr : hearts)
	{
		if (hp <= index)
		{
			itr.SpriteDraw(*heartBlankTex, edgeSpace + heartScale * index + heartShake[index % heartShake.size()].x, gaugeSizeY + edgeSpace * 2 + heartShake[index % heartShake.size()].y);
		}
		index++;
	};

	index = 0;
	for (auto& itr : hearts)
	{
		if (hp > index)
		{
			itr.SpriteDraw(*heartTex, edgeSpace + heartScale * index + heartShake[index % heartShake.size()].x, gaugeSizeY + edgeSpace * 2 + heartShake[index % heartShake.size()].y);
		}
		index++;
	};
}

bool Player::IsInvincible()
{
	return invincible > 0;
}

void Player::Erase()
{
	shard += 0.5;
}

void Player::TutorialInitialize(MCB::Texture* tutorial1, MCB::Texture* tutorial2, MCB::Texture* tutorial3,
	MCB::Texture* tutorial4, MCB::Texture* tutorial5, MCB::Texture* tutorial6,
	MCB::Texture* tutorial7, MCB::Texture* tutorial8)
{
	tutorialTexs[0] = tutorial1;//ctrlGuide1
	tutorialTexs[1] = tutorial2;//ctrlGuide2
	tutorialTexs[2] = tutorial3;//ctrlGuide3
	tutorialTexs[3] = tutorial4;//ctrlGuide4
	tutorialTexs[4] = tutorial5;//ctrlGuide5
	tutorialTexs[5] = tutorial6;//ctrlGuide6
	tutorialTexs[6] = tutorial7;//ctrlGuide7
	tutorialTexs[7] = tutorial8;//ctrlGuide8

	for(auto&itr:tutorials)
	{
		itr = itr.CreateSprite();
		itr.anchorPoint = { 0,0 };
	};
}

Player::~Player()
{
	for (auto& itr : kneadedEraserGaugeTexCells)
	{
		itr->free = true;
	}

	for (auto& itr : shardEmptyTexCell)
	{
		itr->free = true;
	}

	heartTexCell->free = true;
	heartBlankTexCell->free = true;
}

void Player::Damage(int damage)
{
	if(invincible <= 0)
	{
		hp -= damage;
		invincible = 70;
		soundmanager->PlaySoundWave(soundEffect[(unsigned int)SoundEffect::Damage]);
	}
}