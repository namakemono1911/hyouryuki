/*****************************************************************************
fade.cpp
Aythor	: 上野　匠
Data	: 2016_09_28
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "polygon.h"
#include "fade.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_FADE	(5)

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
	D3DXVECTOR3	pos;	//座標
	D3DXVECTOR3	size;	//サイズ
	D3DXCOLOR	color;	//色
	FADE_MODE	mode;	//フェードモード
	float		time;	//フェードの時間
	int			id;		//フェードのID
	bool		use;	//使用フラグ
	bool		end;	//終わってるかどうか
}FADE;

FADE fade[MAX_FADE];

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexFade ( LPDIRECT3DDEVICE9 pDevice );
void	opeFade			( void );
void	updateFadeIn	( int num );
void	updateFadeOut	( int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;			//テクスチャインターフェイス
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferFade = NULL;		//頂点バッファ管理インターフェイス

int				g_fadeNum = 0;							//使われているフェードの数

//////////////////////////////////////////////////////////////////////////////
// 変数名	: initFade
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void	initFade	( void )
{	
	//変数宣言
	int		i;		//loop
	LPDIRECT3DDEVICE9 pDevice = getDevice();	//デバイスポインタ取得

	//構造体初期化
	for(i = 0; i < MAX_FADE; i++)
	{
		fade[i].use = false;
		fade[i].end	= false;
	}

	//デバイス取得
	makeVertexFade(pDevice);
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: uninitFade
// 返り値	:
// 説明		: デバイスの終了
//////////////////////////////////////////////////////////////////////////////
void uninitFade ( void )
{
	//テクスチャインターフェイスの解放
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//頂点バッファ管理の解放
	if(g_pVtxBufferFade != NULL)
	{
		g_pVtxBufferFade->Release();
		g_pVtxBufferFade = NULL;
	}

	g_fadeNum = 0;
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: updateFade
// 返り値	:
// 説明		: 更新
//////////////////////////////////////////////////////////////////////////////
void	updateFade	( void )
{
	//変数宣言
	VERTEX2D	*pVtx;		
	int			i, j;		//loop

	//モードごと処理
	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			switch(fade[i].mode)
			{
			case FADE_IN:
				{
					updateFadeIn(i);
					break;
				}

			case FADE_OUT:
				{
					updateFadeOut(i);
					break;
				}

			case FADE_MODE_NONE:
				{
					return;
				}
			}

			j++;
		}
	}

	//頂点バッファをロックして仮想アドレスを取得
	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			//頂点座標の設定(2D座標・右回り)
			pVtx[0].pos = D3DXVECTOR3(fade[i].pos.x, fade[i].pos.y, 0.f);
			pVtx[1].pos = D3DXVECTOR3(fade[i].pos.x + fade[i].size.x, fade[i].pos.y, 0.f);
			pVtx[2].pos = D3DXVECTOR3(fade[i].pos.x, fade[i].pos.y + fade[i].size.y, 0.f);
			pVtx[3].pos	= D3DXVECTOR3(fade[i].pos.x + fade[i].size.x, fade[i].pos.y + fade[i].size.y, 0.f);
		
			//頂点カラーの設定(0~155の整数値)
			pVtx[0].color =
			pVtx[1].color = 
			pVtx[2].color = 
			pVtx[3].color = 
			fade[j].color;

			pVtx += NUM_VERTEX;
			j++;
		}
	}
	//私のこころアーンロック♪
	g_pVtxBufferFade->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: updateFadeIn
// 返り値	:
// 説明		: 更新
//////////////////////////////////////////////////////////////////////////////
void	updateFadeIn	( int num )
{
	//変数宣言
	float	percent;	//パーセンテージ

	//割合
	percent = 1.f / fade[num].time;

	//透過度減算
	fade[num].color.a -= percent;

	if(fade[num].color.a <= 0.f)
	{
		fade[num].color.a = 0.f;
		fade[num].use = false;
		fade[num].end = true;
		g_fadeNum--;
	}
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: updateFadeOut
// 返り値	:
// 説明		: 更新
//////////////////////////////////////////////////////////////////////////////
void	updateFadeOut	( int num )
{
	//変数宣言
	float	percent;	//パーセンテージ

	//割合
	percent = 1.f / fade[num].time;

	//透過度減算
	fade[num].color.a += percent;

	if(fade[num].color.a >= 1.f)
	{
		fade[num].color.a = 1.f;
		//fade[num].use = false;
		//g_fadeNum--;
		fade[num].end	= true;
	}
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: setFade
// 返り値	:
// 説明		: フェードの設定
//////////////////////////////////////////////////////////////////////////////
void	setFade	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR color, float time, FADE_MODE mode, int id )
{
	//変数宣言
	int			i;		//loop

	//変数代入
	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].use == false)
		{
			fade[i].pos		=	pos;		//座標
			fade[i].size	=	size;		//サイズ
			fade[i].color	=	color;		//頂点色
			fade[i].time	=	time;		//時間
			fade[i].id		=	id;			//ID
			fade[i].mode	=	mode;		//フェードモード
			fade[i].use		=	true;		//使用フラグ
			fade[i].end		=	false;		//終了フラグ

			g_fadeNum++;					//総数加算
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: drawFade
// 返り値	:
// 説明		: デバイスの初期化
//////////////////////////////////////////////////////////////////////////////
void	drawFade	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	int		i, j;	//loop

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferFade, 0, sizeof(VERTEX2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0,g_pTextureFade);

	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive
				(
				D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
				0,						//オフセット(頂点数)
				NUM_POLYGON				//ポリゴンの数
				);

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexFade
//返り値	: 
//説明		: 頂点作成
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexFade ( LPDIRECT3DDEVICE9 pDevice )
{
	//変数宣言
	VERTEX2D	*pVtx = NULL;;		//仮想アドレス用ポインタ
	int			i;

	//頂点バッファの設定
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX2D) * NUM_VERTEX * MAX_FADE,	//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,				//頂点バッファの使用方法
			FVF_VERTEX_2D,					//
			D3DPOOL_MANAGED,				//メモリ管理方法
			&g_pVtxBufferFade,				//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);
	for(i = 0; i < MAX_FADE; i++)
	{
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;
		pVtx += NUM_VERTEX;
	}
	g_pVtxBufferFade->Unlock();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: getFadeEnd
// 返り値	:
// 説明		: 終わってるかどうか取得
//////////////////////////////////////////////////////////////////////////////
bool	getFadeEnd	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].id == id && fade[i].end == true)
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// 変数名	: releaseFade
// 返り値	:
// 説明		: フェード解放
//////////////////////////////////////////////////////////////////////////////
void	releaseFade	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].id == id)
		{
			fade[i].use = false;
			fade[i].end = false;
		}
	}
}