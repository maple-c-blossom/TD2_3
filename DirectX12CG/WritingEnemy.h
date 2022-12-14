#pragma once
#include "Enemy.h"
class WritingEnemy :public Enemy
{
protected:
	std::vector<std::unique_ptr<Handwriting>> handwriting;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

