/*****************************************************************************
鳥のモデル描画[bird.cpp]
Aythor	: 上野　匠
Data	: 2017_01_15
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "bird.h"
#include "vector.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MAX_BIRD		(30)		//一度に出す最高数
#define	LIMIT_DISTANCE	(1000)		//生存できる限界距離

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	setMatrixBird	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateBird000	( int num );
void	updateBird001	( int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshBird[BIRD_TYPE_MAX] = {NULL};	//メッシュ情報インターフェース
DWORD				g_nNumMatBird = NULL;		//マテリアル情報数
D3DMATERIAL9		*g_pmeshMatBird = NULL;		//マテリアル情報

LPDIRECT3DTEXTURE9	*g_pTextureBird[BIRD_TYPE_MAX];			//テクスチャ情報インターフェース
BIRD				bird[MAX_BIRD];				//モデル構造体
int					g_numBird = 0;				//モデル総数
char				*birdModelName[BIRD_TYPE_MAX] =
{
	"data/model/kamome.x",
	"data/model/flingkamome.x",
};

//////////////////////////////////////////////////////////////////////////////
//関数名	: initBird
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	initBird	( void )
{
	//変数宣言
	LPD3DXBUFFER	pBuffMatBird = NULL;		//マテリアル情報
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j;	//loop

	//構造体初期化
	for(i = 0; i < MAX_BIRD; i++)
	{
		bird[i].use		=	false;
	}
	
	for(j = 0; j < BIRD_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(birdModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatBird,
					NULL,
					&g_nNumMatBird,
					&g_pMeshBird[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "birdモデル読み込みエラー", "error", MB_OK);
			return;
		}

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatBird->GetBufferPointer();

		g_pmeshMatBird	=	new D3DMATERIAL9[g_nNumMatBird];
		g_pTextureBird[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatBird];

		for(i = 0; i < (int)g_nNumMatBird; i++)
		{
			g_pmeshMatBird[i] =	pMat[i].MatD3D;		//マテリアル情報セット
			g_pmeshMatBird[i].Ambient	=	g_pmeshMatBird[i].Diffuse;	//環境光初期化
			g_pTextureBird[j][i] = NULL;				//テクスチャ初期化

			//テクスチャの読み込み
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureBird[j][i])))
				{
					MessageBox(NULL, "エラー", "birdテクスチャエラー", MB_OK);
				}
			}
		}
	}

	//マテリアル情報解放
	pBuffMatBird->Release();
	pBuffMatBird = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitBird
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitBird	( void )
{
	//変数宣言
	DWORD	i, j;		//loop

	//メッシュ情報の解放
	for(i = 0; i < BIRD_TYPE_MAX; i++)
	{
		if(g_pMeshBird[i] != NULL)
		{
			g_pMeshBird[i]->Release();

			delete[] g_pMeshBird;
			g_pMeshBird[i] = NULL;
		}
	}

	//テクスチャ解放
	for(i = 0; i < BIRD_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatBird; j++)
		{
			if(g_pTextureBird[i] != NULL)
			{
				g_pTextureBird[i][j]->Release();
				g_pTextureBird[i][j] = NULL;
			}
		}
		delete[] g_pTextureBird[i];
		g_pTextureBird[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateBird
//返り値	:
//説明		:	更新処理
////////////////////////////////////////////////////////////////////////////
void	updateBird	( void )
{
	//変数宣言
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numBird; i++)
	{
		if(bird[i].use == true)
		{
			switch(bird[i].type)
			{
			case BIRD_TYPE_000:
				{
					updateBird000(i);
					break;
				}

			case BIRD_TYPE_001:
				{
					updateBird001(i);
					break;
				}

			default:
				{
					MessageBox(NULL, "error", "birdupdateエラー", MB_OK);
					break;
				}
			}
			
			if(getPosLen(getCameraPos(), bird[i].pos) > LIMIT_DISTANCE)
			{
				releaseBird(i);
			}

			j++;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateBird000
//返り値	: 
//説明		: モデルタイプ000の更新
//////////////////////////////////////////////////////////////////////////////
void	updateBird000	( int num )
{
	//変数宣言


}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateBird001
//返り値	: 
//説明		: モデルタイプ001の更新
//////////////////////////////////////////////////////////////////////////////
void	updateBird001	( int num )
{
	//変数宣言

	//角度更新
	bird[num].angle.x += 0.005f;
	bird[num].angle.x = fmod(bird[num].angle.x, PI * 2);

	//進行方向変更
	bird[num].moveVec = D3DXVECTOR3(
		-cos(bird[num].angle.x - PI * 0.5f),
		0.f,
		sin(bird[num].angle.x - PI * 0.5f));

	D3DXVec3Normalize(&bird[num].moveVec, &bird[num].moveVec);

	//座標加算
	bird[num].pos = D3DXVECTOR3(
		bird[num].move * bird[num].moveVec.x + bird[num].pos.x,
		bird[num].move * bird[num].moveVec.y + bird[num].pos.y,
		bird[num].move * bird[num].moveVec.z + bird[num].pos.z);

	setHitBallPos(bird[num].collisionID, bird[num].pos);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawBird
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	drawBird	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//変数宣言
	
	for(i = 0, j = 0; j < g_numBird; i++)
	{
		if(bird[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatBird; k++)
			{
				//行列設定
				setMatrixBird(pDev, i);

				//マテリアル設定
				pDev->SetMaterial(&g_pmeshMatBird[k]);

				//テクスチャ読み込み
				pDev->SetTexture(0, g_pTextureBird[bird[i].type][k]);

				//メッシュ描画
				g_pMeshBird[bird[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixBird
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixBird	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//変数宣言
	D3DXMATRIX	world;				//ワールド行列
	D3DXMATRIX	move;				//平行移動行列
	D3DXMATRIX	rot;				//回転行列

	//ワールド行列初期化
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);

	//座標変換
	D3DXMatrixRotationYawPitchRoll(&rot, bird[num].angle.x, bird[num].angle.y, bird[num].angle.z);		//回転行列
	D3DXMatrixTranslation(&move, bird[num].pos.x, bird[num].pos.y, bird[num].pos.z);	//平行移動
	
	//行列合成
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getBird
//返り値	: 
//説明		: モデルのポインタ取得
//////////////////////////////////////////////////////////////////////////////
BIRD	*getBird	( int num )
{
	if(bird[num].use != false)
	{
		return &bird[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setBird
//返り値	: 
//説明		: モデルのセット
//////////////////////////////////////////////////////////////////////////////
void	setBird	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move, BIRD_TYPE type)
{
	//変数宣言
	int		i;	//loop

	for(i = 0; i < MAX_BIRD; i++)
	{
		if(bird[i].use == false)
		{
			bird[i].pos		=	pos;
			bird[i].size	=	size;
			bird[i].angle	=	angle;
			bird[i].moveVec	=	moveVec;
			bird[i].move	=	move;
			bird[i].type	=	type;
			bird[i].use		=	true;

			//コリジョン設定
			bird[i].collisionID = setHitBall(pos, 5.f, TYPE_BIRD);

			g_numBird++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: releaseBird
//返り値	: 
//説明		: この鳥を無かったことに
//////////////////////////////////////////////////////////////////////////////
void	releaseBird	( int num )
{
	//コリジョン解放
	releaseHitBall(bird[num].collisionID);

	
	bird[num].use = false;
	g_numBird--;
}