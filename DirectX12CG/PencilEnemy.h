#pragma once
#include "WritingEnemy.h"
#include "Status.h"


class PencilEnemy:public WritingEnemy
{
public:
	std::vector<MCB::Vector3D> movePoint{};
	std::vector<ADXCollider> attackCol;

private:
	int movePointIndex = 0;
	float nearestMovePointDistance = 0;
	int notApproachingCount = 0;
	int trunCount = 0;
	int damage = 1;
	MCB::Vector3D attackVec;
	int writingCount;
public:
	void UniqueInitialize() override;
	void UniqueUpdate(bool limitMove = true) override;
	void Draw() override;
	void UpdateMatrix(MCB::ICamera* camera) override;
	void AttackCheck();
	void AttackHit();
};

