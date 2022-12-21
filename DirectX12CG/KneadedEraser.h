#pragma once

#include <list>
#include "Object3d.h"

class KneadedEraser :public MCB::Object3d
{
public:
	static std::list<KneadedEraser*> setAllKneadedEraser;
	static std::list<KneadedEraser*> kneadedErasers;

public:
	static void StaticUpdate();
};