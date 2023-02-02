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
	static std::list<MCB::Object3d*> captureList;
private:
	const float maxShard = 100;
	const int maxKneadedErasers = 40;
	const float kneadedEraserDistance = 1.0f;
	const float maxMoveSpeed = 0.3;
	const float maxRotateSpeed = 15;

	MCB::Input* input = MCB::Input::GetInstance();
	int hp = 10;
	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE };
	std::vector<int> gamePadConfig = { GAMEPAD_A,GAMEPAD_B,GAMEPAD_X,GAMEPAD_Y };
	Float3* gamePadAxisConfig = { &input->gamePad->LStick };
	std::array<Float2, 8> rotateMoveRecord = {};
	int rotateMoveRecordNum = 0;
	std::list<KneadedEraser> kneadedErasers{};
	int invincible = 0;
	bool visible = true;
	float weight = 1;
	float shard = 10;
	float directionAngle = 0;
	float moveSpeedPercentage = 0;
	bool rotateTapped = false;
	bool rotateMode = false;
	bool rotateCanceled = false;
	float rotateTapTime = 0;
	float rotateModeCount = 0;
	float postRotateCount = 0;
	MCB::Vector3D prevPos{};
	MCB::Vector3D velocity{};
	std::array<MCB::Sprite, 2> tutorials;//‚±‚ñ‚È‚É—v‚ç‚È‚¢‚©‚à
	std::array<MCB::Texture*, 8> tutorialTexs;

	TextureManager* loader = TextureManager::GetInstance();

	std::array<MCB::Sprite, 2> kneadedEraserGauges;
	std::array<MCB::TextureCell*, 2> kneadedEraserGaugeTexCells;
	std::array<MCB::Texture*, 2> kneadedEraserGaugeTexs;

	Timer animationTime;
	int animeNum;
	bool trueMakingKneadedEraser;
public:
	float shardCost = 1;
	float shardRotateCost = 0.02f;
	MCB::Model* KneadedEraserModel = nullptr;
	int GetHp() { return hp; }
	MCB::Vector3D GetVelocity() { return velocity; }
	bool GetTrueMake() { return trueMakingKneadedEraser; }
	bool GetRotaMode() { return rotateMode; }
	bool GetVisible() { return visible; };
	void Damage(int damage);
	bool IsInvincible();
	std::list<KneadedEraser> GetKneadedErasers();
	std::list<KneadedEraser>* GetKneadedErasersPtr();
	void Initialize();
	void Update(bool moveLimitFlag = true);
	void Draw();
	void TutorialDraw();
	void UpdateMatrix(MCB::ICamera* camera);
	float GetShard() { return shard; };
	float SetShard(float num) { return shard = num; };
	void Erase();
	void TutorialInitialize(MCB::Texture* tutorial1, MCB::Texture* tutorial2, MCB::Texture* tutorial3,
		MCB::Texture* tutorial4, MCB::Texture* tutorial5, MCB::Texture* tutorial6,
		MCB::Texture* tutorial7, MCB::Texture* tutorial8);
public:
	static Player* GetPlayer() { return playerPtr; };
	static std::list<MCB::Object3d*>* GetCaptureList() { return &captureList; };
};