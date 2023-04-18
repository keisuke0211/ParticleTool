//========================================
// meain.h
// Author:KEISUKE OTONO
//========================================

#ifndef _MAIN_H_
#define _MAIN_H_             

#include<windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include"XInput.h"
#include"d3dx9.h"  //�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0X0800) //�r���h���̌x���Ώ��p�̃}�N��
#include"dinput.h"//���͏����ɕK�v//�Q�d�C���N���[�h�X�q�̃}�N�����`����

/* ingui */
#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_internal.h"
#include <d3d9.h>
#include <tchar.h>
//========================================
// ���C�u����
//========================================

#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�����������ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	//���͏����ɕK�v
#pragma comment(lib,"Opengl32.lib")	//�t�H�O�����ɕK�v
#include"xaudio2.h"					//�T�E���h�����ɕK�v
//========================================
// �}�N����`
//========================================
#define SCREEN_WIDTH		(1280.0f)														// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720.0f)														// �E�C���h�E�̍����@��main.cpp����ړ�
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					// ���_�t�H�[�}�b�g�y2D�z
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g�y3D�z
#define SWITCH_TIME			(600)															// �؂�ւ�鎞��(1�b = 60)�@���ꕔ�̉�ʐ؂�ւ��Ɏd�l
#define WINDOW_MODE			(false)															// �E�C���h�E���[�h�̐؂�ւ�
#define DEFAULT_INT			(0)																// int�̏����l
#define DEFAULT_FLOA		(0.0f)															// float�̏����l
#define INIT_RHW			D3DXVECTOR3(0.0f,1.0f,0.0f)										// D3DXVECTOR3�̏�����
#define INIT_D3DXVECTOR3	D3DXVECTOR3(0.0f,0.0f,0.0f)										// D3DXVECTOR3�̏����l
#define INIT_D3DXCOLOR		D3DXCOLOR(1,1,1,1)												// D3DXCOLOR�̏����l
#define DIAGONALLY_UP		(0.25f)		//�΂ߏ�
#define DIAGONALLY_DOWN		(0.75f)		//�΂߉�
#define BESIDE				(0.5f)		//��
#define ABOVE				(0.0f)		//��
#define UNDER				(1.0f)		//��
#define TXT_MAX				(256)		// ������̍ő吔(�ėp)
//========================================
// ���\����
//========================================

//���_���y2D�z�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��W���i1.0f�ŌŒ�j
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_2D;
//���_���y3D�z�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_3D;

// ���C�������̏��\����
typedef struct
{
	HWND				hWnd;		// �E�C���h�E�n���h��(���ʎq)
	D3DXVECTOR3			windowPos;	// �E�C���h�E�̈ʒu
}Main;

//========================================
// �v���g�^�C�v�錾
//========================================
LPDIRECT3DDEVICE9 GetDevice(void);
void WindowMode(bool bScreen);								// �E�C���h�E�̃��[�h��؂�ւ���
HWND *GetWindowHandle(void);
D3DXVECTOR3 GetWindowPos(void);

#endif