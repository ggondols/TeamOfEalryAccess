#pragma once
#include "cGameNode.h"
class StartScene:public cGameNode
{
public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();

	virtual void Update();
public:
	StartScene();
	~StartScene();
};

