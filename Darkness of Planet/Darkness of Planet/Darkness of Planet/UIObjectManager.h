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

	void Clamp(float& x, float& y);

public:
	UIObjectManager();
	~UIObjectManager();

	void AddRoot(string key, cUIObject* root, bool isShow = false);
	void AddChild(string key, cUIObject* chield);
	void AddRoot(string key, UI_TYPE type, bool isShow = false);
	void AddChild(string key, UI_TYPE type);

	void SetPosition(string key, float x, float y, float xDetails = 0.0f, float yDetails = 0.0f);
	void SetPosition(string key, int tag, float x, float y, float xDetails = 0.0f, float yDetails = 0.0f);
	void SetSize(string key, float x, float y);
	void SetSize(string key, int tag, float x, float y);

	POINT GetPosition(string key);
	POINT GetPosition(string key, int tag);

	// UIImageView에서만 사용..
	void SetAlpha(string key, DWORD alpha);
	void SetAlpha(string key, int tag, DWORD alpha);
	void SetTexture(string key, string fileAddress);
	void SetTexture(string key, int tag, string fileAddress);
	void SetScale(string key, float x, float y);
	void SetScale(string key, int tag, float x, float y);
	// UIImageView에서만 사용 여기까지..

	// UITextView에서만 사용..
	void SetText(string key, string text);
	void SetText(string key, int tag, string text);
	void SetTime(string key, float* time);
	void SetTime(string key, int tag, float* time);
	void SetDrawTextFormat(string key, DWORD formet);
	void SetDrawTextFormat(string key, int tag, DWORD formet);
	// UITextView에서만 사용.. 여기까지..

	void SetShowState(string key, bool isShow);
	void ReleaseRoot(string key);

	cUIObject* FindRoot(string key);
	cUIObject* GetChildByTag(string key, int tag);
	cUIObject* GetSelectChild(string key);
	cUIObject* GetSelectChild(string key, cUIObject* elseObject);
	bool CheckCollidedRect(string key, float x, float y);
	bool CheckCollidedRect(string key, int tag, float x, float y);
	bool CheckShowState(string key);

	void Setup();
	void Update();
	void Update(string key);
	void Render();
	void Render(string key);
	void Destroy();
};

