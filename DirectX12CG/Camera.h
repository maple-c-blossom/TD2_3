#pragma once
#include "ICamera.h"
#include "Object3d.h"
#include "Util.h"


class Player;
namespace MCB
{
	class Camera :public ICamera
	{
	public:
		void Inilialize()override;
		void Update()override;

		void WorldPositionUpdate(DirectX::XMMATRIX playerMatrix, DirectX::XMFLOAT3 playerPosition,bool isBillBord);

		void WorldPositionInit();

		DirectX::XMFLOAT3 Transform(DirectX::XMFLOAT3 forward, WorldMatrix matworld);

		//ワールド変換行列
		DirectX::XMMATRIX GetMadWorld();

		//WorldMatrix GetMadWorld2();
		Shake* GetShakePtr() { return &shakeX; }

		Player* player;
	private:

		std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();

		//Object3d* object3d = nullptr;
		Shake shakeX;
		Shake shakeY;
		Shake shakeZ;
		
		Float3 distance = {0,45,-60};

		DirectX::XMFLOAT3 eyeStartPos;

		//プレイヤーのワールド変換行列
		DirectX::XMMATRIX playerMatrix_;
		 
		//レールカメラの初期座標
		DirectX::XMFLOAT3 firstPos = { 0,0,0 };

		//レールカメラの初期座標
		DirectX::XMFLOAT3 angle = { 0,0,0 };

		MCB::Vector3D directVec = { 0,0,0 };

	};
}

