/*****************************************************************************
render.h
Aythor	: ���@��
Data	: 2016_07_05
=============================================================================
Updata

*****************************************************************************/
#ifndef RENDER_H
#define RENDER_H

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	TOP_LEFT = 0,			//����
	TOP_RIGHT,				//�E��
	BOT_RIGHT,				//�E��
	BOT_LEFT,				//����
	ROT_POS_MAX,			//���_����
	ROT_POS_NONE			//����NULL
}ROT_POS;					//��]�|���S���̗~�������W�ʒu

typedef struct
{
	D3DXVECTOR3	pos;		//�\�����W
	D3DXVECTOR3	size;		//�|���S���̕�����
}POLYGON;

typedef struct
{
	POLYGON		conf;		//���S���W�ƃT�C�Y
	float	rot;			//�|���S���̉�]��
	FLOAT	length;			//�|���S���̑Ίp���̒���
	float	angle;			//�|���S���̑Ίp���̊p�x
}ROTPOLYGON;

typedef struct
{
	D3DXVECTOR2	pos;		//�e�N�X�`���̕`��ʒu
	D3DXVECTOR2	size;		//�e�N�X�`���̃T�C�Y
}TEXTURE;

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void		normal2D				( void );
void		multiDevice				( void );
void		addDevice				( void );
void		subDevice				( LPDIRECT3DDEVICE9 pDevice );
void		initPolygon				( POLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size );
void		initRotPolygon			( ROTPOLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size );
void		initTexture				( TEXTURE	*tex, D3DXVECTOR2 pos, D3DXVECTOR2 size );
void		setTexture				( VERTEX3D *pVtx, TEXTURE texture );
void		setBuffer				( VERTEX3D *pVtx, POLYGON polygon );
void		setListBuffer			( VERTEX3D *pVtx, POLYGON polygon );
void		setRotBuffer			( VERTEX3D *pVtx, ROTPOLYGON polygon );
void		setDegenerate			( VERTEX3D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 nextPos );
D3DXVECTOR3	getRotPos				( ROTPOLYGON polygon, int pos );
void		setDegeneratePolygon	( VERTEX3D *pVtx, POLYGON polygon1, POLYGON polygon2 );

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

#endif