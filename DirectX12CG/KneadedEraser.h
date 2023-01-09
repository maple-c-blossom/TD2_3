#pragma once

#include <list>
#include "Object3d.h"

class KneadedEraser :public MCB::Object3d
{
private:
	static std::list<KneadedEraser*> allKneadedEraser;
	static std::list<KneadedEraser*> kneadedErasers;

public:
	void UniqueUpdate();

public:
	static void StaticUpdate();
	static std::list<KneadedEraser*> GetAllKneadedEraser() { return kneadedErasers; };
};