//========================================
// 
// エフェクトの処理
// author KEISUKE OOTONO
// 
//========================================
// *** effect.cpp ***
//========================================
#include "effect.h"
#include <stdio.h>

//========================================
// プロトタイプ宣言
//========================================
void EffectFade(Effect *pEffect);	// フェード処理
//========================================
// グローバル変数
//========================================
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;						//頂点バッファポインタ
static LPDIRECT3DTEXTURE9		g_pTextureEffect[NUM_EFFECT] = {};			//テクスチャへのポインタ
static Effect g_aEffect[MAX_EFFECT];										// エフェクトの構造体
static EffectType g_aEffectType[NUM_EFFECT];								// エフェクト種類の構造体sta
static Type g_aType[NUM_EFFECT];											// 画像名の構造体
static int	g_nEffectNum;													// エフェクトの数
// エフェクトの情報の取得
Effect	*GetEffect(void)
{
	return g_aEffect;
}
// エフェクト種類の取得
EffectType *GetEffectType(void)
{
	return g_aEffectType;
}
// 画像名の取得
Type *GetType(void)
{
	return g_aType;
}
//========================================
// エフェクトの初期化処理
// author KEISUKE OOTONO
//========================================
void InitEffect(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/* テクスチャの設定 */
	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEffectType[nCntTexture].aFileName,
			&g_pTextureEffect[nCntTexture]);
	}

	/* エフェクトの情報の初期化処理 */
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
		// 画像名をエフェクト種類の画像名にコピーする
		memcpy(&g_aType[nCntTexture].aName, &g_aEffectType[nCntTexture].aName, sizeof(g_aEffectType[nCntTexture].aName));
	}
	/* 頂点バッファの生成 */
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	/* 設定用ポインタ */
	VERTEX_3D *pVtx;

	/* 頂点バッファのロックと頂点情報へのポインタを取得 */
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		/* 頂点座標の設定 */
		pVtx[0].pos = INIT_D3DXVECTOR3;
		pVtx[1].pos = INIT_D3DXVECTOR3;
		pVtx[2].pos = INIT_D3DXVECTOR3;
		pVtx[3].pos = INIT_D3DXVECTOR3;
		
		/* rhwの設定 */
		pVtx[0].nor = INIT_RHW;
		pVtx[1].nor = INIT_RHW;
		pVtx[2].nor = INIT_RHW;
		pVtx[3].nor = INIT_RHW;
		
		/* 頂点カラー */
		pVtx[0].col = INIT_D3DXCOLOR;
		pVtx[1].col = INIT_D3DXCOLOR;
		pVtx[2].col = INIT_D3DXCOLOR;
		pVtx[3].col = INIT_D3DXCOLOR;

		/* テクスチャ座標 */
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}
	/* 頂点バッファの破棄 */
	g_pVtxBuffEffect->Unlock();
}
//========================================
// エフェクトの終了処理
// author KEISUKE OOTONO
//========================================
void UninitEffect(void)
{
	/* テクスチャの破棄 */
	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		if (g_pTextureEffect[nCntTexture] != NULL)
		{
			g_pTextureEffect[nCntTexture]->Release();
			g_pTextureEffect[nCntTexture] = NULL;
		}
	}

	
	/* 頂点バッファの破棄 */
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//========================================
// エフェクトの更新処理
// author KEISUKE OOTONO
//========================================
void UpdateEffect(void)
{
	Particle *pPar = GetParticle();
	ParticleType *pPType = GetParticleType();
	Effect *pEffect = GetEffect();

	/* テクスチャ座標の更新 */
	VERTEX_3D *pVtx;

	/* 頂点バッファのロック */
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++,pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//エフェクトが使用されている

			// X・Zの移動量にかける割合
			float fRateXZ = 0.0f;
			// Yの移動量にかける割合
			float fRateY = 0.0f;

			// 速度減衰
			pEffect->move.x += (0.0f - pEffect->move.x) * 0.05f;
			pEffect->move.y += (0.0f - pEffect->move.y) * 0.05f;
			pEffect->move.z += (0.0f - pEffect->move.z) * 0.05f;


			/* エフェクの位置更新 */
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;
			pEffect->pos.z += pEffect->move.z;

			// フェード処理
			EffectFade(pEffect);

			/* 頂点座標の設定 */
			pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, -0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, -0.0f);

			/* 頂点カラーの設定 */
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			/* 寿命と大きさを減らしていく */
			pEffect->nLife--;

			if (pEffect->nLife == 0)
			{//寿命が尽きた

				if (pEffect->bCot == true)
				{
					SetParticle(pEffect->pos, pEffect->nCotIdx);
					pEffect->bCot = false;
				}

				pEffect->bUse = false;

				
			}
		}
	}
	/* 頂点バッファのアンロック */
	g_pVtxBuffEffect->Unlock();
}
//========================================
// エフェクトの描画処理
// author KEISUKE OOTONO
//========================================
void DrawEffect(void)
{
	/* デバイスのポインタ */
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans, mtxRot;				//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	/* デバイスの所得 */
	pDevice = GetDevice();

	/* データストリームの設定 */

	/* 頂点バッファをデータストリームに設定 */
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

	/* 頂点フォーマットの設定 */
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		/* Zテストを無効にする */
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ライティングを有効・無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, g_aEffect[nCntEffect].bLight);

		// アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		switch (g_aEffect[nCntEffect].nAlphaType)
		{
		case TYPE_NONE:	// 乗算
			break;

		case TYPE_ADD:	// 加算
			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case TYPE_SUB:	// 減算
			// αブレンディングを減算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		}

		for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
		{
			if (g_aEffect[nCntEffect].bUse)
			{//エフェクトが使用されている

				 /* ワールドマトリックスの初期化 */
				D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);//逆行列を求める
				g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
				g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
				g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

				/* 位置を反映 */
				D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

				D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

				/* ワールドマトリックスの設定 */
				pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

				/* テクスチャの設定 */
				pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

				/* ポリゴンの描画 */
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
			}
		}
		/* Zテストを有効にする */
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//　ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

		/* αブレンディングを元に戻す */
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//========================================
// エフェクトの設定処理
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

			/* 描画 */
			g_aEffect[nCntEffect].nAlphaType = pPar->aDraw.nAlphaType;
			g_aEffect[nCntEffect].nFadeTime = pPar->aDraw.nFadeTime;
			/* 連続再生 */
			g_aEffect[nCntEffect].bCot = pPar->bCot;

			g_aEffect[nCntEffect].bLight = pPar->aDraw.bLight;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
	}
}
//========================================
// エフェクト画像の読み込み処理
// author KEISUKE OOTONO
//========================================
void LoadEffect(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch [TXT_MAX];	// データ検索用
	EffectType *pEffect = g_aEffectType;


	for (int nCntTexture = 0; nCntTexture < NUM_EFFECT; nCntTexture++)
	{
		g_aEffectType[nCntTexture].bData = false;
	}
	// エフェクト数を初期化
	g_nEffectNum = 0;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(EFFECT_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}
		
		if (g_nEffectNum == NUM_EFFECT)
		{// 処理を終了
			fclose(pFile);
			break;
		}
		if (!strcmp(aDataSearch, "EFFECT")) {
			fscanf(pFile, "%s", &pEffect->aFileName);	// ファイル名
			fscanf(pFile, "%s", &pEffect->aName);		// 画像名
			pEffect->bData = true;
			pEffect++;				// ポインタを進める
			g_nEffectNum++;			// エフェクト数を加算
		}
	}
}
//========================================
// エフェクトフェード処理
// author KEISUKE OOTONO
//========================================
void EffectFade(Effect *pEffect)
{
	// フェード開始するか
	if (pEffect->nLife <= pEffect->nFadeTime)
	{
		/* サイズ */
		pEffect->fRadius -= pEffect->fRadius / pEffect->nFadeTime;

		/* 不透明度更新 */
		pEffect->col.a *= ((float)pEffect->nFadeTime / (float)pEffect->nInitLife);

		pEffect->nFadeTime--;
	}
}