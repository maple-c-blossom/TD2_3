#pragma once

#include "Object3d.h"
#include "KneadedEraser.h"
#include "Input.h"
#include "ICamera.h"
#include "Sprite.h"

class Player :public MCB::Object3d
{
private:
	static Player* playerPtr;
	static std::vector<MCB::Object3d*> captureList;
	static int captureCount;
private:
	const float maxShard = 100;
	const int maxKneadedErasers = 40;
	const float kneadedEraserDistance = 1.0f;
	const float maxMoveSpeed = 0.3;
	const float maxRotateSpeed = 15;

	MCB::Input* input = MCB::Input::GetInstance();
	int hp = 10;
	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE };
	int gamePadConfig = { GAMEPAD_A };
	Float3 gamePadAxisConfig = { input->gamePad->LStick };
	std::vector<KneadedEraser> kneadedErasers{};
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
	std::array<MCB::Sprite, 2> tutorials;//‚±‚ñ‚È‚É—v‚ç‚È‚¢‚©‚à
	std::array<MCB::Texture*, 8> tutorialTexs;

	Timer animationTime;
	int animeNum;
public:
	MCB::Model* KneadedEraserModel = nullptr;
	int GetHp() { return hp; }
	bool GetVisible() { return visible; };
	void Damage(int damage);
	bool IsInvincible();
	std::vector<KneadedEraser> GetKneadedErasers();
	std::vector<KneadedEraser>* GetKneadedErasersPtr();
	void Initialize();
	void Update();
	void Draw();
	void TutorialDraw();
	void UpdateMatrix(MCB::ICamera* camera);
	float GetShard() { return shard; };
	void Erase();
	void TutorialInitialize(MCB::Texture* tutorial1, MCB::Texture* tutorial2, MCB::Texture* tutorial3,
		MCB::Texture* tutorial4, MCB::Texture* tutorial5, MCB::Texture* tutorial6,
		MCB::Texture* tutorial7, MCB::Texture* tutorial8);
public:
	static Player* GetPlayer() { return playerPtr; };
	static std::vector<MCB::Object3d*>* GetCaptureList() { return &captureList; };
};