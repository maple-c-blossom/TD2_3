#include "ADXMatrix4.h"

ADXMatrix4::ADXMatrix4()
{
	IdentityMatrix();
}

ADXMatrix4::ADXMatrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

XMMATRIX ADXMatrix4::ConvertToXMMatrix()
{
	XMMATRIX outPutXMM;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			outPutXMM.r[i].m128_f32[j] = m[i][j];
		}
	}
	return outPutXMM;
}

ADXMatrix4 ADXMatrix4::ConvertToADXMatrix(XMMATRIX mat)
{
	ADXMatrix4 outPutADXM;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			outPutADXM.m[i][j] = mat.r[i].m128_f32[j];
		}
	}
	return outPutADXM;
}

//�t�s��
ADXMatrix4 ADXMatrix4::Inverse() {

	const int N = 4;

	ADXMatrix4 inv;

	double sweep[N][N * 2];

	double a; /* �萔�{�p */

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			/* sweep�̍����ɋt�s������߂�s����Z�b�g */
			sweep[i][j] = m[i][j];

			/* sweep�̉E���ɒP�ʍs����Z�b�g */
			sweep[i][N + j] = IdentityMatrix().m[i][j];
		}
	}


	/* �S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ� */
	for (int k = 0; k < N; k++) {

		/* sweep[k][k]�Ɋ|�����1�ɂȂ�l�����߂� */
		a = 1 / sweep[k][k];

		/* ����i�Q�j�Fk�s�ڂ�a�{���� */
		for (int j = 0; j < N * 2; j++) {
			/* ����ɂ��sweep[k][k]��1�ɂȂ� */
			sweep[k][j] *= a;
		}

		/* ����i�R�j�ɂ��k�s�ڈȊO�̍s��k��ڂ�0�ɂ��� */
		for (int i = 0; i < N; i++) {
			if (i == k) {
				/* k�s�ڂ͂��̂܂� */
				continue;
			}

			/* k�s�ڂɊ|����l�����߂� */
			a = -sweep[i][k];

			for (int j = 0; j < N * 2; j++) {
				/* i�s�ڂ�k�s�ڂ�a�{�����s�𑫂� */
				/* ����ɂ��sweep[i][k]��0�ɂȂ� */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweep�̉E������mat�̋t�s�� */
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			inv.m[i][j] = sweep[i][N + j];
		}
	}

	return inv;
}

// ������Z�q�I�[�o�[���[�h
ADXMatrix4& ADXMatrix4::operator*=(const ADXMatrix4& m2)
{
	ADXMatrix4 m1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m1.m[i][j] = m[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j]
				+ m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return *this;
}

//���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
ADXVector3 ADXMatrix4::transform(const ADXVector3& v, const ADXMatrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	ADXVector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

// 2�����Z�q�I�[�o�[���[�h
ADXMatrix4 operator*(ADXMatrix4& m1, ADXMatrix4& m2)
{
	ADXMatrix4 result = m1;

	return result *= m2;
}

//�P�ʍs��
ADXMatrix4 IdentityMatrix()
{
	return
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
}
