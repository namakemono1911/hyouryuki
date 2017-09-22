/*****************************************************************************
メッシュドーム描画
Aythor	: 上野　匠
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"camera.h"
#include	"meshDome.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	DOME_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	DOME_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_DOME	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferDome	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferDome	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixDome	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshDome [DOME_TEX_MAX]	=	{};	//テクスチャインターフェイス

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferDome	=	NULL;	//バッファフィールド
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferDome	=	NULL;	//インデックスバッファインターフェイス

DOME	dome[MAX_DOME];
int		g_numDome	=	0;			//フィールドの総数
char	*DomeTexName[]	=			//テクスチャの名前
{
	"data/texture/sky001.jpg"		//空テクスチャ2
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initMeshDome
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshDome ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//dome初期化
	for(i = 0; i < MAX_DOME; i++)
	{
		//使用フラグ
		dome[i].use	=	false;
	}

	makeVertexBufferDome(pDevice);

	for(i = 0; i < DOME_TEX_MAX; i++)
	{
		//テクスチャの読み込み
		if(FAILED(D3DXCreateTextureFromFile(pDevice, DomeTexName[i], &g_pTextureMeshDome[i])))
		{
			MessageBox(NULL, "エラー", "エラー", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMeshDome
//返り値	: 
//説明		: フィールド設定
//////////////////////////////////////////////////////////////////////////////
void	setMeshDome	( D3DXVECTOR3 pos, float len, D3DXVECTOR2 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color, DOME_TEX tex, int id )
{
	//変数宣言
	int		i;		//loop

	for(i = 0; i < MAX_DOME; i++)
	{
		//使われていない構造体探索
		if(dome[i].use	==	false)
		{
			//数値代入
			dome[i].use		=	true;
			dome[i].pos		=	pos;
			dome[i].len		=	len;
			dome[i].numTyle	=	numTyle;
			dome[i].rot		=	rot;
			dome[i].color	=	color;
			dome[i].tex		=	tex;
			dome[i].id		=	id;

			g_numDome++;

			makeVertexBufferDome(getDevice());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getDomePos
//返り値	: 
//説明		: 壁の座標取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getDomePos	( int num )
{
	return	dome[num].pos;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitMeshDome
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshDome	( void )
{
	//変数宣言
	int		i;	//loop

	//頂点バッファの解放
	if(g_pVtxBufferDome	!=	NULL)
	{
		g_pVtxBufferDome->Release();
		g_pVtxBufferDome	=	NULL;
	}

	//頂点インデックスの解放
	if(g_pIdxBufferDome	!=	NULL)
	{
		g_pIdxBufferDome->Release();
		g_pIdxBufferDome	=	NULL;
	}

	//テクスチャバッファの解放
	for(i = 0; i < DOME_TEX_MAX; i++)
	{
		if(g_pTextureMeshDome[i]	!=	NULL)
		{
			g_pTextureMeshDome[i]->Release();
			g_pTextureMeshDome[i]	=	NULL;
		}
	}

	//総数終了
	g_numDome	=	0;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateMeshDome
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updateMeshDome	( void )
{
	//変数宣言
	int		i, j;		//loop
	
	for(i = 0, j = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			dome[i].pos		=	getCameraPos();
			dome[i].pos.y	=	0.f;
			dome[i].rot.x	+=	0.01f;
			dome[i].rot.x	=	fmod(dome[i].rot.x, 360);

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawMeshDome
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawMeshDome ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//必要な頂点インデックス数
	int		numPtv;		//必要なプリミティブ数
	int		minIdx;		//最小インデックス
	int		startIdx;	//スタートインデックス

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferDome, 0, sizeof(VERTEX3D));
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//サンプラーステートの設定
	pDevice->SetIndices(g_pIdxBufferDome);
	
	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0, startIdx = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			//必要な情報計算
			numVtx	+=	5 * (int)dome[i].numTyle.y;
			numVtx	+=	(int)(dome[i].numTyle.y + 1) * ((int)dome[i].numTyle.x - 1) * 2;

			numPtv	=	(int)dome[i].numTyle.y + ((int)dome[i].numTyle.y - 1) * 4;
			numPtv	+=	((int)dome[i].numTyle.x - 1) * ((int)dome[i].numTyle.y + 1) * 2;

			//テクスチャセット
			pDevice->SetTexture(0,g_pTextureMeshDome[dome[i].tex]);
			
			//ワールドマトリックス設定
			setMatrixDome(pDevice, i);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//描画方法
				startIdx,						//最初の頂点インデックスまでのオフセット
				minIdx,							//最小の頂点インデックス
				numVtx,							//頂点数
				0,								//スタートインデックス
				numPtv);						//プリミティブ数

			//変数初期化
			minIdx		+=	numVtx;
			startIdx	=	(int)dome[i].numTyle.x * ((int)dome[i].numTyle.y + 1) + 1;
			numVtx		=	0;
			numPtv		=	0;
			j++;
		}
	}

	//ライトの影響on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixDome
//返り値	: 
//説明		: ワールドマトリックス設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixDome	( LPDIRECT3DDEVICE9 pDevice, int num )
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
		dome[num].pos.x,
		dome[num].pos.y,
		dome[num].pos.z);

	//回転
	radian	=	D3DXVECTOR3(
		dome[num].rot.x * PI / 180.f,
		dome[num].rot.y * PI / 180.f,
		dome[num].rot.z * PI / 180.f);

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
//関数名	: makeVertexMeshDome
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferDome	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	VERTEX3D	*pVtx;		//仮想アドレス用ポインタ
	float		posY;		//描画位置
	float		len;		//半径
	float		rot;		//角度
	int			i, j;		//ループ
	int			x, y;		//xとyのループ
	int			num;		//必要な頂点数
	D3DXVECTOR3	fullSize;	//フィールド全体のサイズ

	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			num	+=	(int)dome[i].numTyle.x * ((int)dome[i].numTyle.y + 1) + 1;
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
			&g_pVtxBufferDome,						//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// 頂点情報登録
	//=============================
	g_pVtxBufferDome->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, rot = 90, j = 0; j < g_numDome; i++)
	{
		if(dome[i].use	== true)
		{
			//ドームの頂点
			pVtx[0].pos		=	D3DXVECTOR3(
				0.f,
				dome[i].len,
				0.f);

			//頂点色
			pVtx[0].color	= dome[i].color;

			//テクスチャ座標
			pVtx[0].tex		= D3DXVECTOR2(0.f, 0.f);

			//ポインタ移動
			pVtx++;

			for(y = 0; y < dome[i].numTyle.x; y++)
			{
				//角度算出
				rot	=	90 * ((dome[i].numTyle.x - (y + 1)) / dome[i].numTyle.x);

				//Y座標確定
				posY	= dome[i].len * sin(rot * (PI / 180));
				len		= dome[i].len * cos(rot * (PI / 180));

				for(x = 0; x < dome[i].numTyle.y + 1; x++)
				{
					//x,zの角度
					rot	=	360 * (x / dome[i].numTyle.y);

					//座標
					pVtx[0].pos	=	D3DXVECTOR3(
						len * cos(rot * (PI / 180)),
						posY,
						len * sin(rot * (PI / 180)));

					//頂点色
					pVtx[0].color	=	dome[i].color;

					//テクスチャ座標
					pVtx[0].tex		=	D3DXVECTOR2(
						(1.f / dome[i].numTyle.y) * x,
						(1.f / dome[i].numTyle.x) * (y + 1));

					//ポインタ移動
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferDome->Unlock();

	//=============================
	// インデックス作成
	//=============================
	makeIndexBufferDome(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeIndexBuffer
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferDome	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	HRESULT		hr;		//インデックス設定の確認
	WORD		*pIdx;	//インデックス番号
	int			i, j, k;//loop
	int			x, y;	//xとzのloop
	int			num;	//必要なインデックス数
	int			minIdx;	//最小インデックス
	
	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			num	+=	5 * (int)dome[i].numTyle.y;
			num	+=	(int)(dome[i].numTyle.y + 1) * ((int)dome[i].numTyle.x - 1) * 2;
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
		&g_pIdxBufferDome,			//インデックスバッファインターフェイス
		NULL);

	//=============================
	// インデックス登録
	//=============================
	g_pIdxBufferDome->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, minIdx = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			//ドームの1番上
			for(k = 0; k < dome[i].numTyle.y; k++)
			{
				pIdx[0]	=	minIdx;
				pIdx[1]	=	k + 1 + k % 2;
				pIdx[2]	=	k + 2 - k % 2;
				pIdx	+=	3;

				//縮退ポリゴン
				if(k < dome[i].numTyle.y - 1)
				{
					pIdx[3]	=	pIdx[2];
					pIdx[0]	=	minIdx;
					pIdx	+=	2;
				}
			}

			//そっから下
			for(y = 0; y < dome[i].numTyle.x - 1; y++)
			{
				for(x = 0; x < dome[i].numTyle.y + 1; x++)
				{
					pIdx[0]	=	(WORD)((dome[i].numTyle.y + 1) * y + x + 1);
					pIdx[1]	=	(WORD)((dome[i].numTyle.y + 1) * (y + 1) + x + 1);
					pIdx	+=	2;
				}
			}

			minIdx	=	(int)dome[i].numTyle.y * 3 + ((int)dome[i].numTyle.y - 1) * 2;
			minIdx	=	((int)dome[i].numTyle.x - 1) + ((int)dome[i].numTyle.y + 1) * 2;
			j++;
		}
	}
	
	g_pIdxBufferDome->Unlock();

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setDomePos
//返り値	: 
//説明		: ドーム座標セット
//////////////////////////////////////////////////////////////////////////////
void	setDomePos	( int num, D3DXVECTOR3 pos )
{
	dome[num].pos = pos;
}