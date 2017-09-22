/*****************************************************************************
�^�C�g��
Aythor	: ���@��
Data	: 2017_02_15
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
#define TEX_NAME	"data/texture/title.png"

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexTitle ( LPDIRECT3DDEVICE9 pDevice );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureTitle = NULL;			//�e�N�X�`���C���^�[�t�F�C�X
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferTitle = NULL;		//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	initTitle
//�Ԃ�l	:	
//����		:	�^�C�g���ɕK�v�ȃt�@�C���̏�����
//////////////////////////////////////////////////////////////////////////////
HRESULT	initTitle	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();	//�f�o�C�X�|�C���^�擾
	
	//�e�N�X�`���ǂݍ���
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureTitle)))
	{
		MessageBox(NULL, "�G���[", "title�e�N�X�`���G���[", MB_OK);
	}

	//�f�o�C�X�擾
	makeVertexTitle(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	uninitTitle
//�Ԃ�l	:	
//����		:	�^�C�g���ɕK�v�ȃt�@�C���̏I��
//////////////////////////////////////////////////////////////////////////////
void	uninitTitle	( void )
{

	//�e�N�X�`���C���^�[�t�F�C�X�̉��
	if(g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//���_�o�b�t�@�Ǘ��̉��
	if(g_pVtxBufferTitle != NULL)
	{
		g_pVtxBufferTitle->Release();
		g_pVtxBufferTitle = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateTitle
//�Ԃ�l	:	
//����		:	�^�C�g���ɕK�v�ȃt�@�C���̍X�V
//////////////////////////////////////////////////////////////////////////////
void	updateTitle	( void )
{
	if(GetKeyboardTrigger(DIK_RETURN) || GetMouseTrigger(MOUSE_LEFT))
	{
		setFade(D3DXVECTOR3(0.f, 0.f, 0.f),
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f),
			D3DXCOLOR(1.f, 1.f, 1.f, 0.f),
			100,
			FADE_OUT,
			0);
	}
	
	if(getFadeEnd(0) == true)
	{
		releaseFade(0);
		setMode(GAME_MODE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	drawTitle
//�Ԃ�l	:	
//����		:	�^�C�g���ɕK�v�ȃt�@�C���̕`��
//////////////////////////////////////////////////////////////////////////////
void	drawTitle	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	int		i, j;	//loop

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferTitle, 0, sizeof(VERTEX2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,g_pTextureTitle);

	//�|���S���̕`��
	pDevice->DrawPrimitive
		(
		D3DPT_TRIANGLESTRIP,	//�|���S���̎��
		0,						//�I�t�Z�b�g(���_��)
		NUM_POLYGON				//�|���S���̐�
		);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexTitle
//�Ԃ�l	: 
//����		: ���_�쐬
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexTitle ( LPDIRECT3DDEVICE9 pDevice )
{
	//�ϐ��錾
	VERTEX2D	*pVtx = NULL;;		//���z�A�h���X�p�|�C���^
	int			i;

	//���_�o�b�t�@�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX2D) * NUM_VERTEX,	//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,				//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_2D,					//
			D3DPOOL_MANAGED,				//�������Ǘ����@
			&g_pVtxBufferTitle,				//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	g_pVtxBufferTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos	=	D3DXVECTOR3(0.f, 0.f, 0.f);
	pVtx[1].pos	=	D3DXVECTOR3(SCREEN_WIDTH, 0.f, 0.f);
	pVtx[2].pos	=	D3DXVECTOR3(0.f, SCREEN_HEIGHT, 0.f);
	pVtx[3].pos	=	D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);
	
	pVtx[0].tex	=	D3DXVECTOR2(0.f, 0.f);
	pVtx[1].tex	=	D3DXVECTOR2(1.f, 0.f);
	pVtx[2].tex	=	D3DXVECTOR2(0.f, 1.f);
	pVtx[3].tex	=	D3DXVECTOR2(1.f, 1.f);

	//���_�J���[�̐ݒ�(0~155�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx += NUM_VERTEX;

	g_pVtxBufferTitle->Unlock();

	return S_OK;
}