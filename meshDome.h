/*****************************************************************************
メッシュドーム描画
Aythor	: 上野　匠
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
#ifndef	MESH_DOME_H
#define	MESH_DOME_H

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_DOME	(5)		//フィールドの最大数

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	DOME_TEX_001 = 0,
	DOME_TEX_MAX,			//テクスチャ総数
	DOME_TEX_NULL,			//null
}DOME_TEX;

typedef	struct
{
	D3DXVECTOR3	pos;		//表示座標
	D3DXVECTOR2	numTyle;	//縦横何タイルか
	D3DXCOLOR	color;		//頂点色
	D3DXVECTOR3	rot;		//ポリゴンの回転量
	float		len;		//半径
	DOME_TEX	tex;		//テクスチャの種類
	bool		use;		//使用フラグ
	int			id;			//ナンバーID
}DOME;

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshDome		( void );
void	setMeshDome			( D3DXVECTOR3 pos, float len, D3DXVECTOR2 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color, DOME_TEX tex, int id );
D3DXVECTOR3	getDomePos		( int num );
void	uninitMeshDome		( void );
void	updateMeshDome		( void );
void	drawMeshDome		( void );

#endif