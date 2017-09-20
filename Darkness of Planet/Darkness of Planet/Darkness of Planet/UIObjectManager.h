#pragma once
#include "cSingletonBase.h"

class cUIObject;

enum UI_TYPE
{
	UITYPE_IMAGE,
	UITYPE_TEXT,
	UITYPE_BUTTON,
	UITYPE_END
};

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
	void AddRoot(string key, UI_TYPE type, bool isShow = false);
	void AddChild(string key, UI_TYPE type);

	void SetPosition(string key, float x, float y);
	void SetPosition(string key, int tag, float x, float y);

	// UIImageView에서만 사용..
	void SetTexture(string key, string fileAddress);
	void SetTexture(string key, int tag, string fileAddress);
	void SetScale(string key, float x, float y);
	void SetScale(string key, int tag, float x, float y);
	// UIImageView에서만 사용 여기까지..

	void SetShowState(string key, bool isShow);
	void ReleaseRoot(string key);

	cUIObject* FindRoot(string key);
	cUIObject* GetChildByTag(string key, int tag);
	bool GetCollidedRect(string key, float x, float y);
	bool GetCollidedRect(string key, int tag, float x, float y);
	bool CheckShowState(string key);

	void Setup();
	void Update();
	void Update(string key);
	void Render();
	void Render(string key);
	void Destroy();
};

