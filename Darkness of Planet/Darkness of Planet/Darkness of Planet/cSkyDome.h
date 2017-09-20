#pragma once


class cSkyDome
{
private:

	LPD3DXEFFECT			m_pEffect;
	LPD3DXMESH				m_pMesh;
	DWORD					m_dMatarial;
	D3DXHANDLE				m_hTexture;
	D3DXHANDLE				m_hMatWorld;
	D3DXHANDLE				m_hMatView;
	D3DXHANDLE				m_hMatProjection;

public:
	cSkyDome();
	~cSkyDome();

	void Setup();
	void Update();
	void Render();

	LPD3DXEFFECT LoadEffect(const char* szFileName);

};

