/*****************************************************************************
Xファイル描画[model.cpp]
Aythor	: 上野　匠
Data	: 2016_11_04
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	MODEL_NAME		"data/model/raft.x"
#define	SPEED			(2)

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	setMatrixModel	( LPDIRECT3DDEVICE9 pDev );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshModel = NULL;		//メッシュ情報インターフェース
LPD3DXBUFFER		g_pBuffMatModel = NULL;		//マテリアル情報
DWORD				g_nNumMatModel = NULL;		//マテリアル情報数

HIT_VTX				top;						//HitBox用
HIT_VTX				bottom;
LPDIRECT3DTEXTURE9	g_pTextureModel[4];			//テクスチャ情報インターフェース
MODEL				model;						//モデル構造体

//////////////////////////////////////////////////////////////////////////////
//関数名	: initModel
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	initModel	( void )
{
	//変数宣言
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	D3DXVECTOR3		max, min;
	int		i;

	//構造体初期化
	model.pos		=	D3DXVECTOR3(0.f, 0.f, 0.f);
	model.angle		=	D3DXVECTOR3(PI, 0.f, 0.f);
	model.MoveVec	=	D3DXVECTOR3(0.f, 0.f, 1.f);
	model.move		=	SPEED;

	hr	=	D3DXLoadMeshFromX(MODEL_NAME,
				D3DXMESH_MANAGED,
				pDev,
				NULL,
				&g_pBuffMatModel,
				NULL,
				&g_nNumMatModel,
				&g_pMeshModel);
	
	if(FAILED(hr))
	{
		MessageBox(NULL, "モデル読み込みエラー", "error", MB_OK);
	}

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for(i = 0; i < (int)g_nNumMatModel; i++)
	{
		//テクスチャの読み込み
		if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureModel[i])))
		{
			MessageBox(NULL, "エラー", "エラー", MB_OK);
		}
	}

	//コリジョン頂点設定
	getXfileVtx(MODEL_NAME, &max, &min);
	max.y = 5.f;
	setHitBoxVtx(max, min, &top, &bottom);
	model.collisionID = setHitBox(model.pos, top, bottom, TYPE_PLAYER);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitModel
//返り値	: 
//説明		: 終了
//////////////////////////////////////////////////////////////////////////////
void	uninitModel	( void )
{
	//メッシュ情報の解放
	if(g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアル情報の解放
	if(g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateModel
//返り値	:
//説明		:	更新処理
////////////////////////////////////////////////////////////////////////////
void	updateModel	( void )
{
	//変数宣言
	int		i;		//loop
	HIT_BOX	player;
	HIT_BOX	work;

	player = getHitBox(model.collisionID);

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(getHitBoxType(i) == TYPE_ROCK)
		{
			work = getHitBox(i);

			if(hitBox(player, work) == true)
			{

			}
		}
	}

	//コリジョン座標更新
	setHitBoxPos(model.collisionID, model.pos);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawModel
//返り値	: 
//説明		: 初期化処理
//////////////////////////////////////////////////////////////////////////////
void	drawModel	( void )
{
	//変数宣言
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i;//変数宣言
	
	//行列設定
	setMatrixModel(pDev);

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for(i = 0; i < (int)g_nNumMatModel; i++)
	{
		//マテリアル設定
		pDev->SetMaterial(&matDef);

		//テクスチャ読み込み
		pDev->SetTexture(0, g_pTextureModel[i]);

		//メッシュ描画
		g_pMeshModel->DrawSubset(i);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrixModel
//返り値	: 
//説明		: 行列設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixModel	( LPDIRECT3DDEVICE9 pDev )
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
	D3DXMatrixRotationYawPitchRoll(&rot, model.angle.x, model.angle.y, model.angle.z);		//回転行列
	D3DXMatrixTranslation(&move, model.pos.x, model.pos.y, model.pos.z);	//平行移動
	
	//行列合成
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: getModel
//返り値	: 
//説明		: モデルのポインタ取得
//////////////////////////////////////////////////////////////////////////////
MODEL	*getModel	( void )
{
	return &model;
}