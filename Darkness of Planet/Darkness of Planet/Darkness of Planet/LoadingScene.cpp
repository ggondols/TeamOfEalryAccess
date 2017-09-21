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

	


	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(3 * i + 200, 0, -(100 + 3 * j));
			char str[256];
			sprintf_s(str, "S(%d,%d,%d)L(0,0,0)", (int)temp.x, (int)temp.y, (int)temp.z);
			m_pLoading->LoadWay2(str, NODEMANAGER->GetNode(), temp, D3DXVECTOR3(0, 0, 0));

		}
	}
	
	m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");
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