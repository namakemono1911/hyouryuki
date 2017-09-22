/*****************************************************************************
���؂̃��f���\��[wood.h]
Aythor	: ���@��
Data	: 2017_01_18
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "vector.h"
#include "wood.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_WOOD		(30)		//��x�ɏo���ō���
#define	LIMIT_DISTANCE	(1000)		//���E����

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWood	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateWood000	( int num );
void	updateWood001	( int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshWood[WOOD_TYPE_MAX] = {NULL};	//���b�V�����C���^�[�t�F�[�X
DWORD				g_nNumMatWood = NULL;					//�}�e���A�����
D3DMATERIAL9		*g_pmeshMatWood = NULL;					//�}�e���A�����

LPDIRECT3DTEXTURE9	*g_pTextureWood[WOOD_TYPE_MAX];			//�e�N�X�`�����C���^�[�t�F�[�X
WOOD				wood[MAX_WOOD];							//���f���\����
int					g_numWood = 0;							//���f������
char				*woodModelName[WOOD_TYPE_MAX] =			//�e�N�X�`���t�@�C����
{
	"data/model/wood000.x",
	"data/model/wood001.x",
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initWood
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	initWood	( void )
{
	//�ϐ��錾
	LPD3DXBUFFER		pBuffMatWood = NULL;		//�}�e���A�����
	HRESULT				hr;
	D3DMATERIAL9		matDef;
	D3DXMATERIAL		*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j;	//loop

	//�\���̏�����
	for(i = 0; i < MAX_WOOD; i++)
	{
		wood[i].use		=	false;
	}
	
	for(j = 0; j < WOOD_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(woodModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatWood,
					NULL,
					&g_nNumMatWood,
					&g_pMeshWood[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "wood���f���ǂݍ��݃G���[", "error", MB_OK);
			return;
		}

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatWood->GetBufferPointer();

		g_pmeshMatWood	=	new D3DMATERIAL9[g_nNumMatWood];
		g_pTextureWood[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatWood];

		for(i = 0; i < (int)g_nNumMatWood; i++)
		{
			g_pmeshMatWood[i] =	pMat[i].MatD3D;		//�}�e���A�����Z�b�g
			g_pmeshMatWood[i].Ambient	=	g_pmeshMatWood[i].Diffuse;	//����������
			g_pTextureWood[j][i] = NULL;				//�e�N�X�`��������

			//�e�N�X�`���̓ǂݍ���
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureWood[j][i])))
				{
					MessageBox(NULL, "�G���[", "wood�e�N�X�`���G���[", MB_OK);
				}
			}
		}
	}

	//�}�e���A�������
	pBuffMatWood->Release();
	pBuffMatWood = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitWood
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitWood	( void )
{
	//�ϐ��錾
	DWORD	i, j;		//loop

	//���b�V�����̉��
	for(i = 0; i < WOOD_TYPE_MAX; i++)
	{
		if(g_pMeshWood[i] != NULL)
		{
			g_pMeshWood[i]->Release();

			delete[] g_pMeshWood;
			g_pMeshWood[i] = NULL;
		}
	}

	//�e�N�X�`�����
	for(i = 0; i < WOOD_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatWood; j++)
		{
			if(g_pTextureWood[i] != NULL)
			{
				g_pTextureWood[i][j]->Release();
				g_pTextureWood[i][j] = NULL;
			}
		}
		delete[] g_pTextureWood[i];
		g_pTextureWood[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateWood
//�Ԃ�l	:
//����		:	�X�V����
////////////////////////////////////////////////////////////////////////////
void	updateWood	( void )
{
	//�ϐ��錾
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numWood; i++)
	{
		if(wood[i].use == true)
		{
			if(getPosLen(getCameraPos(), wood[i].pos) > LIMIT_DISTANCE)
			{
				releaseWood(i);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawWood
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	drawWood	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//�ϐ��錾
	
	for(i = 0, j = 0; j < g_numWood; i++)
	{
		if(wood[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatWood; k++)
			{
				//�s��ݒ�
				setMatrixWood(pDev, i);

				//�}�e���A���ݒ�
				pDev->SetMaterial(&g_pmeshMatWood[k]);

				//�e�N�X�`���ǂݍ���
				pDev->SetTexture(0, g_pTextureWood[wood[i].type][k]);

				//���b�V���`��
				g_pMeshWood[wood[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixWood
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWood	( LPDIRECT3DDEVICE9 pDev, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;		//���[���h�s��
	D3DXMATRIX	scale;		//�g�k�s��
	D3DXMATRIX	rot;		//��]�s��
	D3DXMATRIX	move;		//���s�ړ��s��

	//���[���h�s�񏉊���
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&move);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&scale);

	//���W�ϊ�
	D3DXMatrixScaling(&scale, wood[num].size.x, wood[num].size.y, wood[num].size.z);					//�g�k
	D3DXMatrixRotationYawPitchRoll(&rot, wood[num].angle.x, wood[num].angle.y, wood[num].angle.z);		//��]�s��
	D3DXMatrixTranslation(&move, wood[num].pos.x, wood[num].pos.y, wood[num].pos.z);					//���s�ړ�
	
	//�s�񍇐�
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getWood
//�Ԃ�l	: 
//����		: ���f���̃|�C���^�擾
//////////////////////////////////////////////////////////////////////////////
WOOD	*getWood	( int num )
{
	if(wood[num].use != false)
	{
		return &wood[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setWood
//�Ԃ�l	: 
//����		: ���f���̃Z�b�g
//////////////////////////////////////////////////////////////////////////////
void	setWood	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, D3DXVECTOR3 moveVec, float move, WOOD_TYPE type )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_WOOD; i++)
	{
		if(wood[i].use == false)
		{
			wood[i].pos		=	pos;
			wood[i].size	=	size;
			wood[i].angle	=	angle;
			wood[i].moveVec	=	moveVec;
			wood[i].move	=	move;
			wood[i].type	=	type;
			wood[i].use		=	true;

			g_numWood++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: releaseWood
//�Ԃ�l	: 
//����		: ���؃����[�X
//////////////////////////////////////////////////////////////////////////////
void	releaseWood	( int num )
{
	wood[num].use = false;
	g_numWood--;
}