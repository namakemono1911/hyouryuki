/*****************************************************************************
���b�V���h�[���`��
Aythor	: ���@��
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"camera.h"
#include	"meshDome.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	DOME_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	DOME_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_DOME	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferDome	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferDome	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixDome	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshDome [DOME_TEX_MAX]	=	{};	//�e�N�X�`���C���^�[�t�F�C�X

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferDome	=	NULL;	//�o�b�t�@�t�B�[���h
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferDome	=	NULL;	//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X

DOME	dome[MAX_DOME];
int		g_numDome	=	0;			//�t�B�[���h�̑���
char	*DomeTexName[]	=			//�e�N�X�`���̖��O
{
	"data/texture/sky001.jpg"		//��e�N�X�`��2
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initMeshDome
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshDome ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//dome������
	for(i = 0; i < MAX_DOME; i++)
	{
		//�g�p�t���O
		dome[i].use	=	false;
	}

	makeVertexBufferDome(pDevice);

	for(i = 0; i < DOME_TEX_MAX; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		if(FAILED(D3DXCreateTextureFromFile(pDevice, DomeTexName[i], &g_pTextureMeshDome[i])))
		{
			MessageBox(NULL, "�G���[", "�G���[", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMeshDome
//�Ԃ�l	: 
//����		: �t�B�[���h�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMeshDome	( D3DXVECTOR3 pos, float len, D3DXVECTOR2 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color, DOME_TEX tex, int id )
{
	//�ϐ��錾
	int		i;		//loop

	for(i = 0; i < MAX_DOME; i++)
	{
		//�g���Ă��Ȃ��\���̒T��
		if(dome[i].use	==	false)
		{
			//���l���
			dome[i].use		=	true;
			dome[i].pos		=	pos;
			dome[i].len		=	len;
			dome[i].numTyle	=	numTyle;
			dome[i].rot		=	rot;
			dome[i].color	=	color;
			dome[i].tex		=	tex;
			dome[i].id		=	id;

			g_numDome++;

			makeVertexBufferDome(getDevice());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getDomePos
//�Ԃ�l	: 
//����		: �ǂ̍��W�擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getDomePos	( int num )
{
	return	dome[num].pos;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitMeshDome
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshDome	( void )
{
	//�ϐ��錾
	int		i;	//loop

	//���_�o�b�t�@�̉��
	if(g_pVtxBufferDome	!=	NULL)
	{
		g_pVtxBufferDome->Release();
		g_pVtxBufferDome	=	NULL;
	}

	//���_�C���f�b�N�X�̉��
	if(g_pIdxBufferDome	!=	NULL)
	{
		g_pIdxBufferDome->Release();
		g_pIdxBufferDome	=	NULL;
	}

	//�e�N�X�`���o�b�t�@�̉��
	for(i = 0; i < DOME_TEX_MAX; i++)
	{
		if(g_pTextureMeshDome[i]	!=	NULL)
		{
			g_pTextureMeshDome[i]->Release();
			g_pTextureMeshDome[i]	=	NULL;
		}
	}

	//�����I��
	g_numDome	=	0;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateMeshDome
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateMeshDome	( void )
{
	//�ϐ��錾
	int		i, j;		//loop
	
	for(i = 0, j = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			dome[i].pos		=	getCameraPos();
			dome[i].pos.y	=	0.f;
			dome[i].rot.x	+=	0.01f;
			dome[i].rot.x	=	fmod(dome[i].rot.x, 360);

			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawMeshDome
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawMeshDome ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//�K�v�Ȓ��_�C���f�b�N�X��
	int		numPtv;		//�K�v�ȃv���~�e�B�u��
	int		minIdx;		//�ŏ��C���f�b�N�X
	int		startIdx;	//�X�^�[�g�C���f�b�N�X

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferDome, 0, sizeof(VERTEX3D));
	
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetIndices(g_pIdxBufferDome);
	
	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0, startIdx = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			//�K�v�ȏ��v�Z
			numVtx	+=	5 * (int)dome[i].numTyle.y;
			numVtx	+=	(int)(dome[i].numTyle.y + 1) * ((int)dome[i].numTyle.x - 1) * 2;

			numPtv	=	(int)dome[i].numTyle.y + ((int)dome[i].numTyle.y - 1) * 4;
			numPtv	+=	((int)dome[i].numTyle.x - 1) * ((int)dome[i].numTyle.y + 1) * 2;

			//�e�N�X�`���Z�b�g
			pDevice->SetTexture(0,g_pTextureMeshDome[dome[i].tex]);
			
			//���[���h�}�g���b�N�X�ݒ�
			setMatrixDome(pDevice, i);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//�`����@
				startIdx,						//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
				minIdx,							//�ŏ��̒��_�C���f�b�N�X
				numVtx,							//���_��
				0,								//�X�^�[�g�C���f�b�N�X
				numPtv);						//�v���~�e�B�u��

			//�ϐ�������
			minIdx		+=	numVtx;
			startIdx	=	(int)dome[i].numTyle.x * ((int)dome[i].numTyle.y + 1) + 1;
			numVtx		=	0;
			numPtv		=	0;
			j++;
		}
	}

	//���C�g�̉e��on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixDome
//�Ԃ�l	: 
//����		: ���[���h�}�g���b�N�X�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixDome	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;		//���[���h�s��
	D3DXMATRIX	move;		//���s�ړ��s��
	D3DXMATRIX	rot;		//��]�s��
	D3DXVECTOR3	radian;		//�x���ϊ�

	//�s�񏉊���
	D3DXMatrixIdentity(&world);

	//���s�ړ�
	D3DXMatrixTranslation(&move,	
		dome[num].pos.x,
		dome[num].pos.y,
		dome[num].pos.z);

	//��]
	radian	=	D3DXVECTOR3(
		dome[num].rot.x * PI / 180.f,
		dome[num].rot.y * PI / 180.f,
		dome[num].rot.z * PI / 180.f);

	D3DXMatrixRotationYawPitchRoll(&rot,
		radian.x,
		radian.y,
		radian.z);	//��]�s��

	//�s�񍇐�
	world	*=	rot;		//��]
	world	*=	move;		//���s�ړ��s��

	pDevice->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexMeshDome
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferDome	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;		//���z�A�h���X�p�|�C���^
	float		posY;		//�`��ʒu
	float		len;		//���a
	float		rot;		//�p�x
	int			i, j;		//���[�v
	int			x, y;		//x��y�̃��[�v
	int			num;		//�K�v�Ȓ��_��
	D3DXVECTOR3	fullSize;	//�t�B�[���h�S�̂̃T�C�Y

	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			num	+=	(int)dome[i].numTyle.x * ((int)dome[i].numTyle.y + 1) + 1;
			j++;
		}
	}

	//=============================
	// ���_�o�b�t�@�쐬
	//=============================
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * num,					//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_3D,							//�Ȃ񂩐ݒ�
			D3DPOOL_MANAGED,						//�������Ǘ����@
			&g_pVtxBufferDome,						//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// ���_���o�^
	//=============================
	g_pVtxBufferDome->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, rot = 90, j = 0; j < g_numDome; i++)
	{
		if(dome[i].use	== true)
		{
			//�h�[���̒��_
			pVtx[0].pos		=	D3DXVECTOR3(
				0.f,
				dome[i].len,
				0.f);

			//���_�F
			pVtx[0].color	= dome[i].color;

			//�e�N�X�`�����W
			pVtx[0].tex		= D3DXVECTOR2(0.f, 0.f);

			//�|�C���^�ړ�
			pVtx++;

			for(y = 0; y < dome[i].numTyle.x; y++)
			{
				//�p�x�Z�o
				rot	=	90 * ((dome[i].numTyle.x - (y + 1)) / dome[i].numTyle.x);

				//Y���W�m��
				posY	= dome[i].len * sin(rot * (PI / 180));
				len		= dome[i].len * cos(rot * (PI / 180));

				for(x = 0; x < dome[i].numTyle.y + 1; x++)
				{
					//x,z�̊p�x
					rot	=	360 * (x / dome[i].numTyle.y);

					//���W
					pVtx[0].pos	=	D3DXVECTOR3(
						len * cos(rot * (PI / 180)),
						posY,
						len * sin(rot * (PI / 180)));

					//���_�F
					pVtx[0].color	=	dome[i].color;

					//�e�N�X�`�����W
					pVtx[0].tex		=	D3DXVECTOR2(
						(1.f / dome[i].numTyle.y) * x,
						(1.f / dome[i].numTyle.x) * (y + 1));

					//�|�C���^�ړ�
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferDome->Unlock();

	//=============================
	// �C���f�b�N�X�쐬
	//=============================
	makeIndexBufferDome(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeIndexBuffer
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferDome	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	HRESULT		hr;		//�C���f�b�N�X�ݒ�̊m�F
	WORD		*pIdx;	//�C���f�b�N�X�ԍ�
	int			i, j, k;//loop
	int			x, y;	//x��z��loop
	int			num;	//�K�v�ȃC���f�b�N�X��
	int			minIdx;	//�ŏ��C���f�b�N�X
	
	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			num	+=	5 * (int)dome[i].numTyle.y;
			num	+=	(int)(dome[i].numTyle.y + 1) * ((int)dome[i].numTyle.x - 1) * 2;
			j++;
		}
	}

	//=============================
	// �C���f�b�N�X�쐬
	//=============================
	hr	=	pDevice->CreateIndexBuffer(
		sizeof(WORD) *  num,	//�K�v�ȃC���f�b�N�X�T�C�Y
		D3DUSAGE_WRITEONLY,			//�g�p�p�r�t���O
		D3DFMT_INDEX16,				//�C���f�b�N�X�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,			//�������̊Ǘ����@�i���C�����[�h�j
		&g_pIdxBufferDome,			//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X
		NULL);

	//=============================
	// �C���f�b�N�X�o�^
	//=============================
	g_pIdxBufferDome->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, minIdx = 0; j < g_numDome; i++)
	{
		if(dome[i].use	==	true)
		{
			//�h�[����1�ԏ�
			for(k = 0; k < dome[i].numTyle.y; k++)
			{
				pIdx[0]	=	minIdx;
				pIdx[1]	=	k + 1 + k % 2;
				pIdx[2]	=	k + 2 - k % 2;
				pIdx	+=	3;

				//�k�ރ|���S��
				if(k < dome[i].numTyle.y - 1)
				{
					pIdx[3]	=	pIdx[2];
					pIdx[0]	=	minIdx;
					pIdx	+=	2;
				}
			}

			//�������牺
			for(y = 0; y < dome[i].numTyle.x - 1; y++)
			{
				for(x = 0; x < dome[i].numTyle.y + 1; x++)
				{
					pIdx[0]	=	(WORD)((dome[i].numTyle.y + 1) * y + x + 1);
					pIdx[1]	=	(WORD)((dome[i].numTyle.y + 1) * (y + 1) + x + 1);
					pIdx	+=	2;
				}
			}

			minIdx	=	(int)dome[i].numTyle.y * 3 + ((int)dome[i].numTyle.y - 1) * 2;
			minIdx	=	((int)dome[i].numTyle.x - 1) + ((int)dome[i].numTyle.y + 1) * 2;
			j++;
		}
	}
	
	g_pIdxBufferDome->Unlock();

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setDomePos
//�Ԃ�l	: 
//����		: �h�[�����W�Z�b�g
//////////////////////////////////////////////////////////////////////////////
void	setDomePos	( int num, D3DXVECTOR3 pos )
{
	dome[num].pos = pos;
}