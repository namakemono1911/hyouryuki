/*****************************************************************************
メッセージBOX表示処理[main.cpp]
Aythor	: 上野　匠
Data	: 2016_05_17
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include <time.h>
#include "input.h"
#include "right.h"
#include "camera.h"
#include "game.h"
#include "title.h"
#include "render.h"
#include "fade.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define CLASS_NAME	"サンプル"		//ウィンドウのクラス名
#define WINDOW_NAME	"ウィンドウ"	//ウィンドウの名前

#define TIMER_INTERVAL	(1000/60)	//待ち時間

#define ID_BUTTON000	(101)		//ボタンID 
#define ID_BUTTON001	(102)
#define ID_EDIT000		(103)		//EDIT
#define ID_EDIT001		(104)
#define ID_TIMER		(105)		//タイマー

#define	POLYGON_WIDTH	(1.f)		//ポリゴンの幅
#define	POLYGON_HEIGHT	(1.f)		//ポリゴンの高さ

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT init (HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void uninit ( void );
void update ( void );
void draw ( void );

#ifdef _DEBUG
void drawFPS ( void );
void	drawOpe	( void );
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3D9			g_PD3D = NULL;				//Direct3Dインターフェイス
LPDIRECT3DDEVICE9	g_PD3DDevice = NULL;		//Direct3Dデバイスのポインタ
MODE				g_mode	=	MODE_NULL;		//ゲームのモード

HWND hWnd;

int g_nCountFPS = NULL;							//FPSカウンター
//デバッグ用変数
#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;	//フォントへのインターフェース
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////
//関数名	: WinMain
//返り値	: 
//説明		: ウィンドウの設定
//////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//変数宣言
	MSG msg;

	DWORD dwFrameCount;
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	//描画ウィンドウサイズ設定
	RECT cr = {10, 10, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT};
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&cr, style, FALSE);

	//デスクトップのサイズ取得
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int ww = cr.right - cr.left;				//ウィンドウの幅
	int wh = cr.bottom - cr.top;				//ウィンドウの高さ

	int dw = dr.right - dr.left;				//デスクトップの幅
	int dh = dr.bottom - dr.top;				//デスクトップの高さ

	int wx = ww > dw ? 0 : (dw - ww) / 2;		//デスクトップの表示位置X
	int wy = wh > dh ? 0 : (dh - wh) / 2;		//デスクトップの表示位置Y

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);				//WNDCLASSEXのメモリサイズを指定
	wcex.style			= CS_CLASSDC;						//表示するウィンドウのスタイルを設定
	wcex.lpfnWndProc	= WndProc;							//ウィンドウプロシージャのアドレス（関数名）を指定する
	wcex.cbClsExtra		= 0;								//通常は使用しないので"0"を指定
	wcex.cbWndExtra		= 0;								//通常は使用しないので"0"を指定
	wcex.hInstance		= hInstance;						//WinMainのパラメータのインスタンスハンドル
	wcex.hIcon			= NULL;								//使用するアイコンを指定（Windowsが持っている
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		//マウスカーソルを指定
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);		//ウィンドウのクライアント領域の背景色を指定
	wcex.lpszMenuName	= NULL;								//Windowにつけるメニューを設定
	wcex.lpszClassName	= CLASS_NAME;						//ウィンドウクラスの名前
	wcex.hIconSm		= NULL;								//小さいアイコンが指定された場合の情報を記述

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウを作成
	hWnd = CreateWindowEx(0,		//拡張ウィンドウスタイル
		CLASS_NAME, WINDOW_NAME,	//ウィンドウクラスの名前
		WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME),		//◎ウィンドウスタイル
		wx,							//ウィンドウの左上X座標
		wy,							//ウィンドウの左上Y座標
		ww,							//ウィンドウの幅
		wh,							//ウィンドウの高さ
		NULL,						//親ウィンドウのハンドル
		NULL,						//メニューハンドルまたは子ウィンドウID
		hInstance,					//インスタンス
		NULL);						//ウィンドウ作成データ

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);		//指定されたウィンドウを表示
	UpdateWindow(hWnd);				//ウィンドウ更新
	

	//変数初期化
	if(FAILED(init(hInstance, hWnd, TRUE)))
	{
		MessageBox(hWnd, "初期化失敗", "エラー", MB_RIGHT);
		return 0;
	}

	//分解能設定
	timeBeginPeriod(1);

	//各カウンターの初期化
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	
#if _DEBUG
		//文字フォント設定
		D3DXCreateFont(g_PD3DDevice,
			18, 
			0, 
			0, 
			0, 
			FALSE, 
			SHIFTJIS_CHARSET, 
			OUT_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, 
			DEFAULT_PITCH, 
			"Tarminal", 
			&g_pFont);
#endif //_DEDBUG

	//メッセージループ
	for(;;)
	{
		//処理時間取得
		dwCurrentTime = timeGetTime();

		//FPS取得
		if((dwCurrentTime - dwFPSLastTime) >= 500)
		{
			g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
			dwFPSLastTime = dwCurrentTime;
			dwFrameCount = 0;
		}

		if(dwCurrentTime - dwExecLastTime >= 1000 / 60)
		{
			//処理した時間取得
			dwExecLastTime = dwCurrentTime;

			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
			{

				//Windowsの処理
				if(msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			//Directxの処理
			update();
			draw();

			dwFrameCount++;
		}
	}
	
	uninit();
	return (int)msg. wParam;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: WndProc
//返り値	: 
//説明		: ウィンドウプロシージャ
//			  (CALLBACK : Win32API関数を呼び出すときの規約
//			  hWnd		: ウィンドウのハンドル
//			  uMsg		: メッセージの識別子
//			  wParam	: メッセージの最初のパラメータ
//			  lParam	: メッセージの2番目のパラメータ
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//変数宣言
	UINT ID;

	//受信メッセージ分岐
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			//セーブの確認余地
			PostQuitMessage(0);
			break;
		}

	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{
					ShowCursor(TRUE);
					ID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONEXCLAMATION);
					if(ID == IDYES)
					{
						DestroyWindow(hWnd);
					}
					else
					{
						ShowCursor(FALSE);
					}

					break;
				}
			}
		}

	default:
		{
			break;
		}
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////////
//関数名	: Init
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
HRESULT init (HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//変数宣言
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//乱数設定
	srand((int)time(NULL));

	//Direct3Dインターフェイスの取得
	g_PD3D = Direct3DCreate9(D3D9b_SDK_VERSION);

	if(g_PD3D == NULL)
	{
		return E_FAIL;
	}

	if(FAILED(g_PD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼントパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = (UINT)SCREEN_WIDTH;
	d3dpp.BackBufferHeight = (UINT)SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの作成
	if(FAILED(g_PD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp, &g_PD3DDevice)))
	{
		//ハードウェアで描画できなかったら頂点処理がsoftware描画がhardwareで処理
		if(FAILED(g_PD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &g_PD3DDevice)))
		{
			//最終手段、全部softwareで処理
			if(FAILED(g_PD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp, &g_PD3DDevice)))
			{
				return E_FAIL;
			}
		}

	}

	//描画モード
	multiDevice();
	
	//キーボード設定の初期化
	InitKeyboard(hInstance, hWnd);

	//ライト初期化
	initRight();

	//カメラ初期化
	initCamera();

	//フェード初期化
	initFade();

	//ゲームモード初期化
	setMode(GAME_MODE);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninit
//返り値	: 
//説明		: 終了処理
//////////////////////////////////////////////////////////////////////////////
void uninit ( void )
{
	//フェード終了
	uninitFade();

	//Direct3Dデバイスの解放
	if(g_PD3DDevice != NULL)
	{
		g_PD3DDevice->Release();
		g_PD3DDevice = NULL;
	}

	//Direct3Dインターフェイスの解放
	if(g_PD3D != NULL)
	{
		g_PD3D->Release();
		g_PD3D = NULL;
	}

	//キーボード終了
	UninitKeyboard();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: update
//返り値	: 
//説明		: 更新処理
//////////////////////////////////////////////////////////////////////////////
void update ( void )
{
	//キーボード更新
	UpdateKeyboard();

	switch(g_mode)
	{
	case TITLE_MODE:
		{
			updateTitle();
			break;
		}

	case GAME_MODE:
		{
			updateGame();
			break;
		}

	case RESULT_MODE:
		{
			break;
		}
	}

	//フェード更新
	updateFade();

	//カメラ更新
	updateCamera();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: draw
//返り値	: 
//説明		: 描画処理
//////////////////////////////////////////////////////////////////////////////
void draw ( void )
{
	//変数宣言
	
	//画面（バックバッファ）
	g_PD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	//描画開始
	if(SUCCEEDED(g_PD3DDevice->BeginScene()))
	{
		switch(g_mode)
		{
		case TITLE_MODE:
			{
				drawTitle();
				break;
			}

		case GAME_MODE:
			{
				drawGame();
				break;
			}

		case RESULT_MODE:
			{
				break;
			}
		}

		//フェード描画
		drawFade();

#ifdef _DEBUG
		//FPSカウンター
		drawFPS();
		drawOpe();
#endif //_DEBUG

		//描画終了
		g_PD3DDevice->EndScene();
	}

	//バックバッファの入れ替え
	g_PD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////////
//関数名	: drawFPS
//返り値	: 
//説明		: FPSカウンター描画
//////////////////////////////////////////////////////////////////////////////
void drawFPS ( void )
{
	//変数宣言
	char aStr[256];
	RECT rc = {0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT};

	sprintf(&aStr[0], "FPS:%d", g_nCountFPS);
	g_pFont->DrawText(NULL, (LPCSTR)aStr, -1, &rc, DT_RIGHT, D3DCOLOR_RGBA(64, 200, 64, 255));
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawOpe
//返り値	: 
//説明		: FPSカウンター描画
//////////////////////////////////////////////////////////////////////////////
void	drawOpe	( void )
{
	//変数宣言
	char	str[256];
	RECT	rc	=	{0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT};
	int		i;
	CAMERA	*camera = getCamera();

	for(i = 0; i < 4; i++)
	{
		sprintf(&str[0], "MOUSE STATE%d:%d\n", i, GetMouseState(i));
		rc.top += 20;
		g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(64, 200, 64, 255));
	}
	
	rc.bottom = (LONG)SCREEN_HEIGHT;
	rc.top = 0;
	rc.left += 300;

	sprintf(&str[0], "MOUSE POS X:%f\n", GetMousePos().x);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	sprintf(&str[0], "MOUSE POS Y:%f\n", GetMousePos().y);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	
	rc.bottom = (LONG)SCREEN_HEIGHT;
	rc.top = 0;
	rc.left += 300;
	
	sprintf(&str[0], "CAMERA ANGLE:%f\n", camera->angleXZ);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	
	sprintf(&str[0], "CAMERA X:%f\n", camera->posAt.x);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	sprintf(&str[0], "CAMERA Y:%f\n", camera->posAt.y);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	sprintf(&str[0], "CAMERA Z:%f\n", camera->posAt.z);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	rc.bottom = (LONG)SCREEN_HEIGHT;
	rc.top = 0;
	rc.left += 300;
	
	sprintf(&str[0], "CAMERA POS EYE X:%f\n", camera->posEye.x);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	sprintf(&str[0], "CAMERA POS EYE Y:%f\n", camera->posEye.y);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

	sprintf(&str[0], "CAMERA POS EYE Z:%f\n", camera->posEye.z);
	rc.top += 20;
	g_pFont->DrawText(NULL, (LPCSTR)str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////
//関数名	: getDevice
//返り値	: 
//説明		: デバイス取得
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DDEVICE9	getDevice	( void )
{
	return	g_PD3DDevice;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMode
//返り値	: 
//説明		: ゲームモード設定
//////////////////////////////////////////////////////////////////////////////
void	setMode	( MODE mode )
{
	//変数宣言
	MODE	oldMode;	//変更前のゲームモード

	oldMode = g_mode;
	g_mode = mode;

	switch(oldMode)
	{
	case TITLE_MODE:
		{
			uninitTitle();
			break;
		}

	case GAME_MODE:
		{
			uninitGame();
			break;
		}

	case RESULT_MODE:
		{
			break;
		}
	}

	switch(g_mode)
	{
	case TITLE_MODE:
		{
			initTitle();
			break;
		}

	case GAME_MODE:
		{
			initGame();
			break;
		}

	case RESULT_MODE:
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getHwnd
//返り値	: 
//説明		: hWnd取得
//////////////////////////////////////////////////////////////////////////////
HWND	getHwnd	( void )
{
	return hWnd;
}