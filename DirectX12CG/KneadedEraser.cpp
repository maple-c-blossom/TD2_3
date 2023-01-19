#include "KneadedEraser.h"

std::list<KneadedEraser*> KneadedEraser::allKneadedEraser{};
std::list<KneadedEraser*> KneadedEraser::kneadedErasers{};

void KneadedEraser::UniqueUpdate()
{
	KneadedEraser::allKneadedEraser.push_back(this);
	allObjPtr.push_back(this);
	for (auto& colItr : colliders)
	{
		colItr.Update(this);
	}

	for (auto& colItr : colliders)
	{
		for (auto& colListItr : colItr.collideList)
		{
			if (!colListItr->pushable_ && !colListItr->isTrigger && colListItr->collideLayer != 3)
			{
				deleteFlag = true;
			}
		}
	}
}

void KneadedEraser::StaticUpdate()
{
	kneadedErasers = allKneadedEraser;
	allKneadedEraser.clear();
}