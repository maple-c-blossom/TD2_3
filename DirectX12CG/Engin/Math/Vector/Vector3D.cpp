#include "Vector3D.h"
#include <cmath>


using namespace MCB;
MCB::Vector3D::Vector3D(Float3 start, Float3 end)
{
	vec_ = end - start;
}

MCB::Vector3D::Vector3D(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end)
{
	vec_.x_ = end.x - start.x;
	vec_.y_ = end.y - start.y;
	vec_.z_ = end.z - start.z;
}



MCB::Vector3D::Vector3D()
{
	vec_.x_ = 0;
	vec_.y_ = 0;
	vec_.z_ = 0;
}
MCB::Vector3D::Vector3D(Float3 vec_)
{
	this->vec_ = vec_;
}
MCB::Vector3D::Vector3D(float x_, float y, float z)
{
	vec_.x_ = x_;
	vec_.y_ = y;
	vec_.z_ = z;
}
Vector3D MCB::Vector3D::V3Get(Float3 start, Float3 end)
{
	Vector3D temp(start, end);
	return temp;
}

Vector3D MCB::Vector3D::V3Get(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end)
{
	Vector3D temp(start, end);
	return temp;
}
float MCB::Vector3D::V3Len() const
{
	return sqrtf(((vec_.x_ * vec_.x_) + (vec_.y_ * vec_.y_) + (vec_.z_ * vec_.z_)));
}

void MCB::Vector3D::V3Norm()
{
	float VecLen = V3Len();
	if (VecLen == 0.0f)
	{
		//assert(0);
		return;
	}

	vec_.x_ = vec_.x_ / VecLen;
	vec_.y_ = vec_.y_ / VecLen;
	vec_.z_ = vec_.z_ / VecLen;

}

Vector3D MCB::Vector3D::normal(Vector3D v)
{
	Vector3D ret = v;
	ret.V3Norm();
	return ret;
}

float MCB::Vector3D::GetV3Dot(Vector3D vector)
{
	return (vec_.x_ * vector.vec_.x_) + (vec_.y_ * vector.vec_.y_) + (vec_.z_ * vector.vec_.z_);
}

Vector3D MCB::Vector3D::GetV3Cross(Vector3D vector)
{
	Vector3D temp;
	temp.vec_.x_ = vec_.y_ * vector.vec_.z_ - vec_.z_ * vector.vec_.y_;
	temp.vec_.y_ = vec_.z_ * vector.vec_.x_ - vec_.x_ * vector.vec_.z_;
	temp.vec_.z_ = vec_.x_ * vector.vec_.y_ - vec_.y_ * vector.vec_.x_;
	return temp;
}

Vector3D MCB::Vector3D::GetV3Cross(Vector3D avector, Vector3D bvector)
{
	Vector3D temp;
	temp.vec_.x_ = avector.vec_.y_ * bvector.vec_.z_ - avector.vec_.z_ * bvector.vec_.y_;
	temp.vec_.y_ = avector.vec_.z_ * bvector.vec_.x_ - avector.vec_.x_ * bvector.vec_.z_;
	temp.vec_.z_ = avector.vec_.x_ * bvector.vec_.y_ - avector.vec_.y_ * bvector.vec_.x_;
	return temp;
}

Vector3D MCB::Vector3D::GetUpVec(Vector3D RightVec, Vector3D frontVec)
{
	Vector3D ans;
	RightVec.V3Norm();
	frontVec.V3Norm();
	ans = ans.GetV3Cross(frontVec, RightVec);
	ans.V3Norm();

	return ans;
}


Vector3D MCB::Vector3D::GetRightVec(Vector3D frontVec, Vector3D UpVec)
{
	Vector3D ans;
	frontVec.V3Norm();
	UpVec.V3Norm();
	ans = ans.GetV3Cross(UpVec, frontVec);
	ans.V3Norm();

	return ans;
}

DirectX::XMFLOAT3 MCB::Vector3D::ConvertXMFloat3()
{
	DirectX::XMFLOAT3 temp;
	temp.x = vec_.x_, temp.y = vec_.y_, temp.z = vec_.z_;
	return temp;
}

float MCB::Vector3D::GetInnerProduct(Vector3D vector)
{
	Vector3D temp;
	temp.vec_ = vec_;
	temp.V3Norm();
	vector.V3Norm();
	return acos(temp.GetV3Dot(vector));
}

Vector3D& MCB::Vector3D::operator+=(const Vector3D& vec_)
{
	this->vec_.x_ += vec_.vec_.x_;
	this->vec_.y_ += vec_.vec_.y_;
	this->vec_.z_ += vec_.vec_.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator-=(const Vector3D& vec_)
{
	this->vec_.x_ -= vec_.vec_.x_;
	this->vec_.y_ -= vec_.vec_.y_;
	this->vec_.z_ -= vec_.vec_.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator*=(const float& k)
{
	this->vec_.x_ *= k;
	this->vec_.y_ *= k;
	this->vec_.z_ *= k;
	return *this;
}

Vector3D& MCB::Vector3D::operator/=(const float& k)
{
	this->vec_.x_ /= k;
	this->vec_.y_ /= k;
	this->vec_.z_ /= k;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const Float3 a)
{
	this->vec_.x_ = a.x_;
	this->vec_.y_ = a.y_;
	this->vec_.z_ = a.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const Vector3D a)
{
	this->vec_.x_ = a.vec_.x_;
	this->vec_.y_ = a.vec_.y_;
	this->vec_.z_ = a.vec_.z_;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const DirectX::XMFLOAT3 a)
{
	this->vec_.x_ = a.x;
	this->vec_.y_ = a.y;
	this->vec_.z_ = a.z;
	return *this;
}

Vector3D MCB::operator+(Vector3D& vecA, Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp += vecB;
	return temp;
}

Vector3D MCB::operator-(Vector3D& vecA, Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp -= vecB;
	return temp;
}

Vector3D MCB::operator-(DirectX::XMFLOAT3& vecA, Vector3D& vecB)
{
	Vector3D temp;
	temp.vec_.x_ = vecA.x - vecB.vec_.x_;
	temp.vec_.y_ = vecA.y - vecB.vec_.y_;
	temp.vec_.z_ = vecA.z - vecB.vec_.z_;
	return temp;
}

Vector3D MCB::operator-(Vector3D& vecA, DirectX::XMFLOAT3& vecB)
{
	Vector3D temp;
	temp.vec_.x_ = vecA.vec_.x_ - vecB.x;
	temp.vec_.y_ = vecA.vec_.y_ - vecB.y;
	temp.vec_.z_ = vecA.vec_.z_ - vecB.z;
	return temp;
}

Vector3D MCB::operator*(Vector3D& vecA, float k)
{
	Vector3D temp;
	temp = vecA;
	temp *= k;
	return temp;
}

Vector3D MCB::operator/(Vector3D& vecA, float k)
{
	Vector3D temp;
	temp = vecA;
	temp /= k;
	return temp;
}

Vector3D MCB::operator-(Vector3D& vecA)
{
	Vector3D temp;
	temp.vec_.x_ = vecA.vec_.x_ * -1;
	temp.vec_.y_ = vecA.vec_.y_ * -1;
	temp.vec_.z_ = vecA.vec_.z_ * -1;
	return temp;
}
