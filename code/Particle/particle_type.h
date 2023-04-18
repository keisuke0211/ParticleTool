//========================================
// 
// �p�[�e�B�N���̎�ނ̃w�b�_�t�@�C��
// author KEISUKE OOTONO
// 
//========================================
// *** particle_type.h ***
//========================================
#ifndef _PARTICLE_TYPE_H_
#define _PARTICLE_TYPE_H_  
#include "../main.h"
#include "particle.h"
//****************************************
// �}�N����`
//****************************************
#define PARTICLE_TYPE		(240)			//�p�[�e�B�N����ނ̍ő吔
//========================================
// �񋓌^�̒�`
//========================================
typedef enum
{
	SHAPE_POINT = 0, // �_
	SHAPE_CIRCLE,    // �~
	SHAPE_SPHERE,    // ��
	SHAPE_MAX,
}SHAPE;
//****************************************
// �\���̂̒�`
//****************************************
// �p�[�e�B�N���E�`��
typedef struct
{
	int nFadeTime;		// �t�F�[�h����
	int nAlphaType;		// �A���t�@���
	bool bLight;		// ���C�e�B���O�̗L��������
} ParticleDraw1;

// �F
typedef struct
{
	D3DXCOLOR col;		// �F
	D3DXCOLOR colRandamMax;		// �����_���ȐF�͈̔�(�ő�)
	D3DXCOLOR colRandamMin;		// �����_���ȐF�͈̔�(�ŏ�)
	bool bColRandom;			// �����_���ŐF��ύX���邩
} Color1;

// �~
typedef struct
{
	int nDir;			// �����i�����E�����j
	int nLay;			// �z�u���@
	float fRadius;	// �~�̔��a
} Circle1;

// ��
typedef struct
{
	int Sepalate;		// ������
	int Split;			// ������
	float fRadius;	// ���̔��a
	int nFormType;		// �������@
} Sphere1;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu�i�ݒ�ʒu�j
	D3DXVECTOR3 Cotpos;	// �ʒu�i�A���Đ��j
	D3DXVECTOR3 rot;	// ����
	D3DXVECTOR3 move;	// �ړ���
	Color1 aColor;		// �F

						/* ���C�� */
	int Form;			// ��x�ɐ�������p�[�e�B�N����
	int nM_Life;		// ����
	int nType;			// �摜���
						/* �`�� */
	ParticleDraw1 aDraw;

	/* �A���Đ� */
	bool bCot;			// �A���Đ��̗L��
	int nCotIdx;		// �Đ�����p�[�e�B�N���ԍ�

	/* ���x */
	float fAttenuation;	// ������

	/* �`�� */
	int nShape;				// �`��
	Circle1 aCircle;		// �~
	Sphere1 aSphere;		// ��

	/* �T�u */
	int sType;			// ���x�v�Z
	int nS_Life;		// ����
	float fRadius10;	// ���a�i�傫���j

	bool bUse;			// �g�p�t���O
} ParticleType;//parts
//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitParticleType(void);

// �c�[���p
void LoadParticleType(void);
void SaveParticleType(void);

ParticleType *GetParticleType(void);

#endif