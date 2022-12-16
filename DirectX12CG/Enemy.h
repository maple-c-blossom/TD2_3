#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
#include <list>
#include "Handwriting.h"
class Enemy:public MCB::Object3d
{
protected:
	int hp;
	float speed;
	MCB::Vector3D velocity;
	// �yADXEngine�R���z�S�ẴI�u�W�F�N�g������z��
	static std::list<Enemy*> allEnemyPtr;
	// �yADXEngine�R���z�S�ẴI�u�W�F�N�g���������z��
	static std::list<Enemy*> enemies;
public:
	virtual void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model) = 0;
	virtual void UniqueUpdate() = 0;
	static void StaticUpdate();
	void Update();
	//�yADXEngine�R���z
	static std::list<Enemy*> GetAllEnemies();
	virtual void Draw() = 0;
};

