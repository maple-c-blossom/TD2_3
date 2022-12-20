#pragma once
#include "Enemy.h"
#include "ICamera.h"
class WritingEnemy :public Enemy
{
protected:
	std::list<std::unique_ptr<Handwriting>> handwriting;
	float Movement;
	MCB::Model* handwritingModel;
public:
	virtual void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed) = 0;
	void SetHandwritingModel(MCB::Model* model);
	virtual void UniqueUpdate() = 0;
	virtual void Draw() = 0;
	virtual void UpdateMatrix(MCB::ICamera* camera) = 0;
};

