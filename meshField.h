/*****************************************************************************
メッシュフィールド描画
Aythor	: 上野　匠
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
#ifndef	MESH_FIELD_H
#define	MESH_FIELD_H

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_FIELD	(256)		//フィールドの最大数

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	FIELD_TEX_000	=	0,	//芝生テクスチャ1
	FIELD_TEX_001,			//芝生テクスチャ2
	FIELD_TEX_002,			//波テクスチャ1
	FIELD_TEX_003,			//波テクスチャ2
	FIELD_TEX_MAX,			//テクスチャ総数
	FIELD_TEX_NULL,			//null
}FIELD_TEX;

typedef	struct
{
	D3DXVECTOR3	pos;		//表示座標
	D3DXVECTOR3	size;		//ポリゴンの幅高さ
	D3DXVECTOR3	numTyle;	//縦横何タイルか
	D3DXVECTOR3	rot;		//ポリゴンの回転量
	D3DXCOLOR	color;		//頂点色
	D3DXVECTOR2	texPos;		//テクスチャ座標
	D3DXVECTOR2	texMove;	//テクスチャアニメーションの移動速度
	FIELD_TEX	tex;		//テクスチャの種類
	int			id;			//ナンバーID
	bool		use;		//使用フラグ
}FIELD;
//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
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