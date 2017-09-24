#pragma once

class LDYCharacter;
class LDYiMap;
class LDYHeightMap;

class cShadowMapping
{

private:

	LPD3DXEFFECT			m_pCreateShadow;
	LPD3DXEFFECT			m_pApplyShadow;

	LPD3DXMESH				m_pDisc;
	LPD3DXMESH				m_pTorus;

	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DTEXTURE9		m_pMapRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;

	//create fx용핸들
	D3DXHANDLE				m_hCmatWorld;
	D3DXHANDLE				m_hCmatLightView;
	D3DXHANDLE				m_hCmatLightProjection;

	//apply fx용 핸들
	D3DXHANDLE				m_hApplyTexture;
	D3DXHANDLE				m_hAmatWorld;
	D3DXHANDLE				m_hAmatLightView;
	D3DXHANDLE				m_hAmatLightProjection;
	D3DXHANDLE				m_hAm_vec4LightPosition;
	D3DXHANDLE				m_hAmatViewProjection;
	D3DXHANDLE				m_hAgObjectColor;

	D3DXMATRIXA16			matWorld;
	float					gRotationY;
	D3DXMATRIXA16  matView, matProjection, matViewProjection, matCWorld;

	D3DXVECTOR4				m_vec4LightPosition;
	D3DXVECTOR4				gObjectColor;
	D3DXVECTOR4				gDiscColor;
	D3DXMATRIXA16			matS;
	D3DXMATRIXA16			matLightView;
	D3DXMATRIXA16			matLightProjection;

	LDYCharacter*			m_pCharacter;
	LDYHeightMap*							m_pMap;
	LPD3DXMESH mapmesh;
	LPD3DXEFFECT			m_pMapShadow;
public:
	cShadowMapping();
	~cShadowMapping();
	
	void Setup();
	void Update();
	void Render();

	LPD3DXEFFECT LoadEffect(const char* szFileName);


};

