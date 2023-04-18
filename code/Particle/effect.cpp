//========================================
// 
// �G�t�F�N�g�̏���
// author KEISUKE OOTONO
// 
//========================================
// *** effect.cpp ***
//========================================
#include "effect.h"
#include <stdio.h>

//========================================
// �v���g�^�C�v�錾
//========================================
void EffectFade(Effect *pEffect);	// �t�F�[�h����
//========================================
// �O���[�o���ϐ�
//========================================
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;						//���_�o�b�t�@�|�C���^
static LPDIRECT3DTEXTURE9		g_pTextureEffect[NUM_EFFECT] = {};			//�e�N�X�`���ւ̃|�C���^
static Effect g_aEffect[MAX_EFFECT];										// �G�t�F�N�g�̍\����
static EffectType g_aEffectType[NUM_EFFECT];								// �G�t�F�N�g��ނ̍\����sta
static Type g_aType[NUM_EFFECT];											// �摜���̍\����
static int	g_nEffectNum;													// �G�t�F�N�g�̐�
// �G�t�F�N�g�̏��̎擾
Effect	*GetEffect(void)
{
	return g_aEffect;
}
// �G�t�F�N�g��ނ̎擾
EffectType *GetEffectType(void)
{
	return g_aEffectType;
}
// �摜���̎擾
Type *GetType(void)
{
	return g_aType;
}
//========================================
// �G�t�F�N�g�̏���������
// author KEISUKE OOTONO
//========================================
void InitEffect(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/* �e�N�X�`���̐ݒ� */
	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEffectType[nCntTexture].aFileName,
			&g_pTextureEffect[nCntTexture]);
	}

	/* �G�t�F�N�g�̏��̏��������� */
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = INIT_D3DXVECTOR3;
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 6.0f;
		g_aEffect[nCntEffect].nLife = 25;
		g_aEffect[nCntEffect].nInitLife = 25;
		g_aEffect[nCntEffect].nAlphaType = 0;
		g_aEffect[nCntEffect].nCotIdx = 0;
		g_aEffect[nCntEffect].nFadeTime = 0;
		g_aEffect[nCntEffect].bCot = false;
		g_aEffect[nCntEffect].bLight = false;
		g_aEffect[nCntEffect].bUse = false;
	}

	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		// �摜�����G�t�F�N�g��ނ̉摜���ɃR�s�[����
		memcpy(&g_aType[nCntTexture].aName, &g_aEffectType[nCntTexture].aName, sizeof(g_aEffectType[nCntTexture].aName));
	}
	/* ���_�o�b�t�@�̐��� */
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	/* �ݒ�p�|�C���^ */
	VERTEX_3D *pVtx;

	/* ���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾 */
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		/* ���_���W�̐ݒ� */
		pVtx[0].pos = INIT_D3DXVECTOR3;
		pVtx[1].pos = INIT_D3DXVECTOR3;
		pVtx[2].pos = INIT_D3DXVECTOR3;
		pVtx[3].pos = INIT_D3DXVECTOR3;
		
		/* rhw�̐ݒ� */
		pVtx[0].nor = INIT_RHW;
		pVtx[1].nor = INIT_RHW;
		pVtx[2].nor = INIT_RHW;
		pVtx[3].nor = INIT_RHW;
		
		/* ���_�J���[ */
		pVtx[0].col = INIT_D3DXCOLOR;
		pVtx[1].col = INIT_D3DXCOLOR;
		pVtx[2].col = INIT_D3DXCOLOR;
		pVtx[3].col = INIT_D3DXCOLOR;

		/* �e�N�X�`�����W */
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	/* ���_�o�b�t�@�̔j�� */
	g_pVtxBuffEffect->Unlock();
}
//========================================
// �G�t�F�N�g�̏I������
// author KEISUKE OOTONO
//========================================
void UninitEffect(void)
{
	/* �e�N�X�`���̔j�� */
	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		if (g_pTextureEffect[nCntTexture] != NULL)
		{
			g_pTextureEffect[nCntTexture]->Release();
			g_pTextureEffect[nCntTexture] = NULL;
		}
	}

	
	/* ���_�o�b�t�@�̔j�� */
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//========================================
// �G�t�F�N�g�̍X�V����
// author KEISUKE OOTONO
//========================================
void UpdateEffect(void)
{
	Particle *pPar = GetParticle();
	ParticleType *pPType = GetParticleType();
	Effect *pEffect = GetEffect();

	/* �e�N�X�`�����W�̍X�V */
	VERTEX_3D *pVtx;

	/* ���_�o�b�t�@�̃��b�N */
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++,pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//�G�t�F�N�g���g�p����Ă���

			// X�EZ�̈ړ��ʂɂ����銄��
			float fRateXZ = 0.0f;
			// Y�̈ړ��ʂɂ����銄��
			float fRateY = 0.0f;

			// ���x����
			pEffect->move.x += (0.0f - pEffect->move.x) * 0.05f;
			pEffect->move.y += (0.0f - pEffect->move.y) * 0.05f;
			pEffect->move.z += (0.0f - pEffect->move.z) * 0.05f;


			/* �G�t�F�N�̈ʒu�X�V */
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;
			pEffect->pos.z += pEffect->move.z;

			// �t�F�[�h����
			EffectFade(pEffect);

			/* ���_���W�̐ݒ� */
			pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, -0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, -0.0f);

			/* ���_�J���[�̐ݒ� */
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			/* �����Ƒ傫�������炵�Ă��� */
			pEffect->nLife--;

			if (pEffect->nLife == 0)
			{//�������s����

				if (pEffect->bCot == true)
				{
					SetParticle(pEffect->pos, pEffect->nCotIdx);
					pEffect->bCot = false;
				}

				pEffect->bUse = false;

				
			}
		}
	}
	/* ���_�o�b�t�@�̃A�����b�N */
	g_pVtxBuffEffect->Unlock();
}
//========================================
// �G�t�F�N�g�̕`�揈��
// author KEISUKE OOTONO
//========================================
void DrawEffect(void)
{
	/* �f�o�C�X�̃|�C���^ */
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans, mtxRot;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	/* �f�o�C�X�̏��� */
	pDevice = GetDevice();

	/* �f�[�^�X�g���[���̐ݒ� */

	/* ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ� */
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

	/* ���_�t�H�[�}�b�g�̐ݒ� */
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		/* Z�e�X�g�𖳌��ɂ��� */
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���C�e�B���O��L���E�����ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, g_aEffect[nCntEffect].bLight);

		// �A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		switch (g_aEffect[nCntEffect].nAlphaType)
		{
		case TYPE_NONE:	// ��Z
			break;

		case TYPE_ADD:	// ���Z
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case TYPE_SUB:	// ���Z
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		}

		for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
		{
			if (g_aEffect[nCntEffect].bUse)
			{//�G�t�F�N�g���g�p����Ă���

				 /* ���[���h�}�g���b�N�X�̏����� */
				D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

				//�r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
				D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);//�t�s������߂�
				g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
				g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
				g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

				/* �ʒu�𔽉f */
				D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

				D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

				/* ���[���h�}�g���b�N�X�̐ݒ� */
				pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

				/* �e�N�X�`���̐ݒ� */
				pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

				/* �|���S���̕`�� */
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
			}
		}
		/* Z�e�X�g��L���ɂ��� */
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//�@���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �A���t�@�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

		/* ���u�����f�B���O�����ɖ߂� */
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//========================================
// �G�t�F�N�g�̐ݒ菈��
// author KEISUKE OOTONO
//========================================
void SetEffect(Particle_Sub *pParSub, Particle *pPar)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pParSub->pos;
			g_aEffect[nCntEffect].move = pParSub->move;
			g_aEffect[nCntEffect].col = pParSub->col;
			g_aEffect[nCntEffect].fRadius = pParSub->fRadius;
			g_aEffect[nCntEffect].nLife = pParSub->nLife;
			g_aEffect[nCntEffect].nInitLife = pPar->aDraw.nFadeTime;
			g_aEffect[nCntEffect].nType = pPar->nType;
			g_aEffect[nCntEffect].nCotIdx = pPar->nCotIdx;

			/* �`�� */
			g_aEffect[nCntEffect].nAlphaType = pPar->aDraw.nAlphaType;
			g_aEffect[nCntEffect].nFadeTime = pPar->aDraw.nFadeTime;
			/* �A���Đ� */
			g_aEffect[nCntEffect].bCot = pPar->bCot;

			g_aEffect[nCntEffect].bLight = pPar->aDraw.bLight;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
	}
}
//========================================
// �G�t�F�N�g�摜�̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//========================================
void LoadEffect(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch [TXT_MAX];	// �f�[�^�����p
	EffectType *pEffect = g_aEffectType;


	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		g_aEffectType[nCntTexture].bData = false;
	}
	// �G�t�F�N�g����������
	g_nEffectNum = 0;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(EFFECT_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
	 //�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END"))
		{// �ǂݍ��݂��I��
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// �܂�Ԃ�
			continue;
		}
		
		if (g_nEffectNum == NUM_EFFECT)
		{// �������I��
			fclose(pFile);
			break;
		}
		if (!strcmp(aDataSearch, "EFFECT")) {
			fscanf(pFile, "%s", &pEffect->aFileName);	// �t�@�C����
			fscanf(pFile, "%s", &pEffect->aName);		// �摜��
			pEffect->bData = true;
			pEffect++;				// �|�C���^��i�߂�
			g_nEffectNum++;			// �G�t�F�N�g�������Z
		}
	}
}
//========================================
// �G�t�F�N�g�t�F�[�h����
// author KEISUKE OOTONO
//========================================
void EffectFade(Effect *pEffect)
{
	// �t�F�[�h�J�n���邩
	if (pEffect->nLife <= pEffect->nFadeTime)
	{
		/* �T�C�Y */
		pEffect->fRadius -= pEffect->fRadius / pEffect->nFadeTime;

		/* �s�����x�X�V */
		pEffect->col.a *= ((float)pEffect->nFadeTime / (float)pEffect->nInitLife);

		pEffect->nFadeTime--;
	}
}