#pragma once
#include "cGameNode.h"
class StartScene:public cGameNode
{
public:
	LPD3DXFONT	m_pFont;
	RECT		m_rt;
	char		str[512];
public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();

	virtual void Update();
public:
	StartScene();
	~StartScene();
};

