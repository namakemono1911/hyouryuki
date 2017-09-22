/*****************************************************************************
���b�V���t�B�[���h�`��
Aythor	: ���@��
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
#ifndef	MESH_FIELD_H
#define	MESH_FIELD_H

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_FIELD	(256)		//�t�B�[���h�̍ő吔

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	FIELD_TEX_000	=	0,	//�Ő��e�N�X�`��1
	FIELD_TEX_001,			//�Ő��e�N�X�`��2
	FIELD_TEX_002,			//�g�e�N�X�`��1
	FIELD_TEX_003,			//�g�e�N�X�`��2
	FIELD_TEX_MAX,			//�e�N�X�`������
	FIELD_TEX_NULL,			//null
}FIELD_TEX;

typedef	struct
{
	D3DXVECTOR3	pos;		//�\�����W
	D3DXVECTOR3	size;		//�|���S���̕�����
	D3DXVECTOR3	numTyle;	//�c�����^�C����
	D3DXVECTOR3	rot;		//�|���S���̉�]��
	D3DXCOLOR	color;		//���_�F
	D3DXVECTOR2	texPos;		//�e�N�X�`�����W
	D3DXVECTOR2	texMove;	//�e�N�X�`���A�j���[�V�����̈ړ����x
	FIELD_TEX	tex;		//�e�N�X�`���̎��
	int			id;			//�i���o�[ID
	bool		use;		//�g�p�t���O
}FIELD;
//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT		initMeshField		( void );
void		setMeshField		( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color ,D3DXVECTOR2 texMove, FIELD_TEX tex, int id );
void		uninitMeshField		( void );
void		updateMeshField		( void );
void		drawMeshField		( void );
FIELD		*getField			( int id );
D3DXVECTOR3	getFieldPos			( int id );
D3DXVECTOR3	getFieldSize		( int id );
D3DXCOLOR	getFieldColor		( int id );
D3DXVECTOR2	getFieldTexMove		( int id );
FIELD_TEX	getFieldTex			( int id );
void		setFieldPos			( D3DXVECTOR3 pos, int id );
void		releaseField		( int id );


#endif