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
	LPD3DXEFFECT			m_pEffect;
	float				m_fAlpha;
	float				m_fDelta;
	D3DXHANDLE			m_hAlphahandle;
public:
	virtual void Update();
	virtual void Start();
	virtual void Stop();
	virtual void Render();
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	LPD3DXEFFECT LoadEffectHpp(const char* szFileName);
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
	//LPD3DXEFFECT LoadEffect(const char * szFileName);
public:
	TeicIceExplosion();
	~TeicIceExplosion();
};

