#pragma once
#include "WritingEnemy.h"
#include "PencilBullet.h"
class mechanicalPencil :public WritingEnemy
{
private:
	int trunCount = 0;
	int damage = 1;
	Timer attackTimer;
	std::vector < ADXCollider> attackCol;
	std::vector<std::unique_ptr<PencilBullet>> bullets;
	MCB::Model* bulletModel;
public:
	void UniqueInitialize() override;
	void UniqueUpdate(bool flag = true) override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void AttackCheck();
	void AttackHit();
	void Turn();//�ړ��������]�p
};