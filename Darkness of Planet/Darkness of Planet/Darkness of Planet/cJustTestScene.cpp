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
	m_pSkinnedMesh = new TeicSkinnedMesh("object/xFile/Wolf/", "Wolf.X");
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, this, 0));

	m_pCamera->Setup(m_pCtrl->GetPosition());
	m_pGrid->Setup();
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;
	D3D::SetLight();
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->LightEnable(0, true);

	//노드 생성 부분
	int sizeX = 10;
	int sizeZ = 10;

	m_pNodeGrid.m_vCol.resize(sizeZ);
	for (int i = 0; i < sizeZ; i++)
	{
		//m_pNodeGrid.m_vCol[i].m_vRow.resize(sizeX);
		for (int j = 0; j < sizeX; j++)
		{
			//HankcNode temp(i, j, 1);
			//m_pNodeGrid.m_vCol[i].m_vRow[j] = temp; //동적 할당용 
			m_pNodeGrid.m_vCol[i].m_vRow.push_back(HankcNode(i, j, 1)); //정적 할당용
			//코드 수정이 필요
		}
	}

	return S_OK;
}

void cJustTestScene::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCtrl);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSkinnedMesh);
}

void cJustTestScene::Update()
{
	m_pCtrl->Update();
	m_pMap->GetHeight(m_pCtrl->GetPosition()->x, m_pCtrl->GetPosition()->y, m_pCtrl->GetPosition()->z);
	m_pCamera->Update();
	if (m_pSkinnedMesh != NULL)
	{
		/*m_vecSkinnedMesh[0]->SetPosition(*m_pCrtCtrl->GetPosition());
		m_vecSkinnedMesh[0]->SetAngle(m_pCrtCtrl->getAngle());*/
		
		
		m_pSkinnedMesh->SetPosition(*m_pCtrl->GetPosition());
		m_pSkinnedMesh->SetRotationAngle(m_pCtrl->getAngle());
		
		if (m_pCtrl->getMoving())
		{

			if (m_pSkinnedMesh->GetAninum() != 9)
			{
				m_pSkinnedMesh->SetAnimation(9);
			}
		}
		else if (m_pCtrl->getAttacking())
		{
			if (m_pSkinnedMesh->GetAninum() != 1)
			{
				m_pSkinnedMesh->SetAnimation(1);
			}
		}
		else
		{
			if (m_pSkinnedMesh->GetAninum() != 0)
			{
				m_pSkinnedMesh->SetAnimation(0);
			}
		}
	}
}

void cJustTestScene::CallbackOn(int n)
{
	if (n == 0)
	{
		if (m_pSkinnedMesh->GetAninum() == 9)
		{
			m_pCtrl->setMoving(false);
		}
		if (m_pSkinnedMesh->GetAninum() == 1)
		{
			m_pCtrl->setAttacking(false);
		}
	}
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pSkinnedMesh) m_pSkinnedMesh->UpdateAndRender();
}

