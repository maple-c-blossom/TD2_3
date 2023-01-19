#include "WritingEnemy.h"
std::list<std::unique_ptr<Handwriting>> WritingEnemy::handwriting{};
void WritingEnemy::SetHandwritingModel(MCB::Model* model)
{
	handwritingModel = model;
}

std::list<std::unique_ptr<Handwriting>>* WritingEnemy::GetHandWrite()
{
	return &handwriting;
}

void WritingEnemy::StaticUpdate()
{
	for (auto& itr : handwriting)
	{
		itr->Update();
	}
}

void WritingEnemy::StaticDraw()
{
	for (auto& itr : handwriting)
	{
		itr->Draw();
	}
}
