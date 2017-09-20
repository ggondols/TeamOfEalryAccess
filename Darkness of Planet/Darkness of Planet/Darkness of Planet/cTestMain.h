#pragma once
#include "cGameNode.h"
class cTestMain:public cGameNode
{
public:
	bool			m_bCursor;
	LPDIRECT3DSURFACE9 surfcursor;
	LPDIRECT3DTEXTURE9 m_cursortex;

public:
	LPD3DXFONT	m_pFont;
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();

	IDirect3DSurface9* LoadBMP(std::string filename, IDirect3DSurface9* surface);

	cTestMain();
	~cTestMain();
};

