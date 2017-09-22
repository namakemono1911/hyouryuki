/*****************************************************************************
ライト設定
Aythor	: 上野　匠
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//ヘッダーファイルインクルード
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"right.h"

//////////////////////////////////////////////////////////////////////////////
//関数名	: initRight
//返り値	: 
//説明		: ポリゴン初期化
//////////////////////////////////////////////////////////////////////////////
void	initRight	( void )
{
	//変数宣言
	D3DLIGHT9			light[3] = {};
	D3DXVECTOR3			vecDir;
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//ライティング初期化
	light[0].Type = D3DLIGHT_DIRECTIONAL;
	light[0].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//平行光源の色
	light[0].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//環境光源の色
	light[0].Position = D3DXVECTOR3(0.f, 300.f, -30.f);					//ライトの位置
	vecDir = D3DXVECTOR3(0.f, -300.f, 450.f);							//平行光源の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&light[0].Direction, &vecDir);		//ベクトルの正規化

	light[1].Type = D3DLIGHT_DIRECTIONAL;
	light[1].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);				//平行光源の色
	light[1].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//環境光源の色
	light[1].Position = D3DXVECTOR3(0.f, -100.f, 30.f);					//ライトの位置
	vecDir = D3DXVECTOR3(0.f, 100.f, -30.f);							//平行光源の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&light[1].Direction, &vecDir);		//ベクトルの正規化
	
	light[2].Type = D3DLIGHT_DIRECTIONAL;
	light[2].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//平行光源の色
	light[2].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//環境光源の色
	light[2].Position = D3DXVECTOR3(0.f, 30.f, 300.f);					//ライトの位置
	vecDir = D3DXVECTOR3(0.f, -30.f, -300.f);							//平行光源の向き
	D3DXVec3Normalize((D3DXVECTOR3*)&light[2].Direction, &vecDir);		//ベクトルの正規化

	pDevice->SetLight(0, &light[0]);									//デバイスに0番目のライトを設定
	pDevice->LightEnable(0, TRUE);										//0番ライトON
	
	//pDevice->SetLight(1, &light[1]);									//デバイスに1番目のライトを設定
	//pDevice->LightEnable(1, TRUE);										//1番ライトON
	
	//pDevice->SetLight(2, &light[2]);									//デバイスに1番目のライトを設定
	//pDevice->LightEnable(2, TRUE);										//1番ライトON
}