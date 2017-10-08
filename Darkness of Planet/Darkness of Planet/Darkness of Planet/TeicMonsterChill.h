#pragma once
#include "TeicEffectParent.h"
#include "TeicParticleSystem.h"

class TeicMonsterChill:public TeicEffectParent
{
private:

	TeicParticleSystem*	m_pParticle;
	bool				m_bStart;
	float				m_fEndtiming;


public:
	virtual void Update();
	virtual void Start();
	virtual void Stop();
	virtual void Render();

	virtual void Setup(D3DXVECTOR3	position, D3DXVECTOR3 characterpos);
	void Callbackon(int n);
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
public:
	TeicMonsterChill();
	~TeicMonsterChill();
};

