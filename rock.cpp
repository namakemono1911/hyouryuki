/*****************************************************************************
��̃��f���\��[rock.h]
Aythor	: ���@��
Data	: 2017_02_07
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "vector.h"
#include "rock.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MAX_ROCK		(30)		//��x�ɏo���ō���
#define	LIMIT_DISTANCE	(1000)		//���E����

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	setMatrixRock	( LPDIRECT3DDEVICE9 pDev, int num );
void	updateRock000	( int num );
void	updateRock001	( int num );
void	setCollision	( LPDIRECT3DDEVICE9 pDevice, LPD3DXMESH mesh );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshRock[ROCK_TYPE_MAX] = {NULL};	//���b�V�����C���^�[�t�F�[�X
LPDIRECT3DTEXTURE9	*g_pTextureRock[ROCK_TYPE_MAX];			//�e�N�X�`�����C���^�[�t�F�[�X
DWORD				g_nNumMatRock = NULL;					//�}�e���A�����
D3DMATERIAL9		*g_pmeshMatRock = NULL;					//�}�e���A�����

HIT_VTX				top[ROCK_TYPE_MAX];						//HitBox�p
HIT_VTX				bottom[ROCK_TYPE_MAX];									
ROCK				rock[MAX_ROCK];							//���f���\����
int					g_numRock = 0;							//���f������
char				*rockModelName[ROCK_TYPE_MAX] =			//�e�N�X�`���t�@�C����
{
	"data/model/rock001.x",
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initRock
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	initRock	( void )
{
	//�ϐ��錾
	LPD3DXBUFFER		pBuffMatRock = NULL;		//�}�e���A�����
	HRESULT				hr;
	D3DMATERIAL9		matDef;
	D3DXMATERIAL		*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	D3DXVECTOR3			max, min;	//�ő�ŏ����_
	int		i, j;	//loop

	//�\���̏�����
	for(i = 0; i < MAX_ROCK; i++)
	{
		rock[i].use		=	false;
	}
	
	for(j = 0; j < ROCK_TYPE_MAX; j++)
	{
		hr	=	D3DXLoadMeshFromX(rockModelName[j],
					D3DXMESH_MANAGED,
					pDev,
					NULL,
					&pBuffMatRock,
					NULL,
					&g_nNumMatRock,
					&g_pMeshRock[j]);
	
		if(FAILED(hr))
		{
			MessageBox(NULL, "rock���f���ǂݍ��݃G���[", "error", MB_OK);
			return;
		}

		//�R���W�����o�^
		getXfileVtx(rockModelName[j], &max, &min);
		setHitBoxVtx(max, min, &top[j], &bottom[j]);

		pDev->GetMaterial(&matDef);
		pMat = (D3DXMATERIAL*)pBuffMatRock->GetBufferPointer();

		g_pmeshMatRock	=	new D3DMATERIAL9[g_nNumMatRock];
		g_pTextureRock[j]	=	new LPDIRECT3DTEXTURE9[g_nNumMatRock];

		for(i = 0; i < (int)g_nNumMatRock; i++)
		{
			g_pmeshMatRock[i] =	pMat[i].MatD3D;		//�}�e���A�����Z�b�g
			g_pmeshMatRock[i].Ambient	=	g_pmeshMatRock[i].Diffuse;	//����������
			g_pTextureRock[j][i] = NULL;				//�e�N�X�`��������

			//�e�N�X�`���̓ǂݍ���
			if(pMat[i].pTextureFilename != NULL && lstrlen(pMat[i].pTextureFilename) > 0)
			{
				if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureRock[j][i])))
				{
					MessageBox(NULL, "�G���[", "rock�e�N�X�`���G���[", MB_OK);
				}
			}
		}
	}

	//�}�e���A�������
	pBuffMatRock->Release();
	pBuffMatRock = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitRock
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitRock	( void )
{
	//�ϐ��錾
	DWORD	i, j;		//loop

	//���b�V�����̉��
	for(i = 0; i < ROCK_TYPE_MAX; i++)
	{
		if(g_pMeshRock[i] != NULL)
		{
			g_pMeshRock[i]->Release();

			delete[] g_pMeshRock;
			g_pMeshRock[i] = NULL;
		}
	}

	//�e�N�X�`�����
	for(i = 0; i < ROCK_TYPE_MAX; i++)
	{
		for(j = 0; j < g_nNumMatRock; j++)
		{
			if(g_pTextureRock[i] != NULL)
			{
				g_pTextureRock[i][j]->Release();
				g_pTextureRock[i][j] = NULL;
			}
		}
		delete[] g_pTextureRock[i];
		g_pTextureRock[i] = NULL;
	}


}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateRock
//�Ԃ�l	:
//����		:	�X�V����
////////////////////////////////////////////////////////////////////////////
void	updateRock	( void )
{
	//�ϐ��錾
	int		i, j;	//loop

	for(i = 0, j = 0; j < g_numRock; i++)
	{
		if(rock[i].use == true)
		{
			if(getPosLen(getCameraPos(), rock[i].pos) > LIMIT_DISTANCE)
			{
				releaseRock(i);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawRock
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	drawRock	( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i, j, k;//�ϐ��錾
	
	for(i = 0, j = 0; j < g_numRock; i++)
	{
		if(rock[i].use == true)
		{
			for(k = 0; k < (int)g_nNumMatRock; k++)
			{
				//�s��ݒ�
				setMatrixRock(pDev, i);

				//�}�e���A���ݒ�
				pDev->SetMaterial(&g_pmeshMatRock[k]);

				//�e�N�X�`���ǂݍ���
				pDev->SetTexture(0, g_pTextureRock[rock[i].type][k]);

				//���b�V���`��
				g_pMeshRock[rock[i].type]->DrawSubset(k);
			}

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixRock
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixRock	( LPDIRECT3DDEVICE9 pDev, int num )
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
	D3DXMatrixScaling(&scale, rock[num].size.x, rock[num].size.y, rock[num].size.z);					//�g�k
	D3DXMatrixRotationYawPitchRoll(&rot, rock[num].angle.x, rock[num].angle.y, rock[num].angle.z);		//��]�s��
	D3DXMatrixTranslation(&move, rock[num].pos.x, rock[num].pos.y, rock[num].pos.z);					//���s�ړ�
	
	//�s�񍇐�
	world	*=	scale;
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getRock
//�Ԃ�l	: 
//����		: ���f���̃|�C���^�擾
//////////////////////////////////////////////////////////////////////////////
ROCK	*getRock	( int num )
{
	if(rock[num].use != false)
	{
		return &rock[num];
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setRock
//�Ԃ�l	: 
//����		: ���f���̃Z�b�g
//////////////////////////////////////////////////////////////////////////////
void	setRock	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 angle, ROCK_TYPE type )
{
	//�ϐ��錾
	int		i;	//loop
	for(i = 0; i < MAX_ROCK; i++)
	{
		if(rock[i].use == false)
		{
			rock[i].pos		=	pos;
			rock[i].size	=	size;
			rock[i].angle	=	angle;
			rock[i].type	=	type;
			rock[i].use		=	true;

			//�R���W�����ݒ�
			rock[i].collisionID = setHitBox(pos, top[type], bottom[type], TYPE_ROCK);

			g_numRock++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: releaseRock
//�Ԃ�l	: 
//����		: �⃊���[�X
//////////////////////////////////////////////////////////////////////////////
void	releaseRock	( int num )
{
	rock[num].use = false;
	g_numRock--;
}