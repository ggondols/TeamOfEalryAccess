#include "stdafx.h"
#include "StartScene.h"
#include "cUIImageView.h"
#include "cFontManager.h"

StartScene::StartScene()
{
	m_pFont = NULL;
}


StartScene::~StartScene()
{
	//SAFE_RELEASE(m_pFont);
}

HRESULT StartScene::Setup()
{
	cUIImageView* pLodingBgImage = new cUIImageView;
	pLodingBgImage->SetTexture("./UI/loadingBg.png");
	UIOBJECTMANAGER->AddRoot("LoadingBg", pLodingBgImage, true);


	m_pFont =FONTMANAGER->GetFont(cFontManager::E_LODING);
	m_rt = RectMake(1200, 600, 200, 200);
	sprintf_s(str, "Start");
	SOUNDMANAGER->play("BGM_Avenge");
	
	return S_OK;
}

void StartScene::Release()
{
	UIOBJECTMANAGER->ReleaseRoot("LoadingBg");
}

void StartScene::Render()
{
	UIOBJECTMANAGER->Render("LoadingBg");
	m_pFont->DrawTextA(NULL, str, strlen(str), &m_rt, DT_CENTER, D3DCOLOR_XRGB(255, 255, 255));
}

void StartScene::Update()
{
	UIOBJECTMANAGER->Update("LoadingBg");

	if (PtInRect(&m_rt, g_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			
			SCENEMANAGER->changeScene("DarknessofPlanetMainScene");
		}
	}
}
