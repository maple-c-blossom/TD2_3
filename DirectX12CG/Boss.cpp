#include "Boss.h"
using namespace MCB;
using namespace std;

void Boss::EnemyPop(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed, int popNum)
{
	for (int i = 0; i < popNum; i++)
	{
		unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
		temp->Initialize({ 1,0,0 }, { 0,0,0 }, enemyModel, 0.5f);
		temp->SetHandwritingModel(handwrModel);
		enemys.push_back(move(temp));
	}
}

void Boss::Update()
{

	for (auto& itr : colliders)
	{
		itr.Update(this);
	}
}
