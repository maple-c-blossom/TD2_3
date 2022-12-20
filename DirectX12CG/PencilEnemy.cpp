#include "PencilEnemy.h"


using namespace MCB;
using namespace std;

void PencilEnemy::Initialize(MCB::Vector3D velocity, MCB::Float3 position, MCB::Model* model, float speed)
{
	this->velocity = velocity;
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
	this->model = model;
	this->speed = speed;

}

void PencilEnemy::UniqueUpdate()
{
	velocity.V3Norm();
	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;
	Movement += speed;
	if (Movement > WRITING_RADIUS)
	{
		unique_ptr<Handwriting> temp = make_unique<Handwriting>();
		temp->Initialize({ position.x,position.y,position.z }, handwritingModel);
		handwriting.push_back(move(temp));
	}
	for (auto& itr : handwriting)
	{
		itr->Update();
	}

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

	handwriting.remove_if([](auto& itr) {return itr->GetLifeTimeOver(); });

	allEnemyPtr.push_back(this);
}

void PencilEnemy::Draw()
{
	Object3d::Draw();
	for (auto& itr : handwriting)
	{
		itr->Draw();
	}
}

void PencilEnemy::UpdateMatrix(MCB::ICamera* camera)
{
	for (auto& itr : handwriting)
	{
		itr->Object3d::Update(*camera->GetView(),*camera->GetProjection());
	}
	Object3d::Update(*camera->GetView(), *camera->GetProjection());
}



void PencilEnemy::Turn()
{
	velocity *= -1;
}
