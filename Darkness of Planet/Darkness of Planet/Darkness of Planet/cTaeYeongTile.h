#pragma once
class cTaeYeongTile
{
private:
	std::vector<ST_PC_VERTEX> m_vecTile;
	IDirect3DVertexBuffer9* vb;
public:
	
	void Setup(int num, float distance);
	void Render();
	void Release();
public:
	cTaeYeongTile();
	~cTaeYeongTile();
};

