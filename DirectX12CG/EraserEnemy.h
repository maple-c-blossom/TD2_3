#pragma once
#include "Enemy.h"
class EraserEnemy :public Enemy
{
public:
	void Initialize(MCB::Vector3D velocity,MCB::Float3 position,MCB::Model* model) override;
	void Update() override;
	void Draw() override;
};

