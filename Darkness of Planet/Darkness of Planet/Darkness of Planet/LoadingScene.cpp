#include "stdafx.h"
#include "LoadingScene.h"
#include "Loading.h"

LoadingScene::LoadingScene()
	: m_pLoading(NULL)
{
}

LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::Setup()
{
	m_pLoading = new Loading;
	m_pLoading->Setup();

	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);
	
		
	return S_OK;
}

void LoadingScene::Release()
{
	m_pLoading->Release();
	SAFE_DELETE(m_pLoading);
}

void LoadingScene::Update()
{
	m_pLoading->Update();

	if (!m_pLoading->LoadNext())
	{
		SCENEMANAGER->changeScene("cTestMain");
	}
}

void LoadingScene::Render()
{
	m_pLoading->Render();
}