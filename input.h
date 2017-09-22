//=============================================================================
//
// 入力処理 [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	MOUSE_LEFT = 0,		//左クリック
	MOUSE_RIGHT,		//右クリック
	MOUSE_MIDDLE,		//スクロールボタン
}MOUSE_BUTTON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);
bool GetMousePress(int button);
bool GetKeyboardTrigger(int nKey);
bool GetMouseTrigger(int button);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);
BYTE GetMouseState( int num );
BYTE GetMouseTriggerState( int num );
D3DXVECTOR2	GetMousePos	( void );
D3DXVECTOR2	GetMouseMoving	( void );
void	SetMouseCenter	( void );
void	FixingMouse	( bool fixing );

#endif