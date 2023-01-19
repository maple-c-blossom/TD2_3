#include "EraserEnemy.h"

void EraserEnemy::UniqueInitialize()
{
	for (auto& itr : attackObj.colliders)
	{
		itr.isTrigger = true;
	}
	Object3d::Init();
}

void EraserEnemy::UniqueUpdate()
{
	velocity.V3Norm();

	position.x += velocity.vec.x * speed;
	position.y += velocity.vec.y * speed;
	position.z += velocity.vec.z * speed;




}

void EraserEnemy::Draw()
{
	Object3d::Draw();
}

void EraserEnemy::Turn()
{
	velocity *= -1;

}
