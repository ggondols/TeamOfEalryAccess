#pragma once
#include "TeicEffectParent.h"
#include "TeicParticleSystem.h"
#include "TeicEnemy.h"
class TeicIceExplosion :public	TeicEffectParent
{
private:
	TeicEnemy*			m_pMesh;
	TeicParticleSystem*	m_pParticle;
	bool				m_bStart;
	float				m_fEndtiming;
public:
	virtual void Update();
	virtual void Start();
	virtual void Stop();
	virtual void Render();
	virtual void Setup(D3DXVECTOR3	position);



public:
	TeicIceExplosion();
	~TeicIceExplosion();
};

