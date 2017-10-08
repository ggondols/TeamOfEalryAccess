#pragma once
#include "TeicEffectParent.h"
class TeicEffect:public TeicEffectParent
{
private:
	
	LPD3DXMESH				m_pMesh;
	LPD3DXMESH				m_pMesh2;
	LPD3DXMESH				m_pMesh3;
	LPD3DXEFFECT			m_pEffect;
	LPDIRECT3DTEXTURE9		m_pFlame;
	LPDIRECT3DTEXTURE9		m_pNoise;
	LPDIRECT3DTEXTURE9      m_pfire_opacity;
	float		m_fSize;
	int			m_iSpriteXnum;
	int			m_iSpriteYnum;
	float		m_fTimeChange;
	bool				m_bStart;
	D3DXHANDLE hTech;
public:
	void Setup(char* Textfilename, float size,int Xnum, int Ynum);
	virtual	void Update();
	virtual void Render();
	virtual void Start();
	virtual void Stop();
	LPD3DXEFFECT LoadEffect(const char * szFileName);
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
public:
	TeicEffect();
	~TeicEffect();
};

