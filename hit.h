/*****************************************************************************
�����蔻�菈��[hit.cpp]
Aythor	: ���@��
Data	: 2017_02_14
=============================================================================
Updata

*****************************************************************************/
#ifndef HIT_H
#define HIT_H

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define MAX_HIT_BOX		(30)		//�q�b�g�{�b�N�X�̍ő吔
#define	MAX_HIT_BALL	(40)		//�q�b�g�{�[���̍ő吔
#define	MAX_HIT_SQUARE	(40)		//�q�b�g�ʂ̍ő吔

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	HIT_BALL_000 = 0,
	HIT_BALL_001,
	HIT_BALL_TEX_MAX
}HIT_BALL_TEX;

typedef enum
{
	TYPE_MAX
}PBJ_TYPE_2D;

typedef enum
{
	TYPE_PLAYER = 0,
	TYPE_BIRD,
	TYPE_SPEAR,
	TYPE_WOOD,
	TYPE_ROCK,
	BOJ_TYPE_MAX,
	BOJ_TYPE_NONE
}OBJ_TYPE_3D;

typedef	struct
{
	D3DXVECTOR3		t_r;					//�E��
	D3DXVECTOR3		t_l;					//����
	D3DXVECTOR3		b_r;					//�E��
	D3DXVECTOR3		b_l;					//����
}HIT_VTX;

typedef struct
{
	D3DXVECTOR3		pos;					//���S�_
	HIT_VTX			vtx;					//��`�̊e���_���
	bool			use;					//�g�p�t���O
}HIT_SQUARE;

typedef struct
{
	D3DXVECTOR3		pos;					//���S�_
	HIT_VTX			top;					//��̎l���_
	HIT_VTX			bottom;					//���̎l���_
	OBJ_TYPE_3D		type;					//�R���W�����̃^�C�v
	bool			use;					//�g�p�t���O
}HIT_BOX;

typedef struct
{
	LPD3DXMESH		mesh;					//���f���̃��b�V�����
	D3DXVECTOR3		pos;					//���S���W
	float			len;					//���a
	OBJ_TYPE_3D		type;					//�I�u�W�F�N�g�̃^�C�v
	bool			use;					//�g�p�t���O
}HIT_BALL;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	initHit			( void );
void	uninitHit		( void );
void	updateHit		( void );
void	drawHit			( void );

int		setHitBox		(D3DXVECTOR3 pos, HIT_VTX top, HIT_VTX bottom, OBJ_TYPE_3D type);
void	setHitBoxVtx	(D3DXVECTOR3 max, D3DXVECTOR3 min, HIT_VTX *top, HIT_VTX *bottom);
int		setHitSquare	(D3DXVECTOR3 pos, HIT_VTX vtx);
int		setHitBall		(D3DXVECTOR3 pos, float len, OBJ_TYPE_3D type);
void	releaseHitBall	(int num);
void	setHitBoxPos	(int num, D3DXVECTOR3 pos);
void	setHitBallPos	(int num, D3DXVECTOR3 pos);
void	getXfileVtx		(char *fileName, D3DXVECTOR3 *max, D3DXVECTOR3 *min);
OBJ_TYPE_3D	getHitBoxType	( int num );
HIT_BOX		getHitBox	( int num );

bool	hitVec			( D3DXVECTOR2 vec1, D3DXVECTOR2 vec2, D3DXVECTOR2 point );
bool	hitFacePoint	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 target );
bool	hitBox			( HIT_BOX box1, HIT_BOX box2 );
bool	hitFaceLine		( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine);
bool	hitBoxLine		( HIT_BOX box, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine );
bool	hitBoall		( HIT_BALL ball1, HIT_BALL ball2 );

#endif