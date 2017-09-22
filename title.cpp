/*****************************************************************************
タイトル
Aythor	: 上野　匠
Data	: 2017_02_15
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//インクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
#define TEX_NAME	"data/texture/title.png"

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexTitle ( LPDIRECT3DDEVICE9 pDevice );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureTitle = NULL;			//テクスチャインターフェイス
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferTitle = NULL;		//頂点バッファ管理インターフェイス

//////////////////////////////////////////////////////////////////////////////
//関数名	:	initTitle
//返り値	:	
//説明		:	タイトルに必要なファイルの初期化
//////////////////////////////////////////////////////////////////////////////
HRESULT	initTitle	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();	//デバイスポインタ取得
	
	//テクスチャ読み込み
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureTitle)))
	{
		MessageBox(NULL, "エラー", "titleテクスチャエラー", MB_OK);
	}

	//デバイス取得
	makeVertexTitle(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	uninitTitle
//返り値	:	
//説明		:	タイトルに必要なファイルの終了
//////////////////////////////////////////////////////////////////////////////
void	uninitTitle	( void )
{

	//テクスチャインターフェイスの解放
	if(g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//頂点バッファ管理の解放
	if(g_pVtxBufferTitle != NULL)
	{
		g_pVtxBufferTitle->Release();
		g_pVtxBufferTitle = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateTitle
//返り値	:	
//説明		:	タイトルに必要なファイルの更新
//////////////////////////////////////////////////////////////////////////////
void	updateTitle	( void )
{
	if(GetKeyboardTrigger(DIK_RETURN) || GetMouseTrigger(MOUSE_LEFT))
	{
		setFade(D3DXVECTOR3(0.f, 0.f, 0.f),
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f),
			D3DXCOLOR(1.f, 1.f, 1.f, 0.f),
			100,
			FADE_OUT,
			0);
	}
	
	if(getFadeEnd(0) == true)
	{
		releaseFade(0);
		setMode(GAME_MODE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	drawTitle
//返り値	:	
//説明		:	タイトルに必要なファイルの描画
//////////////////////////////////////////////////////////////////////////////
void	drawTitle	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	int		i, j;	//loop

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferTitle, 0, sizeof(VERTEX2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0,g_pTextureTitle);

	//ポリゴンの描画
	pDevice->DrawPrimitive
		(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
		0,						//オフセット(頂点数)
		NUM_POLYGON				//ポリゴンの数
		);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexTitle
//返り値	: 
//説明		: 頂点作成
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexTitle ( LPDIRECT3DDEVICE9 pDevice )
{
	//変数宣言
	VERTEX2D	*pVtx = NULL;;		//仮想アドレス用ポインタ
	int			i;

	//頂点バッファの設定
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX2D) * NUM_VERTEX,	//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,				//頂点バッファの使用方法
			FVF_VERTEX_2D,					//
			D3DPOOL_MANAGED,				//メモリ管理方法
			&g_pVtxBufferTitle,				//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	g_pVtxBufferTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos	=	D3DXVECTOR3(0.f, 0.f, 0.f);
	pVtx[1].pos	=	D3DXVECTOR3(SCREEN_WIDTH, 0.f, 0.f);
	pVtx[2].pos	=	D3DXVECTOR3(0.f, SCREEN_HEIGHT, 0.f);
	pVtx[3].pos	=	D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);
	
	pVtx[0].tex	=	D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex	=	D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex	=	D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex	=	D3DXVECTOR2(1.f, 1.f);

	//頂点カラーの設定(0~155の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx += NUM_VERTEX;

	g_pVtxBufferTitle->Unlock();

	return S_OK;
}