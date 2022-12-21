#include "KneadedEraser.h"

std::list<KneadedEraser*> KneadedEraser::setAllKneadedEraser{};
std::list<KneadedEraser*> KneadedEraser::kneadedErasers{};

void KneadedEraser::StaticUpdate()
{
	kneadedErasers = setAllKneadedEraser;
	setAllKneadedEraser.clear();
}