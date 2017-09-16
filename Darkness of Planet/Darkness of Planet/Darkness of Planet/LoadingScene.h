#pragma once
#include "cGameNode.h"

class Loading;

class LoadingScene : public cGameNode
{
private:
	Loading* m_pLoading;

public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();

	LoadingScene();
	~LoadingScene();
};

