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

void UIObjectManager::SetShowState(string key, bool isShow)
{
	m_mapShowStateIter = m_mapUIShowState.find(key);

	if (m_mapShowStateIter != m_mapUIShowState.end())
	{
		m_mapShowStateIter->second = isShow;
	}
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

void UIObjectManager::Destroy()
{
	for (UIObjectsIter it = m_mapUIObject.begin(); it != m_mapUIObject.end(); it++)
	{
		SAFE_RELEASE(it->second);
	}

	m_mapUIObject.clear();
}
