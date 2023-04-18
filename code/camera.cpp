//========================================
// �J�����̏��� 
// Author:��a�c��
//========================================
#include "camera.h"
#include "input.h"
//========================================
// �O���[�o���ϐ�
//========================================
Camera g_camera[MAX_CAMERA];		//�J�����̏��

float ra, rb, rc;
//========================================
// ����������
//========================================
void InitCamera(void)
{
	for (int nCntCma = 0; nCntCma < MAX_CAMERA; nCntCma++)
	{
		if (nCntCma == 0)
		{
			g_camera[nCntCma].posV = D3DXVECTOR3(0.0f, 210.0f, -340.0f);	//�J�����̈ʒu 0 110 -240
			g_camera[nCntCma].posR = D3DXVECTOR3(0.0f, 110.0f, -100.0f);	//����ꏊ
			g_camera[nCntCma].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_camera[nCntCma].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
			g_camera[nCntCma].spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//��]��
			g_camera[nCntCma].viewport.X = 0.0f;
			g_camera[nCntCma].viewport.Y = 0.0f;
			g_camera[nCntCma].viewport.Width = 1280.0f;
			g_camera[nCntCma].viewport.Height = 720.0f;
			g_camera[nCntCma].bUse = true;
			g_camera[nCntCma].bFog = true;
		}

		g_camera[nCntCma].viewport.MinZ = 0.0;
		g_camera[nCntCma].viewport.MaxZ = 1.0f;
		ra = DEFAULT_FLOA;
		rb = DEFAULT_FLOA;
		rc = DEFAULT_FLOA;
	}
	
	
}
//========================================
// �I������
//========================================
void UninitCamera(void)
{

}
//****************************************
// �X�V����
//****************************************
void UpdateCamera(void)
{
	for (int nCntCma = 0; nCntCma < MAX_CAMERA; nCntCma++)
	{
		g_camera[nCntCma].posOldV = g_camera[nCntCma].posV;
		g_camera[nCntCma].posOldR = g_camera[nCntCma].posR;

		ra = g_camera[nCntCma].posV.x - g_camera[nCntCma].posR.x;
		rb = g_camera[nCntCma].posV.z - g_camera[nCntCma].posR.z;
		g_camera[nCntCma].length = sqrtf(ra * ra + rb * rb);

		//�㉺�ړ�
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (-DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (-DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (-DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (-DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (-DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (-DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (-DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (-DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
			else
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (-BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (-BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (-BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (-BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (DIAGONALLY_UP - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (DIAGONALLY_DOWN - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
			else
			{
				g_camera[0].posV.x += sinf(D3DX_PI * (BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posV.z += cosf(D3DX_PI * (BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

				g_camera[0].posR.x += sinf(D3DX_PI * (BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
				g_camera[0].posR.z += cosf(D3DX_PI * (BESIDE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{
			g_camera[0].posV.x += sinf(D3DX_PI * (ABOVE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			g_camera[0].posV.z += cosf(D3DX_PI * (ABOVE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

			g_camera[0].posR.x += sinf(D3DX_PI * (ABOVE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			g_camera[0].posR.z += cosf(D3DX_PI * (ABOVE - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_camera[0].posV.x += sinf(D3DX_PI * (UNDER - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			g_camera[0].posV.z += cosf(D3DX_PI * (UNDER - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

			g_camera[0].posR.x += sinf(D3DX_PI * (UNDER - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;
			g_camera[0].posR.z += cosf(D3DX_PI * (UNDER - g_camera[nCntCma].rot.z)) * CAMERA_SPEED;

		}

		//���_�E�㉺�ړ�
		if (GetKeyboardPress(DIK_G) == true)
		{
			g_camera[0].posV.y += cosf(D3DX_PI * ABOVE) * CAMERA_SPEED1;
			g_camera[0].posR.y += cosf(D3DX_PI * ABOVE) * CAMERA_SPEED1;
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{
			g_camera[0].posV.y += cosf(D3DX_PI * UNDER) * CAMERA_SPEED1;
			g_camera[0].posR.y += cosf(D3DX_PI * UNDER) * CAMERA_SPEED1;
		}

		//���̉�]�E���_
		if (GetKeyboardPress(DIK_Z) == true)
		{
			g_camera[0].rot.z -= ROT_SPEED;

			g_camera[0].posV.x = g_camera[0].posR.x + sinf(D3DX_PI * (1.0f - g_camera[0].rot.z)) * g_camera[0].length;
			g_camera[0].posV.z = g_camera[0].posR.z + cosf(D3DX_PI * (1.0f - g_camera[0].rot.z)) * g_camera[0].length;

		}
		else if (GetKeyboardPress(DIK_C) == true)
		{
			g_camera[0].rot.z += ROT_SPEED;

			g_camera[0].posV.x = g_camera[0].posR.x + sinf(D3DX_PI * (1.0f - g_camera[0].rot.z)) * g_camera[0].length;
			g_camera[0].posV.z = g_camera[0].posR.z + cosf(D3DX_PI * (1.0f - g_camera[0].rot.z)) * g_camera[0].length;
		}

		//���̉�]�E�����_
		if (GetKeyboardPress(DIK_Q) == true)
		{
			g_camera[0].rot.z -= ROT_SPEED;

			g_camera[0].posR.x = g_camera[0].posV.x + sinf(D3DX_PI * g_camera[0].rot.z) * g_camera[0].length;
			g_camera[0].posR.z = g_camera[0].posV.z + cosf(D3DX_PI * g_camera[0].rot.z) * g_camera[0].length;

		}
		else if (GetKeyboardPress(DIK_E) == true)
		{
			g_camera[0].rot.z += 0.01f;

			g_camera[0].posR.x = g_camera[0].posV.x + sinf(D3DX_PI * g_camera[0].rot.z) * g_camera[0].length;
			g_camera[0].posR.z = g_camera[0].posV.z + cosf(D3DX_PI * g_camera[0].rot.z) * g_camera[0].length;
		}

	}
}
//****************************************
// �ݒ菈��
//****************************************
void SetCamera(int nldx)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&g_camera[nldx].viewport);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nldx].mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera[nldx].mtxProjection,
		D3DXToRadian(36.0f),						/* ����p */
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	/*��ʂ̃A�X�y�N�g��*/
		10.0f,										/*Z�l�̍ŏ��l*/
		5000.0f);									/*Z�l�̍ő�l*/

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nldx].mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera[nldx].mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera[nldx].mtxView,
		&g_camera[nldx].posV,			/*���_*/
		&g_camera[nldx].posR,			/*�����_*/
		&g_camera[nldx].vecU);			/*������x�N�g��*/

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nldx].mtxView);
}
Camera *Getcamara(int nCntCamara)
{
	return &g_camera[nCntCamara];
}