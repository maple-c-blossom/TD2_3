#include "PencilEnemy.h"


void PencilEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model)
{
}

void PencilEnemy::Update()
{

}

void PencilEnemy::Draw()
{
	Object3d::Draw();
	for (auto& itr : handwriting)
	{
		itr->Draw();
	}
}
