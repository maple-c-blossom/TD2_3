#pragma once

#include "Object3d.h"
class Substie :public MCB::Object3d
{
private:
	std::vector<int> keyConfig{};
	float shard = 0;
	MCB::Vector3D  prevPos{};
	MCB::Vector3D velocity{};
	
	const float maxShard = 100;

};