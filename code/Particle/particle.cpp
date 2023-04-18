//========================================
// 
// パーティクルの処理
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
// プロトタイプ宣言
//****************************************
void SetCol(Particle *pPar);													// 色の設定
void SetCommon(Particle *pPar,ParticleType *pPType, int nIdx);					// 共通設定
/* サブパーティクル */
void SetSubParticle(Particle *pPar,int nCntApe);								// 設定
void DelSebParticle(int nCntPar);												// 削除
void SetSphere(Particle *pPar,int nCntPar);										// 球 生成処理
//****************************************
// グローバル変数
//****************************************
static Particle		g_aParticle[MAX_PARTICLE];			// パーティクルの情報
static Particle_Sub	g_aParticleSub[PARTICLE_SUB];		// サブパーティクルの情報

//========== *** パーティクルの情報を取得 ***
Particle *GetParticle(void)
{
	return g_aParticle;
}
//========== *** パーティクル・サブの情報を取得 ***
Particle_Sub *GetParticle_Sub(void)
{
	return g_aParticleSub;
}
//****************************************
// パーティクルの初期化処理
// author KEISUKE OOTONO
//****************************************
void InitParticle(void)
{
	Particle *pPar = g_aParticle;
	Particle_Sub *pParSub = g_aParticleSub;

	/* 構造体の初期化 */
	
	// メインパーティクル
	for (int nCntPar = 0; nCntPar < MAX_PARTICLE; nCntPar++, pPar++)
	{
		pPar->pos = INIT_D3DXVECTOR3;
		pPar->rot = INIT_D3DXVECTOR3;
		pPar->speed = INIT_D3DXVECTOR3;
		/* 色 */
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
		/* 描画 */
		pPar->aDraw.nAlphaType = 0;
		pPar->aDraw.nFadeTime = 0;
		/* 連続再生 */
		pPar->bCot = false;
		pPar->nCotIdx = 0;
		/* 円 */
		pPar->aCircle.nDir = 0;
		pPar->aCircle.nLay = 0;
		pPar->aCircle.fRadius = 10;
		/* 球 */
		pPar->aSphere.Sepalate = 8;
		pPar->aSphere.Split = 8;
		pPar->aSphere.fRadius = 3;
		pPar->aSphere.nFormType = 0;

	}

	// サブパーティクル
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
// パーティクルの終了処理
// author KEISUKE OOTONO
//****************************************
void UninitParticle(void)
{

}
//****************************************
// パーティクルの更新処理
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
						//エフェクト設定
						SetEffect(pParSub, pPar);
					}
				}
			}
			else if (pPar->bSet == false)
			{
				// 色の設定
				SetCol(pPar);

				if (pPar->nShape == 2)
				{
					// 球の生成処理
					SetSphere(pPar,nCntPar);
				}
				else
				{
					for (int nCntForm = 0; nCntForm < pPar->Form; nCntForm++)
					{
						if (pPar->nShape == 1)
						{
							// 間隔配置
							if (pPar->aCircle.nLay == 0)
							{
								pPar->fAnglePos = ((D3DX_PI * 2) / (float)pPar->Form) * nCntForm;
							}
							// ランダム配置
							if (pPar->aCircle.nLay == 1)
							{
								pPar->fAnglePos = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
							}
						}
						// サブパーティクルの設定
						SetSubParticle(pPar, nCntPar);
					}
				}
				g_aParticle[nCntPar].bSet = true;
			}

			//寿命の減少
			pPar->nLife--;
			if (pPar->nLife <= 0)
			{
				pPar->bUse = false;
				pPar->bSet = false;

				// サブパーティクルの削除
				DelSebParticle(nCntPar);
			}
		}
	}
}

//****************************************
// パーティクルの描画処理
// author KEISUKE OOTONO
//****************************************
void DrawParticle(void)
{

}
//****************************************
// パーティクルの設定処理
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

			//共通設定
			SetCommon(pPar,pPType,nIdx);

			/* 連続再生 */
			pPar->nCotIdx = pPType[nIdx].nCotIdx;

			pPar->bUse = true;
			break;
		}
	}
}

//****************************************
// サブパーティクルの設定処理
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
			// メインの番号を代入
			pParSub->nIdx = nCntApe;

			//位置の設定
			switch (pPar->nShape)
			{
			// 点
			case 0:
			{
				pParSub->pos = pPar->pos;
			}
			break;
			// 縦の円
			case 1:
			{
				// 垂直
				if (pPar->aCircle.nDir == 0)
				{
					pParSub->pos = D3DXVECTOR3(
						pPar->pos.x + sinf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.y + cosf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.z);
				}
				// 水平
				else if (pPar->aCircle.nDir == 1)
				{
					pParSub->pos = D3DXVECTOR3(
						pPar->pos.x + sinf(pPar->fAnglePos) * pPar->aCircle.fRadius,
						pPar->pos.y,
						pPar->pos.z + cosf(pPar->fAnglePos) * pPar->aCircle.fRadius);
				}

			}
			break;
			// 球
			case 2:
			{
				pParSub->pos = pPar->aSphere.Setpos;

				// 角度の方向に移動する
				pParSub->move = pParSub->pos - pPar->pos;
			}
			break;
			}

			if (pPar->nShape == 2)
			{


			}
			else
			{
				// 速度計算
				SpeedCalc(pPar, pParSub, pPType, pParSub->nIdx);
				pParSub->move = pParSub->move;
			}

			/* 寿命・半径 */
			pParSub->fRadius = pPar[nCntApe].fRadius;
			pParSub->nLife = pPar[nCntApe].nS_Life;
			/* 色 */
			pParSub->col = pPar->aColor.col;

			pParSub->bUse = true;

			//エフェクト設定
			SetEffect(pParSub,pPar);

			break;
		}
	}
}
//****************************************
// パーティクル・共通設定処理
// author KEISUKE OOTONO
//****************************************
void SetCommon(Particle *pPar, ParticleType *pPType,int nIdx)
{
	// 度→ラジアンに変換
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

	/* 色 */
	pPar->aColor.col = pPType[nIdx].aColor.col;
	pPar->aColor.bColRandom = pPType[nIdx].aColor.bColRandom;
	pPar->aColor.colRandamMin = pPType[nIdx].aColor.colRandamMin;
	pPar->aColor.colRandamMin = pPType[nIdx].aColor.colRandamMax;
	/* 描画 */
	pPar->aDraw.nFadeTime = pPType[nIdx].aDraw.nFadeTime;
	pPar->aDraw.nAlphaType = pPType[nIdx].aDraw.nAlphaType;
	pPar->aDraw.bLight = pPType[nIdx].aDraw.bLight;

	/* 連続再生 */
	pPar->bCot = pPType[nIdx].bCot;

	/* 円 */
	pPar->aCircle.nDir = pPType[nIdx].aCircle.nDir;
	pPar->aCircle.nLay = pPType[nIdx].aCircle.nLay;
	pPar->aCircle.fRadius = pPType[nIdx].aCircle.fRadius;

	/* 球 */
	pPar->aSphere.Sepalate = pPType[nIdx].aSphere.Sepalate;
	pPar->aSphere.Split = pPType[nIdx].aSphere.Split;
	pPar->aSphere.fRadius = pPType[nIdx].aSphere.fRadius;
	pPar->aSphere.nFormType = pPType[nIdx].aSphere.nFormType;
}
//****************************************
// サブパーティクルの削除処理
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
// 球パーティクル生成処理
// author KEISUKE OOTONO
//******************************************
void SetSphere(Particle *pPar, int nCntPar)
{
	float yRadian = 0.0f;		//縦で分割した時の横１周分のY座標が決まる角度

	//位置の設定
	switch (pPar->aSphere.nFormType)
	{
	case 0:
	{
		//天面の出っ張り頂点の設定
		pPar->aSphere.Setpos = D3DXVECTOR3(pPar->pos.x, pPar->pos.y + pPar->aSphere.fRadius, pPar->pos.z);
		SetSubParticle(pPar, nCntPar);

		//２番目の頂点から、横の分割数　‐　底面の出っ張りの１頂点　回数分 for文を回す
		for (int nCntDevideY = 1; nCntDevideY < pPar->aSphere.Sepalate; nCntDevideY++)
		{
			//---------------------------------------------------------------------------------------------------------
			//	MEMO：縦に分割する数を使って原点から横１周分の高さまでの角度計算 (半周分で分割点が分かるので、3.14
			//---------------------------------------------------------------------------------------------------------
			yRadian += D3DX_PI / pPar->aSphere.Sepalate;
			float TempLen = fabsf(sinf(yRadian) * pPar->aSphere.fRadius);	//X・Zの半径
			float rot_Y = D3DX_PI;									//Y軸の角度
			float Height_Y = cosf(yRadian) * pPar->aSphere.fRadius;		//Yの高さ
			for (int nCntDevideX = 0; nCntDevideX < pPar->aSphere.Split; nCntDevideX++)
			{//横１周分の頂点座標を設定

				pPar->aSphere.Setpos =
					D3DXVECTOR3(
						pPar->pos.x + sinf(rot_Y) * TempLen,	//Xの位置
						pPar->pos.y + Height_Y,					//Yの位置
						pPar->pos.z + cosf(rot_Y) * TempLen);	//Zの位置
																//角度を　全体の角度÷分割数で割った答え分、引く
				rot_Y -= (D3DX_PI*2.0f) / pPar->aSphere.Split;

				SetSubParticle(pPar, nCntPar);
			}
		}

		//底面の出っ張り頂点の設定
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
					pPar->pos.x + sinf(rot_Y) * TempLen,	//Xの位置
					pPar->pos.y + Height_Y,					//Yの位置
					pPar->pos.z + cosf(rot_Y) * TempLen);	//Zの位置


			SetSubParticle(pPar, nCntPar);
		}
	}
	break;
	}
}
//****************************************
// 色の設定処理
// author KEISUKE OOTONO
//****************************************
void SetCol(Particle *pPar)
{
	// ランダムカラー
	if (pPar->aColor.bColRandom)
	{
		pPar->aColor.col.r = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.r - pPar->aColor.colRandamMin.r)) + pPar->aColor.colRandamMin.r;
		pPar->aColor.col.g = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.g - pPar->aColor.colRandamMin.g)) + pPar->aColor.colRandamMin.g;
		pPar->aColor.col.b = ((rand() / (float)RAND_MAX) * (pPar->aColor.colRandamMax.b - pPar->aColor.colRandamMin.b)) + pPar->aColor.colRandamMin.b;
	}
}
//********************************************************************************
// テスト
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