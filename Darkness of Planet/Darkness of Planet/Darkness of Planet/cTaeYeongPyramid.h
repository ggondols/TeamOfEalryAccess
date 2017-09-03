#pragma once
class cTaeYeongPyramid
{
private:
	ST_PN_VERTEX		m_aPyramid[18];
	D3DXMATRIXA16		matWorld;
	IDirect3DVertexBuffer9* vb;
public:

	void Setup(float x, float y, float z, float RotationZangle, float RotationYangle, DWORD color = D3DCOLOR_XRGB(255, 255, 255));
	void Render();
	void Release();
public:
	cTaeYeongPyramid();
	~cTaeYeongPyramid();
};

