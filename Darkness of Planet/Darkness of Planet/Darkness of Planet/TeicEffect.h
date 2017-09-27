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
	float		m_fSize;
	int			m_iSpriteXnum;
	int			m_iSpriteYnum;
	float		m_fTimeChange;
	D3DXHANDLE hTech;
public:
	void Setup(char* Textfilename, float size,int Xnum, int Ynum);
	virtual	void Update();
	virtual void Render();
	virtual void Start();
	virtual void Stop();
	LPD3DXEFFECT LoadEffect(const char * szFileName);
	TeicEffect();
	~TeicEffect();
};

