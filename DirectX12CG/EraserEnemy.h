#pragma once
#include "Enemy.h"

class EraserEnemy :public Enemy
{
public:
	MCB::Model* handwritingModel;
	void UniqueInitialize() override;
	void UniqueUpdate() override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void Turn();//�ړ��������]�p
};

