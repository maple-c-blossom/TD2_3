#pragma once
#include "Enemy.h"

class EraserEnemy :public Enemy
{
public:
	MCB::Model* handwritingModel;
	void UniqueInitialize() override;
	void UniqueUpdate(bool limitMove = true) override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void Turn();//ˆÚ“®•ûŒü”½“]—p
};

