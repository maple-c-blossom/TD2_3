#pragma once

#include "Object3d.h"
#include "KneadedEraser.h"
#include "Input.h"
#include "ICamera.h"
#include "Sprite.h"
#include "Status.h"
#include "BossDamageEffect.h"
#include "Sound.h"


class Player :public MCB::Object3d
{
private:
	static Player* playerPtr;
	static std::list<MCB::Object3d*> captureList;
	std::list<std::unique_ptr<BossDamageEffect>>effects;
private:
	enum class SoundEffect
	{
		Damage,
		Spin,
	};

	std::array<int, 2> soundEffect;

	MCB::Input* input = MCB::Input::GetInstance();
	int hp = maxHP;
	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE };
	std::vector<int> gamePadConfig = { GAMEPAD_A,GAMEPAD_B,GAMEPAD_X,GAMEPAD_Y };
	Float3* gamePadAxisConfig = { &input->gamePad->LStick };
	std::array<Float2, 8> rotateMoveRecord = {};
	int rotateMoveRecordNum = 0;
	std::list<KneadedEraser> kneadedErasers{};
	int invincible = 0;
	bool visible = true;
	float weight = 1;
	float shard = 30;
	float directionAngle = 0;
	float holdDirectionAngle = 0;
	float moveSpeedPercentage = 0;
	bool prevTrueMakingKneadedEraser = false;
	bool makingKneadedEraserAllow = true;
	bool rotateTapped = false;
	bool rotateMode = false;
	bool rotateCanceled = false;
	float rotateTapTime = 0;
	float rotateModeCount = 0;
	float postRotateCount = 0;
	bool rotateSoundPlay;
	int rotateSound;
	MCB::Vector3D prevPos{};
	MCB::Vector3D velocity{};

	TextureManager* loader = TextureManager::GetInstance();

	std::array<MCB::Sprite, 2> tutorials;
	std::array<MCB::Texture*, 8> tutorialTexs;

	std::array<MCB::Sprite, 2> kneadedEraserGauges;
	std::array<MCB::TextureCell*, 2> kneadedEraserGaugeTexCells;
	std::array<MCB::Texture*, 2> kneadedEraserGaugeTexs;

	MCB::Sprite shardEmpty;
	std::array<MCB::TextureCell*, 2> shardEmptyTexCell;
	std::array<MCB::Texture*, 2> shardEmptyTex;
	int shardEmptyDisplay;

	std::array<MCB::Sprite, maxHP> hearts;
	MCB::Texture* heartTex;
	MCB::TextureCell* heartTexCell;
	MCB::TextureCell* heartBlankTexCell;
	MCB::Texture* heartBlankTex;

	std::array<MCB::Float2, 3> heartShake;
	Timer animationTime;
	int animeNum;
	bool trueMakingKneadedEraser;
public:
	~Player();
	bool deth = false;
public:
	SoundManager* soundmanager;
	Model* sphereModel;
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
	void DethUpdate();
	void Draw();
	void TutorialDraw();
	void StatusDraw();
	void UpdateMatrix(MCB::ICamera* camera);
	float GetShard() { return shard; };
	void SetShard(float num) { shard = num; };
	bool GetMakingKneadedEraserAllow() { return makingKneadedEraserAllow; };
	void SetMakingKneadedEraserAllow(float setAllow) { makingKneadedEraserAllow = setAllow; };
	void Erase();
	void TutorialInitialize(MCB::Texture* tutorial1, MCB::Texture* tutorial2, MCB::Texture* tutorial3,
		MCB::Texture* tutorial4, MCB::Texture* tutorial5, MCB::Texture* tutorial6,
		MCB::Texture* tutorial7, MCB::Texture* tutorial8);

public:
	static Player* GetPlayer() { return playerPtr; };
	static std::list<MCB::Object3d*>* GetCaptureList() { return &captureList; };
};