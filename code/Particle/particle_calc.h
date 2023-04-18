//========================================
// 
// �p�[�e�B�N���̌v�Z�̃w�b�_�t�@�C��
// author KEISUKE OOTONO
// 
//========================================
// *** particle_calc.h ***
//========================================
#ifndef _PARTICLE_CALC_H_
#define _PARTICLE_CALC_H_  
#include "../main.h"
#include "particle.h"
#include "particle_type.h"
//****************************************
// �\���̂̒�`
//****************************************

// �x�N�g��
struct Vector3 {
	double x, y, z;
	// �R���X�g���N�^
	Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	// �x�N�g���̑傫����Ԃ��֐�
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}
	// �x�N�g����P�ʃx�N�g���ɐ��K������֐�
	void normalize() {
		double len = length();
		x /= len;
		y /= len;
		z /= len;
	}
};
//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���x�v�Z
void SpeedCalc(Particle *pPar, Particle_Sub *pParSub,ParticleType *pPType, int nCntPar);
// �����x�N�g����@���x�N�g���ɕϊ�����֐�
Vector3 directionToNormal(const Vector3& dir);
// 3�����x�N�g���̊O�ς��v�Z����֐�
Vector3 cross(const Vector3& v1, const Vector3& v2);
#endif