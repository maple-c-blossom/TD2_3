#include "KneadedEraser.h"

std::list<KneadedEraser*> KneadedEraser::allKneadedEraser{};
std::list<KneadedEraser*> KneadedEraser::kneadedErasers{};

void KneadedEraser::UniqueUpdate()
{
	KneadedEraser::allKneadedEraser.push_back(this);
	for (auto& colItr : colliders)
	{
		colItr.Update(this);
	}
}

void KneadedEraser::StaticUpdate()
{
	kneadedErasers = allKneadedEraser;
	allKneadedEraser.clear();
}