//========================================
// 
// imGui�̃w�b�_�t�@�C��
// author KEISUKE OOTONO
// 
//========================================
#ifndef _IMGUI_MAIN_H_
#define _IMGUI_MAIN_H_  
#include "main.h"

//****************************************
// ���\����
//****************************************

// �G�t�F�N�g�̍\����
typedef struct
{
	int nIdx;		// �Đ�����p�[�e�B�N���ԍ�
	int nCntTime;	// ����
	int nCntLoop;	// ���[�v�Ԋu
	bool bLoop;		// ���[�v�Đ����Ă��邩�ǂ���
	bool bStop;		// �ꎞ��~
} ParticleSystem;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitImguiMain(HWND hWnd);
void UninitImguiMain(void);
void UpdateImguiMain(void);
void DrawImguiMain(void);
ParticleSystem *GetParticleSystem(void);
#endif