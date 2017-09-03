#pragma once
class cTaeYeongCube
{
private:

	D3DXVECTOR3			position;
	D3DXMATRIXA16		matWorld;
	float				rotationangle;
	LPDIRECT3DTEXTURE9 m_pTexture;
	IDirect3DVertexBuffer9* vb;
	ID3DXMesh*			Mesh;
public:
	ST_PNT_VERTEX		m_aVertex[36];
	
	void Update();
	void Setup();
	void Setup(char* fullpath, int x, int y);
	void Render();
	void Release();
	void SetMesh();
	D3DXVECTOR3* getPosition() { return &position; };
public:
	cTaeYeongCube();
	~cTaeYeongCube();
};

