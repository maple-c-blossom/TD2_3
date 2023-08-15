#include "Camera.h"
#include "DirectXMath.h"
#include "Util.h"
#include "DxWindow.h"
#include "Player.h"
#include "Boss.h"
using namespace MCB;
using namespace DirectX;

void Camera::Inilialize()
{
	view_.CreateMatrixView(XMFLOAT3(0.0f, 60.0f, -50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f));
	projection_.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)DxWindow::GetInstance()->sWINDOW_WIDTH_ / DxWindow::GetInstance()->sWINDOW_HEIGHT_, 0.1f, 4000.0f);
}

void Camera::WorldPositionInit()
{
	object3d_->Init();
	object3d_->position_ = firstPos;
	object3d_->rotation_ = angle;

}

void Camera::Update()
{
	XMFLOAT3 targetStart = view_.target_;
	if (player && boss)
	{
		if ((!player->deth) && !(boss->afterdethDown) && !(boss->beforedethDown) && !(boss->dethDown))
		{
			view_.eye_.x = player->position_.x + distance.x_;
			view_.eye_.y = player->position_.y + distance.y_;
			view_.eye_.z = player->position_.z + distance.z_;
			eyeEaseStartPos = view_.eye_;
			view_.target_ = player->position_;
			targetEaseStartPos = view_.target_;
			timer.Set(300);
			isOktimer.Set(30);
		}
		else if (boss->afterdethDown)
		{
			timer.SafeUpdate();
			int interval = 150;
			if (timer.NowTime() >= interval)
			{
				view_.eye_.x = InOutQuad(eyeEaseStartPos.x, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.eye_.y = InOutQuad(eyeEaseStartPos.y, 75, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.eye_.z = InOutQuad(eyeEaseStartPos.z, -10, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.x = InOutQuad(targetEaseStartPos.x, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.y = InOutQuad(targetEaseStartPos.y, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.z = InOutQuad(targetEaseStartPos.z, -10, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.up_.z = InOutQuad(0, 1, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.up_.y = InOutQuad(1, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);

			}

			if (isOktimer.IsEnd() && timer.IsEnd())
			{
				isok = true;
			}
			else if (timer.IsEnd())
			{
				isOktimer.Update();
			}

		}
		else if (player->deth)
		{
			timer.SafeUpdate();
			int interval = 150;
			if (timer.NowTime() >= interval)
			{
				view_.eye_.x = InOutQuad(eyeEaseStartPos.x, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.eye_.y = InOutQuad(eyeEaseStartPos.y, 75, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.eye_.z = InOutQuad(eyeEaseStartPos.z, -10, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.x = InOutQuad(targetEaseStartPos.x, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.y = InOutQuad(targetEaseStartPos.y, 0, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.target_.z = InOutQuad(targetEaseStartPos.z, -10, timer.GetEndTime() - interval, timer.NowTime() - interval);
				view_.up_.z = InOutQuad(0, 1, timer.GetEndTime(), timer.NowTime() - interval);
				view_.up_.y = InOutQuad(1, 0, timer.GetEndTime(), timer.NowTime() - interval);
			}
			if (isOktimer.IsEnd() && timer.IsEnd())
			{
				isok = true;
			}
			else if (timer.IsEnd())
			{
				isOktimer.Update();
			}


		}


		eyeStartPos = view_.eye_;
		targetStart = view_.target_;
		shakeY = shakeX;
		shakeZ = shakeX;
		float offset = shakeX.shakeUpdateR();
		float offsetY = shakeY.shakeUpdateR();
		float offsetZ = shakeZ.shakeUpdateR();
		if (offset != 0)
		{
			Vector3D frontVec(view_.eye_, view_.target_);
			frontVec.V3Norm();
			Vector3D upVec = upVec.GetUpVec(frontVec);
			upVec.V3Norm();
			Vector3D RightVec = RightVec.GetRightVec(frontVec, upVec);
			RightVec.V3Norm();
			Float3 finalOffSet = {
				offset * frontVec.vec_.x_ + offset * upVec.vec_.x_ + offset * RightVec.vec_.x_,
				offsetY * frontVec.vec_.y_ + offsetY * upVec.vec_.y_ + offsetY * RightVec.vec_.y_,
				offsetZ * frontVec.vec_.z_ + offsetZ * upVec.vec_.z_ + offsetZ * RightVec.vec_.z_

			};
			view_.eye_.x = eyeStartPos.x + finalOffSet.x_;
			view_.eye_.y = eyeStartPos.y + finalOffSet.y_;
			view_.eye_.z = eyeStartPos.z + finalOffSet.z_;
			view_.target_.x = targetStart.x + finalOffSet.x_;
			view_.target_.y = targetStart.y + finalOffSet.y_;
			view_.target_.z = targetStart.z + finalOffSet.z_;
		}
	}
	view_.UpDateMatrixView();
	projection_.UpdataMatrixProjection();
	view_.eye_ = eyeStartPos;
	view_.target_ = targetStart;
}

void Camera::WorldPositionUpdate(DirectX::XMMATRIX playerMatrix, DirectX::XMFLOAT3 playerPosition, bool isBillBord)
{
	//object3d_->Update(view,projection);


	object3d_->matWorld_.SetMatScale(object3d_->scale_.x, object3d_->scale_.y, object3d_->scale_.z);
	object3d_->matWorld_.SetMatRot(object3d_->rotation_.x, object3d_->rotation_.y, object3d_->rotation_.z, false);
	object3d_->matWorld_.SetMatTrans(object3d_->position_.x, object3d_->position_.y, object3d_->position_.z);

	if (isBillBord)
	{
		if (object3d_->parent_ == nullptr)
		{
			object3d_->matWorld_.UpdataBillBordMatrixWorld(view_);
		}
		else
		{
			object3d_->matWorld_.UpdataMatrixWorld();
		}
	}
	else
	{
		object3d_->matWorld_.UpdataMatrixWorld();
	}

	if (object3d_->parent_ != nullptr)
	{
		object3d_->matWorld_.matWorld_ *= object3d_->parent_->matWorld_.matWorld_;
	}

	object3d_->constMapTranceform_->world = object3d_->matWorld_.matWorld_ * view_.mat_;
	object3d_->constMapTranceform_->viewproj = projection_.mat_;
	object3d_->constMapTranceform_->cameraPos.x_ = view_.eye_.x;
	object3d_->constMapTranceform_->cameraPos.y_ = view_.eye_.y;
	object3d_->constMapTranceform_->cameraPos.z_ = view_.eye_.z;

	/*object3d_->constMapTranceform->cameraPos.x = playerPosition.x;
	object3d_->constMapTranceform->cameraPos.y = playerPosition.y;
	object3d_->constMapTranceform->cameraPos.z = playerPosition.z;*/


}


XMMATRIX Camera::GetMadWorld()
{
	//ワールド座標を入れる変数
	XMMATRIX matWorld;
	matWorld = object3d_->matWorld_.matWorld_;

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
//	matWorld = object3d_->matWorld;
//
//	return matWorld;
//}

XMFLOAT3 Camera::Transform(DirectX::XMFLOAT3 forward, WorldMatrix matWorld)
{
	XMFLOAT3 resultVec;

	resultVec.x = (forward.x * matWorld.matWorld_.r[0].m128_f32[0]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[0]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[0]);
	resultVec.y = (forward.x * matWorld.matWorld_.r[0].m128_f32[1]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[1]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[1]);
	resultVec.z = (forward.x * matWorld.matWorld_.r[0].m128_f32[2]) + (forward.y * matWorld.matWorld_.r[1].m128_f32[2]) + (forward.z * matWorld.matWorld_.r[2].m128_f32[2]);

	return resultVec;
}



