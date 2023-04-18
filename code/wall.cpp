//========================================
// �ǂ̏���
// Author:KEISUKE OTONO
//========================================
#include "wall.h"
//========================================
// �O���[�o���ϐ�
//========================================
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//���_�o�b�t�@�|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;			//���_�o�b�t�@�|�C���^
D3DXMATRIX g_mtxWorldWall;//���[���h�}�g���b�N�X

Wall g_Wall[NUM_WALL];
//========================================
// ����������
//========================================
void InitWall(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\floor001.png",
		&g_pTextureWall);

	g_Wall[0].pos = D3DXVECTOR3(0.0f, 10.0f, 350.0f);
	g_Wall[0].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Wall[0].fWidhtX = 30;
	g_Wall[0].fWidhtY = 30;
	g_Wall[0].nBlock_X = 20;
	g_Wall[0].nBlock_Y = 20;
	g_Wall[0].nVertex = (g_Wall[0].nBlock_X + 1) * (g_Wall[0].nBlock_Y + 1);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Wall[0].nVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//�ݒ�p�|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	// ���_���W/�@���x�N�g��/�e�N�X�`�����W�̐ݒ�	

	for (int nCntHeight = 0; nCntHeight < g_Wall[0].nBlock_Y + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < g_Wall[0].nBlock_X + 1; nCntWidth++)
		{
			pVtx[(nCntHeight * (g_Wall[0].nBlock_X + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
				((float)nCntHeight - (float)g_Wall[0].nBlock_X / 2) * g_Wall[0].fWidhtX,
					((float)nCntWidth - (float)g_Wall[0].nBlock_Y / 2) * g_Wall[0].fWidhtY,
					0.0f);

			pVtx[(nCntHeight * (g_Wall[0].nBlock_X + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					0.0f,
					1.0f,
					0.0f);

			pVtx[(nCntHeight * (g_Wall[0].nBlock_X + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntWidth * ((float)g_Wall[0].nBlock_X / (float)g_Wall[0].nBlock_X),
					nCntHeight * ((float)g_Wall[0].nBlock_Y / (float)g_Wall[0].nBlock_Y));
		}
	}

	for (int nCntVtx = 0; nCntVtx < ((g_Wall[0].nBlock_X + 1) * (g_Wall[0].nBlock_Y + 1)); nCntVtx++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffWall->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_Wall[0].nBlock_Y * 2 * (g_Wall[0].nBlock_X + 1)) + ((g_Wall[0].nBlock_Y - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffWall,
		NULL);

	WORD *pIdx;//�C���f�b�N�X���ւ̃|�C���^

			   //�C���f�b�N�X�o�b�t�@�����b�N�������_�ԍ��f�[�^�ւ̃|�C���^������
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;	// �C���f�b�N�X�̃J�E���g

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < g_Wall[0].nBlock_Y; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < g_Wall[0].nBlock_X + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((g_Wall[0].nBlock_X + 1) * (nCntHeight + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((g_Wall[0].nBlock_X + 1) * nCntHeight);
			nCntIdx += 2;
		}

		if (nCntHeight < g_Wall[0].nBlock_Y - 1)
		{
			pIdx[nCntIdx] = ((g_Wall[0].nBlock_X + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (g_Wall[0].nBlock_X + 1) * (nCntHeight + 2);
			nCntIdx += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffWall->Unlock();
}
//========================================
// �I������
//========================================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}
}
//========================================
// �X�V����
//========================================
void UpdateWall(void)
{

}
//========================================
// �`�揈��
//========================================
void DrawWall(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

												//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldWall);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[0].rot.y,
		g_Wall[0].rot.x, g_Wall[0].rot.z);

	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Wall[0].pos.x, g_Wall[0].pos.y, g_Wall[0].pos.z);

	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffWall);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureWall);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_Wall[0].nVertex,//�p�ӂ������_�̐�
		0,
		((g_Wall[0].nBlock_X * g_Wall[0].nBlock_Y) * 2) + (4 * (g_Wall[0].nBlock_Y - 1)));//�`�悷��v���~�e�B�u�̐�
}