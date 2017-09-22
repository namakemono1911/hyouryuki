/*****************************************************************************
�����蔻�菈��[hit.cpp]
Aythor	: ���@��
Data	: 2017_02_14
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "hit.h"
#include "vector.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexHit ( LPDIRECT3DDEVICE9 pDevice );
void	setMatrixHit	( LPDIRECT3DDEVICE9 pDev, int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitBox = NULL;		//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferHitSquare = NULL;	//
LPDIRECT3DTEXTURE9		g_pTexBufferHitBall[HIT_BALL_TEX_MAX] = {NULL};		//���̃e�N�X�`��

HIT_SQUARE	g_hitSquare[MAX_HIT_SQUARE];	//�q�b�g�ʍ\����	
HIT_BOX		g_hitBox[MAX_HIT_BOX];			//�q�b�g�{�b�N�X�\����
HIT_BALL	g_hitBall[MAX_HIT_BALL];		//�q�b�g�{�[���\����
int			g_numHit;						//�����蔻��̑���
char		*hitBallTex[HIT_BALL_TEX_MAX] =
{
	"data/texture/ball000.png",
	"data/texture/ball001.png"
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initHit
//�Ԃ�l	: 
//����		: ������
//////////////////////////////////////////////////////////////////////////////
void	initHit	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;		//�f�o�C�X���
	int		i;		//loop

	pDevice = getDevice();

	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		g_hitSquare[i].use = false;
	}
	
	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		g_hitBall[i].use = false;
	}
	
	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		g_hitSquare[i].use = false;
	}

	//�e�N�X�`���̓ǂݍ���
	for(i = 0; i < HIT_BALL_TEX_MAX; i++)
	{
		if(FAILED(D3DXCreateTextureFromFile(pDevice, hitBallTex[i], &g_pTexBufferHitBall[i])))
			{
				MessageBox(NULL, "�G���[", "bird�e�N�X�`���G���[", MB_OK);
			}
	}

	makeVertexHit(pDevice);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitHit
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitHit	( void )
{
	//�ϐ��錾
	int		i;		//loop

	if(g_pVtxBufferHitBox != NULL)
	{
		g_pVtxBufferHitBox->Release();
		g_pVtxBufferHitBox = NULL;
	}

	if(g_pVtxBufferHitSquare != NULL)
	{
		g_pVtxBufferHitSquare->Release();
		g_pVtxBufferHitSquare = NULL;
	}

	for(i = 0; i < HIT_BALL_TEX_MAX; i++)
	{
		if(g_pTexBufferHitBall[i] != NULL)
		{
			g_pTexBufferHitBall[i]->Release();
			g_pTexBufferHitBall[i] = NULL;
		}
	}

	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].mesh != NULL)
		{
			g_hitBall[i].mesh->Release();
			g_hitBall[i].mesh = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateHit
//�Ԃ�l	: 
//����		: �X�V
//////////////////////////////////////////////////////////////////////////////
void	updateHit	( void )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;
	int			i;		//loop

	g_pVtxBufferHitBox->Lock(0, 0, (void**)&pVtx, 0);
	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		pVtx[i * 22 + 0].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 1].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 2].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.b_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.b_l.z);
		pVtx[i * 22 + 3].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);

		pVtx[i * 22 + 4].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_l.z);
		pVtx[i * 22 + 5].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_r.z);

		pVtx[i * 22 + 6].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_l.z);
		pVtx[i * 22 + 7].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);

		pVtx[i * 22 + 8].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 9].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);
		pVtx[i * 22 + 10].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 11].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);
		pVtx[i * 22 + 12].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_r.z);
		pVtx[i * 22 + 13].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].top.t_r.z);

		pVtx[i * 22 + 14].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_r.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_r.z);
		pVtx[i * 22 + 15].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);
		pVtx[i * 22 + 16].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_r.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_r.y, 
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_r.z);

		pVtx[i * 22 + 17].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 18].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.t_l.z);
		pVtx[i * 22 + 19].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].top.b_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].top.b_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].top.b_l.z);
		pVtx[i * 22 + 20].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.t_l.x, 
			g_hitBox[i].pos.y + g_hitBox[i].bottom.t_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.t_l.z);
		pVtx[i * 22 + 21].pos		=	D3DXVECTOR3(
			g_hitBox[i].pos.x + g_hitBox[i].bottom.b_l.x,
			g_hitBox[i].pos.y + g_hitBox[i].bottom.b_l.y,
			g_hitBox[i].pos.z + g_hitBox[i].bottom.b_l.z);
			
		pVtx[i * 22 + 0].color		=
		pVtx[i * 22 + 1].color		=
		pVtx[i * 22 + 2].color		=
		pVtx[i * 22 + 3].color		=
		pVtx[i * 22 + 4].color		=
		pVtx[i * 22 + 5].color		=
		pVtx[i * 22 + 6].color		=
		pVtx[i * 22 + 7].color		=
		pVtx[i * 22 + 8].color		=
		pVtx[i * 22 + 9].color		=
		pVtx[i * 22 + 10].color		=
		pVtx[i * 22 + 11].color		=
		pVtx[i * 22 + 12].color		=
		pVtx[i * 22 + 13].color		=
		pVtx[i * 22 + 14].color		=
		pVtx[i * 22 + 15].color		=
		pVtx[i * 22 + 16].color		=
		pVtx[i * 22 + 17].color		=
		pVtx[i * 22 + 18].color		=
		pVtx[i * 22 + 19].color		=
		pVtx[i * 22 + 20].color		=
		pVtx[i * 22 + 21].color		=	D3DCOLOR_RGBA(192, 192, 255, 192);
	}

	g_pVtxBufferHitBox->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawHit
//�Ԃ�l	: 
//����		: �`��
//////////////////////////////////////////////////////////////////////////////
void	drawHit	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;
	int		numFace;	//�ʂ̐�
	int		i, j;			//loop
	D3DXMATRIX	world;				//���[���h�s��

	pDevice = getDevice();
	D3DXMatrixIdentity(&world);
	pDevice->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
	
	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferHitSquare, 0, sizeof(VERTEX3D));

	//���_�t�H�[�}�b�g�ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//=============================
	// �|���S���̕`��
	//=============================
	//��
	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		if(g_hitSquare[i].use == true)
		{
			pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,	//�|���S���̎��
					NUM_VERTEX * i,			//�J�n���_
					NUM_POLYGON				//�ʂ̐�
				);
		}
	}
	
	//��
	pDevice->SetStreamSource(0, g_pVtxBufferHitBox, 0, sizeof(VERTEX3D));
	//�e�N�X�`���ǂݍ���
	pDevice->SetTexture(0, NULL);
	//���_�t�H�[�}�b�g�ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(g_hitBox[i].use == true)
		{
			pDevice->DrawPrimitive
				(
					D3DPT_TRIANGLESTRIP,	//�|���S���̎��
					22 * i,					//�J�n���_
					NUM_POLYGON * 6 + 8	//�ʂ̐�
				);
		}
	}

	//��
	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].use == true)
		{
			//�s��ݒ�
			setMatrixHit(pDevice, i);
			
			//�e�N�X�`���ǂݍ���
			pDevice->SetTexture(0, g_pTexBufferHitBall[0]);

			//�`��
			g_hitBall[i].mesh->DrawSubset(0);
		}
	}
	
	//���C�g�̉e��on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixHit
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixHit	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;				//���[���h�s��
	D3DXMATRIX	move;				//���s�ړ��s��

	//���[���h�s�񏉊���
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);

	//���W�ϊ�
	D3DXMatrixTranslation(&move, g_hitBall[num].pos.x, g_hitBall[num].pos.y, g_hitBall[num].pos.z);	//���s�ړ�
	
	//�s�񍇐�
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitBox
//
//����		:	pos		���S���W
//				top		����4�_
//				bottom	�����4�_
//				type	�I�u�W�F�N�g�^�C�v
//
//�Ԃ�l	:	���Ԗڂ̔z��ɔ��肪�i�[���ꂽ��
//����		:	�q�b�g�{�b�N�X��ݒ肷��
//////////////////////////////////////////////////////////////////////////////
int	setHitBox	(D3DXVECTOR3 pos, HIT_VTX top, HIT_VTX bottom, OBJ_TYPE_3D type)
{
	//�ϐ��錾
	int			i;		//loop
	VERTEX3D	*pVtx;	//���_�|�C���^

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(g_hitBox[i].use == false)
		{
			g_hitBox[i].pos		=	pos;
			g_hitBox[i].top		=	top;
			g_hitBox[i].bottom	=	bottom;
			g_hitBox[i].type	=	type;
			g_hitBox[i].use		=	true;

			//���_���ݒ�
			g_pVtxBufferHitBox->Lock(0, 0, (void **)&pVtx, 0);

			pVtx[i * 22 + 0].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 1].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 2].pos		=	D3DXVECTOR3(pos.x + top.b_l.x, pos.y + top.b_l.y, pos.z + top.b_l.z);
			pVtx[i * 22 + 3].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);

			pVtx[i * 22 + 4].pos		=	D3DXVECTOR3(pos.x + bottom.b_l.x, pos.y + bottom.b_l.y, pos.z + bottom.b_l.z);
			pVtx[i * 22 + 5].pos		=	D3DXVECTOR3(pos.x + bottom.b_r.x, pos.y + bottom.b_r.y, pos.z + bottom.b_r.z);

			pVtx[i * 22 + 6].pos		=	D3DXVECTOR3(pos.x + bottom.t_l.x, pos.y + bottom.t_l.y, pos.z + bottom.t_l.z);
			pVtx[i * 22 + 7].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);

			pVtx[i * 22 + 8].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 9].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);
			pVtx[i * 22 + 10].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 11].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);
			pVtx[i * 22 + 12].pos		=	D3DXVECTOR3(pos.x + top.b_r.x, pos.y + top.b_r.y, pos.z + top.b_r.z);
			pVtx[i * 22 + 13].pos		=	D3DXVECTOR3(pos.x + top.t_r.x, pos.y + top.t_r.y, pos.z + top.t_r.z);

			pVtx[i * 22 + 14].pos		=	D3DXVECTOR3(pos.x + bottom.b_r.x, pos.y + bottom.b_r.y, pos.z + bottom.b_r.z);
			pVtx[i * 22 + 15].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);
			pVtx[i * 22 + 16].pos		=	D3DXVECTOR3(pos.x + bottom.t_r.x, pos.y + bottom.t_r.y, pos.z + bottom.t_r.z);

			pVtx[i * 22 + 17].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 18].pos		=	D3DXVECTOR3(pos.x + top.t_l.x, pos.y + top.t_l.y, pos.z + top.t_l.z);
			pVtx[i * 22 + 19].pos		=	D3DXVECTOR3(pos.x + top.b_l.x, pos.y + top.b_l.y, pos.z + top.b_l.z);
			pVtx[i * 22 + 20].pos		=	D3DXVECTOR3(pos.x + bottom.t_l.x, pos.y + bottom.t_l.y, pos.z + bottom.t_l.z);
			pVtx[i * 22 + 21].pos		=	D3DXVECTOR3(pos.x + bottom.b_l.x, pos.y + bottom.b_l.y, pos.z + bottom.b_l.z);
			
			pVtx[i * 22 + 0].color		=
			pVtx[i * 22 + 1].color		=
			pVtx[i * 22 + 2].color		=
			pVtx[i * 22 + 3].color		=
			pVtx[i * 22 + 4].color		=
			pVtx[i * 22 + 5].color		=
			pVtx[i * 22 + 6].color		=
			pVtx[i * 22 + 7].color		=
			pVtx[i * 22 + 8].color		=
			pVtx[i * 22 + 9].color		=
			pVtx[i * 22 + 10].color		=
			pVtx[i * 22 + 11].color		=
			pVtx[i * 22 + 12].color		=
			pVtx[i * 22 + 13].color		=
			pVtx[i * 22 + 14].color		=
			pVtx[i * 22 + 15].color		=
			pVtx[i * 22 + 16].color		=
			pVtx[i * 22 + 17].color		=
			pVtx[i * 22 + 18].color		=
			pVtx[i * 22 + 19].color		=
			pVtx[i * 22 + 20].color		=
			pVtx[i * 22 + 21].color		=	D3DCOLOR_RGBA(192, 192, 255, 192);

			g_pVtxBufferHitBox->Unlock();

			return i;
		}
	}

	//�S�Ă̍\���̂��g���Ă���
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitBoxVtx
//
//����		:	max		�ő咸�_
//				min		�Œᒸ�_
//				*top	����p�o�̓|�C���^
//				*bottom	����p�o�̓|�C���^
//
//�Ԃ�l	:
//����		:	�q�b�g�{�b�N�X�̒��_�ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setHitBoxVtx	(D3DXVECTOR3 max, D3DXVECTOR3 min, HIT_VTX *top, HIT_VTX *bottom)
{
	top->t_l = D3DXVECTOR3(min.x, max.y, max.z);
	top->t_r = D3DXVECTOR3(max.x, max.y, max.z);
	top->b_l = D3DXVECTOR3(min.x, max.y, min.z);
	top->b_r = D3DXVECTOR3(max.x, max.y, min.z);

	bottom->t_l = D3DXVECTOR3(min.x, min.y, max.z);
	bottom->t_r = D3DXVECTOR3(max.x, min.y, max.z);
	bottom->b_l = D3DXVECTOR3(min.x, min.y, min.z);
	bottom->b_r = D3DXVECTOR3(max.x, min.y, min.z);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitSquare
//
//����		:	pos		���S���W
//				vtx		4���̍��W
//
//�Ԃ�l	:	�\���̂̔z��ԍ�
//����		:	�ʂ̓����蔻��ǉ�
//////////////////////////////////////////////////////////////////////////////
int	setHitSquare	(D3DXVECTOR3 pos, HIT_VTX vtx)
{
	//�ϐ��錾
	VERTEX3D	*pVtx;	
	int			i;		//loop

	for(i = 0; i < MAX_HIT_SQUARE; i++)
	{
		if(g_hitSquare[i].use == false)
		{
			g_hitSquare[i].pos	=	pos;
			g_hitSquare[i].vtx	=	vtx;
			g_hitSquare[i].use	=	true;

			g_pVtxBufferHitSquare->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[i * NUM_VERTEX + 0].pos	=	D3DXVECTOR3(pos.x + vtx.t_l.x, pos.y + vtx.t_l.y, pos.z + vtx.t_l.z);
			pVtx[i * NUM_VERTEX + 1].pos	=	D3DXVECTOR3(pos.x + vtx.t_r.x, pos.y + vtx.t_r.y, pos.z + vtx.t_r.z);
			pVtx[i * NUM_VERTEX + 2].pos	=	D3DXVECTOR3(pos.x + vtx.b_l.x, pos.y + vtx.b_l.y, pos.z + vtx.b_l.z);
			pVtx[i * NUM_VERTEX + 3].pos	=	D3DXVECTOR3(pos.x + vtx.b_r.x, pos.y + vtx.b_r.y, pos.z + vtx.b_r.z);

			pVtx[i * NUM_VERTEX + 0].color	=
			pVtx[i * NUM_VERTEX + 1].color	=
			pVtx[i * NUM_VERTEX + 2].color	=
			pVtx[i * NUM_VERTEX + 3].color	=	D3DXCOLOR(0.f, 0.f, 1.f, 0.3f);

			g_pVtxBufferHitSquare->Unlock();

			return i;
		}
	}

	//�S�Ă̍\���̂��g���Ă���
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitBall
//
//����		:	pos		���S���W
//				len		���a�̒���
//
//�Ԃ�l	:	�\���̂̔z��ԍ�
//����		:	���̓����蔻��ǉ�
//////////////////////////////////////////////////////////////////////////////
int	setHitBall	(D3DXVECTOR3 pos, float len, OBJ_TYPE_3D type)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;
	int		i;		//loop

	pDevice = getDevice();

	for(i = 0; i < MAX_HIT_BALL; i++)
	{
		if(g_hitBall[i].use == false)
		{
			g_hitBall[i].pos	=	pos;
			g_hitBall[i].len	=	len;
			g_hitBall[i].type	=	type;
			g_hitBall[i].use	=	true;

			D3DXCreateSphere(
				pDevice,					// �`���̃f�o�C�X
				g_hitBall[i].len,			// ���a�B0.0f�ł�����OK�B�}�C�i�X��NG
				32,							// �X���C�X�̐��B���₷�Ƃ��ꂢ�ȋ��ɂȂ�͂�
				16,							// �X�^�b�N���B���₷�Ƃ��ꂢ�ȋ��ɂȂ�͂�
				&g_hitBall[i].mesh,			// �o�͂��郁�b�V��
				NULL						// ���܂�g��Ȃ��炵�����NULL�ɂ��Ƃ���
			);

			return i;
		}
	}

	//�S�Ă̍\���̂��g���Ă���
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	releaseHitBall
//
//����		:	num		�z��ԍ�
//
//�Ԃ�l	:
//����		:	���̍\���̉��
//////////////////////////////////////////////////////////////////////////////
void	releaseHitBall	(int num)
{
	if(num < MAX_HIT_BALL)
	{
		g_hitBall[num].use	=	false;
		g_hitBall[num].mesh->Release();
		g_hitBall[num].mesh = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitBoxPos
//
//����		:	num		�z��ԍ�
//				pos		�ݒ肵�������W
//
//�Ԃ�l	:
//����		:	���W�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setHitBoxPos	(int num, D3DXVECTOR3 pos)
{
	if(num < MAX_HIT_BOX)
	{
		g_hitBox[num].pos = pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setHitBallPos
//
//����		:	num		�z��ԍ�
//				pos		�ݒ肵�������W
//
//�Ԃ�l	:
//����		:	���̓����蔻��̍��W�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setHitBallPos	(int num, D3DXVECTOR3 pos)
{
	if(num < MAX_HIT_BALL)
	{
		g_hitBall[num].pos = pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	getXfileVtx
//
//����		:	*fileName	����������X�t�@�C���̖��O
//				*max		�ő咸�_�o�͐�
//				*min		�ŏ����_�o�͐�
//
//�Ԃ�l	:
//����		:	X�t�@�C������ő�ŏ����_�擾
//////////////////////////////////////////////////////////////////////////////
void	getXfileVtx		(char *fileName, D3DXVECTOR3 *max, D3DXVECTOR3 *min)
{
	//�ϐ��錾
	FILE		*fp = NULL;			//�t�@�C���|�C���^
	char		target[256];		//�������[�h
	char		check[256];			//�`�F�b�N���[�h
	D3DXVECTOR3	work;				//���[�N
	char		end = '\0';			//��������
	HIT_VTX		top, bottom;		//HitBox�p

	//�ϐ�������
	*max = *min = work = D3DXVECTOR3(0.f, 0.f, 0.f);

	//�T�������񌟍�
	sprintf(target, "Mesh {\n");

	//�t�@�C���I�[�v��
	fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		printf("�t�@�C���I�[�v�����s");
		rewind(stdin);
		getchar();
		return;
	}

	//������T��
	while(strcmp(check, target) != 0)
	{
		check[0] = '\0';
		fgets(check, 255, fp);
	}
	
	//���_��
	fgets(check, 255, fp);

	//���W�擾
	while(end != ';')
	{
		fscanf(fp, "%f;%f;%f;%c", &work.x, &work.y, &work.z, &end);

		//�ő咸�_
		if(work.x > max->x)
			max->x = work.x;
		if(work.y > max->y)
			max->y = work.y;
		if(work.z > max->z)
			max->z = work.z;

		//�Œᒸ�_
		if(work.x < min->x)
			min->x = work.x;
		if(work.y < min->y)
			min->y = work.y;
		if(work.z < min->z)
			min->z = work.z;
	}

	//�t�@�C���I��
	if(fp != NULL)
	{
		fclose(fp);
	}
}

/////////////////////////////////////////////////////////////////////////////
//�֐���	:	getHitBoxType
//
//����		:	num		�z��ԍ�
//
//�Ԃ�l	:	�I�u�W�F�N�g�^�C�v
//����		:	�q�b�g�{�b�N�X�̃I�u�W�F�N�g�^�C�v�擾
//////////////////////////////////////////////////////////////////////////////
OBJ_TYPE_3D	getHitBoxType	( int num )
{
	if(g_hitBox[num].use == true)
	{
		return g_hitBox[num].type;
	}

	return BOJ_TYPE_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//�֐���	:	getHitBox
//
//����		:	num		�z��ԍ�
//
//�Ԃ�l	:	HitBox�ϐ�
//����		:	�q�b�g�{�b�N�X�擾
//////////////////////////////////////////////////////////////////////////////
HIT_BOX	getHitBox	( int num )
{
	return g_hitBox[num];
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexHit
//�Ԃ�l	: 
//����		: ���_�쐬
//////////////////////////////////////////////////////////////////////////////
HRESULT makeVertexHit ( LPDIRECT3DDEVICE9 pDevice )
{
	//���_�o�b�t�@�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * 22 * MAX_HIT_BOX,	//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,				//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_3D,					//
			D3DPOOL_MANAGED,				//�������Ǘ����@
			&g_pVtxBufferHitBox,			//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * MAX_HIT_SQUARE * NUM_VERTEX,	//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,				//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_3D,					//
			D3DPOOL_MANAGED,				//�������Ǘ����@
			&g_pVtxBufferHitSquare,			//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: hitVec
//�Ԃ�l	: 
//����		: ���Ɠ_�̏Փ˔���
//////////////////////////////////////////////////////////////////////////////
bool	hitVec	( D3DXVECTOR2 vec1, D3DXVECTOR2 vec2, D3DXVECTOR2 point )
{
	if((vec2.x - vec1.x) * (point.y - vec1.y) - (point.x - vec1.x) * (vec2.y - vec1.y) <= 0)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: hitFacePoint
//�Ԃ�l	: 
//����		: �ʂƓ_�ł̓����蔻��
//////////////////////////////////////////////////////////////////////////////
bool	hitFacePoint	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 target )
{
	//�ϐ��錾
	D3DXVECTOR3		normal;		//�@���x�N�g��
	double			inner;		//����

	//�@���x�N�g�������߂�
	normal = getNormalVec(point, vec1, vec2);
	normal = vec3Normalize(normal);

	//���߂����_�܂ł̃x�N�g�������߂�
	target = target - point;
	target = vec3Normalize(target);

	//�@���x�N�g���Ɠ_�̓��ς����߂�
	inner = getInnerProduct(normal, target);

	//���ς�0��菬������Γ������Ă���
	if(inner <= 0)
	{
		return true;
	}

	//�����łȂ����false
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	hitBox
//
//����		:	box1	��r��������
//				box2	��r�Ώ�
//
//�Ԃ�l	:	true�œ������Ă���
//����		:	�ʂƓ_�̓����蔻��ɂ�闧���̂̓����蔻��
//////////////////////////////////////////////////////////////////////////////
bool	hitBox	( HIT_BOX box1, HIT_BOX box2 )
{
	//�ϐ��錾
	int				i;		//loop
	D3DXVECTOR3		target[NUM_VERTEX * 2];

	//�ϐ����
	target[0] = box2.top.t_l;
	target[1] = box2.top.t_r;
	target[2] = box2.top.b_l;
	target[3] = box2.top.b_r;
	target[4] = box2.bottom.t_l;
	target[5] = box2.bottom.t_r;
	target[6] = box2.bottom.b_l;
	target[7] = box2.bottom.b_r;

	for(i = 0; i < NUM_VERTEX * 2; i++)
	{
		//box2�Ƃ̔���
		if(hitFacePoint(box1.top.t_l, box1.top.t_r, box1.top.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.bottom.t_l, box1.bottom.t_r, box1.bottom.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_r, box1.bottom.t_r, box1.top.b_r, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_l, box1.bottom.t_l, box1.top.b_l, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.b_l, box1.bottom.b_l, box1.top.b_r, target[i]) == false)
			return false;

		if(hitFacePoint(box1.top.t_l, box1.top.t_r, box1.bottom.t_l, target[i]) == false)
			return false;
	}

	//�S�ăp�X����Ɠ������Ă���
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//�֐���	:	hitFaceLine
//
//����		:	point	���_
//				face	�|���S��
//				sLine	�����̎n�_
//				eLine	�����̏I�_
//
//�Ԃ�l	:	true�œ������Ă���
//����		:	�ʂƐ��̓����蔻��
//////////////////////////////////////////////////////////////////////////////
bool	hitFaceLine	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine)
{
	//�ϐ��錾
	D3DXVECTOR3		normal;		//�@���x�N�g��
	double			inner[2];	//����

	//�@���x�N�g���擾
	normal = getNormalVec(point, vec1, vec2);
	normal = vec3Normalize(normal);

	//�������_�ɍ��킹��
	sLine = vec3Normalize(sLine - point);
	sLine = vec3Normalize(sLine);
	eLine = vec3Normalize(eLine - point);
	eLine = vec3Normalize(eLine);

	//���ώ擾
	inner[0] = getInnerProduct(normal, sLine);
	inner[1] = getInnerProduct(normal, eLine);

	//�Е����݊p�̏ꍇ�������Ă���
	if((inner[0] <= 0.f && inner[1] >= 0.f) ||
		(inner[0] >= 0.f && inner[1] <= 0.f))
	{
		return true;
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//�֐���	:	hitBoxLine
//
//����		:	box		�������肽����
//				sLine	�����̎n�_
//				eLine	�����̏I�_
//
//�Ԃ�l	:	true�œ������Ă���
//����		:	���Ɛ��̓����蔻��
//////////////////////////////////////////////////////////////////////////////
bool	hitBoxLine	( HIT_BOX box, D3DXVECTOR3 sLine, D3DXVECTOR3 eLine )
{
	if(hitFaceLine(box.top.t_l, box.top.t_r, box.top.b_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.bottom.t_l, box.bottom.t_r, box.bottom.b_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_l, box.top.b_l, box.bottom.t_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_r, box.top.b_r, box.bottom.t_r, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.t_l, box.top.t_r, box.bottom.t_l, sLine, eLine) == true)
		return true;

	if(hitFaceLine(box.top.b_l, box.top.b_r, box.bottom.b_l, sLine, eLine) == true)
		return true;
}

/////////////////////////////////////////////////////////////////////////////
//�֐���	:	hitBall
//
//����		:	box		�������肽����
//				sLine	�����̎n�_
//				eLine	�����̏I�_
//
//�Ԃ�l	:	true�œ������Ă���
//����		:	���Ƌ��̓����蔻��
//////////////////////////////////////////////////////////////////////////////
bool	hitBoall	( HIT_BALL ball1, HIT_BALL ball2 )
{
	//�ϐ��錾
	float		len;		//���Ƌ��̋���

	//��̋��̋������v�Z
	len = sqrt((ball2.pos.x - ball1.pos.x) * (ball2.pos.x - ball1.pos.x)
		+ (ball2.pos.y - ball1.pos.y) * (ball2.pos.y - ball1.pos.y)
		+ (ball2.pos.z - ball1.pos.y) * (ball2.pos.z - ball1.pos.y));

	//�������̔��a�̍��v�������
	len -= ball1.len + ball2.len;

	//len��0���傫����Γ������Ă���
	if(len >= 0)
	{
		return true;
	}

	return false;
}