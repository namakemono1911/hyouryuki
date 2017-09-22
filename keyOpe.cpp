/*****************************************************************************
キー操作
Aythor	: 上野　匠
Data	: 2016_12_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"input.h"
#include	"vector.h"
#include	"camera.h"
#include	"meshField.h"
#include	"game.h"
#include	"keyOpe.h"
#include	"model.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define MOUSE_SPEED		(0.005f)

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	PLAY_FIRST = 0,
	PLAY_THIRD,
}PLAY_MODE;

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	cameraOpe	( void );
void	playerOpe	( void );
void	craftOpe	( void );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
OPE_MODE	g_opeMode = OPE_NULL;

float		g_length = 0;		//モデルからカメラの距離
D3DXVECTOR3	g_cameraPos;		//カメラの位置
PLAY_MODE	g_playMode = PLAY_FIRST;			//操作モード

//////////////////////////////////////////////////////////////////////////////
//関数名	: initKeyOpe
//返り値	: 
//説明		: キー操初期化
//////////////////////////////////////////////////////////////////////////////
void	initKeyOpe	( void )
{
	//変数宣言
	CAMERA	*Camera = getCamera();

	g_opeMode = OPE_PLAY;
	Camera->angleXZ = PI * 0.5f;
	g_cameraPos = D3DXVECTOR3(0.f, 10.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitKeyOpe
//返り値	: 
//説明		: キー操作終了
//////////////////////////////////////////////////////////////////////////////
void	uninitKeyOpe	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateKeyOpe
//返り値	: 
//説明		: キー操作更新
//////////////////////////////////////////////////////////////////////////////
void	updateKeyOpe	( void )
{
	switch(g_opeMode)
	{
	case OPE_PLAY:
		{
			//プレイヤー操作
			playerOpe();

			//カメラ操作
			cameraOpe();

			break;
		}

	case OPE_CRAFT:
		{
			//クラフト操作
			craftOpe();

			break;
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{
		setMode(TITLE_MODE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateKeyOpe
//返り値	: 
//説明		: ゲーム中のキー操作
//////////////////////////////////////////////////////////////////////////////
void	gameKeyOpe	( int num )
{

}

//////////////////////////////////////////////////////////////////////////////
//関数名	: cameraOpe
//返り値	: 
//説明		: カメラ操作
//////////////////////////////////////////////////////////////////////////////
void	cameraOpe	( void )
{
	//変数宣言
	CAMERA		*Camera	=	getCamera();
	MODEL		*Model	=	getModel();
	D3DXVECTOR3	cameraVec;
	float		z;

	//=============================
	// 注視点移動
	//=============================
	Camera->angleXZ	+=	GetMouseMoving().x * MOUSE_SPEED;
	Camera->angleYZ	+=	GetMouseMoving().y * MOUSE_SPEED;
	Camera->angleXZ	=	fmod(Camera->angleXZ, PI * 2);
	if(Camera->angleXZ <= 0)
	{
		Camera->angleXZ += PI * 2;
	}

	if(Camera->angleYZ > PI * 0.5f)
	{
		Camera->angleYZ	=	PI * 0.5f;
	}
	if(Camera->angleYZ < -PI * 0.5f)
	{
		Camera->angleYZ = -PI * 0.5f;
	}

	Camera->posAt	=	D3DXVECTOR3(
		Camera->len * cos(Camera->angleXZ) + Model->pos.x,
		Camera->len * sin(Camera->angleYZ) + Model->pos.y,
		Model->pos.z);

	z = sqrt(Camera->len * Camera->len - (Camera->len * cos(Camera->angleXZ) * Camera->len * cos(Camera->angleXZ)));
	
	if(Camera->angleXZ >= PI)
	{
		z *= -1;
	}
	Camera->posAt.z += z;

	//=============================
	// 平行移動
	//=============================
	cameraVec = Camera->posAt - Model->pos;

	Camera->posEye	=	D3DXVECTOR3(
		-cameraVec.x * g_length + Model->pos.x + g_cameraPos.x,
		-cameraVec.y * g_length + Model->pos.y + g_cameraPos.y,
		-cameraVec.z * g_length + Model->pos.z + g_cameraPos.z);

	//=============================
	//三人称に切り替え 
	//=============================
	if(GetMouseTrigger(MOUSE_MIDDLE))
	{
		if(g_playMode == PLAY_FIRST)
		{
			g_length += 5;
			g_cameraPos.y = 30.f;
			g_playMode = PLAY_THIRD;
		}

		else if(g_playMode == PLAY_THIRD)
		{
			g_length = 0;
			g_cameraPos.y = 10;
			g_playMode = PLAY_FIRST;
		}
	}

	GetMousePos();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: playerOpe
//返り値	: 
//説明		: カメラ操作
//////////////////////////////////////////////////////////////////////////////
void	playerOpe	( void )
{
	//変数宣言
	CAMERA		*Camera	=	getCamera();		//カメラ構造体のポインタ
	MODEL		*Model	=	getModel();			//モデル構造体のポインタ
	D3DXVECTOR3	posEye, posAt;

	if(GetKeyboardPress(DIK_W))
	{
		Model->pos = D3DXVECTOR3(
			Model->MoveVec.x * Model->move + Model->pos.x,
			Model->MoveVec.y * Model->move + Model->pos.y,
			Model->MoveVec.z * Model->move + Model->pos.z);
	}

	if(GetKeyboardPress(DIK_A))
	{
		Model->angle.x -= 0.04f;
	}

	if(GetKeyboardPress(DIK_D))
	{
		Model->angle.x += 0.04f;
	}

	Model->MoveVec = D3DXVECTOR3(
		sin(Model->angle.x + PI),
		0.f,
		cos(Model->angle.x + PI));

	if(GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_LEFT))
	{
		posEye = Camera->posEye;
		posEye.y = 0.f;

		posAt = Camera->posAt;
		posAt.y = 0.f;

		posAt -= posEye;
		posEye = D3DXVECTOR3(0.f, 0.f, -Camera->len);

		Model->angle.x = (float)getAngleVector(posEye, posAt);
		if(posAt.x < 0)
		{
			Model->angle *= -1;
		}
	}

	/*
	if(GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{
		Model->pos = D3DXVECTOR3(
			Camera->posEye.x,
			Model->pos.y,
			Camera->posEye.z);
	}
	*/
	if(GetKeyboardTrigger(DIK_TAB))
	{
		//クラフトモードに移行
		g_opeMode = OPE_CRAFT;
		
		//カメラを主観に変更
		g_length = 0;
		g_cameraPos.y = 10;
		g_playMode = PLAY_FIRST;

		//マウス操作無効
		ShowCursor(TRUE);
		FixingMouse(false);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: craftOpe
//返り値	: 
//説明		: クラフト操作
//////////////////////////////////////////////////////////////////////////////
void	craftOpe	( void )
{
	if(GetKeyboardTrigger(DIK_TAB))
	{
		//ゲームモードに戻る
		g_opeMode = OPE_PLAY;

		//マウス操作有効
		ShowCursor(FALSE);
		FixingMouse(true);
	}
}