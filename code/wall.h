//****************************************
//	wall.h
//	Author:KEISUKE OTONO
//****************************************
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

//****************************************
// マクロ定義
//****************************************
#define NUM_WALL			(1)			//メッシュの配置数
//****************************************
// 構造体の定義
//****************************************

//メッシュフィールド・床の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int	nBlock_X;				//横のブロック数
	int nBlock_Y;				//縦のブロック数
	int nVertex;				//頂点数
	int nIdx;					//インデックス数
	int nPolygon;				//ポリゴン数

	float			fWidhtX;	//横幅
	float			fWidhtY;	//高さ
}Wall;

//****************************************
// プロトタイプ宣言
//****************************************
void InitWall (void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

#endif 