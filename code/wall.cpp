//========================================
// 壁の処理
// Author:KEISUKE OTONO
//========================================
#include "wall.h"
//========================================
// グローバル変数
//========================================
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//頂点バッファポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;			//頂点バッファポインタ
D3DXMATRIX g_mtxWorldWall;//ワールドマトリックス

Wall g_Wall[NUM_WALL];
//========================================
// 初期化処理
//========================================
void InitWall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
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

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Wall[0].nVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定	

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
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファのアンロック
	g_pVtxBuffWall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_Wall[0].nBlock_Y * 2 * (g_Wall[0].nBlock_X + 1)) + ((g_Wall[0].nBlock_Y - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffWall,
		NULL);

	WORD *pIdx;//インデックス情報へのポインタ

			   //インデックスバッファをロックしｍ頂点番号データへのポインタを所得
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;	// インデックスのカウント

	//頂点番号データの設定
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

	//インデックスバッファをアンロックする
	g_pIdxBuffWall->Unlock();
}
//========================================
// 終了処理
//========================================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}
}
//========================================
// 更新処理
//========================================
void UpdateWall(void)
{

}
//========================================
// 描画処理
//========================================
void DrawWall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldWall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[0].rot.y,
		g_Wall[0].rot.x, g_Wall[0].rot.z);

	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Wall[0].pos.x, g_Wall[0].pos.y, g_Wall[0].pos.z);

	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffWall);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureWall);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_Wall[0].nVertex,//用意した頂点の数
		0,
		((g_Wall[0].nBlock_X * g_Wall[0].nBlock_Y) * 2) + (4 * (g_Wall[0].nBlock_Y - 1)));//描画するプリミティブの数
}