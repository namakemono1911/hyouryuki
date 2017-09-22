/*****************************************************************************
槍のモデル表示[spear.h]
Aythor	: 上野　匠
Data	: 2017_01_23
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "spear.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MODEL_NAME		"data/model/spear.x"
#define	SPEED			(2)
#define	MAX_SPEAR		(30)

#define GRAVITY			(0.03f)

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	setMatrixSpear	( LPDIRECT3DDEVICE9 pDev, int num );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshSpear		= NULL;		//メッシュ情報インターフェース
DWORD				g_nNumMatSpear		= NULL;		//マテリアル情報数
D3DMATERIAL9		*g_pmeshMatSpear	= NULL;		//マテリアル情報

LPDIRECT3DTEXTURE9	*g_pTextureSpear;			//テクスチャ情報インターフェース
SPEAR				spear[MAX_SPEAR];			//モデル構造体

int					g_numSpear = 0;				//出てるモデル数

//////////////////////////////////////////////////////////////////////////////
//関数名	: initSpear
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	initSpear	( void )
{
	//変数宣言
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPD3DXBUFFER	bufMatSpear;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i;

	//構造体初期化
	for(i = 0; i < MAX_SPEAR; i++)
	{
		spear[i].use	=	false;
	}

	hr	=	D3DXLoadMeshFromX(MODEL_NAME,
				D3DXMESH_MANAGED,
				pDev,
				NULL,
				&bufMatSpear,
				NULL,
				&g_nNumMatSpear,
				&g_pMeshSpear);
	
	if(FAILED(hr))
	{
		MessageBox(NULL, "モデル読み込みエラー", "error", MB_OK);
	}

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)bufMatSpear->GetBufferPointer();
	
	//テクスチャバッファ作成
	g_pmeshMatSpear	=	new D3DMATERIAL9[g_nNumMatSpear];
	g_pTextureSpear	=	new LPDIRECT3DTEXTURE9[g_nNumMatSpear];
	
	for(i = 0; i < (int)g_nNumMatSpear; i++)
	{
		g_pmeshMatSpear[i] =	pMat[i].MatD3D;		//マテリアル情報セット
		g_pmeshMatSpear[i].Ambient	=	g_pmeshMatSpear[i].Diffuse;	//環境光初期化
		g_pTextureSpear[i] = NULL;				//テクスチャ初期化

		//テクスチャの読み込み
		if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureSpear[i])))
		{
			if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureSpear[i])))
			{
				MessageBox(NULL, "エラー", "spearテクスチャエラー", MB_OK);
			}
		}
	}

	bufMatSpear->Release();
	bufMatSpear = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitSpear
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitSpear	( void )
{
	//変数宣言
	int		i;	//loop

	//メッシュ情報の解放
	if(g_pMeshSpear != NULL)
	{
		g_pMeshSpear->Release();
		g_pMeshSpear = NULL;
	}
	
	//テクスチャ解放
	for(i = 0; i < (int)g_nNumMatSpear; i++)
	{
		if(g_pTextureSpear[i] != NULL)
		{
			g_pTextureSpear[i]->Release();
			g_pTextureSpear[i] = NULL;
		}
	}

	delete[] g_pTextureSpear[i];
	g_pTextureSpear[i] = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateSpear
//返り値	:
//説明		:	更新処理
////////////////////////////////////////////////////////////////////////////
void	updateSpear	( void )
{
	//変数宣言
	int		i, j;		//loop

	for(i = 0, j = 0; j < g_numSpear; i++)
	{
		if(spear[i].use == true)
		{
			spear[i].pos = D3DXVECTOR3(
				spear[i].MoveVec.x * spear[i].move + spear[i].pos.x,
				spear[i].MoveVec.y * spear[i].move + spear[i].pos.y + spear[i].gravity,
				spear[i].MoveVec.z * spear[i].move + spear[i].pos.z);

			spear[i].gravity -= GRAVITY;

			if(spear[i].pos.y <= -10.f)
			{
				spear[i].use = false;
				g_numSpear--;
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawSpear
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	drawSpear	( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//変数宣言
	
	for(i = 0, j = 0; j < g_numSpear; i++)
	{
		if(spear[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatSpear; k++)
			{
				//行列設定
				setMatrixSpear(pDev, i);

				//マテリアル設定
				pDev->SetMaterial(&g_pmeshMatSpear[k]);

				//テクスチャ読み込み
				pDev->SetTexture(0, g_pTextureSpear[k]);

				//メッシュ描画
				g_pMeshSpear->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixSpear
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixSpear	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//変数宣言
	D3DXMATRIX	world;				//ワールド行列
	D3DXMATRIX	scale;				//拡縮行列
	D3DXMATRIX	rot;				//回転行列
	D3DXMATRIX	move;				//平行移動行列

	//ワールド行列初期化
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scale);

	//座標変換
	D3DXMatrixScaling(&scale, spear[num].size.x, spear[num].size.y, spear[num].size.z);	//拡縮行列
	D3DXMatrixRotationYawPitchRoll(&rot, spear[num].angle.x, spear[num].angle.y, spear[num].angle.z);		//回転行列
	D3DXMatrixTranslation(&move, spear[num].pos.x, spear[num].pos.y, spear[num].pos.z);	//平行移動
	
	//行列合成
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getSpear
//返り値	: 
//説明		: モデルのポインタ取得
//////////////////////////////////////////////////////////////////////////////
SPEAR	*getSpear	( int num )
{
	return &spear[num];
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setSpear
//返り値	: 
//説明		: 槍セット
//////////////////////////////////////////////////////////////////////////////
void	setSpear	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move )
{
	//変数宣言
	int		i;		//loop

	for(i = 0; i < MAX_SPEAR; i++)
	{
		if(spear[i].use == false)
		{
			//変数セット
			spear[i].pos		=	pos;		//座標
			spear[i].size		=	size;		//サイズ
			spear[i].angle		=	angle;		//角度
			spear[i].MoveVec	=	moveVec;	//方向ベクトル
			spear[i].move		=	move;		//速度
			spear[i].gravity	=	0.f;		//重力
			spear[i].use		=	true;		//使用フラグ

			g_numSpear++;
			break;
		}
	}
}