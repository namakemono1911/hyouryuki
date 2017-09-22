/*****************************************************************************
�|���S���`��
Aythor	: ���@��
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"polygon.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME		("data/texture/kobeni0.png")	//�e�N�X�`���̃A�h���X

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
VERTEX3D g_aVertex[NUM_VERTEX];

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	makeVertexPolygon	( void );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
float				angle = 0;					//�p�x
D3DXVECTOR3			pos	= D3DXVECTOR3(0, 0, 0);	//���W
LPDIRECT3DTEXTURE9	g_pTexturePolygon = NULL;	//�e�N�X�`���C���^�[�t�F�C�X

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initPolygon
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void initPolygon ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//�|���S���̕`��ʒu�ݒ�
	makeVertexPolygon();

	//�e�N�X�`���̓ǂݍ���
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTexturePolygon)))
	{
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitPolygon
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitPolygon	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updatePolygon
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updatePolygon	( void )
{
	D3DXMATRIX	world[2];					//���[���h�s��
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	if(GetKeyboardPress(DIK_A) == true)
	{
		angle += 0.1f;
	}

	if(GetKeyboardPress(DIK_D) == true)
	{
		angle -= 0.1f;
	}

	if(GetKeyboardPress(DIK_LEFT))
	{
		pos.x -= 1.f;
	}

	if(GetKeyboardPress(DIK_RIGHT))
	{
		pos.x += 1.f;
	}

	if(GetKeyboardPress(DIK_UP))
	{
		pos.y += 1.f;
	}

	if(GetKeyboardPress(DIK_DOWN))
	{
		pos.y -= 1.f;
	}

	//���[���h�s��̐ݒ�
	D3DXMatrixIdentity(&world[0]);								//�s�񏉊���
	D3DXMatrixIdentity(&world[1]);

	D3DXMatrixRotationYawPitchRoll(&world[0], angle, 0.0f, 0.0f);	//��]�s��
	D3DXMatrixTranslation(&world[1], pos.x, pos.y, pos.z);			//���s�ړ�

	world[0] *= world[1];

	pDevice->SetTransform(D3DTS_WORLD, &world[0]);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawPolygon
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawPolygon ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0,g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitiveUP
		(
		D3DPT_TRIANGLESTRIP,	//�|���S���̎��
		NUM_POLYGON,			//�|���S���̐�
		&g_aVertex[0],			//���_�f�[�^�̐擪�A�h���X
		sizeof(VERTEX3D)		//���_�f�[�^1���̃T�C�Y
		);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexPolygon
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	makeVertexPolygon	( void )
{
	//���_���W�̐ݒ�(2D���W�E�E���)
	g_aVertex[0].pos = D3DXVECTOR3(-0.01f * 243, 0.01f * 432, 0.0f);
	g_aVertex[1].pos = D3DXVECTOR3(0.01f * 243, 0.01f * 432, 0.0f);
	g_aVertex[2].pos = D3DXVECTOR3(-0.01f * 243, -0.01f * 432, 0.0f);
	g_aVertex[3].pos = D3DXVECTOR3(0.01f * 243, -0.01f * 432, 0.0f);

	//�@���x�N�g����ݒ�
	g_aVertex[0].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[1].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[2].nor = D3DXVECTOR3(0.f, 0.f, -1.f);
	g_aVertex[3].nor = D3DXVECTOR3(0.f, 0.f, -1.f);

	//���_�J���[�̐ݒ�(0~155�̐����l)
	g_aVertex[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_aVertex[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�ݒ�
	g_aVertex[0].tex = D3DXVECTOR2(0.f, 0.f);
	g_aVertex[1].tex = D3DXVECTOR2(1.f, 0.f);
	g_aVertex[2].tex = D3DXVECTOR2(0.f, 1.f);
	g_aVertex[3].tex = D3DXVECTOR2(1.f, 1.f);
}