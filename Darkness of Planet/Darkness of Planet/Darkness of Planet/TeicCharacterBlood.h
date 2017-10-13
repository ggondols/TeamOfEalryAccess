#pragma once
#include "TeicEffectParent.h"
class TeicCharacterBlood:public TeicEffectParent
{
private:
	LPD3DXEFFECT			m_pEffect;
	LPDIRECT3DTEXTURE9		m_pTexture[3];
	ST_PT_VERTEX			m_stVertex[6];
	float					m_fAlpha;
	float					m_fDelta;
	bool					m_bStart;
public:
		virtual void Update();
		virtual void Start();
		virtual void Render();
		virtual void Stop() ;
		virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
		virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
		virtual bool IsRunning();
public:
	LPD3DXEFFECT LoadEffect(const char * szFileName);
public:
	TeicCharacterBlood();
	~TeicCharacterBlood();
};

