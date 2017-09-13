#include "stdafx.h"
#include "cJustTestScene.h"
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"

CRITICAL_SECTION cs;

DWORD WINAPI ThFunc1(LPVOID lpParam)
{
	cJustTestScene* temp = (cJustTestScene*)lpParam;


	EnterCriticalSection(&cs);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			
			TeicEnemy* pSkinnedMesh = new TeicEnemy;
			pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(3 * i + 200, 0, -(100 + 3 * j)));
			//	pSkinnedMesh->SetPosition(D3DXVECTOR3(0,0,0));

			pSkinnedMesh->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, temp, 1));


			temp->m_vecEnemy.push_back(pSkinnedMesh);
			TeicCollisionMove* tempmove;
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(pSkinnedMesh->GetSkinnedMesh());
			tempmove->SetCallback(bind(&cJustTestScene::CallbackOn, temp, 2));
			tempmove->SetSpeed(5);
			temp->m_vecEnemyCollisionMove.push_back(tempmove);

		}
	}

	temp->m_bThread = true;
	LeaveCriticalSection(&cs);




	return 0;
}

cJustTestScene::cJustTestScene()
	: m_pMap(NULL)
	, m_pNode(NULL)
	, m_bThread(false)
{
}

cJustTestScene::~cJustTestScene()
{
}

HRESULT cJustTestScene::Setup()
{
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	cUIImageView* pAimImage = new cUIImageView;
	pAimImage->SetTexture("./UI/aimNormal.png");
	pAimImage->SetPosition(viewport.Width / 2.0f - 15, viewport.Height / 2.0f - 15);
	pAimImage->SetIsCenter(true);
	pAimImage->SetScale(0.1f, 0.1f);

	UIOBJECTMANAGER->AddRoot("aimTest", pAimImage, true);

	cUIImageView* pLifeImageDown = new cUIImageView;
	pLifeImageDown->SetTexture("./UI/lifeBarDown.bmp");
	pLifeImageDown->SetPosition(10, viewport.Height - 30);
	cUIImageView* pLifeImageUp = new cUIImageView;
	pLifeImageUp->SetTexture("./UI/lifeBarUp.bmp");

	UIOBJECTMANAGER->AddRoot("lifeTest", pLifeImageDown, true);
	UIOBJECTMANAGER->AddChild("lifeTest", pLifeImageUp);

	m_pCamera = new Hank::cCamera;
	m_pGrid = new Hank::cGrid;

	m_pCharacter = new TeicCharacter;
	m_pCharacter->Setup("object/xFile/tiger/", "tiger.X");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&cJustTestScene::CallbackOn, this, 0));

	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pGrid->Setup();
	cHeightMap* pHeightMap = new cHeightMap;
	pHeightMap->Load("map/", "HeightMap.raw", "terrain.jpg");
	m_pMap = pHeightMap;

	//노드 추가 합니다.
	m_pNode = new HankcGrid;
	int sizeX = 257;
	int sizeZ = 257;
	m_pNode->m_vRow.resize(sizeZ);
	for (int i = 0; i < sizeZ; i++)
	{
		m_pNode->m_vRow[i].m_vCol.resize(sizeX);
		for (int j = 0; j < sizeX; j++)
		{
			m_pNode->m_vRow[i].m_vCol[j].InitFrame(i, j); // 일반 노드만 생성합니다.
			m_pNode->m_vRow[i].m_vCol[j].InitPosition(1); // 생성된 노드를 기반으로 버텍스와 중점 좌상단점을 설정합니다.
		}
	}

//	m_pNode->m_vCol[0].m_vRow[0].m_pBoundInfo
//////////////////여기서 부터 다시

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

	SAFE_DELETE(m_pNode);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pUITest);
	SAFE_RELEASE(m_pUITest2);
	SAFE_DELETE(m_pCharacter);
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		SAFE_DELETE(m_vecEnemy[i]);
		SAFE_RELEASE(m_vecEnemyCollisionMove[i]);
	}
}

void cJustTestScene::Update()
{
	cUIImageView* pAim = (cUIImageView*)UIOBJECTMANAGER->FindRoot("aimTest");
	pAim->SetTexture("./UI/aimNormal.png");
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		pAim->SetTexture("./UI/aimHit.png");
	}

	if (KEYMANAGER->isOnceKeyDown('H'))
	{
		cUIObject* child = UIOBJECTMANAGER->FindRoot("lifeTest")->GetChildByTag(1);
		child->SetSize(ST_SIZE(child->GetSize().fWidth - 20 > 0? child->GetSize().fWidth - 20 : 0,
			child->GetSize().fHeight));
	}

	if (TIMEMANAGER->getWorldTime() > m_fTime + 10.0f)
	{
		m_fTime = 10.0f;
		DWORD dwThID1;
		HANDLE hThreads;

		unsigned long ulStackSize = 0;
		dwThID1 = 0;
		hThreads = NULL;
		hThreads = CreateThread(NULL, ulStackSize, ThFunc1, this, CREATE_SUSPENDED, &dwThID1);
		ResumeThread(hThreads);
	}

	if (TIMEMANAGER->getWorldTime() > 25 - m_fTime)
	{
		m_fTime = INF;
		if (m_bThread)
		{
			for (int i = 0; i < m_vecEnemyCollisionMove.size(); i++)
			{
				m_vecEnemyCollisionMove[i]->SetFrom(m_vecEnemy[i]->GetPosition());
				
				m_vecEnemyCollisionMove[i]->SetTo(m_pNode->m_vRow[m_pCharacter->GetNodeNum().y].m_vCol[m_pCharacter->GetNodeNum().x].m_vPosList->m_vCenterPos);
				m_vecEnemyCollisionMove[i]->Start();

			}
		}
	}

	m_pCamera->Update();
	
	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->SetCollision(false);
		}
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			for (int j = i + 1; j < m_vecEnemy.size(); j++)
			{
				CollisionCheck(m_vecEnemy[i], m_vecEnemy[j]);
			}
		}
		for (int i = 0; i < m_vecEnemyCollisionMove.size(); i++)
		{
			m_vecEnemyCollisionMove[i]->Update();


			if (m_vecEnemyCollisionMove[i]->m_bStart)
			{
				if (m_vecEnemy[i]->GetAninum() != 1)
				{
					m_vecEnemy[i]->SetAnimation(1);
				}
			}
		}
	}
	//m_pMap->GetHeight(m_pCtrl->GetPosition()->x, m_pCtrl->GetPosition()->y, m_pCtrl->GetPosition()->z);
	m_pCharacter->Update();
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		m_pMap->GetHeight(m_vecEnemy[i]->GetPositionPointer()->x, m_vecEnemy[i]->GetPositionPointer()->y, m_vecEnemy[i]->GetPositionPointer()->z);

	}

	UIOBJECTMANAGER->Update();
}

void cJustTestScene::CallbackOn(int number)
{
	//////0 캐릭터   1 적   2  적 움직임
	if (number == 0)
	{
		if (m_pCharacter->GetAninum() == 1)
		{
			m_pCharacter->SetMoving(false);
		}
		if (m_pCharacter->GetAninum() == 3)
		{
			m_pCharacter->SetAttacking(false);
		}
	}
	else if (number == 1)
	{

	}
}

bool cJustTestScene::CollisionCheck(TeicEnemy * A, TeicEnemy * B)
{
	if (D3DXVec3Length(&(A->GetPosition() - B->GetPosition())) < 1.5)
	{
		float Adist = D3DXVec3Length(&(A->GetPosition() - m_pCharacter->GetPosition()));
		float Bdist = D3DXVec3Length(&(B->GetPosition() - m_pCharacter->GetPosition()));
		if (Adist < Bdist)
		{
			B->SetCollision( true);
		}
		else
		{
			A->SetCollision(true);
		}
		return true;
	}


	return false;
}

void cJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pCharacter) m_pCharacter->UpdateAndRender();
	/*for each (auto p in m_vecEnemy)
	{
		p->UpdateAndRender();
	}*/
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		m_vecEnemy[i]->UpdateAndRender();
	}

	UIOBJECTMANAGER->Render();
}

