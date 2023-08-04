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
	_11_ = 1.0f; _12_ = 0.0f; _13_ = 0.0f; _14_ = 0.0f;
	_21_ = 0.0f; _22_ = 1.0f; _23_ = 0.0f; _24_ = 0.0f;
	_31_ = 0.0f; _32_ = 0.0f; _33_ = 1.0f; _34_ = 0.0f;
	_41_ = 0.0f; _42_ = 0.0f; _43_ = 0.0f; _44_ = 1.0f;
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

	temp._41_ = x;
	temp._42_ = y;
	temp._43_ = z;
	temp._44_ = 1;

	return temp;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixScaling(float x, float y, float z)
{

	MCBMatrix temp;
	temp.MCBMatrixIdentity();

	temp._11_ = x;
	temp._22_ = y;
	temp._33_ = z;
	temp._44_ = 1;

	return temp;
	return MCBMatrix();
}

std::array<std::array<float, 4>, 4> MCB::MCBMatrix::ConvertMatrixArray(MCB::MCBMatrix mat)
{
	std::array<std::array<float, 4>, 4> temp = {};

	temp[0][0] = mat._11_, temp[0][1] = mat._12_, temp[0][2] = mat._13_, temp[0][3] = mat._14_;
	temp[1][0] = mat._21_, temp[1][1] = mat._22_, temp[1][2] = mat._23_, temp[1][3] = mat._24_;
	temp[2][0] = mat._31_, temp[2][1] = mat._32_, temp[2][2] = mat._33_, temp[2][3] = mat._34_;
	temp[3][0] = mat._41_, temp[3][1] = mat._42_, temp[3][2] = mat._43_, temp[3][3] = mat._44_;

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
	temp._11_ = ArrayMat[0][0], temp._12_ = ArrayMat[0][1], temp._13_ = ArrayMat[0][2], temp._14_ = ArrayMat[0][3];
	temp._21_ = ArrayMat[1][0], temp._22_ = ArrayMat[1][1], temp._23_ = ArrayMat[1][2], temp._24_ = ArrayMat[1][3];
	temp._31_ = ArrayMat[2][0], temp._32_ = ArrayMat[2][1], temp._33_ = ArrayMat[2][2], temp._34_ = ArrayMat[2][3];
	temp._41_ = ArrayMat[3][0], temp._42_ = ArrayMat[3][1], temp._43_ = ArrayMat[3][2], temp._44_ = ArrayMat[3][3];
	return temp;
}

MCBMatrix MCB::MCBMatrix::ConvertMatrixMCBMat(std::array<std::array<float, 4>, 4> ArrayMat)
{
	MCBMatrix temp;
	temp._11_ = ArrayMat[0][0], temp._12_ = ArrayMat[0][1], temp._13_ = ArrayMat[0][2], temp._14_ = ArrayMat[0][3];
	temp._21_ = ArrayMat[1][0], temp._22_ = ArrayMat[1][1], temp._23_ = ArrayMat[1][2], temp._24_ = ArrayMat[1][3];
	temp._31_ = ArrayMat[2][0], temp._32_ = ArrayMat[2][1], temp._33_ = ArrayMat[2][2], temp._34_ = ArrayMat[2][3];
	temp._41_ = ArrayMat[3][0], temp._42_ = ArrayMat[3][1], temp._43_ = ArrayMat[3][2], temp._44_ = ArrayMat[3][3];
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

	matrix._22_ = cosf(angle);
	matrix._23_ = sinf(angle);

	matrix._32_ = -sinf(angle);
	matrix._33_ = cosf(angle);

	return matrix;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRotaY(float angle)
{
	MCBMatrix matrix;

	matrix.MCBMatrixIdentity();
	matrix,_11_ = cosf(angle);
	matrix._31_ = -sin(angle);

	matrix._13_ = sinf(angle);
	matrix._33_ = cosf(angle);

	return matrix;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRotaZ(float angle)
{
	MCBMatrix matrix;
	matrix.MCBMatrixIdentity();

	matrix._11_ = cos(angle);
	matrix._12_ = sin(angle);

	matrix._21_ = -sin(angle);
	matrix._22_ = cos(angle);

	return matrix;
}


MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRota(double angle, DirectX::XMFLOAT3 vec_tor)
{
	MCBMatrix matRot = ReturnMatrixIdentity();

	matRot._11_ = (vec_tor.x * vec_tor.x) * (1 - cos(angle)) + cos(angle);
	matRot._12_ = vec_tor.x * vec_tor.y * (1 - cos(angle)) - vec_tor.z * sin(angle);
	matRot._13_ = vec_tor.x * vec_tor.z * (1 - cos(angle)) + vec_tor.y * sin(angle);
	matRot._14_ = 0;

	matRot._21_ = vec_tor.x * vec_tor.y * (1 - cos(angle)) + vec_tor.z * sin(angle);
	matRot._22_ = (vec_tor.y * vec_tor.y) * (1 - cos(angle)) + cos(angle);
	matRot._23_ = vec_tor.y * vec_tor.z * (1 - cos(angle)) + vec_tor.x * sin(angle);
	matRot._24_ = 0;

	matRot._31_ = vec_tor.x * vec_tor.z * (1 - cos(angle)) - vec_tor.y * sin(angle);
	matRot._32_ = vec_tor.y * vec_tor.z * (1 - cos(angle)) + vec_tor.x * sin(angle);
	matRot._33_ = (vec_tor.z * vec_tor.z) * (1 - cos(angle)) + cos(angle);
	matRot._34_ = 0;

	matRot._41_ = 0;
	matRot._42_ = 0;
	matRot._43_ = 0;
	matRot._44_ = 1;

	return matRot;


}

void MCB::MCBMatrix::SetRotaMatrix(Float3 angle)
{

}

MCB::MCBMatrix MCB::MCBMatrix::ReturnMatrixIdentity()
{
	MCBMatrix ans;
		ans._11_ = 1.0f; ans._12_ = 0.0f; ans._13_ = 0.0f; ans._14_ = 0.0f;
		ans._21_ = 0.0f; ans._22_ = 1.0f; ans._23_ = 0.0f; ans._24_ = 0.0f;
		ans._31_ = 0.0f; ans._32_ = 0.0f; ans._33_ = 1.0f; ans._34_ = 0.0f;
		ans._41_ = 0.0f; ans._42_ = 0.0f; ans._43_ = 0.0f; ans._44_ = 1.0f;
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
	float w = v.vec_.x_ * m._14_ + v.vec_.y_ * m._24_ + v.vec_.z_ * m._34_ + m._44_;

	Vector3D result =
	{
		(v.vec_.x_ * m._11_ + v.vec_.y_ * m._21_ + v.vec_.z_ * m._31_ + m._41_) / w,
		(v.vec_.x_ * m._12_ + v.vec_.y_ * m._22_ + v.vec_.z_ * m._32_ + m._42_) / w,
		(v.vec_.x_ * m._13_ + v.vec_.y_ * m._23_ + v.vec_.z_ * m._33_ + m._43_) / w
	};

	return result;
}

//MCB::MCBMatrix MCB::MCBMatrix::operator*(MCBMatrix matrix)
//{
//	MCBMatrix ans;
//
//#pragma region １行目
//	ans._11_ = (_11_ * matrix._11_) + (_12_ * matrix._21_) + (_13_ * matrix._31_) + (_14_ * matrix._41_);
//	ans._12_ = (_11_ * matrix._12_) + (_12_ * matrix._22_) + (_13_ * matrix._32_) + (_14_ * matrix._42_);
//	ans._13_ = (_11_ * matrix._13_) + (_12_ * matrix._23_) + (_13_ * matrix._33_) + (_14_ * matrix._43_);
//	ans._14_ = (_11_ * matrix._14_) + (_12_ * matrix._24_) + (_13_ * matrix._34_) + (_14_ * matrix._44_);
//#pragma endregion 1行目
//
//#pragma region 2行目
//	ans._21_ = (_21_ * matrix._11_) + (_22_ * matrix._21_) + (_23_ * matrix._31_) + (_24_ * matrix._41_);
//	ans._22_ = (_21_ * matrix._12_) + (_22_ * matrix._22_) + (_23_ * matrix._32_) + (_24_ * matrix._42_);
//	ans._23_ = (_21_ * matrix._13_) + (_22_ * matrix._23_) + (_23_ * matrix._33_) + (_24_ * matrix._43_);
//	ans._24_ = (_21_ * matrix._14_) + (_22_ * matrix._24_) + (_23_ * matrix._34_) + (_24_ * matrix._44_);
//#pragma endregion 2行目
//
//#pragma region 3行目
//	ans._31_ = (_31_ * matrix._11_) + (_32_ * matrix._21_) + (_33_ * matrix._31_) + (_34_ * matrix._41_);
//	ans._32_ = (_31_ * matrix._12_) + (_32_ * matrix._22_) + (_33_ * matrix._32_) + (_34_ * matrix._42_);
//	ans._33_ = (_31_ * matrix._13_) + (_32_ * matrix._23_) + (_33_ * matrix._33_) + (_34_ * matrix._43_);
//	ans._34_ = (_31_ * matrix._14_) + (_32_ * matrix._24_) + (_33_ * matrix._34_) + (_34_ * matrix._44_);
//#pragma endregion 3行目
//
//#pragma region 4行目
//	ans._41_ = (_41_ * matrix._11_) + (_42_ * matrix._21_) + (_43_ * matrix._31_) + (_44_ * matrix._41_);
//	ans._42_ = (_41_ * matrix._12_) + (_42_ * matrix._22_) + (_43_ * matrix._32_) + (_44_ * matrix._42_);
//	ans._43_ = (_41_ * matrix._13_) + (_42_ * matrix._23_) + (_43_ * matrix._33_) + (_44_ * matrix._43_);
//	ans._44_ = (_41_ * matrix._14_) + (_42_ * matrix._24_) + (_43_ * matrix._34_) + (_44_ * matrix._44_);
//#pragma endregion 4行目
//
//	return ans;
//}

DirectX::XMMATRIX MCB::MCBMatrix::MatrixConvertXMMatrix(MCBMatrix mat)
{
	XMMATRIX ans;
	ans.r[0].m128_f32[0] = mat._11_;
	ans.r[0].m128_f32[1] = mat._12_;
	ans.r[0].m128_f32[2] = mat._13_;
	ans.r[0].m128_f32[3] = mat._14_;

	ans.r[1].m128_f32[0] = mat._21_;
	ans.r[1].m128_f32[1] = mat._22_;
	ans.r[1].m128_f32[2] = mat._23_;
	ans.r[1].m128_f32[3] = mat._24_;

	ans.r[2].m128_f32[0] = mat._31_;
	ans.r[2].m128_f32[1] = mat._32_;
	ans.r[2].m128_f32[2] = mat._33_;
	ans.r[2].m128_f32[3] = mat._34_;

	ans.r[3].m128_f32[0] = mat._41_;
	ans.r[3].m128_f32[1] = mat._42_;
	ans.r[3].m128_f32[2] = mat._43_;
	ans.r[3].m128_f32[3] = mat._44_;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixConvertXMMatrix(XMMATRIX mat)
{
	MCBMatrix ans;
	ans._11_ = mat.r[0].m128_f32[0];
	ans._12_ = mat.r[0].m128_f32[1];
	ans._13_ = mat.r[0].m128_f32[2];
	ans._14_ = mat.r[0].m128_f32[3];
						 
	ans._21_ = mat.r[1].m128_f32[0] ;
	ans._22_ = mat.r[1].m128_f32[1] ;
	ans._23_ = mat.r[1].m128_f32[2] ;
	ans._24_ = mat.r[1].m128_f32[3] ;
						 
	ans._31_ = mat.r[2].m128_f32[0] ;
	ans._32_ = mat.r[2].m128_f32[1] ;
	ans._33_ = mat.r[2].m128_f32[2] ;
	ans._34_ = mat.r[2].m128_f32[3] ;
						 
	ans._41_ = mat.r[3].m128_f32[0];
	ans._42_ = mat.r[3].m128_f32[1] ;
	ans._43_ = mat.r[3].m128_f32[2] ;
	ans._44_ = mat.r[3].m128_f32[3] ;
	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator*(float s)
{
	MCBMatrix ans;
	ans._11_ = _11_ * s;
	ans._12_ = _12_ * s;
	ans._13_ = _13_ * s;
	ans._14_ = _14_ * s;

	ans._21_ = _21_ * s;
	ans._22_ = _22_ * s;
	ans._23_ = _23_ * s;
	ans._24_ = _24_ * s;

	ans._31_ = _31_ * s;
	ans._32_ = _32_ * s;
	ans._33_ = _33_ * s;
	ans._34_ = _34_ * s;

	ans._41_ = _41_ * s;
	ans._42_ = _42_ * s;
	ans._43_ = _43_ * s;
	ans._44_ = _44_ * s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator/(float s)
{
	MCBMatrix ans;
	ans._11_ = _11_ / s;
	ans._12_ = _12_ / s;
	ans._13_ = _13_ / s;
	ans._14_ = _14_ / s;

	ans._21_ = _21_ / s;
	ans._22_ = _22_ / s;
	ans._23_ = _23_ / s;
	ans._24_ = _24_ / s;

	ans._31_ = _31_ / s;
	ans._32_ = _32_ / s;
	ans._33_ = _33_ / s;
	ans._34_ = _34_ / s;

	ans._41_ = _41_ / s;
	ans._42_ = _42_ / s;
	ans._43_ = _43_ / s;
	ans._44_ = _44_ / s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator+(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11_ = _11_ + matrix._11_; 
	ans._12_ = _12_ + matrix._12_; 
	ans._13_ = _13_ + matrix._13_; 
	ans._14_ = _14_ + matrix._14_; 

	ans._21_ = _21_ + matrix._21_;
	ans._22_ = _22_ + matrix._22_;
	ans._23_ = _23_ + matrix._23_;
	ans._24_ = _24_ + matrix._24_;

	ans._31_ = _31_ + matrix._31_;
	ans._32_ = _32_ + matrix._32_;
	ans._33_ = _33_ + matrix._33_;
	ans._34_ = _34_ + matrix._34_;

	ans._41_ = _41_ + matrix._41_;
	ans._42_ = _42_ + matrix._42_;
	ans._43_ = _43_ + matrix._43_;
	ans._44_ = _44_ + matrix._44_;


	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator-(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11_ = _11_ - matrix._11_;
	ans._12_ = _12_ - matrix._12_;
	ans._13_ = _13_ - matrix._13_;
	ans._14_ = _14_ - matrix._14_;

	ans._21_ = _21_ - matrix._21_;
	ans._22_ = _22_ - matrix._22_;
	ans._23_ = _23_ - matrix._23_;
	ans._24_ = _24_ - matrix._24_;

	ans._31_ = _31_ - matrix._31_;
	ans._32_ = _32_ - matrix._32_;
	ans._33_ = _33_ - matrix._33_;
	ans._34_ = _34_ - matrix._34_;

	ans._41_ = _41_ - matrix._41_;
	ans._42_ = _42_ - matrix._42_;
	ans._43_ = _43_ - matrix._43_;
	ans._44_ = _44_ - matrix._44_;


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
		return _11_;
		break;

	case 1:
		return _12_;
		break;

	case 2:
		return _13_;
		break;
		
	case 3:
		return _14_;
		break;

	case 4:
		return _21_;
		break;

	case 5:
		return _22_;
		break;

	case 6:
		return _23_;
		break;

	case 7:
		return _24_;
		break;

	case 8:
		return _31_;
		break;

	case 9:
		return _32_;
		break;

	case 10:
		return _33_;
		break;

	case 11:
		return _34_;
		break;

	case 12:
		return _41_;
		break;

	case 13:
		return _42_;
		break;

	case 14:
		return _43_;
		break;

	case 15:
		return _44_;
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
	ans._11_ = (matrix1._11_ * matrix2._11_) + (matrix1._12_ * matrix2._21_) + (matrix1._13_ * matrix2._31_) + (matrix1._14_ * matrix2._41_);
	ans._12_ = (matrix1._11_ * matrix2._12_) + (matrix1._12_ * matrix2._22_) + (matrix1._13_ * matrix2._32_) + (matrix1._14_ * matrix2._42_);
	ans._13_ = (matrix1._11_ * matrix2._13_) + (matrix1._12_ * matrix2._23_) + (matrix1._13_ * matrix2._33_) + (matrix1._14_ * matrix2._43_);
	ans._14_ = (matrix1._11_ * matrix2._14_) + (matrix1._12_ * matrix2._24_) + (matrix1._13_ * matrix2._34_) + (matrix1._14_ * matrix2._44_);
#pragma endregion 1行目

#pragma region 2行目
	ans._21_ = (matrix1._21_ * matrix2._11_) + (matrix1._22_ * matrix2._21_) + (matrix1._23_ * matrix2._31_) + (matrix1._24_ * matrix2._41_);
	ans._22_ = (matrix1._21_ * matrix2._12_) + (matrix1._22_ * matrix2._22_) + (matrix1._23_ * matrix2._32_) + (matrix1._24_ * matrix2._42_);
	ans._23_ = (matrix1._21_ * matrix2._13_) + (matrix1._22_ * matrix2._23_) + (matrix1._23_ * matrix2._33_) + (matrix1._24_ * matrix2._43_);
	ans._24_ = (matrix1._21_ * matrix2._14_) + (matrix1._22_ * matrix2._24_) + (matrix1._23_ * matrix2._34_) + (matrix1._24_ * matrix2._44_);
#pragma endregion 2行目

#pragma region 3行目
	ans._31_ = (matrix1._31_ * matrix2._11_) + (matrix1._32_ * matrix2._21_) + (matrix1._33_ * matrix2._31_) + (matrix1._34_ * matrix2._41_);
	ans._32_ = (matrix1._31_ * matrix2._12_) + (matrix1._32_ * matrix2._22_) + (matrix1._33_ * matrix2._32_) + (matrix1._34_ * matrix2._42_);
	ans._33_ = (matrix1._31_ * matrix2._13_) + (matrix1._32_ * matrix2._23_) + (matrix1._33_ * matrix2._33_) + (matrix1._34_ * matrix2._43_);
	ans._34_ = (matrix1._31_ * matrix2._14_) + (matrix1._32_ * matrix2._24_) + (matrix1._33_ * matrix2._34_) + (matrix1._34_ * matrix2._44_);
#pragma endregion 3行目

#pragma region 4行目
	ans._41_ = (matrix1._41_ * matrix2._11_) + (matrix1._42_ * matrix2._21_) + (matrix1._43_ * matrix2._31_) + (matrix1._44_ * matrix2._41_);
	ans._42_ = (matrix1._41_ * matrix2._12_) + (matrix1._42_ * matrix2._22_) + (matrix1._43_ * matrix2._32_) + (matrix1._44_ * matrix2._42_);
	ans._43_ = (matrix1._41_ * matrix2._13_) + (matrix1._42_ * matrix2._23_) + (matrix1._43_ * matrix2._33_) + (matrix1._44_ * matrix2._43_);
	ans._44_ = (matrix1._41_ * matrix2._14_) + (matrix1._42_ * matrix2._24_) + (matrix1._43_ * matrix2._34_) + (matrix1._44_ * matrix2._44_);
#pragma endregion 4行目

	return ans;
}
