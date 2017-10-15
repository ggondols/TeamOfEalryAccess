#pragma once
#include "cGameNode.h"
class EndingScene:public cGameNode
{
public:
	LPD3DXFONT	m_pFont;
	LPD3DXFONT	m_pFont2;
	RECT		m_rt;
	RECT		m_rt2;
	char		str[512];
	char		time[512];
public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();

	virtual void Update();
public:
	EndingScene();
	~EndingScene();
};

