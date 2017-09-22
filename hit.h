/*****************************************************************************
当たり判定処理[hit.cpp]
Aythor	: 上野　匠
Data	: 2017_02_14
=============================================================================
Updata

*****************************************************************************/
#ifndef HIT_H
#define HIT_H

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define MAX_HIT_BOX		(30)		//ヒットボックスの最大数
#define	MAX_HIT_BALL	(40)		//ヒットボールの最大数
#define	MAX_HIT_SQUARE	(40)		//ヒット面の最大数

//////////////////////////////////////////////////////////////////////////////
//構造体定義
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
	D3DXVECTOR3		t_r;					//右上
	D3DXVECTOR3		t_l;					//左上
	D3DXVECTOR3		b_r;					//右下
	D3DXVECTOR3		b_l;					//左下
}HIT_VTX;

typedef struct
{
	D3DXVECTOR3		pos;					//中心点
	HIT_VTX			vtx;					//矩形の各頂点情報
	bool			use;					//使用フラグ
}HIT_SQUARE;

typedef struct
{
	D3DXVECTOR3		pos;					//中心点
	HIT_VTX			top;					//上の四頂点
	HIT_VTX			bottom;					//下の四頂点
	OBJ_TYPE_3D		type;					//コリジョンのタイプ
	bool			use;					//使用フラグ
}HIT_BOX;

typedef struct
{
	LPD3DXMESH		mesh;					//モデルのメッシュ情報
	D3DXVECTOR3		pos;					//中心座標
	float			len;					//半径
	OBJ_TYPE_3D		type;					//オブジェクトのタイプ
	bool			use;					//使用フラグ
}HIT_BALL;

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
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