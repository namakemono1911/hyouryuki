/*****************************************************************************
�p�[�e�B�N��
Aythor	: ���@��
Data	: 2016_11_22
=============================================================================
Updata

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////
//�w�b�_�[�t�@�C���C���N���[�h
//////////////////////////////////////////////////////////////////////////////
#include	"main.h"
#include	<d3dx9math.h>
#include	"render.h"
#include	"input.h"
#include	"camera.h"
#include	"particle.h"

//////////////////////////////////////////////////////////////////////////////
//�萔��`
//////////////////////////////////////////////////////////////////////////////
#define	TEX_NAME			("data/texture/effect003.jpg")	//�e�N�X�`���̃A�h���X
#define	MAX_PARTICLE		(2000)			//�ő�p�[�e�B�N����
#define	MAX_PARTICLE_PARAM	(5)				//�ő�p�[�e�B�N���p�����[�^��

#define	WIDTH				(80.f)
#define	HEIGHT				(80.f)

//////////////////////////////////////////////////////////////////////////////
//�\���̒�`
//////////////////////////////////////////////////////////////////////////////
typedef	struct
{
	D3DXVECTOR3		pos;		//���W
	D3DXVECTOR3		vec;		//�����x�N�g��
	D3DXCOLOR		color;		//���_�F
	float			angle;		//�Ίp�̊p�x
	float			move;		//�ړ����x
	float			len;		//�Ίp��
	float			life;		//����
	bool			use;		//true�Ŏg���Ă���
}PARTICLE;

typedef	struct
{
	D3DXVECTOR3		source;		//������
	D3DXVECTOR3		size;		//�摜�T�C�Y
	D3DXVECTOR3		vecEx;		//�����x�N�g���͈̔�
	D3DXVECTOR3		rot;		//���ˊp
	D3DXCOLOR		colorMax;	//���_�F�̍ő�l
	D3DXCOLOR		colorMin;	//���_�F�̍Œ�l
	float			moveMax;	//�ړ����x�̍ő�l
	float			moveMin;	//�ړ����x�̍Œ�l
	float			lenMax;		//�Ίp���̍ő�l
	float			lenMin;		//�Ίp���̍Œ�l
	float			lifeMax;	//�����̍ő�l
	float			lifeMin;	//�����̍Œ�l
	int				value;		//1�t���[���ӂ�̔�����
	int				time;		//���t���[���o����
	bool			use;		//�g�p�t���O
}PARTICLE_PARAM;

typedef	enum
{
	OPE_SOURCE = 0,				//����������
	OPE_VEC,					//�����x�N�g������
	OPE_ROT,					//�ˊp����
	OPE_COLOR,					//���_�F����
	OPE_MOVE,					//�����x����
	OPE_LEN,					//���a����
	OPE_LIFE,					//��������
	OPE_VALUE,					//���ː�����
	OPE_NULL					//null
}PARTICLE_OPE;

//////////////////////////////////////////////////////////////////////////////
//�v���g�^�C�v�錾
//////////////////////////////////////////////////////////////////////////////
void	initStruct				( void );
HRESULT	makeVertexParticle		( LPDIRECT3DDEVICE9 pDevice );
HRESULT	makeIndexBufferParticle	( LPDIRECT3DDEVICE9	pDevice );
void	setMatrixParticle		( LPDIRECT3DDEVICE9 pDevice, int num );
void	setParticle				( int numParticle, int numParam );
void	opeParticle				( void );

//////////////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
//////////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE9		g_pTextureParticle		=	NULL;	//�e�N�X�`���C���^�[�t�F�C�X
LPDIRECT3DVERTEXBUFFER9	g_pVtxBufferParticle	=	NULL;	//���_�o�b�t�@

PARTICLE			particle[MAX_PARTICLE];					//�p�[�e�B�N�����
PARTICLE_PARAM		param[MAX_PARTICLE_PARAM];				//�p�[�e�B�N���̃p�����[�^
PARTICLE_OPE		particleOpe;							//�p�[�e�B�N���̑�����@

int					g_numParticle	=	0;						//�p�[�e�B�N����
int					g_numParticleParam	=	0;				//�p�[�e�B�N���p�����[�^��
int					opeMode		=	0;						//���샂�[�h

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initParticle
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void initParticle ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();

	//�\���̏�����
	initStruct();

	//�p�[�e�B�N���Z�b�g
	/*
	setParticleAll(
		D3DXVECTOR3(-30.f, 5.f, 0.f),
		D3DXVECTOR3(WIDTH, HEIGHT, 0.f),
		D3DXVECTOR3(360, 360, 360),
		D3DXVECTOR3(0.f, 0.f, 0.f),
		D3DXCOLOR(1.f, 0.f, 0.5f, 1.f),
		D3DXCOLOR(0.5f, 0.0f, 0.0f, 0.1f),
		10.f,
		1.f,
		1.f,
		0.5f,
		1.f, 
		0.5f,
		10,
		-1);*/
	/*
	//�p�[�e�B�N��2�Z�b�g
	setParticleAll(
		D3DXVECTOR3(0.f, 5.f, 0.f),
		D3DXVECTOR3(WIDTH, HEIGHT, 0.f),
		D3DXVECTOR3(360, 0, 360),
		D3DXVECTOR3(0.f, 0.0f, 0.0f),
		D3DXCOLOR(1.f, 1.f, 1.f, 0.1f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f),
		5.f,
		2.f,
		0.05f,
		0.01f,
		0.3f, 
		0.01f,
		3,
		-1);*/

	//�|���S���̕`��ʒu�ݒ�
	makeVertexParticle(pDevice);

	//�e�N�X�`���̓ǂݍ���
	if(FAILED(D3DXCreateTextureFromFile(pDevice, TEX_NAME, &g_pTextureParticle)))
	{
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: initStruct
//�Ԃ�l	: 
//����		: �|���S��������
//////////////////////////////////////////////////////////////////////////////
void	initStruct	( void )
{
	//�ϐ��錾
	int		i;	//loop

	//=============================
	//�\���̏�����
	//=============================
	for(i = 0; i < MAX_PARTICLE; i++)
	{
		//���W
		particle[i].pos		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//�x�N�g��
		particle[i].vec		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//���_�F
		particle[i].color	=	D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

		//�Ίp��
		particle[i].len		=	0.f;

		//�����x
		particle[i].move	=	0.f;

		//����
		particle[i].life	=	0.f;

		//�Ίp�̊p�x
		particle[i].angle	=	0.f;

		//�g�p�t���O
		particle[i].use		=	false;
	}

	//=============================
	// �p�����[�^������
	//=============================
	for(i = 0; i < MAX_PARTICLE_PARAM; i++)
	{
		//������
		param[i].source	=	D3DXVECTOR3(0.f, 5.f, 0.f);

		//�x�N�g���͈̔�
		param[i].vecEx		=	D3DXVECTOR3(
			360,
			360,
			360);

		//�ˊp
		param[i].rot		=	D3DXVECTOR3(0.f, 0.f, 0.f);

		//���_�F�̍ő�l
		param[i].colorMax	=	D3DXCOLOR(1.0f, 1.0f,  1.0f, 1.0f);

		//���_�F�̍Œ�l
		param[i].colorMin	=	D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);

		//�傫���̍ő�l
		param[i].lenMax		=	5.f;

		//�傫���̍Œ�l
		param[i].lenMin		=	1.f;

		//�����̍ő�l
		param[i].lifeMax	=	1.0f;

		//�����̍Œ�l
		param[i].lifeMin	=	0.5f;

		//�ړ����x�̍ő�l
		param[i].moveMax	=	0.5f;

		//�ړ����x�̍Œ�l
		param[i].moveMin	=	0.01f;

		//������
		param[i].value		=	10;

		//�g�p�t���O
		param[i].use		=	false;
	}
	
	//=============================
	// �p�����[�^������
	//=============================
	particleOpe	=	OPE_SOURCE;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setParticleAll
//�Ԃ�l	: 
//����		: �p�[�e�B�N���̃p�����[�^���ׂĐݒ�
//////////////////////////////////////////////////////////////////////////////
int	setParticleAll	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 vec, D3DXVECTOR3 rot, D3DXCOLOR colMax, D3DXCOLOR colMin, float lenMax, float lenMin, float lifeMax, float lifeMin, float moveMax, float moveMin, int value, int time )
{
	//�ϐ��錾
	int	i;		//loop

	for(i = 0; i < MAX_PARTICLE_PARAM; i++)
	{
		//���g�p�̃p�����[�^�T��
		if(param[i].use	==	false)
		{
			//=============================
			// ���ꂼ��͈͐ݒ�
			//=============================
			param[i].source		=	pos;		//������
			param[i].size		=	size;		//�摜�̃T�C�Y
			param[i].vecEx		=	vec;		//�����x�N�g��
			param[i].rot		=	rot;		//�p�x
			param[i].colorMax	=	colMax;		//�ō����_�F
			param[i].colorMin	=	colMin;		//�Œᒸ�_�F
			param[i].lenMax		=	lenMax;		//�ő唼�a
			param[i].lenMin		=	lenMin;		//�Œᔼ�a
			param[i].lifeMax	=	lifeMax;	//�ő����
			param[i].lifeMin	=	lifeMin;	//�Œ����
			param[i].moveMax	=	moveMax;	//�ō����x
			param[i].moveMin	=	moveMin;	//�Œᑬ�x
			param[i].value		=	value;		//1�t���[��������
			param[i].time		=	time;		//����
			param[i].use		=	true;		//�g�p�t���O

			g_numParticleParam++;					//�p�����[�^�������Z

			//�ݒ肵���z��ԍ��ԋp
			return i;
		}
	}

	//�S�Ďg���Ă���
	return	-1;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setParticlePos
//�Ԃ�l	: 
//����		: �p�[�e�B�N���̔������ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setParticlePos	( D3DXVECTOR3 pos, int num )
{
	if(param[num].use	==	true)
	{
		param[num].source	=	pos;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: uninitParticle
//�Ԃ�l	: 
//����		: �|���S���I��
//////////////////////////////////////////////////////////////////////////////
void	uninitParticle	( void )
{
	//���_�o�b�t�@���
	if(g_pVtxBufferParticle	!=	NULL)
	{
		g_pVtxBufferParticle->Release();
		g_pVtxBufferParticle	=	NULL;
	}

	//�e�N�X�`���o�b�t�@release
	if(g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	g_numParticle		=	0;
	g_numParticleParam	=	0;
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: releaseParticle
//�Ԃ�l	: 
//����		: �p�[�e�B�N���̃����[�X
//////////////////////////////////////////////////////////////////////////////
void	releaseParticle	(int num )
{
	if(param[num].use	==	true)
	{
		param[num].use	=	false;
		g_numParticleParam--;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: updateParticle
//�Ԃ�l	: 
//����		: �|���S���X�V
//////////////////////////////////////////////////////////////////////////////
void	updateParticle	( void )
{
	//�ϐ��錾
	VERTEX3D	*pVtx;	//���_���
	int		i, j, k, n;		//loop
	int		work;			//���[�N

	//=============================
	// �L�[����
	//=============================
	//opeParticle();

	//=============================
	//�p�[�e�B�N������
	//=============================
	//�g�p����Ă���p�����[�^�T��
	for(i = 0, j = 0; j < g_numParticleParam && i < MAX_PARTICLE_PARAM; i++)
	{
		if(param[i].use	==	true)
		{
			//���Ԋm�F
			if(param[i].time	>=	0)
			{
				param[i].time--;
				if(param[i].time	<=	0)
				{
					releaseParticle(i);
				}
			}

			//�g�p����Ă��Ȃ��p�[�e�B�N���T��
			for(k = 0, n = 0; n < param[i].value && k < MAX_PARTICLE; k++)
			{
				if(k == MAX_PARTICLE - 1)
				{
					break;
				}

				if(particle[k].use	==	false)
				{
					//�����Z�b�g
					setParticle(k, i);

					n++;
				}
			}

			j++;
		}
	}

	//=============================
	//�ݒ�X�V
	//=============================
	work	=	g_numParticle;
	for(i = 0, j = 0; j < work && i < MAX_PARTICLE; i++)
	{
		//�g�p����Ă�����X�V
		if(particle[i].use	==	true)
		{
			//���W
			particle[i].pos	=	D3DXVECTOR3(
				particle[i].move * particle[i].vec.x + particle[i].pos.x,
				particle[i].move * particle[i].vec.y + particle[i].pos.y,
				particle[i].move * particle[i].vec.z + particle[i].pos.z);

			//���_�F
			particle[i].color.a	-=	particle[i].life;

			if(particle[i].color.a <= 0)
			{
				particle[i].color.a	=	0.f;
				particle[i].use		=	false;
				g_numParticle--;
			}

			j++;
		}
	}

	//=============================
	//���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾
	//=============================
	g_pVtxBufferParticle->Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0, j = 0; j < g_numParticle && i < MAX_PARTICLE; i++)
	{
		if(particle[i].use	==	true)
		{
			//���W�ݒ�
			pVtx[0].pos	=	D3DXVECTOR3(
				particle[i].len * -cos(particle[i].angle),
				particle[i].len * -sin(particle[i].angle),
				0.f);

			pVtx[1].pos	=	D3DXVECTOR3(
				particle[i].len * -cos(particle[i].angle),
				particle[i].len * sin(particle[i].angle),
				0.f);

			pVtx[2].pos	=	D3DXVECTOR3(
				particle[i].len * cos(particle[i].angle),
				particle[i].len * -sin(particle[i].angle),
				0.f);

			pVtx[3].pos	=	D3DXVECTOR3(
				particle[i].len * cos(particle[i].angle),
				particle[i].len * sin(particle[i].angle),
				0.f);

			//���_�F
			pVtx[0].color	=	particle[i].color;
			pVtx[1].color	=	particle[i].color;
			pVtx[2].color	=	particle[i].color;
			pVtx[3].color	=	particle[i].color;

			//�e�N�X�`�����W
			pVtx[0].tex		=	D3DXVECTOR2(0.f, 1.f);
			pVtx[1].tex		=	D3DXVECTOR2(0.f, 0.f);
			pVtx[2].tex		=	D3DXVECTOR2(1.f, 1.f);
			pVtx[3].tex		=	D3DXVECTOR2(1.f, 0.f);

			//�|�C���^�ړ�
			pVtx	+=	NUM_VERTEX;
			j++;
		}
	}

	//���̂�����A�[�����b�N��
	g_pVtxBufferParticle->Unlock();
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setParticle
//�Ԃ�l	: 
//����		: �p�[�e�B�N���ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setParticle	( int numParticle, int numParam )
{
	//�ϐ��錾
	float	colorWork;		//���_�F�̃��[�N
	float	colorMax;		//�ő咸�_�F�̃��[�N
	float	colorMin;		//�Œᒸ�_�F�̃��[�N
	float	moveMax;		//�ő�����x�̃��[�N
	float	moveMin;		//�Œ�����x�̃��[�N
	float	lenMax;			//�ő唼�a�̃��[�N
	float	lenMin;			//�Œᔼ�a�̃��[�N
	float	lifeMax;		//�ő�����̃��[�N
	float	lifeMin;		//�Œ�����̃��[�N
	float	angle[3];		//�p�x�̃��[�N

	//�������Z
	g_numParticle++;

	//�g�p�t���O
	particle[numParticle].use	=	true;

	//�Ίp�̊p�x
	particle[numParticle].angle	=	atan2(param[numParam].size.y, param[numParam].size.x);

	//���W
	particle[numParticle].pos	=	param[numParam].source;

	//�����x�N�g��
	if(param[numParam].vecEx.x	>	0)
	{
		angle[0]	=	(float)(rand() % (int)param[numParam].vecEx.x);
		angle[0]	=	angle[0] * PI / 180;
	}
	else
	{
		angle[0]	=	0.f;
	}

	if(param[numParam].vecEx.y	>	0)
	{
		angle[1]	=	(float)(rand() % (int)param[numParam].vecEx.y);
		angle[1]	=	angle[1] * PI / 180;
	}
	else
	{
		angle[1]	=	0.f;
	}

	if(param[numParam].vecEx.z	>	0)
	{
		angle[2]	=	(float)(rand() % (int)param[numParam].vecEx.z);
		angle[2]	=	angle[2] * PI / 180;
	}
	else
	{
		angle[2]	=	0.f;
	}

	particle[numParticle].vec	=	D3DXVECTOR3(
		sin(angle[0]) + param[numParam].rot.x,
		sin(angle[1]) + param[numParam].rot.y,
		sin(angle[2]) + param[numParam].rot.z);

	//�����x
	moveMax	=	param[numParam].moveMax * 100.f;
	moveMin	=	param[numParam].moveMin * 100.f;
	if(moveMax - moveMin	>	0)
	{
		particle[numParticle].move	=	(float)(rand() % (int)(moveMax - moveMin) + moveMin);
		particle[numParticle].move	*=	0.01f;
	}
	else
	{
		particle[numParticle].move	=	param[numParam].moveMax;
	}

	//�Ίp��
	lenMax	=	param[numParam].lenMax * 100.f;
	lenMin	=	param[numParam].lenMin * 100.f;
	if(lenMax - lenMin	>	0)
	{
		particle[numParticle].len	=	(float)(rand() % (int)(lenMax - lenMin) + lenMin);
		particle[numParticle].len	*=	0.01f;
	}
	else
	{
		particle[numParticle].len	=	param[numParam].lenMax;
	}

	//���_�F
	colorMax	=	param[numParam].colorMax.r * 100.f;
	colorMin	=	param[numParam].colorMin.r * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.r;
	}
	particle[numParticle].color.r	=	colorWork;

	colorMax	=	param[numParam].colorMax.g * 100.f;
	colorMin	=	param[numParam].colorMin.g * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.g;
	}
	particle[numParticle].color.g	=	colorWork;
	
	colorMax	=	param[numParam].colorMax.b * 100.f;
	colorMin	=	param[numParam].colorMin.b * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.b;
	}
	particle[numParticle].color.b	=	colorWork;
	
	colorMax	=	param[numParam].colorMax.a * 100.f;
	colorMin	=	param[numParam].colorMin.a * 100.f;
	if(colorMax - colorMin	>	0)
	{
		colorWork	=	(float)(rand() % (int)(colorMax - colorMin));
		colorWork	*=	0.01f;
	}
	else
	{
		colorWork	=	param[numParam].colorMax.a;
	}
	particle[numParticle].color.a	=	colorWork;

	//����
	lifeMax	=	param[numParam].lifeMax * 1000.f;
	lifeMin	=	param[numParam].lifeMin * 1000.f;
	if(lifeMax - lifeMin	> 0)
	{
		particle[numParticle].life	=	(float)(rand() % (int)(lifeMax - lifeMin) + lifeMin);
		particle[numParticle].life	*=	0.00001f;
	}
	else
	{
		particle[numParticle].life	=	param[numParam].lifeMax;
	}
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: drawParticle
//�Ԃ�l	: 
//����		: �|���S���`��
//////////////////////////////////////////////////////////////////////////////
void drawParticle ( void )
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9	pDevice	=	getDevice();
	int		i, j;		//loop

	//���C�g�̉e��off
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//���Z�����ɐ؂�ւ�
	addDevice();

	//z�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ZENABLE, false);		//z�o�b�t�@��ON�EOFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);	//�o�b�t�@�ւ̏�������ON�EOFF

	pDevice->SetFVF(FVF_VERTEX_3D);

	//�X�g���[���쐬
	pDevice->SetStreamSource(0, g_pVtxBufferParticle, 0, sizeof(VERTEX3D));

	for(i = 0, j = 0; j < g_numParticle; i++)
	{
		if(particle[i].use	==	true)
		{
		//���[���h�}�g���b�N�X�ݒ�
		setMatrixParticle(pDevice, i);

		//�e�N�X�`���Z�b�g
		pDevice->SetTexture(0,g_pTextureParticle);

		//�|���S���̕`��
		pDevice->DrawPrimitive
			(
			D3DPT_TRIANGLESTRIP,			//�|���S���̎��
			NUM_VERTEX * j,					//�I�t�Z�b�g(���_��)
			NUM_POLYGON						//�|���S���̐�
			);

		j++;
		}
	}

	//���C�g�̉e��on
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//��Z�����ɐ؂�ւ�
	multiDevice();

	//z�e�X�g����
	pDevice->SetRenderState(D3DRS_ZENABLE, true);		//z�o�b�t�@��ON�EOFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);	//�o�b�t�@�ւ̏�������ON�EOFF
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: setMatrix
//�Ԃ�l	: 
//����		: �}�g���b�N�X�ݒ�
//////////////////////////////////////////////////////////////////////////////
void	setMatrixParticle	( LPDIRECT3DDEVICE9 pDevice, int num )
{
	//�ϐ��錾
	D3DXMATRIX	world;			//���[���h�s��
	D3DXMATRIX	move;			//�ړ��s��
	D3DXMATRIX	ViewInverse;	//�J�����̋t�s��

	//�t�s��쐬
	D3DXMatrixTranspose(&ViewInverse, &getViewMtx());

	ViewInverse._14	=	0.0f;
	ViewInverse._24	=	0.0f;
	ViewInverse._34	=	0.0f;

	//���[���h�s��̐ݒ�
	//�s�񏉊���
	D3DXMatrixIdentity(&world);

	//���s�ړ�
	D3DXMatrixTranslation(&move,	
		particle[num].pos.x,
		particle[num].pos.y,
		particle[num].pos.z);

	//�s�񍇐�
	world	*=	ViewInverse;
	world	*=	move;

	pDevice->SetTransform(D3DTS_WORLD, &world);		//�s����f�o�C�X�ɐݒ�
}

//////////////////////////////////////////////////////////////////////////////
//�֐���	: makeVertexParticle
//�Ԃ�l	: 
//����		: �|���S���`��ʒu�ݒ�
//////////////////////////////////////////////////////////////////////////////
HRESULT	makeVertexParticle	( LPDIRECT3DDEVICE9 pDevice )
{
	//���_�o�b�t�@�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer
			(
			sizeof(VERTEX3D) * MAX_PARTICLE * MAX_PARTICLE_PARAM * NUM_VERTEX,	//���_�o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@
			FVF_VERTEX_3D,							//�Ȃ񂩐ݒ�
			D3DPOOL_MANAGED,						//�������Ǘ����@
			&g_pVtxBufferParticle,					//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
			NULL
			)
		))
	{
		return E_FAIL;
	}

	return	S_OK;
}