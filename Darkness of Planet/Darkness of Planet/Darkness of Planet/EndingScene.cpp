#include "stdafx.h"
#include "EndingScene.h"
#include "cUIImageView.h"
#include "cFontManager.h"


EndingScene::EndingScene()
{
	m_pFont = NULL;
}


EndingScene::~EndingScene()
{
}

HRESULT EndingScene::Setup()
{
	cUIImageView* pLodingBgImage = new cUIImageView;
	pLodingBgImage->SetTexture("./UI/ending.jpg");
	UIOBJECTMANAGER->AddRoot("LoadingBg", pLodingBgImage, true);


	m_pFont = FONTMANAGER->GetFont(cFontManager::E_ENDING);
	m_pFont2 = FONTMANAGER->GetFont(cFontManager::E_LODING);
	m_rt = RectMake(200, 100, 1000, 1000);
	sprintf_s(str, "VICTORY");
	sprintf_s(time, "FinishTime: %.2fÃÊ", TIMEMANAGER->getWorldTime());


	m_rt2 = RectMake(200, 650, 1000, 1000);
	return S_OK;
}

void EndingScene::Release()
{

}

void EndingScene::Render()
{
	UIOBJECTMANAGER->Render("LoadingBg");
	m_pFont->DrawTextA(NULL, str, strlen(str), &m_rt, DT_CENTER, D3DCOLOR_XRGB(255, 255, 255));
	m_pFont2->DrawTextA(NULL, time, strlen(time), &m_rt2, DT_CENTER, D3DCOLOR_XRGB(255, 255, 255));
}

void EndingScene::Update()
{
	UIOBJECTMANAGER->Update("LoadingBg");

	
}
