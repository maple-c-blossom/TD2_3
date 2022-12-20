#include "Substie.h"

using namespace MCB;

void Substie::Initialize()
{
	prevPos = position;
	velocity = Vector3D{ 0,0,0 };
}

void Substie::Update()
{
	velocity = position - prevPos;
	prevPos = position;

	velocity *= 0.8;
	velocity.vec.y /= 0.8;
	velocity.vec.y -= 0.015;
	if (input->IsKeyDown(keyConfig[0]))//input系は基本前にIsが付くよ。
	{
		velocity.vec.z += 0.05;
	}
	if (input->IsKeyDown(keyConfig[1]))
	{
		velocity.vec.z -= 0.05;
	}
	if (input->IsKeyDown(keyConfig[2]))
	{
		velocity.vec.x += 0.05;
	}
	if (input->IsKeyDown(keyConfig[3]))
	{
		velocity.vec.x -= 0.05;
	}

	if (input->IsKeyDown(keyConfig[4]))
	{
		//ねりけしを出す
	}
}