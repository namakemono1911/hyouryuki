/*****************************************************************************
パーティクル
Aythor	: 上野　匠
Data	: 2016_11_22
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	<d3dx9math.h>
#include	"render.h"
#include	"input.h"
#include	"camera.h"
#include	"particle.h"

//////////////////////////////////////////////////////////////////////////////
//定数定義
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME			("data/texture/effect003.jpg")	//テクスチャのアドレス
#define	MAX_PARTICLE		(2000)			//最大パーティクル数
#define	MAX_PARTICLE_PARAM	(5)				//最大パーティクルパラメータ数

#define	WIDTH				(80.f)
#define	HEIGHT				(80.f)

//////////////////////////////////////////////////////////////////////////////
//構造体定義
//////////////////////////////////////////////////////////////////////////////
typedef	struct
{
	D3DXVECTOR3		pos;		//座標
	D3DXVECTOR3		vec;		//方向ベクトル
	D3DXCOLOR		color;		//頂点色
	float			angle;		//対角の角度
	float			move;		//移動速度
	float			len;		//対角線
	float			life;		//寿命
	bool			use;		//trueで使われている
}PARTICLE;

typedef	struct
{
	D3DXVECTOR3		source;		//発生源
	D3DXVECTOR3		size;		//画像サイズ
	D3DXVECTOR3		vecEx;		//方向ベクトルの範囲
	D3DXVECTOR3		rot;		//発射角
	D3DXCOLOR		colorMax;	//頂点色の最大値
	D3DXCOLOR		colorMin;	//頂点色の最低値
	float			moveMax;	//移動速度の最大値
	float			moveMin;	//移動速度の最低値
	float			lenMax;		//対角線の最大値
	float			lenMin;		//対角線の最低値
	float			lifeMax;	//寿命の最大値
	float			lifeMin;	//寿命の最低値
	int				value;		//1フレーム辺りの発生量
	int				time;		//何フレーム出すか
	bool			use;		//使用フラグ
}PARTICLE_PARAM;

typedef	enum
{
	OPE_SOURCE = 0,				//発生源操作
	OPE_VEC,					//方向ベクトル操作
	OPE_ROT,					//射角操作
	OPE_COLOR,					//頂点色操作
	OPE_MOVE,					//加速度操作
	OPE_LEN,					//半径操作
	OPE_LIFE,					//寿命操作
	OPE_VALUE,					//発射数操作
	OPE_NULL					//null
}PARTICLE_OPE;

//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	initStruct				( void );
HRESULT	makeVertexParticle		( LPDIRECT3DDEVICE9 pDevice );
HRESULT	makeIndexBufferParticle	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixParticle		( LPDIRECT3DDEVICE9 pDevice, int num );
void	setParticle				( int numParticle, int numParam );
void	opeParticle				( void );

//////////////////////////////////////////////////////////////////////////////
//グローバル変数
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureParticle		=	NULL;	//テクスチャインターフェイス
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferParticle	=	NULL;	//頂点バッファ

PARTICLE			particle[MAX_PARTICLE];					//パーティクル情報
PARTICLE_PARAM		param[MAX_PARTICLE_PARAM];				//パーティクルのパラメータ
PARTICLE_OPE		particleOpe;							//パーティクルの操作方法

int					g_numParticle	=	0;						//パーティクル数
int					g_numParticleParam	=	0;				//パーティクルパラメータ数
int					opeMode		=	0;						//操作モード

//////////////////////////////////////////////////////////////////////////////
//関数名	: initParticle
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void initParticle ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//構造体初期化
	initStruct();

	//パーティクルセット
	/*
	setParticleAll(
		D3DXVECTOR3(-30.f, 5.f, 0.f),
		D3DXVECTOR3(WIDTH, HEIGHT, 0.f),
		D3DXVECTOR3(360, 360, 360),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXCOLOR(1.f, 0.f, 0.5f, 1.f),
		D3DXCOLOR(0.5f, 0.0f, 0.0f, 0.1f),
		10.f,
		1.f,
		1.f,
		0.5f,
		1.f, 
		0.5f,
		10,
		-1);*/
	/*
	//パーティクル2セット
	setParticleAll(
		D3DXVECTOR3(0.f, 5.f, 0.f),
		D3DXVECTOR3(WIDTH, HEIGHT, 0.f),
		D3DXVECTOR3(360, 0, 360),
		D3DXVECTOR3(0.f, 0.0f, 0.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 0.1f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f),
		5.f,
		2.f,
		0.05f,
		0.01f,
		0.3f, 
		0.01f,
		3,
		-1);*/

	//ポリゴンの描画位置設定
	makeVertexParticle(pDevice);

	//テクスチャの読み込み
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureParticle)))
	{
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: initStruct
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void	initStruct	( void )
{
	//変数宣言
	int		i;	//loop

	//=============================
	//構造体初期化
	//=============================
	for(i = 0; i < MAX_PARTICLE; i++)
	{
		//座標
		particle[i].pos		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//ベクトル
		particle[i].vec		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//頂点色
		particle[i].color	=	D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

		//対角線
		particle[i].len		=	0.f;

		//加速度
		particle[i].move	=	0.f;

		//寿命
		particle[i].life	=	0.f;

		//対角の角度
		particle[i].angle	=	0.f;

		//使用フラグ
		particle[i].use		=	false;
	}

	//=============================
	// パラメータ初期化
	//=============================
	for(i = 0; i < MAX_PARTICLE_PARAM; i++)
	{
		//発生源
		param[i].source	=	D3DXVECTOR3(0.f, 5.f, 0.f);

		//ベクトルの範囲
		param[i].vecEx		=	D3DXVECTOR3(
			360,
			360,
			360);

		//射角
		param[i].rot		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//頂点色の最大値
		param[i].colorMax	=	D3DXCOLOR(1.0f, 1.0f,  1.0f, 1.0f);

		//頂点色の最低値
		param[i].colorMin	=	D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);

		//大きさの最大値
		param[i].lenMax		=	5.f;

		//大きさの最低値
		param[i].lenMin		=	1.f;

		//寿命の最大値
		param[i].lifeMax	=	1.0f;

		//寿命の最低値
		param[i].lifeMin	=	0.5f;

		//移動速度の最大値
		param[i].moveMax	=	0.5f;

		//移動速度の最低値
		param[i].moveMin	=	0.01f;

		//発生量
		param[i].value		=	10;

		//使用フラグ
		param[i].use		=	false;
	}
	
	//=============================
	// パラメータ初期化
	//=============================
	particleOpe	=	OPE_SOURCE;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setParticleAll
//返り値	: 
//説明		: パーティクルのパラメータすべて設定
//////////////////////////////////////////////////////////////////////////////
int	setParticleAll	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 vec, D3DXVECTOR3 rot, D3DXCOLOR colMax, D3DXCOLOR colMin, float lenMax, float lenMin, float lifeMax, float lifeMin, float moveMax, float moveMin, int value, int time )
{
	//変数宣言
	int	i;		//loop

	for(i = 0; i < MAX_PARTICLE_PARAM; i++)
	{
		//未使用のパラメータ探索
		if(param[i].use	==	false)
		{
			//=============================
			// それぞれ範囲設定
			//=============================
			param[i].source		=	pos;		//発生源
			param[i].size		=	size;		//画像のサイズ
			param[i].vecEx		=	vec;		//方向ベクトル
			param[i].rot		=	rot;		//角度
			param[i].colorMax	=	colMax;		//最高頂点色
			param[i].colorMin	=	colMin;		//最低頂点色
			param[i].lenMax		=	lenMax;		//最大半径
			param[i].lenMin		=	lenMin;		//最低半径
			param[i].lifeMax	=	lifeMax;	//最大寿命
			param[i].lifeMin	=	lifeMin;	//最低寿命
			param[i].moveMax	=	moveMax;	//最高速度
			param[i].moveMin	=	moveMin;	//最低速度
			param[i].value		=	value;		//1フレーム何発か
			param[i].time		=	time;		//時間
			param[i].use		=	true;		//使用フラグ

			g_numParticleParam++;					//パラメータ総数加算

			//設定した配列番号返却
			return i;
		}
	}

	//全て使われていた
	return	-1;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setParticlePos
//返り値	: 
//説明		: パーティクルの発生源設定
//////////////////////////////////////////////////////////////////////////////
void	setParticlePos	( D3DXVECTOR3 pos, int num )
{
	if(param[num].use	==	true)
	{
		param[num].source	=	pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: uninitParticle
//返り値	: 
//説明		: ポリゴン終了
//////////////////////////////////////////////////////////////////////////////
void	uninitParticle	( void )
{
	//頂点バッファ解放
	if(g_pVtxBufferParticle	!=	NULL)
	{
		g_pVtxBufferParticle->Release();
		g_pVtxBufferParticle	=	NULL;
	}

	//テクスチャバッファrelease
	if(g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	g_numParticle		=	0;
	g_numParticleParam	=	0;
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: releaseParticle
//返り値	: 
//説明		: パーティクルのリリース
//////////////////////////////////////////////////////////////////////////////
void	releaseParticle	(int num )
{
	if(param[num].use	==	true)
	{
		param[num].use	=	false;
		g_numParticleParam--;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: updateParticle
//返り値	: 
//説明		: ポリゴン更新
//////////////////////////////////////////////////////////////////////////////
void	updateParticle	( void )
{
	//変数宣言
	VERTEX3D	*pVtx;	//頂点情報
	int		i, j, k, n;		//loop
	int		work;			//ワーク

	//=============================
	// キー操作
	//=============================
	//opeParticle();

	//=============================
	//パーティクル生成
	//=============================
	//使用されているパラメータ探索
	for(i = 0, j = 0; j < g_numParticleParam && i < MAX_PARTICLE_PARAM; i++)
	{
		if(param[i].use	==	true)
		{
			//時間確認
			if(param[i].time	>=	0)
			{
				param[i].time--;
				if(param[i].time	<=	0)
				{
					releaseParticle(i);
				}
			}

			//使用されていないパーティクル探索
			for(k = 0, n = 0; n < param[i].value && k < MAX_PARTICLE; k++)
			{
				if(k == MAX_PARTICLE - 1)
				{
					break;
				}

				if(particle[k].use	==	false)
				{
					//乱数セット
					setParticle(k, i);

					n++;
				}
			}

			j++;
		}
	}

	//=============================
	//設定更新
	//=============================
	work	=	g_numParticle;
	for(i = 0, j = 0; j < work && i < MAX_PARTICLE; i++)
	{
		//使用されていたら更新
		if(particle[i].use	==	true)
		{
			//座標
			particle[i].pos	=	D3DXVECTOR3(
				particle[i].move * particle[i].vec.x + particle[i].pos.x,
				particle[i].move * particle[i].vec.y + particle[i].pos.y,
				particle[i].move * particle[i].vec.z + particle[i].pos.z);

			//頂点色
			particle[i].color.a	-=	particle[i].life;

			if(particle[i].color.a <= 0)
			{
				particle[i].color.a	=	0.f;
				particle[i].use		=	false;
				g_numParticle--;
			}

			j++;
		}
	}

	//=============================
	//頂点バッファをロックして仮想アドレスを取得
	//=============================
	g_pVtxBufferParticle->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numParticle && i < MAX_PARTICLE; i++)
	{
		if(particle[i].use	==	true)
		{
			//座標設定
			pVtx[0].pos	=	D3DXVECTOR3(
				particle[i].len * -cos(particle[i].angle),
				particle[i].len * -sin(particle[i].angle),
				0.f);

			pVtx[1].pos	=	D3DXVECTOR3(
				particle[i].len * -cos(particle[i].angle),
				particle[i].len * sin(particle[i].angle),
				0.f);

			pVtx[2].pos	=	D3DXVECTOR3(
				particle[i].len * cos(particle[i].angle),
				particle[i].len * -sin(particle[i].angle),
				0.f);

			pVtx[3].pos	=	D3DXVECTOR3(
				particle[i].len * cos(particle[i].angle),
				particle[i].len * sin(particle[i].angle),
				0.f);

			//頂点色
			pVtx[0].color	=	particle[i].color;
			pVtx[1].color	=	particle[i].color;
			pVtx[2].color	=	particle[i].color;
			pVtx[3].color	=	particle[i].color;

			//テクスチャ座標
			pVtx[0].tex		=	D3DXVECTOR2(0.f, 1.f);
			pVtx[1].tex		=	D3DXVECTOR2(0.f, 0.f);
			pVtx[2].tex		=	D3DXVECTOR2(1.f, 1.f);
			pVtx[3].tex		=	D3DXVECTOR2(1.f, 0.f);

			//ポインタ移動
			pVtx	+=	NUM_VERTEX;
			j++;
		}
	}

	//私のこころアーンロック♪
	g_pVtxBufferParticle->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setParticle
//返り値	: 
//説明		: パーティクル設定
//////////////////////////////////////////////////////////////////////////////
void	setParticle	( int numParticle, int numParam )
{
	//変数宣言
	float	colorWork;		//頂点色のワーク
	float	colorMax;		//最大頂点色のワーク
	float	colorMin;		//最低頂点色のワーク
	float	moveMax;		//最大加速度のワーク
	float	moveMin;		//最低加速度のワーク
	float	lenMax;			//最大半径のワーク
	float	lenMin;			//最低半径のワーク
	float	lifeMax;		//最大寿命のワーク
	float	lifeMin;		//最低寿命のワーク
	float	angle[3];		//角度のワーク

	//総数加算
	g_numParticle++;

	//使用フラグ
	particle[numParticle].use	=	true;

	//対角の角度
	particle[numParticle].angle	=	atan2(param[numParam].size.y, param[numParam].size.x);

	//座標
	particle[numParticle].pos	=	param[numParam].source;

	//方向ベクトル
	if(param[numParam].vecEx.x	>	0)
	{
		angle[0]	=	(float)(rand() % (int)param[numParam].vecEx.x);
		angle[0]	=	angle[0] * PI / 180;
	}
	else
	{
		angle[0]	=	0.f;
	}

	if(param[numParam].vecEx.y	>	0)
	{
		angle[1]	=	(float)(rand() % (int)param[numParam].vecEx.y);
		angle[1]	=	angle[1] * PI / 180;
	}
	else
	{
		angle[1]	=	0.f;
	}

	if(param[numParam].vecEx.z	>	0)
	{
		angle[2]	=	(float)(rand() % (int)param[numParam].vecEx.z);
		angle[2]	=	angle[2] * PI / 180;
	}
	else
	{
		angle[2]	=	0.f;
	}

	particle[numParticle].vec	=	D3DXVECTOR3(
		sin(angle[0]) + param[numParam].rot.x,
		sin(angle[1]) + param[numParam].rot.y,
		sin(angle[2]) + param[numParam].rot.z);

	//加速度
	moveMax	=	param[numParam].moveMax * 100.f;
	moveMin	=	param[numParam].moveMin * 100.f;
	if(moveMax - moveMin	>	0)
	{
		particle[numParticle].move	=	(float)(rand() % (int)(moveMax - moveMin) + moveMin);
		particle[numParticle].move	*=	0.01f;
	}
	else
	{
		particle[numParticle].move	=	param[numParam].moveMax;
	}

	//対角線
	lenMax	=	param[numParam].lenMax * 100.f;
	lenMin	=	param[numParam].lenMin * 100.f;
	if(lenMax - lenMin	>	0)
	{
		particle[numParticle].len	=	(float)(rand() % (int)(lenMax - lenMin) + lenMin);
		particle[numParticle].len	*=	0.01f;
	}
	else
	{
		particle[numParticle].len	=	param[numParam].lenMax;
	}

	//頂点色
	colorMax	=	param[numParam].colorMax.r * 100.f;
	colorMin	=	param[numParam].colorMin.r * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.r;
	}
	particle[numParticle].color.r	=	colorWork;

	colorMax	=	param[numParam].colorMax.g * 100.f;
	colorMin	=	param[numParam].colorMin.g * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.g;
	}
	particle[numParticle].color.g	=	colorWork;
	
	colorMax	=	param[numParam].colorMax.b * 100.f;
	colorMin	=	param[numParam].colorMin.b * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.b;
	}
	particle[numParticle].color.b	=	colorWork;
	
	colorMax	=	param[numParam].colorMax.a * 100.f;
	colorMin	=	param[numParam].colorMin.a * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.a;
	}
	particle[numParticle].color.a	=	colorWork;

	//寿命
	lifeMax	=	param[numParam].lifeMax * 1000.f;
	lifeMin	=	param[numParam].lifeMin * 1000.f;
	if(lifeMax - lifeMin	> 0)
	{
		particle[numParticle].life	=	(float)(rand() % (int)(lifeMax - lifeMin) + lifeMin);
		particle[numParticle].life	*=	0.00001f;
	}
	else
	{
		particle[numParticle].life	=	param[numParam].lifeMax;
	}
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: drawParticle
//返り値	: 
//説明		: ポリゴン描写
//////////////////////////////////////////////////////////////////////////////
void drawParticle ( void )
{
	//変数宣言
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop

	//ライトの影響off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//加算合成に切り替え
	addDevice();

	//zテスト有効
	pDevice->SetRenderState(D3DRS_ZENABLE, false);		//zバッファのON・OFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);	//バッファへの書き込みON・OFF

	pDevice->SetFVF(FVF_VERTEX_3D);

	//ストリーム作成
	pDevice->SetStreamSource(0, g_pVtxBufferParticle, 0, sizeof(VERTEX3D));

	for(i = 0, j = 0; j < g_numParticle; i++)
	{
		if(particle[i].use	==	true)
		{
		//ワールドマトリックス設定
		setMatrixParticle(pDevice, i);

		//テクスチャセット
		pDevice->SetTexture(0,g_pTextureParticle);

		//ポリゴンの描画
		pDevice->DrawPrimitive
			(
			D3DPT_TRIANGLESTRIP,			//ポリゴンの種類
			NUM_VERTEX * j,					//オフセット(頂点数)
			NUM_POLYGON						//ポリゴンの数
			);

		j++;
		}
	}

	//ライトの影響on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//乗算合成に切り替え
	multiDevice();

	//zテスト無効
	pDevice->SetRenderState(D3DRS_ZENABLE, true);		//zバッファのON・OFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);	//バッファへの書き込みON・OFF
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: setMatrix
//返り値	: 
//説明		: マトリックス設定
//////////////////////////////////////////////////////////////////////////////
void	setMatrixParticle	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//変数宣言
	D3DXMATRIX	world;			//ワールド行列
	D3DXMATRIX	move;			//移動行列
	D3DXMATRIX	ViewInverse;	//カメラの逆行列

	//逆行列作成
	D3DXMatrixTranspose(&ViewInverse, &getViewMtx());

	ViewInverse._14	=	0.0f;
	ViewInverse._24	=	0.0f;
	ViewInverse._34	=	0.0f;

	//ワールド行列の設定
	//行列初期化
	D3DXMatrixIdentity(&world);

	//平行移動
	D3DXMatrixTranslation(&move,	
		particle[num].pos.x,
		particle[num].pos.y,
		particle[num].pos.z);

	//行列合成
	world	*=	ViewInverse;
	world	*=	move;

	pDevice->SetTransform(D3DTS_WORLD, &world);		//行列をデバイスに設定
}

//////////////////////////////////////////////////////////////////////////////
//関数名	: makeVertexParticle
//返り値	: 
//説明		: ポリゴン描画位置設定
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexParticle	( LPDIRECT3DDEVICE9 pDevice )
{
	//頂点バッファの設定
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * MAX_PARTICLE * MAX_PARTICLE_PARAM * NUM_VERTEX,	//頂点バッファサイズ
			D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
			FVF_VERTEX_3D,							//なんか設定
			D3DPOOL_MANAGED,						//メモリ管理方法
			&g_pVtxBufferParticle,					//頂点バッファ管理インターフェイス
			NULL
			)
		))
	{
		return E_FAIL;
	}

	return	S_OK;
}