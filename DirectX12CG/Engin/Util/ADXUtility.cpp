#include "ADXUtility.h"
#include <math.h>

using namespace MCB;


Vector3D ADXUtility::Lerp(Vector3D nowPos, Vector3D newPos, float progress)
{
	Vector3D ret;
	if (progress > 1)
	{
		progress = 1;
	}
	if (progress < 0)
	{
		progress = 0;
	}
	ret = nowPos + ((newPos - nowPos) * progress);
	return ret;
}

//�p�x�̍������߂�@�ړI�̊p�x�ւ����������鎞�Ɏg���Ă݂悤
float ADXUtility::AngleDiff(float angle1, float angle2)
{
	angle1 = fmod(angle1, 2.f * 3.1415f);
	angle2 = fmod(angle2, 2.f * 3.1415f);
	float angle3 = angle2 - 2.f * 3.1415f;
	float angle4 = angle2 + 2.f * 3.1415f;

	float nearestAngleDiff = angle2 - angle1;
	if (fabsf(nearestAngleDiff) > fabsf(angle3 - angle1))
	{
		nearestAngleDiff = angle3 - angle1;
	}
	if (fabsf(nearestAngleDiff) > fabsf(angle4 - angle1))
	{
		nearestAngleDiff = angle4 - angle1;
	}
	return nearestAngleDiff;
}

//inputStart����inputEnd�܂ł̒l��outputStart����outputEnd�܂ł̒l�ɕϊ�����
float ADXUtility::ValueMapping(float nowValue, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	return outputStart + (outputEnd - outputStart) * ((nowValue - inputStart) / (inputEnd - inputStart));
}