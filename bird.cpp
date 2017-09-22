/*****************************************************************************
���̃��f���`��[bird.cpp]
Aythor	: ���@��
Data	: 2017_01_15
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "bird.h"
#include "vector.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_BIRD		(30)		//��x�ɏo���ō���
#define	LIMIT_DISTANCE	(1000)		//�����ł�����E����

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	setMatrixBird	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateBird000	( int num );
void	updateBird001	( int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshBird[BIRD_TYPE_MAX] = {NULL};	//���b�V�����C���^�[�t�F�[�X
DWORD				g_nNumMatBird = NULL;		//�}�e���A�����
D3DMATERIAL9		*g_pmeshMatBird = NULL;		//�}�e���A�����

LPDIRECT3DTEXTURE9	*g_pTextureBird[BIRD_TYPE_MAX];			//�e�N�X�`�����C���^�[�t�F�[�X
BIRD				bird[MAX_BIRD];				//���f���\����
int					g_numBird = 0;				//���f������
char				*birdModelName[BIRD_TYPE_MAX] =
{
	"data/model/kamome.x",
	"data/model/flingkamome.x",
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initBird
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	initBird	( void )
{
	//�ϐ��錾
	LPD3DXBUFFER	pBuffMatBird = NULL;		//�}�e���A�����
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j;	//loop

	//�\���̏�����
	for(i = 0; i < MAX_BIRD; i++)
	{
		bird[i].use		=	false;
	}
	
	for(j = 0; j < BIRD_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(birdModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatBird,
					NULL,
					&g_nNumMatBird,
					&g_pMeshBird[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "bird���f���ǂݍ��݃G���[", "error", MB_OK);
			return;
		}

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatBird->GetBufferPointer();

		g_pmeshMatBird	=	new D3DMATERIAL9[g_nNumMatBird];
		g_pTextureBird[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatBird];

		for(i = 0; i < (int)g_nNumMatBird; i++)
		{
			g_pmeshMatBird[i] =	pMat[i].MatD3D;		//�}�e���A�����Z�b�g
			g_pmeshMatBird[i].Ambient	=	g_pmeshMatBird[i].Diffuse;	//����������
			g_pTextureBird[j][i] = NULL;				//�e�N�X�`��������

			//�e�N�X�`���̓ǂݍ���
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureBird[j][i])))
				{
					MessageBox(NULL, "�G���[", "bird�e�N�X�`���G���[", MB_OK);
				}
			}
		}
	}

	//�}�e���A�������
	pBuffMatBird->Release();
	pBuffMatBird = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitBird
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitBird	( void )
{
	//�ϐ��錾
	DWORD	i, j;		//loop

	//���b�V�����̉��
	for(i = 0; i < BIRD_TYPE_MAX; i++)
	{
		if(g_pMeshBird[i] != NULL)
		{
			g_pMeshBird[i]->Release();

			delete[] g_pMeshBird;
			g_pMeshBird[i] = NULL;
		}
	}

	//�e�N�X�`�����
	for(i = 0; i < BIRD_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatBird; j++)
		{
			if(g_pTextureBird[i] != NULL)
			{
				g_pTextureBird[i][j]->Release();
				g_pTextureBird[i][j] = NULL;
			}
		}
		delete[] g_pTextureBird[i];
		g_pTextureBird[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateBird
//�Ԃ�l	:
//����		:	�X�V����
////////////////////////////////////////////////////////////////////////////
void	updateBird	( void )
{
	//�ϐ��錾
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numBird; i++)
	{
		if(bird[i].use == true)
		{
			switch(bird[i].type)
			{
			case BIRD_TYPE_000:
				{
					updateBird000(i);
					break;
				}

			case BIRD_TYPE_001:
				{
					updateBird001(i);
					break;
				}

			default:
				{
					MessageBox(NULL, "error", "birdupdate�G���[", MB_OK);
					break;
				}
			}
			
			if(getPosLen(getCameraPos(), bird[i].pos) > LIMIT_DISTANCE)
			{
				releaseBird(i);
			}

			j++;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateBird000
//�Ԃ�l	: 
//����		: ���f���^�C�v000�̍X�V
//////////////////////////////////////////////////////////////////////////////
void	updateBird000	( int num )
{
	//�ϐ��錾


}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateBird001
//�Ԃ�l	: 
//����		: ���f���^�C�v001�̍X�V
//////////////////////////////////////////////////////////////////////////////
void	updateBird001	( int num )
{
	//�ϐ��錾

	//�p�x�X�V
	bird[num].angle.x += 0.005f;
	bird[num].angle.x = fmod(bird[num].angle.x, PI * 2);

	//�i�s�����ύX
	bird[num].moveVec = D3DXVECTOR3(
		-cos(bird[num].angle.x - PI * 0.5f),
		0.f,
		sin(bird[num].angle.x - PI * 0.5f));

	D3DXVec3Normalize(&bird[num].moveVec, &bird[num].moveVec);

	//���W���Z
	bird[num].pos = D3DXVECTOR3(
		bird[num].move * bird[num].moveVec.x + bird[num].pos.x,
		bird[num].move * bird[num].moveVec.y + bird[num].pos.y,
		bird[num].move * bird[num].moveVec.z + bird[num].pos.z);

	setHitBallPos(bird[num].collisionID, bird[num].pos);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawBird
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	drawBird	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//�ϐ��錾
	
	for(i = 0, j = 0; j < g_numBird; i++)
	{
		if(bird[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatBird; k++)
			{
				//�s��ݒ�
				setMatrixBird(pDev, i);

				//�}�e���A���ݒ�
				pDev->SetMaterial(&g_pmeshMatBird[k]);

				//�e�N�X�`���ǂݍ���
				pDev->SetTexture(0, g_pTextureBird[bird[i].type][k]);

				//���b�V���`��
				g_pMeshBird[bird[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixBird
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixBird	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;				//���[���h�s��
	D3DXMATRIX	move;				//���s�ړ��s��
	D3DXMATRIX	rot;				//��]�s��

	//���[���h�s�񏉊���
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);

	//���W�ϊ�
	D3DXMatrixRotationYawPitchRoll(&rot, bird[num].angle.x, bird[num].angle.y, bird[num].angle.z);		//��]�s��
	D3DXMatrixTranslation(&move, bird[num].pos.x, bird[num].pos.y, bird[num].pos.z);	//���s�ړ�
	
	//�s�񍇐�
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getBird
//�Ԃ�l	: 
//����		: ���f���̃|�C���^�擾
//////////////////////////////////////////////////////////////////////////////
BIRD	*getBird	( int num )
{
	if(bird[num].use != false)
	{
		return &bird[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setBird
//�Ԃ�l	: 
//����		: ���f���̃Z�b�g
//////////////////////////////////////////////////////////////////////////////
void	setBird	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move, BIRD_TYPE type)
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_BIRD; i++)
	{
		if(bird[i].use == false)
		{
			bird[i].pos		=	pos;
			bird[i].size	=	size;
			bird[i].angle	=	angle;
			bird[i].moveVec	=	moveVec;
			bird[i].move	=	move;
			bird[i].type	=	type;
			bird[i].use		=	true;

			//�R���W�����ݒ�
			bird[i].collisionID = setHitBall(pos, 5.f, TYPE_BIRD);

			g_numBird++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: releaseBird
//�Ԃ�l	: 
//����		: ���̒��𖳂��������Ƃ�
//////////////////////////////////////////////////////////////////////////////
void	releaseBird	( int num )
{
	//�R���W�������
	releaseHitBall(bird[num].collisionID);

	
	bird[num].use = false;
	g_numBird--;
}