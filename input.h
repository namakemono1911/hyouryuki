//=============================================================================
//
// ���͏��� [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef enum
{
	MOUSE_LEFT = 0,		//���N���b�N
	MOUSE_RIGHT,		//�E�N���b�N
	MOUSE_MIDDLE,		//�X�N���[���{�^��
}MOUSE_BUTTON;

//*****************************************************************************
// �v���g�^�C�v�錾
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