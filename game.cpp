/*****************************************************************************
ゲームモード
Aythor	: 上野　匠
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//インクルード
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "polygon.h"
#include "model.h"
#include "meshField.h"
#include "particle.h"
#include "meshWall.h"
#include "meshDome.h"
#include "keyOpe.h"
#include "input.h"
#include "bird.h"
#include "wood.h"
#include "rock.h"
#include "spear.h"
#include "game.h"
#include "fade.h"
#include "render.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	NUM_FIELD_X		(50)		//フィールドの横のタイルの数
#define	NUM_FIELD_Y		(50)		//フィールドの縦のタイルの数
#define	FIELD_SIZE_X	(10)		//フィールドの横幅
#define	FIELD_SIZE_Y	(10)		//フィールドの奥行

#define	BIRD_PARCENT	(80)		//カモメが出る確率
#define BIRD000_PARCENT	(30)		//カモメ000が出る確率
#define	BIRD001_PARCENT	(70)		//カモメ001が出る確率
#define	BIRD_MAX_SPEED	(0.5)		//カモメの最高速度

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	updateGameField	( void );
void	randObj			( D3DXVECTOR3 pos );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
int		fieldIdManage[2][FIELD_Y][FIELD_X];

//////////////////////////////////////////////////////////////////////////////
//関数名	:	initGame
//返り値	:	
//説明		:	ゲームに必要なファイルの初期化
//////////////////////////////////////////////////////////////////////////////
HRESULT	initGame	( void )
{
	//変数宣言
	int			x, y;		//フィールドの縦横分loop
	D3DXVECTOR2	fieldSize;	//フィールド全体のサイズ

	ShowCursor(FALSE);
	FixingMouse(true);
	
	//=============================
	// フェード初期化
	//=============================
	setFade(D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		100,
		FADE_IN,
		0);

	//=============================
	// ポリゴン初期化
	//=============================
	//initPolygon();

	//=============================
	// キー操作初期化
	//=============================
	initKeyOpe();

	//=============================
	// コリジョン初期化
	//=============================
	initHit();

	//=============================
	// モデル初期化
	//=============================
	initModel();

	//=============================
	// パーティクル初期化
	//=============================
	initParticle();

	//=============================
	// メッシュフィールド初期化
	//=============================
	initMeshField();

	fieldSize	=	D3DXVECTOR2(
		-(FIELD_SIZE_X * NUM_FIELD_X * 0.5f) * (FIELD_X - 1),
		(FIELD_SIZE_Y * NUM_FIELD_Y * 0.5f) * (FIELD_Y - 1));

	for(y = 0; y < FIELD_Y; y++)
	{
		for(x = 0; x < FIELD_X; x++)
		{
			//フィールド設定
			setMeshField(
				D3DXVECTOR3(fieldSize.x + (FIELD_SIZE_X * NUM_FIELD_X * x), 0.0f, fieldSize.y - (FIELD_SIZE_Y * NUM_FIELD_Y * y)),
				D3DXVECTOR3(FIELD_SIZE_X, 0.f, FIELD_SIZE_Y),
				D3DXVECTOR3(NUM_FIELD_X, 0, NUM_FIELD_Y),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXCOLOR(0.7f, 0.7f, 1.f, 1.f),
				D3DXVECTOR2(-0.001f, 0.001f),
				FIELD_TEX_003,
				x + y * FIELD_X);
			fieldIdManage[0][y][x]	=	x + y * FIELD_X;
			
			setMeshField(
				D3DXVECTOR3(fieldSize.x + (FIELD_SIZE_X * NUM_FIELD_X * x), 0.01f, fieldSize.y - (FIELD_SIZE_Y * NUM_FIELD_Y * y)),
				D3DXVECTOR3(FIELD_SIZE_X, 0.f, FIELD_SIZE_Y),
				D3DXVECTOR3(NUM_FIELD_X, 0, NUM_FIELD_Y),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.3f),
				D3DXVECTOR2(-0.0005f, 0.0005f),
				FIELD_TEX_002,
				x + y * FIELD_Y + FIELD_X * FIELD_Y);
			fieldIdManage[1][y][x]	=	x + y * FIELD_Y + FIELD_X * FIELD_Y;
		}
	}

	//=============================
	// メッシュウォール初期化
	//=============================
	//initMeshWall();

	//=============================
	// メッシュドーム初期化
	//=============================
	initMeshDome();

	setMeshDome(
		D3DXVECTOR3(0.f, 0.f, 0.f),
		(30.f * 0.5f) * 30,
		D3DXVECTOR2(16, 32),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		DOME_TEX_001,
		DOME_ID_000);

	//=============================
	// 鳥モデル初期化
	//=============================
	initBird();
	setBird(
		D3DXVECTOR3(0.f, 10.f, 30.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.3f,
		BIRD_TYPE_001);

	setBird(
		D3DXVECTOR3(20.f, 10.f, 30.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		BIRD_TYPE_000);
	
	//=============================
	// 流木モデル初期化
	//=============================
	initWood();

	setWood(
		D3DXVECTOR3(-20.f, 5.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 10.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		WOOD_TYPE_000);
	
	setWood(
		D3DXVECTOR3(-40.f, 5.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 10.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		WOOD_TYPE_001);
	
	//=============================
	// 槍モデル初期化
	//=============================
	initSpear();
	
	setSpear(
		D3DXVECTOR3(0.f, 10.f, 10.f),
		D3DXVECTOR3(1.f, 1.f, 1.f),
		D3DXVECTOR3(0.f, PI, 0.f),
		D3DXVECTOR3(0.f, 1.f, 0.5f),
		2.f);
	
	//=============================
	// 岩モデル初期化
	//=============================
	initRock();

	setRock(
		D3DXVECTOR3(50.f, 10.f, 30.f),
		D3DXVECTOR3(1.f, 1.f, 1.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		ROCK_TYPE_000);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	uninitGame
//返り値	:	
//説明		:	ゲーム終了
//////////////////////////////////////////////////////////////////////////////
void	uninitGame	( void )
{
	//ポリゴン終了
	//uninitPolygon();

	//岩終了
	uninitRock();

	//槍終了
	uninitSpear();

	//流木終了
	uninitWood();

	//鳥終了
	uninitBird();

	//モデル終了
	uninitModel();

	//パーティクル終了
	uninitParticle();

	//メッシュフィールド終了
	uninitMeshField();

	//メッシュウォール終了
	//uninitMeshWall();

	//メッシュドーム終了
	uninitMeshDome();

	//コリジョン終了
	uninitHit();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateGame
//返り値	:	
//説明		:	ゲーム更新
//////////////////////////////////////////////////////////////////////////////
void	updateGame	( void )
{
	//ポリゴン更新
	//updatePolygon();

	//キー操作更新
	updateKeyOpe();

	//槍更新
	updateSpear();

	//流木更新
	updateWood();

	//鳥更新
	updateBird();

	//岩更新
	updateRock();

	//モデル更新
	updateModel();

	//パーティクル更新
	updateParticle();

	//メッシュフィールド更新
	updateMeshField();

	//メッシュウォール更新
	//updateMeshWall();

	//メッシュドーム更新
	updateMeshDome();

	//フィールド情報更新
	updateGameField();

	//コリジョン更新
	updateHit();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	drawGame
//返り値	:	
//説明		:	ゲーム描画
//////////////////////////////////////////////////////////////////////////////
void	drawGame	( void )
{
	//3D描画モード
	multiDevice();

	//ポリゴン描写
	//drawPolygon();

	//槍描画
	drawSpear();

	//岩描画
	drawRock();
		
	//メッシュフィールド描画
	drawMeshField();

	//メッシュウォール描画
	//drawMeshWall();

	//メッシュドーム描画
	drawMeshDome();

	//鳥描画
	drawBird();

	//流木描画
	drawWood();

	//モデル描画
	drawModel();

	//パーティクル描画
	drawParticle();

	//コリジョン描画
	drawHit();

	//2D描画モード
	normal2D();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	updateGameField
//返り値	:	
//説明		:	ゲーム中のフィールド更新
//////////////////////////////////////////////////////////////////////////////
void	updateGameField	( void )
{
	//変数宣言
	CAMERA		*Camera	=	getCamera();		//カメラ構造体のポインタ
	FIELD		*Field	=	getField(fieldIdManage[0][1][1]);	//フィールド構造体のポインタ
	FIELD		*FieldWork;
	int			i;			//loop
	int			idWork;		//IDワーク

	if(Camera->posEye.x > Field->pos.x + (Field->size.x * Field->numTyle.x * 0.5f))
	{
		for(i = 0; i < FIELD_Y; i++)
		{
			FieldWork = getField(fieldIdManage[0][i][FIELD_X - 1]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[0][i][0]);
			
			FieldWork = getField(fieldIdManage[1][i][FIELD_X - 1]);
			
			setFieldPos(D3DXVECTOR3(
				FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[1][i][0]);

			idWork	=	fieldIdManage[0][i][0];
			fieldIdManage[0][i][0] = fieldIdManage[0][i][1];
			fieldIdManage[0][i][1] = fieldIdManage[0][i][2];
			fieldIdManage[0][i][2] = idWork;
			
			idWork	=	fieldIdManage[1][i][0];
			fieldIdManage[1][i][0] = fieldIdManage[1][i][1];
			fieldIdManage[1][i][1] = fieldIdManage[1][i][2];
			fieldIdManage[1][i][2] = idWork;
			
			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}

	if(Camera->posEye.x < Field->pos.x - (Field->size.x * Field->numTyle.x * 0.5f))
	{
		for(i = 0; i < FIELD_Y; i++)
		{
			FieldWork = getField(fieldIdManage[0][i][0]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x - (FieldWork->size.x * FieldWork->numTyle.x),
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[0][i][FIELD_X - 1]);
			
			FieldWork = getField(fieldIdManage[1][i][0]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x - (FieldWork->size.x * FieldWork->numTyle.x),
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[1][i][FIELD_X - 1]);

			idWork	=	fieldIdManage[0][i][FIELD_X - 1];
			fieldIdManage[0][i][2] = fieldIdManage[0][i][1];
			fieldIdManage[0][i][1] = fieldIdManage[0][i][0];
			fieldIdManage[0][i][0] = idWork;
			
			idWork	=	fieldIdManage[1][i][FIELD_X - 1];
			fieldIdManage[1][i][2] = fieldIdManage[1][i][1];
			fieldIdManage[1][i][1] = fieldIdManage[1][i][0];
			fieldIdManage[1][i][0] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}

	if(Camera->posEye.z > Field->pos.z + (Field->size.z * Field->numTyle.z * 0.5f))
	{
		for(i = 0; i < FIELD_X; i++)
		{
			FieldWork = getField(fieldIdManage[0][0][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[0][FIELD_X - 1][i]);
			
			FieldWork = getField(fieldIdManage[1][0][i]);
			
			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][FIELD_X - 1][i]);
			
			idWork	=	fieldIdManage[0][FIELD_X - 1][i];
			fieldIdManage[0][2][i] = fieldIdManage[0][1][i];
			fieldIdManage[0][1][i] = fieldIdManage[0][0][i];
			fieldIdManage[0][0][i] = idWork;
			
			idWork	=	fieldIdManage[1][FIELD_X - 1][i];
			fieldIdManage[1][2][i] = fieldIdManage[1][1][i];
			fieldIdManage[1][1][i] = fieldIdManage[1][0][i];
			fieldIdManage[1][0][i] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}
	
	if(Camera->posEye.z < Field->pos.z - (Field->size.z * Field->numTyle.z * 0.5f))
	{
		for(i = 0; i < FIELD_X; i++)
		{
			FieldWork = getField(fieldIdManage[0][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z - (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[0][0][i]);
			
			FieldWork = getField(fieldIdManage[1][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z - (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][0][i]);

			idWork	=	fieldIdManage[0][0][i];
			fieldIdManage[0][0][i] = fieldIdManage[0][1][i];
			fieldIdManage[0][1][i] = fieldIdManage[0][2][i];
			fieldIdManage[0][2][i] = idWork;
			
			idWork	=	fieldIdManage[1][0][i];
			fieldIdManage[1][0][i] = fieldIdManage[1][1][i];
			fieldIdManage[1][1][i] = fieldIdManage[1][2][i];
			fieldIdManage[1][2][i] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	:	randObj
//返り値	:	
//説明		:	ゲーム中のオブジェクトランダム配置
//////////////////////////////////////////////////////////////////////////////
void	randObj	( D3DXVECTOR3 pos )
{
	//カモメをランダム配置
	if(rand() % 100 <= 0)
	{
		if(rand() % 80 <= BIRD000_PARCENT)
		{
			setBird(
				pos,
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				0.3f,
				BIRD_TYPE_000);
		}
		else
		{
			pos.y += 80;
			setBird(
				pos,
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				(rand() % (int)(BIRD_MAX_SPEED * 100)) * 0.01f,
				BIRD_TYPE_001);
		}
	}
}