/*****************************************************************************
���b�V���h�[���`��
Aythor	: ���@��
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
#ifndef	MESH_DOME_H
#define	MESH_DOME_H

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_DOME	(5)		//�t�B�[���h�̍ő吔

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	DOME_TEX_001 = 0,
	DOME_TEX_MAX,			//�e�N�X�`������
	DOME_TEX_NULL,			//null
}DOME_TEX;

typedef	struct
{
	D3DXVECTOR3	pos;		//�\�����W
	D3DXVECTOR2	numTyle;	//�c�����^�C����
	D3DXCOLOR	color;		//���_�F
	D3DXVECTOR3	rot;		//�|���S���̉�]��
	float		len;		//���a
	DOME_TEX	tex;		//�e�N�X�`���̎��
	bool		use;		//�g�p�t���O
	int			id;			//�i���o�[ID
}DOME;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshDome		( void );
void	setMeshDome			( D3DXVECTOR3 pos, float len, D3DXVECTOR2 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color, DOME_TEX tex, int id );
D3DXVECTOR3	getDomePos		( int num );
void	uninitMeshDome		( void );
void	updateMeshDome		( void );
void	drawMeshDome		( void );

#endif