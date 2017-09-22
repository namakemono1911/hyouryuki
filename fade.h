/*****************************************************************************
fade.h
Aythor	: ���@��
Data	: 2016_09_28
=============================================================================
Updata

*****************************************************************************/
#ifndef	FADE_H
#define	FADE_H

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	FADE_IN = 0,		//�t�F�[�h�C��
	FADE_OUT,			//�t�F�[�h�A�E�g
	FADE_MODE_MAX,		//�t�F�[�h���[�h����
	FADE_MODE_NONE		//NULL
}FADE_MODE;


//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	initFade		( void );
void	uninitFade		( void );
void	updateFade		( void );
void	setFade			( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR color, float time, FADE_MODE mode, int id );
void	drawFade		( void );
bool	getFadeEnd		( int id );
void	releaseFade		( int id );


#endif