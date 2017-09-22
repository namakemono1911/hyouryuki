/*****************************************************************************
ビルボード
Aythor	: 上野　匠
Data	: 2016_11_22
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	<d3dx9math.h>
#include	"input.h"
#include	"camera.h"
#include	"billboard.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME		("data/texture/kobeni0.png")	//テクスチャのアドレス

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
VERTEX3D g_aVertexBillboard[NUM_VERTEX];

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	makeVertexBillboard	( void );
void	setMatrix			( LPDIRECT3DDEVICE9 pDevice );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
float				angleBillboard	= 0;					//角度
D3DXVECTOR3			posBillboard	= D3DXVECTOR3(0, 0, 0);	//座標
LPDIRECT3DTEXTURE9	g_pTextureBillboard = NULL;				//テクスチャインターフェイス

//////////////////////////////////////////////////////////////////////////////
//関数名	: initBillboard
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initBillboard ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//ポリゴンの描画位置設定
	makeVertexBillboard();

	//テクスチャの読み込み
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureBillboard)))
	{
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitBillboard
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitBillboard	( void )
{
	//テクスチャバッファrelease
	if(g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateBillboard
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updateBillboard	( void )
{
	
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawBillboard
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawBillboard ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	
	//マトリックス設定
	setMatrix(pDevice);

	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//αテスト有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	
	//αテストの条件式
	pDevice->SetRenderState(D3DRS_ALPHAREF, 192);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0,g_pTextureBillboard);

	//ポリゴンの描画
	pDevice->DrawPrimitiveUP
		(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
		NUM_POLYGON,			//ポリゴンの数
		&g_aVertexBillboard[0],	//頂点データの先頭アドレス
		sizeof(VERTEX3D)		//頂点データ1個分のサイズ
		);

	//ライトの影響on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//αテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrix
//返り値	: 
//説明		: マトリックス設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrix	( LPDIRECT3DDEVICE9 pDevice )
{
	//変数宣言
	D3DXMATRIX	world[2];		//ワールド行列
	D3DXMATRIX	ViewInverse;	//カメラの逆行列			

	/*if(GetKeyboardPress(DIK_A) == true)
	{
		angleBillboard += 0.1f;
	}

	if(GetKeyboardPress(DIK_D) == true)
	{
		angleBillboard -= 0.1f;
	}

	if(GetKeyboardPress(DIK_LEFT))
	{
		posBillboard.x -= 1.f;
	}

	if(GetKeyboardPress(DIK_RIGHT))
	{
		posBillboard.x += 1.f;
	}

	if(GetKeyboardPress(DIK_UP))
	{
		posBillboard.y += 1.f;
	}

	if(GetKeyboardPress(DIK_DOWN))
	{
		posBillboard.y -= 1.f;
	}*/

	//逆行列作成
	D3DXMatrixInverse(&ViewInverse, NULL, &getViewMtx());

	ViewInverse._41	=	0.0f;
	ViewInverse._42	=	0.0f;
	ViewInverse._43	=	0.0f;

	//ワールド行列の設定
	D3DXMatrixIdentity(&world[0]);								//行列初期化
	D3DXMatrixIdentity(&world[1]);

	D3DXMatrixRotationYawPitchRoll(&world[0], 0.0f, 0.0f, angleBillboard);	//回転行列
	D3DXMatrixTranslation(&world[1], posBillboard.x, posBillboard.y, posBillboard.z);			//平行移動

	//行列合成
	world[0]	*=	world[1];
	world[0]	*=	ViewInverse;

	pDevice->SetTransform(D3DTS_WORLD, &world[0]);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexBillboard
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
void	makeVertexBillboard	( void )
{
	//頂点座標の設定(2D座標・右回り)
	g_aVertexBillboard[0].pos = D3DXVECTOR3(-0.01f * 243, 0.02f * 432, 0.0f);
	g_aVertexBillboard[1].pos = D3DXVECTOR3(0.01f * 243, 0.02f * 432, 0.0f);
	g_aVertexBillboard[2].pos = D3DXVECTOR3(-0.01f * 243, 0.0f, 0.0f);
	g_aVertexBillboard[3].pos = D3DXVECTOR3(0.01f * 243, 0.0f * 432, 0.0f);

	//法線ベクトルを設定
	g_aVertexBillboard[0].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[1].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[2].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[3].nor = D3DXVECTOR3(0.f, 0.f, -1.f);

	//頂点カラーの設定(0~155の整数値)
	g_aVertexBillboard[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標設定
	g_aVertexBillboard[0].tex = D3DXVECTOR2(0.f, 0.f);
	g_aVertexBillboard[1].tex = D3DXVECTOR2(1.f, 0.f);
	g_aVertexBillboard[2].tex = D3DXVECTOR2(0.f, 1.f);
	g_aVertexBillboard[3].tex = D3DXVECTOR2(1.f, 1.f);
}