/*****************************************************************************
メッシュフィールド描画
Aythor	: 上野　匠
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//インクルード
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"input.h"
#include	"meshField.h"
#include	"render.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	FIELD_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	FIELD_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_FIELD	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferField	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferField	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixField	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshField [FIELD_TEX_MAX]	=	{};	//テクスチャインターフェイス

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferField	=	NULL;	//バッファフィールド
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferField	=	NULL;	//インデックスバッファインターフェイス

FIELD	field[MAX_FIELD];
int		g_numField	=	0;			//フィールドの総数
char	*FieldTexName[]	=			//テクスチャの名前
{
	"data/texture/field000.jpg",
	"data/texture/field001.jpg",
	"data/texture/field002.jpg",
	"data/texture/field003.jpg"
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initMeshField
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshField ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//field初期化
	for(i = 0; i < MAX_FIELD; i++)
	{
		//使用フラグ
		field[i].use	=	false;
		field[i].id		=	MAX_FIELD + 1;
	}

	for(i = 0; i < FIELD_TEX_MAX; i++)
	{
		//テクスチャの読み込み
		if(FAILED(D3DXCreateTextureFromFile(pDevice, FieldTexName[i], &g_pTextureMeshField[i])))
		{
			MessageBox(NULL, "エラー", "fieldエラー", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMeshField
//返り値	: 
//説明		: フィールド設定
//////////////////////////////////////////////////////////////////////////////
void	setMeshField	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color ,D3DXVECTOR2 texMove, FIELD_TEX tex, int id )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use	==	false)
		{
			//数値代入
			field[i].use		=	true;
			field[i].pos		=	pos;
			field[i].size		=	size;
			field[i].numTyle	=	numTyle;
			field[i].rot		=	rot;
			field[i].color		=	color;
			field[i].texPos		=	D3DXVECTOR2(0.f, 0.f);
			field[i].texMove	=	texMove;
			field[i].tex		=	tex;
			field[i].id			=	id;

			g_numField++;

			makeVertexBufferField(pDevice);
			break;
		}
	
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitMeshField
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshField	( void )
{
	//変数宣言
	int		i;	//loop

	//頂点バッファの解放
	if(g_pVtxBufferField	!=	NULL)
	{
		g_pVtxBufferField->Release();
		g_pVtxBufferField	=	NULL;
	}

	//頂点インデックスの解放
	if(g_pIdxBufferField	!=	NULL)
	{
		g_pIdxBufferField->Release();
		g_pIdxBufferField	=	NULL;
	}

	for(i = 0; i < FIELD_TEX_MAX; i++)
	{
		//テクスチャバッファの解放
		if(g_pTextureMeshField[i]	!=	NULL)
		{
			g_pTextureMeshField[i]->Release();
			g_pTextureMeshField[i]	=	NULL;
		}
	}

	g_numField = 0;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateMeshField
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updateMeshField	( void )
{
	//変数宣言
	VERTEX3D	*pVtx;	//頂点情報ポインタ
	int			i, j;		//ループ
	int			x, z;		//xとzのループ
	int			num;		//必要な頂点数
	D3DXVECTOR3	fullSize;	//フィールド全体のサイズ

	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z + 1));
			j++;
		}
	}

	//=============================
	// テクスチャアニメ
	//=============================
	for(i = 0; i < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			field[i].texPos	=	D3DXVECTOR2(
				field[i].texPos.x + field[i].texMove.x,
				field[i].texPos.y + field[i].texMove.y);
		}
	}

	//=============================
	// 頂点バッファ登録
	//=============================
	g_pVtxBufferField->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//全体のサイズ計算
			fullSize	=	D3DXVECTOR3(
				field[i].numTyle.x * field[i].size.x,
				0.f,
				field[i].numTyle.z * field[i].size.z);

			for(z = 0; z < field[i].numTyle.z + 1; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//頂点
					pVtx[0].pos	=	D3DXVECTOR3(
						(field[i].size.x * x) - (fullSize.x * 0.5f),
						0.f,
						-(field[i].size.z * z) + (fullSize.z * 0.5f));

					//頂点色
					pVtx[0].color	=	field[i].color;

					//法線ベクトル
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, 0.f);

					//テクスチャ座標
					pVtx[0].tex		=	D3DXVECTOR2(field[i].texPos.x + (float)x, field[i].texPos.y + (float)z);

					//ポインタ移動
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferField->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawMeshField
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawMeshField ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//必要な頂点インデックス数
	int		numPtv;		//必要なプリミティブ数
	int		minIdx;		//最小インデックス数
	int		startVtxIdx;	//スタートバーテクスインデックス

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferField, 0, sizeof(VERTEX3D));
	
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//サンプラーステートの設定
	pDevice->SetIndices(g_pIdxBufferField);

	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0, startVtxIdx = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//必要な情報計算
			numVtx	=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z * 2));
			numVtx	+=	(int)((field[i].numTyle.z - 1) * 2);

			numPtv	=	(int)(field[i].numTyle.x * field[i].numTyle.z * 2 + (field[i].numTyle.z - 1) * 4);

			//テクスチャセット
			pDevice->SetTexture(0,g_pTextureMeshField[field[i].tex]);

			//ワールドマトリックス設定
			setMatrixField(pDevice, i);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//描画方法
				startVtxIdx,					//最初の頂点インデックスまでのオフセット
				minIdx,							//最小の頂点インデックス
				numVtx,							//頂点数
				0,								//スタートインデックス
				numPtv);						//プリミティブ数

			//変数初期化
			minIdx		=	numVtx;		//最小インデックス代入
			//startVtxIdx	=	numVtx;
			startVtxIdx	+=	((int)field[i].numTyle.x + 1) * ((int)field[i].numTyle.z + 1);	//次のスタートインデックス
			//startVtxIdx	-=	minIdx;
			numVtx		=	0;			//頂点数初期化
			numPtv		=	0;			//プリミティブ数初期化
			j++;
		}
	}

	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixField
//返り値	: 
//説明		: ワールドマトリックス設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixField	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//変数宣言
	D3DXMATRIX	world;		//ワールド行列
	D3DXMATRIX	move;		//平行移動行列
	D3DXMATRIX	rot;		//回転行列

	//行列初期化
	D3DXMatrixIdentity(&world);

	//平行移動
	D3DXMatrixTranslation(&move,	
		field[num].pos.x,
		field[num].pos.y,
		field[num].pos.z);

	//回転
	D3DXMatrixRotationYawPitchRoll(&rot,
		field[num].rot.x,
		field[num].rot.y,
		field[num].rot.z);	//回転行列

	//行列合成
	world	*=	rot;		//回転
	world	*=	move;		//平行移動

	pDevice->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexMeshField
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferField	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	VERTEX3D	*pVtx;		//仮想アドレス用ポインタ
	D3DXVECTOR3	pos;		//描画位置
	int			i, j;		//ループ
	int			x, z;		//xとzのループ
	int			num;		//必要な頂点数
	D3DXVECTOR3	fullSize;	//フィールド全体のサイズ

	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z + 1));
			j++;
		}
	}

	if(g_pVtxBufferField != NULL)
	{
		g_pVtxBufferField->Release();
		g_pVtxBufferField = NULL;
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
			&g_pVtxBufferField,						//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// 頂点情報登録
	//=============================
	g_pVtxBufferField->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//全体のサイズ計算
			fullSize	=	D3DXVECTOR3(
				field[i].numTyle.x * field[i].size.x,
				0.f,
				field[i].numTyle.z * field[i].size.z);

			for(z = 0; z < field[i].numTyle.z + 1; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//頂点
					pVtx[0].pos	=	D3DXVECTOR3(
						(field[i].size.x * x) - (fullSize.x * 0.5f),
						0.f,
						-(field[i].size.z * z) + (fullSize.z * 0.5f));

					//頂点色
					pVtx[0].color	=	field[i].color;

					//法線ベクトル
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, 0.f);

					//テクスチャ座標
					pVtx[0].tex		=	D3DXVECTOR2(field[i].texPos.x + (float)x, field[i].texPos.y + (float)z);

					//ポインタ移動
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferField->Unlock();

	//=============================
	// インデックス作成
	//=============================
	makeIndexBufferField(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeIndexBuffer
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferField	( LPDIRECT3DDEVICE9	pDevice )
{
	//変数宣言
	HRESULT		hr;		//インデックス設定の確認
	WORD		*pIdx;	//インデックス番号
	int			i, j;	//loop
	int			x, z;	//xとzのloop
	int			num;	//必要なインデックス数		
	int			numIdx;	//n個目のインデックス数
	
	//=============================
	// 何頂点必要か
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z * 2));
			num	+=	(int)((field[i].numTyle.z - 1) * 2);
			j++;
		}
	}

	if(g_pIdxBufferField != NULL)
	{
		g_pIdxBufferField->Release();
		g_pIdxBufferField = NULL;
	}

	//=============================
	// インデックス作成
	//=============================
	hr	=	pDevice->CreateIndexBuffer(
		sizeof(WORD) *  num,	//必要なインデックスサイズ
		D3DUSAGE_WRITEONLY,			//使用用途フラグ
		D3DFMT_INDEX16,				//インデックスのフォーマット
		D3DPOOL_MANAGED,			//メモリの管理方法（お任せモード）
		&g_pIdxBufferField,			//インデックスバッファインターフェイス
		NULL);

	//=============================
	// インデックス登録
	//=============================
	g_pIdxBufferField->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, numIdx = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			for(z = 0; z < field[i].numTyle.z; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//インデックス登録
					pIdx[0]	=	(WORD)((field[i].numTyle.x + 1) * (z + 1) + x + numIdx);
					pIdx[1]	=	(WORD)((field[i].numTyle.x + 1) * z + x + numIdx);

					//ポインタ移動
					pIdx	+=	2;
				}

				//縮退ポリゴン
				if(z	<	field[i].numTyle.z - 1)
				{
					//インデックス登録
					x--;
					pIdx[0]	=	(WORD)((field[i].numTyle.x + 1) * z + x + numIdx);
					pIdx[1]	=	(WORD)((field[i].numTyle.x + 1) * (z + 1) + x + numIdx + 1);

					//ポインタ移動
					pIdx	+=	2;
				}
			}

			//次が始まるインデックス数
			numIdx	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.y + 1));

			j++;
		}
	}

	g_pIdxBufferField->Unlock();

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getField
//返り値	: 
//説明		: フィールドのポインタ
//////////////////////////////////////////////////////////////////////////////
FIELD	*getField	( int id )
{
	//変数宣言
	int		i;	//loop
	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return &field[i];
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getFieldPos
//返り値	: 
//説明		: 座標取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getFieldPos	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].pos;
		}
	}
	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getFieldSize
//返り値	: 
//説明		: サイズ取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getFieldSize	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].size;
		}
	}
	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getFieldColor
//返り値	: 
//説明		: 頂点色取得
//////////////////////////////////////////////////////////////////////////////
D3DXCOLOR	getFieldColor	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].color;
		}
	}
	return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getFieldTexMove
//返り値	: 
//説明		: テクスチャ移動量取得
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR2	getFieldTexMove	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].texMove;
		}
	}
	return D3DXVECTOR2(0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getFieldTexMove
//返り値	: 
//説明		: テクスチャタイプ取得
//////////////////////////////////////////////////////////////////////////////
FIELD_TEX	getFieldTex	( int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].tex;
		}
	}
	return FIELD_TEX_NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setFieldPos
//返り値	: 
//説明		: サイズ取得
//////////////////////////////////////////////////////////////////////////////
void	setFieldPos	( D3DXVECTOR3 pos, int id )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			field[i].pos = pos;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: releaseField
//返り値	: 
//説明		: フィールドリリース
//////////////////////////////////////////////////////////////////////////////
void	releaseField	( int id )
{
	//変数宣言
	int		i;		//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			field[i].use = false;

			g_numField--;
			return;
		}
	}
}