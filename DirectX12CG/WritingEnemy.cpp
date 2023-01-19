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
