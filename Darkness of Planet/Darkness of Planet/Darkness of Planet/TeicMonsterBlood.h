#pragma once
#include "TeicEffectParent.h"
class TeicMonsterBlood:public TeicEffectParent
{
private:
	LPD3DXEFFECT			m_pEffect;
	LPDIRECT3DTEXTURE9		m_pTexture;
	ST_PT_VERTEX			m_stVertex[6];
	int					m_iAlpha;
	int					m_iDelta;
	bool					m_bStart;
	D3DXMATRIX				m_World;
public:
	virtual void Update();
	virtual void Start();
	virtual void Render();
	virtual void Stop();
	virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
	virtual bool IsRunning();
public:
	LPD3DXEFFECT LoadEffect(const char * szFileName);
public:
	TeicMonsterBlood();
	~TeicMonsterBlood();
};

