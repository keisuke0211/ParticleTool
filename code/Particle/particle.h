//========================================
// 
// パーティクルのヘッダファイル
// author KEISUKE OOTONO
// 
//========================================
//  *** particle.h ***
//========================================
#ifndef _PARTICLE_H_		//このマクロ定義がされてなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

#include "../main.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define MAX_PARTICLE		(120)			//パーティクルの最大数
#define PARTICLE_SUB		(2560)			//サブパーティクルから出現するの最大数

// パーティクルクラス

class CParticle
{
public:	// 列挙型
public: // 構造体

	//-----------------------------------------------------------------------------
	// エフェクトカラーの構造体
	// Author  : KEISUKE OOTONO
	// outline : エフェクトの色関係の構造体
	//-----------------------------------------------------------------------------
	struct Color
	{
		D3DXCOLOR col;				// 色
		D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
		D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
		bool bColRandom;			// ランダムで色を変更するか
	};

	//-----------------------------------------------------------------------------
	// エフェクト描画の構造体
	// Author  : KEISUKE OOTONO
	// outline : エフェクトの描画関係の構造体
	//-----------------------------------------------------------------------------
	struct Draw
	{
		int nFadeTime;		// フェード時間
		int nAlphaType;		// アルファ種類
		bool bLight;		// ライティングの有効か無効
	};

	//-----------------------------------------------------------------------------
	// 円の構造体
	// Author  : KEISUKE OOTONO
	// outline : パーティクル・形状（円）関係の構造体
	//-----------------------------------------------------------------------------
	struct Circle
	{
		int nDir;			// 方向（垂直・水平）
		int nLay;			// 配置方法
		float fRadius01;	// 円の半径
	};

	//-----------------------------------------------------------------------------
	// 球の構造体
	// Author  : KEISUKE OOTONO
	// outline : パーティクル・形状（球）関係の構造体
	//-----------------------------------------------------------------------------
	struct Sphere
	{
		D3DXVECTOR3 Setpos;	// 位置（設定位置）
		int Sepalate;		// 分割数
		int Split;			// 分割数
		float fRadius02;	// 球の半径
		int nFormType;		// 生成方法
	};

	//-----------------------------------------------------------------------------
	// パーティクル情報の構造体
	// Author  : KEISUKE OOTONO
	// outline : パーティクルの構造体
	//-----------------------------------------------------------------------------
	struct Info
	{
		/* 構造体 */
		Draw aDraw;		// 描画
		Color aColor;	// カラー
		Circle aCircle;	// 円
		Sphere aSphere;	// 球

		/* 列挙型 */

	};
public:	// 関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
public:	// 変数
	Info m_Info;
};

//****************************************
// 構造体の定義
//****************************************

// パーティクル・描画
typedef struct
{
	int nFadeTime;		// フェード時間
	int nAlphaType;		// アルファ種類
	bool bLight;		// ライティングの有効か無効
} ParticleDraw;

// 色
typedef struct
{
	D3DXCOLOR col;				// カラー
	D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
	D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
	bool bColRandom;			// ランダムで色を変更するか

} Color;

// 円
typedef struct
{
	int nDir;			// 方向（垂直・水平）
	int nLay;			// 配置方法
	float fRadius;		// 円の半径
} Circle;

// 球
typedef struct
{
	D3DXVECTOR3 Setpos;	// 位置（設定位置）
	int Sepalate;		// 分割数
	int Split;			// 分割数
	float fRadius;		// 球の半径
	int nFormType;		// 生成方法
} Sphere;
//========================================
//パーティクル・メインの構造体
//========================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置（発生位置）
	D3DXVECTOR3 rot;	// 回転
	D3DXVECTOR3 speed;	// 移動量(x.y.z)
	Color aColor;		// 色
	int nLife;			// 寿命
	int nType;			// 画像種類
	int Form;			// 一度に生成するパーティクル数
	ParticleDraw aDraw;	// 描画
	/* 連続再生 */
	bool bCot;			// 連続再生の有無
	int nCotIdx;		// 再生するパーティクル番号
	/* 形状 */
	int nShape;			// 形状種類
	Circle aCircle;		// 円
	Sphere aSphere;		// 球
	
	/* サブ */
	int nMoveType;		// 移動種類
	int nS_Life;		// 寿命
	float fAnglePos;	// 角度（位置用）
	float fRadius;		// 半径（大きさ）

	bool bSet;			// 生成したか
	bool bUse;			// 使用しているかどうか

} Particle;

//========================================
//パーティクル・サブの構造体
//========================================

typedef struct
{
	D3DXVECTOR3 pos;	// 位置（発生位置）
	D3DXVECTOR3 rot;	// 向き
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;		// 色

	int nLife;			// 寿命
	int nIdx;			// メインパーティクルの番号

	float fRot;			// 角度（移動用）
	float fRadius;		// 半径（大きさ）
	bool bUse;			// 使用しているかどうか
} Particle_Sub;




//****************************************
// プロトタイプ宣言
//****************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos,int nIdx);

Particle		*GetParticle(void);
Particle_Sub	*GetParticle_Sub(void);
#endif