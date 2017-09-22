/*****************************************************************************
岩のモデル表示[rock.h]
Aythor	: 上野　匠
Data	: 2017_02_07
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "vector.h"
#include "rock.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_ROCK		(30)		//一度に出す最高数
#define	LIMIT_DISTANCE	(1000)		//限界距離

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	setMatrixRock	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateRock000	( int num );
void	updateRock001	( int num );
void	setCollision	( LPDIRECT3DDEVICE9 pDevice, LPD3DXMESH mesh );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshRock[ROCK_TYPE_MAX] = {NULL};	//メッシュ情報インターフェース
LPDIRECT3DTEXTURE9	*g_pTextureRock[ROCK_TYPE_MAX];			//テクスチャ情報インターフェース
DWORD				g_nNumMatRock = NULL;					//マテリアル情報数
D3DMATERIAL9		*g_pmeshMatRock = NULL;					//マテリアル情報

HIT_VTX				top[ROCK_TYPE_MAX];						//HitBox用
HIT_VTX				bottom[ROCK_TYPE_MAX];									
ROCK				rock[MAX_ROCK];							//モデル構造体
int					g_numRock = 0;							//モデル総数
char				*rockModelName[ROCK_TYPE_MAX] =			//テクスチャファイル名
{
	"data/model/rock001.x",
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initRock
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	initRock	( void )
{
	//変数宣言
	LPD3DXBUFFER		pBuffMatRock = NULL;		//マテリアル情報
	HRESULT				hr;
	D3DMATERIAL9		matDef;
	D3DXMATERIAL		*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	D3DXVECTOR3			max, min;	//最大最小頂点
	int		i, j;	//loop

	//構造体初期化
	for(i = 0; i < MAX_ROCK; i++)
	{
		rock[i].use		=	false;
	}
	
	for(j = 0; j < ROCK_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(rockModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatRock,
					NULL,
					&g_nNumMatRock,
					&g_pMeshRock[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "rockモデル読み込みエラー", "error", MB_OK);
			return;
		}

		//コリジョン登録
		getXfileVtx(rockModelName[j], &max, &min);
		setHitBoxVtx(max, min, &top[j], &bottom[j]);

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatRock->GetBufferPointer();

		g_pmeshMatRock	=	new D3DMATERIAL9[g_nNumMatRock];
		g_pTextureRock[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatRock];

		for(i = 0; i < (int)g_nNumMatRock; i++)
		{
			g_pmeshMatRock[i] =	pMat[i].MatD3D;		//マテリアル情報セット
			g_pmeshMatRock[i].Ambient	=	g_pmeshMatRock[i].Diffuse;	//環境光初期化
			g_pTextureRock[j][i] = NULL;				//テクスチャ初期化

			//テクスチャの読み込み
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureRock[j][i])))
				{
					MessageBox(NULL, "エラー", "rockテクスチャエラー", MB_OK);
				}
			}
		}
	}

	//マテリアル情報解放
	pBuffMatRock->Release();
	pBuffMatRock = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitRock
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitRock	( void )
{
	//変数宣言
	DWORD	i, j;		//loop

	//メッシュ情報の解放
	for(i = 0; i < ROCK_TYPE_MAX; i++)
	{
		if(g_pMeshRock[i] != NULL)
		{
			g_pMeshRock[i]->Release();

			delete[] g_pMeshRock;
			g_pMeshRock[i] = NULL;
		}
	}

	//テクスチャ解放
	for(i = 0; i < ROCK_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatRock; j++)
		{
			if(g_pTextureRock[i] != NULL)
			{
				g_pTextureRock[i][j]->Release();
				g_pTextureRock[i][j] = NULL;
			}
		}
		delete[] g_pTextureRock[i];
		g_pTextureRock[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateRock
//返り値	:
//説明		:	更新処理
////////////////////////////////////////////////////////////////////////////
void	updateRock	( void )
{
	//変数宣言
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numRock; i++)
	{
		if(rock[i].use == true)
		{
			if(getPosLen(getCameraPos(), rock[i].pos) > LIMIT_DISTANCE)
			{
				releaseRock(i);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawRock
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	drawRock	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//変数宣言
	
	for(i = 0, j = 0; j < g_numRock; i++)
	{
		if(rock[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatRock; k++)
			{
				//行列設定
				setMatrixRock(pDev, i);

				//マテリアル設定
				pDev->SetMaterial(&g_pmeshMatRock[k]);

				//テクスチャ読み込み
				pDev->SetTexture(0, g_pTextureRock[rock[i].type][k]);

				//メッシュ描画
				g_pMeshRock[rock[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixRock
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixRock	( LPDIRECT3DDEVICE9 pDev, int num )
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
	D3DXMatrixScaling(&scale, rock[num].size.x, rock[num].size.y, rock[num].size.z);					//拡縮
	D3DXMatrixRotationYawPitchRoll(&rot, rock[num].angle.x, rock[num].angle.y, rock[num].angle.z);		//回転行列
	D3DXMatrixTranslation(&move, rock[num].pos.x, rock[num].pos.y, rock[num].pos.z);					//平行移動
	
	//行列合成
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getRock
//返り値	: 
//説明		: モデルのポインタ取得
//////////////////////////////////////////////////////////////////////////////
ROCK	*getRock	( int num )
{
	if(rock[num].use != false)
	{
		return &rock[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setRock
//返り値	: 
//説明		: モデルのセット
//////////////////////////////////////////////////////////////////////////////
void	setRock	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, ROCK_TYPE type )
{
	//変数宣言
	int		i;	//loop
	for(i = 0; i < MAX_ROCK; i++)
	{
		if(rock[i].use == false)
		{
			rock[i].pos		=	pos;
			rock[i].size	=	size;
			rock[i].angle	=	angle;
			rock[i].type	=	type;
			rock[i].use		=	true;

			//コリジョン設定
			rock[i].collisionID = setHitBox(pos, top[type], bottom[type], TYPE_ROCK);

			g_numRock++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: releaseRock
//返り値	: 
//説明		: 岩リリース
//////////////////////////////////////////////////////////////////////////////
void	releaseRock	( int num )
{
	rock[num].use = false;
	g_numRock--;
}