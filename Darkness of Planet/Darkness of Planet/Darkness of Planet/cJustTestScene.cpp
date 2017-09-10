#include "stdafx.h"
#include "cJustTestScene.h"
#include "iMap.h"
#include "cHeightMap.h"

cJustTestScene::cJustTestScene()
	: m_pMap(NULL)
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
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;

	return S_OK;
}

void cJustTestScene::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pMap);
}

void cJustTestScene::Update()
{
	m_pCamera->Update();
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
}

