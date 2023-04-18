//========================================
// 
// imGuiの処理
// author KEISUKE OOTONO
// 
//========================================
#include "imgui_main.h"
#include "Particle\effect.h"
#include "Particle\Particle.h"
//****************************************
// 構造体の定義
//****************************************
// CHR:プレイヤーの設定情報構造体
typedef struct
{
	bool bInfo;	// 情報
}Display;

//========================================
// プロトタイプ宣言
//========================================
void MenuFile(void);	// メニュー処理

/* ヘッダー関連 */
void ImageHeader(EffectType *pEtp, ParticleType *pPType);	// 画像
void ShapeHeader(Particle *pPar, ParticleType *pPType);		// 形状
void ColorHeader(Particle *pPar, ParticleType *pPType);		// 色
void DrawHeader(ParticleType *pPType);						// 描画

/* ウィンドウ関連 */
void ParticleInfo(ParticleType *pPType, Type *pType);		// パーティクルの情報
//========================================
// グローバル変数
//========================================
D3DXCOLOR col;
D3DXVECTOR3 pos;
D3DXVECTOR3 rot;
Display g_Display;
static int g_nIdx; // パーティクルの番号
int nCnt;
ParticleSystem g_ParticleSystem;

ParticleSystem *GetParticleSystem(void)
{
	return &g_ParticleSystem;
}
//========================================
// ヘルプマークの処理
// author KEISUKE OOTONO
//========================================
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
//========================================
// imGuiの初期化処理
// author KEISUKE OOTONO
//========================================
void InitImguiMain(HWND hWnd)
{
	ParticleSystem *pSys = GetParticleSystem();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの情報

	// Show the window
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.Fonts->AddFontFromFileTTF("data//GAMEDATA//FONTS//meiryo.ttc", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);// GetGlyphRangesJapanese

	/* 構造体の初期化 */
	g_Display.bInfo = true;

	pSys->nIdx = 0;
	pSys->nCntTime = 0;
	pSys->nCntLoop = 30;
	pSys->bLoop = false;
}
//========================================
// imGuiの終了処理
// author KEISUKE OOTONO
//========================================
void UninitImguiMain(void)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
//========================================
// imGuiの更新処理
// author KEISUKE OOTONO
//========================================
void UpdateImguiMain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの情報
	ImGuiIO& io = ImGui::GetIO();
	Particle *pPar = GetParticle();
	ParticleType *pPType = GetParticleType();
	ParticleSystem * pSys = GetParticleSystem();
	EffectType *pEtp = GetEffectType();
	Type *pType = GetType();

	// Demonstrate the various window flags.Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;
	ImGuiWindowFlags window_flags = 0;

	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;

	int   bar_data[11] = { 2 };
	float x_data[1000] = {1};
	float y_data[1000] = {3};

	// ImGui関数の呼び出し
	{
		/* ウインドウ */
		ImGui::SetNextWindowPos(ImVec2(20.0f, 30.0f), ImGuiCond_Once);	// 位置
		ImGui::SetNextWindowSize(ImVec2(335, 580), ImGuiCond_Once);		// サイズ
		ImGui::Begin(u8"パーティクル設定								ver1.0", NULL, window_flags);			// 名前

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			MenuFile();

			ImGui::EndMenuBar();
		}

		/* パーティクル */

		ImGui::InputInt(u8"番号", &g_nIdx);
		ImGui::Checkbox(u8"保存", &pPType[g_nIdx].bUse);
		ImGui::SameLine(); HelpMarker(u8"保存するかどうか");

		ImGui::SeparatorText(u8"再生");
		ImGui::Checkbox(u8"連続再生", &pPType[g_nIdx].bCot);

		if (pPType[g_nIdx].bCot)
		{
			ImGui::InputInt(u8"再生する番号", &pPType[g_nIdx].nCotIdx);
		}
		ImGui::SeparatorText(u8"頂点");

		if (ImGui::TreeNode(u8"位置"))
		{

			ImGui::DragFloat3(" ", pPType[g_nIdx].pos);

			if (ImGui::Button(u8"初期化"))
				pPType[g_nIdx].pos = INIT_D3DXVECTOR3;

			ImGui::TreePop();
		}
		if (ImGui::TreeNode(u8"向き"))
		{
			ImGui::DragFloat3(" ", pPType[g_nIdx].rot, 1, 0.0f, 360.0f);
			ImGui::SameLine(); HelpMarker(u8"0～360度　(-3.14～3.14)");
			if (ImGui::Button(u8"初期化"))
				pPType[g_nIdx].rot = INIT_D3DXVECTOR3;

			ImGui::TreePop();
		}

		if (ImGui::CollapsingHeader(u8"形状"))
		{
			// 形状処理
			ShapeHeader(pPar,pPType);
		}
		if (ImGui::CollapsingHeader(u8"色"))
		{
			// 色処理
			ColorHeader(pPar, pPType);
		}

		if (ImGui::CollapsingHeader(u8"描画"))
		{
			if (ImGui::TreeNode(u8"画像"))
			{
				// 画像処理
				ImageHeader(pEtp, pPType);
			}
			// 描画処理
			DrawHeader(pPType);
		}
		if (ImGui::CollapsingHeader(u8"パーティクル設定"))
		{
			if (ImGui::TreeNode(u8"移動速度"))
			{
				const char* sType[] = { u8"通常", u8"乱数",u8"指定" };
				ImGui::Combo(u8"移動方法", &pPType[g_nIdx].sType, sType, IM_ARRAYSIZE(sType));

				if (pPType[g_nIdx].sType == 2)
				{
					ImGui::DragFloat(u8"速度", &pPType[g_nIdx].move.y);
				}
				else
				{
					ImGui::DragFloat3(u8"速度", pPType[g_nIdx].move);
				}


				if (ImGui::Button(u8"初期化"))
				{
					pPType[g_nIdx].move = INIT_D3DXVECTOR3;
					pPType[g_nIdx].sType = 0;
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode(u8"生成数"))
			{
				ImGui::InputInt(" ", &pPType[g_nIdx].Form);
				ImGui::SameLine(); HelpMarker(u8"一度に生成する量");

				if (ImGui::Button(u8"初期化"))
				{
					pPType[g_nIdx].Form = 1;
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode(u8"半径"))
			{
				ImGui::InputFloat(" ", &pPType[g_nIdx].fRadius10, 0.1f, 1.0f, "%.1f");

				if (ImGui::Button(u8"初期化"))
				{
					pPType[g_nIdx].fRadius10 = 1.0f;
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode(u8"フェード"))
			{
				ImGui::DragInt(u8"時間", &pPType[g_nIdx].aDraw.nFadeTime, 1, 0, pPType[g_nIdx].nS_Life);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode(u8"寿命"))
			{
				ImGui::DragInt(u8"メイン", &pPType[g_nIdx].nM_Life, 1, 0, MAX_M_LIFE);
				ImGui::DragInt(u8"サブ", &pPType[g_nIdx].nS_Life, 1, 0, MAX_S_LIFE);

				// 寿命よりフェード時間が長いとき
				if (pPType[g_nIdx].nS_Life <= pPType[g_nIdx].aDraw.nFadeTime)
				{
					pPType[g_nIdx].aDraw.nFadeTime = pPType[g_nIdx].nS_Life;
				}
				// 寿命の最低値固定
				if (pPType[g_nIdx].nM_Life <= 0)
				{
					pPType[g_nIdx].nM_Life = 1;
				}

				if (pPType[g_nIdx].nS_Life <= 0)
				{
					pPType[g_nIdx].nS_Life = 1;
				}

				if (ImGui::Button(u8"初期化"))
				{
					pPType[g_nIdx].nM_Life = 1;
					pPType[g_nIdx].nS_Life = 30;
				}

				ImGui::TreePop();
			}
		}

		/* 終了 */
		ImGui::End();
	}

	{
		if (g_Display.bInfo)
		{
			ParticleInfo(pPType,pType);
			ImGui::End();
		}
	}

	{
		ImGui::SetNextWindowPos(ImVec2(1000.0f, 450.0f), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Once);			// サイズ
		ImGui::Begin(u8"パーティクルエフェクト", &g_Display.bInfo);


		ImGui::InputInt(u8"番号", &pSys->nIdx);
		if (ImGui::Button(u8"再生"))
		{
			if (pSys->bLoop == false)
			{
				SetParticle(pPType[pSys->nIdx].pos,pSys->nIdx);
				pSys->bStop = false;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button(u8"一時停止"))
		{
			if (pSys->bStop)
			{
				pSys->bStop = false;
			}
			else
			{
				pSys->bStop = true;
			}
		}
		ImGui::Checkbox(u8"ループ", &pSys->bLoop);

		if (pSys->bLoop == true)
		{
			ImGui::InputInt(u8"間隔", &pSys->nCntLoop);
		}
		else if (pSys->bLoop == true)
		{
			pSys->nCntTime = 0;
		}


		ImGui::End();
	}

	if (pSys->bLoop)
	{
		if (pSys->nCntTime <= 0)
		{
			SetParticle(pPType[pSys->nIdx].pos, pSys->nIdx);
			pSys->nCntTime = pSys->nCntLoop;
		}
		else
		{
			pSys->nCntTime--;
		}
	}
	ImGui::EndFrame();

}
//========================================
// imGuiの描画処理
// author KEISUKE OOTONO
//========================================
void DrawImguiMain(void)
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//========================================
// メニュー・ファイルの処理
// author KEISUKE OOTONO
//========================================
void MenuFile(void)
{
	if (ImGui::BeginMenu(u8"ファイル"))
	{
		if (ImGui::MenuItem(u8"読み込み"))
		{
			LoadParticleType();
		}
		if (ImGui::MenuItem(u8"書き込み"))
		{
			SaveParticleType();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu(u8"表示"))
	{
		if (ImGui::Button(u8"情報"))
		{
			if (g_Display.bInfo == false)
			{
				g_Display.bInfo = true;
			}
			else if (g_Display.bInfo == true)
			{
				g_Display.bInfo = false;
			}
		}
		ImGui::EndMenu();
	}
}

//========================================
// 画像ヘッダーの処理
// author KEISUKE OOTONO
//========================================
void ImageHeader(EffectType *pEtp, ParticleType *pPType)
{
	static bool disable_mouse_wheel = false;
	static bool disable_menu = true;

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		if (disable_mouse_wheel)
			window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
		for (int nType = 0; nType < NUM_EFFECT; nType++, pEtp++)
		{
			ImGui::Text(u8"%03d:%s", nType, pEtp->aName);
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		if (disable_mouse_wheel)
			window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		if (!disable_menu)
			window_flags |= ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 260), true, window_flags);

		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
		{
			for (int nType = 0; nType < NUM_EFFECT; nType++, pEtp++)
			{

				char buf[32];
				sprintf(buf, "%03d", nType);
				ImGui::TableNextColumn();

				if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
				{
					pPType[g_nIdx].nType = nType;
				}
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	ImGui::TreePop();
}
//========================================
// 形状ヘッダーの処理
// author KEISUKE OOTONO
//========================================
void ShapeHeader(Particle *pPar, ParticleType *pPType)
{
	if (ImGui::TreeNode(u8"形状"))
	{
		const char* Shape[] = { u8"点", u8"円",u8"球"};
		ImGui::Combo(u8" ", &pPType[g_nIdx].nShape, Shape, IM_ARRAYSIZE(Shape));

		if (pPType[g_nIdx].nShape == 1)
		{
			const char* Dir[] = { u8"垂直", u8"水平" };
			ImGui::Combo(u8"方向", &pPType[g_nIdx].aCircle.nDir, Dir, IM_ARRAYSIZE(Dir));

			const char* Lay[] = { u8"間隔", u8"ランダム" };
			ImGui::Combo(u8"配置方法", &pPType[g_nIdx].aCircle.nLay, Lay, IM_ARRAYSIZE(Lay));

			ImGui::InputFloat(u8"半径", &pPType[g_nIdx].aCircle.fRadius, 0.1f, 1.0f, "%.1f");
		}
		else if (pPType[g_nIdx].nShape == 2)
		{
			const char* Form[] = { u8"間隔", u8"ランダム"};
			ImGui::Combo(u8"生成方法", &pPType[g_nIdx].aSphere.nFormType, Form, IM_ARRAYSIZE(Form));
			ImGui::InputInt(u8"球の分割数", &pPType[g_nIdx].aSphere.Sepalate);
			pPType[g_nIdx].aSphere.Split = pPType[g_nIdx].aSphere.Sepalate;
			ImGui::InputFloat(u8"半径", &pPType[g_nIdx].aSphere.fRadius, 0.1f, 1.0f, "%.1f");
		}

		ImGui::TreePop();
	}
}
//========================================
// カラーヘッダーの処理
// author KEISUKE OOTONO
//========================================
void ColorHeader(Particle *pPar, ParticleType *pPType)
{
	static bool alpha = true;
	static bool alpha_bar = true;
	static bool side_preview = true;
	static bool ref_color = false;
	static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
	static int display_mode = 0;
	static int picker_mode = 0;
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	static bool hdr = false;
	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
	static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

	ImGui::Checkbox(u8"ランダムカラー", &pPType[g_nIdx].aColor.bColRandom);

	if (pPType[g_nIdx].aColor.bColRandom)
	{
		ImGui::ColorEdit3(u8"最低値", pPType[g_nIdx].aColor.colRandamMin);
		ImGui::ColorEdit3(u8"最大値", pPType[g_nIdx].aColor.colRandamMax);
	}

	else
	{
		ImGui::Checkbox(u8"aバー表示", &alpha_bar);

		ImGui::Combo(u8"表示設定", &display_mode, u8"全て\0無し\0RGB\0HSV\0Hex\0");

		ImGuiColorEditFlags flags = misc_flags;
		if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
		if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;
		if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
		if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
		if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
		if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
		if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
		if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
		if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;

		ImGui::ColorPicker4(u8"現在の色##4", (float*)pPType[g_nIdx].aColor.col, flags, ref_color ? &ref_color_v.x : NULL);

		if (ImGui::Button(u8"初期化"))
			pPType[g_nIdx].aColor.col = INIT_D3DXCOLOR;
	}
}
//========================================
// 描画ヘッダーの処理
// author KEISUKE OOTONO
//========================================
void DrawHeader(ParticleType *pPType)
{
	const char* AlphaType[] = { u8"加算", u8"減算",u8"乗算" };
	ImGui::Combo(u8"アルファタイプ", &pPType[g_nIdx].aDraw.nAlphaType, AlphaType, IM_ARRAYSIZE(AlphaType));

	ImGui::Checkbox(u8"ライティング", &pPType[g_nIdx].aDraw.bLight);
}
//========================================
// パーティクル情報の処理
// author KEISUKE OOTONO
//========================================
void ParticleInfo(ParticleType *pPType, Type *pType)
{
	ImGui::SetNextWindowPos(ImVec2(1000.0f, 30.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(250, 350), ImGuiCond_Once);		// サイズ
	ImGui::Begin(u8"パーティクル情報", &g_Display.bInfo);
	ImGui::Text(u8"番号　　：(%d)", g_nIdx);
	ImGui::Text(u8"画像　　：(%03d:%s)", pPType[g_nIdx].nType, pType[pPType->nType].aName);
	ImGui::Text(u8"座標　　：(%.1f,%.1f,%.1f)", pPType[g_nIdx].pos.x, pPType[g_nIdx].pos.y, pPType[g_nIdx].pos.z);
	ImGui::Text(u8"向き　　：(%.1f,%.1f,%.1f)", pPType[g_nIdx].rot.x, pPType[g_nIdx].rot.y, pPType[g_nIdx].rot.z);
	ImGui::Text(u8"色　　　：(%.2f,%.2f,%.2f,%.2f)", pPType[g_nIdx].aColor.col.r, pPType[g_nIdx].aColor.col.g, pPType[g_nIdx].aColor.col.b, pPType[g_nIdx].aColor.col.a);
	ImGui::Text(u8"速度　　：(%.1f,%.1f,%.1f)", pPType[g_nIdx].move.x, pPType[g_nIdx].move.y, pPType[g_nIdx].move.z);
	ImGui::Text(u8"生成数　：(%d)", pPType[g_nIdx].Form);
	ImGui::Text(u8"半径　　：(%.1f)", pPType[g_nIdx].fRadius10);
	ImGui::Text(u8"フェード：(%d)", pPType[g_nIdx].aDraw.nFadeTime);
	ImGui::Text(u8"寿命１　：(%d)", pPType[g_nIdx].nM_Life);
	ImGui::Text(u8"寿命２　：(%d)", pPType[g_nIdx].nS_Life);

	if (ImGui::Button(u8"終了"))
		g_Display.bInfo = false;
}
