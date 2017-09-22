/*****************************************************************************
render.h
Aythor	: 上野　匠
Data	: 2016_07_05
=============================================================================
Updata

*****************************************************************************/
#ifndef RENDER_H
#define RENDER_H

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	TOP_LEFT = 0,			//左上
	TOP_RIGHT,				//右上
	BOT_RIGHT,				//右下
	BOT_LEFT,				//左下
	ROT_POS_MAX,			//頂点総数
	ROT_POS_NONE			//実質NULL
}ROT_POS;					//回転ポリゴンの欲しい座標位置

typedef struct
{
	D3DXVECTOR3	pos;		//表示座標
	D3DXVECTOR3	size;		//ポリゴンの幅高さ
}POLYGON;

typedef struct
{
	POLYGON		conf;		//中心座標とサイズ
	float	rot;			//ポリゴンの回転量
	FLOAT	length;			//ポリゴンの対角線の長さ
	float	angle;			//ポリゴンの対角線の角度
}ROTPOLYGON;

typedef struct
{
	D3DXVECTOR2	pos;		//テクスチャの描画位置
	D3DXVECTOR2	size;		//テクスチャのサイズ
}TEXTURE;

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
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
//構造体定義
//////////////////////////////////////////////////////////////////////////////

#endif