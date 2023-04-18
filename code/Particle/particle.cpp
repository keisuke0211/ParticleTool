//========================================
// 
// �p�[�e�B�N���̏���
// author KEISUKE OOTONO
// 
//========================================
// *** Particle.cpp ***
//========================================
#include "particle.h"
#include "Effect.h"
#include "particle_calc.h"
#include "particle_type.h"
#include "assert.h"
//****************************************
// �v���g�^�C�v�錾
//****************************************
void SetCol(Particle *pPar);													// �F�̐ݒ�
void SetCommon(Particle *pPar,ParticleType *pPType, int nIdx);					// ���ʐݒ�
/* �T�u�p�[�e�B�N�� */
void SetSubParticle(Particle *pPar,int nCntApe);								// �ݒ�
void DelSebParticle(int nCntPar);												// �폜
void SetSphere(Particle *pPar,int nCntPar);										// �� ��������
//****************************************
// �O���[�o���ϐ�
//****************************************
static Particle		g_aParticle[MAX_PARTICLE];			// �p�[�e�B�N���̏��
static Particle_Sub	g_aParticleSub[PARTICLE_SUB];		// �T�u�p�[�e�B�N���̏��

//========== *** �p�[�e�B�N���̏����擾 ***
Particle *GetParticle(void)
{
	return g_aParticle;
}
//========== *** �p�[�e�B�N���E�T�u�̏����擾 ***
Particle_Sub *GetParticle_Sub(void)
{
	return g_aParticleSub;
}
//****************************************
// �p�[�e�B�N���̏���������
// author KEISUKE OOTONO
//****************************************
void InitParticle(void)
{
	Particle *pPar = g_aParticle;
	Particle_Sub *pParSub = g_aParticleSub;

	/* �\���̂̏����� */
	
	// ���C���p�[�e�B�N��
	for (int nCntPar = 0; nCntPar < MAX_PARTICLE; nCntPar++, pPar++)
	{
		pPar->pos = INIT_D3DXVECTOR3;
		pPar->rot = INIT_D3DXVECTOR3;
		pPar->speed = INIT_D3DXVECTOR3;
		/* �F */
		pPar->aColor.col = INIT_D3DXCOLOR;
		pPar->aColor.colRandamMax = INIT_D3DXCOLOR;
		pPar->aColor.colRandamMin = INIT_D3DXCOLOR;
		pPar->aColor.bColRandom = false;
		pPar->nLife = 0;
		pPar->nType = 11;
		pPar->Form = 0;
		pPar->nShape = 0;
		pPar->nMoveType = 0;
		pPar->nS_Life = 0;
		pPar->fAnglePos = 0.0f;

		pPar->fRadius = 0.0f;
		pPar->bSet = false;
		pPar->bUse = false;
		pPar->aDraw.bLight = false;
		/* �`�� */
		pPar->aDraw.nAlphaType = 0;
		pPar->aDraw.nFadeTime = 0;
		/* �A���Đ� */
		pPar->bCot = false;
		pPar->nCotIdx = 0;
		/* �~ */
		pPar->aCircle.nDir = 0;
		pPar->aCircle.nLay = 0;
		pPar->aCircle.fRadius = 10;
		/* �� */
		pPar->aSphere.Sepalate = 8;
		pPar->aSphere.Split = 8;
		pPar->aSphere.fRadius = 3;
		pPar->aSphere.nFormType = 0;

	}

	// �T�u�p�[�e�B�N��
	for (int nCntParSu = 0; nCntParSu < PARTICLE_SUB; nCntParSu++, pParSub++)
	{
		pParSub->pos = INIT_D3DXVECTOR3;
		pParSub->rot = INIT_D3DXVECTOR3;
		pParSub->fRot = 0;
		pParSub->move = INIT_D3DXVECTOR3;
		pParSub->col = INIT_D3DXCOLOR;
		pParSub->nLife = 0;
		pParSub->fRadius = 0.0f;
		pParSub->nIdx = 0;
		pParSub->bUse = false;
	}
}
//****************************************
// �p�[�e�B�N���̏I������
// author KEISUKE OOTONO
//****************************************
void UninitParticle(void)
{

}
//****************************************
// �p�[�e�B�N���̍X�V����
// author KEISUKE OOTONO
//****************************************
void UpdateParticle(void)
{
	Particle *pPar = g_aParticle;
	Particle_Sub *pParSub = g_aParticleSub;
	ParticleType *pPType = GetParticleType();

	for (int nCntPar = 0; nCntPar < MAX_PARTICLE; nCntPar++, pPar++)
	{
		if (pPar->bUse)
		{
			if (pPar->bSet)
			{
				for (int nCntSub = 0; nCntSub < PARTICLE_SUB; nCntSub++, pParSub++)
				{
					if (g_aParticleSub[nCntSub].bUse && g_aParticleSub[nCntPar].nIdx == nCntPar)
					{
						//�G�t�F�N�g�ݒ�
						SetEffect(pParSub, pPar);
					}
				}
			}
			else if (pPar->bSet == false)
			{
				// �F�̐ݒ�
				SetCol(pPar);

				if (pPar->nShape == 2)
				{
					// ���̐�������
					SetSphere(pPar,nCntPar);
				}
				else
				{
					for (int nCntForm = 0; nCntForm < pPar->Form; nCntForm++)
					{
						if (pPar->nShape == 1)
						{
							// �Ԋu�z�u
							if (pPar->aCircle.nLay == 0)
							{
								pPar->fAnglePos = ((D3DX_PI * 2) / (float)pPar->Form) * nCntForm;
							}
							// �����_���z�u
							if (pPar->aCircle.nLay == 1)
							{
								pPar->fAnglePos = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
							}
						}
						// �T�u�p�[�e�B�N���̐ݒ�
						SetSubParticle(pPar, nCntPar);
					}
				}
				g_aParticle[nCntPar].bSet = true;
			}

			//�����̌���
			pPar->nLife--;
			if (pPar->nLife <= 0)
			{
				pPar->bUse = false;
				pPar->bSet = false;

				// �T�u�p�[�e�B�N���̍폜
				DelSebParticle(nCntPar);
			}
		}
	}
}

//****************************************
// �p�[�e�B�N���̕`�揈��
// author KEISUKE OOTONO
//****************************************
void DrawParticle(void)
{

}
//****************************************
// �p�[�e�B�N���̐ݒ菈��
// author KEISUKE OOTONO
//****************************************
void SetParticle(D3DXVECTOR3 pos, int nIdx)
{
	Particle *pPar = g_aParticle;
	ParticleType *pPType = GetParticleType();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pPar++)
	{
		if (pPar->bUse == false)
		{
			pPar->pos = pos;

			//���ʐݒ�
			SetCommon(pPar,pPType,nIdx);

			/* �A���Đ� */
			pPar->nCotIdx = pPType[nIdx].nCotIdx;

			pPar->bUse = true;
			break;
		}
	}
}

//****************************************
// �T�u�p�[�e�B�N���̐ݒ菈��
// author KEISUKE OOTONO
//****************************************
void SetSubParticle(Particle *pPar, int nCntApe)
{
	Particle_Sub *pParSub = g_aParticleSub;
	ParticleType *pPType = GetParticleType();

	for (int nCntSub = 0; nCntSub < PARTICLE_SUB; nCntSub++, pParSub++)
	{
		if (pParSub->bUse == false)
		{
			// ���C���̔ԍ�����
			pParSub->nIdx = nCntApe;

			//�ʒu�̐ݒ�
			switch (pPar->nShape)
			{
			// �_
			case 0:
			{
				pParSub->pos = pPar->pos;
			}
			break;
			// �c�̉~
			case 1:
			{
				// ����
				if (pPar->aCircle.nDir == 0)
				{
					pParSub->pos = D3DXVECTOR3(
						pPar->pos.x + sinf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.y + cosf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.z);
				}
				// ����
				else if (pPar->aCircle.nDir == 1)
				{
					pParSub->pos = D3DXVECTOR3(
						pPar->pos.x + sinf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.y,
						pPar->pos.z + cosf(pPar->fAnglePos) * pPar->aCircle.fRadius);
				}

			}
			break;
			// ��
			case 2:
			{
				pParSub->pos = pPar->aSphere.Setpos;

				// �p�x�̕����Ɉړ�����
				pParSub->move = pParSub->pos - pPar->pos;
			}
			break;
			}

			if (pPar->nShape == 2)
			{


			}
			else
			{
				// ���x�v�Z
				SpeedCalc(pPar, pParSub, pPType, pParSub->nIdx);
				pParSub->move = pParSub->move;
			}

			/* �����E���a */
			pParSub->fRadius = pPar[nCntApe].fRadius;
			pParSub->nLife = pPar[nCntApe].nS_Life;
			/* �F */
			pParSub->col = pPar->aColor.col;

			pParSub->bUse = true;

			//�G�t�F�N�g�ݒ�
			SetEffect(pParSub,pPar);

			break;
		}
	}
}
//****************************************
// �p�[�e�B�N���E���ʐݒ菈��
// author KEISUKE OOTONO
//****************************************
void SetCommon(Particle *pPar, ParticleType *pPType,int nIdx)
{
	// �x�����W�A���ɕϊ�
	pPar->rot.x = (pPType[nIdx].rot.x * (D3DX_PI / 180)) - D3DX_PI;
	pPar->rot.y = (pPType[nIdx].rot.y * (D3DX_PI / 180)) - D3DX_PI;
	pPar->rot.z = (pPType[nIdx].rot.z * (D3DX_PI / 180)) - D3DX_PI;

	pPar->speed = pPType[nIdx].move;
	pPar->nShape = pPType[nIdx].nShape;
	pPar->nMoveType = pPType[nIdx].sType;
	pPar->nType = pPType[nIdx].nType;
	pPar->nLife = pPType[nIdx].nM_Life;
	pPar->nS_Life = pPType[nIdx].nS_Life;
	pPar->fRadius = pPType[nIdx].fRadius10;
	pPar->Form = pPType[nIdx].Form;
	pPar->bUse = true;

	/* �F */
	pPar->aColor.col = pPType[nIdx].aColor.col;
	pPar->aColor.bColRandom = pPType[nIdx].aColor.bColRandom;
	pPar->aColor.colRandamMin = pPType[nIdx].aColor.colRandamMin;
	pPar->aColor.colRandamMin = pPType[nIdx].aColor.colRandamMax;
	/* �`�� */
	pPar->aDraw.nFadeTime = pPType[nIdx].aDraw.nFadeTime;
	pPar->aDraw.nAlphaType = pPType[nIdx].aDraw.nAlphaType;
	pPar->aDraw.bLight = pPType[nIdx].aDraw.bLight;

	/* �A���Đ� */
	pPar->bCot = pPType[nIdx].bCot;

	/* �~ */
	pPar->aCircle.nDir = pPType[nIdx].aCircle.nDir;
	pPar->aCircle.nLay = pPType[nIdx].aCircle.nLay;
	pPar->aCircle.fRadius = pPType[nIdx].aCircle.fRadius;

	/* �� */
	pPar->aSphere.Sepalate = pPType[nIdx].aSphere.Sepalate;
	pPar->aSphere.Split = pPType[nIdx].aSphere.Split;
	pPar->aSphere.fRadius = pPType[nIdx].aSphere.fRadius;
	pPar->aSphere.nFormType = pPType[nIdx].aSphere.nFormType;
}
//****************************************
// �T�u�p�[�e�B�N���̍폜����
// author KEISUKE OOTONO
//****************************************
void DelSebParticle(int nCntPar)
{
	for (int nCntSub = 0; nCntSub < PARTICLE_SUB; nCntSub++)
	{
		if (g_aParticleSub[nCntSub].nIdx == nCntPar)
		{
			g_aParticleSub[nCntSub].bUse = false;
		}
	}
}
//******************************************
// ���p�[�e�B�N����������
// author KEISUKE OOTONO
//******************************************
void SetSphere(Particle *pPar, int nCntPar)
{
	float yRadian = 0.0f;		//�c�ŕ����������̉��P������Y���W�����܂�p�x

	//�ʒu�̐ݒ�
	switch (pPar->aSphere.nFormType)
	{
	case 0:
	{
		//�V�ʂ̏o�����蒸�_�̐ݒ�
		pPar->aSphere.Setpos = D3DXVECTOR3(pPar->pos.x, pPar->pos.y + pPar->aSphere.fRadius, pPar->pos.z);
		SetSubParticle(pPar, nCntPar);

		//�Q�Ԗڂ̒��_����A���̕������@�]�@��ʂ̏o������̂P���_�@�񐔕� for������
		for (int nCntDevideY = 1; nCntDevideY < pPar->aSphere.Sepalate; nCntDevideY++)
		{
			//---------------------------------------------------------------------------------------------------------
			//	MEMO�F�c�ɕ������鐔���g���Č��_���牡�P�����̍����܂ł̊p�x�v�Z (�������ŕ����_��������̂ŁA3.14
			//---------------------------------------------------------------------------------------------------------
			yRadian += D3DX_PI / pPar->aSphere.Sepalate;
			float TempLen = fabsf(sinf(yRadian) * pPar->aSphere.fRadius);	//X�EZ�̔��a
			float rot_Y = D3DX_PI;									//Y���̊p�x
			float Height_Y = cosf(yRadian) * pPar->aSphere.fRadius;		//Y�̍���
			for (int nCntDevideX = 0; nCntDevideX < pPar->aSphere.Split; nCntDevideX++)
			{//���P�����̒��_���W��ݒ�

				pPar->aSphere.Setpos =
					D3DXVECTOR3(
						pPar->pos.x + sinf(rot_Y) * TempLen,	//X�̈ʒu
						pPar->pos.y + Height_Y,					//Y�̈ʒu
						pPar->pos.z + cosf(rot_Y) * TempLen);	//Z�̈ʒu
																//�p�x���@�S�̂̊p�x���������Ŋ������������A����
				rot_Y -= (D3DX_PI*2.0f) / pPar->aSphere.Split;

				SetSubParticle(pPar, nCntPar);
			}
		}

		//��ʂ̏o�����蒸�_�̐ݒ�
		pPar->aSphere.Setpos = D3DXVECTOR3(pPar->pos.x, pPar->pos.y + -pPar->aSphere.fRadius, pPar->pos.z);
		SetSubParticle(pPar, nCntPar);
	}
	break;
	case 1:
	{
		for (int nCntForm = 0; nCntForm < pPar->Form; nCntForm++)
		{
			yRadian = (float)(rand() % ((int)(100 * 1 * D3DX_PI) + 1)) / (float)100;
			float TempLen = fabsf(sinf(yRadian) * pPar->aSphere.fRadius);
			float rot_Y = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
			float Height_Y = cosf(yRadian) * pPar->aSphere.fRadius;

			pPar->aSphere.Setpos =
				D3DXVECTOR3(
					pPar->pos.x + sinf(rot_Y) * TempLen,	//X�̈ʒu
					pPar->pos.y + Height_Y,					//Y�̈ʒu
					pPar->pos.z + cosf(rot_Y) * TempLen);	//Z�̈ʒu


			SetSubParticle(pPar, nCntPar);
		}
	}
	break;
	}
}
//****************************************
// �F�̐ݒ菈��
// author KEISUKE OOTONO
//****************************************
void SetCol(Particle *pPar)
{
	// �����_���J���[
	if (pPar->aColor.bColRandom)
	{
		pPar->aColor.col.r = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.r - pPar->aColor.colRandamMin.r)) + pPar->aColor.colRandamMin.r;
		pPar->aColor.col.g = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.g - pPar->aColor.colRandamMin.g)) + pPar->aColor.colRandamMin.g;
		pPar->aColor.col.b = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.b - pPar->aColor.colRandamMin.b)) + pPar->aColor.colRandamMin.b;
	}
}
//********************************************************************************
// �e�X�g
//********************************************************************************
HRESULT CParticle::Init()
{
	return S_OK;
}
void CParticle::Uninit()
{

}
void CParticle::Update()
{

}
void CParticle::Draw()
{

}