/*****************************************************************************
ベクトルを使うのに便利なファイル[vector.cpp]
Aythor	: 上野　匠
Data	: 2017_01_12
=============================================================================
Updata

*****************************************************************************/
#ifndef VECTOR_H
#define VECTOR_H
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
double	getLenVector	( D3DXVECTOR3 vec );
double	getInnerProduct	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 );
double	getAngleVector	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 );
double	getPosLen		( D3DXVECTOR3 line1, D3DXVECTOR3 line2 );
D3DXVECTOR3	getCrossProduct	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 );
D3DXVECTOR3	getNormalVec	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 );
D3DXVECTOR3	vec3Normalize	( D3DXVECTOR3 vec );

#endif