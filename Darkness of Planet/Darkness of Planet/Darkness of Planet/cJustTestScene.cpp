#include "stdafx.h"
#include "cJustTestScene.h"

cJustTestScene::cJustTestScene()
{
}

cJustTestScene::~cJustTestScene()
{
}

HRESULT cJustTestScene::Setup()
{
	m_pCamera = new Hank::cCamera;
	m_pCamera->Setup();
	//드 래곤 볼 1성 gu        ////////////찾았습니다  김태영
	return S_OK;
}

void cJustTestScene::Release()
{
}

void cJustTestScene::Update()
{
	m_pCamera->Update();
}

void cJustTestScene::Render()
{
	
}

