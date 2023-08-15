#include "KneadedEraser.h"

std::vector<KneadedEraser*> KneadedEraser::allKneadedEraser{};
std::vector<KneadedEraser*> KneadedEraser::kneadedErasers{};

void KneadedEraser::UniqueUpdate()
{
	KneadedEraser::allKneadedEraser.push_back(this);

	UpdateData();

	for (auto& colItr : colliders)
	{
		for (auto& colListItr : colItr.GetCollideList())
		{
			if (!colListItr->pushable_ && !colListItr->isTrigger && colListItr->collideLayer != 3)
			{
				deleteFlag_ = true;
			}
		}
	}
}

void KneadedEraser::StaticUpdate()
{
	kneadedErasers = allKneadedEraser;
	allKneadedEraser.clear();
}