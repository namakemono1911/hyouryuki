/*****************************************************************************
ポリゴン描画
Aythor	: 上野　匠
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"polygon.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME		("data/texture/kobeni0.png")	//テクスチャのアドレス

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
VERTEX3D g_aVertex[NUM_VERTEX];

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	makeVertexPolygon	( void );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
float				angle = 0;					//角度
D3DXVECTOR3			pos	= D3DXVECTOR3(0, 0, 0);	//座標
LPDIRECT3DTEXTURE9	g_pTexturePolygon = NULL;	//テクスチャインターフェイス

//////////////////////////////////////////////////////////////////////////////
//関数名	: initPolygon
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initPolygon ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//ポリゴンの描画位置設定
	makeVertexPolygon();

	//テクスチャの読み込み
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTexturePolygon)))
	{
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitPolygon
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitPolygon	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updatePolygon
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updatePolygon	( void )
{
	D3DXMATRIX	world[2];					//ワールド行列
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	if(GetKeyboardPress(DIK_A) == true)
	{
		angle += 0.1f;
	}

	if(GetKeyboardPress(DIK_D) == true)
	{
		angle -= 0.1f;
	}

	if(GetKeyboardPress(DIK_LEFT))
	{
		pos.x -= 1.f;
	}

	if(GetKeyboardPress(DIK_RIGHT))
	{
		pos.x += 1.f;
	}

	if(GetKeyboardPress(DIK_UP))
	{
		pos.y += 1.f;
	}

	if(GetKeyboardPress(DIK_DOWN))
	{
		pos.y -= 1.f;
	}

	//ワールド行列の設定
	D3DXMatrixIdentity(&world[0]);								//行列初期化
	D3DXMatrixIdentity(&world[1]);

	D3DXMatrixRotationYawPitchRoll(&world[0], angle, 0.0f, 0.0f);	//回転行列
	D3DXMatrixTranslation(&world[1], pos.x, pos.y, pos.z);			//平行移動

	world[0] *= world[1];

	pDevice->SetTransform(D3DTS_WORLD, &world[0]);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawPolygon
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawPolygon ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0,g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitiveUP
		(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
		NUM_POLYGON,			//ポリゴンの数
		&g_aVertex[0],			//頂点データの先頭アドレス
		sizeof(VERTEX3D)		//頂点データ1個分のサイズ
		);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexPolygon
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
void	makeVertexPolygon	( void )
{
	//頂点座標の設定(2D座標・右回り)
	g_aVertex[0].pos = D3DXVECTOR3(-0.01f * 243, 0.01f * 432, 0.0f);
	g_aVertex[1].pos = D3DXVECTOR3(0.01f * 243, 0.01f * 432, 0.0f);
	g_aVertex[2].pos = D3DXVECTOR3(-0.01f * 243, -0.01f * 432, 0.0f);
	g_aVertex[3].pos = D3DXVECTOR3(0.01f * 243, -0.01f * 432, 0.0f);

	//法線ベクトルを設定
	g_aVertex[0].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[1].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[2].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[3].nor = D3DXVECTOR3(0.f, 0.f, -1.f);

	//頂点カラーの設定(0~155の整数値)
	g_aVertex[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標設定
	g_aVertex[0].tex = D3DXVECTOR2(0.f, 0.f);
	g_aVertex[1].tex = D3DXVECTOR2(1.f, 0.f);
	g_aVertex[2].tex = D3DXVECTOR2(0.f, 1.f);
	g_aVertex[3].tex = D3DXVECTOR2(1.f, 1.f);
}