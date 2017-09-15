#pragma once
#include "cSingletonBase.h"

class cUIObject;

class UIObjectManager : public cSingletonBase<UIObjectManager>
{
private:
	typedef map<string, cUIObject*>					UIObjects;
	typedef map<string, bool>						UIShowState;
	typedef map<string, cUIObject*>::iterator		UIObjectsIter;
	typedef map<string, bool>::iterator				UIShowStateIter;

private:
	UIObjects				m_mapUIObject;
	UIShowState				m_mapUIShowState;

	UIObjectsIter			m_mapUIObjectIter;
	UIShowStateIter			m_mapShowStateIter;

	LPD3DXSPRITE			m_pSprite;

public:
	UIObjectManager();
	~UIObjectManager();

	void AddRoot(string key, cUIObject* root, bool isShow = false);
	void AddChild(string key, cUIObject* chield);
	void SetShowState(string key, bool isShow);

	cUIObject* FindRoot(string key);
	cUIObject* GetChildByTag(string key, int tag);
	bool CheckShowState(string key);

	void Setup();
	void Update();
	void Render();
	void Destroy();
};

