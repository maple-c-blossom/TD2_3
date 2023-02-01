#include "Camera.h"
#include "DirectXMath.h"
#include "Util.h"
#include "DxWindow.h"
#include "Player.h"
using namespace MCB;
using namespace DirectX;

void Camera::Inilialize()
{
	view.CreateMatrixView(XMFLOAT3(0.0f, 60.0f, -50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f));
	projection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)DxWindow::GetInstance()->window_width / DxWindow::GetInstance()->window_height, 0.1f, 4000.0f);
}

void Camera::WorldPositionInit()
{
	object3d->Init();
	object3d->position = firstPos;
	object3d->rotation = angle;
	
}

void Camera::Update()
{
	if (player)
	{
		view.eye.x = player->position.x + distance.x;
		view.eye.y = player->position.y + distance.y;
		view.eye.z = player->position.z + distance.z;
		view.target = player->position;
	}

	eyeStartPos = view.eye;
	XMFLOAT3 targetStart = view.target;
	shakeY = shakeX;
	shakeZ = shakeX;
	float offset = shakeX.shakeUpdateR();
	float offsetY = shakeY.shakeUpdateR();
	float offsetZ = shakeZ.shakeUpdateR();
	if (offset != 0)
	{
		Vector3D frontVec(view.eye,view.target);
		frontVec.V3Norm();
		Vector3D upVec = upVec.GetUpVec(frontVec);
		upVec.V3Norm();
		Vector3D RightVec = RightVec.GetRightVec(frontVec,upVec);
		RightVec.V3Norm();
		Float3 finalOffSet = {
			offset * frontVec.vec.x + offset * upVec.vec.x + offset * RightVec.vec.x,
			offsetY * frontVec.vec.y + offsetY * upVec.vec.y + offsetY * RightVec.vec.y,
			offsetZ * frontVec.vec.z + offsetZ * upVec.vec.z + offsetZ * RightVec.vec.z

		};
		view.eye.x = eyeStartPos.x + finalOffSet.x;
		view.eye.y = eyeStartPos.y + finalOffSet.y;
		view.eye.z = eyeStartPos.z + finalOffSet.z;
		view.target.x = targetStart.x + finalOffSet.x;
		view.target.y = targetStart.y + finalOffSet.y;
		view.target.z = targetStart.z + finalOffSet.z;
	}
	view.UpDateMatrixView();
	projection.UpdataMatrixProjection();
	view.eye = eyeStartPos;
	view.target = targetStart;
}

void Camera::WorldPositionUpdate(DirectX::XMMATRIX playerMatrix, DirectX::XMFLOAT3 playerPosition,bool isBillBord)
{
	//object3d->Update(view,projection);


	object3d->matWorld.SetMatScale(object3d->scale.x, object3d->scale.y, object3d->scale.z);
	object3d->matWorld.SetMatRot(object3d->rotation.x, object3d->rotation.y, object3d->rotation.z, false);
	object3d->matWorld.SetMatTrans(object3d->position.x, object3d->position.y, object3d->position.z);

	if (isBillBord)
	{
		if (object3d->parent == nullptr)
		{
			object3d->matWorld.UpdataBillBordMatrixWorld(view);
		}
		else
		{
			object3d->matWorld.UpdataMatrixWorld();
		}
	}
	else
	{
		object3d->matWorld.UpdataMatrixWorld();
	}

	if (object3d->parent != nullptr)
	{
		object3d->matWorld.matWorld *= object3d->parent->matWorld.matWorld;
	}

	object3d->constMapTranceform->world = object3d->matWorld.matWorld * view.mat;
	object3d->constMapTranceform->viewproj = projection.mat;
	object3d->constMapTranceform->cameraPos.x = view.eye.x;
	object3d->constMapTranceform->cameraPos.y = view.eye.y;
	object3d->constMapTranceform->cameraPos.z = view.eye.z;

	/*object3d->constMapTranceform->cameraPos.x = playerPosition.x;
	object3d->constMapTranceform->cameraPos.y = playerPosition.y;
	object3d->constMapTranceform->cameraPos.z = playerPosition.z;*/


}


XMMATRIX Camera::GetMadWorld()
{
	//ワールド座標を入れる変数
	XMMATRIX matWorld;
	matWorld = object3d->matWorld.matWorld;

	return matWorld;
}


//void MCB::Camera::SetPlayerMatrix(DirectX::XMMATRIX playerMatrix)
//{
//	playerMatrix_ = playerMatrix;
//}

////
//WorldMatrix Camera::GetMadWorld2()
//{
//	//ワールド座標を入れる変数
//	WorldMatrix matWorld;
//	matWorld = object3d->matWorld;
//
//	return matWorld;
//}

XMFLOAT3 Camera::Transform(DirectX::XMFLOAT3 forward, WorldMatrix matWorld)
{
	XMFLOAT3 resultVec;

	resultVec.x = (forward.x * matWorld.matWorld.r[0].m128_f32[0]) + (forward.y * matWorld.matWorld.r[1].m128_f32[0]) + (forward.z * matWorld.matWorld.r[2].m128_f32[0]);
	resultVec.y = (forward.x * matWorld.matWorld.r[0].m128_f32[1]) + (forward.y * matWorld.matWorld.r[1].m128_f32[1]) + (forward.z * matWorld.matWorld.r[2].m128_f32[1]);
	resultVec.z = (forward.x * matWorld.matWorld.r[0].m128_f32[2]) + (forward.y * matWorld.matWorld.r[1].m128_f32[2]) + (forward.z * matWorld.matWorld.r[2].m128_f32[2]);

	return resultVec;
}



