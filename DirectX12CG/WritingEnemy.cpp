#include "WritingEnemy.h"
std::vector<std::unique_ptr<Handwriting>> WritingEnemy::handwriting{};
void WritingEnemy::SetHandwritingModel(MCB::Model* model)
{
	handwritingModel = model;
}

std::vector<std::unique_ptr<Handwriting>>* WritingEnemy::GetHandWrite()
{
	return &handwriting;
}

void WritingEnemy::StaticUpdate()
{

	for (int i = 0; i < handwriting.size(); i++)
	{
		if (Object3d::DeleteAllowed(handwriting[i].get()))
		{
			handwriting.erase(handwriting.begin() + i);
			i--;
		}
	}
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
