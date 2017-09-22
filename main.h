/*****************************************************************************
���b�Z�[�WBOX�\������[main.h]
Aythor	: ���@��
Data	: 2016_05_17
=============================================================================
Updata

*****************************************************************************/
#ifndef MAIN_H
#define MAIN_H

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define SCREEN_WIDTH	(1920 * 0.6)		//�E�B���h�E�̍���
#define SCREEN_HEIGHT	(1080 * 0.6)		//�E�B���h�E�̕�

#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2D�|���S���ݒ�
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)	//3D�|���S���ݒ�
#define NUM_VERTEX		(4)			//���_��
#define NUM_POLYGON		(2)			//�|���S����

#define PI				(3.141592f)	//�~����

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <stdio.h>
#include <d3dx9.h>
#define	DisableLoad	(0x0800)
#include <dinput.h>
#include <XAudio2.h>

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
	D3DXVECTOR3 pos;		//���W
	float rhw;
	D3DCOLOR color;			//���_�F
	D3DXVECTOR2	tex;		//�e�N�X�`�����W
}VERTEX2D;		//�|���S���`�ʗp�\����

typedef enum
{
	TITLE_MODE = 0,			//�^�C�g�����[�h
	GAME_MODE,				//�Q�[�����[�h
	RESULT_MODE,			//���U���g���[�h
	MODE_MAX,				//���[�h�ő吔
	MODE_NULL				//null
}MODE;

typedef struct
{
	D3DXVECTOR3	pos;		//���W
	D3DXVECTOR3	nor;		//�@��
	D3DCOLOR	color;		//���_���W
	D3DXVECTOR2	tex;		//�e�N�X�`�����W
}VERTEX3D;

//////////////////////////////////////////////////////////////////////////////
//���C�u�����̃����N
//////////////////////////////////////////////////////////////////////////////
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")		//�V�X�e�������擾�ɕK�v
#pragma comment (lib, "dinput8.lib")	//���͑��u���C�u����

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DDEVICE9	getDevice	( void );
void				setMode		( MODE mode );
HWND				getHwnd		( void );

#endif