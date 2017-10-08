#pragma once
#include "TeicEffectParent.h"

class TeicLaser:public TeicEffectParent
{
private:
	
	LPD3DXMESH			m_pMesh;
	LPDIRECT3DTEXTURE9  m_pTexture;
	bool				m_bStart;
	float				m_fEndtiming;
	LPD3DXEFFECT			m_pEffect;
	float				m_fAlpha;
	float				m_fDelta;
	D3DXMATRIX			m_Matworld;
	float				m_fTime;
public:
	virtual void Update();
	virtual void Start();
	virtual void Stop();
	virtual void Render();

	virtual void Setup(D3DXVECTOR3	position, D3DXVECTOR3 characterpos);
	LPD3DXEFFECT LoadEffect(const char * szFileName);
	
	void Callbackon(int n);
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);

	virtual bool IsRunning();
	LPD3DXMESH LoadModel(const char * filename);
public:
	TeicLaser();
	~TeicLaser();
	
};

