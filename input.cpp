//=============================================================================
//
// ���͏��� [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define NUM_MOUSE_MAX		(4)		// �}�E�X�̍ő吔
#define	LIMIT_COUNT_REPEAT	(20)	// ���s�[�g�J�E���g���~�b�^�[

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
    int     x;			//X���W
    int     y;			//Y���W
    int     imgWidth;	//�}�E�X�摜��
    int     imgHeight;	//�}�E�X�摜����
	bool	fixing;		//true�ňʒu�Œ�
}MSTATE;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;					// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��񃏁[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

DIMOUSESTATE			g_mouseState;						//�}�E�X�̏�ԍ\����
MSTATE					g_Mstate;							//�}�E�X�̍��W�Ƃ�������
BYTE					g_mouseStateTrigger[NUM_MOUSE_MAX];	//�}�E�X�̃g���K�[��񃏁[�N
BYTE					g_mouseStateRelease[NUM_MOUSE_MAX];	//�}�E�X�̃����[�X��񃏁[�N

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
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
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	if(g_pInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}


	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();
	g_pDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	if(g_pDevMouse != NULL)
	{// ���̓f�o�C�X(�}�E�X)�̊J��
		// �}�E�X�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE			aKeyState[NUM_KEY_MAX];
	DIMOUSESTATE	mouseState;
	RECT			WinPos;
	POINT			Mpos;

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			//�L�[�g���K�[���̎擾
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

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}

	//�}�E�X
	if(SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{
		//�}�E�X�Œ�
		if(g_Mstate.fixing == true)
		{
			//���݂̃}�E�X���W�擾
			GetCursorPos(&Mpos);
			ScreenToClient(getHwnd(), &Mpos);

			g_mouseState.lX	=	Mpos.x - g_Mstate.x;
			g_mouseState.lY	=	Mpos.y - g_Mstate.y;

			GetWindowRect(getHwnd(), &WinPos);
			SetCursorPos(WinPos.left + (int)SCREEN_WIDTH / 2, WinPos.top + (int)SCREEN_HEIGHT/ 2);
		}

		for(int i = 0; i < NUM_MOUSE_MAX; i++)
		{
			//�}�E�X�g���K�[���̎擾
			g_mouseStateTrigger[i] = (g_mouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & mouseState.rgbButtons[i];
			g_mouseStateRelease[i] = (g_mouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & g_mouseState.rgbButtons[i];

			// �}�E�X�v���X����ۑ�
			g_mouseState.rgbButtons[i] = mouseState.rgbButtons[i];
		}

		if(g_mouseState.lX != 0)
		{
			int i = 0;
		}

		//���݂̃}�E�X���W�擾
		GetCursorPos(&Mpos);
		ScreenToClient(getHwnd(), &Mpos);
		g_Mstate.x = Mpos.x;
		g_Mstate.y = Mpos.y;
	}
	else
	{
		//�}�E�X�ւ̃A�N�Z�X���擾
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̃v���X��Ԃ��擾
//=============================================================================
bool GetMousePress(int button)
{
	return (g_mouseState.rgbButtons[button] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetMouseTrigger(int button)
{
	return (g_mouseStateTrigger[button] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̏�Ԏ擾
//=============================================================================
BYTE GetMouseState	( int num )
{
	return g_mouseState.rgbButtons[num];
}

//=============================================================================
// �}�E�X�g���K�[�̏�Ԏ擾
//=============================================================================
BYTE	GetMouseTriggerState	( int num )
{
	return g_mouseStateTrigger[num];
}

//=============================================================================
// �}�E�X�̃X�N���[�����W�擾
//=============================================================================
D3DXVECTOR2	GetMousePos	( void )
{
	return D3DXVECTOR2((float)g_Mstate.x, (float)g_Mstate.y);
}

//=============================================================================
// �}�E�X�̈ړ��l�擾
//=============================================================================
D3DXVECTOR2	GetMouseMoving	( void )
{
	return D3DXVECTOR2((float)g_mouseState.lX, (float)g_mouseState.lY);
}

//=============================================================================
// �}�E�X���E�B���h�E�̐^�񒆂ɐݒ�
//=============================================================================
void	SetMouseCenter	( void )
{
	//�ϐ��錾
	RECT	WinPos;		//�f�X�N�g�b�v��̃E�C���h�E�̍��W

	GetWindowRect(getHwnd(), &WinPos);
	SetCursorPos((int)WinPos.left + (int)SCREEN_WIDTH / 2, (int)WinPos.top + (int)SCREEN_HEIGHT/ 2);
}

//=============================================================================
// �}�E�X�Œ�
//=============================================================================
void	FixingMouse	( bool fixing )
{
	//�ϐ��錾
	POINT Mpos;

	//�}�E�X�Œ�ɐݒ�
	g_Mstate.fixing = fixing;

	if(g_Mstate.fixing == true)
	{
		//�}�E�X�ړ�
		SetMouseCenter();

		//���݂̃}�E�X���W�擾
		GetCursorPos(&Mpos);
		ScreenToClient(getHwnd(), &Mpos);
		g_Mstate.x = Mpos.x;
		g_Mstate.y = Mpos.y;
	}
	else
	{
		//�}�E�X�ړ��l������
		g_mouseState.lX = 0;
		g_mouseState.lY = 0;
		g_mouseState.lZ = 0;
	}
}