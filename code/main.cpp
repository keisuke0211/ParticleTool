//========================================
// ���C���̏���
// Author:��a�c��
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "imgui_main.h"
#include "sound.h"				// �T�E���h
#include "input.h"				// ����
#include "light.h"				// ���C�g
#include "camera.h"				// �J����
#include "floor.h"				// ��
#include "wall.h"				// ��
#include "Particle\effect.h"	// �G�t�F�N�g
#include "Particle\particle.h"	// �p�[�e�B�N��
//========================================
// �}�N����`
//========================================
#define CLASS_NAME    "WindowClass"		//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME   ""				//�E�C���h�E�̖��O(�L���v�V�����ɕ\��)

// (��ImGui�������蔻��̍ۂ�Direct3D�f�o�C�X�̉�ʃT�C�Y���Q�Ƃ��Ă���A�덷�𖄂߂�ׂ̐��l)
#define IMGUI_WINDOW_CORRECT_X (0.98775f)	// ImGui�p�̃E�B���h�E�̈�␳�lX
#define IMGUI_WINDOW_CORRECT_Y (0.94735f)	// ImGui�p�̃E�B���h�E�̈�␳�lY
//========================================
// �v���g�^�C�v�錾
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void Load(void);
void DrawFPS(void);
void ResetDevice();

// ���b�Z�[�W �n���h��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//========================================
// �O���[�o���ϐ�
//========================================
LPDIRECT3D9		  g_pD3D = NULL;			    // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		    // Direct3D�f�o�C�X�ւ̃|�C���^
D3DPRESENT_PARAMETERS d3dpp = {};				//�v���[���e�[�V�����p�����[�^
LPD3DXFONT g_pFont = NULL;						// �t�H���g�ւ̃|�C���^
int DisplayMode = 1;							// ������@�\���@ON/OFF
int g_nCountFPS = 0;							// FPS�J�E���^

bool bFps = true;
Main	g_main;									// ���C�������̏��\����

//========== *** �E�C���h�E�n���h�����擾 ***
HWND *GetWindowHandle(void)
{
	return &g_main.hWnd;
}
//========== *** �E�C���h�E�̈ʒu���擾 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}
//========================================
// ���̏���
//========================================

//���C�������̏��
Main *GetMain(void)
{
	return &g_main;
}

//========================================
// ���C���֐�
//========================================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

	// �E�C���h�E�N���X�̍\����

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						  //WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								  //�E�C���h�E�̃X�^�C��
		WindowProc,								  //�E�C���h�E�v���V�[�W��
		0,										  //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,										  //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hinstance,								  //�C���X�^���X�n���h��
		(HICON)LoadImage(NULL,"icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),		  //�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),				  //�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),				  //�N���C�A���g�̈�̔w�i�F
		NULL,									  //���j���[�o�[
		CLASS_NAME,								  //�E�C���h�E�N���X�̖��O
		(HICON)LoadImage(NULL,"icon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),		  //�t�@�C���̃A�C�R��
	};
	MSG msg;									  //���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//��ʃT�C�Y�̍\����
	// �E�C���h�E�N���X�̓o�^

	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�̐���
	g_main.hWnd = CreateWindowEx(0, //�g���E�B���h�E�X�^�C��
		CLASS_NAME,          //�E�C���h�E�N���X�̖��O
		WINDOW_NAME,         //�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW, //�E�C���h�E�X�^�C��
		CW_USEDEFAULT,       //�E�C���h�E�̍���X���W
		CW_USEDEFAULT,       //�E�C���h�E�̍���Y���W
		SCREEN_WIDTH,        //�E�C���h�E�̕�
		SCREEN_HEIGHT,       //�E�C���h�E�̍���
		NULL,                //�e�E�C���h�E�̃n���h��
		NULL,                //���j���[�n���h���܂��͎q�E�C���h�EID
		hinstance,           //�C���X�^���X�n���h��
		NULL);               //�E�C���h�E�쐬�f�[

	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	//�E�C���h�E���[�h�̐؂�ւ�
	WindowMode(WINDOW_MODE);

	/* ���������� */
	if (FAILED(Init(hinstance, g_main.hWnd, TRUE)))
	{//�����������Ɏ��s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;				//����������
	dwExecLastTime = timeGetTime();	//���ݎ���������
	dwFrameCount = 0;				//����������
	dwFPSLastTime = timeGetTime();	//���ݎ���������

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���

			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg); //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);  //�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}

		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();//���ݎ���������

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//005�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o�߂�����

				dwExecLastTime = dwCurrentTime;//�����J�n�̎���(���ݎ���)��ۑ�

				RECT rec;	// �傫�����󂯎��RECT�\����

				// �E�C���h�E�̏����擾
				GetWindowRect(g_main.hWnd, &rec);

				// �E�C���h�E�̈ʒu��ݒ�
				g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;//�t���[���J�E���g�����Z
			}
		}
	}
	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//========================================
// �E�C���h�E�v���V�[�W��
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
	case WM_DESTROY: //�E�C���h�E�j�������b�Z�[�W

					 //WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroy���b�Z�[�W�𑗂�
			}
			else
			{
				return 0;
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroy���b�Z�[�W�𑗂�
		}
		else
		{
			return 0;
		}
		break;
	}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);//����̏�����Ԃ�
}

//========================================
// �f�o�C�X�̏���
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//========================================
// ����������
//========================================
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow)//TRUE:�E�C���h�E/FALSE:�t���X�N���[��
{
	D3DDISPLAYMODE d3ddm;        //�f�B�X�v���C���[�h
								 //Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));   //�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH * IMGUI_WINDOW_CORRECT_X;		//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT * IMGUI_WINDOW_CORRECT_Y;	//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;								//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;											//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;								//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;											//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//�C���^�[�o��

																		//Direct3D�f�o�C�X������
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,                     //�`�揈��
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//���_����
		&d3dpp,
		&g_pD3DDevice)))                    //�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,//�`�揈��
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,//���_����
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,                     //�`�揈��
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//���_����
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE,
		"Terminal", &g_pFont);

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//D3DCULL_NONE�@�J�����O���� D3DCULL_CW �\�ʃJ�����O�@D3DCULL_CCW�@���ʃJ�����O
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitInput(hlnstance, hWnd)))
	{
		return E_FAIL;
	}

	// �ǂݍ��ݏ���
	Load();

	// imGui
	InitImguiMain(hWnd);

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�J����
	InitCamera();
	//���C�g
	InitLight();

	//�e��I�u�W�F�N�g�̏���������
	InitFloor();		// ��
	InitWall();			// ��
	InitEffect();		// �G�t�F�N�g
	InitParticle();		// �p�[�e�B�N��
	InitParticleType();	// �p�[�e�B�N���E���
	
	return S_OK;
}
//========================================
// �I������
//========================================
void Uninit(void)
{
	//����������������������������������
	//�@�@�e��I�u�W�F�N�g�̏I������
	//����������������������������������

	/* �I������ */

	UninitImguiMain();	// imGui
	UninitFloor();		// ��
	UninitWall();		// ��
	UninitEffect();		// �G�t�F�N�g
	UninitParticle();	// �p�[�e�B�N��

	//�T�E���h�̏I������
	StopSound();

	UninitSound();

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	
	// �f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	
	// �I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//========================================
// �X�V����
//========================================
void Update(void)
{
	Camera *pCamera = Getcamara(2);
	ParticleSystem * pSys = GetParticleSystem();

	//����������������������������������
	//�@�@�e��I�u�W�F�N�g�̍X�V����
	//����������������������������������

	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�J����
	UpdateCamera();
	//���C�g
	UpdateLight();

	UpdateImguiMain();	// imGui
	if (pSys->bStop == false)
	{
		UpdateEffect();		// �G�t�F�N�g
	}
	UpdateParticle();	// �p�[�e�B�N��
	UpdateFloor();		// ��
	UpdateWall();		// ��
}

//========================================
// �`�揈��
//========================================
void Draw(void)
{
	Camera *pCamera = Getcamara(0);
	D3DVIEWPORT9 veiwportDef;

	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//���݂̃r���[�|�[�g���擾
		g_pD3DDevice->GetViewport(&veiwportDef);

		// �J�����̐�����
		for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++, pCamera++)
		{
			if (pCamera->bUse == true)
			{
				SetCamera(nCntCamera);		// �J����(3D)�̐ݒ菈��

				// ��ʃN���A
				g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				//����������������������������������
				//�@�@�e��I�u�W�F�N�g�̕`�揈��
				//����������������������������������
				

				DrawFloor();	// ��
				//DrawWall();	// ��
				DrawEffect();	// �G�t�F�N�g
				DrawParticle();	// �p�[�e�B�N��
				DrawImguiMain();// imGui
			}
		}

		//�r���[�|�[�g�����ɖ߂�
		g_pD3DDevice->SetViewport(&veiwportDef);

#ifdef _DEBUG
		//�e�L�X�g�̕`�揈��
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

		char aStr[300];

		//FPS�̕\��
		//DrawFPS();


#endif // _DEBUG

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//========================================
// �ǂݍ��ݏ���
//========================================
void Load(void)
{
	LoadEffect();	// �G�t�F�N�g
}
//========================================
// FPS����
//========================================
void DrawFPS(void)
{
	RECT rect = { 0,230,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[521];

	//������ɑ��
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255,255, 255));
}
//========================================
// �E�C���h�E���[�h�̐؂�ւ�
//========================================
void WindowMode(bool bScreen)
{
	HDC hDC			// �f�X�N�g�b�v�̃n���h��
		= GetDC(GetDesktopWindow());
	int nScrSizeX	// �f�X�N�g�b�v�̉�ʃT�C�YX
		= GetDeviceCaps(hDC, HORZRES);
	int nScrSizeY	// �f�X�N�g�b�v�̉�ʃT�C�YY
		= GetDeviceCaps(hDC, VERTRES);

	// �f�X�N�g�b�v�̃n���h���������
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bScreen)
	{// �S��ʃt���O���^�̎��A
	 // �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// �S��ʃt���O���U�̎��A
	 // �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
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
