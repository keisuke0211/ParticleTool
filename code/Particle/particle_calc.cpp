//========================================
// 
// �p�[�e�B�N���̌v�Z����
// author KEISUKE OOTONO
// 
//========================================
// *** particle_calc.cpp ***
//========================================
#include "particle_calc.h"
//****************************************
// ���x�v�Z�̏���
// author KEISUKE OOTONO
//****************************************
void SpeedCalc(Particle *pPar, Particle_Sub *pParSub, ParticleType *pPType, int nCntPar)
{
	/* ���x */
	float fSpeed = 0.0f;
	float fSpeedX = 0.0f;
	float fSpeedY = 0.0f;
	float fSpeedZ = 0.0f;

	switch (pPar->nMoveType)
	{
	case 0:
	{
		fSpeedX = pPar->speed.x;
		fSpeedY = pPar->speed.y;
		fSpeedZ = pPar->speed.z;

		pParSub->move.x = fSpeedX;
		pParSub->move.y = fSpeedY;
		pParSub->move.z = fSpeedZ;
		break;
	}
	case 1:
	{
		//�p�x�̐ݒ�
		pParSub->fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

		if (pPar->speed.x <= 0)
		{
			pPar->speed.x = 1.0f;
			pPType[nCntPar].move.x = 1.0f;
		}
		if (pPar->speed.z <= 0)
		{
			pPar->speed.z = 1.0f;
			pPType[nCntPar].move.z = 1.0f;
		}

		fSpeedX = (float)(rand() % (2 * (int)pPar->speed.x)) / (int)pPar->speed.x + 1;
		fSpeedY = pPar->speed.y;
		fSpeedZ = (float)(rand() % (2 * (int)pPar->speed.z)) / (int)pPar->speed.z + 1;

		pParSub->move.x = sinf(pParSub->fRot) * fSpeedX;
		pParSub->move.y = fSpeedY;
		pParSub->move.z = cosf(pParSub->fRot) * fSpeedZ;

		pParSub->move = pParSub->move;
		break;
	}
	case 2:
	{
		Vector3 rotVec = {
			pPar->rot.x,
			pPar->rot.y,
			pPar->rot.z,
		};

		Vector3 moveVec = directionToNormal(rotVec);

		pParSub->move.x = ((sinf(pPar->rot.y) + sinf(pPar->rot.z)) / 2) * pPar->speed.y;
		pParSub->move.y = ((cosf(pPar->rot.x) + cosf(pPar->rot.z)) / 2) * pPar->speed.y;
		pParSub->move.z = ((cosf(pPar->rot.y) + sinf(pPar->rot.x)) / 2) * pPar->speed.y;

		break;
	}
	}
}
//****************************************
// 3�����x�N�g���̊O�ς��v�Z����֐�
// author KEISUKE OOTONO
//****************************************
Vector3 cross(const Vector3& v1, const Vector3& v2) {
	double x = v1.y * v2.z - v1.z * v2.y;
	double y = v1.z * v2.x - v1.x * v2.z;
	double z = v1.x * v2.y - v1.y * v2.x;
	return Vector3(x, y, z);
}
//******************************************
// �����x�N�g����@���x�N�g���ɕϊ�����֐�
// author KEISUKE OOTONO
//******************************************
Vector3 directionToNormal(const Vector3& dir)
{
	// dir �x�N�g����P�ʃx�N�g���ɐ��K������
	Vector3 normal = dir;
	normal.normalize();

	// y �������̒P�ʃx�N�g��
	Vector3 up = Vector3(0, 1, 0);
	// dir �x�N�g���� y �������̒P�ʃx�N�g���̊O�ς����
	Vector3 right = cross(up, normal);
	// right �x�N�g���� dir �x�N�g���̊O�ς����
	Vector3 newUp = cross(normal, right);
	// �@���x�N�g����Ԃ�
	return newUp;
};
