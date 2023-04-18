//========================================
// 床の処理
// Author:KEISUKE OTONO
//========================================
#include "floor.h"
//========================================
// グローバル変数
//========================================
LPDIRECT3DTEXTURE9		g_pTextureFloor = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;			//頂点バッファポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffFloor = NULL;			//頂点バッファポインタ
D3DXMATRIX g_mtxWorldFloor;//ワールドマトリックス

Floor g_Floor[NUM_FLOOR];
//========================================
// 初期化処理
//========================================
void InitFloor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
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

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Floor[0].nVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFloor,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffFloor->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定	

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
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファのアンロック
	g_pVtxBuffFloor->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_Floor[0].nBlock_X * 2 * (g_Floor[0].nBlock_Z + 1)) + ((g_Floor[0].nBlock_X - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffFloor,
		NULL);

	WORD *pIdx;//インデックス情報へのポインタ

			   //インデックスバッファをロックしｍ頂点番号データへのポインタを所得
	g_pIdxBuffFloor->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;	// インデックスのカウント

						//頂点番号データの設定
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

	//インデックスバッファをアンロックする
	g_pIdxBuffFloor->Unlock();
}
//========================================
// 終了処理
//========================================
void UninitFloor(void)
{
	//テクスチャの破棄
	if (g_pTextureFloor != NULL)
	{
		g_pTextureFloor->Release();
		g_pTextureFloor = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffFloor != NULL)
	{
		g_pIdxBuffFloor->Release();
		g_pIdxBuffFloor = NULL;
	}
}
//========================================
// 更新処理
//========================================
void UpdateFloor(void)
{

}
//========================================
// 描画処理
//========================================
void DrawFloor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldFloor);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Floor[0].rot.y,
		g_Floor[0].rot.x, g_Floor[0].rot.z);

	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Floor[0].pos.x, g_Floor[0].pos.y, g_Floor[0].pos.z);

	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFloor);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffFloor);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFloor);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_Floor[0].nVertex,//用意した頂点の数
		0,
		((g_Floor[0].nBlock_Z * g_Floor[0].nBlock_X) * 2) + (4 * (g_Floor[0].nBlock_X - 1)));//描画するプリミティブの数
}