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
	m_pSkinnedMesh = new TeicSkinnedMesh("object/xFile/wolf/", "wolf.X");
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, this, 0));
	m_pCollisionMove = new TeicCollisionMove;
	m_pCollisionMove->SetSkinnedTarget(m_pSkinnedMesh);
	m_pCollisionMove->SetCallback(bind(&cJustTestScene::CallbackOn, this, 1));
	m_pCamera->Setup(m_pSkinnedMesh->GetPositionPointer());
	m_pGrid->Setup();
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;
	D3D::SetLight();
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->LightEnable(0, true);
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
	m_pCamera->Update();
	m_pCollisionMove->Update();
	if (m_pSkinnedMesh != NULL)
	{
		/*m_vecSkinnedMesh[0]->SetPosition(*m_pCrtCtrl->GetPosition());
		m_vecSkinnedMesh[0]->SetAngle(m_pCrtCtrl->getAngle());*/
		
		
		/*m_pSkinnedMesh->SetPosition(*m_pCtrl->GetPosition());
		m_pSkinnedMesh->SetRotationAngle(m_pCtrl->getAngle());*/
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			m_pCollisionMove->SetSpeed(5);
			m_pCollisionMove->SetFrom(m_pSkinnedMesh->GetPosition());
			m_pCollisionMove->SetTo(D3DXVECTOR3(RND->getFromFloatTo(0, 100), 0, RND->getFromFloatTo(0, -100)));
			//m_pCollisionMove->SetTo(D3DXVECTOR3(5, 0, -5));

			m_pCollisionMove->Start();
		}
		
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
	//m_pMap->GetHeight(m_pCtrl->GetPosition()->x, m_pCtrl->GetPosition()->y, m_pCtrl->GetPosition()->z);

	m_pMap->GetHeight(m_pSkinnedMesh->GetPositionPointer()->x, m_pSkinnedMesh->GetPositionPointer()->y, m_pSkinnedMesh->GetPositionPointer()->z);
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
	else if (n == 1)
	{

	}
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pSkinnedMesh) m_pSkinnedMesh->UpdateAndRender();
}

