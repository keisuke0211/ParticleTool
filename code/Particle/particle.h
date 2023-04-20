//========================================
// 
// �p�[�e�B�N���̃w�b�_�t�@�C��
// author KEISUKE OOTONO
// 
//========================================
//  *** particle.h ***
//========================================
#ifndef _PARTICLE_H_		//���̃}�N����`������ĂȂ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "../main.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define MAX_PARTICLE		(120)			//�p�[�e�B�N���̍ő吔
#define PARTICLE_SUB		(2560)			//�T�u�p�[�e�B�N������o������̍ő吔

// �p�[�e�B�N���N���X

class CParticle
{
public:	// �񋓌^
public: // �\����

	//-----------------------------------------------------------------------------
	// �G�t�F�N�g�J���[�̍\����
	// Author  : KEISUKE OOTONO
	// outline : �G�t�F�N�g�̐F�֌W�̍\����
	//-----------------------------------------------------------------------------
	struct Color
	{
		D3DXCOLOR col;				// �F
		D3DXCOLOR colRandamMax;		// �����_���ȐF�͈̔�(�ő�)
		D3DXCOLOR colRandamMin;		// �����_���ȐF�͈̔�(�ŏ�)
		bool bColRandom;			// �����_���ŐF��ύX���邩
	};

	//-----------------------------------------------------------------------------
	// �G�t�F�N�g�`��̍\����
	// Author  : KEISUKE OOTONO
	// outline : �G�t�F�N�g�̕`��֌W�̍\����
	//-----------------------------------------------------------------------------
	struct Draw
	{
		int nFadeTime;		// �t�F�[�h����
		int nAlphaType;		// �A���t�@���
		bool bLight;		// ���C�e�B���O�̗L��������
	};

	//-----------------------------------------------------------------------------
	// �~�̍\����
	// Author  : KEISUKE OOTONO
	// outline : �p�[�e�B�N���E�`��i�~�j�֌W�̍\����
	//-----------------------------------------------------------------------------
	struct Circle
	{
		int nDir;			// �����i�����E�����j
		int nLay;			// �z�u���@
		float fRadius01;	// �~�̔��a
	};

	//-----------------------------------------------------------------------------
	// ���̍\����
	// Author  : KEISUKE OOTONO
	// outline : �p�[�e�B�N���E�`��i���j�֌W�̍\����
	//-----------------------------------------------------------------------------
	struct Sphere
	{
		D3DXVECTOR3 Setpos;	// �ʒu�i�ݒ�ʒu�j
		int Sepalate;		// ������
		int Split;			// ������
		float fRadius02;	// ���̔��a
		int nFormType;		// �������@
	};

	//-----------------------------------------------------------------------------
	// �p�[�e�B�N�����̍\����
	// Author  : KEISUKE OOTONO
	// outline : �p�[�e�B�N���̍\����
	//-----------------------------------------------------------------------------
	struct Info
	{
		/* �\���� */
		Draw aDraw;		// �`��
		Color aColor;	// �J���[
		Circle aCircle;	// �~
		Sphere aSphere;	// ��

		/* �񋓌^ */

	};
public:	// �֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
public:	// �ϐ�
	Info m_Info;
};

//****************************************
// �\���̂̒�`
//****************************************

// �p�[�e�B�N���E�`��
typedef struct
{
	int nFadeTime;		// �t�F�[�h����
	int nAlphaType;		// �A���t�@���
	bool bLight;		// ���C�e�B���O�̗L��������
} ParticleDraw;

// �F
typedef struct
{
	D3DXCOLOR col;				// �J���[
	D3DXCOLOR colRandamMax;		// �����_���ȐF�͈̔�(�ő�)
	D3DXCOLOR colRandamMin;		// �����_���ȐF�͈̔�(�ŏ�)
	bool bColRandom;			// �����_���ŐF��ύX���邩

} Color;

// �~
typedef struct
{
	int nDir;			// �����i�����E�����j
	int nLay;			// �z�u���@
	float fRadius;		// �~�̔��a
} Circle;

// ��
typedef struct
{
	D3DXVECTOR3 Setpos;	// �ʒu�i�ݒ�ʒu�j
	int Sepalate;		// ������
	int Split;			// ������
	float fRadius;		// ���̔��a
	int nFormType;		// �������@
} Sphere;
//========================================
//�p�[�e�B�N���E���C���̍\����
//========================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu�i�����ʒu�j
	D3DXVECTOR3 rot;	// ��]
	D3DXVECTOR3 speed;	// �ړ���(x.y.z)
	Color aColor;		// �F
	int nLife;			// ����
	int nType;			// �摜���
	int Form;			// ��x�ɐ�������p�[�e�B�N����
	ParticleDraw aDraw;	// �`��
	/* �A���Đ� */
	bool bCot;			// �A���Đ��̗L��
	int nCotIdx;		// �Đ�����p�[�e�B�N���ԍ�
	/* �`�� */
	int nShape;			// �`����
	Circle aCircle;		// �~
	Sphere aSphere;		// ��
	
	/* �T�u */
	int nMoveType;		// �ړ����
	int nS_Life;		// ����
	float fAnglePos;	// �p�x�i�ʒu�p�j
	float fRadius;		// ���a�i�傫���j

	bool bSet;			// ����������
	bool bUse;			// �g�p���Ă��邩�ǂ���

} Particle;

//========================================
//�p�[�e�B�N���E�T�u�̍\����
//========================================

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu�i�����ʒu�j
	D3DXVECTOR3 rot;	// ����
	D3DXVECTOR3 move;	// �ړ���
	D3DXCOLOR col;		// �F

	int nLife;			// ����
	int nIdx;			// ���C���p�[�e�B�N���̔ԍ�

	float fRot;			// �p�x�i�ړ��p�j
	float fRadius;		// ���a�i�傫���j
	bool bUse;			// �g�p���Ă��邩�ǂ���
} Particle_Sub;




//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos,int nIdx);

Particle		*GetParticle(void);
Particle_Sub	*GetParticle_Sub(void);
#endif