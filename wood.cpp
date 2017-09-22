/*****************************************************************************
流木のモデル表示[wood.h]
Aythor	: 上野　匠
Data	: 2017_01_18
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "vector.h"
#include "wood.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_WOOD		(30)		//一度に出す最高数
#define	LIMIT_DISTANCE	(1000)		//限界距離

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWood	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateWood000	( int num );
void	updateWood001	( int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshWood[WOOD_TYPE_MAX] = {NULL};	//メッシュ情報インターフェース
DWORD				g_nNumMatWood = NULL;					//マテリアル情報数
D3DMATERIAL9		*g_pmeshMatWood = NULL;					//マテリアル情報

LPDIRECT3DTEXTURE9	*g_pTextureWood[WOOD_TYPE_MAX];			//テクスチャ情報インターフェース
WOOD				wood[MAX_WOOD];							//モデル構造体
int					g_numWood = 0;							//モデル総数
char				*woodModelName[WOOD_TYPE_MAX] =			//テクスチャファイル名
{
	"data/model/wood000.x",
	"data/model/wood001.x",
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initWood
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	initWood	( void )
{
	//変数宣言
	LPD3DXBUFFER		pBuffMatWood = NULL;		//マテリアル情報
	HRESULT				hr;
	D3DMATERIAL9		matDef;
	D3DXMATERIAL		*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j;	//loop

	//構造体初期化
	for(i = 0; i < MAX_WOOD; i++)
	{
		wood[i].use		=	false;
	}
	
	for(j = 0; j < WOOD_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(woodModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatWood,
					NULL,
					&g_nNumMatWood,
					&g_pMeshWood[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "woodモデル読み込みエラー", "error", MB_OK);
			return;
		}

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatWood->GetBufferPointer();

		g_pmeshMatWood	=	new D3DMATERIAL9[g_nNumMatWood];
		g_pTextureWood[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatWood];

		for(i = 0; i < (int)g_nNumMatWood; i++)
		{
			g_pmeshMatWood[i] =	pMat[i].MatD3D;		//マテリアル情報セット
			g_pmeshMatWood[i].Ambient	=	g_pmeshMatWood[i].Diffuse;	//環境光初期化
			g_pTextureWood[j][i] = NULL;				//テクスチャ初期化

			//テクスチャの読み込み
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureWood[j][i])))
				{
					MessageBox(NULL, "エラー", "woodテクスチャエラー", MB_OK);
				}
			}
		}
	}

	//マテリアル情報解放
	pBuffMatWood->Release();
	pBuffMatWood = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitWood
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitWood	( void )
{
	//変数宣言
	DWORD	i, j;		//loop

	//メッシュ情報の解放
	for(i = 0; i < WOOD_TYPE_MAX; i++)
	{
		if(g_pMeshWood[i] != NULL)
		{
			g_pMeshWood[i]->Release();

			delete[] g_pMeshWood;
			g_pMeshWood[i] = NULL;
		}
	}

	//テクスチャ解放
	for(i = 0; i < WOOD_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatWood; j++)
		{
			if(g_pTextureWood[i] != NULL)
			{
				g_pTextureWood[i][j]->Release();
				g_pTextureWood[i][j] = NULL;
			}
		}
		delete[] g_pTextureWood[i];
		g_pTextureWood[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateWood
//返り値	:
//説明		:	更新処理
////////////////////////////////////////////////////////////////////////////
void	updateWood	( void )
{
	//変数宣言
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numWood; i++)
	{
		if(wood[i].use == true)
		{
			if(getPosLen(getCameraPos(), wood[i].pos) > LIMIT_DISTANCE)
			{
				releaseWood(i);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawWood
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	drawWood	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//変数宣言
	
	for(i = 0, j = 0; j < g_numWood; i++)
	{
		if(wood[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatWood; k++)
			{
				//行列設定
				setMatrixWood(pDev, i);

				//マテリアル設定
				pDev->SetMaterial(&g_pmeshMatWood[k]);

				//テクスチャ読み込み
				pDev->SetTexture(0, g_pTextureWood[wood[i].type][k]);

				//メッシュ描画
				g_pMeshWood[wood[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixWood
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWood	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//変数宣言
	D3DXMATRIX	world;		//ワールド行列
	D3DXMATRIX	scale;		//拡縮行列
	D3DXMATRIX	rot;		//回転行列
	D3DXMATRIX	move;		//平行移動行列

	//ワールド行列初期化
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scale);

	//座標変換
	D3DXMatrixScaling(&scale, wood[num].size.x, wood[num].size.y, wood[num].size.z);					//拡縮
	D3DXMatrixRotationYawPitchRoll(&rot, wood[num].angle.x, wood[num].angle.y, wood[num].angle.z);		//回転行列
	D3DXMatrixTranslation(&move, wood[num].pos.x, wood[num].pos.y, wood[num].pos.z);					//平行移動
	
	//行列合成
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getWood
//返り値	: 
//説明		: モデルのポインタ取得
//////////////////////////////////////////////////////////////////////////////
WOOD	*getWood	( int num )
{
	if(wood[num].use != false)
	{
		return &wood[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setWood
//返り値	: 
//説明		: モデルのセット
//////////////////////////////////////////////////////////////////////////////
void	setWood	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move, WOOD_TYPE type )
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_WOOD; i++)
	{
		if(wood[i].use == false)
		{
			wood[i].pos		=	pos;
			wood[i].size	=	size;
			wood[i].angle	=	angle;
			wood[i].moveVec	=	moveVec;
			wood[i].move	=	move;
			wood[i].type	=	type;
			wood[i].use		=	true;

			g_numWood++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: releaseWood
//返り値	: 
//説明		: 流木リリース
//////////////////////////////////////////////////////////////////////////////
void	releaseWood	( int num )
{
	wood[num].use = false;
	g_numWood--;
}