#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
class Enemy:public MCB::Object3d
{
private:
	std::vector<std::unique_ptr<Object3d>> handwriting;//’Ê‚Á‚½Œã
public:
};

