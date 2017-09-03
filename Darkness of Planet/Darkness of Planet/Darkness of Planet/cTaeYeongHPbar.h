#pragma once
class cTaeYeongHPbar
{
private:
	ID3DXMesh*			Mesh;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DMATERIAL9		Whitematerial;
	D3DXVECTOR3			m_vPosition;
	D3DXMATRIX			m_HPStatus;
	vector<cTaeYeongHPbar*>		m_vecChild;
public:
	ST_PNT_VERTEX		m_Vertex[6];
	float				m_fHeight;
	float				m_fXcut;
public:
	
	void AddChild(cTaeYeongHPbar* child);
	void Render();
	void Render(D3DXMATRIX parent, float x);
	void Update(D3DXVECTOR3& position);
	void SetHP(float x); ////0~1
	void Setup(char* Fullpath, D3DXVECTOR3& position, float height);
	void Setup(char* Fullpath, D3DXVECTOR3& position, float height, float xCut);

public:
	cTaeYeongHPbar();
	~cTaeYeongHPbar();
};

