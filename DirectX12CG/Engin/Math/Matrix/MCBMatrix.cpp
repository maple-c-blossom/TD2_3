#include "MCBMatrix.h"
#include <stdexcept>
#include <string>

using namespace DirectX;
using namespace MCB;


//【ADXEngine由来】単位行列を返す
MCB::MCBMatrix MCB::MCBMatrix::IdentityMatrix()
{
	MCBMatrix ret;
	ret.MCBMatrixIdentity();
	return ret;
}

void MCB::MCBMatrix::MCBMatrixIdentity()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

MCBMatrix MCB::MCBMatrix::GetMCBUdentuty()
{
	MCBMatrix temp;
	temp.MCBMatrixIdentity();
	return temp;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixTransrate(float x, float y, float z)
{
	MCBMatrix temp;
	temp.MCBMatrixIdentity();

	temp._41 = x;
	temp._42 = y;
	temp._43 = z;
	temp._44 = 1;

	return temp;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixScaling(float x, float y, float z)
{

	MCBMatrix temp;
	temp.MCBMatrixIdentity();

	temp._11 = x;
	temp._22 = y;
	temp._33 = z;
	temp._44 = 1;

	return temp;
	return MCBMatrix();
}

std::array<std::array<float, 4>, 4> MCB::MCBMatrix::ConvertMatrixArray(MCB::MCBMatrix mat)
{
	std::array<std::array<float, 4>, 4> temp = {};

	temp[0][0] = mat._11, temp[0][1] = mat._12, temp[0][2] = mat._13, temp[0][3] = mat._14;
	temp[1][0] = mat._21, temp[1][1] = mat._22, temp[1][2] = mat._23, temp[1][3] = mat._24;
	temp[2][0] = mat._31, temp[2][1] = mat._32, temp[2][2] = mat._33, temp[2][3] = mat._34;
	temp[3][0] = mat._41, temp[3][1] = mat._42, temp[3][2] = mat._43, temp[3][3] = mat._44;

	return temp;
}

std::array<std::array<float, 4>, 4> MCB::MCBMatrix::GetConvertMatrixArray(MCB::MCBMatrix mat)
{
	std::array<std::array<float, 4>, 4> temp = {};
	temp = ConvertMatrixArray(mat);
	return temp;
}

MCBMatrix MCB::MCBMatrix::ConvertMatrixMCBMat(float** ArrayMat)
{
	MCBMatrix temp;
	temp._11 = ArrayMat[0][0], temp._12 = ArrayMat[0][1], temp._13 = ArrayMat[0][2], temp._14 = ArrayMat[0][3];
	temp._21 = ArrayMat[1][0], temp._22 = ArrayMat[1][1], temp._23 = ArrayMat[1][2], temp._24 = ArrayMat[1][3];
	temp._31 = ArrayMat[2][0], temp._32 = ArrayMat[2][1], temp._33 = ArrayMat[2][2], temp._34 = ArrayMat[2][3];
	temp._41 = ArrayMat[3][0], temp._42 = ArrayMat[3][1], temp._43 = ArrayMat[3][2], temp._44 = ArrayMat[3][3];
	return temp;
}

MCBMatrix MCB::MCBMatrix::ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat)
{
	MCBMatrix temp;
	temp._11 = ArrayMat[0][0], temp._12 = ArrayMat[0][1], temp._13 = ArrayMat[0][2], temp._14 = ArrayMat[0][3];
	temp._21 = ArrayMat[1][0], temp._22 = ArrayMat[1][1], temp._23 = ArrayMat[1][2], temp._24 = ArrayMat[1][3];
	temp._31 = ArrayMat[2][0], temp._32 = ArrayMat[2][1], temp._33 = ArrayMat[2][2], temp._34 = ArrayMat[2][3];
	temp._41 = ArrayMat[3][0], temp._42 = ArrayMat[3][1], temp._43 = ArrayMat[3][2], temp._44 = ArrayMat[3][3];
	return temp;
}

MCBMatrix MCB::MCBMatrix::GetConvertMatrucNCBMat(float** ArrayMat)
{
	MCBMatrix temp;
	temp = temp.ConvertMatrixMCBMat(ArrayMat);
	return temp;
}


MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRotaX(float angle)
{
	MCBMatrix matrix;

	matrix.MCBMatrixIdentity();

	matrix._22 = cosf(angle);
	matrix._23 = sinf(angle);

	matrix._32 = -sinf(angle);
	matrix._33 = cosf(angle);

	return matrix;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRotaY(float angle)
{
	MCBMatrix matrix;

	matrix.MCBMatrixIdentity();
	matrix,_11 = cosf(angle);
	matrix._31 = -sin(angle);

	matrix._13 = sinf(angle);
	matrix._33 = cosf(angle);

	return matrix;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRotaZ(float angle)
{
	MCBMatrix matrix;
	matrix.MCBMatrixIdentity();

	matrix._11 = cos(angle);
	matrix._12 = sin(angle);

	matrix._21 = -sin(angle);
	matrix._22 = cos(angle);

	return matrix;
}


MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRota(double angle, DirectX::XMFLOAT3 vector)
{
	MCBMatrix matRot = ReturnMatrixIdentity();

	matRot._11 = (vector.x * vector.x) * (1 - cos(angle)) + cos(angle);
	matRot._12 = vector.x * vector.y * (1 - cos(angle)) - vector.z * sin(angle);
	matRot._13 = vector.x * vector.z * (1 - cos(angle)) + vector.y * sin(angle);
	matRot._14 = 0;

	matRot._21 = vector.x * vector.y * (1 - cos(angle)) + vector.z * sin(angle);
	matRot._22 = (vector.y * vector.y) * (1 - cos(angle)) + cos(angle);
	matRot._23 = vector.y * vector.z * (1 - cos(angle)) + vector.x * sin(angle);
	matRot._24 = 0;

	matRot._31 = vector.x * vector.z * (1 - cos(angle)) - vector.y * sin(angle);
	matRot._32 = vector.y * vector.z * (1 - cos(angle)) + vector.x * sin(angle);
	matRot._33 = (vector.z * vector.z) * (1 - cos(angle)) + cos(angle);
	matRot._34 = 0;

	matRot._41 = 0;
	matRot._42 = 0;
	matRot._43 = 0;
	matRot._44 = 1;

	return matRot;


}

void MCB::MCBMatrix::SetRotaMatrix(Float3 angle)
{

}

MCB::MCBMatrix MCB::MCBMatrix::ReturnMatrixIdentity()
{
	MCBMatrix ans;
		ans._11 = 1.0f; ans._12 = 0.0f; ans._13 = 0.0f; ans._14 = 0.0f;
		ans._21 = 0.0f; ans._22 = 1.0f; ans._23 = 0.0f; ans._24 = 0.0f;
		ans._31 = 0.0f; ans._32 = 0.0f; ans._33 = 1.0f; ans._34 = 0.0f;
		ans._41 = 0.0f; ans._42 = 0.0f; ans._43 = 0.0f; ans._44 = 1.0f;
	return ans;
}

//【ADXEngine由来】逆行列
MCBMatrix MCBMatrix::Inverse() {

	const int N = 4;

	MCBMatrix ret = *this;

	std::array<std::array<float, 4>, 4> inv = ConvertMatrixArray(ret);

	double sweep[N][N * 2];

	double a; /* 定数倍用 */

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			/* sweepの左側に逆行列を求める行列をセット */
			sweep[i][j] = inv[i][j];

			/* sweepの右側に単位行列をセット */
			sweep[i][N + j] = ConvertMatrixArray(IdentityMatrix())[i][j];
		}
	}


	/* 全ての列の対角成分に対する繰り返し */
	for (int k = 0; k < N; k++) {

		/* sweep[k][k]に掛けると1になる値を求める */
		a = 1 / sweep[k][k];

		/* 操作（２）：k行目をa倍する */
		for (int j = 0; j < N * 2; j++) {
			/* これによりsweep[k][k]が1になる */
			sweep[k][j] *= a;
		}

		/* 操作（３）によりk行目以外の行のk列目を0にする */
		for (int i = 0; i < N; i++) {
			if (i == k) {
				/* k行目はそのまま */
				continue;
			}

			/* k行目に掛ける値を求める */
			a = -sweep[i][k];

			for (int j = 0; j < N * 2; j++) {
				/* i行目にk行目をa倍した行を足す */
				/* これによりsweep[i][k]が0になる */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweepの右半分がmatの逆行列 */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			inv[i][j] = sweep[i][N + j];
		}
	}

	ret = ConvertMatrixMCBMat(inv);

	return ret;
}

//【ADXEngine由来】座標変換（ベクトルと行列の掛け算をする）
Vector3D MCBMatrix::transform(const Vector3D& v, const MCBMatrix& m)
{
	float w = v.vec_.x_ * m._14 + v.vec_.y_ * m._24 + v.vec_.z_ * m._34 + m._44;

	Vector3D result = 
	{
		(v.vec_.x_ * m._11 + v.vec_.y_ * m._21 + v.vec_.z_ * m._31 + m._41) / w,
		(v.vec_.x_ * m._12 + v.vec_.y_ * m._22 + v.vec_.z_ * m._32 + m._42) / w,
		(v.vec_.x_ * m._13 + v.vec_.y_ * m._23 + v.vec_.z_ * m._33 + m._43) / w
	};

	return result;
}

//MCB::MCBMatrix MCB::MCBMatrix::operator*(MCBMatrix matrix)
//{
//	MCBMatrix ans;
//
//#pragma region １行目
//	ans._11 = (_11 * matrix._11) + (_12 * matrix._21) + (_13 * matrix._31) + (_14 * matrix._41);
//	ans._12 = (_11 * matrix._12) + (_12 * matrix._22) + (_13 * matrix._32) + (_14 * matrix._42);
//	ans._13 = (_11 * matrix._13) + (_12 * matrix._23) + (_13 * matrix._33) + (_14 * matrix._43);
//	ans._14 = (_11 * matrix._14) + (_12 * matrix._24) + (_13 * matrix._34) + (_14 * matrix._44);
//#pragma endregion 1行目
//
//#pragma region 2行目
//	ans._21 = (_21 * matrix._11) + (_22 * matrix._21) + (_23 * matrix._31) + (_24 * matrix._41);
//	ans._22 = (_21 * matrix._12) + (_22 * matrix._22) + (_23 * matrix._32) + (_24 * matrix._42);
//	ans._23 = (_21 * matrix._13) + (_22 * matrix._23) + (_23 * matrix._33) + (_24 * matrix._43);
//	ans._24 = (_21 * matrix._14) + (_22 * matrix._24) + (_23 * matrix._34) + (_24 * matrix._44);
//#pragma endregion 2行目
//
//#pragma region 3行目
//	ans._31 = (_31 * matrix._11) + (_32 * matrix._21) + (_33 * matrix._31) + (_34 * matrix._41);
//	ans._32 = (_31 * matrix._12) + (_32 * matrix._22) + (_33 * matrix._32) + (_34 * matrix._42);
//	ans._33 = (_31 * matrix._13) + (_32 * matrix._23) + (_33 * matrix._33) + (_34 * matrix._43);
//	ans._34 = (_31 * matrix._14) + (_32 * matrix._24) + (_33 * matrix._34) + (_34 * matrix._44);
//#pragma endregion 3行目
//
//#pragma region 4行目
//	ans._41 = (_41 * matrix._11) + (_42 * matrix._21) + (_43 * matrix._31) + (_44 * matrix._41);
//	ans._42 = (_41 * matrix._12) + (_42 * matrix._22) + (_43 * matrix._32) + (_44 * matrix._42);
//	ans._43 = (_41 * matrix._13) + (_42 * matrix._23) + (_43 * matrix._33) + (_44 * matrix._43);
//	ans._44 = (_41 * matrix._14) + (_42 * matrix._24) + (_43 * matrix._34) + (_44 * matrix._44);
//#pragma endregion 4行目
//
//	return ans;
//}

DirectX::XMMATRIX MCB::MCBMatrix::MatrixConvertXMMatrix(MCBMatrix mat)
{
	XMMATRIX ans;
	ans.r[0].m128_f32[0] = mat._11;
	ans.r[0].m128_f32[1] = mat._12;
	ans.r[0].m128_f32[2] = mat._13;
	ans.r[0].m128_f32[3] = mat._14;

	ans.r[1].m128_f32[0] = mat._21;
	ans.r[1].m128_f32[1] = mat._22;
	ans.r[1].m128_f32[2] = mat._23;
	ans.r[1].m128_f32[3] = mat._24;

	ans.r[2].m128_f32[0] = mat._31;
	ans.r[2].m128_f32[1] = mat._32;
	ans.r[2].m128_f32[2] = mat._33;
	ans.r[2].m128_f32[3] = mat._34;

	ans.r[3].m128_f32[0] = mat._41;
	ans.r[3].m128_f32[1] = mat._42;
	ans.r[3].m128_f32[2] = mat._43;
	ans.r[3].m128_f32[3] = mat._44;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixConvertXMMatrix(XMMATRIX mat)
{
	MCBMatrix ans;
	ans._11 = mat.r[0].m128_f32[0];
	ans._12 = mat.r[0].m128_f32[1];
	ans._13 = mat.r[0].m128_f32[2];
	ans._14 = mat.r[0].m128_f32[3];
						 
	ans._21 = mat.r[1].m128_f32[0] ;
	ans._22 = mat.r[1].m128_f32[1] ;
	ans._23 = mat.r[1].m128_f32[2] ;
	ans._24 = mat.r[1].m128_f32[3] ;
						 
	ans._31 = mat.r[2].m128_f32[0] ;
	ans._32 = mat.r[2].m128_f32[1] ;
	ans._33 = mat.r[2].m128_f32[2] ;
	ans._34 = mat.r[2].m128_f32[3] ;
						 
	ans._41 = mat.r[3].m128_f32[0];
	ans._42 = mat.r[3].m128_f32[1] ;
	ans._43 = mat.r[3].m128_f32[2] ;
	ans._44 = mat.r[3].m128_f32[3] ;
	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator*(float s)
{
	MCBMatrix ans;
	ans._11 = _11 * s;
	ans._12 = _12 * s;
	ans._13 = _13 * s;
	ans._14 = _14 * s;

	ans._21 = _21 * s;
	ans._22 = _22 * s;
	ans._23 = _23 * s;
	ans._24 = _24 * s;

	ans._31 = _31 * s;
	ans._32 = _32 * s;
	ans._33 = _33 * s;
	ans._34 = _34 * s;

	ans._41 = _41 * s;
	ans._42 = _42 * s;
	ans._43 = _43 * s;
	ans._44 = _44 * s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator/(float s)
{
	MCBMatrix ans;
	ans._11 = _11 / s;
	ans._12 = _12 / s;
	ans._13 = _13 / s;
	ans._14 = _14 / s;

	ans._21 = _21 / s;
	ans._22 = _22 / s;
	ans._23 = _23 / s;
	ans._24 = _24 / s;

	ans._31 = _31 / s;
	ans._32 = _32 / s;
	ans._33 = _33 / s;
	ans._34 = _34 / s;

	ans._41 = _41 / s;
	ans._42 = _42 / s;
	ans._43 = _43 / s;
	ans._44 = _44 / s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator+(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11 = _11 + matrix._11; 
	ans._12 = _12 + matrix._12; 
	ans._13 = _13 + matrix._13; 
	ans._14 = _14 + matrix._14; 

	ans._21 = _21 + matrix._21;
	ans._22 = _22 + matrix._22;
	ans._23 = _23 + matrix._23;
	ans._24 = _24 + matrix._24;

	ans._31 = _31 + matrix._31;
	ans._32 = _32 + matrix._32;
	ans._33 = _33 + matrix._33;
	ans._34 = _34 + matrix._34;

	ans._41 = _41 + matrix._41;
	ans._42 = _42 + matrix._42;
	ans._43 = _43 + matrix._43;
	ans._44 = _44 + matrix._44;


	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator-(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11 = _11 - matrix._11;
	ans._12 = _12 - matrix._12;
	ans._13 = _13 - matrix._13;
	ans._14 = _14 - matrix._14;

	ans._21 = _21 - matrix._21;
	ans._22 = _22 - matrix._22;
	ans._23 = _23 - matrix._23;
	ans._24 = _24 - matrix._24;

	ans._31 = _31 - matrix._31;
	ans._32 = _32 - matrix._32;
	ans._33 = _33 - matrix._33;
	ans._34 = _34 - matrix._34;

	ans._41 = _41 - matrix._41;
	ans._42 = _42 - matrix._42;
	ans._43 = _43 - matrix._43;
	ans._44 = _44 - matrix._44;


	return ans;
}

MCB::MCBMatrix& MCB::MCBMatrix::operator*=(MCBMatrix matrix)
{
	*this = *this * matrix;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator*=(float s)
{
	*this = *this * s;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator/=(float s)
{
	*this = *this / s;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator+=(MCBMatrix matrix)
{
	*this = *this + matrix;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator-=(MCBMatrix matrix)
{
	*this = *this - matrix;
	return *this;
}

float MCB::MCBMatrix::operator[](unsigned int n)
{
	switch (n)
	{
	case 0:
		return _11;
		break;

	case 1:
		return _12;
		break;

	case 2:
		return _13;
		break;
		
	case 3:
		return _14;
		break;

	case 4:
		return _21;
		break;

	case 5:
		return _22;
		break;

	case 6:
		return _23;
		break;

	case 7:
		return _24;
		break;

	case 8:
		return _31;
		break;

	case 9:
		return _32;
		break;

	case 10:
		return _33;
		break;

	case 11:
		return _34;
		break;

	case 12:
		return _41;
		break;

	case 13:
		return _42;
		break;

	case 14:
		return _43;
		break;

	case 15:
		return _44;
		break;
	default:
		throw std::out_of_range(std::string("無効な値が入力されました。0～15までの範囲の整数を入力してください。"));
		return -114514.1919810f;
		break;
	}

	throw std::out_of_range(std::string("無効な値が入力されました。0～15までの範囲の整数を入力してください。"));
	return -114514.1919810f;
}

MCBMatrix MCB::operator*(MCBMatrix matrix1, MCBMatrix matrix2)
{
	MCBMatrix ans;

#pragma region １行目
	ans._11 = (matrix1._11 * matrix2._11) + (matrix1._12 * matrix2._21) + (matrix1._13 * matrix2._31) + (matrix1._14 * matrix2._41);
	ans._12 = (matrix1._11 * matrix2._12) + (matrix1._12 * matrix2._22) + (matrix1._13 * matrix2._32) + (matrix1._14 * matrix2._42);
	ans._13 = (matrix1._11 * matrix2._13) + (matrix1._12 * matrix2._23) + (matrix1._13 * matrix2._33) + (matrix1._14 * matrix2._43);
	ans._14 = (matrix1._11 * matrix2._14) + (matrix1._12 * matrix2._24) + (matrix1._13 * matrix2._34) + (matrix1._14 * matrix2._44);
#pragma endregion 1行目

#pragma region 2行目
	ans._21 = (matrix1._21 * matrix2._11) + (matrix1._22 * matrix2._21) + (matrix1._23 * matrix2._31) + (matrix1._24 * matrix2._41);
	ans._22 = (matrix1._21 * matrix2._12) + (matrix1._22 * matrix2._22) + (matrix1._23 * matrix2._32) + (matrix1._24 * matrix2._42);
	ans._23 = (matrix1._21 * matrix2._13) + (matrix1._22 * matrix2._23) + (matrix1._23 * matrix2._33) + (matrix1._24 * matrix2._43);
	ans._24 = (matrix1._21 * matrix2._14) + (matrix1._22 * matrix2._24) + (matrix1._23 * matrix2._34) + (matrix1._24 * matrix2._44);
#pragma endregion 2行目

#pragma region 3行目
	ans._31 = (matrix1._31 * matrix2._11) + (matrix1._32 * matrix2._21) + (matrix1._33 * matrix2._31) + (matrix1._34 * matrix2._41);
	ans._32 = (matrix1._31 * matrix2._12) + (matrix1._32 * matrix2._22) + (matrix1._33 * matrix2._32) + (matrix1._34 * matrix2._42);
	ans._33 = (matrix1._31 * matrix2._13) + (matrix1._32 * matrix2._23) + (matrix1._33 * matrix2._33) + (matrix1._34 * matrix2._43);
	ans._34 = (matrix1._31 * matrix2._14) + (matrix1._32 * matrix2._24) + (matrix1._33 * matrix2._34) + (matrix1._34 * matrix2._44);
#pragma endregion 3行目

#pragma region 4行目
	ans._41 = (matrix1._41 * matrix2._11) + (matrix1._42 * matrix2._21) + (matrix1._43 * matrix2._31) + (matrix1._44 * matrix2._41);
	ans._42 = (matrix1._41 * matrix2._12) + (matrix1._42 * matrix2._22) + (matrix1._43 * matrix2._32) + (matrix1._44 * matrix2._42);
	ans._43 = (matrix1._41 * matrix2._13) + (matrix1._42 * matrix2._23) + (matrix1._43 * matrix2._33) + (matrix1._44 * matrix2._43);
	ans._44 = (matrix1._41 * matrix2._14) + (matrix1._42 * matrix2._24) + (matrix1._43 * matrix2._34) + (matrix1._44 * matrix2._44);
#pragma endregion 4行目

	return ans;
}
