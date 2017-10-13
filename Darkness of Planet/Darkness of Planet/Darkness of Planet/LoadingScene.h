#pragma once
#include "cGameNode.h"

class Loading;

class LoadingScene : public cGameNode
{
private:

	bool			m_bCursor;

	LPDIRECT3DSURFACE9 surfcursor;
	LPDIRECT3DTEXTURE9 m_cursortex;
	LPDIRECT3DTEXTURE9 m_cursortex2;


	
public:

	Loading* m_pLoading;
	HANDLE thread1;
	DWORD dwThread1;
	bool			m_bIsthreading;
	CRITICAL_SECTION cs;
public:
	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();

	virtual void Update();

	LoadingScene();
	~LoadingScene();
};

