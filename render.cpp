/*****************************************************************************
render.cpp
Aythor	: 上野　匠
Data	: 2016_07_05
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "render.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// 変数名	: initDevice
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void normal2D ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	////テクスチャの設定
 //   // テクスチャの設定
 //   pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
 //   pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
 //   pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

 //   // アルファ・ブレンディングを行う
 //   pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
 //   // 透過処理を行う
 //   pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
 //   // 半透明処理を行う
 //   pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	/*pDevice->SetTextureStageState(0,	D3DTSS_COLORARG1	, D3DTA_TEXTURE	);
	pDevice->SetTextureStageState(0,	D3DTSS_COLOROP	, D3DTOP_BLENDTEXTUREALPHA	);
	pDevice->SetTextureStageState(0,	D3DTSS_COLORARG2	, D3DTA_DIFFUSE	);
	pDevice->SetTextureStageState(0,	D3DTSS_ALPHAOP	, D3DTOP_SELECTARG2	);
	pDevice->SetTextureStageState(0,	D3DTSS_ALPHAARG2	, D3DTA_DIFFUSE	);*/

	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1 , D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//頂点フォーマットの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: multiDevice
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void multiDevice ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//頂点フォーマットの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//g_PD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: addDevice
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void addDevice ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//頂点フォーマットの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//テクスチャの色設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: subDevice
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void subDevice ( LPDIRECT3DDEVICE9 pDevice )
{
	//演算方法設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLEND_ONE);

	//合成元の設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//合成先の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: initPolygon
//引数		: *polygon	初期化したいPOLYGON変数
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initPolygon ( POLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size )
{
	polygon->pos = pos;
	polygon->size = size;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	initRotPolygon
//
//引数		:	*polygon	初期化したいPOLYGON変数
//				width		設定したい画像の幅
//				height		設定したい画像の高さ
//
//返り値	:	
//説明		:	 回転ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initRotPolygon ( ROTPOLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size )
{
	//中心座標初期化
	polygon->conf.pos = pos;

	//サイズ初期化
	polygon->conf.size = size;

	//ポリゴンの角度
	polygon->rot = 0.0f;

	//対角線の長さ
	polygon->length = sqrt(size.x * size.x + size.y * size.y) * 0.5f;

	//ポリゴンの対角線の角度
	polygon->angle = atan2(size.y, size.x);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: initTexture
//引数		: *tex		設定したいテクスチャ情報
//			  pos		UV開始位置
//			  size		テクスチャ1つ分のサイズ
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initTexture ( TEXTURE	*tex, D3DXVECTOR2 pos, D3DXVECTOR2 size )
{
	tex->pos = pos;
	tex->size = size;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setTexture
//
//引数		:	*pVtx	設定したい画像のバッファアドレス
//				texture	設定したいTEXTURE型の変数
//
//返り値	: 
//説明		:	ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void setTexture ( VERTEX3D *pVtx, TEXTURE texture )
{
	//左上UV
	pVtx[0].tex = D3DXVECTOR2(
		texture.pos.x,
		texture.pos.y);

	//右上UV
	pVtx[1].tex = D3DXVECTOR2(
		texture.pos.x + texture.size.x,
		texture.pos.y);

	//左下UV
	pVtx[2].tex = D3DXVECTOR2(
		texture.pos.x,
		texture.pos.y + texture.size.y);

	//右下UV
	pVtx[3].tex = D3DXVECTOR2(
		texture.pos.x + texture.size.x,
		texture.pos.y + texture.size.y);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setBuffer
//
//引数		:	*pVtx	設定したい画像のバッファアドレス
//				polygon	バッファに設定したいPOLYGON型変数
//
//返り値	: 
//説明		: バッファに座標を設定
//////////////////////////////////////////////////////////////////////////////
void setBuffer ( VERTEX3D *pVtx, POLYGON polygon )
{
	//左上座標
	pVtx[0].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);
	
	//右上座標
	pVtx[1].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//左下座標
	pVtx[2].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
	
	//右下座標
	pVtx[3].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setListBuffer
//
//引数		:	*pVtx	設定したい画像のバッファアドレス
//				polygon	バッファに設定したいPOLYGON型変数
//
//返り値	: 
//説明		: バッファにトライアングルリストの座標を設定
//////////////////////////////////////////////////////////////////////////////
void setListBuffer ( VERTEX3D *pVtx, POLYGON polygon )
{
	//左上座標
	pVtx[0].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);
	
	//右上座標
	pVtx[1].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//左下座標
	pVtx[2].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);

	//右上座標
	pVtx[3].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//右下座標
	pVtx[4].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);

	//左下座標
	pVtx[5].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setRotBuffer
//
//引数		:	*pVtx	設定したい画像のバッファアドレス
//				polygon	バッファに設定したい回転画像
//
//返り値	: 
//説明		: バッファに回転座標を設定
//////////////////////////////////////////////////////////////////////////////
void setRotBuffer ( VERTEX3D *pVtx, ROTPOLYGON polygon )
{
	//左上座標
	pVtx[0].pos = D3DXVECTOR3(
		-cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
		-sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//右上座標
	pVtx[1].pos = D3DXVECTOR3(
		cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
		sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//左下座標
	pVtx[2].pos = D3DXVECTOR3(
		-cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
		-sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//右下座標
	pVtx[3].pos = D3DXVECTOR3(
		cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
		sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getRotPos
//
//引数		:	polygon	取得したい回転ポリゴン
//
//返り値	: 
//説明		: 回転ポリゴンの座標取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getRotPos	( ROTPOLYGON polygon, int pos )
{
	switch(pos)
	{
	case TOP_LEFT:			//左上の座標
		{
			return D3DXVECTOR3(
				-cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
				-sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case TOP_RIGHT:			//右上の座標
		{
			return D3DXVECTOR3(
				cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
				sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case BOT_RIGHT:			//右下の座標
		{
			return D3DXVECTOR3(
				cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
				sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case BOT_LEFT:			//左下の座標
		{
			return D3DXVECTOR3(
				-cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
				-sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}
	}

	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setDegeneratePolygon
//
//引数		:	polygon	取得したい回転ポリゴン
//
//返り値	: 
//説明		:	縮退ポリゴン設定
//////////////////////////////////////////////////////////////////////////////
void	setDegeneratePolygon	( VERTEX3D *pVtx, POLYGON polygon1, POLYGON polygon2 )
{
	pVtx[4].pos = D3DXVECTOR3(
		polygon1.pos.x + (polygon1.size.x * 0.5f),
		polygon1.pos.y + (polygon1.size.y * 0.5f),
		0.f);

	pVtx[5].pos = D3DXVECTOR3(
		polygon2.pos.x - (polygon2.size.x * 0.5f),
		polygon2.pos.y - (polygon2.size.y * 0.5f),
		0.f);
}