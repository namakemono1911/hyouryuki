/*****************************************************************************
���b�V���t�B�[���h�`��
Aythor	: ���@��
Data	: 2016_11_12
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	"input.h"
#include	"meshField.h"
#include	"render.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	SIZE_WIDTH		(10)
#define	SIZE_HEIGHT		(10)

#define	TILE_WIDTH		(3)
#define	TILE_HEIGHT		(3)
#define	NUM_TILE		(TILE_WIDTH * TILE_HEIGHT)

#define	FIELD_WIDTH		(SIZE_WIDTH * TILE_WIDTH)
#define	FIELD_HEIGHT	(SIZE_HEIGHT * TILE_HEIGHT)

#define	NUM_VERTEX_FIELD	((NUM_TILE * 4) + (TILE_HEIGHT - 1) * 2)

#define	NUM_INDEX		((TILE_WIDTH + 1) * (TILE_HEIGHT + 1))

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferField	( LPDIRECT3DDEVICE9	pDevice );
HRESULT	makeIndexBufferField	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixField	( LPDIRECT3DDEVICE9 pDevice, int num );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9	g_pTextureMeshField [FIELD_TEX_MAX]	=	{};	//�e�N�X�`���C���^�[�t�F�C�X

LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferField	=	NULL;	//�o�b�t�@�t�B�[���h
LPDIRECT3DINDEXBUFFER9	g_pIdxBufferField	=	NULL;	//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X

FIELD	field[MAX_FIELD];
int		g_numField	=	0;			//�t�B�[���h�̑���
char	*FieldTexName[]	=			//�e�N�X�`���̖��O
{
	"data/texture/field000.jpg",
	"data/texture/field001.jpg",
	"data/texture/field002.jpg",
	"data/texture/field003.jpg"
};

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initMeshField
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
HRESULT initMeshField ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	//field������
	for(i = 0; i < MAX_FIELD; i++)
	{
		//�g�p�t���O
		field[i].use	=	false;
		field[i].id		=	MAX_FIELD + 1;
	}

	for(i = 0; i < FIELD_TEX_MAX; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		if(FAILED(D3DXCreateTextureFromFile(pDevice, FieldTexName[i], &g_pTextureMeshField[i])))
		{
			MessageBox(NULL, "�G���[", "field�G���[", MB_OK);
			return	E_FAIL;
		}
	}

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMeshField
//�Ԃ�l	: 
//����		: �t�B�[���h�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMeshField	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 numTyle, D3DXVECTOR3 rot,D3DXCOLOR color ,D3DXVECTOR2 texMove, FIELD_TEX tex, int id )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i;		//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use	==	false)
		{
			//���l���
			field[i].use		=	true;
			field[i].pos		=	pos;
			field[i].size		=	size;
			field[i].numTyle	=	numTyle;
			field[i].rot		=	rot;
			field[i].color		=	color;
			field[i].texPos		=	D3DXVECTOR2(0.f, 0.f);
			field[i].texMove	=	texMove;
			field[i].tex		=	tex;
			field[i].id			=	id;

			g_numField++;

			makeVertexBufferField(pDevice);
			break;
		}
	
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitMeshField
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitMeshField	( void )
{
	//�ϐ��錾
	int		i;	//loop

	//���_�o�b�t�@�̉��
	if(g_pVtxBufferField	!=	NULL)
	{
		g_pVtxBufferField->Release();
		g_pVtxBufferField	=	NULL;
	}

	//���_�C���f�b�N�X�̉��
	if(g_pIdxBufferField	!=	NULL)
	{
		g_pIdxBufferField->Release();
		g_pIdxBufferField	=	NULL;
	}

	for(i = 0; i < FIELD_TEX_MAX; i++)
	{
		//�e�N�X�`���o�b�t�@�̉��
		if(g_pTextureMeshField[i]	!=	NULL)
		{
			g_pTextureMeshField[i]->Release();
			g_pTextureMeshField[i]	=	NULL;
		}
	}

	g_numField = 0;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateMeshField
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateMeshField	( void )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;	//���_���|�C���^
	int			i, j;		//���[�v
	int			x, z;		//x��z�̃��[�v
	int			num;		//�K�v�Ȓ��_��
	D3DXVECTOR3	fullSize;	//�t�B�[���h�S�̂̃T�C�Y

	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z + 1));
			j++;
		}
	}

	//=============================
	// �e�N�X�`���A�j��
	//=============================
	for(i = 0; i < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			field[i].texPos	=	D3DXVECTOR2(
				field[i].texPos.x + field[i].texMove.x,
				field[i].texPos.y + field[i].texMove.y);
		}
	}

	//=============================
	// ���_�o�b�t�@�o�^
	//=============================
	g_pVtxBufferField->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//�S�̂̃T�C�Y�v�Z
			fullSize	=	D3DXVECTOR3(
				field[i].numTyle.x * field[i].size.x,
				0.f,
				field[i].numTyle.z * field[i].size.z);

			for(z = 0; z < field[i].numTyle.z + 1; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//���_
					pVtx[0].pos	=	D3DXVECTOR3(
						(field[i].size.x * x) - (fullSize.x * 0.5f),
						0.f,
						-(field[i].size.z * z) + (fullSize.z * 0.5f));

					//���_�F
					pVtx[0].color	=	field[i].color;

					//�@���x�N�g��
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, 0.f);

					//�e�N�X�`�����W
					pVtx[0].tex		=	D3DXVECTOR2(field[i].texPos.x + (float)x, field[i].texPos.y + (float)z);

					//�|�C���^�ړ�
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferField->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawMeshField
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawMeshField ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop
	int		numVtx;		//�K�v�Ȓ��_�C���f�b�N�X��
	int		numPtv;		//�K�v�ȃv���~�e�B�u��
	int		minIdx;		//�ŏ��C���f�b�N�X��
	int		startVtxIdx;	//�X�^�[�g�o�[�e�N�X�C���f�b�N�X

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferField, 0, sizeof(VERTEX3D));
	
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetIndices(g_pIdxBufferField);

	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	for(i = 0, j = 0, numVtx = 0, numPtv = 0, minIdx = 0, startVtxIdx = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//�K�v�ȏ��v�Z
			numVtx	=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z * 2));
			numVtx	+=	(int)((field[i].numTyle.z - 1) * 2);

			numPtv	=	(int)(field[i].numTyle.x * field[i].numTyle.z * 2 + (field[i].numTyle.z - 1) * 4);

			//�e�N�X�`���Z�b�g
			pDevice->SetTexture(0,g_pTextureMeshField[field[i].tex]);

			//���[���h�}�g���b�N�X�ݒ�
			setMatrixField(pDevice, i);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,			//�`����@
				startVtxIdx,					//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
				minIdx,							//�ŏ��̒��_�C���f�b�N�X
				numVtx,							//���_��
				0,								//�X�^�[�g�C���f�b�N�X
				numPtv);						//�v���~�e�B�u��

			//�ϐ�������
			minIdx		=	numVtx;		//�ŏ��C���f�b�N�X���
			//startVtxIdx	=	numVtx;
			startVtxIdx	+=	((int)field[i].numTyle.x + 1) * ((int)field[i].numTyle.z + 1);	//���̃X�^�[�g�C���f�b�N�X
			//startVtxIdx	-=	minIdx;
			numVtx		=	0;			//���_��������
			numPtv		=	0;			//�v���~�e�B�u��������
			j++;
		}
	}

	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixField
//�Ԃ�l	: 
//����		: ���[���h�}�g���b�N�X�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixField	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;		//���[���h�s��
	D3DXMATRIX	move;		//���s�ړ��s��
	D3DXMATRIX	rot;		//��]�s��

	//�s�񏉊���
	D3DXMatrixIdentity(&world);

	//���s�ړ�
	D3DXMatrixTranslation(&move,	
		field[num].pos.x,
		field[num].pos.y,
		field[num].pos.z);

	//��]
	D3DXMatrixRotationYawPitchRoll(&rot,
		field[num].rot.x,
		field[num].rot.y,
		field[num].rot.z);	//��]�s��

	//�s�񍇐�
	world	*=	rot;		//��]
	world	*=	move;		//���s�ړ�

	pDevice->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexMeshField
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexBufferField	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;		//���z�A�h���X�p�|�C���^
	D3DXVECTOR3	pos;		//�`��ʒu
	int			i, j;		//���[�v
	int			x, z;		//x��z�̃��[�v
	int			num;		//�K�v�Ȓ��_��
	D3DXVECTOR3	fullSize;	//�t�B�[���h�S�̂̃T�C�Y

	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z + 1));
			j++;
		}
	}

	if(g_pVtxBufferField != NULL)
	{
		g_pVtxBufferField->Release();
		g_pVtxBufferField = NULL;
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
			&g_pVtxBufferField,						//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	//=============================
	// ���_���o�^
	//=============================
	g_pVtxBufferField->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			//�S�̂̃T�C�Y�v�Z
			fullSize	=	D3DXVECTOR3(
				field[i].numTyle.x * field[i].size.x,
				0.f,
				field[i].numTyle.z * field[i].size.z);

			for(z = 0; z < field[i].numTyle.z + 1; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//���_
					pVtx[0].pos	=	D3DXVECTOR3(
						(field[i].size.x * x) - (fullSize.x * 0.5f),
						0.f,
						-(field[i].size.z * z) + (fullSize.z * 0.5f));

					//���_�F
					pVtx[0].color	=	field[i].color;

					//�@���x�N�g��
					pVtx[0].nor		=	D3DXVECTOR3(0.f, 1.f, 0.f);

					//�e�N�X�`�����W
					pVtx[0].tex		=	D3DXVECTOR2(field[i].texPos.x + (float)x, field[i].texPos.y + (float)z);

					//�|�C���^�ړ�
					pVtx++;
				}
			}

			j++;
		}
	}

	g_pVtxBufferField->Unlock();

	//=============================
	// �C���f�b�N�X�쐬
	//=============================
	makeIndexBufferField(pDevice);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeIndexBuffer
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeIndexBufferField	( LPDIRECT3DDEVICE9	pDevice )
{
	//�ϐ��錾
	HRESULT		hr;		//�C���f�b�N�X�ݒ�̊m�F
	WORD		*pIdx;	//�C���f�b�N�X�ԍ�
	int			i, j;	//loop
	int			x, z;	//x��z��loop
	int			num;	//�K�v�ȃC���f�b�N�X��		
	int			numIdx;	//n�ڂ̃C���f�b�N�X��
	
	//=============================
	// �����_�K�v��
	//=============================
	for(i = 0, j = 0, num = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			num	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.z * 2));
			num	+=	(int)((field[i].numTyle.z - 1) * 2);
			j++;
		}
	}

	if(g_pIdxBufferField != NULL)
	{
		g_pIdxBufferField->Release();
		g_pIdxBufferField = NULL;
	}

	//=============================
	// �C���f�b�N�X�쐬
	//=============================
	hr	=	pDevice->CreateIndexBuffer(
		sizeof(WORD) *  num,	//�K�v�ȃC���f�b�N�X�T�C�Y
		D3DUSAGE_WRITEONLY,			//�g�p�p�r�t���O
		D3DFMT_INDEX16,				//�C���f�b�N�X�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,			//�������̊Ǘ����@�i���C�����[�h�j
		&g_pIdxBufferField,			//�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X
		NULL);

	//=============================
	// �C���f�b�N�X�o�^
	//=============================
	g_pIdxBufferField->Lock(0, 0, (void**)&pIdx, 0);

	for(i = 0, j = 0, numIdx = 0; j < g_numField; i++)
	{
		if(field[i].use	==	true)
		{
			for(z = 0; z < field[i].numTyle.z; z++)
			{
				for(x = 0; x < field[i].numTyle.x + 1; x++)
				{
					//�C���f�b�N�X�o�^
					pIdx[0]	=	(WORD)((field[i].numTyle.x + 1) * (z + 1) + x + numIdx);
					pIdx[1]	=	(WORD)((field[i].numTyle.x + 1) * z + x + numIdx);

					//�|�C���^�ړ�
					pIdx	+=	2;
				}

				//�k�ރ|���S��
				if(z	<	field[i].numTyle.z - 1)
				{
					//�C���f�b�N�X�o�^
					x--;
					pIdx[0]	=	(WORD)((field[i].numTyle.x + 1) * z + x + numIdx);
					pIdx[1]	=	(WORD)((field[i].numTyle.x + 1) * (z + 1) + x + numIdx + 1);

					//�|�C���^�ړ�
					pIdx	+=	2;
				}
			}

			//�����n�܂�C���f�b�N�X��
			numIdx	+=	(int)((field[i].numTyle.x + 1) * (field[i].numTyle.y + 1));

			j++;
		}
	}

	g_pIdxBufferField->Unlock();

	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getField
//�Ԃ�l	: 
//����		: �t�B�[���h�̃|�C���^
//////////////////////////////////////////////////////////////////////////////
FIELD	*getField	( int id )
{
	//�ϐ��錾
	int		i;	//loop
	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return &field[i];
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getFieldPos
//�Ԃ�l	: 
//����		: ���W�擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getFieldPos	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].pos;
		}
	}
	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getFieldSize
//�Ԃ�l	: 
//����		: �T�C�Y�擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getFieldSize	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].size;
		}
	}
	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getFieldColor
//�Ԃ�l	: 
//����		: ���_�F�擾
//////////////////////////////////////////////////////////////////////////////
D3DXCOLOR	getFieldColor	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].color;
		}
	}
	return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getFieldTexMove
//�Ԃ�l	: 
//����		: �e�N�X�`���ړ��ʎ擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR2	getFieldTexMove	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].texMove;
		}
	}
	return D3DXVECTOR2(0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getFieldTexMove
//�Ԃ�l	: 
//����		: �e�N�X�`���^�C�v�擾
//////////////////////////////////////////////////////////////////////////////
FIELD_TEX	getFieldTex	( int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			return field[i].tex;
		}
	}
	return FIELD_TEX_NULL;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setFieldPos
//�Ԃ�l	: 
//����		: �T�C�Y�擾
//////////////////////////////////////////////////////////////////////////////
void	setFieldPos	( D3DXVECTOR3 pos, int id )
{
	//�ϐ��錾
	int		i;	//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			field[i].pos = pos;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: releaseField
//�Ԃ�l	: 
//����		: �t�B�[���h�����[�X
//////////////////////////////////////////////////////////////////////////////
void	releaseField	( int id )
{
	//�ϐ��錾
	int		i;		//loop

	for(i = 0; i < MAX_FIELD; i++)
	{
		if(field[i].use == true && field[i].id == id)
		{
			field[i].use = false;

			g_numField--;
			return;
		}
	}
}