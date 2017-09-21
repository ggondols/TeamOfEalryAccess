#pragma once
class cSkyCloud
{
private:

	LPD3DXEFFECT			m_pEffect;
	LPD3DXMESH				m_pMesh;
	DWORD					m_dMatarial;
	D3DXHANDLE				m_hTexture;
	D3DXHANDLE				m_hMatWorld;
	D3DXHANDLE				m_hMatView;
	D3DXHANDLE				m_hMatProjection;
	D3DXHANDLE				m_hFSharpness;
	D3DXHANDLE				m_hFTime;
	int						m_fDegree;
	float					ftest;
	float					m_trans;


public:
	cSkyCloud();
	~cSkyCloud();

	void Setup();
	void Update();
	void Render();

	LPD3DXEFFECT LoadEffect(const char* szFileName);
};

