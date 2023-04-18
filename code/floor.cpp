//========================================
// ���̏���
// Author:KEISUKE OTONO
//========================================
#include "floor.h"
//========================================
// �O���[�o���ϐ�
//========================================
LPDIRECT3DTEXTURE9		g_pTextureFloor = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;			//���_�o�b�t�@�|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffFloor = NULL;			//���_�o�b�t�@�|�C���^
D3DXMATRIX g_mtxWorldFloor;//���[���h�}�g���b�N�X

Floor g_Floor[NUM_FLOOR];
//========================================
// ����������
//========================================
void InitFloor(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\floor001.png",
		&g_pTextureFloor);

	g_Floor[0].pos = D3DXVECTOR3(0.0f, -20.0f, 100.0f);
	g_Floor[0].rot = INIT_D3DXVECTOR3;
	g_Floor[0].fWidhtX = 30;
	g_Floor[0].fWidhtZ = 30;
	g_Floor[0].nBlock_X = 20;
	g_Floor[0].nBlock_Z = 20;
	g_Floor[0].nVertex = (g_Floor[0].nBlock_Z + 1) * (g_Floor[0].nBlock_X + 1);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Floor[0].nVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFloor,
		NULL);

	//�ݒ�p�|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxBuffFloor->Lock(0, 0, (void **)&pVtx, 0);

	// ���_���W/�@���x�N�g��/�e�N�X�`�����W�̐ݒ�	

	for (int nCntHeight = 0; nCntHeight < g_Floor[0].nBlock_X + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < g_Floor[0].nBlock_Z + 1; nCntWidth++)
		{
			pVtx[(nCntHeight * (g_Floor[0].nBlock_Z + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
				((float)nCntHeight - (float)g_Floor[0].nBlock_X / 2) * g_Floor[0].fWidhtX,
					0.0f,
					((float)nCntWidth - (float)g_Floor[0].nBlock_Z / 2) * g_Floor[0].fWidhtZ);

			pVtx[(nCntHeight * (g_Floor[0].nBlock_Z + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					0.0f,
					1.0f,
					0.0f);

			pVtx[(nCntHeight * (g_Floor[0].nBlock_Z + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntHeight * ((float)g_Floor[0].nBlock_Z / (float)g_Floor[0].nBlock_Z),
					nCntWidth * ((float)g_Floor[0].nBlock_X / (float)g_Floor[0].nBlock_X));
		}
	}

	for (int nCntVtx = 0; nCntVtx < ((g_Floor[0].nBlock_Z + 1) * (g_Floor[0].nBlock_X + 1)); nCntVtx++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffFloor->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_Floor[0].nBlock_X * 2 * (g_Floor[0].nBlock_Z + 1)) + ((g_Floor[0].nBlock_X - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffFloor,
		NULL);

	WORD *pIdx;//�C���f�b�N�X���ւ̃|�C���^

			   //�C���f�b�N�X�o�b�t�@�����b�N�������_�ԍ��f�[�^�ւ̃|�C���^������
	g_pIdxBuffFloor->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;	// �C���f�b�N�X�̃J�E���g

						//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntDepth = 0; nCntDepth < g_Floor[0].nBlock_X; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < g_Floor[0].nBlock_Z + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((g_Floor[0].nBlock_Z + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((g_Floor[0].nBlock_Z + 1) * nCntDepth);
			nCntIdx += 2;
		}

		if (nCntDepth < g_Floor[0].nBlock_X - 1)
		{
			pIdx[nCntIdx] = ((g_Floor[0].nBlock_Z + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (g_Floor[0].nBlock_Z + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffFloor->Unlock();
}
//========================================
// �I������
//========================================
void UninitFloor(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureFloor != NULL)
	{
		g_pTextureFloor->Release();
		g_pTextureFloor = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffFloor != NULL)
	{
		g_pIdxBuffFloor->Release();
		g_pIdxBuffFloor = NULL;
	}
}
//========================================
// �X�V����
//========================================
void UpdateFloor(void)
{

}
//========================================
// �`�揈��
//========================================
void DrawFloor(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

												//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldFloor);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Floor[0].rot.y,
		g_Floor[0].rot.x, g_Floor[0].rot.z);

	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Floor[0].pos.x, g_Floor[0].pos.y, g_Floor[0].pos.z);

	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFloor);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffFloor);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFloor);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_Floor[0].nVertex,//�p�ӂ������_�̐�
		0,
		((g_Floor[0].nBlock_Z * g_Floor[0].nBlock_X) * 2) + (4 * (g_Floor[0].nBlock_X - 1)));//�`�悷��v���~�e�B�u�̐�
}