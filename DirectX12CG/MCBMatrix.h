#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"
namespace MCB
{

	class MCBMatrix
	{
	public:
		float _11; float _12; float _13; float _14;
		float _21; float _22; float _23; float _24;
		float _31; float _32; float _33; float _34;
		float _41; float _42; float _43; float _44;
		
	public:
		//yADXEngine—R—ˆz’PˆÊs—ñ‚ğ•Ô‚·
		static MCBMatrix IdentityMatrix();

		void MCBMatrixIdentity();

		static MCBMatrix GetMCBUdentuty();

		MCBMatrix MCBMatrixTransrate(float x, float y, float z);

		MCBMatrix MCBMatrixScaling(float x, float y, float z);

		static float** ConvertMatrixArray(MCBMatrix mat);

		static float** GetConvertMatrixArray(MCBMatrix mat);

		MCBMatrix ConvertMatrixMCBMat(float** ArrayMat);

		static MCBMatrix GetConvertMatrucNCBMat(float** ArrayMat);

		MCBMatrix MCBMatrixRotaX(float angle);

		MCBMatrix MCBMatrixRotaY(float angle);

		MCBMatrix MCBMatrixRotaZ(float angle);

		MCBMatrix MCBMatrixRota(double angle,DirectX::XMFLOAT3 vector);

		void SetRotaMatrix(Float3 angle);

		MCBMatrix ReturnMatrixIdentity();

		//yADXEngine—R—ˆz‹ts—ñ
		MCBMatrix Inverse();

		//yADXEngine—R—ˆzÀ•W•ÏŠ·iƒxƒNƒgƒ‹‚Æs—ñ‚ÌŠ|‚¯Z‚ğ‚·‚éj
		static Vector3D transform(const Vector3D& v, const MCBMatrix& m);

		MCBMatrix MCBMatrixLookAtLH(Float3 eye, Float3 target, Float3 up);

		MCBMatrix MatrixTranspose(MCBMatrix mat);

		MCBMatrix MatrixReverse(MCBMatrix mat);

		//XMMatrix‚É•ÏŠ·‚·‚éŠÖ”
		static DirectX::XMMATRIX MatrixConvertXMMatrix(MCBMatrix mat);

		static MCBMatrix MCBMatrixConvertXMMatrix( DirectX::XMMATRIX mat);

		//MCBMatrix operator*(MCBMatrix matrix);
		MCBMatrix operator*(float s);
		MCBMatrix operator/(float s);
		MCBMatrix operator+(MCBMatrix matrix);
		MCBMatrix operator-(MCBMatrix matrix);

		MCBMatrix& operator*=(MCBMatrix matrix);
		MCBMatrix& operator*=(float s);
		MCBMatrix& operator/=(float s);
		MCBMatrix& operator+=(MCBMatrix matrix);
		MCBMatrix& operator-=(MCBMatrix matrix);

		float operator[](unsigned int n);

	};
		MCBMatrix operator*(MCBMatrix matrix1,MCBMatrix matrix2);

}

