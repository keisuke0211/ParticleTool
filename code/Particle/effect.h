//========================================
// 
// エフェクトのヘッダファイル
// author KEISUKE OOTONO
// 
//========================================
//  *** effect.h ***
//========================================
#ifndef _EFFECT_H_	// このマクロ定義がされていなかった時
#define _EFFECT_H_	// 二重インクルード防止のマクロを定義する
#include "../main.h"
#include "particle.h"
#include "particle_type.h"
//****************************************
// マクロ定義
//****************************************
#define EFFECT_DATA_FILE_PATH (".\\data\\GAMEDATA\\EFFECT\\EFFECT_DATA.txt")
#define MAX_EFFECT	(10240)			// エフェクトの最大数
#define NUM_EFFECT	(100)			// エフェクトの種類
#define MAX_M_LIFE	(60)			// メイン寿命の最大値
#define MAX_S_LIFE	(300)			// サブ寿命の最大値
//****************************************
// 列挙型
//****************************************
enum ALPHABLENDTYPE
{
	TYPE_ADD = 0,		// 加算
	TYPE_SUB,			// 減算
	TYPE_NONE,			// 乗算
	MAX_TYPE,
};
//****************************************
// 情報構造体
//****************************************

// エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3	rot;		// 向き
	D3DXVECTOR3 move;		// 移動量
	D3DXMATRIX	mtxWorld;	// ワールドマトリックス
	D3DXCOLOR col;			// 頂点カラー
	float fRadius;			// 半径（大きさ）
	int nLife;				// 寿命（表示時間）
	int nInitLife;			// 初期の寿命
	int nType;				// 画像種類
	int nAlphaType;			// アルファ種類
	int nShape;				// 
	/* 描画 */
	int nFadeTime;			// フェードする時間
	/* 連続再生 */
	bool bCot;				// 連続再生の有無
	int nCotIdx;			// 再生するパーティクル番号

	bool bLight;			// ライティングの有効か無効
	bool bUse;				// 使用しているかどうか
} Effect;

// エフェクトの構造体
typedef struct
{
	char aName[TXT_MAX];		// 画像名
	char aFileName[TXT_MAX];	// ファイル名
	bool bData;					// データの有無
} EffectType;
// エフェクトの構造体
typedef struct
{
	char aName[TXT_MAX];		// 画像名
} Type;
//****************************************
// クラス
//****************************************
class CEffect
{
public:// 構造体
	typedef struct
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3	rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXMATRIX	mtxWorld;	// ワールドマトリックス
		D3DXCOLOR col;			// 頂点カラー
		float fRadius;			// 半径（大きさ）
		int nLife;				// 寿命（表示時間）
		int nInitLife;			// 初期の寿命
		int nType;				// 画像種類
		int nAlphaType;			// アルファ種類
		int nShape;				// 
								/* 描画 */
		int nFadeTime;			// フェードする時間
								/* 連続再生 */
		bool bCot;				// 連続再生の有無
		int nCotIdx;			// 再生するパーティクル番号

		bool bLight;			// ライティングの有効か無効
		bool bUse;				// 使用しているかどうか
	} Info;

public:// 関数
	CEffect();		// コンストラクタ
	~CEffect();		// デストラクタ
	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画
	void Load();	// 読み込み
	void Set(Particle_Sub *pParSub, Particle *pPar);// 設定
private:// 関数
	void Fade(Info *pInfo);	// フェード

public:// 変数
	Info m_Info[MAX_EFFECT];	// 情報
};
//****************************************
// プロトタイプ宣言
//****************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void LoadEffect(void);
void SetEffect(Particle_Sub *pParSub, Particle *pPar);
Effect	*GetEffect(void);
EffectType *GetEffectType(void);
Type *GetType(void);
// 追加する事
// 飛散幅の調整
// マルチテクスチャ
// 重力の追加
// メッシュエフェクト　円
// メッシュエフェクト　球
// メッシュエフェクト　円錐
// 軌跡エフェクトの有無
// 速度減衰の有無

/* 余裕があれば */
// インスタンシングの実装（描画を一括で行う処理）

#endif