#include "stdafx.h"
#include "UIObjectManager.h"
#include "cUIButton.h"
#include "cUIImageView.h"
#include "cUITextView.h"

UIObjectManager::UIObjectManager()
	: m_pSprite(NULL)
{
}

UIObjectManager::~UIObjectManager()
{
}

void UIObjectManager::Clamp(float & x, float & y)
{
	if (x > 1.0f) x = 1.0f;
	else if (x < 0.0f) x = 0.0f;

	if (y > 1.0f) y = 1.0f;
	else if (y < 0.0f) y = 0.0f;
}

void UIObjectManager::AddRoot(string key, cUIObject * root, bool isShow /*= false*/)
{
	cUIObject* findUI = FindRoot(key);
	if (!findUI)
	{
		m_mapUIObject.insert(make_pair(key, root));
		m_mapUIShowState.insert(make_pair(key, isShow));
	}
}

void UIObjectManager::AddChild(string key, cUIObject * child)
{
	cUIObject* findUI = FindRoot(key);
	if (findUI)
	{
		child->SetTag(findUI->GetChildSize() + 1);
		findUI->AddChild(child);
	}
}

void UIObjectManager::AddRoot(string key, UI_TYPE type, bool isShow)
{
	cUIObject* findUI = FindRoot(key);

	if (!findUI)
	{
		cUIObject* pRoot = NULL;

		switch (type)
		{
		case UITYPE_IMAGE:
			pRoot = new cUIImageView;
			break;
		case UITYPE_TEXT:
			pRoot = new cUITextView;
			break;
		case UITYPE_BUTTON:
			pRoot = new cUIButton;
			break;
		}

		if (pRoot)
		{
			m_mapUIObject.insert(make_pair(key, pRoot));
			m_mapUIShowState.insert(make_pair(key, isShow));
		}
	}
}

void UIObjectManager::AddChild(string key, UI_TYPE type)
{
	cUIObject* findUI = FindRoot(key);
	if (findUI)
	{
		cUIObject* pChild = NULL;

		switch (type)
		{
		case UITYPE_IMAGE:
			pChild = new cUIImageView;
			pChild->SetTag(findUI->GetChildSize() + 1);
			pChild->SetAlpha(findUI->GetAlpha());
			findUI->AddChild(pChild);
			break;
		case UITYPE_TEXT:
			pChild = new cUITextView;
			pChild->SetTag(findUI->GetChildSize() + 1);
			findUI->AddChild(pChild);
			break;
		case UITYPE_BUTTON:
			pChild = new cUIButton;
			pChild->SetTag(findUI->GetChildSize() + 1);
			findUI->AddChild(pChild);
			break;
		}
	}
}

//인자->루트의 키네임 : x, y의화면중에서의 비율(0.0f~1.0f) : x, y값에 수치값으로 추가하고싶으면 추가(기본0.0f로 설정되어있음)
void UIObjectManager::SetPosition(string key, float x, float y, float xDetails/* = 0.0f*/, float yDetails/* = 0.0f*/)
{
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	//x, y 가 1.0f보다 크면 1.0f, 0.0f보다 작으면 0.0f로 변환
	Clamp(x, y);
	FindRoot(key)->SetPosition((viewport.Width * x) + xDetails, (viewport.Height * y) + yDetails);
}

//인자->루트의 키네임 : 차일드의 테그번호(첫번째 차일드는 1) : 루트의 width, height의 퍼센트(0.0f~1.0f) : 수치값으로 추가(기본0.0f)
void UIObjectManager::SetPosition(string key, int tag, float x, float y, float xDetails/* = 0.0f*/, float yDetails/* = 0.0f*/)
{
	//x, y 가 1.0f보다 크면 1.0f, 0.0f보다 작으면 0.0f로 변환
	Clamp(x, y);
	cUIObject* parent = FindRoot(key);
	parent->GetChildByTag(tag)->SetPosition((parent->GetSize().fWidth * x) + xDetails, (parent->GetSize().fHeight * y) + yDetails);
}

void UIObjectManager::SetSize(string key, float x, float y)
{
	FindRoot(key)->SetSize(ST_SIZE(x, y));
}

void UIObjectManager::SetSize(string key, int tag, float x, float y)
{
	FindRoot(key)->GetChildByTag(tag)->SetSize(ST_SIZE(x, y));
}

void UIObjectManager::GetPosition(string key, OUT float & x, OUT float & y)
{
	x = FindRoot(key)->GetPointPosition().x;
	y = FindRoot(key)->GetPointPosition().y;
}

void UIObjectManager::GetPosition(string key, int tag, OUT float & x, OUT float & y)
{
	x = FindRoot(key)->GetChildByTag(tag)->GetPointPosition().x;
	y = FindRoot(key)->GetChildByTag(tag)->GetPointPosition().y;
}

void UIObjectManager::SetAlpha(string key, DWORD alpha)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key);
	if (pImageView) pImageView->SetAlpha(alpha);
}

void UIObjectManager::SetAlpha(string key, int tag, DWORD alpha)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key)->GetChildByTag(tag);
	if (pImageView) pImageView->SetAlpha(alpha);
}

void UIObjectManager::SetTexture(string key, string fileAddress)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key);
	if(pImageView) pImageView->SetTexture(fileAddress);
}

void UIObjectManager::SetTexture(string key, int tag, string fileAddress)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key)->GetChildByTag(tag);
	if (pImageView) pImageView->SetTexture(fileAddress);
}

void UIObjectManager::SetScale(string key, float x, float y)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key);
	if (pImageView) pImageView->SetScale(x, y);
}

void UIObjectManager::SetScale(string key, int tag, float x, float y)
{
	cUIImageView* pImageView = (cUIImageView*)FindRoot(key)->GetChildByTag(tag);
	if (pImageView) pImageView->SetScale(x, y);
}

void UIObjectManager::SetText(string key, string text)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key);
	if (pTextView) pTextView->SetText(text);
}

void UIObjectManager::SetText(string key, int tag, string text)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key)->GetChildByTag(tag);
	if (pTextView) pTextView->SetText(text);
}

void UIObjectManager::SetTime(string key, float* time)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key);
	if (pTextView) pTextView->SetTimeAddress(time);
}

void UIObjectManager::SetTime(string key, int tag, float * time)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key)->GetChildByTag(tag);
	if (pTextView) pTextView->SetTimeAddress(time);
}

void UIObjectManager::SetDrawTextFormat(string key, DWORD formet)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key);
	if (pTextView) pTextView->SetDrawTextFormat(formet);
}

void UIObjectManager::SetDrawTextFormat(string key, int tag, DWORD formet)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key)->GetChildByTag(tag);
	if (pTextView) pTextView->SetDrawTextFormat(formet);
}

void UIObjectManager::SetTextColor(string key, D3DCOLOR color)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key);
	if (pTextView) pTextView->SetColor(color);
}

void UIObjectManager::SetTextColor(string key, int tag, D3DCOLOR color)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key)->GetChildByTag(tag);
	if (pTextView) pTextView->SetColor(color);
}

void UIObjectManager::SetFontType(string key, cFontManager::eFontType type)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key);
	if (pTextView) pTextView->SetFontType(type);
}

void UIObjectManager::SetFontType(string key, int tag, cFontManager::eFontType type)
{
	cUITextView* pTextView = (cUITextView*)FindRoot(key)->GetChildByTag(tag);
	if (pTextView) pTextView->SetFontType(type);
}

void UIObjectManager::SetShowState(string key, bool isShow)
{
	m_mapShowStateIter = m_mapUIShowState.find(key);

	if (m_mapShowStateIter != m_mapUIShowState.end())
	{
		m_mapShowStateIter->second = isShow;
	}
}

void UIObjectManager::ReleaseRoot(string key)
{
	m_mapUIObjectIter = m_mapUIObject.find(key);

	if (m_mapUIObjectIter != m_mapUIObject.end())
	{
		SAFE_RELEASE(m_mapUIObjectIter->second);
		SAFE_DELETE(m_mapUIObjectIter->second);
	}

	m_mapUIObject.erase(key);
	m_mapUIShowState.erase(key);
}

cUIObject * UIObjectManager::FindRoot(string key)
{
	m_mapUIObjectIter = m_mapUIObject.find(key);

	if (m_mapUIObjectIter != m_mapUIObject.end())
	{
		return m_mapUIObjectIter->second;
	}

	return nullptr;
}

cUIObject * UIObjectManager::GetChildByTag(string key, int tag)
{
	return FindRoot(key)->GetChildByTag(tag);
}

cUIObject * UIObjectManager::GetSelectChild(string key)
{
	cUIObject* pUIObject = NULL;

	pUIObject = FindRoot(key)->GetChildByPosition(g_ptMouse);

	return pUIObject;
}

cUIObject * UIObjectManager::GetSelectChild(string key, cUIObject * elseObject)
{
	cUIObject* pUIObject = NULL;

	pUIObject = FindRoot(key)->GetChildByPosition(g_ptMouse, elseObject);

	return pUIObject;
}

bool UIObjectManager::CheckCollidedRect(string key, float x, float y)
{
	RECT rc;
	FindRoot(key)->GetRect(&rc);

	if(PtInRect(&rc, g_ptMouse))
	{
		return true;
	}
	return false;
}

bool UIObjectManager::CheckCollidedRect(string key, int tag, float x, float y)
{
	RECT rc;
	FindRoot(key)->GetChildByTag(tag)->GetRect(&rc);

	if (PtInRect(&rc, g_ptMouse))
	{
		return true;
	}
	return false;
}

bool UIObjectManager::CheckShowState(string key)
{
	m_mapShowStateIter = m_mapUIShowState.find(key);

	if (m_mapShowStateIter != m_mapUIShowState.end())
	{
		return m_mapShowStateIter->second;
	}

	return false;
}

void UIObjectManager::Setup()
{
	D3DXCreateSprite(GETDEVICE, &m_pSprite);
}

void UIObjectManager::Update()
{
	for each(auto p in m_mapUIObject)
	{
		p.second->Update();
	}
}

void UIObjectManager::Update(string key)
{
	FindRoot(key)->Update();
}

void UIObjectManager::Render()
{
	for each(auto p in m_mapUIObject)
	{
		if (CheckShowState(p.first))
		{
			p.second->Render(m_pSprite);
		}
	}
}

void UIObjectManager::Render(string key)
{
	FindRoot(key)->Render(m_pSprite);
}

void UIObjectManager::Destroy()
{
	for (UIObjectsIter it = m_mapUIObject.begin(); it != m_mapUIObject.end(); it++)
	{
		SAFE_RELEASE(it->second);
	}

	m_mapUIObject.clear();
	m_mapUIShowState.clear();
}
