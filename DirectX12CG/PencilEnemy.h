#pragma once
#include "WritingEnemy.h"
#include "Status.h"


class PencilEnemy:public WritingEnemy
{
public:
	std::vector<MCB::Vector3D> movePoint{};

private:
	int movePointIndex = 0;
	float nearestMovePointDistance = 0;
	int notApproachingCount = 0;
	int trunCount = 0;
	int damage = 1;

public:
	void Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed) override;
	void UniqueUpdate() override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void AttackCheck();
	void AttackHit();
	void Turn();//ˆÚ“®•ûŒü”½“]—p
};

