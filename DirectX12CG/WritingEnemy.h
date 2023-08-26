#pragma once
#include "Enemy.h"
#include "ICamera.h"
class WritingEnemy :public Enemy
{
protected:
	static std::vector<std::unique_ptr<Handwriting>> handwriting;
	float Movement;
	MCB::Model* handwritingModel;
public:
	virtual void UniqueInitialize() = 0;
	void SetHandwritingModel(MCB::Model* model);
	virtual void UniqueUpdate(bool limitMove = true) = 0;
	virtual void Draw() = 0;
	virtual void UpdateMatrix(MCB::ICamera* camera) = 0;
	static std::vector<std::unique_ptr<Handwriting>>* GetHandWrite();
	static void StaticDraw();
	static void StaticUpdate();
};
