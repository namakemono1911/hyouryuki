/*****************************************************************************
���b�V���E�H�[���`��
Aythor	: ���@��
Data	: 2016_12_10
=============================================================================
Updata

*****************************************************************************/
#include	"main.h"
#include	"input.h"
#include	"meshWall.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	WALL_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	WALL_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_WALL	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferWall	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferWall	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixWall	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshWall [WALL_TEX_MAX]	=	{};	//�e�N�X�`���C���^�[�t�F�C�X

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferWall	=	NULL;	//�o�b�t�@�t�B�[���h
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferWall	=	NULL;	//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X

WALL	wall[MAX_WALL];
int		g_numWall	=	0;			//�t�B�[���h�̑���
char	*WallTexName[]	=			//�e�N�X�`���̖��O
{
	"data/texture/wall000.png",
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initMeshWall
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshWall ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//wall������
	for(i = 0; i < MAX_WALL; i++)
	{
		//�g�p�t���O
		wall[i].use	=	false;
	}

	//��1�Z�b�g
	setMeshWall(
		D3DXVECTOR3(0.f, 0.f, 45.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(0.f,0.f, 0.f),
		WALL_TEX_000);

	//��2
	setMeshWall(
		D3DXVECTOR3(45.f, 0.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(90.f, 0.f, 0.f),
		WALL_TEX_000);
	
	//��3
	setMeshWall(
		D3DXVECTOR3(0.f, 0.f, -45.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(180.f, 0.f, 0.f),
		WALL_TEX_000);
	
	//��2
	setMeshWall(
		D3DXVECTOR3(-45.f, 0.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 0.f),
		D3DXVECTOR3(9, 1, 0),
		D3DXVECTOR3(270.f, 0.f, 0.f),
		WALL_TEX_000);

	makeVertexBufferWall(pDevice);

	for(i = 0; i < WALL_TEX_MAX; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		if(FAILED(D3DXCreateTextureFromFile(pDevice, WallTexName[i], &g_pTextureMeshWall[i])))
		{
			MessageBox(NULL, "�G���[", "�G���[", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMeshWall
//�Ԃ�l	: 
//����		: �t�B�[���h�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMeshWall	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot, WALL_TEX tex )
{
	//�ϐ��錾
	int		i;		//loop

	for(i = 0; i < MAX_WALL; i++)
	{
		//�g���Ă��Ȃ��\���̒T��
		if(wall[i].use	==	false)
		{
			//���l���
			wall[i].use		=	true;
			wall[i].pos		=	pos;
			wall[i].size	=	size;
			wall[i].numTyle	=	numTyle;
			wall[i].rot		=	rot;
			wall[i].tex		=	tex;

			g_numWall++;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getWallPos
//�Ԃ�l	: 
//����		: �ǂ̍��W�擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getWallPos	( int num )
{
	return	wall[num].pos;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitMeshWall
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshWall	( void )
{
	//�ϐ��錾
	int		i;	//loop

	//���_�o�b�t�@�̉��
	if(g_pVtxBufferWall	!=	NULL)
	{
		g_pVtxBufferWall->Release();
		g_pVtxBufferWall	=	NULL;
	}

	//���_�C���f�b�N�X�̉��
	if(g_pIdxBufferWall	!=	NULL)
	{
		g_pIdxBufferWall->Release();
		g_pIdxBufferWall	=	NULL;
	}

	for(i = 0; i < WALL_TEX_MAX; i++)
	{
		//�e�N�X�`���o�b�t�@�̉��
		if(g_pTextureMeshWall[i]	!=	NULL)
		{
			g_pTextureMeshWall[i]->Release();
			g_pTextureMeshWall[i]	=	NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateMeshWall
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateMeshWall	( void )
{

}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawMeshWall
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawMeshWall ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//�K�v�Ȓ��_�C���f�b�N�X��
	int		numPtv;		//�K�v�ȃv���~�e�B�u��
	int		minIdx;		//�ŏ��C���f�b�N�X		

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferWall, 0, sizeof(VERTEX3D));
	
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetIndices(g_pIdxBufferWall);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			//�K�v�ȏ��v�Z
			numVtx	=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y * 2));
			numVtx	+=	(int)((wall[i].numTyle.y - 1) * 2);

			numPtv	=	(int)(wall[i].numTyle.x * wall[i].numTyle.y * 2 + (wall[i].numTyle.y - 1) * 4);

			//�e�N�X�`���Z�b�g
			pDevice->SetTexture(0,g_pTextureMeshWall[wall[i].tex]);

			//���[���h�}�g���b�N�X�ݒ�
			setMatrixWall(pDevice, i);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//�`����@
				0,								//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
				minIdx,							//�ŏ��̒��_�C���f�b�N�X
				numVtx,							//���_��
				0,								//�X�^�[�g�C���f�b�N�X
				numPtv);						//�v���~�e�B�u��

			//�ϐ�������
			minIdx	+=	numVtx;
			numVtx	=	0;
			numPtv	=	0;
			j++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixWall
//�Ԃ�l	: 
//����		: ���[���h�}�g���b�N�X�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixWall	( LPDIRECT3DDEVICE9 pDevice, int num )
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
		wall[num].pos.x,
		wall[num].pos.y,
		wall[num].pos.z);

	//��]
	radian	=	D3DXVECTOR3(
		wall[num].rot.x * PI / 180.f,
		wall[num].rot.y * PI / 180.f,
		wall[num].rot.z * PI / 180.f);

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
//�֐���	: makeVertexMeshWall
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferWall	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;		//���z�A�h���X�p�|�C���^
	D3DXVECTOR3	pos;		//�`��ʒu
	int			i, j;		//���[�v
	int			x, y;		//x��y�̃��[�v
	int			num;		//�K�v�Ȓ��_��
	D3DXVECTOR3	fullSize;	//�t�B�[���h�S�̂̃T�C�Y

	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			num	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y + 1));
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
			&g_pVtxBufferWall,						//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// ���_���o�^
	//=============================
	g_pVtxBufferWall->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			//�S�̂̃T�C�Y�v�Z
			fullSize	=	D3DXVECTOR3(
				wall[i].numTyle.x * wall[i].size.x,
				wall[i].numTyle.y * wall[i].size.y,
				0.f);

			for(y = 0; y < wall[i].numTyle.y + 1; y++)
			{
				for(x = 0; x < wall[i].numTyle.x + 1; x++)
				{
					//���_
					pVtx[0].pos	=	D3DXVECTOR3(
						(wall[i].size.x * x) - (fullSize.x * 0.5f),
						fullSize.y - (wall[i].size.y * y),
						0.f);

					//���_�F
					pVtx[0].color	=	D3DCOLOR_RGBA(255, 255, 255, 255);

					//�@���x�N�g��
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, -0.5f);

					//�e�N�X�`�����W
					pVtx[0].tex		=	D3DXVECTOR2((float)x, (float)y);

					//�|�C���^�ړ�
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferWall->Unlock();

	//=============================
	// �C���f�b�N�X�쐬
	//=============================
	makeIndexBufferWall(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeIndexBuffer
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferWall	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	HRESULT		hr;		//�C���f�b�N�X�ݒ�̊m�F
	WORD		*pIdx;	//�C���f�b�N�X�ԍ�
	int			i, j;	//loop
	int			x, y;	//x��z��loop
	int			num;	//�K�v�ȃC���f�b�N�X��		
	int			numIdx;	//n�ڂ̃C���f�b�N�X��
	
	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			num	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y * 2));
			num	+=	(int)((wall[i].numTyle.y - 1) * 2);
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
		&g_pIdxBufferWall,			//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X
		NULL);

	//=============================
	// �C���f�b�N�X�o�^
	//=============================
	g_pIdxBufferWall->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, numIdx = 0; j < g_numWall; i++)
	{
		if(wall[i].use	==	true)
		{
			for(y = 0; y < wall[i].numTyle.y; y++)
			{
				for(x = 0; x < wall[i].numTyle.x + 1; x++)
				{
					//�C���f�b�N�X�o�^
					pIdx[0]	=	(WORD)(wall[i].numTyle.x + 1) * (y + 1) + x + numIdx;
					pIdx[1]	=	(WORD)(wall[i].numTyle.x + 1) * y + x + numIdx;

					//�|�C���^�ړ�
					pIdx	+=	2;
				}

				//�k�ރ|���S��
				if(y	<	wall[i].numTyle.y - 1)
				{
					//�C���f�b�N�X�o�^
					x--;
					pIdx[0]	=	(WORD)(wall[i].numTyle.x + 1) * y + x + numIdx;
					pIdx[1]	=	(WORD)(wall[i].numTyle.x + 1) * (y + 1) + x + numIdx + 1;

					//�|�C���^�ړ�
					pIdx	+=	2;
				}
			}

			//�����n�܂�C���f�b�N�X��
			numIdx	+=	(int)((wall[i].numTyle.x + 1) * (wall[i].numTyle.y + 1));

			j++;
		}
	}

	g_pIdxBufferWall->Unlock();

	return	S_OK;
}