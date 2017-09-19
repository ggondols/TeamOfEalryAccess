#include "stdafx.h"
#include "LDYcJustTestScene.h"
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "LDYCharacter.h"
#include "cSkyBoxCube.h"

static CRITICAL_SECTION cs;


static DWORD WINAPI ThFunc1(LPVOID lpParam)
{
	LDYcJustTestScene* temp = (LDYcJustTestScene*)lpParam;


	EnterCriticalSection(&cs);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			TeicEnemy* pSkinnedMesh = new TeicEnemy;
			pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(3 * i + 200, 0, -(100 + 3 * j)));
			//	pSkinnedMesh->SetPosition(D3DXVECTOR3(0,0,0));

			pSkinnedMesh->SetCallbackfunction(bind(&LDYcJustTestScene::CallbackOn, temp, 1));


			temp->m_vecEnemy.push_back(pSkinnedMesh);
		}
	}




	temp->m_vecEnemyWay.resize(temp->m_vecEnemy.size());
	temp->m_vecEnemyCollisionMove.resize(temp->m_vecEnemy.size());


	vector<D3DXVECTOR3> wayway = temp->m_pAstar->FindWay(temp->m_vecEnemy[0]->GetNodeNum().x, temp->m_vecEnemy[0]->GetNodeNum().y,
		temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);
	temp->m_EnemyTarget = wayway[wayway.size() - 1];
	for (int i = 0; i < temp->m_vecEnemyWay.size(); i++)
	{
		temp->m_vecEnemyWay[i] = wayway;
		temp->m_vecEnemyCollisionMove[i] = new TeicMoveSequence;

		for (int j = 0; j < temp->m_vecEnemyWay[i].size(); j++)
		{
			if (j + 1 >= temp->m_vecEnemyWay[i].size())break;
			TeicCollisionMove* tempmove;
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(temp->m_vecEnemy[i]->GetSkinnedMesh());
			tempmove->SetSpeed(5);
			tempmove->SetFrom(temp->m_vecEnemyWay[i][j]);
			tempmove->SetTo(temp->m_vecEnemyWay[i][j + 1]);
			temp->m_vecEnemyCollisionMove[i]->AddAction(tempmove);

		}
	}

	temp->m_bThread = true;
	LeaveCriticalSection(&cs);

	return 0;
}



LDYcJustTestScene::LDYcJustTestScene()
	: /*m_pMap(NULL)*/
	m_pNode(NULL)
	, m_bThread(false)
	, m_pUITest(NULL)
	, m_pSprite(NULL)
	, m_pAstar(NULL)
	, m_EnemyTarget(D3DXVECTOR3(0, 0, 0))
	, m_iBodyUpgrade(1)
	, m_iHeadUpgrade(1)
	, m_eWeaponType(Wp_Melee)
	, m_eStateType(ST_Idle)
	, m_pSkyBox(NULL)
{
}


LDYcJustTestScene::~LDYcJustTestScene()
{
	SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pNode);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCharacter);
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		SAFE_DELETE(m_vecEnemy[i]);
		SAFE_DELETE(m_vecEnemyCollisionMove[i]);

	}

	SAFE_DELETE(m_pSkyBox);

}

HRESULT LDYcJustTestScene::Setup()
{
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	cUIImageView* pAimImage = new cUIImageView;
	pAimImage->SetTexture("./UI/aimNormal.png");
	pAimImage->SetPosition(viewport.Width / 2.0f - 15, viewport.Height / 2.0f - 15);
	pAimImage->SetIsCenter(true);
	pAimImage->SetScale(0.1f, 0.1f);

	UIOBJECTMANAGER->AddRoot("aimTest", pAimImage, false);

	cUIImageView* pLifeImageDown = new cUIImageView;
	pLifeImageDown->SetTexture("./UI/lifeBarDown.bmp");
	pLifeImageDown->SetPosition(10, viewport.Height - 30);
	cUIImageView* pLifeImageUp = new cUIImageView;
	pLifeImageUp->SetTexture("./UI/lifeBarUp.bmp");

	UIOBJECTMANAGER->AddRoot("lifeTest", pLifeImageDown, true);
	UIOBJECTMANAGER->AddChild("lifeTest", pLifeImageUp);

	cUIImageView* pInventoryImage = new cUIImageView;
	pInventoryImage->SetTexture("./UI/inventory.png");
	UIOBJECTMANAGER->AddRoot("inventory", pInventoryImage, false);

	m_pCamera = new LDYCamera;
	m_pGrid = new Hank::cGrid;

	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, m_iBodyUpgrade);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&LDYcJustTestScene::CallbackOn, this, 0));

	//m_pCamera->Setup(m_pCharacter->GetPositionPointer());
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


	m_pAstar = new TeicAstar;
	m_pAstar->Setup(m_pNode);

	D3D::SetLight();
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->LightEnable(0, true);
	InitializeCriticalSection(&cs);
	m_fTime = 0.0f;
	m_fTime2 = 0.0f;
	m_fTime3 = 0.0f;

	m_pSkyBox = new cSkyBoxCube;
	m_pSkyBox->Setup();


	return S_OK;
}

void LDYcJustTestScene::Release()
{
	m_pGrid->Release();

	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pUITest);
}

void LDYcJustTestScene::Update()
{
	cUIImageView* pAim = (cUIImageView*)UIOBJECTMANAGER->FindRoot("aimTest");
	pAim->SetTexture("./UI/aimNormal.png");

	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		UIOBJECTMANAGER->SetShowState("inventory", !UIOBJECTMANAGER->CheckShowState("inventory"));
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		UIOBJECTMANAGER->SetShowState("aimTest", !UIOBJECTMANAGER->CheckShowState("aimTest"));
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		pAim->SetTexture("./UI/aimHit.png");
	}

	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		cUIObject* child = UIOBJECTMANAGER->GetChildByTag("lifeTest", 1);
		child->SetSize(ST_SIZE(child->GetSize().fWidth - 20 > 0 ? child->GetSize().fWidth - 20 : 0,
			child->GetSize().fHeight));
	}

	if (TIMEMANAGER->getWorldTime() > m_fTime3 + 1.0f)
	{
		m_fTime3 = TIMEMANAGER->getWorldTime();
	}
	if (TIMEMANAGER->getWorldTime() > m_fTime + 5.0f)
	{
		m_fTime = INF;
	}
	if (m_bThread)
	{
		if (TIMEMANAGER->getWorldTime() > m_fTime2 + 20.0f)
		{
			m_fTime2 = INF;

			if (m_bThread)
			{
				for (int i = 0; i < m_vecEnemyCollisionMove.size(); i++)
				{
					m_vecEnemyCollisionMove[i]->Start();
				}
			}
		}
	}

	m_pCamera->Update(m_pCharacter->GetPosition());

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
	m_pCharacter->Update();
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);

	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_pMap->GetHeight(m_vecEnemy[i]->GetPositionPointer()->x, m_vecEnemy[i]->GetPositionPointer()->y, m_vecEnemy[i]->GetPositionPointer()->z);


		}
	}
	ChangeGridInfo();
	if (m_pSkyBox) m_pSkyBox->Update();
	UIOBJECTMANAGER->Update();
}

void LDYcJustTestScene::CallbackOn(int number)
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

bool LDYcJustTestScene::CollisionCheck(TeicEnemy * A, TeicEnemy * B)
{
	if (D3DXVec3Length(&(A->GetPosition() - B->GetPosition())) < 1.5)
	{
		float Adist = D3DXVec3Length(&(A->GetPosition() - m_EnemyTarget));
		float Bdist = D3DXVec3Length(&(B->GetPosition() - m_EnemyTarget));
		if (Adist < Bdist)
		{
			B->SetCollision(true);
		}
		else
		{
			A->SetCollision(true);
		}
		return true;
	}


	return false;
}

void LDYcJustTestScene::ChangeGridInfo()
{
	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			POINT temp;
			D3DXVECTOR3 pos = m_vecEnemy[i]->GetPosition();
			pos.y = 0;
			temp.x = pos.x;
			temp.y = -(pos.z);
			m_vecEnemy[i]->m_PresentGrid = temp;
			if (m_vecEnemy[i]->m_PreviousGrid.x != m_vecEnemy[i]->m_PresentGrid.x ||
				m_vecEnemy[i]->m_PreviousGrid.y != m_vecEnemy[i]->m_PresentGrid.y)
			{
				if (m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo)
					m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.clear();
				if (!m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo)
					m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo = new nNodeBoundInfo;
				m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo->m_vecBounding.push_back(m_vecEnemy[i]->m_BoundingBox);
				m_vecEnemy[i]->m_PreviousGrid = m_vecEnemy[i]->m_PresentGrid;
			}
		}
	}

}

void LDYcJustTestScene::TargetOn()
{
	if (m_bThread)
	{
		for (int i = 0; i < 1; i++)
		{
			if (EnemyPlayerDistance(m_vecEnemy[i]) < 10.0f)
			{
				m_vecEnemyCollisionMove[i]->SetClear();

				m_vecEnemyWay[i] = m_pAstar->FindWay(m_vecEnemy[i]->GetNodeNum().x, m_vecEnemy[i]->GetNodeNum().y,
					m_pCharacter->GetNodeNum().x, m_pCharacter->GetNodeNum().y);

				for (int j = 0; j < m_vecEnemyWay[i].size(); j++)
				{
					if (j + 1 >= m_vecEnemyWay[i].size())break;
					TeicCollisionMove* tempmove;
					tempmove = new TeicCollisionMove;
					tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
					tempmove->SetSpeed(5);
					tempmove->SetFrom(m_vecEnemyWay[i][j]);
					tempmove->SetTo(m_vecEnemyWay[i][j + 1]);
					m_vecEnemyCollisionMove[i]->AddAction(tempmove);
				}
				m_EnemyTarget = m_vecEnemyWay[i][m_vecEnemyWay[i].size() - 1];
				m_vecEnemyCollisionMove[i]->Start();
			}
		}
	}
}

float LDYcJustTestScene::EnemyPlayerDistance(TeicEnemy *ene)
{
	float length;
	length = D3DXVec3Length(&(ene->GetPosition() - m_pCharacter->GetPosition()));
	return length;
}

void LDYcJustTestScene::Render()
{
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pCharacter) m_pCharacter->UpdateAndRender();
	if (m_pSkyBox)m_pSkyBox->Render(m_pCamera);
	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->UpdateAndRender();
		}
	}

	UIOBJECTMANAGER->Render();
}

