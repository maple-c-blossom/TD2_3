#include "KneadedEraser.h"

std::list<KneadedEraser*> KneadedEraser::allKneadedEraser{};
std::list<KneadedEraser*> KneadedEraser::kneadedErasers{};

void KneadedEraser::StaticUpdate()
{
	kneadedErasers = allKneadedEraser;
	allKneadedEraser.clear();
}