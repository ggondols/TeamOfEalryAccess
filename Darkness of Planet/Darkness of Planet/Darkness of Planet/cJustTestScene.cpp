#include "stdafx.h"
#include "cJustTestScene.h"
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"

cJustTestScene::cJustTestScene()
	: m_pMap(NULL)
	, m_pCtrl(NULL)
{
}

cJustTestScene::~cJustTestScene()
{
}

HRESULT cJustTestScene::Setup()
{
	m_pCamera = new Hank::cCamera;
	m_pGrid = new Hank::cGrid;
	m_pCtrl = new TeicPhysicsCrtCtrl;
	/*m_pSkinnedMesh = new cSkinnedMesh("object/xFile/Wolf", "Wolf.X");
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, this, 0));*/

	m_pCamera->Setup(m_pCtrl->GetPosition());
	m_pGrid->Setup();
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;

	return S_OK;
}

void cJustTestScene::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCtrl);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
}

void cJustTestScene::Update()
{
	m_pCtrl->Update();
	m_pMap->GetHeight(m_pCtrl->GetPosition()->x, m_pCtrl->GetPosition()->y, m_pCtrl->GetPosition()->z);
	m_pCamera->Update();
}

void cJustTestScene::CallbackOn(int n)
{
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
}

