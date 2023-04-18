//****************************************
//	floor.h
//	Author:KEISUKE OTONO
//****************************************
#ifndef _FLOOR_H_
#define _FLOOR_H_
#include "main.h"

//****************************************
// マクロ定義
//****************************************
#define NUM_FLOOR			(1)			//メッシュの配置数
//****************************************
// 構造体の定義
//****************************************

//メッシュフィールド・床の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int	nBlock_Z;				//横のブロック数
	int nBlock_X;				//縦のブロック数
	int nVertex;				//頂点数
	int nIdx;					//インデックス数
	int nPolygon;				//ポリゴン数

	float			fWidhtX;	//縦幅
	float			fWidhtZ;	//横幅
}Floor;

//****************************************
// プロトタイプ宣言
//****************************************
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);

#endif 