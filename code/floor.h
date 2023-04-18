//****************************************
//	floor.h
//	Author:KEISUKE OTONO
//****************************************
#ifndef _FLOOR_H_
#define _FLOOR_H_
#include "main.h"

//****************************************
// �}�N����`
//****************************************
#define NUM_FLOOR			(1)			//���b�V���̔z�u��
//****************************************
// �\���̂̒�`
//****************************************

//���b�V���t�B�[���h�E���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	int	nBlock_Z;				//���̃u���b�N��
	int nBlock_X;				//�c�̃u���b�N��
	int nVertex;				//���_��
	int nIdx;					//�C���f�b�N�X��
	int nPolygon;				//�|���S����

	float			fWidhtX;	//�c��
	float			fWidhtZ;	//����
}Floor;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);

#endif 