#pragma once
#include "Object3d.h"
#include <vector>
#include <memory>
#include "Handwriting.h"
class Enemy:public MCB::Object3d
{
private:
	std::vector<std::unique_ptr<Handwriting>> handwriting;//�ʂ�����
	int hp;

public:
};

