#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
#include <list>
#include "Handwriting.h"
#include "ICamera.h"
class Enemy:public MCB::Object3d
{
protected:
	int hp;
	float speed;
	MCB::Vector3D velocity;
	// 【ADXEngine由来】全てのオブジェクトを入れる配列
	static std::list<Enemy*> allEnemyPtr;
	// 【ADXEngine由来】全てのオブジェクトが入った配列
	static std::list<Enemy*> enemies;
public:
	virtual void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model,float speed) = 0;
	virtual void UniqueUpdate() = 0;
	static void StaticUpdate();
	virtual void UpdateMatrix(MCB::ICamera* camera) = 0;
	void Update();
	//【ADXEngine由来】
	static std::list<Enemy*> GetAllEnemies();
	virtual void Draw() = 0;
};

