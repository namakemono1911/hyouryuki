/*****************************************************************************
render.cpp
Aythor	: ���@��
Data	: 2016_07_05
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "render.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: initDevice
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void normal2D ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	////�e�N�X�`���̐ݒ�
 //   // �e�N�X�`���̐ݒ�
 //   pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
 //   pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
 //   pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

 //   // �A���t�@�E�u�����f�B���O���s��
 //   pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
 //   // ���ߏ������s��
 //   pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
 //   // �������������s��
 //   pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	/*pDevice->SetTextureStageState(0,	D3DTSS_COLORARG1	, D3DTA_TEXTURE	);
	pDevice->SetTextureStageState(0,	D3DTSS_COLOROP	, D3DTOP_BLENDTEXTUREALPHA	);
	pDevice->SetTextureStageState(0,	D3DTSS_COLORARG2	, D3DTA_DIFFUSE	);
	pDevice->SetTextureStageState(0,	D3DTSS_ALPHAOP	, D3DTOP_SELECTARG2	);
	pDevice->SetTextureStageState(0,	D3DTSS_ALPHAARG2	, D3DTA_DIFFUSE	);*/

	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1 , D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2 , D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: multiDevice
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void multiDevice ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//g_PD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: addDevice
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void addDevice ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�e�N�X�`���̐F�ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////
// �ϐ���	: subDevice
// �Ԃ�l	:
// ����		: �f�o�C�X�̏�����
//////////////////////////////////////////////////////////////////////////////
void subDevice ( LPDIRECT3DDEVICE9 pDevice )
{
	//���Z���@�ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLEND_ONE);

	//�������̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//������̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initPolygon
//����		: *polygon	������������POLYGON�ϐ�
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void initPolygon ( POLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size )
{
	polygon->pos = pos;
	polygon->size = size;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	initRotPolygon
//
//����		:	*polygon	������������POLYGON�ϐ�
//				width		�ݒ肵�����摜�̕�
//				height		�ݒ肵�����摜�̍���
//
//�Ԃ�l	:	
//����		:	 ��]�|���S��������
//////////////////////////////////////////////////////////////////////////////
void initRotPolygon ( ROTPOLYGON *polygon, D3DXVECTOR3 pos, D3DXVECTOR3 size )
{
	//���S���W������
	polygon->conf.pos = pos;

	//�T�C�Y������
	polygon->conf.size = size;

	//�|���S���̊p�x
	polygon->rot = 0.0f;

	//�Ίp���̒���
	polygon->length = sqrt(size.x * size.x + size.y * size.y) * 0.5f;

	//�|���S���̑Ίp���̊p�x
	polygon->angle = atan2(size.y, size.x);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initTexture
//����		: *tex		�ݒ肵�����e�N�X�`�����
//			  pos		UV�J�n�ʒu
//			  size		�e�N�X�`��1���̃T�C�Y
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void initTexture ( TEXTURE	*tex, D3DXVECTOR2 pos, D3DXVECTOR2 size )
{
	tex->pos = pos;
	tex->size = size;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setTexture
//
//����		:	*pVtx	�ݒ肵�����摜�̃o�b�t�@�A�h���X
//				texture	�ݒ肵����TEXTURE�^�̕ϐ�
//
//�Ԃ�l	: 
//����		:	�|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void setTexture ( VERTEX3D *pVtx, TEXTURE texture )
{
	//����UV
	pVtx[0].tex = D3DXVECTOR2(
		texture.pos.x,
		texture.pos.y);

	//�E��UV
	pVtx[1].tex = D3DXVECTOR2(
		texture.pos.x + texture.size.x,
		texture.pos.y);

	//����UV
	pVtx[2].tex = D3DXVECTOR2(
		texture.pos.x,
		texture.pos.y + texture.size.y);

	//�E��UV
	pVtx[3].tex = D3DXVECTOR2(
		texture.pos.x + texture.size.x,
		texture.pos.y + texture.size.y);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setBuffer
//
//����		:	*pVtx	�ݒ肵�����摜�̃o�b�t�@�A�h���X
//				polygon	�o�b�t�@�ɐݒ肵����POLYGON�^�ϐ�
//
//�Ԃ�l	: 
//����		: �o�b�t�@�ɍ��W��ݒ�
//////////////////////////////////////////////////////////////////////////////
void setBuffer ( VERTEX3D *pVtx, POLYGON polygon )
{
	//������W
	pVtx[0].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);
	
	//�E����W
	pVtx[1].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//�������W
	pVtx[2].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
	
	//�E�����W
	pVtx[3].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setListBuffer
//
//����		:	*pVtx	�ݒ肵�����摜�̃o�b�t�@�A�h���X
//				polygon	�o�b�t�@�ɐݒ肵����POLYGON�^�ϐ�
//
//�Ԃ�l	: 
//����		: �o�b�t�@�Ƀg���C�A���O�����X�g�̍��W��ݒ�
//////////////////////////////////////////////////////////////////////////////
void setListBuffer ( VERTEX3D *pVtx, POLYGON polygon )
{
	//������W
	pVtx[0].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);
	
	//�E����W
	pVtx[1].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//�������W
	pVtx[2].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);

	//�E����W
	pVtx[3].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y - (polygon.size.y * 0.5f),
		0.f);

	//�E�����W
	pVtx[4].pos = D3DXVECTOR3(
		polygon.pos.x + (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);

	//�������W
	pVtx[5].pos = D3DXVECTOR3(
		polygon.pos.x - (polygon.size.x * 0.5f),
		polygon.pos.y + (polygon.size.y * 0.5f),
		0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setRotBuffer
//
//����		:	*pVtx	�ݒ肵�����摜�̃o�b�t�@�A�h���X
//				polygon	�o�b�t�@�ɐݒ肵������]�摜
//
//�Ԃ�l	: 
//����		: �o�b�t�@�ɉ�]���W��ݒ�
//////////////////////////////////////////////////////////////////////////////
void setRotBuffer ( VERTEX3D *pVtx, ROTPOLYGON polygon )
{
	//������W
	pVtx[0].pos = D3DXVECTOR3(
		-cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
		-sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//�E����W
	pVtx[1].pos = D3DXVECTOR3(
		cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
		sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//�������W
	pVtx[2].pos = D3DXVECTOR3(
		-cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
		-sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);

	//�E�����W
	pVtx[3].pos = D3DXVECTOR3(
		cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
		sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
		0.0f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getRotPos
//
//����		:	polygon	�擾��������]�|���S��
//
//�Ԃ�l	: 
//����		: ��]�|���S���̍��W�擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getRotPos	( ROTPOLYGON polygon, int pos )
{
	switch(pos)
	{
	case TOP_LEFT:			//����̍��W
		{
			return D3DXVECTOR3(
				-cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
				-sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case TOP_RIGHT:			//�E��̍��W
		{
			return D3DXVECTOR3(
				cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
				sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case BOT_RIGHT:			//�E���̍��W
		{
			return D3DXVECTOR3(
				cos(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.x,
				sin(polygon.rot + polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}

	case BOT_LEFT:			//�����̍��W
		{
			return D3DXVECTOR3(
				-cos(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.x,
				-sin(polygon.rot - polygon.angle) * polygon.length + polygon.conf.pos.y,
				0.0f);

			break;
		}
	}

	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	setDegeneratePolygon
//
//����		:	polygon	�擾��������]�|���S��
//
//�Ԃ�l	: 
//����		:	�k�ރ|���S���ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setDegeneratePolygon	( VERTEX3D *pVtx, POLYGON polygon1, POLYGON polygon2 )
{
	pVtx[4].pos = D3DXVECTOR3(
		polygon1.pos.x + (polygon1.size.x * 0.5f),
		polygon1.pos.y + (polygon1.size.y * 0.5f),
		0.f);

	pVtx[5].pos = D3DXVECTOR3(
		polygon2.pos.x - (polygon2.size.x * 0.5f),
		polygon2.pos.y - (polygon2.size.y * 0.5f),
		0.f);
}