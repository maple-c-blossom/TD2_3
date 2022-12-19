#pragma once

#include "Object3d.h"
#include "Input.h"

class Substie :public MCB::Object3d
{
private:
	Input* input = Input::GetInstance();

	std::vector<int> keyConfig{ DIK_W,DIK_S,DIK_D,DIK_A,DIK_SPACE };
	float shard = 0;
	MCB::Vector3D  prevPos{};
	MCB::Vector3D velocity{};
	
	const float maxShard = 100;

public:
	void Initialize();
	void Update();
};