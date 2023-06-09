//========================================
// meain.h
// Author:KEISUKE OTONO
//========================================

#ifndef _MAIN_H_
#define _MAIN_H_             

#include<windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include"XInput.h"
#include"d3dx9.h"  //描画処理に必要
#define DIRECTINPUT_VERSION (0X0800) //ビルド時の警告対処用のマクロ
#include"dinput.h"//入力処理に必要//２重インクルード帽子のマクロを定義する

/* ingui */
#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_internal.h"
#include <d3d9.h>
#include <tchar.h>
//========================================
// ライブラリ
//========================================

#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻所得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")	//入力処理に必要
#pragma comment(lib,"Opengl32.lib")	//フォグ処理に必要
#include"xaudio2.h"					//サウンド処理に必要
//========================================
// マクロ定義
//========================================
#define SCREEN_WIDTH		(1280.0f)														// ウインドウの幅
#define SCREEN_HEIGHT		(720.0f)														// ウインドウの高さ　←main.cppから移動
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					// 頂点フォーマット【2D】
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット【3D】
#define SWITCH_TIME			(600)															// 切り替わる時間(1秒 = 60)　※一部の画面切り替えに仕様
#define WINDOW_MODE			(false)															// ウインドウモードの切り替え
#define DEFAULT_INT			(0)																// intの初期値
#define DEFAULT_FLOA		(0.0f)															// floatの初期値
#define INIT_RHW			D3DXVECTOR3(0.0f,1.0f,0.0f)										// D3DXVECTOR3の初期化
#define INIT_D3DXVECTOR3	D3DXVECTOR3(0.0f,0.0f,0.0f)										// D3DXVECTOR3の初期値
#define INIT_D3DXCOLOR		D3DXCOLOR(1,1,1,1)												// D3DXCOLORの初期値
#define DIAGONALLY_UP		(0.25f)		//斜め上
#define DIAGONALLY_DOWN		(0.75f)		//斜め下
#define BESIDE				(0.5f)		//横
#define ABOVE				(0.0f)		//上
#define UNDER				(1.0f)		//下
#define TXT_MAX				(256)		// 文字列の最大数(汎用)
//========================================
// 情報構造体
//========================================

//頂点情報【2D】の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換係数（1.0fで固定）
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_2D;
//頂点情報【3D】の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_3D;

// メイン処理の情報構造体
typedef struct
{
	HWND				hWnd;		// ウインドウハンドル(識別子)
	D3DXVECTOR3			windowPos;	// ウインドウの位置
}Main;

//========================================
// プロトタイプ宣言
//========================================
LPDIRECT3DDEVICE9 GetDevice(void);
void WindowMode(bool bScreen);								// ウインドウのモードを切り替える
HWND *GetWindowHandle(void);
D3DXVECTOR3 GetWindowPos(void);

#endif