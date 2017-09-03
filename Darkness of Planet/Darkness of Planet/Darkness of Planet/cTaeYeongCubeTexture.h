#pragma once
class cTaeYeongCubeTexture
{
private:
	ST_PNT_VERTEX		m_aVertex[36];
	string				m_sBody;
	vector<cTaeYeongCubeTexture*> m_vecChild;
	IDirect3DVertexBuffer9* vb;
public:

	D3DXVECTOR3			position;
	D3DXVECTOR3			m_Center;
	D3DXVECTOR3			m_CenterMoving;
	D3DXVECTOR3         m_Forwarddir;
	D3DXMATRIXA16		matWorld;
	D3DXMATRIXA16		m_matWorld2;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXMATRIXA16		scaling;
	D3DXMATRIXA16		scalingInverse;
	D3DXMATRIXA16		 rotationX;
	D3DXMATRIXA16		 rotationY;
	D3DXMATRIXA16		 rotationZ;
	D3DXMATRIXA16		 move;
	float				 swingangle;
	int					swingdir;
	float				movez;
	float				movex;
	float				rotationyangle;
	cTaeYeongCubeTexture*		m_pParent;






	
	void Setup(D3DXVECTOR3 Scale, D3DXVECTOR3 RotationXYZ, D3DXVECTOR3 Move, const char* imageNamePlusWhere, char* body);
	void Setup(D3DXVECTOR3 Scale, D3DXVECTOR3 RotationXYZ, D3DXVECTOR3 Move, cTaeYeongCubeTexture* parent, const char* imageNamePlusWhere, char* body);
	D3DXMATRIXA16 GetParentWorld(cTaeYeongCubeTexture* parent);
	D3DXMATRIXA16 GetWorldmatrixNoScaling() { return m_matWorld2; };
	void Render();
	void Update();
	void Release();
	void Setscale(float x, float y, float z);
	void Addchild(cTaeYeongCubeTexture* child);

public:
	cTaeYeongCubeTexture();
	~cTaeYeongCubeTexture();
};

