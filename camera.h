/*****************************************************************************
�J��������
Aythor	: ���@��
Data	: 2016_05_17
=============================================================================
Updata

*****************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
	D3DXMATRIX	view;		//�J�����s��
	D3DXVECTOR3	posAt;		//�����_
	D3DXVECTOR3	posEye;		//�J�����̍��W
	D3DXVECTOR3	moveVec;	//�i�s����
	D3DXVECTOR3	vecUp;		//�J�����̕���
	D3DXMATRIX	proj;		//�v���W�F�N�V�����s��
	float		angleYZ;	//XY�̊p�x
	float		angleXZ;	//XZ�̊p�x
	float		atAngle;	//�����_�ƃJ�����̊p�x
	float		len;		//���S����̋���
}CAMERA;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void		initCamera		( void );
void		updateCamera	( void );
D3DXMATRIX	getViewMtx		( void );
D3DXVECTOR3 getCameraPos	( void );
CAMERA		*getCamera		( void );

#endif