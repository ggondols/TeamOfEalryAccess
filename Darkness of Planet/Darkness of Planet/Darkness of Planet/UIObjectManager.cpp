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

	cUIImageView* pImageView = new cUIImageView;
	pImageView->SetTexture("NULL");
	AddRoot("test1", pImageView);

	cUITextView* pTextView = new cUITextView;
	pTextView->SetText("UI 텍스트 출력 실험용..");
	pTextView->SetSize(ST_SIZE(312, 200));
	pTextView->SetPosition(100, 100);
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTag(0);
	AddChild("test1", pTextView);

	cUIImageView* pImageView2 = new cUIImageView;
	pImageView2->SetTexture("NULL");
	AddRoot("test2", pImageView2);

	cUITextView* pTextView2 = new cUITextView;
	pTextView2->SetText("UI 텍스트 출력 실험용2..");
	pTextView2->SetSize(ST_SIZE(312, 200));
	pTextView2->SetPosition(200, 200);
	pTextView2->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView2->SetTag(1);
	AddChild("test2", pTextView2);
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
