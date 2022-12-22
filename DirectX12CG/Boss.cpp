#include "Boss.h"
using namespace MCB;
using namespace std;

void Boss::EnemyPop(MCB::Vector3D velocity, MCB::Float3 position, float speed, int popNum)
{
	for (int i = 0; i < popNum; i++)
	{
		unique_ptr<PencilEnemy> temp = make_unique<PencilEnemy>();
		temp->Initialize({ 1,0,0 }, { 0,0,0 }, enemyModel, 0.5f);
		temp->SetHandwritingModel(handwrModel);
		enemys.push_back(move(temp));
	}
}

void Boss::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, MCB::Model* enemyModel, MCB::Model* handwrModel, float speed, Player* playerPtr)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;
	this->enemyModel = enemyModel;
	this->handwrModel = handwrModel;
	this->playerPtr = playerPtr;
	for (auto& itr : colliders)
	{
		itr.pushable_ = true;
	}
}

void Boss::Update()
{
	velocity.V3Norm();
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;
	if (position.x < -20 || position.x > 20)
	{
		velocity.vec.x *= -1;
	}

	if (position.y < -20 || position.y > 20)
	{
		velocity.vec.y *= -1;
	}

	if (position.z < -20 || position.z > 20)
	{
		velocity.vec.z *= -1;
	}
	for (auto& itr : enemys)
	{
		itr->Update();
	}
	for (auto& itr : colliders)
	{
		itr.Update(this);
	}
}

void Boss::Draw()
{
	Object3d::Draw();
	for (auto& itr : enemys)
	{
		itr->Draw();
	}
}

void Boss::UpdateMatrix(MCB::ICamera* camera)
{
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
	for (auto& itr : enemys)
	{
		itr->UpdateMatrix(camera);
	}
}

void Boss::Damage(int damage)
{
	for (auto& itr : colliders)
	{
		//for (auto& itr2 : playerPtr->colliders)
		//{
		//	hp -= damage;
		//}

	}
}
