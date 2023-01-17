#pragma once

#include "Object3d.h"
#include "KneadedEraser.h"
#include "Input.h"
#include "ICamera.h"

class Player :public MCB::Object3d
{
private:
	static Player* playerPtr;
	static std::list<MCB::Object3d*> captureList;
private:
	const float kneadedEraserDistance = 1.0f;
	const float maxMoveSpeed = 0.3;
	const float maxRotateSpeed = 30;

	MCB::Input* input = MCB::Input::GetInstance();
	int hp = 1;
	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE };
	std::list<KneadedEraser> kneadedErasers{};
	int invincible = 0;
	bool visible = true;
	float weight = 1;
	float shard = 0;
	float directionAngle = 0;
	float moveSpeedPercentage = 0;
	bool rotateTapped = false;
	bool rotateMode = false;
	bool rotateCanceled = false;
	float rotateModeCount;
	MCB::Vector3D  prevPos{};
	MCB::Vector3D velocity{};
	
	const float maxShard = 100;

public:
	int GetHp() { return hp; }
	bool GetVisible() { return visible; };
	void Damage(int damage);
	bool IsInvincible();
	std::list<KneadedEraser> GetKneadedErasers();
	void Initialize();
	void Update();
	void Draw();
	void UpdateMatrix(MCB::ICamera* camera);
	float GetShard() { return shard; };
	void Erase();

public:
	static Player* GetPlayer() { return playerPtr; };
	static std::list<MCB::Object3d*>* GetCaptureList() { return &captureList; };
};