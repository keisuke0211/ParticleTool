//========================================
// 
// �G�t�F�N�g�̃w�b�_�t�@�C��
// author KEISUKE OOTONO
// 
//========================================
//  *** effect.h ***
//========================================
#ifndef _EFFECT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFFECT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����
#include "../main.h"
#include "particle.h"
#include "particle_type.h"
//****************************************
// �}�N����`
//****************************************
#define EFFECT_DATA_FILE_PATH (".\\data\\GAMEDATA\\EFFECT\\EFFECT_DATA.txt")
#define MAX_EFFECT	(10240)			// �G�t�F�N�g�̍ő吔
#define NUM_EFFECT	(100)			// �G�t�F�N�g�̎��
#define MAX_M_LIFE	(60)			// ���C�������̍ő�l
#define MAX_S_LIFE	(300)			// �T�u�����̍ő�l
//****************************************
// �񋓌^
//****************************************
enum ALPHABLENDTYPE
{
	TYPE_ADD = 0,		// ���Z
	TYPE_SUB,			// ���Z
	TYPE_NONE,			// ��Z
	MAX_TYPE,
};
//****************************************
// ���\����
//****************************************

// �G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3	rot;		// ����
	D3DXVECTOR3 move;		// �ړ���
	D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXCOLOR col;			// ���_�J���[
	float fRadius;			// ���a�i�傫���j
	int nLife;				// �����i�\�����ԁj
	int nInitLife;			// �����̎���
	int nType;				// �摜���
	int nAlphaType;			// �A���t�@���
	int nShape;				// 
	/* �`�� */
	int nFadeTime;			// �t�F�[�h���鎞��
	/* �A���Đ� */
	bool bCot;				// �A���Đ��̗L��
	int nCotIdx;			// �Đ�����p�[�e�B�N���ԍ�

	bool bLight;			// ���C�e�B���O�̗L��������
	bool bUse;				// �g�p���Ă��邩�ǂ���
} Effect;

// �G�t�F�N�g�̍\����
typedef struct
{
	char aName[TXT_MAX];		// �摜��
	char aFileName[TXT_MAX];	// �t�@�C����
	bool bData;					// �f�[�^�̗L��
} EffectType;
// �G�t�F�N�g�̍\����
typedef struct
{
	char aName[TXT_MAX];		// �摜��
} Type;
//****************************************
// �N���X
//****************************************
class CEffect
{
public:// �\����
	typedef struct
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXVECTOR3 move;		// �ړ���
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXCOLOR col;			// ���_�J���[
		float fRadius;			// ���a�i�傫���j
		int nLife;				// �����i�\�����ԁj
		int nInitLife;			// �����̎���
		int nType;				// �摜���
		int nAlphaType;			// �A���t�@���
		int nShape;				// 
								/* �`�� */
		int nFadeTime;			// �t�F�[�h���鎞��
								/* �A���Đ� */
		bool bCot;				// �A���Đ��̗L��
		int nCotIdx;			// �Đ�����p�[�e�B�N���ԍ�

		bool bLight;			// ���C�e�B���O�̗L��������
		bool bUse;				// �g�p���Ă��邩�ǂ���
	} Info;

public:// �֐�
	CEffect();		// �R���X�g���N�^
	~CEffect();		// �f�X�g���N�^
	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V
	void Draw();	// �`��
	void Load();	// �ǂݍ���
	void Set(Particle_Sub *pParSub, Particle *pPar);// �ݒ�
private:// �֐�
	void Fade(Info *pInfo);	// �t�F�[�h

public:// �ϐ�
	Info m_Info[MAX_EFFECT];	// ���
};
//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void LoadEffect(void);
void SetEffect(Particle_Sub *pParSub, Particle *pPar);
Effect	*GetEffect(void);
EffectType *GetEffectType(void);
Type *GetType(void);
// �ǉ����鎖
// ��U���̒���
// �}���`�e�N�X�`��
// �d�͂̒ǉ�
// ���b�V���G�t�F�N�g�@�~
// ���b�V���G�t�F�N�g�@��
// ���b�V���G�t�F�N�g�@�~��
// �O�ՃG�t�F�N�g�̗L��
// ���x�����̗L��

/* �]�T������� */
// �C���X�^���V���O�̎����i�`����ꊇ�ōs�������j

#endif