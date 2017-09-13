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

void UIObjectManager::AddRoot(string key, cUIObject * root)
{
	cUIObject* findUI = FindRoot(key);
	if (!findUI)
	{
		m_mapUIObject.insert(make_pair(key, root));
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

cUIObject * UIObjectManager::FindRoot(string key)
{
	m_mapIter = m_mapUIObject.find(key);

	if (m_mapIter != m_mapUIObject.end())
	{
		return m_mapIter->second;
	}

	return nullptr;
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
		p.second->Render(m_pSprite);
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
