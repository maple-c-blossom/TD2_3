#pragma once
#include "Enemy.h"
class WritingEnemy :public Enemy
{
protected:
	std::vector<std::unique_ptr<Handwriting>> handwriting;
public:
	virtual void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

