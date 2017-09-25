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

void UIObjectManager::SetPosition(string key, float x, float y)
{
	FindRoot(key)->SetPosition(x, y);
}

void UIObjectManager::SetPosition(string key, int tag, float x, float y)
{
	FindRoot(key)->GetChildByTag(tag)->SetPosition(x, y);
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

	pUIObject = FindRoot(key)->GetChildByPosition(g_ptMouse);

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
