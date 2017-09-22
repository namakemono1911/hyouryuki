/*****************************************************************************
fade.cpp
Aythor	: ���@��
Data	: 2016_09_28
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "polygon.h"
#include "fade.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_FADE	(5)

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
	D3DXVECTOR3	pos;	//���W
	D3DXVECTOR3	size;	//�T�C�Y
	D3DXCOLOR	color;	//�F
	FADE_MODE	mode;	//�t�F�[�h���[�h
	float		time;	//�t�F�[�h�̎���
	int			id;		//�t�F�[�h��ID
	bool		use;	//�g�p�t���O
	bool		end;	//�I����Ă邩�ǂ���
}FADE;

FADE fade[MAX_FADE];

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexFade ( LPDIRECT3DDEVICE9 pDevice );
void	opeFade			( void );
void	updateFadeIn	( int num );
void	updateFadeOut	( int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;			//�e�N�X�`���C���^�[�t�F�C�X
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferFade = NULL;		//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X

int				g_fadeNum = 0;							//�g���Ă���t�F�[�h�̐�

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: initFade
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void	initFade	( void )
{	
	//�ϐ��錾
	int		i;		//loop
	LPDIRECT3DDEVICE9 pDevice = getDevice();	//�f�o�C�X�|�C���^�擾

	//�\���̏�����
	for(i = 0; i < MAX_FADE; i++)
	{
		fade[i].use = false;
		fade[i].end	= false;
	}

	//�f�o�C�X�擾
	makeVertexFade(pDevice);
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: uninitFade
// �Ԃ�l	:
// ����		: �f�o�C�X�̏I��
//////////////////////////////////////////////////////////////////////////////
void uninitFade ( void )
{
	//�e�N�X�`���C���^�[�t�F�C�X�̉��
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//���_�o�b�t�@�Ǘ��̉��
	if(g_pVtxBufferFade != NULL)
	{
		g_pVtxBufferFade->Release();
		g_pVtxBufferFade = NULL;
	}

	g_fadeNum = 0;
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: updateFade
// �Ԃ�l	:
// ����		: �X�V
//////////////////////////////////////////////////////////////////////////////
void	updateFade	( void )
{
	//�ϐ��錾
	VERTEX2D	*pVtx;		
	int			i, j;		//loop

	//���[�h���Ə���
	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			switch(fade[i].mode)
			{
			case FADE_IN:
				{
					updateFadeIn(i);
					break;
				}

			case FADE_OUT:
				{
					updateFadeOut(i);
					break;
				}

			case FADE_MODE_NONE:
				{
					return;
				}
			}

			j++;
		}
	}

	//���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾
	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			//���_���W�̐ݒ�(2D���W�E�E���)
			pVtx[0].pos = D3DXVECTOR3(fade[i].pos.x, fade[i].pos.y, 0.f);
			pVtx[1].pos = D3DXVECTOR3(fade[i].pos.x + fade[i].size.x, fade[i].pos.y, 0.f);
			pVtx[2].pos = D3DXVECTOR3(fade[i].pos.x, fade[i].pos.y + fade[i].size.y, 0.f);
			pVtx[3].pos	= D3DXVECTOR3(fade[i].pos.x + fade[i].size.x, fade[i].pos.y + fade[i].size.y, 0.f);
		
			//���_�J���[�̐ݒ�(0~155�̐����l)
			pVtx[0].color =
			pVtx[1].color = 
			pVtx[2].color = 
			pVtx[3].color = 
			fade[j].color;

			pVtx += NUM_VERTEX;
			j++;
		}
	}
	//���̂�����A�[�����b�N��
	g_pVtxBufferFade->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: updateFadeIn
// �Ԃ�l	:
// ����		: �X�V
//////////////////////////////////////////////////////////////////////////////
void	updateFadeIn	( int num )
{
	//�ϐ��錾
	float	percent;	//�p�[�Z���e�[�W

	//����
	percent = 1.f / fade[num].time;

	//���ߓx���Z
	fade[num].color.a -= percent;

	if(fade[num].color.a <= 0.f)
	{
		fade[num].color.a = 0.f;
		fade[num].use = false;
		fade[num].end = true;
		g_fadeNum--;
	}
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: updateFadeOut
// �Ԃ�l	:
// ����		: �X�V
//////////////////////////////////////////////////////////////////////////////
void	updateFadeOut	( int num )
{
	//�ϐ��錾
	float	percent;	//�p�[�Z���e�[�W

	//����
	percent = 1.f / fade[num].time;

	//���ߓx���Z
	fade[num].color.a += percent;

	if(fade[num].color.a >= 1.f)
	{
		fade[num].color.a = 1.f;
		//fade[num].use = false;
		//g_fadeNum--;
		fade[num].end	= true;
	}
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: setFade
// �Ԃ�l	:
// ����		: �t�F�[�h�̐ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setFade	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR color, float time, FADE_MODE mode, int id )
{
	//�ϐ��錾
	int			i;		//loop

	//�ϐ����
	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].use == false)
		{
			fade[i].pos		=	pos;		//���W
			fade[i].size	=	size;		//�T�C�Y
			fade[i].color	=	color;		//���_�F
			fade[i].time	=	time;		//����
			fade[i].id		=	id;			//ID
			fade[i].mode	=	mode;		//�t�F�[�h���[�h
			fade[i].use		=	true;		//�g�p�t���O
			fade[i].end		=	false;		//�I���t���O

			g_fadeNum++;					//�������Z
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: drawFade
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void	drawFade	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	int		i, j;	//loop

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferFade, 0, sizeof(VERTEX2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,g_pTextureFade);

	for(i = 0, j = 0; j < g_fadeNum; i++)
	{
		if(fade[i].use == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive
				(
				D3DPT_TRIANGLESTRIP,	//�|���S���̎��
				0,						//�I�t�Z�b�g(���_��)
				NUM_POLYGON				//�|���S���̐�
				);

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexFade
//�Ԃ�l	: 
//����		: ���_�쐬
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexFade ( LPDIRECT3DDEVICE9 pDevice )
{
	//�ϐ��錾
	VERTEX2D	*pVtx = NULL;;		//���z�A�h���X�p�|�C���^
	int			i;

	//���_�o�b�t�@�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX2D) * NUM_VERTEX * MAX_FADE,	//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,				//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_2D,					//
			D3DPOOL_MANAGED,				//�������Ǘ����@
			&g_pVtxBufferFade,				//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);
	for(i = 0; i < MAX_FADE; i++)
	{
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;
		pVtx += NUM_VERTEX;
	}
	g_pVtxBufferFade->Unlock();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: getFadeEnd
// �Ԃ�l	:
// ����		: �I����Ă邩�ǂ����擾
//////////////////////////////////////////////////////////////////////////////
bool	getFadeEnd	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].id == id && fade[i].end == true)
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: releaseFade
// �Ԃ�l	:
// ����		: �t�F�[�h���
//////////////////////////////////////////////////////////////////////////////
void	releaseFade	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FADE; i++)
	{
		if(fade[i].id == id)
		{
			fade[i].use = false;
			fade[i].end = false;
		}
	}
}