#include "stdafx.h"
#include "cJustTestScene.h"
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"

CRITICAL_SECTION cs;

DWORD WINAPI ThFunc1(LPVOID lpParam)
{


	EnterCriticalSection(&cs);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cJustTestScene* temp = (cJustTestScene*)lpParam;
			TeicSkinnedMesh* pSkinnedMesh = new TeicSkinnedMesh("object/xFile/tiger/", "tiger.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(100+10*j, 0, 20*i));
			pSkinnedMesh->SetRandomTrackPosition();
			pSkinnedMesh->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, temp, 1));


			temp->m_vecEnemy.push_back(pSkinnedMesh);
		}
	}
	

	LeaveCriticalSection(&cs);




	return 0;
}

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
	m_pCharacter = new TeicSkinnedMesh("object/xFile/tiger/", "tiger.X");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pCharacter->SetRandomTrackPosition();
	m_pCharacter->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, this, 0));
	m_pCollisionMove = new TeicCollisionMove;
	m_pCollisionMove->SetSkinnedTarget(m_pCharacter);
	m_pCollisionMove->SetCallback(bind(&cJustTestScene::CallbackOn, this, 2));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pGrid->Setup();
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;
	D3D::SetLight();
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->LightEnable(0, true);
	InitializeCriticalSection(&cs);
	m_fTime = 0.0f;
	return S_OK;
}

void cJustTestScene::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCtrl);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);
}

void cJustTestScene::Update()
{

	if (TIMEMANAGER->getWorldTime() > m_fTime+10.0f)
	{
		m_fTime = 100000000000;
		DWORD dwThID1;
		HANDLE hThreads;

		unsigned long ulStackSize = 0;
		dwThID1 = 0;
		hThreads = NULL;
		hThreads = CreateThread(NULL, ulStackSize, ThFunc1, this, CREATE_SUSPENDED, &dwThID1);
		ResumeThread(hThreads);
	}


	m_pCtrl->Update();
	m_pCamera->Update();
	m_pCollisionMove->Update();
	if (m_pCharacter != NULL)
	{
		/*m_vecSkinnedMesh[0]->SetPosition(*m_pCrtCtrl->GetPosition());
		m_vecSkinnedMesh[0]->SetAngle(m_pCrtCtrl->getAngle());*/
		
		
		/*m_pCharacter->SetPosition(*m_pCtrl->GetPosition());
		m_pCharacter->SetRotationAngle(m_pCtrl->getAngle());*/
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			m_pCollisionMove->SetSpeed(5);
			m_pCollisionMove->SetFrom(m_pCharacter->GetPosition());
			m_pCollisionMove->SetTo(D3DXVECTOR3(RND->getFromFloatTo(0, 100), 0, RND->getFromFloatTo(0, -100)));
			//m_pCollisionMove->SetTo(D3DXVECTOR3(5, 0, -5));

			m_pCollisionMove->Start();
		}
		
		if (m_pCtrl->getMoving())
		{

			if (m_pCharacter->GetAninum() != 9)
			{
				m_pCharacter->SetAnimation(9);
			}
		}
		else if (m_pCtrl->getAttacking())
		{
			if (m_pCharacter->GetAninum() != 1)
			{
				m_pCharacter->SetAnimation(1);
			}
		}
		else
		{
			if (m_pCharacter->GetAninum() != 0)
			{
				m_pCharacter->SetAnimation(0);
			}
		}
	}
	//m_pMap->GetHeight(m_pCtrl->GetPosition()->x, m_pCtrl->GetPosition()->y, m_pCtrl->GetPosition()->z);

	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);
}

void cJustTestScene::CallbackOn(int number)
{
	//////0 캐릭터   1 적   2  적 움직임
	if (number == 0)
	{
		if (m_pCharacter->GetAninum() == 9)
		{
			m_pCtrl->setMoving(false);
		}
		if (m_pCharacter->GetAninum() == 1)
		{
			m_pCtrl->setAttacking(false);
		}
	}
	else if (number == 1)
	{

	}
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pCharacter) m_pCharacter->UpdateAndRender();
}

