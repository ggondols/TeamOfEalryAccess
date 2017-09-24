#pragma once
#include "iMap.h"

class cHeightMap : public iMap
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

	virtual void Load(string szFolder,
		string szFile,
		string szTexture,
		DWORD dwBytesPerPixel = 1);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render() override;
	LPD3DXMESH getMesh() override { return m_pMesh; }
};

