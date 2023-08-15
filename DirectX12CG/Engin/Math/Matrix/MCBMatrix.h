#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"
#include <array>
namespace MCB
{

	class MCBMatrix
	{
	public:
		float _11_; float _12_; float _13_; float _14_;
		float _21_; float _22_; float _23_; float _24_;
		float _31_; float _32_; float _33_; float _34_;
		float _41_; float _42_; float _43_; float _44_;
		
	public:
		//yADXEngine—R—ˆz’PˆÊs—ñ‚ğ•Ô‚·
		static MCBMatrix IdentityMatrix();

		void MCBMatrixIdentity();

		static MCBMatrix GetMCBUdentuty();

		MCBMatrix MCBMatrixTransrate(float x, float y, float z);

		MCBMatrix MCBMatrixScaling(float x, float y, float z);

		static std::array<std::array<float,4>,4> ConvertMatrixArray(MCBMatrix mat);

		static std::array<std::array<float, 4>, 4> GetConvertMatrixArray(MCBMatrix mat);

		MCBMatrix ConvertMatrixMCBMat(float** ArrayMat);

		MCBMatrix ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat);

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

