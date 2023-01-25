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
	static int captureCount;
private:
	const float maxShard = 100;
	const int maxKneadedErasers = 40;
	const float kneadedEraserDistance = 1.0f;
	const float maxMoveSpeed = 0.3;
	const float maxRotateSpeed = 30;

	MCB::Input* input = MCB::Input::GetInstance();
	int hp = 10;
	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE };
	int gamePadConfig = { GAMEPAD_A };
	Float3 gamePadAxisConfig = { input->gamePad->LStick };
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
	MCB::Vector3D prevPos{};
	MCB::Vector3D velocity{};

public:
	MCB::Model* KneadedEraserModel = nullptr;
	int GetHp() { return hp; }
	bool GetVisible() { return visible; };
	void Damage(int damage);
	bool IsInvincible();
	std::list<KneadedEraser> GetKneadedErasers();
	std::list<KneadedEraser>* GetKneadedErasersPtr();
	void Initialize();
	void Update();
	void Draw();
	void UpdateMatrix(MCB::ICamera* camera);
	float GetShard() { return shard; };
	void Erase();

public:
	static Player* GetPlayer() { return playerPtr; };
	static std::list<MCB::Object3d*>* GetCaptureList() { return &captureList; };
	static int GetCaptureCount() { return captureCount; };
	static void SetCaptureCount(int num) { captureCount = num; };
};