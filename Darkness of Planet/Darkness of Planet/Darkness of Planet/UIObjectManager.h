#pragma once
#include "cSingletonBase.h"

class cUIObject;

class UIObjectManager : public cSingletonBase<UIObjectManager>
{
private:
	typedef map<string, cUIObject*>					UIObjects;
	typedef map<string, cUIObject*>::iterator		UIObjectsIter;

private:
	UIObjects				m_mapUIObject;
	UIObjectsIter			m_mapIter;
	LPD3DXSPRITE			m_pSprite;

public:
	UIObjectManager();
	~UIObjectManager();

	void AddRoot(string key, cUIObject* root);
	void AddChild(string key, cUIObject* chield);
	cUIObject* FindRoot(string key);

	void Setup();
	void Update();
	void Render();
	void Destroy();
};

