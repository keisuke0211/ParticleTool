//========================================
// 
// imGuiのヘッダファイル
// author KEISUKE OOTONO
// 
//========================================
#ifndef _IMGUI_MAIN_H_
#define _IMGUI_MAIN_H_  
#include "main.h"

//****************************************
// 情報構造体
//****************************************

// エフェクトの構造体
typedef struct
{
	int nIdx;		// 再生するパーティクル番号
	int nCntTime;	// 時間
	int nCntLoop;	// ループ間隔
	bool bLoop;		// ループ再生しているかどうか
	bool bStop;		// 一時停止
} ParticleSystem;

//****************************************
// プロトタイプ宣言
//****************************************
void InitImguiMain(HWND hWnd);
void UninitImguiMain(void);
void UpdateImguiMain(void);
void DrawImguiMain(void);
ParticleSystem *GetParticleSystem(void);
#endif