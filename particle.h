/*****************************************************************************
ビルボード
Aythor	: 上野　匠
Data	: 2016_11_22
=============================================================================
Updata

*****************************************************************************/
#ifndef	PARTICLE_H
#define	PARTICLE_H
//////////////////////////////////////////////////////////////////////////////
//プロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
void	initParticle	( void );
void	uninitParticle	( void );
void	updateParticle	( void );
void	drawParticle	( void );
int	setParticleAll	( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 vec, D3DXVECTOR3 rot, D3DXCOLOR colMax, D3DXCOLOR colMin, float lenMax, float lenMin, float lifeMax, float lifeMin, float moveMax, float moveMin, int value, int time );
void	setParticlePos	( D3DXVECTOR3 pos, int num );
void	releaseParticle	(int num );



#endif