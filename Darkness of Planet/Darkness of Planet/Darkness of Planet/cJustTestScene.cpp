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
	m_pGrid = new Hank::cGrid;
	m_pCamera->Setup();
	m_pGrid->Setup();

	return S_OK;
}

void cJustTestScene::Release()
{
	m_pGrid->Release();
}

void cJustTestScene::Update()
{
	m_pCamera->Update();
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
}

