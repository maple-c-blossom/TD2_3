#pragma once

#include <list>
#include "Object3d.h"

class KneadedEraser :public MCB::Object3d
{
private:
	static std::vector<KneadedEraser*> allKneadedEraser;
	static std::vector<KneadedEraser*> kneadedErasers;

public:
	void UniqueUpdate();

public:
	static void StaticUpdate();
	static std::vector<KneadedEraser*> GetAllKneadedEraser() { return kneadedErasers; };
};