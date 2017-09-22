/*****************************************************************************
�x�N�g�����g���̂ɕ֗��ȃt�@�C��[vector.cpp]
Aythor	: ���@��
Data	: 2017_01_12
=============================================================================
Updata

*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include <math.h>
#include "vector.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getLenVector
//�Ԃ�l	: 
//����		: �x�N�g���̒����擾
//////////////////////////////////////////////////////////////////////////////
double	getLenVector	( D3DXVECTOR3 vec )
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getInnerProduct
//�Ԃ�l	: 
//����		: �x�N�g���̓��ώ擾
//////////////////////////////////////////////////////////////////////////////
double	getInnerProduct	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 )
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getAngleVector
//�Ԃ�l	: 
//����		: �Ȃ��p�̎擾
//////////////////////////////////////////////////////////////////////////////
double	getAngleVector	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 )
{
	//�ϐ��錾
	double	len[2];	//�x�N�g���̒���
	double	cos;	//Cos�p

	len[0]	=	getLenVector(vec1);
	len[1]	=	getLenVector(vec2);

	cos = getInnerProduct(vec1, vec2) / (len[0] * len[1]);

	return	acos(cos);
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getPosLen
//�Ԃ�l	: 
//����		: ��_�ԋ����擾
//////////////////////////////////////////////////////////////////////////////
double	getPosLen	( D3DXVECTOR3 line1, D3DXVECTOR3 line2 )
{
	return sqrt((line2.x - line1.x) * (line2.x - line1.x) + (line2.y - line1.y) * (line2.y - line1.y) + (line2.z - line1.z) * (line2.z - line1.z));
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getPosLen
//�Ԃ�l	: 
//����		: ���K�����ꂽ�ǂ���x�N�g���v�Z
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	wallScratchVector	(D3DXVECTOR3 wallNormal, D3DXVECTOR3 moveVec)
{
	//�ϐ��錾
	D3DXVECTOR3		normal;		//�@���x�N�g��
	float			dot;		//�ǖ@���x�N�g���Ɛi�s�x�N�g���̓���

	//�@���x�N�g�����K��
	D3DXVec3Normalize(&normal, &wallNormal);

	//�ǂƐi�s�x�N�g���̓��όv�Z
	dot = D3DXVec3Dot(&wallNormal, &moveVec);

	//�ǂ���x�N�g���v�Z
	normal *= dot;
	normal = moveVec - normal;
	D3DXVec3Normalize(&normal, &normal);

	return normal;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: getCrossProduct
//�Ԃ�l	: �O��
//����		: �x�N�g���̊O�ώ擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getCrossProduct	( D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 )
{
	//�ϐ��錾
	D3DXVECTOR3	work;		//��Ɨp�ϐ�

	//�O�όv�Z
	work.x = vec1.y * vec2.z - vec1.z * vec2.y;
	work.y = vec1.z * vec2.x - vec1.x * vec2.z;
	work.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return work;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	getNormalVec
//
//����		:	point	�x�N�g���̎n�_
//				vec1	�I�_1
//				vec2	�I�_2
//
//�Ԃ�l	:	�@���x�N�g��
//����		:	�@���x�N�g���擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	getNormalVec	( D3DXVECTOR3 point, D3DXVECTOR3 vec1, D3DXVECTOR3 vec2 )
{
	//�ϐ��錾
	D3DXVECTOR3		work;		//��Ɨp�ϐ�
	double			scale;		//�傫��

	//�x�N�g�������_�ɍ��킹��
	vec1 = vec1 - point;
	vec2 = vec2 - point;

	//�O�ς����߂�
	work = getCrossProduct(vec1, vec2);

	//vec1��vec2�̑傫�������߂�
	scale = sqrt(work.x * work.x + work.y * work.y + work.z * work.z);

	//�O�ς𐳋K��
	work *= scale;

	return work;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	vec3Normalize
//
//����		:	point	�x�N�g���̎n�_
//				vec1	�I�_1
//				vec2	�I�_2
//
//�Ԃ�l	:	�@���x�N�g��
//����		:	�@���x�N�g���擾
//////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	vec3Normalize	( D3DXVECTOR3 vec )
{
	//�ϐ��錾
	double		len;

	len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	vec *= 1.0f / len;

	return vec;
}