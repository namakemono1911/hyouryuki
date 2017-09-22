/*****************************************************************************
�Q�[�����[�h
Aythor	: ���@��
Data	: 2016_12_23
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "camera.h"
#include "polygon.h"
#include "model.h"
#include "meshField.h"
#include "particle.h"
#include "meshWall.h"
#include "meshDome.h"
#include "keyOpe.h"
#include "input.h"
#include "bird.h"
#include "wood.h"
#include "rock.h"
#include "spear.h"
#include "game.h"
#include "fade.h"
#include "render.h"
#include "hit.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	NUM_FIELD_X		(50)		//�t�B�[���h�̉��̃^�C���̐�
#define	NUM_FIELD_Y		(50)		//�t�B�[���h�̏c�̃^�C���̐�
#define	FIELD_SIZE_X	(10)		//�t�B�[���h�̉���
#define	FIELD_SIZE_Y	(10)		//�t�B�[���h�̉��s

#define	BIRD_PARCENT	(80)		//�J�������o��m��
#define BIRD000_PARCENT	(30)		//�J����000���o��m��
#define	BIRD001_PARCENT	(70)		//�J����001���o��m��
#define	BIRD_MAX_SPEED	(0.5)		//�J�����̍ō����x

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	updateGameField	( void );
void	randObj			( D3DXVECTOR3 pos );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
int		fieldIdManage[2][FIELD_Y][FIELD_X];

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	initGame
//�Ԃ�l	:	
//����		:	�Q�[���ɕK�v�ȃt�@�C���̏�����
//////////////////////////////////////////////////////////////////////////////
HRESULT	initGame	( void )
{
	//�ϐ��錾
	int			x, y;		//�t�B�[���h�̏c����loop
	D3DXVECTOR2	fieldSize;	//�t�B�[���h�S�̂̃T�C�Y

	ShowCursor(FALSE);
	FixingMouse(true);
	
	//=============================
	// �t�F�[�h������
	//=============================
	setFade(D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		100,
		FADE_IN,
		0);

	//=============================
	// �|���S��������
	//=============================
	//initPolygon();

	//=============================
	// �L�[���쏉����
	//=============================
	initKeyOpe();

	//=============================
	// �R���W����������
	//=============================
	initHit();

	//=============================
	// ���f��������
	//=============================
	initModel();

	//=============================
	// �p�[�e�B�N��������
	//=============================
	initParticle();

	//=============================
	// ���b�V���t�B�[���h������
	//=============================
	initMeshField();

	fieldSize	=	D3DXVECTOR2(
		-(FIELD_SIZE_X * NUM_FIELD_X * 0.5f) * (FIELD_X - 1),
		(FIELD_SIZE_Y * NUM_FIELD_Y * 0.5f) * (FIELD_Y - 1));

	for(y = 0; y < FIELD_Y; y++)
	{
		for(x = 0; x < FIELD_X; x++)
		{
			//�t�B�[���h�ݒ�
			setMeshField(
				D3DXVECTOR3(fieldSize.x + (FIELD_SIZE_X * NUM_FIELD_X * x), 0.0f, fieldSize.y - (FIELD_SIZE_Y * NUM_FIELD_Y * y)),
				D3DXVECTOR3(FIELD_SIZE_X, 0.f, FIELD_SIZE_Y),
				D3DXVECTOR3(NUM_FIELD_X, 0, NUM_FIELD_Y),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXCOLOR(0.7f, 0.7f, 1.f, 1.f),
				D3DXVECTOR2(-0.001f, 0.001f),
				FIELD_TEX_003,
				x + y * FIELD_X);
			fieldIdManage[0][y][x]	=	x + y * FIELD_X;
			
			setMeshField(
				D3DXVECTOR3(fieldSize.x + (FIELD_SIZE_X * NUM_FIELD_X * x), 0.01f, fieldSize.y - (FIELD_SIZE_Y * NUM_FIELD_Y * y)),
				D3DXVECTOR3(FIELD_SIZE_X, 0.f, FIELD_SIZE_Y),
				D3DXVECTOR3(NUM_FIELD_X, 0, NUM_FIELD_Y),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.3f),
				D3DXVECTOR2(-0.0005f, 0.0005f),
				FIELD_TEX_002,
				x + y * FIELD_Y + FIELD_X * FIELD_Y);
			fieldIdManage[1][y][x]	=	x + y * FIELD_Y + FIELD_X * FIELD_Y;
		}
	}

	//=============================
	// ���b�V���E�H�[��������
	//=============================
	//initMeshWall();

	//=============================
	// ���b�V���h�[��������
	//=============================
	initMeshDome();

	setMeshDome(
		D3DXVECTOR3(0.f, 0.f, 0.f),
		(30.f * 0.5f) * 30,
		D3DXVECTOR2(16, 32),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		DOME_TEX_001,
		DOME_ID_000);

	//=============================
	// �����f��������
	//=============================
	initBird();
	setBird(
		D3DXVECTOR3(0.f, 10.f, 30.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.3f,
		BIRD_TYPE_001);

	setBird(
		D3DXVECTOR3(20.f, 10.f, 30.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		BIRD_TYPE_000);
	
	//=============================
	// ���؃��f��������
	//=============================
	initWood();

	setWood(
		D3DXVECTOR3(-20.f, 5.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 10.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		WOOD_TYPE_000);
	
	setWood(
		D3DXVECTOR3(-40.f, 5.f, 0.f),
		D3DXVECTOR3(10.f, 10.f, 10.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		0.f,
		WOOD_TYPE_001);
	
	//=============================
	// �����f��������
	//=============================
	initSpear();
	
	setSpear(
		D3DXVECTOR3(0.f, 10.f, 10.f),
		D3DXVECTOR3(1.f, 1.f, 1.f),
		D3DXVECTOR3(0.f, PI, 0.f),
		D3DXVECTOR3(0.f, 1.f, 0.5f),
		2.f);
	
	//=============================
	// �⃂�f��������
	//=============================
	initRock();

	setRock(
		D3DXVECTOR3(50.f, 10.f, 30.f),
		D3DXVECTOR3(1.f, 1.f, 1.f),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		ROCK_TYPE_000);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	uninitGame
//�Ԃ�l	:	
//����		:	�Q�[���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitGame	( void )
{
	//�|���S���I��
	//uninitPolygon();

	//��I��
	uninitRock();

	//���I��
	uninitSpear();

	//���؏I��
	uninitWood();

	//���I��
	uninitBird();

	//���f���I��
	uninitModel();

	//�p�[�e�B�N���I��
	uninitParticle();

	//���b�V���t�B�[���h�I��
	uninitMeshField();

	//���b�V���E�H�[���I��
	//uninitMeshWall();

	//���b�V���h�[���I��
	uninitMeshDome();

	//�R���W�����I��
	uninitHit();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateGame
//�Ԃ�l	:	
//����		:	�Q�[���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateGame	( void )
{
	//�|���S���X�V
	//updatePolygon();

	//�L�[����X�V
	updateKeyOpe();

	//���X�V
	updateSpear();

	//���؍X�V
	updateWood();

	//���X�V
	updateBird();

	//��X�V
	updateRock();

	//���f���X�V
	updateModel();

	//�p�[�e�B�N���X�V
	updateParticle();

	//���b�V���t�B�[���h�X�V
	updateMeshField();

	//���b�V���E�H�[���X�V
	//updateMeshWall();

	//���b�V���h�[���X�V
	updateMeshDome();

	//�t�B�[���h���X�V
	updateGameField();

	//�R���W�����X�V
	updateHit();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	drawGame
//�Ԃ�l	:	
//����		:	�Q�[���`��
//////////////////////////////////////////////////////////////////////////////
void	drawGame	( void )
{
	//3D�`�惂�[�h
	multiDevice();

	//�|���S���`��
	//drawPolygon();

	//���`��
	drawSpear();

	//��`��
	drawRock();
		
	//���b�V���t�B�[���h�`��
	drawMeshField();

	//���b�V���E�H�[���`��
	//drawMeshWall();

	//���b�V���h�[���`��
	drawMeshDome();

	//���`��
	drawBird();

	//���ؕ`��
	drawWood();

	//���f���`��
	drawModel();

	//�p�[�e�B�N���`��
	drawParticle();

	//�R���W�����`��
	drawHit();

	//2D�`�惂�[�h
	normal2D();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	updateGameField
//�Ԃ�l	:	
//����		:	�Q�[�����̃t�B�[���h�X�V
//////////////////////////////////////////////////////////////////////////////
void	updateGameField	( void )
{
	//�ϐ��錾
	CAMERA		*Camera	=	getCamera();		//�J�����\���̂̃|�C���^
	FIELD		*Field	=	getField(fieldIdManage[0][1][1]);	//�t�B�[���h�\���̂̃|�C���^
	FIELD		*FieldWork;
	int			i;			//loop
	int			idWork;		//ID���[�N

	if(Camera->posEye.x > Field->pos.x + (Field->size.x * Field->numTyle.x * 0.5f))
	{
		for(i = 0; i < FIELD_Y; i++)
		{
			FieldWork = getField(fieldIdManage[0][i][FIELD_X - 1]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[0][i][0]);
			
			FieldWork = getField(fieldIdManage[1][i][FIELD_X - 1]);
			
			setFieldPos(D3DXVECTOR3(
				FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[1][i][0]);

			idWork	=	fieldIdManage[0][i][0];
			fieldIdManage[0][i][0] = fieldIdManage[0][i][1];
			fieldIdManage[0][i][1] = fieldIdManage[0][i][2];
			fieldIdManage[0][i][2] = idWork;
			
			idWork	=	fieldIdManage[1][i][0];
			fieldIdManage[1][i][0] = fieldIdManage[1][i][1];
			fieldIdManage[1][i][1] = fieldIdManage[1][i][2];
			fieldIdManage[1][i][2] = idWork;
			
			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}

	if(Camera->posEye.x < Field->pos.x - (Field->size.x * Field->numTyle.x * 0.5f))
	{
		for(i = 0; i < FIELD_Y; i++)
		{
			FieldWork = getField(fieldIdManage[0][i][0]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x - (FieldWork->size.x * FieldWork->numTyle.x),
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[0][i][FIELD_X - 1]);
			
			FieldWork = getField(fieldIdManage[1][i][0]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x - (FieldWork->size.x * FieldWork->numTyle.x),
				FieldWork->pos.y,
				FieldWork->pos.z),
				fieldIdManage[1][i][FIELD_X - 1]);

			idWork	=	fieldIdManage[0][i][FIELD_X - 1];
			fieldIdManage[0][i][2] = fieldIdManage[0][i][1];
			fieldIdManage[0][i][1] = fieldIdManage[0][i][0];
			fieldIdManage[0][i][0] = idWork;
			
			idWork	=	fieldIdManage[1][i][FIELD_X - 1];
			fieldIdManage[1][i][2] = fieldIdManage[1][i][1];
			fieldIdManage[1][i][1] = fieldIdManage[1][i][0];
			fieldIdManage[1][i][0] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}

	if(Camera->posEye.z > Field->pos.z + (Field->size.z * Field->numTyle.z * 0.5f))
	{
		for(i = 0; i < FIELD_X; i++)
		{
			FieldWork = getField(fieldIdManage[0][0][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[0][FIELD_X - 1][i]);
			
			FieldWork = getField(fieldIdManage[1][0][i]);
			
			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z + (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][FIELD_X - 1][i]);
			
			idWork	=	fieldIdManage[0][FIELD_X - 1][i];
			fieldIdManage[0][2][i] = fieldIdManage[0][1][i];
			fieldIdManage[0][1][i] = fieldIdManage[0][0][i];
			fieldIdManage[0][0][i] = idWork;
			
			idWork	=	fieldIdManage[1][FIELD_X - 1][i];
			fieldIdManage[1][2][i] = fieldIdManage[1][1][i];
			fieldIdManage[1][1][i] = fieldIdManage[1][0][i];
			fieldIdManage[1][0][i] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}
	
	if(Camera->posEye.z < Field->pos.z - (Field->size.z * Field->numTyle.z * 0.5f))
	{
		for(i = 0; i < FIELD_X; i++)
		{
			FieldWork = getField(fieldIdManage[0][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z - (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[0][0][i]);
			
			FieldWork = getField(fieldIdManage[1][FIELD_X - 1][i]);

			setFieldPos(D3DXVECTOR3(
				FieldWork->pos.x,
				FieldWork->pos.y,
				FieldWork->pos.z - (FieldWork->size.z * FieldWork->numTyle.z)),
				fieldIdManage[1][0][i]);

			idWork	=	fieldIdManage[0][0][i];
			fieldIdManage[0][0][i] = fieldIdManage[0][1][i];
			fieldIdManage[0][1][i] = fieldIdManage[0][2][i];
			fieldIdManage[0][2][i] = idWork;
			
			idWork	=	fieldIdManage[1][0][i];
			fieldIdManage[1][0][i] = fieldIdManage[1][1][i];
			fieldIdManage[1][1][i] = fieldIdManage[1][2][i];
			fieldIdManage[1][2][i] = idWork;

			randObj(D3DXVECTOR3(FieldWork->size.x * FieldWork->numTyle.x + FieldWork->pos.x,
							FieldWork->pos.y,
							FieldWork->pos.z));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	:	randObj
//�Ԃ�l	:	
//����		:	�Q�[�����̃I�u�W�F�N�g�����_���z�u
//////////////////////////////////////////////////////////////////////////////
void	randObj	( D3DXVECTOR3 pos )
{
	//�J�����������_���z�u
	if(rand() % 100 <= 0)
	{
		if(rand() % 80 <= BIRD000_PARCENT)
		{
			setBird(
				pos,
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				0.3f,
				BIRD_TYPE_000);
		}
		else
		{
			pos.y += 80;
			setBird(
				pos,
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				D3DXVECTOR3(0.f, 0.f, 0.f),
				(rand() % (int)(BIRD_MAX_SPEED * 100)) * 0.01f,
				BIRD_TYPE_001);
		}
	}
}