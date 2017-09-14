#pragma once
#include "cGameNode.h"
class cTestMain:public cGameNode
{
public:
	LPD3DXFONT	m_pFont;
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	cTestMain();
	~cTestMain();
};

