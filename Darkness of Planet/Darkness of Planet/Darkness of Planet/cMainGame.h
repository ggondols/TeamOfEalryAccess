#pragma once
#include "cGameNode.h"

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

class cMainGame : public cGameNode
{
public:
	cMainGame();
	virtual ~cMainGame();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
	

};

