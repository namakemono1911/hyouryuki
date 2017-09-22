/*****************************************************************************
当たり判定処理[hit.cpp]
Aythor	: 上野　匠
Data	: 2017_02_14
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "hit.h"
#include "vector.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexHit ( LPDIRECT3DDEVICE9 pDevice );
void	setMatrixHit	( LPDIRECT3DDEVICE9 pDev, int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitBox = NULL;		//頂点バッファ管理インターフェイス
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitSquare = NULL;	//
LPDIRECT3DTEXTURE9		g_pTexBufferHitBall[HIT_BALL_TEX_MAX] = {NULL};		//球のテクスチャ

HIT_SQUARE	g_hitSquare[MAX_HIT_SQUARE];	//ヒット面構造体	
HIT_BOX		g_hitBox[MAX_HIT_BOX];			//ヒットボックス構造体
HIT_BALL	g_hitBall[MAX_HIT_BALL];		//ヒットボール構造体
int			g_numHit;						//当たり判定の総数
char		*hitBallTex[HIT_BALL_TEX_MAX] =
{
	"data/texture/ball000.png",
	"data/texture/ball001.png"
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initHit
//返り値	: 
//説明		: 初期化
//////////////////////////////////////////////////////////////////////////////
void	initHit	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice;		//デバイス情報
	int		i;		//loop

	pDevice = getDevice();

	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		g_hitSquare[i].use = false;
	}
	
	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		g_hitBall[i].use = false;
	}
	
	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		g_hitSquare[i].use = false;
	}

	//テクスチャの読み込み
	for(i = 0; i < HIT_BALL_TEX_MAX; i++)
	{
		if(FAILED(D3DXCreateTextureFromFile(pDevice, hitBallTex[i], &g_pTexBufferHitBall[i])))
			{
				MessageBox(NULL, "エラー", "birdテクスチャエラー", MB_OK);
			}
	}

	makeVertexHit(pDevice);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitHit
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitHit	( void )
{
	//変数宣言
	int		i;		//loop

	if(g_pVtxBufferHitBox != NULL)
	{
		g_pVtxBufferHitBox->Release();
		g_pVtxBufferHitBox = NULL;
	}

	if(g_pVtxBufferHitSquare != NULL)
	{
		g_pVtxBufferHitSquare->Release();
		g_pVtxBufferHitSquare = NULL;
	}

	for(i = 0; i < HIT_BALL_TEX_MAX; i++)
	{
		if(g_pTexBufferHitBall[i] != NULL)
		{
			g_pTexBufferHitBall[i]->Release();
			g_pTexBufferHitBall[i] = NULL;
		}
	}

	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].mesh != NULL)
		{
			g_hitBall[i].mesh->Release();
			g_hitBall[i].mesh = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateHit
//返り値	: 
//説明		: 更新
//////////////////////////////////////////////////////////////////////////////
void	updateHit	( void )
{
	//変数宣言
	VERTEX3D	*pVtx;
	int			i;		//loop

	g_pVtxBufferHitBox->Lock(0, 0, (void**)&pVtx, 0);
	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		pVtx[i * 22 + 0].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 1].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 2].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.b_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.b_l.z);
		pVtx[i * 22 + 3].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);

		pVtx[i * 22 + 4].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_l.z);
		pVtx[i * 22 + 5].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_r.z);

		pVtx[i * 22 + 6].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_l.z);
		pVtx[i * 22 + 7].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);

		pVtx[i * 22 + 8].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 9].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);
		pVtx[i * 22 + 10].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 11].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);
		pVtx[i * 22 + 12].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);
		pVtx[i * 22 + 13].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 14].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_r.z);
		pVtx[i * 22 + 15].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);
		pVtx[i * 22 + 16].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);

		pVtx[i * 22 + 17].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 18].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 19].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.b_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_l.z);
		pVtx[i * 22 + 20].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_l.z);
		pVtx[i * 22 + 21].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_l.z);
			
		pVtx[i * 22 + 0].color		=
		pVtx[i * 22 + 1].color		=
		pVtx[i * 22 + 2].color		=
		pVtx[i * 22 + 3].color		=
		pVtx[i * 22 + 4].color		=
		pVtx[i * 22 + 5].color		=
		pVtx[i * 22 + 6].color		=
		pVtx[i * 22 + 7].color		=
		pVtx[i * 22 + 8].color		=
		pVtx[i * 22 + 9].color		=
		pVtx[i * 22 + 10].color		=
		pVtx[i * 22 + 11].color		=
		pVtx[i * 22 + 12].color		=
		pVtx[i * 22 + 13].color		=
		pVtx[i * 22 + 14].color		=
		pVtx[i * 22 + 15].color		=
		pVtx[i * 22 + 16].color		=
		pVtx[i * 22 + 17].color		=
		pVtx[i * 22 + 18].color		=
		pVtx[i * 22 + 19].color		=
		pVtx[i * 22 + 20].color		=
		pVtx[i * 22 + 21].color		=	D3DCOLOR_RGBA(192, 192, 255, 192);
	}

	g_pVtxBufferHitBox->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawHit
//返り値	: 
//説明		: 描画
//////////////////////////////////////////////////////////////////////////////
void	drawHit	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice;
	int		numFace;	//面の数
	int		i, j;			//loop
	D3DXMATRIX	world;				//ワールド行列

	pDevice = getDevice();
	D3DXMatrixIdentity(&world);
	pDevice->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
	
	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferHitSquare, 0, sizeof(VERTEX3D));

	//頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//=============================
	// ポリゴンの描画
	//=============================
	//面
	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		if(g_hitSquare[i].use == true)
		{
			pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
					NUM_VERTEX * i,			//開始頂点
					NUM_POLYGON				//面の数
				);
		}
	}
	
	//箱
	pDevice->SetStreamSource(0, g_pVtxBufferHitBox, 0, sizeof(VERTEX3D));
	//テクスチャ読み込み
	pDevice->SetTexture(0, NULL);
	//頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(g_hitBox[i].use == true)
		{
			pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,	//ポリゴンの種類
					22 * i,					//開始頂点
					NUM_POLYGON * 6 + 8	//面の数
				);
		}
	}

	//球
	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].use == true)
		{
			//行列設定
			setMatrixHit(pDevice, i);
			
			//テクスチャ読み込み
			pDevice->SetTexture(0, g_pTexBufferHitBall[0]);

			//描画
			g_hitBall[i].mesh->DrawSubset(0);
		}
	}
	
	//ライトの影響on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixHit
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixHit	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//変数宣言
	D3DXMATRIX	world;				//ワールド行列
	D3DXMATRIX	move;				//平行移動行列

	//ワールド行列初期化
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);

	//座標変換
	D3DXMatrixTranslation(&move, g_hitBall[num].pos.x, g_hitBall[num].pos.y, g_hitBall[num].pos.z);	//平行移動
	
	//行列合成
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitBox
//
//引数		:	pos		中心座標
//				top		上底の4点
//				bottom	下底の4点
//				type	オブジェクトタイプ
//
//返り値	:	何番目の配列に判定が格納されたか
//説明		:	ヒットボックスを設定する
//////////////////////////////////////////////////////////////////////////////
int	setHitBox	(D3DXVECTOR3 pos, HIT_VTX top, HIT_VTX bottom, OBJ_TYPE_3D type)
{
	//変数宣言
	int			i;		//loop
	VERTEX3D	*pVtx;	//頂点ポインタ

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(g_hitBox[i].use == false)
		{
			g_hitBox[i].pos		=	pos;
			g_hitBox[i].top		=	top;
			g_hitBox[i].bottom	=	bottom;
			g_hitBox[i].type	=	type;
			g_hitBox[i].use		=	true;

			//頂点情報設定
			g_pVtxBufferHitBox->Lock(0, 0, (void **)&pVtx, 0);

			pVtx[i * 22 + 0].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 1].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 2].pos		=	D3DXVECTOR3(pos.x + top.b_l.x, pos.y + top.b_l.y, pos.z + top.b_l.z);
			pVtx[i * 22 + 3].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);

			pVtx[i * 22 + 4].pos		=	D3DXVECTOR3(pos.x + bottom.b_l.x, pos.y + bottom.b_l.y, pos.z + bottom.b_l.z);
			pVtx[i * 22 + 5].pos		=	D3DXVECTOR3(pos.x + bottom.b_r.x, pos.y + bottom.b_r.y, pos.z + bottom.b_r.z);

			pVtx[i * 22 + 6].pos		=	D3DXVECTOR3(pos.x + bottom.t_l.x, pos.y + bottom.t_l.y, pos.z + bottom.t_l.z);
			pVtx[i * 22 + 7].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);

			pVtx[i * 22 + 8].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 9].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);
			pVtx[i * 22 + 10].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 11].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);
			pVtx[i * 22 + 12].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);
			pVtx[i * 22 + 13].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 14].pos		=	D3DXVECTOR3(pos.x + bottom.b_r.x, pos.y + bottom.b_r.y, pos.z + bottom.b_r.z);
			pVtx[i * 22 + 15].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);
			pVtx[i * 22 + 16].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);

			pVtx[i * 22 + 17].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 18].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 19].pos		=	D3DXVECTOR3(pos.x + top.b_l.x, pos.y + top.b_l.y, pos.z + top.b_l.z);
			pVtx[i * 22 + 20].pos		=	D3DXVECTOR3(pos.x + bottom.t_l.x, pos.y + bottom.t_l.y, pos.z + bottom.t_l.z);
			pVtx[i * 22 + 21].pos		=	D3DXVECTOR3(pos.x + bottom.b_l.x, pos.y + bottom.b_l.y, pos.z + bottom.b_l.z);
			
			pVtx[i * 22 + 0].color		=
			pVtx[i * 22 + 1].color		=
			pVtx[i * 22 + 2].color		=
			pVtx[i * 22 + 3].color		=
			pVtx[i * 22 + 4].color		=
			pVtx[i * 22 + 5].color		=
			pVtx[i * 22 + 6].color		=
			pVtx[i * 22 + 7].color		=
			pVtx[i * 22 + 8].color		=
			pVtx[i * 22 + 9].color		=
			pVtx[i * 22 + 10].color		=
			pVtx[i * 22 + 11].color		=
			pVtx[i * 22 + 12].color		=
			pVtx[i * 22 + 13].color		=
			pVtx[i * 22 + 14].color		=
			pVtx[i * 22 + 15].color		=
			pVtx[i * 22 + 16].color		=
			pVtx[i * 22 + 17].color		=
			pVtx[i * 22 + 18].color		=
			pVtx[i * 22 + 19].color		=
			pVtx[i * 22 + 20].color		=
			pVtx[i * 22 + 21].color		=	D3DCOLOR_RGBA(192, 192, 255, 192);

			g_pVtxBufferHitBox->Unlock();

			return i;
		}
	}

	//全ての構造体が使われていた
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitBoxVtx
//
//引数		:	max		最大頂点
//				min		最低頂点
//				*top	上程用出力ポインタ
//				*bottom	下底用出力ポインタ
//
//返り値	:
//説明		:	ヒットボックスの頂点位置設定
//////////////////////////////////////////////////////////////////////////////
void	setHitBoxVtx	(D3DXVECTOR3 max, D3DXVECTOR3 min, HIT_VTX *top, HIT_VTX *bottom)
{
	top->t_l = D3DXVECTOR3(min.x, max.y, max.z);
	top->t_r = D3DXVECTOR3(max.x, max.y, max.z);
	top->b_l = D3DXVECTOR3(min.x, max.y, min.z);
	top->b_r = D3DXVECTOR3(max.x, max.y, min.z);

	bottom->t_l = D3DXVECTOR3(min.x, min.y, max.z);
	bottom->t_r = D3DXVECTOR3(max.x, min.y, max.z);
	bottom->b_l = D3DXVECTOR3(min.x, min.y, min.z);
	bottom->b_r = D3DXVECTOR3(max.x, min.y, min.z);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitSquare
//
//引数		:	pos		中心座標
//				vtx		4隅の座標
//
//返り値	:	構造体の配列番号
//説明		:	面の当たり判定追加
//////////////////////////////////////////////////////////////////////////////
int	setHitSquare	(D3DXVECTOR3 pos, HIT_VTX vtx)
{
	//変数宣言
	VERTEX3D	*pVtx;	
	int			i;		//loop

	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		if(g_hitSquare[i].use == false)
		{
			g_hitSquare[i].pos	=	pos;
			g_hitSquare[i].vtx	=	vtx;
			g_hitSquare[i].use	=	true;

			g_pVtxBufferHitSquare->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[i * NUM_VERTEX + 0].pos	=	D3DXVECTOR3(pos.x + vtx.t_l.x, pos.y + vtx.t_l.y, pos.z + vtx.t_l.z);
			pVtx[i * NUM_VERTEX + 1].pos	=	D3DXVECTOR3(pos.x + vtx.t_r.x, pos.y + vtx.t_r.y, pos.z + vtx.t_r.z);
			pVtx[i * NUM_VERTEX + 2].pos	=	D3DXVECTOR3(pos.x + vtx.b_l.x, pos.y + vtx.b_l.y, pos.z + vtx.b_l.z);
			pVtx[i * NUM_VERTEX + 3].pos	=	D3DXVECTOR3(pos.x + vtx.b_r.x, pos.y + vtx.b_r.y, pos.z + vtx.b_r.z);

			pVtx[i * NUM_VERTEX + 0].color	=
			pVtx[i * NUM_VERTEX + 1].color	=
			pVtx[i * NUM_VERTEX + 2].color	=
			pVtx[i * NUM_VERTEX + 3].color	=	D3DXCOLOR(0.f, 0.f, 1.f, 0.3f);

			g_pVtxBufferHitSquare->Unlock();

			return i;
		}
	}

	//全ての構造体が使われていた
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitBall
//
//引数		:	pos		中心座標
//				len		半径の長さ
//
//返り値	:	構造体の配列番号
//説明		:	球の当たり判定追加
//////////////////////////////////////////////////////////////////////////////
int	setHitBall	(D3DXVECTOR3 pos, float len, OBJ_TYPE_3D type)
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice;
	int		i;		//loop

	pDevice = getDevice();

	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].use == false)
		{
			g_hitBall[i].pos	=	pos;
			g_hitBall[i].len	=	len;
			g_hitBall[i].type	=	type;
			g_hitBall[i].use	=	true;

			D3DXCreateSphere(
				pDevice,					// 描画先のデバイス
				g_hitBall[i].len,			// 半径。0.0fでも実はOK。マイナスはNG
				32,							// スライスの数。増やすときれいな球になるはず
				16,							// スタック数。増やすときれいな球になるはず
				&g_hitBall[i].mesh,			// 出力するメッシュ
				NULL						// あまり使わないらしいんでNULLにしといた
			);

			return i;
		}
	}

	//全ての構造体が使われていた
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	releaseHitBall
//
//引数		:	num		配列番号
//
//返り値	:
//説明		:	球の構造体解放
//////////////////////////////////////////////////////////////////////////////
void	releaseHitBall	(int num)
{
	if(num < MAX_HIT_BALL)
	{
		g_hitBall[num].use	=	false;
		g_hitBall[num].mesh->Release();
		g_hitBall[num].mesh = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitBoxPos
//
//引数		:	num		配列番号
//				pos		設定したい座標
//
//返り値	:
//説明		:	座標設定
//////////////////////////////////////////////////////////////////////////////
void	setHitBoxPos	(int num, D3DXVECTOR3 pos)
{
	if(num < MAX_HIT_BOX)
	{
		g_hitBox[num].pos = pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	setHitBallPos
//
//引数		:	num		配列番号
//				pos		設定したい座標
//
//返り値	:
//説明		:	球の当たり判定の座標設定
//////////////////////////////////////////////////////////////////////////////
void	setHitBallPos	(int num, D3DXVECTOR3 pos)
{
	if(num < MAX_HIT_BALL)
	{
		g_hitBall[num].pos = pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	getXfileVtx
//
//引数		:	*fileName	検索したいXファイルの名前
//				*max		最大頂点出力先
//				*min		最小頂点出力先
//
//返り値	:
//説明		:	Xファイルから最大最小頂点取得
//////////////////////////////////////////////////////////////////////////////
void	getXfileVtx		(char *fileName, D3DXVECTOR3 *max, D3DXVECTOR3 *min)
{
	//変数宣言
	FILE		*fp = NULL;			//ファイルポインタ
	char		target[256];		//検索ワード
	char		check[256];			//チェックワード
	D3DXVECTOR3	work;				//ワーク
	char		end = '\0';			//検索文字
	HIT_VTX		top, bottom;		//HitBox用

	//変数初期化
	*max = *min = work = D3DXVECTOR3(0.f, 0.f, 0.f);

	//探索文字列検索
	sprintf(target, "Mesh {\n");

	//ファイルオープン
	fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		printf("ファイルオープン失敗");
		rewind(stdin);
		getchar();
		return;
	}

	//文字列探索
	while(strcmp(check, target) != 0)
	{
		check[0] = '\0';
		fgets(check, 255, fp);
	}
	
	//頂点数
	fgets(check, 255, fp);

	//座標取得
	while(end != ';')
	{
		fscanf(fp, "%f;%f;%f;%c", &work.x, &work.y, &work.z, &end);

		//最大頂点
		if(work.x > max->x)
			max->x = work.x;
		if(work.y > max->y)
			max->y = work.y;
		if(work.z > max->z)
			max->z = work.z;

		//最低頂点
		if(work.x < min->x)
			min->x = work.x;
		if(work.y < min->y)
			min->y = work.y;
		if(work.z < min->z)
			min->z = work.z;
	}

	//ファイル終了
	if(fp != NULL)
	{
		fclose(fp);
	}
}

/////////////////////////////////////////////////////////////////////////////
//関数名	:	getHitBoxType
//
//引数		:	num		配列番号
//
//返り値	:	オブジェクトタイプ
//説明		:	ヒットボックスのオブジェクトタイプ取得
//////////////////////////////////////////////////////////////////////////////
OBJ_TYPE_3D	getHitBoxType	( int num )
{
	if(g_hitBox[num].use == true)
	{
		return g_hitBox[num].type;
	}

	return BOJ_TYPE_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//関数名	:	getHitBox
//
//引数		:	num		配列番号
//
//返り値	:	HitBox変数
//説明		:	ヒットボックス取得
//////////////////////////////////////////////////////////////////////////////
HIT_BOX	getHitBox	( int num )
{
	return g_hitBox[num];
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexHit
//返り値	: 
//説明		: 頂点作成
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexHit ( LPDIRECT3DDEVICE9 pDevice )
{
	//頂点バッファの設定
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * 22 * MAX_HIT_BOX,	//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,				//頂点バッファの使用方法
			FVF_VERTEX_3D,					//
			D3DPOOL_MANAGED,				//メモリ管理方法
			&g_pVtxBufferHitBox,			//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * MAX_HIT_SQUARE * NUM_VERTEX,	//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,				//頂点バッファの使用方法
			FVF_VERTEX_3D,					//
			D3DPOOL_MANAGED,				//メモリ管理方法
			&g_pVtxBufferHitSquare,			//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: hitVec
//返り値	: 
//説明		: 線と点の衝突判定
//////////////////////////////////////////////////////////////////////////////
bool	hitVec	( D3DXVECTOR2 vec1, D3DXVECTOR2 vec2, D3DXVECTOR2 point )
{
	if((vec2.x - vec1.x) * (point.y - vec1.y) - (point.x - vec1.x) * (vec2.y - vec1.y) <= 0)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: hitFacePoint
//返り値	: 
//説明		: 面と点での当たり判定
//////////////////////////////////////////////////////////////////////////////
bool	hitFacePoint	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 target )
{
	//変数宣言
	D3DXVECTOR3		normal;		//法線ベクトル
	double			inner;		//内積

	//法線ベクトルを求める
	normal = getNormalVec(point, vec1, vec2);
	normal = vec3Normalize(normal);

	//求めたい点までのベクトルを求める
	target = target - point;
	target = vec3Normalize(target);

	//法線ベクトルと点の内積を求める
	inner = getInnerProduct(normal, target);

	//内積が0より小さければ当たっている
	if(inner <= 0)
	{
		return true;
	}

	//そうでなければfalse
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	hitBox
//
//引数		:	box1	比較したい箱
//				box2	比較対象
//
//返り値	:	trueで当たっている
//説明		:	面と点の当たり判定による立方体の当たり判定
//////////////////////////////////////////////////////////////////////////////
bool	hitBox	( HIT_BOX box1, HIT_BOX box2 )
{
	//変数宣言
	int				i;		//loop
	D3DXVECTOR3		target[NUM_VERTEX * 2];

	//変数代入
	target[0] = box2.top.t_l;
	target[1] = box2.top.t_r;
	target[2] = box2.top.b_l;
	target[3] = box2.top.b_r;
	target[4] = box2.bottom.t_l;
	target[5] = box2.bottom.t_r;
	target[6] = box2.bottom.b_l;
	target[7] = box2.bottom.b_r;

	for(i = 0; i < NUM_VERTEX * 2; i++)
	{
		//box2との判定
		if(hitFacePoint(box1.top.t_l, box1.top.t_r, box1.top.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.bottom.t_l, box1.bottom.t_r, box1.bottom.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_r, box1.bottom.t_r, box1.top.b_r, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_l, box1.bottom.t_l, box1.top.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.b_l, box1.bottom.b_l, box1.top.b_r, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_l, box1.top.t_r, box1.bottom.t_l, target[i]) == false)
			return false;
	}

	//全てパスすると当たっている
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//関数名	:	hitFaceLine
//
//引数		:	point	原点
//				face	板ポリゴン
//				sLine	線分の始点
//				eLine	線分の終点
//
//返り値	:	trueで当たっている
//説明		:	面と線の当たり判定
//////////////////////////////////////////////////////////////////////////////
bool	hitFaceLine	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine)
{
	//変数宣言
	D3DXVECTOR3		normal;		//法線ベクトル
	double			inner[2];	//内積

	//法線ベクトル取得
	normal = getNormalVec(point, vec1, vec2);
	normal = vec3Normalize(normal);

	//線を原点に合わせる
	sLine = vec3Normalize(sLine - point);
	sLine = vec3Normalize(sLine);
	eLine = vec3Normalize(eLine - point);
	eLine = vec3Normalize(eLine);

	//内積取得
	inner[0] = getInnerProduct(normal, sLine);
	inner[1] = getInnerProduct(normal, eLine);

	//片方が鈍角の場合当たっている
	if((inner[0] <= 0.f && inner[1] >= 0.f) ||
		(inner[0] >= 0.f && inner[1] <= 0.f))
	{
		return true;
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//関数名	:	hitBoxLine
//
//引数		:	box		判定を取りたい箱
//				sLine	線分の始点
//				eLine	線分の終点
//
//返り値	:	trueで当たっている
//説明		:	箱と線の当たり判定
//////////////////////////////////////////////////////////////////////////////
bool	hitBoxLine	( HIT_BOX box, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine )
{
	if(hitFaceLine(box.top.t_l, box.top.t_r, box.top.b_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.bottom.t_l, box.bottom.t_r, box.bottom.b_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_l, box.top.b_l, box.bottom.t_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_r, box.top.b_r, box.bottom.t_r, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_l, box.top.t_r, box.bottom.t_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.b_l, box.top.b_r, box.bottom.b_l, sLine, eLine) == true)
		return true;
}

/////////////////////////////////////////////////////////////////////////////
//関数名	:	hitBall
//
//引数		:	box		判定を取りたい箱
//				sLine	線分の始点
//				eLine	線分の終点
//
//返り値	:	trueで当たっている
//説明		:	球と球の当たり判定
//////////////////////////////////////////////////////////////////////////////
bool	hitBoall	( HIT_BALL ball1, HIT_BALL ball2 )
{
	//変数宣言
	float		len;		//球と球の距離

	//二つの球の距離を計算
	len = sqrt((ball2.pos.x - ball1.pos.x) * (ball2.pos.x - ball1.pos.x)
		+ (ball2.pos.y - ball1.pos.y) * (ball2.pos.y - ball1.pos.y)
		+ (ball2.pos.z - ball1.pos.y) * (ball2.pos.z - ball1.pos.y));

	//距離を二つの半径の合計から引く
	len -= ball1.len + ball2.len;

	//lenが0より大きければ当たっている
	if(len >= 0)
	{
		return true;
	}

	return false;
}