#pragma once

#include <list>

class KneadedEraser
{
public:
	static std::list<KneadedEraser*> setAllKneadedEraser;
	static std::list<KneadedEraser*> kneadedErasers;

public:
	static void StaticUpdate();
};