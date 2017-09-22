/*****************************************************************************
��̃��f���\��[rock.h]
Aythor	: ���@��
Data	: 2017_02_07
=============================================================================
Updata

*****************************************************************************/
#ifndef ROCK_H
#define ROCK_H

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	ROCK_TYPE_000 = 0,			//��1
	ROCK_TYPE_MAX,				//���f����ނ̍ő吔
	ROCK_TYPE_NULL
}ROCK_TYPE;

typedef struct
{
	D3DXVECTOR3		pos;		//���W
	D3DXVECTOR3		angle;		//�p�x
	D3DXVECTOR3		size;		//�T�C�Y
	D3DXVECTOR3		collision;	//�R���W����							
	bool			use;		//�g�p�t���O
	ROCK_TYPE		type;		//���f���̎��
	int				collisionID;//�R���W����ID
}ROCK;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	initRock	( void );
void	uninitRock	( void );
void	updateRock	( void );
void	drawRock	( void );
ROCK	*getRock	( int num );
void	setRock		( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, ROCK_TYPE type);
void	releaseRock	( int num );

#endif