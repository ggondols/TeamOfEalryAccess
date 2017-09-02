#pragma once
#include "cGameNode.h"

class cJustTestScene : public cGameNode
{
public:
	cJustTestScene();
	virtual ~cJustTestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
};