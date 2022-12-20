#pragma once
#include "WritingEnemy.h"
#include "Status.h"
class PencilEnemy:public WritingEnemy
{
private:
	int trunCount = 0;
public:
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed) override;
	void UniqueUpdate() override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void Turn();//ˆÚ“®•ûŒü”½“]—p
};

