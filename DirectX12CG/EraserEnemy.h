#pragma once
#include "Enemy.h"
class EraserEnemy :public Enemy
{
public:
	void UniqueInitialize() override;
	void UniqueUpdate() override;
	void Draw() override;
	void Turn();//ˆÚ“®•ûŒü”½“]—p
};

