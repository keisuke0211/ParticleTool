//========================================
// 
// �p�[�e�B�N����ނ̏���
// author KEISUKE OOTONO
// 
//========================================
// *** particle_type.cpp ***
//========================================
#include "particle_type.h"
//****************************************
// �}�N����`
//****************************************
#define FILE_PATH ("data")	// �p�[�e�B�N���f�[�^�̃t�@�C���p�X
//****************************************
// �v���g�^�C�v�錾
//****************************************
void LoadColor(FILE *pFile, ParticleType *pPType);	// �F���̓ǂݍ���
void LoadDraw(FILE *pFile, ParticleType *pPType);	// �`����̓ǂݍ���
void LoadSpeed(FILE *pFile, ParticleType *pPType);	// ���x���̓ǂݍ���
void LoadLife(FILE *pFile, ParticleType *pPType);	// �������̓ǂݍ���


//****************************************
// �O���[�o���ϐ�
//****************************************
static ParticleType g_aParticleType[PARTICLE_TYPE];		// �p�[�e�B�N����ނ̏��

//========== *** �p�[�e�B�N���E��ނ̏����擾 ***
ParticleType *GetParticleType(void)
{
	return g_aParticleType;
}

//****************************************
// �p�[�e�B�N���̏���������
// author KEISUKE OOTONO
//****************************************
void InitParticleType(void)
{
	ParticleType *pPType = GetParticleType();

	// �p�[�e�B�N���̎��
	for (int nCntPar = 0; nCntPar < PARTICLE_TYPE; nCntPar++, pPType++)
	{
		pPType->pos = INIT_D3DXVECTOR3;
		pPType->Cotpos = INIT_D3DXVECTOR3;
		pPType->rot = INIT_D3DXVECTOR3;
		pPType->move = INIT_D3DXVECTOR3;
		/* �F */
		pPType->aColor.col = INIT_D3DXCOLOR;
		pPType->aColor.colRandamMax = INIT_D3DXCOLOR;
		pPType->aColor.colRandamMin = INIT_D3DXCOLOR;
		pPType->aColor.bColRandom = false;

		/* ���C�� */
		pPType->Form = 1;
		pPType->nShape = 0;
		pPType->nM_Life = 1;
		pPType->nType = 11;
		pPType->aDraw.nAlphaType = 0;
		pPType->aDraw.bLight = false;
		/* �A���Đ� */
		pPType->bCot = false;
		pPType->nCotIdx = 0;
		/* �T�u */
		pPType->sType = 0;
		pPType->nS_Life = 60;
		pPType->fRadius10 = 3.0f;

		/* �~ */
		pPType->aCircle.nDir = 0;
		pPType->aCircle.nLay = 0;
		pPType->aCircle.fRadius = 10;

		/* �� */
		pPType->aSphere.Sepalate = 8;
		pPType->aSphere.Split = 8;
		pPType->aSphere.fRadius = 1;
		pPType->aSphere.nFormType = 0;

		pPType->bUse = false;
	}
}
//****************************************
// �p�[�e�B�N���̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//****************************************
void LoadParticleType(void)
{
	HWND *pHwnd = GetWindowHandle();

	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];

	// �t�@�C���I���̃_�C�A���O�\��
	if (szPath[0] == TEXT('\0'))
	{
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = *pHwnd;
		ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
		ofn.lpstrFile = szFile;				// �I���t�@�C���i�[
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = TEXT("�e�L�X�g����(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("�t�@�C�����J��");
		ofn.Flags = OFN_FILEMUSTEXIST;
	}
	// �J��(O)�n�{�^���������ꂽ��
	if (GetOpenFileName(&ofn))
	{
		// �p�[�e�B�N���E��ނ̏��
		ParticleType *pPType = GetParticleType();
		char aDataSearch[TXT_MAX];		// �f�[�^�����p
		FILE *pFile;					// �t�@�C���̃|�C���^

		// �t�@�C�����J��
		pFile = fopen(szFile, "r");

		if (pFile == NULL)
		{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		 //�������I������
			return;
		}
			
			// END_PARTICLE ��������܂œǂݍ��݂��J��Ԃ�
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// ����

				if (!strcmp(aDataSearch, "END_PARTICLE"))
				{// �ǂݍ��݂��I��
					fclose(pFile);
					break;
				}
				else if (aDataSearch[0] == '#')
				{// �܂�Ԃ�
					continue;
				}
				else if (!strcmp(aDataSearch, "PARTICLESET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (!strcmp(aDataSearch, "END_PARTICLESET")) { break; }									// �ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "PLAY:"))		{ fscanf(pFile, "%d", &pPType->bCot); }		// �A���Đ��̗L��
						else if (!strcmp(aDataSearch, "PLAY_IDX:")) { fscanf(pFile, "%d", &pPType->nCotIdx); }	// �Đ�����p�[�e�B�N���ԍ�
						else if (!strcmp(aDataSearch, "POS:")) {												// �ʒu
							fscanf(pFile, "%f", &pPType->pos.x);	// X
							fscanf(pFile, "%f", &pPType->pos.y);	// Y
							fscanf(pFile, "%f", &pPType->pos.z);	// Z
						}
						else if (!strcmp(aDataSearch, "ROT:")) {												// ����
							fscanf(pFile, "%f", &pPType->rot.x);	// X
							fscanf(pFile, "%f", &pPType->rot.y);	// Y
							fscanf(pFile, "%f", &pPType->rot.z);	// Z
						}
						else if (!strcmp(aDataSearch, "SHAPE:")) { fscanf(pFile, "%d", &pPType->nShape); }		// �`��

						/* �~ */
						else if (!strcmp(aDataSearch, "CIRCLE"))
						{
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����
									 if (!strcmp(aDataSearch, "END_CIRCLE")) { break; }												// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "DIR:"))		{ fscanf(pFile, "%d", &pPType->aCircle.nDir); }			// ����
								else if (!strcmp(aDataSearch, "LAY:"))		{ fscanf(pFile, "%d", &pPType->aCircle.nLay); }			// �z�u���@
								else if (!strcmp(aDataSearch, "RADIUS:"))	{ fscanf(pFile, "%.2f", &pPType->aCircle.fRadius); }	// ���a
							}
						}
						/* �� */
						else if (!strcmp(aDataSearch, "SPHERE"))
						{
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����
									 if (!strcmp(aDataSearch, "END_SPHERE")) { break; }												// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "FORMTYPE:")) { fscanf(pFile, "%d", &pPType->aSphere.nFormType); }	// �������@
								else if (!strcmp(aDataSearch, "RADIUS:"))	{ fscanf(pFile, "%f", &pPType->aSphere.fRadius); }	// ���a
								else if (!strcmp(aDataSearch, "SEPALATE:")) { fscanf(pFile, "%d", &pPType->aSphere.Sepalate); }		// ������
								else if (!strcmp(aDataSearch, "SPLIT:"))	{ fscanf(pFile, "%d", &pPType->aSphere.Split); }		// ������
							}
						}

						else if (!strcmp(aDataSearch, "COLOR")){ LoadColor(pFile, pPType); }						// �F�@�@�ݒ�
						else if (!strcmp(aDataSearch, "DRAW"))	{ LoadDraw(pFile, pPType); }						// �`��@�ݒ�
						else if (!strcmp(aDataSearch, "SPEED")) { LoadSpeed(pFile, pPType); }						// ���x�@�ݒ�
						else if (!strcmp(aDataSearch, "FORM:"))	{ fscanf(pFile, "%d", &pPType->Form); }				// ������
						else if (!strcmp(aDataSearch, "RADIUS:")) { fscanf(pFile, "%f", &pPType->fRadius10); }		// ���a
						else if (!strcmp(aDataSearch, "FADE:")) { fscanf(pFile, "%d", &pPType->aDraw.nFadeTime); }	// �t�F�[�h����
						else if (!strcmp(aDataSearch, "LIFE"))	{ LoadLife(pFile, pPType); }						// �����@�ݒ�

					}

					pPType->bUse = true;
					pPType++;
				}

			}
	}
}
//****************************************
// �p�[�e�B�N���̏����o������
// author KEISUKE OOTONO
//****************************************
void SaveParticleType(void)
{
	HWND *pHwnd = GetWindowHandle();

	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];

	// �t�@�C���I���̃_�C�A���O�\��
	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = *pHwnd;
		ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
		ofn.lpstrFile = szFile;				// �I���t�@�C���i�[
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".txt");
		ofn.lpstrFilter = TEXT("�e�L�X�g����(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("���O��t���ĕۑ�");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	// �m�ۑ�(S)�n�{�^���������ꂽ��
	if (GetSaveFileName(&ofn))
	{
		// �p�[�e�B�N���E��ނ̏��
		ParticleType *pPType = GetParticleType();
		FILE *pFile; // �t�@�C���̃|�C���^

		// �t�@�C�����J��
		pFile = fopen(szFile, "w");

		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ

			// �p�[�e�B�N��������������
			{

				fprintf(pFile, "PARTICLE\n\n");

				for (int nCntPar = 0; nCntPar < PARTICLE_TYPE; nCntPar++, pPType++)
				{
					if (pPType->bUse)
					{
						fprintf(pFile, "# %03d ------------------------------------------------\n",nCntPar);

						fprintf(pFile, "PARTICLESET\n");

						// �A���Đ�
						fprintf(pFile, "	PLAY:		%d\n", pPType->bCot);
						fprintf(pFile, "	PLAY_IDX:	%d\n", pPType->nCotIdx);

						// �ʒu
						fprintf(pFile, "	POS:		%.1f %.1f %.1f\n", pPType->pos.x, pPType->pos.y, pPType->pos.z);

						// ����
						fprintf(pFile, "	ROT:		%.1f %.1f %.1f\n", pPType->rot.x, pPType->rot.y, pPType->rot.z);

						// �`��
						{
							fprintf(pFile, "	SHAPE:			%d\n", pPType->nShape);

							switch (pPType->nShape)
							{
							case SHAPE_POINT:
							{

							}
							break;
							case SHAPE_CIRCLE:
							{
								fprintf(pFile, "	CIRCLE\n");
								fprintf(pFile, "		DIR:	%d\n", pPType->aCircle.nDir);
								fprintf(pFile, "		LAY:	%d\n", pPType->aCircle.nLay);
								fprintf(pFile, "		RADIUS:	%.2f\n", pPType->aCircle.fRadius);
								fprintf(pFile, "	END_CIRCLE\n");

							}
							break;
							case SHAPE_SPHERE:
							{
								fprintf(pFile, "	SPHERE\n");
								fprintf(pFile, "		FORMTYPE:	%d\n", pPType->aSphere.nFormType);
								fprintf(pFile, "		SEPALATE:	%d\n", pPType->aSphere.Sepalate);
								fprintf(pFile, "		SPLIT:		%d\n", pPType->aSphere.Split);
								fprintf(pFile, "		RADIUS:		%.2f\n", pPType->aSphere.fRadius);
								fprintf(pFile, "	END_SPHERE\n");
							}
							break;
							}
						}

						// �F
						{
							fprintf(pFile, "	COLOR\n");
							fprintf(pFile, "		RANDAM:		%d\n", pPType->aColor.bColRandom);
							fprintf(pFile, "		MAINCOLOR:	%.2f %.2f %.2f %.2f\n", pPType->aColor.col.r, pPType->aColor.col.g, pPType->aColor.col.b, pPType->aColor.col.a);
							fprintf(pFile, "		COLORMAX:	%.2f %.2f %.2f %.2f\n", pPType->aColor.colRandamMax.r, pPType->aColor.colRandamMax.g, pPType->aColor.colRandamMax.b, pPType->aColor.colRandamMax.a);
							fprintf(pFile, "		COLORMIN:	%.2f %.2f %.2f %.2f\n", pPType->aColor.colRandamMin.r, pPType->aColor.colRandamMin.g, pPType->aColor.colRandamMin.b, pPType->aColor.colRandamMin.a);
							fprintf(pFile, "	END_COLOR\n");
						}

						// �`��
						{
							fprintf(pFile, "	DRAW\n");
							fprintf(pFile, "		ALPHA_TYPE:	%d\n", pPType->aDraw.nAlphaType);
							fprintf(pFile, "		LIGHT:		%d\n", pPType->aDraw.bLight);
							fprintf(pFile, "	END_DRAW\n");
						}

						// �ړ����x
						{
							fprintf(pFile, "	SPEED\n");
							fprintf(pFile, "		TYPE:	%d\n", pPType->sType);
							fprintf(pFile, "		MOVE:	%.1f %.1f %.1f\n", pPType->move.x, pPType->move.y, pPType->move.z);
							fprintf(pFile, "	END_SPEED\n");
						}

						// ������
						{
							fprintf(pFile, "	FORM:	%d\n", pPType->Form);
						}

						// ���a
						{
							fprintf(pFile, "	RADIUS:	%.1f\n", pPType->fRadius10);
						}

						// �t�F�[�h
						{
							fprintf(pFile, "	FADE:	%d\n", pPType->aDraw.nFadeTime);
						}

						// ����
						{
							fprintf(pFile, "	LIFE\n");
							fprintf(pFile, "		MAIN:	%d\n", pPType->nM_Life);
							fprintf(pFile, "		SABU:	%d\n", pPType->nS_Life);
							fprintf(pFile, "	END_LIFE\n");
						}

						fprintf(pFile, "END_PARTICLESET\n\n");
					}
				}
				fprintf(pFile, "END_PARTICLE\n");
			}

			// �t�@�C�������
			fclose(pFile);
		}
		else
		{// �t�@�C�����J���Ȃ������ꍇ

		}
	}
}
//****************************************
// �p�[�e�B�N���E�F�̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//****************************************
void LoadColor(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// �f�[�^�����p

	// �F ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

			 if (!strcmp(aDataSearch, "END_COLOR"))	{ break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "RANDAM:")) { fscanf(pFile, "%d", &pPType->aColor.bColRandom); }	// �����_���J���[�̗L��
		else if (!strcmp(aDataSearch, "MAINCOLOR:")){ 			// ���C���J���[
			fscanf(pFile, "%f", &pPType->aColor.col.r);
			fscanf(pFile, "%f", &pPType->aColor.col.g);
			fscanf(pFile, "%f", &pPType->aColor.col.b);
			fscanf(pFile, "%f", &pPType->aColor.col.a);
		}
		else if (!strcmp(aDataSearch, "COLORMAX:")) {			// �����_���J���[�̍ő�l
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.r);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.g);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.b);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.a);
		}
		else if (!strcmp(aDataSearch, "COLORMIN:")) { 			// �����_���J���[�̍Œ�l
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.r);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.g);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.b);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.a);
		}
	}
}
//****************************************
// �p�[�e�B�N���E�`��̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//****************************************
void LoadDraw(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// �f�[�^�����p

	// �`�� ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (!strcmp(aDataSearch, "END_DRAW"))			{ break; }											// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ALPHA_TYPE:"))	{ fscanf(pFile, "%d", &pPType->aDraw.nAlphaType); }	// �A���t�@���
		else if (!strcmp(aDataSearch, "LIGHT:"))		{ fscanf(pFile, "%d", &pPType->aDraw.bLight); }		// ���C�e�B���O�̗L��
	}
}
//****************************************
// �p�[�e�B�N���E���x�̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//****************************************
void LoadSpeed(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// �f�[�^�����p

	// ���x ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

			 if (!strcmp(aDataSearch, "END_SPEED"))	{ break; }									// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "TYPE:"))		{ fscanf(pFile, "%d", &pPType->sType); }	// ���x�v�Z
		else if (!strcmp(aDataSearch, "MOVE:"))		{ 											// ���x
			fscanf(pFile, "%f", &pPType->move.x);
			fscanf(pFile, "%f", &pPType->move.y);
			fscanf(pFile, "%f", &pPType->move.z);
		}
	}
}
//****************************************
// �p�[�e�B�N���E�����̓ǂݍ��ݏ���
// author KEISUKE OOTONO
//****************************************
void LoadLife(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// �f�[�^�����p

	// ���� ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

			 if (!strcmp(aDataSearch, "END_LIFE"))	{ break; }									// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "MAIN:"))		{ fscanf(pFile, "%d", &pPType->nM_Life); }	// ���C��
		else if (!strcmp(aDataSearch, "SABU:"))		{ fscanf(pFile, "%d", &pPType->nS_Life); }	// �T�u
	}
}