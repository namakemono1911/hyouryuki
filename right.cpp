/*****************************************************************************
���C�g�ݒ�
Aythor	: ���@��
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"right.h"

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initRight
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void	initRight	( void )
{
	//�ϐ��錾
	D3DLIGHT9			light[3] = {};
	D3DXVECTOR3			vecDir;
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//���C�e�B���O������
	light[0].Type = D3DLIGHT_DIRECTIONAL;
	light[0].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//���s�����̐F
	light[0].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//�������̐F
	light[0].Position = D3DXVECTOR3(0.f, 300.f, -30.f);					//���C�g�̈ʒu
	vecDir = D3DXVECTOR3(0.f, -300.f, 450.f);							//���s�����̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&light[0].Direction, &vecDir);		//�x�N�g���̐��K��

	light[1].Type = D3DLIGHT_DIRECTIONAL;
	light[1].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);				//���s�����̐F
	light[1].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//�������̐F
	light[1].Position = D3DXVECTOR3(0.f, -100.f, 30.f);					//���C�g�̈ʒu
	vecDir = D3DXVECTOR3(0.f, 100.f, -30.f);							//���s�����̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&light[1].Direction, &vecDir);		//�x�N�g���̐��K��
	
	light[2].Type = D3DLIGHT_DIRECTIONAL;
	light[2].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//���s�����̐F
	light[2].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);					//�������̐F
	light[2].Position = D3DXVECTOR3(0.f, 30.f, 300.f);					//���C�g�̈ʒu
	vecDir = D3DXVECTOR3(0.f, -30.f, -300.f);							//���s�����̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&light[2].Direction, &vecDir);		//�x�N�g���̐��K��

	pDevice->SetLight(0, &light[0]);									//�f�o�C�X��0�Ԗڂ̃��C�g��ݒ�
	pDevice->LightEnable(0, TRUE);										//0�ԃ��C�gON
	
	//pDevice->SetLight(1, &light[1]);									//�f�o�C�X��1�Ԗڂ̃��C�g��ݒ�
	//pDevice->LightEnable(1, TRUE);										//1�ԃ��C�gON
	
	//pDevice->SetLight(2, &light[2]);									//�f�o�C�X��1�Ԗڂ̃��C�g��ݒ�
	//pDevice->LightEnable(2, TRUE);										//1�ԃ��C�gON
}