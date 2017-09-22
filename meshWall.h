/*****************************************************************************
���b�V���E�H�[���`��
Aythor	: ���@��
Data	: 2016_12_10
=============================================================================
Updata

*****************************************************************************/
#ifndef	MESH_WALL_H
#define	MESH_WALL_H

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_WALL	(256)		//�t�B�[���h�̍ő吔

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	WALL_TEX_000	=	0,	//�����e�N�X�`��1
	WALL_TEX_MAX,			//�e�N�X�`������
	WALL_TEX_NULL,			//null
}WALL_TEX;

typedef	struct
{
	D3DXVECTOR3	pos;		//�\�����W
	D3DXVECTOR3	size;		//�|���S���̕�����
	D3DXVECTOR3	numTyle;	//�c�����^�C����
	D3DXVECTOR3	rot;		//�|���S���̉�]��
	WALL_TEX	tex;		//�e�N�X�`���̎��
	bool		use;		//�g�p�t���O
}WALL;
//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshWall		( void );
void	setMeshWall			( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot, WALL_TEX tex );
D3DXVECTOR3	getWallPos		( int num );
void	uninitMeshWall		( void );
void	updateMeshWall		( void );
void	drawMeshWall		( void );

#endif