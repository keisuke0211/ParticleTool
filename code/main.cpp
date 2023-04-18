//========================================
// メインの処理
// Author:大殿慶輔
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "imgui_main.h"
#include "sound.h"				// サウンド
#include "input.h"				// 入力
#include "light.h"				// ライト
#include "camera.h"				// カメラ
#include "floor.h"				// 床
#include "wall.h"				// 壁
#include "Particle\effect.h"	// エフェクト
#include "Particle\particle.h"	// パーティクル
//========================================
// マクロ定義
//========================================
#define CLASS_NAME    "WindowClass"		//ウインドウクラスの名前
#define WINDOW_NAME   ""				//ウインドウの名前(キャプションに表示)

// (※ImGuiが当たり判定の際にDirect3Dデバイスの画面サイズを参照しており、誤差を埋める為の数値)
#define IMGUI_WINDOW_CORRECT_X (0.98775f)	// ImGui用のウィンドウ領域補正値X
#define IMGUI_WINDOW_CORRECT_Y (0.94735f)	// ImGui用のウィンドウ領域補正値Y
//========================================
// プロトタイプ宣言
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void Load(void);
void DrawFPS(void);
void ResetDevice();

// メッセージ ハンドラ
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//========================================
// グローバル変数
//========================================
LPDIRECT3D9		  g_pD3D = NULL;			    // Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		    // Direct3Dデバイスへのポインタ
D3DPRESENT_PARAMETERS d3dpp = {};				//プレゼンテーションパラメータ
LPD3DXFONT g_pFont = NULL;						// フォントへのポインタ
int DisplayMode = 1;							// 操作方法表示　ON/OFF
int g_nCountFPS = 0;							// FPSカウンタ

bool bFps = true;
Main	g_main;									// メイン処理の情報構造体

//========== *** ウインドウハンドルを取得 ***
HWND *GetWindowHandle(void)
{
	return &g_main.hWnd;
}
//========== *** ウインドウの位置を取得 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}
//========================================
// 情報の所得
//========================================

//メイン処理の情報
Main *GetMain(void)
{
	return &g_main;
}

//========================================
// メイン関数
//========================================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

	// ウインドウクラスの構造体

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						  //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								  //ウインドウのスタイル
		WindowProc,								  //ウインドウプロシージャ
		0,										  //0にする(通常は使用しない)
		0,										  //0にする(通常は使用しない)
		hinstance,								  //インスタンスハンドル
		(HICON)LoadImage(NULL,"icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),		  //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				  //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				  //クライアント領域の背景色
		NULL,									  //メニューバー
		CLASS_NAME,								  //ウインドウクラスの名前
		(HICON)LoadImage(NULL,"icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),		  //ファイルのアイコン
	};
	MSG msg;									  //メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//画面サイズの構造体
	// ウインドウクラスの登録

	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウの生成
	g_main.hWnd = CreateWindowEx(0, //拡張ウィンドウスタイル
		CLASS_NAME,          //ウインドウクラスの名前
		WINDOW_NAME,         //ウインドウの名前
		WS_OVERLAPPEDWINDOW, //ウインドウスタイル
		CW_USEDEFAULT,       //ウインドウの左上X座標
		CW_USEDEFAULT,       //ウインドウの左上Y座標
		SCREEN_WIDTH,        //ウインドウの幅
		SCREEN_HEIGHT,       //ウインドウの高さ
		NULL,                //親ウインドウのハンドル
		NULL,                //メニューハンドルまたは子ウインドウID
		hinstance,           //インスタンスハンドル
		NULL);               //ウインドウ作成デー

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	//ウインドウモードの切り替え
	WindowMode(WINDOW_MODE);

	/* 初期化処理 */
	if (FAILED(Init(hinstance, g_main.hWnd, TRUE)))
	{//初期化処理に失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻を所得
	dwFrameCount = 0;				//初期化する
	dwFPSLastTime = timeGetTime();	//現在時刻を所持

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//WM_QUITメッセージを受け取ったらメッセージループを抜ける

			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);  //ウインドウプロシージャへメッセージを送出
			}

		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を所得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//005秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPSを測定した時刻を保存
				dwFrameCount = 0;//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過したら

				dwExecLastTime = dwCurrentTime;//処理開始の時刻(現在時刻)を保存

				RECT rec;	// 大きさを受け取るRECT構造体

				// ウインドウの情報を取得
				GetWindowRect(g_main.hWnd, &rec);

				// ウインドウの位置を設定
				g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;//フレームカウントを加算
			}
		}
	}
	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//========================================
// ウインドウプロシージャ
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	int nID;

	switch (uMsg)
	{
	/*case WM_SIZE:
		if (g_pD3DDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}*/
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY: //ウインドウ破棄もメッセージ

					 //WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
			}
			else
			{
				return 0;
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
		}
		else
		{
			return 0;
		}
		break;
	}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す
}

//========================================
// デバイスの所得
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//========================================
// 初期化処理
//========================================
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow)//TRUE:ウインドウ/FALSE:フルスクリーン
{
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード
								 //Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));   //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH * IMGUI_WINDOW_CORRECT_X;		//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT * IMGUI_WINDOW_CORRECT_Y;	//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;								//バックバッファの形式
	d3dpp.BackBufferCount = 1;											//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;								//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;											//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//インターバル

																		//Direct3Dデバイスも生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,                     //描画処理
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//頂点処理
		&d3dpp,
		&g_pD3DDevice)))                    //描画処理と頂点処理をハードウェアで行う
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,//描画処理
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,                     //描画処理
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE,
		"Terminal", &g_pFont);

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//D3DCULL_NONE　カリング無し D3DCULL_CW 表面カリング　D3DCULL_CCW　裏面カリング
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	if (FAILED(InitInput(hlnstance, hWnd)))
	{
		return E_FAIL;
	}

	// 読み込み処理
	Load();

	// imGui
	InitImguiMain(hWnd);

	//サウンドの初期化処理
	InitSound(hWnd);

	//カメラ
	InitCamera();
	//ライト
	InitLight();

	//各種オブジェクトの初期化処理
	InitFloor();		// 床
	InitWall();			// 壁
	InitEffect();		// エフェクト
	InitParticle();		// パーティクル
	InitParticleType();	// パーティクル・種類
	
	return S_OK;
}
//========================================
// 終了処理
//========================================
void Uninit(void)
{
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　　各種オブジェクトの終了処理
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	/* 終了処理 */

	UninitImguiMain();	// imGui
	UninitFloor();		// 床
	UninitWall();		// 壁
	UninitEffect();		// エフェクト
	UninitParticle();	// パーティクル

	//サウンドの終了処理
	StopSound();

	UninitSound();

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	
	// デバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	
	// オブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//========================================
// 更新処理
//========================================
void Update(void)
{
	Camera *pCamera = Getcamara(2);
	ParticleSystem * pSys = GetParticleSystem();

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　　各種オブジェクトの更新処理
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//キーボードの更新処理
	UpdateKeyboard();

	//カメラ
	UpdateCamera();
	//ライト
	UpdateLight();

	UpdateImguiMain();	// imGui
	if (pSys->bStop == false)
	{
		UpdateEffect();		// エフェクト
	}
	UpdateParticle();	// パーティクル
	UpdateFloor();		// 床
	UpdateWall();		// 壁
}

//========================================
// 描画処理
//========================================
void Draw(void)
{
	Camera *pCamera = Getcamara(0);
	D3DVIEWPORT9 veiwportDef;

	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//現在のビューポートを取得
		g_pD3DDevice->GetViewport(&veiwportDef);

		// カメラの数分回す
		for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCamera++)
		{
			if (pCamera->bUse == true)
			{
				SetCamera(nCntCamera);		// カメラ(3D)の設定処理

				// 画面クリア
				g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
				//　　各種オブジェクトの描画処理
				//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
				

				DrawFloor();	// 床
				//DrawWall();	// 壁
				DrawEffect();	// エフェクト
				DrawParticle();	// パーティクル
				DrawImguiMain();// imGui
			}
		}

		//ビューポートを元に戻す
		g_pD3DDevice->SetViewport(&veiwportDef);

#ifdef _DEBUG
		//テキストの描画処理
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

		char aStr[300];

		//FPSの表示
		//DrawFPS();


#endif // _DEBUG

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//========================================
// 読み込み処理
//========================================
void Load(void)
{
	LoadEffect();	// エフェクト
}
//========================================
// FPS処理
//========================================
void DrawFPS(void)
{
	RECT rect = { 0,230,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[521];

	//文字列に代入
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255,255, 255));
}
//========================================
// ウインドウモードの切り替え
//========================================
void WindowMode(bool bScreen)
{
	HDC hDC			// デスクトップのハンドル
		= GetDC(GetDesktopWindow());
	int nScrSizeX	// デスクトップの画面サイズX
		= GetDeviceCaps(hDC, HORZRES);
	int nScrSizeY	// デスクトップの画面サイズY
		= GetDeviceCaps(hDC, VERTRES);

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bScreen)
	{// 全画面フラグが真の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_main.hWnd);
}

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pD3DDevice->Reset(&d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}
