#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
#include <list>
#include "ICamera.h"
#include "ADXCollider.h"
#include "Timer.h"
#include "KneadedEraser.h"
#include "Handwriting.h"

class Player;
class Enemy:public MCB::Object3d
{
protected:
	int hp;
	float speed;
	MCB::Vector3D velocity;
	MCB::Object3d attackObj;
	bool attack = false;
	bool beforeAttack = false;
	bool prevBeforeAttack;
	Timer beforeAttackTimer;
	Timer AttackTimer;
	Vector3D captureLocalPos{};
	KneadedEraser* capture = nullptr;
	KneadedEraser* prevCapture = nullptr;
	// 【ADXEngine由来】全てのオブジェクトを入れる配列
	static std::list<Enemy*> allEnemyPtr;
	// 【ADXEngine由来】全てのオブジェクトが入った配列
	static std::list<Enemy*> enemies;

	static std::list<Handwriting*> handwritingPtr;

	static std::list<Handwriting*> allHandwritingPtr;

	void AttackTimerUpdate();
public:
	virtual void UniqueInitialize() = 0;
	virtual void UniqueUpdate() = 0;
	static void StaticUpdate();
	virtual void UpdateMatrix(MCB::ICamera* camera) = 0;
	void AttackStart();//攻撃開始のフラグをtrueにする
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed);
	void Update();
	bool IsAttack();
	//【ADXEngine由来】
	static std::list<Enemy*> GetAllEnemies();
	static void PushBackAllHandWriting(Handwriting* ptr);
	virtual void Draw() = 0;
};

