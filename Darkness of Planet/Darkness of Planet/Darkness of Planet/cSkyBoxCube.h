#pragma once
class LDYCamera;

class cSkyBoxCube
{
private:

	LPD3DXEFFECT			m_pEffect;
	LPD3DXMESH				m_pMesh;
	DWORD					m_dMatarial;
	LPDIRECT3DCUBETEXTURE9	m_pCubeTexture;
	D3DXHANDLE				m_hViewProjection;
	D3DXHANDLE				m_hTexture;
	D3DXHANDLE				m_hvViewPosition;



public:
	cSkyBoxCube();
	~cSkyBoxCube();


	void Setup();
	void Update();
	void Render(LDYCamera* pCamera);

	LPD3DXEFFECT LoadEffect(const char* szFileName);

};

