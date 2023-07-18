#pragma once

#include "Vector3D.h"

class ADXUtility
{
public:
	static MCB::Vector3D Lerp(MCB::Vector3D nowPos, MCB::Vector3D newPos, float progress);
	static float AngleDiff(float angle1, float angle2);
	static float ValueMapping(float nowValue, float inputStart, float inputEnd,
		float outputStart, float outputEnd);
};
