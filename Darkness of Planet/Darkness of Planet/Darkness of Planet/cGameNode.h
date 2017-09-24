#pragma once
#include "cGameNode.h"
#include "structs.h"

class cSceneInfo
{
public:
	std::list< LightOmni_s* >			m_pLightOmni;
	std::list< LightDirectional_s* >	m_pLightDirectional;
	std::list< LightSpot_s* >			m_pLightSpot;

	D3DXVECTOR4 m_vecAmbientColor;
};


class cGameNode
{
protected:
private:
	bool _managerInit;


public:
	cGameNode();
	virtual ~cGameNode();

	virtual HRESULT Setup();
	virtual HRESULT Setup(bool _managerInit);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	//bool About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	//virtual INT_PTR CALLBACK  About(HWND, UINT, WPARAM, LPARAM);

	//게임 씬의 정보를 가진다.
	cSceneInfo* m_pSceneInfo;
};
