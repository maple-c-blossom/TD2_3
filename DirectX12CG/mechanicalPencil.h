#pragma once
#include "WritingEnemy.h"
#include "PencilBullet.h"
class mechanicalPencil :public WritingEnemy
{
private:
	int trunCount = 0;
	int damage = 1;
	Timer attackTimer;
	std::list < ADXCollider> attackCol;
	std::list<std::unique_ptr<PencilBullet>> bullets;
	MCB::Model* bulletModel;
public:
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed) override;
	void UniqueUpdate() override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void AttackCheck();
	void AttackHit();
	void Turn();//ˆÚ“®•ûŒü”½“]—p
};