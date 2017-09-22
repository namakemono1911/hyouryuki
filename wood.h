/*****************************************************************************
流木のモデル表示[wood.h]
Aythor	: 上野　匠
Data	: 2017_01_18
=============================================================================
Updata

*****************************************************************************/
#ifndef WOOD_H
#define WOOD_H

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef	enum
{
	WOOD_TYPE_000 = 0,			//流木1
	WOOD_TYPE_001,				//流木2
	WOOD_TYPE_MAX,				//モデル種類の最大数
	WOOD_TYPE_NULL
}WOOD_TYPE;

typedef struct
{
	D3DXVECTOR3		pos;		//座標
	D3DXVECTOR3		angle;		//角度
	D3DXVECTOR3		size;		//サイズ
	D3DXVECTOR3		moveVec;	//進行方向
	float			move;		//進行速度
	bool			use;		//使用フラグ
	WOOD_TYPE		type;		//モデルの種類
}WOOD;

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	initWood	( void );
void	uninitWood	( void );
void	updateWood	( void );
void	drawWood	( void );
WOOD	*getWood	( int num );
void	setWood		( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move, WOOD_TYPE type);
void	releaseWood	( int num );

#endif