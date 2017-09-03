#pragma once
class cHeightMap
{
private:
	LPD3DXMESH			m_pMesh;
	string				m_sTexture;
	D3DMATERIAL9		m_stMtl;
	int					m_nTile;
	vector<D3DXVECTOR3>	m_vecVertex;
public:
	cHeightMap(void);
	virtual ~cHeightMap(void);

	virtual void Load(char* szFolder,
		char* szFile,
		char* szTexture,
		DWORD dwBytesPerPixel = 1);

	void Render();
};

