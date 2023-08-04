#pragma once

#include <DirectXMath.h>
#include "Vector3D.h"

class ADXVector3
{
public:
	float x;
	float y;
	float z;

public:
	ADXVector3();
	ADXVector3(float x, float y, float z);

	DirectX::XMFLOAT3 ConvertToXMFloat3();

	float Length() const;
	ADXVector3 Normalize() const;
	float Dot(const ADXVector3& v) const;
	ADXVector3 Cross(const ADXVector3& v) const;

	MCB::Vector3D ConvertToVector3D();

	ADXVector3 operator+() const;
	ADXVector3 operator-() const;

	ADXVector3& operator+=(const ADXVector3& v);
	ADXVector3& operator-=(const ADXVector3& v);
	ADXVector3& operator*=(float s);
	ADXVector3& operator/=(float s);

	bool operator==(const ADXVector3& v) const;

public:
	static ADXVector3 ConvertToADXVector3(DirectX::XMFLOAT3 xf3);
	static ADXVector3 ConvertToADXVector3(MCB::Vector3D v);
};

const ADXVector3 operator+(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator-(const ADXVector3& v1, const ADXVector3& v2);
const ADXVector3 operator*(const ADXVector3& v, float s);
const ADXVector3 operator*(float s, const ADXVector3& v);
const ADXVector3 operator/(const ADXVector3& v, float s);