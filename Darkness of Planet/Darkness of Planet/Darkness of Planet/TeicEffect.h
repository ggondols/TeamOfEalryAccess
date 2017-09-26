#pragma once
class TeicEffect
{
private:
	LPD3DXMESH	m_pMesh;
	float		m_fSize;
	int			spriteXnum;
	int			spriteYnum;
public:
	void Setup(char* Textfilename, float size,int Xnum, int Ynum);
	void Update();
	void Render();
	TeicEffect();
	~TeicEffect();
};

