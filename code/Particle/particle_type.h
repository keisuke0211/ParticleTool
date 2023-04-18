//========================================
// 
// パーティクルの種類のヘッダファイル
// author KEISUKE OOTONO
// 
//========================================
// *** particle_type.h ***
//========================================
#ifndef _PARTICLE_TYPE_H_
#define _PARTICLE_TYPE_H_  
#include "../main.h"
#include "particle.h"
//****************************************
// マクロ定義
//****************************************
#define PARTICLE_TYPE		(240)			//パーティクル種類の最大数
//========================================
// 列挙型の定義
//========================================
typedef enum
{
	SHAPE_POINT = 0, // 点
	SHAPE_CIRCLE,    // 円
	SHAPE_SPHERE,    // 球
	SHAPE_MAX,
}SHAPE;
//****************************************
// 構造体の定義
//****************************************
// パーティクル・描画
typedef struct
{
	int nFadeTime;		// フェード時間
	int nAlphaType;		// アルファ種類
	bool bLight;		// ライティングの有効か無効
} ParticleDraw1;

// 色
typedef struct
{
	D3DXCOLOR col;		// 色
	D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
	D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
	bool bColRandom;			// ランダムで色を変更するか
} Color1;

// 円
typedef struct
{
	int nDir;			// 方向（垂直・水平）
	int nLay;			// 配置方法
	float fRadius;	// 円の半径
} Circle1;

// 球
typedef struct
{
	int Sepalate;		// 分割数
	int Split;			// 分割数
	float fRadius;	// 球の半径
	int nFormType;		// 生成方法
} Sphere1;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置（設定位置）
	D3DXVECTOR3 Cotpos;	// 位置（連続再生）
	D3DXVECTOR3 rot;	// 向き
	D3DXVECTOR3 move;	// 移動量
	Color1 aColor;		// 色

						/* メイン */
	int Form;			// 一度に生成するパーティクル数
	int nM_Life;		// 寿命
	int nType;			// 画像種類
						/* 描画 */
	ParticleDraw1 aDraw;

	/* 連続再生 */
	bool bCot;			// 連続再生の有無
	int nCotIdx;		// 再生するパーティクル番号

	/* 速度 */
	float fAttenuation;	// 減衰量

	/* 形状 */
	int nShape;				// 形状
	Circle1 aCircle;		// 円
	Sphere1 aSphere;		// 球

	/* サブ */
	int sType;			// 速度計算
	int nS_Life;		// 寿命
	float fRadius10;	// 半径（大きさ）

	bool bUse;			// 使用フラグ
} ParticleType;//parts
//****************************************
// プロトタイプ宣言
//****************************************
void InitParticleType(void);

// ツール用
void LoadParticleType(void);
void SaveParticleType(void);

ParticleType *GetParticleType(void);

#endif