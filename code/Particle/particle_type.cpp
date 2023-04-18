//========================================
// 
// パーティクル種類の処理
// author KEISUKE OOTONO
// 
//========================================
// *** particle_type.cpp ***
//========================================
#include "particle_type.h"
//****************************************
// マクロ定義
//****************************************
#define FILE_PATH ("data")	// パーティクルデータのファイルパス
//****************************************
// プロトタイプ宣言
//****************************************
void LoadColor(FILE *pFile, ParticleType *pPType);	// 色情報の読み込み
void LoadDraw(FILE *pFile, ParticleType *pPType);	// 描画情報の読み込み
void LoadSpeed(FILE *pFile, ParticleType *pPType);	// 速度情報の読み込み
void LoadLife(FILE *pFile, ParticleType *pPType);	// 寿命情報の読み込み


//****************************************
// グローバル変数
//****************************************
static ParticleType g_aParticleType[PARTICLE_TYPE];		// パーティクル種類の情報

//========== *** パーティクル・種類の情報を取得 ***
ParticleType *GetParticleType(void)
{
	return g_aParticleType;
}

//****************************************
// パーティクルの初期化処理
// author KEISUKE OOTONO
//****************************************
void InitParticleType(void)
{
	ParticleType *pPType = GetParticleType();

	// パーティクルの種類
	for (int nCntPar = 0; nCntPar < PARTICLE_TYPE; nCntPar++, pPType++)
	{
		pPType->pos = INIT_D3DXVECTOR3;
		pPType->Cotpos = INIT_D3DXVECTOR3;
		pPType->rot = INIT_D3DXVECTOR3;
		pPType->move = INIT_D3DXVECTOR3;
		/* 色 */
		pPType->aColor.col = INIT_D3DXCOLOR;
		pPType->aColor.colRandamMax = INIT_D3DXCOLOR;
		pPType->aColor.colRandamMin = INIT_D3DXCOLOR;
		pPType->aColor.bColRandom = false;

		/* メイン */
		pPType->Form = 1;
		pPType->nShape = 0;
		pPType->nM_Life = 1;
		pPType->nType = 11;
		pPType->aDraw.nAlphaType = 0;
		pPType->aDraw.bLight = false;
		/* 連続再生 */
		pPType->bCot = false;
		pPType->nCotIdx = 0;
		/* サブ */
		pPType->sType = 0;
		pPType->nS_Life = 60;
		pPType->fRadius10 = 3.0f;

		/* 円 */
		pPType->aCircle.nDir = 0;
		pPType->aCircle.nLay = 0;
		pPType->aCircle.fRadius = 10;

		/* 球 */
		pPType->aSphere.Sepalate = 8;
		pPType->aSphere.Split = 8;
		pPType->aSphere.fRadius = 1;
		pPType->aSphere.nFormType = 0;

		pPType->bUse = false;
	}
}
//****************************************
// パーティクルの読み込み処理
// author KEISUKE OOTONO
//****************************************
void LoadParticleType(void)
{
	HWND *pHwnd = GetWindowHandle();

	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];

	// ファイル選択のダイアログ表示
	if (szPath[0] == TEXT('\0'))
	{
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = *pHwnd;
		ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
		ofn.lpstrFile = szFile;				// 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = TEXT("テキスト文書(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("ファイルを開く");
		ofn.Flags = OFN_FILEMUSTEXIST;
	}
	// 開く(O)］ボタンが押されたら
	if (GetOpenFileName(&ofn))
	{
		// パーティクル・種類の情報
		ParticleType *pPType = GetParticleType();
		char aDataSearch[TXT_MAX];		// データ検索用
		FILE *pFile;					// ファイルのポインタ

		// ファイルを開く
		pFile = fopen(szFile, "r");

		if (pFile == NULL)
		{// 種類毎の情報のデータファイルが開けなかった場合、
		 //処理を終了する
			return;
		}
			
			// END_PARTICLE が見つかるまで読み込みを繰り返す
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索

				if (!strcmp(aDataSearch, "END_PARTICLE"))
				{// 読み込みを終了
					fclose(pFile);
					break;
				}
				else if (aDataSearch[0] == '#')
				{// 折り返す
					continue;
				}
				else if (!strcmp(aDataSearch, "PARTICLESET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (!strcmp(aDataSearch, "END_PARTICLESET")) { break; }									// 読み込みを終了
						else if (!strcmp(aDataSearch, "PLAY:"))		{ fscanf(pFile, "%d", &pPType->bCot); }		// 連続再生の有無
						else if (!strcmp(aDataSearch, "PLAY_IDX:")) { fscanf(pFile, "%d", &pPType->nCotIdx); }	// 再生するパーティクル番号
						else if (!strcmp(aDataSearch, "POS:")) {												// 位置
							fscanf(pFile, "%f", &pPType->pos.x);	// X
							fscanf(pFile, "%f", &pPType->pos.y);	// Y
							fscanf(pFile, "%f", &pPType->pos.z);	// Z
						}
						else if (!strcmp(aDataSearch, "ROT:")) {												// 向き
							fscanf(pFile, "%f", &pPType->rot.x);	// X
							fscanf(pFile, "%f", &pPType->rot.y);	// Y
							fscanf(pFile, "%f", &pPType->rot.z);	// Z
						}
						else if (!strcmp(aDataSearch, "SHAPE:")) { fscanf(pFile, "%d", &pPType->nShape); }		// 形状

						/* 円 */
						else if (!strcmp(aDataSearch, "CIRCLE"))
						{
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索
									 if (!strcmp(aDataSearch, "END_CIRCLE")) { break; }												// 読み込みを終了
								else if (!strcmp(aDataSearch, "DIR:"))		{ fscanf(pFile, "%d", &pPType->aCircle.nDir); }			// 方向
								else if (!strcmp(aDataSearch, "LAY:"))		{ fscanf(pFile, "%d", &pPType->aCircle.nLay); }			// 配置方法
								else if (!strcmp(aDataSearch, "RADIUS:"))	{ fscanf(pFile, "%.2f", &pPType->aCircle.fRadius); }	// 半径
							}
						}
						/* 球 */
						else if (!strcmp(aDataSearch, "SPHERE"))
						{
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索
									 if (!strcmp(aDataSearch, "END_SPHERE")) { break; }												// 読み込みを終了
								else if (!strcmp(aDataSearch, "FORMTYPE:")) { fscanf(pFile, "%d", &pPType->aSphere.nFormType); }	// 生成方法
								else if (!strcmp(aDataSearch, "RADIUS:"))	{ fscanf(pFile, "%f", &pPType->aSphere.fRadius); }	// 半径
								else if (!strcmp(aDataSearch, "SEPALATE:")) { fscanf(pFile, "%d", &pPType->aSphere.Sepalate); }		// 分割数
								else if (!strcmp(aDataSearch, "SPLIT:"))	{ fscanf(pFile, "%d", &pPType->aSphere.Split); }		// 分割数
							}
						}

						else if (!strcmp(aDataSearch, "COLOR")){ LoadColor(pFile, pPType); }						// 色　　設定
						else if (!strcmp(aDataSearch, "DRAW"))	{ LoadDraw(pFile, pPType); }						// 描画　設定
						else if (!strcmp(aDataSearch, "SPEED")) { LoadSpeed(pFile, pPType); }						// 速度　設定
						else if (!strcmp(aDataSearch, "FORM:"))	{ fscanf(pFile, "%d", &pPType->Form); }				// 生成数
						else if (!strcmp(aDataSearch, "RADIUS:")) { fscanf(pFile, "%f", &pPType->fRadius10); }		// 半径
						else if (!strcmp(aDataSearch, "FADE:")) { fscanf(pFile, "%d", &pPType->aDraw.nFadeTime); }	// フェード時間
						else if (!strcmp(aDataSearch, "LIFE"))	{ LoadLife(pFile, pPType); }						// 寿命　設定

					}

					pPType->bUse = true;
					pPType++;
				}

			}
	}
}
//****************************************
// パーティクルの書き出し処理
// author KEISUKE OOTONO
//****************************************
void SaveParticleType(void)
{
	HWND *pHwnd = GetWindowHandle();

	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];

	// ファイル選択のダイアログ表示
	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = *pHwnd;
		ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
		ofn.lpstrFile = szFile;				// 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".txt");
		ofn.lpstrFilter = TEXT("テキスト文書(*.txt)\0*.txt\0");
		ofn.lpstrTitle = TEXT("名前を付けて保存");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	// ［保存(S)］ボタンが押されたら
	if (GetSaveFileName(&ofn))
	{
		// パーティクル・種類の情報
		ParticleType *pPType = GetParticleType();
		FILE *pFile; // ファイルのポインタ

		// ファイルを開く
		pFile = fopen(szFile, "w");

		if (pFile != NULL)
		{// ファイルが開けた場合

			// パーティクル情報を書き込み
			{

				fprintf(pFile, "PARTICLE\n\n");

				for (int nCntPar = 0; nCntPar < PARTICLE_TYPE; nCntPar++, pPType++)
				{
					if (pPType->bUse)
					{
						fprintf(pFile, "# %03d ------------------------------------------------\n",nCntPar);

						fprintf(pFile, "PARTICLESET\n");

						// 連続再生
						fprintf(pFile, "	PLAY:		%d\n", pPType->bCot);
						fprintf(pFile, "	PLAY_IDX:	%d\n", pPType->nCotIdx);

						// 位置
						fprintf(pFile, "	POS:		%.1f %.1f %.1f\n", pPType->pos.x, pPType->pos.y, pPType->pos.z);

						// 向き
						fprintf(pFile, "	ROT:		%.1f %.1f %.1f\n", pPType->rot.x, pPType->rot.y, pPType->rot.z);

						// 形状
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

						// 色
						{
							fprintf(pFile, "	COLOR\n");
							fprintf(pFile, "		RANDAM:		%d\n", pPType->aColor.bColRandom);
							fprintf(pFile, "		MAINCOLOR:	%.2f %.2f %.2f %.2f\n", pPType->aColor.col.r, pPType->aColor.col.g, pPType->aColor.col.b, pPType->aColor.col.a);
							fprintf(pFile, "		COLORMAX:	%.2f %.2f %.2f %.2f\n", pPType->aColor.colRandamMax.r, pPType->aColor.colRandamMax.g, pPType->aColor.colRandamMax.b, pPType->aColor.colRandamMax.a);
							fprintf(pFile, "		COLORMIN:	%.2f %.2f %.2f %.2f\n", pPType->aColor.colRandamMin.r, pPType->aColor.colRandamMin.g, pPType->aColor.colRandamMin.b, pPType->aColor.colRandamMin.a);
							fprintf(pFile, "	END_COLOR\n");
						}

						// 描画
						{
							fprintf(pFile, "	DRAW\n");
							fprintf(pFile, "		ALPHA_TYPE:	%d\n", pPType->aDraw.nAlphaType);
							fprintf(pFile, "		LIGHT:		%d\n", pPType->aDraw.bLight);
							fprintf(pFile, "	END_DRAW\n");
						}

						// 移動速度
						{
							fprintf(pFile, "	SPEED\n");
							fprintf(pFile, "		TYPE:	%d\n", pPType->sType);
							fprintf(pFile, "		MOVE:	%.1f %.1f %.1f\n", pPType->move.x, pPType->move.y, pPType->move.z);
							fprintf(pFile, "	END_SPEED\n");
						}

						// 生成数
						{
							fprintf(pFile, "	FORM:	%d\n", pPType->Form);
						}

						// 半径
						{
							fprintf(pFile, "	RADIUS:	%.1f\n", pPType->fRadius10);
						}

						// フェード
						{
							fprintf(pFile, "	FADE:	%d\n", pPType->aDraw.nFadeTime);
						}

						// 寿命
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

			// ファイルを閉じる
			fclose(pFile);
		}
		else
		{// ファイルが開けなかった場合

		}
	}
}
//****************************************
// パーティクル・色の読み込み処理
// author KEISUKE OOTONO
//****************************************
void LoadColor(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// データ検索用

	// 色 情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

			 if (!strcmp(aDataSearch, "END_COLOR"))	{ break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "RANDAM:")) { fscanf(pFile, "%d", &pPType->aColor.bColRandom); }	// ランダムカラーの有無
		else if (!strcmp(aDataSearch, "MAINCOLOR:")){ 			// メインカラー
			fscanf(pFile, "%f", &pPType->aColor.col.r);
			fscanf(pFile, "%f", &pPType->aColor.col.g);
			fscanf(pFile, "%f", &pPType->aColor.col.b);
			fscanf(pFile, "%f", &pPType->aColor.col.a);
		}
		else if (!strcmp(aDataSearch, "COLORMAX:")) {			// ランダムカラーの最大値
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.r);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.g);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.b);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMax.a);
		}
		else if (!strcmp(aDataSearch, "COLORMIN:")) { 			// ランダムカラーの最低値
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.r);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.g);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.b);
			fscanf(pFile, "%f", &pPType->aColor.colRandamMin.a);
		}
	}
}
//****************************************
// パーティクル・描画の読み込み処理
// author KEISUKE OOTONO
//****************************************
void LoadDraw(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// データ検索用

	// 描画 情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (!strcmp(aDataSearch, "END_DRAW"))			{ break; }											// 読み込みを終了
		else if (!strcmp(aDataSearch, "ALPHA_TYPE:"))	{ fscanf(pFile, "%d", &pPType->aDraw.nAlphaType); }	// アルファ種類
		else if (!strcmp(aDataSearch, "LIGHT:"))		{ fscanf(pFile, "%d", &pPType->aDraw.bLight); }		// ライティングの有無
	}
}
//****************************************
// パーティクル・速度の読み込み処理
// author KEISUKE OOTONO
//****************************************
void LoadSpeed(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// データ検索用

	// 速度 情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

			 if (!strcmp(aDataSearch, "END_SPEED"))	{ break; }									// 読み込みを終了
		else if (!strcmp(aDataSearch, "TYPE:"))		{ fscanf(pFile, "%d", &pPType->sType); }	// 速度計算
		else if (!strcmp(aDataSearch, "MOVE:"))		{ 											// 速度
			fscanf(pFile, "%f", &pPType->move.x);
			fscanf(pFile, "%f", &pPType->move.y);
			fscanf(pFile, "%f", &pPType->move.z);
		}
	}
}
//****************************************
// パーティクル・寿命の読み込み処理
// author KEISUKE OOTONO
//****************************************
void LoadLife(FILE *pFile, ParticleType *pPType)
{
	char	aDataSearch[TXT_MAX];	// データ検索用

	// 寿命 情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

			 if (!strcmp(aDataSearch, "END_LIFE"))	{ break; }									// 読み込みを終了
		else if (!strcmp(aDataSearch, "MAIN:"))		{ fscanf(pFile, "%d", &pPType->nM_Life); }	// メイン
		else if (!strcmp(aDataSearch, "SABU:"))		{ fscanf(pFile, "%d", &pPType->nS_Life); }	// サブ
	}
}