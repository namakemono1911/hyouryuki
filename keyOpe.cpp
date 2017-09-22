/*****************************************************************************
�L�[����
Aythor	: ���@��
Data	: 2016_12_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"input.h"
#include	"vector.h"
#include	"camera.h"
#include	"meshField.h"
#include	"game.h"
#include	"keyOpe.h"
#include	"model.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define MOUSE_SPEED		(0.005f)

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	PLAY_FIRST = 0,
	PLAY_THIRD,
}PLAY_MODE;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	cameraOpe	( void );
void	playerOpe	( void );
void	craftOpe	( void );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
OPE_MODE	g_opeMode = OPE_NULL;

float		g_length = 0;		//���f������J�����̋���
D3DXVECTOR3	g_cameraPos;		//�J�����̈ʒu
PLAY_MODE	g_playMode = PLAY_FIRST;			//���샂�[�h

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initKeyOpe
//�Ԃ�l	: 
//����		: �L�[��������
//////////////////////////////////////////////////////////////////////////////
void	initKeyOpe	( void )
{
	//�ϐ��錾
	CAMERA	*Camera = getCamera();

	g_opeMode = OPE_PLAY;
	Camera->angleXZ = PI * 0.5f;
	g_cameraPos = D3DXVECTOR3(0.f, 10.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitKeyOpe
//�Ԃ�l	: 
//����		: �L�[����I��
//////////////////////////////////////////////////////////////////////////////
void	uninitKeyOpe	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateKeyOpe
//�Ԃ�l	: 
//����		: �L�[����X�V
//////////////////////////////////////////////////////////////////////////////
void	updateKeyOpe	( void )
{
	switch(g_opeMode)
	{
	case OPE_PLAY:
		{
			//�v���C���[����
			playerOpe();

			//�J��������
			cameraOpe();

			break;
		}

	case OPE_CRAFT:
		{
			//�N���t�g����
			craftOpe();

			break;
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{
		setMode(TITLE_MODE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateKeyOpe
//�Ԃ�l	: 
//����		: �Q�[�����̃L�[����
//////////////////////////////////////////////////////////////////////////////
void	gameKeyOpe	( int num )
{

}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: cameraOpe
//�Ԃ�l	: 
//����		: �J��������
//////////////////////////////////////////////////////////////////////////////
void	cameraOpe	( void )
{
	//�ϐ��錾
	CAMERA		*Camera	=	getCamera();
	MODEL		*Model	=	getModel();
	D3DXVECTOR3	cameraVec;
	float		z;

	//=============================
	// �����_�ړ�
	//=============================
	Camera->angleXZ	+=	GetMouseMoving().x * MOUSE_SPEED;
	Camera->angleYZ	+=	GetMouseMoving().y * MOUSE_SPEED;
	Camera->angleXZ	=	fmod(Camera->angleXZ, PI * 2);
	if(Camera->angleXZ <= 0)
	{
		Camera->angleXZ += PI * 2;
	}

	if(Camera->angleYZ > PI * 0.5f)
	{
		Camera->angleYZ	=	PI * 0.5f;
	}
	if(Camera->angleYZ < -PI * 0.5f)
	{
		Camera->angleYZ = -PI * 0.5f;
	}

	Camera->posAt	=	D3DXVECTOR3(
		Camera->len * cos(Camera->angleXZ) + Model->pos.x,
		Camera->len * sin(Camera->angleYZ) + Model->pos.y,
		Model->pos.z);

	z = sqrt(Camera->len * Camera->len - (Camera->len * cos(Camera->angleXZ) * Camera->len * cos(Camera->angleXZ)));
	
	if(Camera->angleXZ >= PI)
	{
		z *= -1;
	}
	Camera->posAt.z += z;

	//=============================
	// ���s�ړ�
	//=============================
	cameraVec = Camera->posAt - Model->pos;

	Camera->posEye	=	D3DXVECTOR3(
		-cameraVec.x * g_length + Model->pos.x + g_cameraPos.x,
		-cameraVec.y * g_length + Model->pos.y + g_cameraPos.y,
		-cameraVec.z * g_length + Model->pos.z + g_cameraPos.z);

	//=============================
	//�O�l�̂ɐ؂�ւ� 
	//=============================
	if(GetMouseTrigger(MOUSE_MIDDLE))
	{
		if(g_playMode == PLAY_FIRST)
		{
			g_length += 5;
			g_cameraPos.y = 30.f;
			g_playMode = PLAY_THIRD;
		}

		else if(g_playMode == PLAY_THIRD)
		{
			g_length = 0;
			g_cameraPos.y = 10;
			g_playMode = PLAY_FIRST;
		}
	}

	GetMousePos();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: playerOpe
//�Ԃ�l	: 
//����		: �J��������
//////////////////////////////////////////////////////////////////////////////
void	playerOpe	( void )
{
	//�ϐ��錾
	CAMERA		*Camera	=	getCamera();		//�J�����\���̂̃|�C���^
	MODEL		*Model	=	getModel();			//���f���\���̂̃|�C���^
	D3DXVECTOR3	posEye, posAt;

	if(GetKeyboardPress(DIK_W))
	{
		Model->pos = D3DXVECTOR3(
			Model->MoveVec.x * Model->move + Model->pos.x,
			Model->MoveVec.y * Model->move + Model->pos.y,
			Model->MoveVec.z * Model->move + Model->pos.z);
	}

	if(GetKeyboardPress(DIK_A))
	{
		Model->angle.x -= 0.04f;
	}

	if(GetKeyboardPress(DIK_D))
	{
		Model->angle.x += 0.04f;
	}

	Model->MoveVec = D3DXVECTOR3(
		sin(Model->angle.x + PI),
		0.f,
		cos(Model->angle.x + PI));

	if(GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
	{
		posEye = Camera->posEye;
		posEye.y = 0.f;

		posAt = Camera->posAt;
		posAt.y = 0.f;

		posAt -= posEye;
		posEye = D3DXVECTOR3(0.f, 0.f, -Camera->len);

		Model->angle.x = (float)getAngleVector(posEye, posAt);
		if(posAt.x < 0)
		{
			Model->angle *= -1;
		}
	}

	/*
	if(GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{
		Model->pos = D3DXVECTOR3(
			Camera->posEye.x,
			Model->pos.y,
			Camera->posEye.z);
	}
	*/
	if(GetKeyboardTrigger(DIK_TAB))
	{
		//�N���t�g���[�h�Ɉڍs
		g_opeMode = OPE_CRAFT;
		
		//�J��������ςɕύX
		g_length = 0;
		g_cameraPos.y = 10;
		g_playMode = PLAY_FIRST;

		//�}�E�X���얳��
		ShowCursor(TRUE);
		FixingMouse(false);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: craftOpe
//�Ԃ�l	: 
//����		: �N���t�g����
//////////////////////////////////////////////////////////////////////////////
void	craftOpe	( void )
{
	if(GetKeyboardTrigger(DIK_TAB))
	{
		//�Q�[�����[�h�ɖ߂�
		g_opeMode = OPE_PLAY;

		//�}�E�X����L��
		ShowCursor(FALSE);
		FixingMouse(true);
	}
}