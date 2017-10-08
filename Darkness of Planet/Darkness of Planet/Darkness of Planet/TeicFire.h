#pragma once
#include "TeicEffectParent.h"
class TeicFire:public TeicEffectParent
{
private:
	vector<ST_PNT_VERTEX>	m_vecVertex;
	LPD3DXEFFECT			m_pEffect;
	LPDIRECT3DTEXTURE9		m_pFlame;
	LPDIRECT3DTEXTURE9		m_pNoise;
	LPDIRECT3DTEXTURE9      m_pfire_opacity;
	float		m_fSize;
	float		m_fEndtiming;
	float		m_fTimeChange;
	float       m_fNowTime;
	bool				m_bStart;
	float				m_fAlpha;
	float				m_fDelta;
	D3DXHANDLE hTech;
	D3DXMATRIX              m_matWorld;
	D3DXMATRIX				m_matTrans;
	D3DXMATRIX				m_matRo;
	D3DXVECTOR3				m_vStart;
	D3DXVECTOR3				m_vFinish;
public:
	void Setup(char* Textfilename, float size);
	virtual	void Update();
	virtual void Render();
	virtual void Start();
	virtual void Stop();
	LPD3DXEFFECT LoadEffect(const char * szFileName);
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
public:
	TeicFire();
	~TeicFire();
};

