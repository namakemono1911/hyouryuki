/*****************************************************************************
���̃��f���\��[spear.h]
Aythor	: ���@��
Data	: 2017_01_23
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "spear.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MODEL_NAME		"data/model/spear.x"
#define	SPEED			(2)
#define	MAX_SPEAR		(30)

#define GRAVITY			(0.03f)

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	setMatrixSpear	( LPDIRECT3DDEVICE9 pDev, int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshSpear		= NULL;		//���b�V�����C���^�[�t�F�[�X
DWORD				g_nNumMatSpear		= NULL;		//�}�e���A�����
D3DMATERIAL9		*g_pmeshMatSpear	= NULL;		//�}�e���A�����

LPDIRECT3DTEXTURE9	*g_pTextureSpear;			//�e�N�X�`�����C���^�[�t�F�[�X
SPEAR				spear[MAX_SPEAR];			//���f���\����

int					g_numSpear = 0;				//�o�Ă郂�f����

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initSpear
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	initSpear	( void )
{
	//�ϐ��錾
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPD3DXBUFFER	bufMatSpear;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i;

	//�\���̏�����
	for(i = 0; i < MAX_SPEAR; i++)
	{
		spear[i].use	=	false;
	}

	hr	=	D3DXLoadMeshFromX(MODEL_NAME,
				D3DXMESH_MANAGED,
				pDev,
				NULL,
				&bufMatSpear,
				NULL,
				&g_nNumMatSpear,
				&g_pMeshSpear);
	
	if(FAILED(hr))
	{
		MessageBox(NULL, "���f���ǂݍ��݃G���[", "error", MB_OK);
	}

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)bufMatSpear->GetBufferPointer();
	
	//�e�N�X�`���o�b�t�@�쐬
	g_pmeshMatSpear	=	new D3DMATERIAL9[g_nNumMatSpear];
	g_pTextureSpear	=	new LPDIRECT3DTEXTURE9[g_nNumMatSpear];
	
	for(i = 0; i < (int)g_nNumMatSpear; i++)
	{
		g_pmeshMatSpear[i] =	pMat[i].MatD3D;		//�}�e���A�����Z�b�g
		g_pmeshMatSpear[i].Ambient	=	g_pmeshMatSpear[i].Diffuse;	//����������
		g_pTextureSpear[i] = NULL;				//�e�N�X�`��������

		//�e�N�X�`���̓ǂݍ���
		if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureSpear[i])))
		{
			if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureSpear[i])))
			{
				MessageBox(NULL, "�G���[", "spear�e�N�X�`���G���[", MB_OK);
			}
		}
	}

	bufMatSpear->Release();
	bufMatSpear = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitSpear
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitSpear	( void )
{
	//�ϐ��錾
	int		i;	//loop

	//���b�V�����̉��
	if(g_pMeshSpear != NULL)
	{
		g_pMeshSpear->Release();
		g_pMeshSpear = NULL;
	}
	
	//�e�N�X�`�����
	for(i = 0; i < (int)g_nNumMatSpear; i++)
	{
		if(g_pTextureSpear[i] != NULL)
		{
			g_pTextureSpear[i]->Release();
			g_pTextureSpear[i] = NULL;
		}
	}

	delete[] g_pTextureSpear[i];
	g_pTextureSpear[i] = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateSpear
//�Ԃ�l	:
//����		:	�X�V����
////////////////////////////////////////////////////////////////////////////
void	updateSpear	( void )
{
	//�ϐ��錾
	int		i, j;		//loop

	for(i = 0, j = 0; j < g_numSpear; i++)
	{
		if(spear[i].use == true)
		{
			spear[i].pos = D3DXVECTOR3(
				spear[i].MoveVec.x * spear[i].move + spear[i].pos.x,
				spear[i].MoveVec.y * spear[i].move + spear[i].pos.y + spear[i].gravity,
				spear[i].MoveVec.z * spear[i].move + spear[i].pos.z);

			spear[i].gravity -= GRAVITY;

			if(spear[i].pos.y <= -10.f)
			{
				spear[i].use = false;
				g_numSpear--;
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawSpear
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	drawSpear	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//�ϐ��錾
	
	for(i = 0, j = 0; j < g_numSpear; i++)
	{
		if(spear[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatSpear; k++)
			{
				//�s��ݒ�
				setMatrixSpear(pDev, i);

				//�}�e���A���ݒ�
				pDev->SetMaterial(&g_pmeshMatSpear[k]);

				//�e�N�X�`���ǂݍ���
				pDev->SetTexture(0, g_pTextureSpear[k]);

				//���b�V���`��
				g_pMeshSpear->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixSpear
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixSpear	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;				//���[���h�s��
	D3DXMATRIX	scale;				//�g�k�s��
	D3DXMATRIX	rot;				//��]�s��
	D3DXMATRIX	move;				//���s�ړ��s��

	//���[���h�s�񏉊���
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scale);

	//���W�ϊ�
	D3DXMatrixScaling(&scale, spear[num].size.x, spear[num].size.y, spear[num].size.z);	//�g�k�s��
	D3DXMatrixRotationYawPitchRoll(&rot, spear[num].angle.x, spear[num].angle.y, spear[num].angle.z);		//��]�s��
	D3DXMatrixTranslation(&move, spear[num].pos.x, spear[num].pos.y, spear[num].pos.z);	//���s�ړ�
	
	//�s�񍇐�
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getSpear
//�Ԃ�l	: 
//����		: ���f���̃|�C���^�擾
//////////////////////////////////////////////////////////////////////////////
SPEAR	*getSpear	( int num )
{
	return &spear[num];
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setSpear
//�Ԃ�l	: 
//����		: ���Z�b�g
//////////////////////////////////////////////////////////////////////////////
void	setSpear	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move )
{
	//�ϐ��錾
	int		i;		//loop

	for(i = 0; i < MAX_SPEAR; i++)
	{
		if(spear[i].use == false)
		{
			//�ϐ��Z�b�g
			spear[i].pos		=	pos;		//���W
			spear[i].size		=	size;		//�T�C�Y
			spear[i].angle		=	angle;		//�p�x
			spear[i].MoveVec	=	moveVec;	//�����x�N�g��
			spear[i].move		=	move;		//���x
			spear[i].gravity	=	0.f;		//�d��
			spear[i].use		=	true;		//�g�p�t���O

			g_numSpear++;
			break;
		}
	}
}