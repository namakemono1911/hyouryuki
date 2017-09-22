/*****************************************************************************
メッシュウォール描画
Aythor	: 上野　匠
Data	: 2016_12_10
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"meshWall.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	WALL_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	WALL_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_WALL	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferWall	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferWall	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixWall	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshWall [WALL_TEX_MAX]	=	{};	//テクスチャインターフェイス

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferWall	=	NULL;	//バッファフィールド
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferWall	=	NULL;	//インデックスバッファインターフェイス

WALL	wall[MAX_WALL];
int		g_numWall	=	0;			//フィールドの総数
char	*WallTexName[]	=			//テクスチャの名前
{
	"data/texture/wall000.png",
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initMeshWall
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshWall ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//wall初期化
	for(i = 0; i < MAX_WALL; i++)
	{
		//使用フラグ
		wall[i].use	=	false;
	}

	//壁1セット
	setMeshWall(
		D3DXVECTOR3(0.f, 0.f, 45.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(0.f,0.f, 0.f),
		WALL_TEX_000);

	//壁2
	setMeshWall(
		D3DXVECTOR3(45.f, 0.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(90.f, 0.f, 0.f),
		WALL_TEX_000);
	
	//壁3
	setMeshWall(
		D3DXVECTOR3(0.f, 0.f, -45.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(180.f, 0.f, 0.f),
		WALL_TEX_000);
	
	//壁2
	setMeshWall(
		D3DXVECTOR3(-45.f, 0.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(270.f, 0.f, 0.f),
		WALL_TEX_000);

	makeVertexBufferWall(pDevice);

	for(i = 0; i < WALL_TEX_MAX; i++)
	{
		//テクスチャの読み込み
		if(FAILED(D3DXCreateTextureFromFile(pDevice, WallTexName[i], &g_pTextureMeshWall[i])))
		{
			MessageBox(NULL, "エラー", "エラー", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMeshWall
//返り値	: 
//説明		: フィールド設定
//////////////////////////////////////////////////////////////////////////////
void	setMeshWall	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot, WALL_TEX tex )
{
	//変数宣言
	int		i;		//loop

	for(i = 0; i < MAX_WALL; i++)
	{
		//使われていない構造体探索
		if(wall[i].use	==	false)
		{
			//数値代入
			wall[i].use		=	true;
			wall[i].pos		=	pos;
			wall[i].size	=	size;
			wall[i].numTyle	=	numTyle;
			wall[i].rot		=	rot;
			wall[i].tex		=	tex;

			g_numWall++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getWallPos
//返り値	: 
//説明		: 壁の座標取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getWallPos	( int num )
{
	return	wall[num].pos;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitMeshWall
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshWall	( void )
{
	//変数宣言
	int		i;	//loop

	//頂点バッファの解放
	if(g_pVtxBufferWall	!=	NULL)
	{
		g_pVtxBufferWall->Release();
		g_pVtxBufferWall	=	NULL;
	}

	//頂点インデックスの解放
	if(g_pIdxBufferWall	!=	NULL)
	{
		g_pIdxBufferWall->Release();
		g_pIdxBufferWall	=	NULL;
	}

	for(i = 0; i < WALL_TEX_MAX; i++)
	{
		//テクスチャバッファの解放
		if(g_pTextureMeshWall[i]	!=	NULL)
		{
			g_pTextureMeshWall[i]->Release();
			g_pTextureMeshWall[i]	=	NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateMeshWall
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updateMeshWall	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawMeshWall
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawMeshWall ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//必要な頂点インデックス数
	int		numPtv;		//必要なプリミティブ数
	int		minIdx;		//最小インデックス		

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferWall, 0, sizeof(VERTEX3D));
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//サンプラーステートの設定
	pDevice->SetIndices(g_pIdxBufferWall);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			//必要な情報計算
			numVtx	=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y * 2));
			numVtx	+=	(int)((wall[i].numTyle.y - 1) * 2);

			numPtv	=	(int)(wall[i].numTyle.x * wall[i].numTyle.y * 2 + (wall[i].numTyle.y - 1) * 4);

			//テクスチャセット
			pDevice->SetTexture(0,g_pTextureMeshWall[wall[i].tex]);

			//ワールドマトリックス設定
			setMatrixWall(pDevice, i);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//描画方法
				0,								//最初の頂点インデックスまでのオフセット
				minIdx,							//最小の頂点インデックス
				numVtx,							//頂点数
				0,								//スタートインデックス
				numPtv);						//プリミティブ数

			//変数初期化
			minIdx	+=	numVtx;
			numVtx	=	0;
			numPtv	=	0;
			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixWall
//返り値	: 
//説明		: ワールドマトリックス設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWall	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//変数宣言
	D3DXMATRIX	world;		//ワールド行列
	D3DXMATRIX	move;		//平行移動行列
	D3DXMATRIX	rot;		//回転行列
	D3DXVECTOR3	radian;		//度数変換

	//行列初期化
	D3DXMatrixIdentity(&world);

	//平行移動
	D3DXMatrixTranslation(&move,	
		wall[num].pos.x,
		wall[num].pos.y,
		wall[num].pos.z);

	//回転
	radian	=	D3DXVECTOR3(
		wall[num].rot.x * PI / 180.f,
		wall[num].rot.y * PI / 180.f,
		wall[num].rot.z * PI / 180.f);

	D3DXMatrixRotationYawPitchRoll(&rot,
		radian.x,
		radian.y,
		radian.z);	//回転行列

	//行列合成
	world	*=	rot;		//回転
	world	*=	move;		//平行移動行列

	pDevice->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexMeshWall
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferWall	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	VERTEX3D	*pVtx;		//仮想アドレス用ポインタ
	D3DXVECTOR3	pos;		//描画位置
	int			i, j;		//ループ
	int			x, y;		//xとyのループ
	int			num;		//必要な頂点数
	D3DXVECTOR3	fullSize;	//フィールド全体のサイズ

	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			num	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y + 1));
			j++;
		}
	}

	//=============================
	// 頂点バッファ作成
	//=============================
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * num,					//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
			FVF_VERTEX_3D,							//なんか設定
			D3DPOOL_MANAGED,						//メモリ管理方法
			&g_pVtxBufferWall,						//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// 頂点情報登録
	//=============================
	g_pVtxBufferWall->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			//全体のサイズ計算
			fullSize	=	D3DXVECTOR3(
				wall[i].numTyle.x * wall[i].size.x,
				wall[i].numTyle.y * wall[i].size.y,
				0.f);

			for(y = 0; y < wall[i].numTyle.y + 1; y++)
			{
				for(x = 0; x < wall[i].numTyle.x + 1; x++)
				{
					//頂点
					pVtx[0].pos	=	D3DXVECTOR3(
						(wall[i].size.x * x) - (fullSize.x * 0.5f),
						fullSize.y - (wall[i].size.y * y),
						0.f);

					//頂点色
					pVtx[0].color	=	D3DCOLOR_RGBA(255, 255, 255, 255);

					//法線ベクトル
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, -0.5f);

					//テクスチャ座標
					pVtx[0].tex		=	D3DXVECTOR2((float)x, (float)y);

					//ポインタ移動
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferWall->Unlock();

	//=============================
	// インデックス作成
	//=============================
	makeIndexBufferWall(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeIndexBuffer
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferWall	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	HRESULT		hr;		//インデックス設定の確認
	WORD		*pIdx;	//インデックス番号
	int			i, j;	//loop
	int			x, y;	//xとzのloop
	int			num;	//必要なインデックス数		
	int			numIdx;	//n個目のインデックス数
	
	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			num	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y * 2));
			num	+=	(int)((wall[i].numTyle.y - 1) * 2);
			j++;
		}
	}

	//=============================
	// インデックス作成
	//=============================
	hr	=	pDevice->CreateIndexBuffer(
		sizeof(WORD) *  num,	//必要なインデックスサイズ
		D3DUSAGE_WRITEONLY,			//使用用途フラグ
		D3DFMT_INDEX16,				//インデックスのフォーマット
		D3DPOOL_MANAGED,			//メモリの管理方法（お任せモード）
		&g_pIdxBufferWall,			//インデックスバッファインターフェイス
		NULL);

	//=============================
	// インデックス登録
	//=============================
	g_pIdxBufferWall->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, numIdx = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			for(y = 0; y < wall[i].numTyle.y; y++)
			{
				for(x = 0; x < wall[i].numTyle.x + 1; x++)
				{
					//インデックス登録
					pIdx[0]	=	(WORD)(wall[i].numTyle.x + 1) * (y + 1) + x + numIdx;
					pIdx[1]	=	(WORD)(wall[i].numTyle.x + 1) * y + x + numIdx;

					//ポインタ移動
					pIdx	+=	2;
				}

				//縮退ポリゴン
				if(y	<	wall[i].numTyle.y - 1)
				{
					//インデックス登録
					x--;
					pIdx[0]	=	(WORD)(wall[i].numTyle.x + 1) * y + x + numIdx;
					pIdx[1]	=	(WORD)(wall[i].numTyle.x + 1) * (y + 1) + x + numIdx + 1;

					//ポインタ移動
					pIdx	+=	2;
				}
			}

			//次が始まるインデックス数
			numIdx	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y + 1));

			j++;
		}
	}

	g_pIdxBufferWall->Unlock();

	return	S_OK;
}