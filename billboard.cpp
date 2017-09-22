/*****************************************************************************
�r���{�[�h
Aythor	: ���@��
Data	: 2016_11_22
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	<d3dx9math.h>
#include	"input.h"
#include	"camera.h"
#include	"billboard.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME		("data/texture/kobeni0.png")	//�e�N�X�`���̃A�h���X

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
VERTEX3D g_aVertexBillboard[NUM_VERTEX];

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	makeVertexBillboard	( void );
void	setMatrix			( LPDIRECT3DDEVICE9 pDevice );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
float				angleBillboard	= 0;					//�p�x
D3DXVECTOR3			posBillboard	= D3DXVECTOR3(0, 0, 0);	//���W
LPDIRECT3DTEXTURE9	g_pTextureBillboard = NULL;				//�e�N�X�`���C���^�[�t�F�C�X

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initBillboard
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void initBillboard ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//�|���S���̕`��ʒu�ݒ�
	makeVertexBillboard();

	//�e�N�X�`���̓ǂݍ���
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureBillboard)))
	{
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitBillboard
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitBillboard	( void )
{
	//�e�N�X�`���o�b�t�@release
	if(g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateBillboard
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateBillboard	( void )
{
	
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawBillboard
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawBillboard ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	
	//�}�g���b�N�X�ݒ�
	setMatrix(pDevice);

	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//���e�X�g�L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	
	//���e�X�g�̏�����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 192);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0,g_pTextureBillboard);

	//�|���S���̕`��
	pDevice->DrawPrimitiveUP
		(
		D3DPT_TRIANGLESTRIP,	//�|���S���̎��
		NUM_POLYGON,			//�|���S���̐�
		&g_aVertexBillboard[0],	//���_�f�[�^�̐擪�A�h���X
		sizeof(VERTEX3D)		//���_�f�[�^1���̃T�C�Y
		);

	//���C�g�̉e��on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//���e�X�g����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrix
//�Ԃ�l	: 
//����		: �}�g���b�N�X�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrix	( LPDIRECT3DDEVICE9 pDevice )
{
	//�ϐ��錾
	D3DXMATRIX	world[2];		//���[���h�s��
	D3DXMATRIX	ViewInverse;	//�J�����̋t�s��			

	/*if(GetKeyboardPress(DIK_A) == true)
	{
		angleBillboard += 0.1f;
	}

	if(GetKeyboardPress(DIK_D) == true)
	{
		angleBillboard -= 0.1f;
	}

	if(GetKeyboardPress(DIK_LEFT))
	{
		posBillboard.x -= 1.f;
	}

	if(GetKeyboardPress(DIK_RIGHT))
	{
		posBillboard.x += 1.f;
	}

	if(GetKeyboardPress(DIK_UP))
	{
		posBillboard.y += 1.f;
	}

	if(GetKeyboardPress(DIK_DOWN))
	{
		posBillboard.y -= 1.f;
	}*/

	//�t�s��쐬
	D3DXMatrixInverse(&ViewInverse, NULL, &getViewMtx());

	ViewInverse._41	=	0.0f;
	ViewInverse._42	=	0.0f;
	ViewInverse._43	=	0.0f;

	//���[���h�s��̐ݒ�
	D3DXMatrixIdentity(&world[0]);								//�s�񏉊���
	D3DXMatrixIdentity(&world[1]);

	D3DXMatrixRotationYawPitchRoll(&world[0], 0.0f, 0.0f, angleBillboard);	//��]�s��
	D3DXMatrixTranslation(&world[1], posBillboard.x, posBillboard.y, posBillboard.z);			//���s�ړ�

	//�s�񍇐�
	world[0]	*=	world[1];
	world[0]	*=	ViewInverse;

	pDevice->SetTransform(D3DTS_WORLD, &world[0]);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexBillboard
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	makeVertexBillboard	( void )
{
	//���_���W�̐ݒ�(2D���W�E�E���)
	g_aVertexBillboard[0].pos = D3DXVECTOR3(-0.01f * 243, 0.02f * 432, 0.0f);
	g_aVertexBillboard[1].pos = D3DXVECTOR3(0.01f * 243, 0.02f * 432, 0.0f);
	g_aVertexBillboard[2].pos = D3DXVECTOR3(-0.01f * 243, 0.0f, 0.0f);
	g_aVertexBillboard[3].pos = D3DXVECTOR3(0.01f * 243, 0.0f * 432, 0.0f);

	//�@���x�N�g����ݒ�
	g_aVertexBillboard[0].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[1].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[2].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertexBillboard[3].nor = D3DXVECTOR3(0.f, 0.f, -1.f);

	//���_�J���[�̐ݒ�(0~155�̐����l)
	g_aVertexBillboard[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertexBillboard[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�ݒ�
	g_aVertexBillboard[0].tex = D3DXVECTOR2(0.f, 0.f);
	g_aVertexBillboard[1].tex = D3DXVECTOR2(1.f, 0.f);
	g_aVertexBillboard[2].tex = D3DXVECTOR2(0.f, 1.f);
	g_aVertexBillboard[3].tex = D3DXVECTOR2(1.f, 1.f);
}