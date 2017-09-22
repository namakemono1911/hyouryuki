//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define NUM_MOUSE_MAX		(4)		// マウスの最大数
#define	LIMIT_COUNT_REPEAT	(20)	// リピートカウントリミッター

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
    int     x;			//X座標
    int     y;			//Y座標
    int     imgWidth;	//マウス画像幅
    int     imgHeight;	//マウス画像高さ
	bool	fixing;		//trueで位置固定
}MSTATE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;					// 入力デバイス(マウス)へのポインタ
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

DIMOUSESTATE			g_mouseState;						//マウスの状態構造体
MSTATE					g_Mstate;							//マウスの座標とかあるやつ
BYTE					g_mouseStateTrigger[NUM_MOUSE_MAX];	//マウスのトリガー情報ワーク
BYTE					g_mouseStateRelease[NUM_MOUSE_MAX];	//マウスのリリース情報ワーク

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	g_Mstate.fixing = false;

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}


	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();
	g_pDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	if(g_pDevMouse != NULL)
	{// 入力デバイス(マウス)の開放
		// マウスへのアクセス権を開放(入力制御終了)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE			aKeyState[NUM_KEY_MAX];
	DIMOUSESTATE	mouseState;
	RECT			WinPos;
	POINT			Mpos;

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			//キートリガー情報の取得
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];


			if(aKeyState[nCnKey] & 0x80)
			{
				if(g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					
					if(g_aKeyState[nCnKey] == 1 || g_aKeyStateRepeatCnt[nCnKey] >=  LIMIT_COUNT_REPEAT)
					{
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
				}
				else
				{
					g_aKeyStateRepeat[nCnKey] = 0;
				}
			}
			else
			{
				g_aKeyStateRepeat[nCnKey] = 0;
				g_aKeyStateRepeatCnt[nCnKey] = 0;
			}

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}

	//マウス
	if(SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{
		//マウス固定
		if(g_Mstate.fixing == true)
		{
			//現在のマウス座標取得
			GetCursorPos(&Mpos);
			ScreenToClient(getHwnd(), &Mpos);

			g_mouseState.lX	=	Mpos.x - g_Mstate.x;
			g_mouseState.lY	=	Mpos.y - g_Mstate.y;

			GetWindowRect(getHwnd(), &WinPos);
			SetCursorPos(WinPos.left + (int)SCREEN_WIDTH / 2, WinPos.top + (int)SCREEN_HEIGHT/ 2);
		}

		for(int i = 0; i < NUM_MOUSE_MAX; i++)
		{
			//マウストリガー情報の取得
			g_mouseStateTrigger[i] = (g_mouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & mouseState.rgbButtons[i];
			g_mouseStateRelease[i] = (g_mouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & g_mouseState.rgbButtons[i];

			// マウスプレス情報を保存
			g_mouseState.rgbButtons[i] = mouseState.rgbButtons[i];
		}

		if(g_mouseState.lX != 0)
		{
			int i = 0;
		}

		//現在のマウス座標取得
		GetCursorPos(&Mpos);
		ScreenToClient(getHwnd(), &Mpos);
		g_Mstate.x = Mpos.x;
		g_Mstate.y = Mpos.y;
	}
	else
	{
		//マウスへのアクセス権取得
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// マウスのプレス状態を取得
//=============================================================================
bool GetMousePress(int button)
{
	return (g_mouseState.rgbButtons[button] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// マウスのトリガー状態を取得
//=============================================================================
bool GetMouseTrigger(int button)
{
	return (g_mouseStateTrigger[button] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// マウスの状態取得
//=============================================================================
BYTE GetMouseState	( int num )
{
	return g_mouseState.rgbButtons[num];
}

//=============================================================================
// マウストリガーの状態取得
//=============================================================================
BYTE	GetMouseTriggerState	( int num )
{
	return g_mouseStateTrigger[num];
}

//=============================================================================
// マウスのスクリーン座標取得
//=============================================================================
D3DXVECTOR2	GetMousePos	( void )
{
	return D3DXVECTOR2((float)g_Mstate.x, (float)g_Mstate.y);
}

//=============================================================================
// マウスの移動値取得
//=============================================================================
D3DXVECTOR2	GetMouseMoving	( void )
{
	return D3DXVECTOR2((float)g_mouseState.lX, (float)g_mouseState.lY);
}

//=============================================================================
// マウスをウィンドウの真ん中に設定
//=============================================================================
void	SetMouseCenter	( void )
{
	//変数宣言
	RECT	WinPos;		//デスクトップ上のウインドウの座標

	GetWindowRect(getHwnd(), &WinPos);
	SetCursorPos((int)WinPos.left + (int)SCREEN_WIDTH / 2, (int)WinPos.top + (int)SCREEN_HEIGHT/ 2);
}

//=============================================================================
// マウス固定
//=============================================================================
void	FixingMouse	( bool fixing )
{
	//変数宣言
	POINT Mpos;

	//マウス固定に設定
	g_Mstate.fixing = fixing;

	if(g_Mstate.fixing == true)
	{
		//マウス移動
		SetMouseCenter();

		//現在のマウス座標取得
		GetCursorPos(&Mpos);
		ScreenToClient(getHwnd(), &Mpos);
		g_Mstate.x = Mpos.x;
		g_Mstate.y = Mpos.y;
	}
	else
	{
		//マウス移動値初期化
		g_mouseState.lX = 0;
		g_mouseState.lY = 0;
		g_mouseState.lZ = 0;
	}
}