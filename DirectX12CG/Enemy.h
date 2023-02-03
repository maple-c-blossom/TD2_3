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
	// �yADXEngine�R���z�S�ẴI�u�W�F�N�g������z��
	static std::vector<Enemy*> allEnemyPtr;
	// �yADXEngine�R���z�S�ẴI�u�W�F�N�g���������z��
	static std::vector<Enemy*> enemies;


	void AttackTimerUpdate();
public:
	static enum Type
	{
		Writing,
		Erase
	};

	Timer imotalTimer;
	int hp;
	int enemyType = Writing;
	virtual void UniqueInitialize() = 0;
	virtual void UniqueUpdate(bool limitMove = true) = 0;
	static void StaticUpdate();
	virtual void UpdateMatrix(MCB::ICamera* camera) = 0;
	void AttackStart();//�U���J�n�̃t���O��true�ɂ���
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed);
	void Update(bool limitMove = true);
	bool IsAttack();
	void IsDamage(int damage);
	//�yADXEngine�R���z
	static std::vector<Enemy*> GetAllEnemies();
	virtual void Draw() = 0;
	KneadedEraser* GetcapturePtr() { return capture; }

private:
	void UniqueOnColliderHit(ADXCollider* myCol, ADXCollider* col);
};

