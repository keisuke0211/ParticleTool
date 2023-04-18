//****************************************
//	wall.h
//	Author:KEISUKE OTONO
//****************************************
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

//****************************************
// �}�N����`
//****************************************
#define NUM_WALL			(1)			//���b�V���̔z�u��
//****************************************
// �\���̂̒�`
//****************************************

//���b�V���t�B�[���h�E���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	int	nBlock_X;				//���̃u���b�N��
	int nBlock_Y;				//�c�̃u���b�N��
	int nVertex;				//���_��
	int nIdx;					//�C���f�b�N�X��
	int nPolygon;				//�|���S����

	float			fWidhtX;	//����
	float			fWidhtY;	//����
}Wall;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitWall (void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

#endif 