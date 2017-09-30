#pragma once
#include "TeicEffectParent.h"
#include "TeicParticleSystem.h"
#include "TeicEnemy.h"
class iMap;
class TeicIceblizzard:public TeicEffectParent
{
private:
	iMap*				m_pHeightmap;
	TeicEnemy*			m_pMesh[20];
	TeicParticleSystem*	m_pParticle;
	bool				m_bStart;
	float				m_fEndtiming;
	LPD3DXEFFECT			m_pEffect;
	float				m_fAlpha;
	float				m_fDelta;
	float				m_fMeshHeight[20];
	bool				m_bOn[20];
	float				m_fSpeed[20];
public:
	virtual void Update();
	virtual void Start() ;
	virtual void Render();
	virtual void Stop() ;
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
	LPD3DXEFFECT LoadEffectHpp(const char * szFileName);
	void Callbackon(int n);
public:
	TeicIceblizzard();
	~TeicIceblizzard();
};

