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

	
	D3DXVECTOR3 temp = D3DXVECTOR3(200, 0, -100);
	/*char str[256];
	sprintf_s(str, "S(%d,%d,%d)L(0,0,0)", 200, 0, -100);
	m_pLoading->LoadWay2(str, NODEMANAGER->GetNode(), temp, D3DXVECTOR3(0, 0, 0));*/
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			char str[256];
			sprintf_s(str, "SX%dSZ%dLX%dLZ%d", 26+j,45+i, 116, 46);
			m_pLoading->LoadWay(str, NODEMANAGER->GetNode(), 26 + j, 45 + i, 116, 46);

		}
	}

	
	//m_pLoading->LoadMesh("", MESH_NORMAL, "object/xFile/", "MP5_Aim_Anims/HeroBodyLv",i,"_MP5.X");
	

	
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv1.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv1.X");
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv2.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv2.X");
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv4.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv4.X");
	for (int i = 1; i < 5; i++)
	{
		char str[128];
		sprintf_s(str, "MP5_Aim_Anims/HeroBodyLv%d_MP5.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "Pistol_Aim_Anims/HeroBodyLv%d_Pistol.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "Player_Melee/HeroBodyLv%d_Player_Melee.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "PR_Base_Anims/HeroBodyLv%d_Base.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "PR_IdleBreak_Anims/HeroBodyLv%d_IdleBreak.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "SMG_Aim_Anims/HeroBodyLv%d_SMG.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);

	}
	m_pLoading->LoadMesh("Weapon/Wp_AA12.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_AA12.X");
	m_pLoading->LoadMesh("Weapon/Wp_AR6.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_AR6.X");
	m_pLoading->LoadMesh("Weapon/Wp_M4.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_M4.X");
	m_pLoading->LoadMesh("Weapon/Wp_MP5.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_MP5.X");
	m_pLoading->LoadMesh("Weapon/Wp_Pistol.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_Pistol.X");
	m_pLoading->LoadMesh("Weapon/Wp_FireGun.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_FireGun.X");


	//m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");
	m_pLoading->LoadHeightMap("terrain", "map/", "final5.raw", "final5.png");

	//m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");
	
	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);

	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursor.png", &m_cursortex);
	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursorHit.png", &m_cursortex2);
	m_cursortex->GetSurfaceLevel(0, &surfcursor);
	GETDEVICE->SetCursorProperties(0, 0, surfcursor);
	
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
		SCENEMANAGER->changeScene("DarknessofPlanetMainScene");
	}
}

void LoadingScene::Render()
{
	m_pLoading->Render();
}