/*****************************************************************************
X�t�@�C���`��[model.cpp]
Aythor	: ���@��
Data	: 2016_11_04
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	MODEL_NAME		"data/model/raft.x"
#define	SPEED			(2)

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	setMatrixModel	( LPDIRECT3DDEVICE9 pDev );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPD3DXMESH			g_pMeshModel = NULL;		//���b�V�����C���^�[�t�F�[�X
LPD3DXBUFFER		g_pBuffMatModel = NULL;		//�}�e���A�����
DWORD				g_nNumMatModel = NULL;		//�}�e���A�����

HIT_VTX				top;						//HitBox�p
HIT_VTX				bottom;
LPDIRECT3DTEXTURE9	g_pTextureModel[4];			//�e�N�X�`�����C���^�[�t�F�[�X
MODEL				model;						//���f���\����

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initModel
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	initModel	( void )
{
	//�ϐ��錾
	HRESULT			hr;
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	D3DXVECTOR3		max, min;
	int		i;

	//�\���̏�����
	model.pos		=	D3DXVECTOR3(0.f, 0.f, 0.f);
	model.angle		=	D3DXVECTOR3(PI, 0.f, 0.f);
	model.MoveVec	=	D3DXVECTOR3(0.f, 0.f, 1.f);
	model.move		=	SPEED;

	hr	=	D3DXLoadMeshFromX(MODEL_NAME,
				D3DXMESH_MANAGED,
				pDev,
				NULL,
				&g_pBuffMatModel,
				NULL,
				&g_nNumMatModel,
				&g_pMeshModel);
	
	if(FAILED(hr))
	{
		MessageBox(NULL, "���f���ǂݍ��݃G���[", "error", MB_OK);
	}

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for(i = 0; i < (int)g_nNumMatModel; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		if(FAILED(D3DXCreateTextureFromFile(pDev, pMat[i].pTextureFilename, &g_pTextureModel[i])))
		{
			MessageBox(NULL, "�G���[", "�G���[", MB_OK);
		}
	}

	//�R���W�������_�ݒ�
	getXfileVtx(MODEL_NAME, &max, &min);
	max.y = 5.f;
	setHitBoxVtx(max, min, &top, &bottom);
	model.collisionID = setHitBox(model.pos, top, bottom, TYPE_PLAYER);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitModel
//�Ԃ�l	: 
//����		: �I��
//////////////////////////////////////////////////////////////////////////////
void	uninitModel	( void )
{
	//���b�V�����̉��
	if(g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A�����̉��
	if(g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateModel
//�Ԃ�l	:
//����		:	�X�V����
////////////////////////////////////////////////////////////////////////////
void	updateModel	( void )
{
	//�ϐ��錾
	int		i;		//loop
	HIT_BOX	player;
	HIT_BOX	work;

	player = getHitBox(model.collisionID);

	for(i = 0; i < MAX_HIT_BOX; i++)
	{
		if(getHitBoxType(i) == TYPE_ROCK)
		{
			work = getHitBox(i);

			if(hitBox(player, work) == true)
			{

			}
		}
	}

	//�R���W�������W�X�V
	setHitBoxPos(model.collisionID, model.pos);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawModel
//�Ԃ�l	: 
//����		: ����������
//////////////////////////////////////////////////////////////////////////////
void	drawModel	( void )
{
	//�ϐ��錾
	D3DMATERIAL9	matDef;
	D3DXMATERIAL	*pMat;
	LPDIRECT3DDEVICE9	pDev = getDevice();
	int		i;//�ϐ��錾
	
	//�s��ݒ�
	setMatrixModel(pDev);

	pDev->GetMaterial(&matDef);
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for(i = 0; i < (int)g_nNumMatModel; i++)
	{
		//�}�e���A���ݒ�
		pDev->SetMaterial(&matDef);

		//�e�N�X�`���ǂݍ���
		pDev->SetTexture(0, g_pTextureModel[i]);

		//���b�V���`��
		g_pMeshModel->DrawSubset(i);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrixModel
//�Ԃ�l	: 
//����		: �s��ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixModel	( LPDIRECT3DDEVICE9 pDev )
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
	D3DXMatrixRotationYawPitchRoll(&rot, model.angle.x, model.angle.y, model.angle.z);		//��]�s��
	D3DXMatrixTranslation(&move, model.pos.x, model.pos.y, model.pos.z);	//���s�ړ�
	
	//�s�񍇐�
	world	*=	rot;
	world	*=	move;

	pDev->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getModel
//�Ԃ�l	: 
//����		: ���f���̃|�C���^�擾
//////////////////////////////////////////////////////////////////////////////
MODEL	*getModel	( void )
{
	return &model;
}