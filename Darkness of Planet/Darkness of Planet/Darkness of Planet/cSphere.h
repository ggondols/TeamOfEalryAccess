#pragma once
class cSphere
{
private:
	LPD3DXMESH	m_pMesh;
	D3DXMATRIXA16 m_matworld;

	D3DMATERIAL9 Whitematerial;
	D3DMATERIAL9 Redmaterial;
	bool Red;
public:
	D3DXVECTOR3				m_center;
	int tilex, tiley;
	cSphere();
	~cSphere();
	void Setup(float x, float y);
	void Setup(D3DXVECTOR3 center, float radius);
	void cSphere::setGo2(float x, float y);
	void Render();
	void Update();
	void SetRed(bool on) { Red = on; };

};

