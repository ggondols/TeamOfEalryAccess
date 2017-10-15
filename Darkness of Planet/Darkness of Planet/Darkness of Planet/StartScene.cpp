#include "stdafx.h"
#include "StartScene.h"
#include "cUIImageView.h"


StartScene::StartScene()
{
}


StartScene::~StartScene()
{
}

HRESULT StartScene::Setup()
{
	cUIImageView* pLodingBgImage = new cUIImageView;
	pLodingBgImage->SetTexture("./UI/loadingBg.png");
	UIOBJECTMANAGER->AddRoot("LoadingBg", pLodingBgImage, true);
	return S_OK;
}

void StartScene::Release()
{
}

void StartScene::Render()
{
	UIOBJECTMANAGER->Render("LoadingBg");
}

void StartScene::Update()
{
	UIOBJECTMANAGER->Update("LoadingBg");
}
