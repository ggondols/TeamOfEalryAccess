#include "stdafx.h"
#include "TeicJustTestScene.h"


#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cSkyDome.h"
#include "cSkyCloud.h"
#include "Inventory.h"

static CRITICAL_SECTION cs;


static DWORD WINAPI ThFunc1(LPVOID lpParam)
{
	EnterCriticalSection(&cs);
	TeicJustTestScene* temp = (TeicJustTestScene*)lpParam;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			TeicEnemy* pSkinnedMesh = new TeicEnemy;
			pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(3 * i + 200, 0, -(100 + 3 * j)));
			pSkinnedMesh->SetCallbackfunction(bind(&TeicJustTestScene::CallbackOn, temp, (i + 1) * 10 + j));
			pSkinnedMesh->SetAttack(5);
			pSkinnedMesh->SetHP(100);
		
			temp->m_vecEnemy.push_back(pSkinnedMesh);
			//temp->m_vecEnemy[i * 10 + j]->GetBoundingSquare()->m_pSkinnedObject = temp->m_vecEnemy[i * 10 + j]->GetSkinnedMesh();
		}
	}

	temp->m_vecEnemyWay.resize(temp->m_vecEnemy.size());
	temp->m_vecEnemyCollisionMove.resize(temp->m_vecEnemy.size());

	for (int i = 0; i < temp->m_vecEnemyWay.size(); i++)
	{
		char str[256];
		//sprintf_s(str, "S(%d,%d,%d)L(0,0,0)", (int)temp->m_vecEnemy[i]->GetPosition().x, (int)temp->m_vecEnemy[i]->GetPosition().y, (int)temp->m_vecEnemy[i]->GetPosition().z);

		//temp->m_vecEnemyWay[i] = WAYMANAGER->GetWay(str);
		//S(%d, %d, %d)L(0, 0, 0)", 200, 0, -100);
		temp->m_vecEnemyWay[i] = WAYMANAGER->GetWay("S(200,0,-100)L(0,0,0)");
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
		temp->m_vecEnemyCollisionMove[i]->Start();
		temp->m_vecEnemyWay[i].clear();
	}

	temp->m_bThread = true;
	LeaveCriticalSection(&cs);

	return 0;
}


static DWORD WINAPI ThFunc2(LPVOID lpParam)
{


	EnterCriticalSection(&cs);
	TeicJustTestScene* temp = (TeicJustTestScene*)lpParam;
	for (int i = 0; i < temp->m_vecEnemy.size(); i++)
	{
		if (!temp->m_vecEnemy[i]->m_bThreadCalOn)continue;
		if (temp->m_vecEnemy[i]->m_bAttackOn)continue;
		temp->m_pAttackNode.x = temp->m_pCharacter->GetNodeNum().x + RND->getFromIntTo(-1, 1);
		temp->m_pAttackNode.y = temp->m_pCharacter->GetNodeNum().y + RND->getFromIntTo(-1, 1);
		temp->m_vecEnemyWay[i] = temp->m_pAstarShort->FindWay(temp->m_vecEnemy[i]->GetNodeNum().x, temp->m_vecEnemy[i]->GetNodeNum().y,
			temp->m_pAttackNode.x, temp->m_pAttackNode.y, temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);



	}
	temp->m_bAstarThread = false;
	LeaveCriticalSection(&cs);



	return 0;
}


TeicJustTestScene::TeicJustTestScene()
	: m_pMap(NULL)
	, m_pNode(NULL)
	, m_bThread(false)
	, m_pAstar(NULL)
	, m_pAstarShort(NULL)
	, m_EnemyTarget(D3DXVECTOR3(0, 0, 0))
	, m_bAstarThread(false)
	/*, m_pCamera(NULL)*/
	, m_iBodyUpgrade(1)

	, m_pFont(NULL)
	, m_pCollision(NULL)
	, m_pShoot(NULL)
	, m_pSkyDome(NULL)
	, m_pSkyCloud(NULL)
	, m_pInventory(NULL)
	,m_fTime5(0.0f)
{
	m_vecAttackSlot.resize(8, false);
}


TeicJustTestScene::~TeicJustTestScene()
{
	SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pAstarShort);
	SAFE_DELETE(m_pShoot);
	//SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCharacter);
	SAFE_DELETE(m_pInventory);

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		SAFE_DELETE(m_vecEnemy[i]);
		SAFE_DELETE(m_vecEnemyCollisionMove[i]);

	}
	SAFE_DELETE(m_pCollision);
	SAFE_DELETE(m_pSkyDome);
	SAFE_DELETE(m_pSkyCloud);
}



HRESULT TeicJustTestScene::Setup()
{
	//////////정현
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

	m_pInventory = new Inventory;
	m_pInventory->Setup();


	///////////동윤




	/*m_pCamera = new LDYCamera;*/
	m_pGrid = new Hank::cGrid;



	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, m_iBodyUpgrade);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(D3DXVECTOR3(20, 0, -20));
	CAMERA->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&TeicJustTestScene::CallbackOn, this, 0));


	/////////////태영
	m_pFont = FONTMANAGER->GetFont(cFontManager::E_NORMAL);
	m_pGrid->Setup();

	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");

	//노드 추가 합니다.
	m_pNode = NODEMANAGER->GetNode();
	//m_pNode = new HankcGrid;
	//int sizeX = 257;
	//int sizeZ = 257;
	//m_pNode->m_vRow.resize(sizeZ);
	//for (int i = 0; i < sizeZ; i++)
	//{
	//	m_pNode->m_vRow[i].m_vCol.resize(sizeX);
	//	for (int j = 0; j < sizeX; j++)
	//	{
	//		m_pNode->m_vRow[i].m_vCol[j].InitFrame(i, j); // 일반 노드만 생성합니다.
	//		m_pNode->m_vRow[i].m_vCol[j].InitPosition(NodeLength); // 생성된 노드를 기반으로 버텍스와 중점 좌상단점을 설정합니다.
	//		m_pNode->m_vRow[i].m_vCol[j].m_pAstarNode = new HankcAstarNode;
	//	}
	//}


	m_pAstar = new TeicAstar;
	m_pAstar->Setup(m_pNode);
	m_pAstarShort = new TeicAstarShort;
	m_pAstarShort->Setup(m_pNode);
	D3D::SetLight();
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	GETDEVICE->LightEnable(0, true);
	InitializeCriticalSection(&cs);
	m_fTime = 0.0f;
	m_fTime2 = 0.0f;
	m_fTime3 = 0.0f;


	m_pBresenham = new TeicBresenham;




	m_pTempSPhere = new cSphere;

	m_pCollision = new TeicObbCollision;

	m_pShoot = new TeicShoot;
	m_pShoot->Setup(m_pNode, CAMERA, m_pCharacter);

	m_pTempEnemy = new TeicEnemy;
	m_pTempEnemy->Setup("object/xFile/wolf/", "wolf.X");
	m_pTempEnemy->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pTempEnemy->SetAnimation(0);

	m_pSkyDome = new cSkyDome;
	m_pSkyDome->Setup();

	m_pSkyCloud = new cSkyCloud;
	m_pSkyCloud->Setup();



	m_pBoss = new TeicBoss;


//	m_pBoss->Setup("object/xFile/Valak/", "Valak.X");
	m_pBoss->Setup("object/xFile/Unit/BattleCruiser/", "Battlecruiser.X");
	//m_pBoss->Setup("object/", "ice_Boom.X");
	//m_pBoss->Setup("sprites/", "ice_Boom.X");

	m_pBoss->SetPosition(D3DXVECTOR3(150, 40, -150));


	m_pBoss->SetAnimation(0);

	//m_pBoss->SetScaleSize(0.1);
	m_pBoss->SetScaleSize(10);

	


	return S_OK;
}

void TeicJustTestScene::Release()
{
	m_pGrid->Release();

	SAFE_DELETE(m_pMap);
	/*SAFE_DELETE(m_pCamera);*/
	SAFE_DELETE(m_pCharacter);


}

void TeicJustTestScene::Update()
{
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);
	m_pBoss->Update(m_pCharacter->GetPosition());
	
	if (KEYMANAGER->isOnceKeyDown('0'))
	{
		m_pBoss->SetAnimation(0);
	}
	if (m_pInventory) m_pInventory->Update(m_pCharacter);
	if (m_pSkyDome)m_pSkyDome->Update();
	if (m_pSkyCloud)m_pSkyCloud->Update();

	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		UIOBJECTMANAGER->SetShowState("inventory", !UIOBJECTMANAGER->CheckShowState("inventory"));
	}

	CAMERA->Update(&m_pCharacter->GetPosition(),0);
	m_pCharacter->Update(CAMERA->getAngleY());
	bool check = ChangeCheckPoint();
	if (m_bThread)
	{


		if (check)
		{
			m_fTime2 = 0;
		}
		if (TIMEMANAGER->getWorldTime() > m_fTime2 + 3.0f)
		{
			m_fTime2 = TIMEMANAGER->getWorldTime();
			if (!m_bAstarThread)
			{
				m_bAstarThread = true;
				DWORD dwThID2;
				HANDLE hThreads2;

				unsigned long ulStackSize = 0;
				dwThID2 = 1;
				hThreads2 = NULL;
				hThreads2 = CreateThread(NULL, ulStackSize, ThFunc2, this, CREATE_SUSPENDED, &dwThID2);
				ResumeThread(hThreads2);
			}

		}
	}

	if (TIMEMANAGER->getWorldTime() > m_fTime + 5.0f)
	{
		m_fTime = INF;
		DWORD dwThID1;
		HANDLE hThreads;

		unsigned long ulStackSize = 0;
		dwThID1 = 0;
		hThreads = NULL;
		hThreads = CreateThread(NULL, ulStackSize, ThFunc1, this, CREATE_SUSPENDED, &dwThID1);
		ResumeThread(hThreads);
	}



	if (m_bThread)
	{
		//CleanHit();
		float callbacktiming = GetCallbackTime();
		
		if (TIMEMANAGER->getWorldTime() > m_fTime4 + callbacktiming)
		{
			m_pCharacter->m_pCtrl->setAttacking(false);
		}
		if (KEYMANAGER->isStayKeyDown('Q'))
		{
			float fire = GetFireRate();
			if (TIMEMANAGER->getWorldTime() > m_fTime5 + fire)
			{
				m_fTime5 = TIMEMANAGER->getWorldTime();
				if (!m_pCharacter->GetAttacking())
				{
					m_pCharacter->m_pCtrl->setAttacking(true);
					m_pCharacter->m_pCtrl->m_fSpeed = 0;
					m_fTime4 = TIMEMANAGER->getWorldTime();
					if (m_pCharacter->getWeaponType() == WP_FireGun)
					{
						/*m_pShoot->Shoot(m_pCharacter->getWeaponType());
						D3DXVECTOR3 target = m_pShoot->GetStartPosition() + m_pShoot->GetDir() * 20;
						target.x += RND->getFromFloatTo(-2, 2);
						target.y += RND->getFromFloatTo(-2, 2);
						target.z += RND->getFromFloatTo(-2, 2);
						SKILLEFFECTMANAGER->play("Flame", target, m_pCharacter->getMuzzlePos() );*/

					}
					else
					{
						m_pShoot->Shoot(m_pCharacter->getWeaponType());
						CAMERA->rebound();
					}
					
				}
				else
				{
					m_fTime4 = TIMEMANAGER->getWorldTime();
					if (m_pCharacter->getWeaponType() == WP_FireGun)
					{
						

						m_pShoot->Shoot(m_pCharacter->getWeaponType());
						D3DXVECTOR3 target = m_pShoot->GetStartPosition() + m_pShoot->GetDir() * 20;
						target.x += RND->getFromFloatTo(-2, 2);
						target.y += RND->getFromFloatTo(-2, 2);
						target.z += RND->getFromFloatTo(-2, 2);
						SKILLEFFECTMANAGER->play("Flame", target, m_pCharacter->getMuzzlePos());

					}
					else
					{
						m_pShoot->Shoot(m_pCharacter->getWeaponType());
						CAMERA->rebound();
					}
				}
			}
			
		}
		CheckDie();


		WayUpdate();
		SlowUpdate();
		CheckSlot();
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (D3DXVec3Length(&(m_vecEnemy[i]->GetPositionYzero() - m_pCharacter->GetPositionYZero())) < m_vecEnemy[i]->m_fAttackRange)
			{
				if (!m_vecEnemy[i]->GetSlot())continue;
				m_vecEnemy[i]->m_bAttackOn = true;
				m_vecEnemy[i]->m_bThreadCalOn = false;
				m_vecEnemyCollisionMove[i]->SetClear();
			}
			/*else if (m_vecEnemy[i]->m_bAttackOn == false)
			{
				m_vecEnemy[i]->m_bAttackOn = false;
			}*/
			if (m_vecEnemy[i]->m_bAttackOn)
			{
				if (m_vecEnemy[i]->GetAninum() != 7)
				{
					m_vecEnemy[i]->SetAnimation(7);
				}
			}
		}


		TotalCollisionCheck();

		for (int i = 0; i < m_vecEnemyCollisionMove.size(); i++)
		{
			if (m_vecEnemy[i]->m_bAttackOn)continue;
			m_vecEnemyCollisionMove[i]->Update();

			if (m_vecEnemyCollisionMove[i]->m_bStart)
			{
				if (m_vecEnemy[i]->GetAninum() == 0)
				{
					if (!SameVector(m_vecEnemy[i]->GetPositionYzero(), m_vecEnemy[i]->m_vPreviousPosition))
					{
						if (m_vecEnemy[i]->GetAninum() != 1)
						{
							m_vecEnemy[i]->SetAnimation(1);
						}
					}

				}
				else if (m_vecEnemy[i]->GetAninum() != 1)
				{
					m_vecEnemy[i]->SetAnimation(1);
				}

			}
		}
		TotalPushCheck2();
		if (TIMEMANAGER->getWorldTime() > m_fTime3 + 0.5f)
		{
			m_fTime3 = TIMEMANAGER->getWorldTime();
			TargetOn();
		}
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (!m_vecEnemy[i]->m_bAttackOn)continue;
			AngleChange(m_vecEnemy[i]);

		}
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_pMap->GetHeight(m_vecEnemy[i]->GetPositionPointer()->x, m_vecEnemy[i]->GetPositionPointer()->y, m_vecEnemy[i]->GetPositionPointer()->z);
		}



		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (SameVector(m_vecEnemy[i]->GetPositionYzero(), m_vecEnemy[i]->m_vPreviousPosition))
			{
				if (m_vecEnemy[i]->m_bAttackOn == false)
				{
					if (m_vecEnemy[i]->GetAninum() != 0)
						m_vecEnemy[i]->SetAnimation(0);
				}
			}
			m_vecEnemy[i]->m_vPreviousPosition = m_vecEnemy[i]->GetPositionYzero();
		}

	}
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);

	ChangeGridInfo();


	UIOBJECTMANAGER->Update();


}

void TeicJustTestScene::CallbackOn(int number)
{
	//////0 캐릭터   10~적  

	///(i + 1)*10 + j   10~29
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
	for (int i = 10; i < 10 + m_vecEnemy.size(); i++)
	{
		if (number == i)
		{
			if (m_vecEnemy[i - 10]->GetAninum() == 7)
			{
				m_vecEnemy[i - 10]->m_bAttackOn = false;
				m_vecEnemy[i - 10]->SetAnimation(1);
			}
		}
	}

}

bool TeicJustTestScene::CollisionCheck(TeicEnemy * A, TeicEnemy * B)
{
	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
		if (m_pCollision->CheckCollision(A->GetBoundingSquare(), B->GetBoundingSquare()) == false)
			return false;
		float Adist = D3DXVec3Length(&(A->GetPositionYzero() - m_EnemyTarget));
		float Bdist = D3DXVec3Length(&(B->GetPositionYzero() - m_EnemyTarget));


		if (Adist < Bdist)
		{
			if (!m_bAttackOn)
			{
				B->SetCollision(true);
			}
			else
			{

				if (!B->m_bThreadCalOn)
				{
					B->m_bThreadCalOn = true;
				}
				if (EnemyPlayerDistance(B) > 10 * NodeLength)
				{
					B->SetCollision(true);
				}

			}


		}
		else
		{
			if (!m_bAttackOn)
			{
				A->SetCollision(true);
			}
			else
			{
				if (!A->m_bThreadCalOn)
				{
					A->m_bThreadCalOn = true;
				}
				if (EnemyPlayerDistance(A) > 10 * NodeLength)
				{
					A->SetCollision(true);
				}
			}


		}
		return true;
	}


	return false;
}



void TeicJustTestScene::TotalPushCheck2()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		for (int j = i + 1; j < m_vecEnemy.size(); j++)
		{
			Push2(m_vecEnemy[i], m_vecEnemy[j]);

		}
	}
	ChangeGridInfo();
	CheckSlot();
}

void TeicJustTestScene::ChangeGridInfo()
{
	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{

			m_vecEnemy[i]->m_PresentGrid = m_vecEnemy[i]->GetNodeNum();
		}

		/////////// 비교

		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (m_vecEnemy[i]->m_PreviousGrid.x == m_vecEnemy[i]->m_PresentGrid.x &&
				m_vecEnemy[i]->m_PreviousGrid.y == m_vecEnemy[i]->m_PresentGrid.y)continue;

			if (m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo != NULL)
			{
				for (int j = 0; j < m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.size(); j++)
				{
					if (m_vecEnemy[i]->GetSkinnedMesh() == m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
					{
						m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.erase(
							m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.begin() + j);

					}
				}
			}


			if (m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo == NULL)
			{
				m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo = new nNodeBoundInfo;
				m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo->m_vecBounding.push_back(
					m_vecEnemy[i]->GetBoundingSquare());
			}
			else
			{
				m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo->m_vecBounding.push_back(
					m_vecEnemy[i]->GetBoundingSquare());
			}
			m_vecEnemy[i]->m_PreviousGrid = m_vecEnemy[i]->GetNodeNum();
		}




	}

}

void TeicJustTestScene::TargetOn()
{


	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (EnemyPlayerDistance(m_vecEnemy[i]) < 30.0f)
		{
			m_bAttackOn = true;
		}
	}
	if (m_bAttackOn)
	{
		m_EnemyTarget = m_pCharacter->GetPositionYZero();
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->SetSpeed(10);
			if (m_vecEnemy[i]->m_bAttackOn)continue;
			if (m_vecEnemy[i]->m_bThreadCalOn)continue;

			m_vecEnemy[i]->m_bThreadCalOn = true;
			m_fTime3 = INF;


		}
	}

}

bool TeicJustTestScene::CheckStraight(TeicEnemy *A)
{
	D3DXVECTOR3 Ene = A->GetPosition();
	D3DXVECTOR3 Char = m_pCharacter->GetPosition();
	m_vecBresnhamNode = m_pBresenham->FindNode(Ene.x, Ene.z, Char.x, Char.z);
	for (int i = 0; i < m_vecBresnhamNode.size(); i++)
	{
		if (m_pNode->m_vRow[m_vecBresnhamNode[i].y].m_vCol[m_vecBresnhamNode[i].x].m_pBoundInfo != NULL)
		{
			for (int j = 0; j < m_pNode->m_vRow[m_vecBresnhamNode[i].y].m_vCol[m_vecBresnhamNode[i].x].m_pBoundInfo->m_vecBounding.size(); j++)
			{
				if (m_pNode->m_vRow[m_vecBresnhamNode[i].y].m_vCol[m_vecBresnhamNode[i].x].m_pBoundInfo->m_vecBounding[j]->st_Type == Bounding_Object)
				{
					return false;
				}
			}

		}
	}

	return true;
}

float TeicJustTestScene::EnemyEnemyDistance(TeicEnemy * A, TeicEnemy * B)
{
	float length;
	length = D3DXVec3Length(&(A->GetPositionYzero() - B->GetPositionYzero()));
	return length;
}

float TeicJustTestScene::EnemyPlayerDistance(TeicEnemy *ene)
{
	float length;
	length = D3DXVec3Length(&(ene->GetPositionYzero() - m_pCharacter->GetPositionYZero()));
	return length;
}



void TeicJustTestScene::Render()
{




	/*if (m_pSkyDome)m_pSkyDome->Render();
	if (m_pSkyCloud)m_pSkyCloud->Render();
	if (m_pInventory) m_pInventory->Render();*/
	char str[2056];
	RECT rc = RectMake(200, 200, 1000, 1000);
	sprintf_s(str, "number:%d", m_pBoss->GetAninum() - 1);
	m_pFont->DrawTextA(NULL, str, strlen(str), &rc, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 0, 0));


	

	//m_pTempEnemy->UpdateAndRender();
	//m_pShoot->Render();
	//GETDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	/*m_pTempSPhere->Setup(D3DXVECTOR3( 0,10,0), 0.1f);
	m_pTempSPhere->Render();*/
	/*m_pTempSPhere->Setup(m_pTempEnemy->GetBoundingSquare()->m_vCenterPos, m_pTempEnemy->GetBoundingSquare()->m_fSizeX/2);
	m_pTempSPhere->Render();
	m_pTempSPhere->Setup(m_pTempEnemy->GetBoundingSquare()->m_vCenterPos, m_pTempEnemy->GetBoundingSquare()->m_fSizeY/2);
	m_pTempSPhere->Render();
	m_pTempSPhere->Setup(m_pTempEnemy->GetBoundingSquare()->m_vCenterPos, m_pTempEnemy->GetBoundingSquare()->m_fSizeZ/2);
	m_pTempSPhere->Render();*/



	m_pGrid->Render();
	//if (m_pMap) m_pMap->Render(m_pCharacter->GetPositionYZero());
	if (m_pMap)m_pMap->frustumcullingRender();
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	if (m_pCharacter) m_pCharacter->UpdateAndRender();


	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (!m_vecEnemy[i]->GetSkinnedMesh()->m_bHit)
				m_vecEnemy[i]->UpdateAndRender();
		}
		/*char str[2056];
		RECT rc = RectMake(300, 300, 1000, 1000);
		sprintf_s(str, "1 %.2f	%.2f	%.2f	2 %.2f	%.2f	%.2f",
			m_vecEnemy[0]->GetPosition().x, m_vecEnemy[0]->GetPosition().y, m_vecEnemy[0]->GetPosition().z,
			m_vecEnemy[1]->GetPosition().x, m_vecEnemy[1]->GetPosition().y, m_vecEnemy[1]->GetPosition().z);
		m_pFont->DrawTextA(NULL, str, strlen(str), &rc, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 0, 0))*/
	}
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pBoss->UpdateAndRender();
	UIOBJECTMANAGER->Render();
	
}


bool TeicJustTestScene::TotalCollisionCheck()
{
	bool check;
	check = false;
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		m_vecEnemy[i]->SetCollision(false);
	}

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		for (int j = i + 1; j < m_vecEnemy.size(); j++)
		{
			if (CollisionCheck(m_vecEnemy[i], m_vecEnemy[j]))
			{
				check = true;
			}
		}
	}
	return check;
}



bool TeicJustTestScene::CheckSlot()
{
	bool temp;
	int count = -1;
	for (int i = 0; i < m_vecAttackSlot.size(); i++)
	{
		m_vecAttackSlot[i] = false;
	}
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		m_vecEnemy[i]->SetSlot(false);
		if (m_bAttackOn)
			m_vecEnemy[i]->m_bThreadCalOn = true;
	}
	m_pAttackNode.x = m_pCharacter->GetNodeNum().x + 1;
	m_pAttackNode.y = m_pCharacter->GetNodeNum().y + 1;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			count++;
			if (i == 0 && j == 0)
			{
				count--;
				continue;
			}
			if (m_pCharacter->GetNodeNum().y + i < 0)continue;
			if (m_pCharacter->GetNodeNum().y + i >= m_pNode->m_vRow.size())continue;
			if (m_pCharacter->GetNodeNum().x + j < 0)continue;
			if (m_pCharacter->GetNodeNum().x + j >= m_pNode->m_vRow.size())continue;
			if (m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo != NULL)
			{
				if (m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					m_vecAttackSlot[count] = true;
					for (int a = 0; a < m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding.size(); a++)
					{
						m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding[a]->m_pSkinnedObject->m_bSlotOn = true;
					}
				}
				else
				{
				}
			}
		}
	}


	temp = false;
	return temp;
}

bool TeicJustTestScene::ChangeCheckPoint()
{
	POINT temp = m_pCharacter->GetNodeNum();
	if (m_pCharacter->m_pPreviousPoint.x != temp.x ||
		m_pCharacter->m_pPreviousPoint.y != temp.y)
	{
		m_pCharacter->m_pPreviousPoint = temp;
		return true;
	}
	return false;
}

void TeicJustTestScene::AngleChange(TeicEnemy * A)
{
	/*D3DXVec3Normalize(&A_B, &A_B);
	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&A_B,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);
	D3DXMATRIX	skinnedRot;
	D3DXMatrixRotationY(&skinnedRot, D3DX_PI / 2);
	matR = matR* skinnedRot;*/


	D3DXVECTOR3 A_B = m_pCharacter->GetPositionYZero() - A->GetPositionYzero();
	D3DXVec3Normalize(&A_B, &A_B);

	D3DXVECTOR3 base = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &base, &A_B);
	float targetangle = acosf(D3DXVec3Dot(&base, &A_B));
	if (Cross.y < 0) targetangle = D3DX_PI * 2 - targetangle;
	float Angle = A->GetRoationAngle();
	if (targetangle > Angle)
	{
		Angle += 0.05;
		if (Angle > targetangle) Angle = targetangle;
	}
	else
	{
		Angle -= 0.05;
		if (Angle < targetangle) Angle = targetangle;
	}
	A->SetRotationAngle(Angle);



}

void TeicJustTestScene::WayUpdate()
{
	for (int i = 0; i < m_vecEnemyWay.size(); i++)
	{
		if (m_vecEnemyWay[i].size() != 0)
		{
			m_vecEnemyCollisionMove[i]->SetClear();
			if (m_vecEnemyWay[i].size() <= 1)continue;
			TeicCollisionMove* tempmove;
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
			tempmove->SetSpeed(m_vecEnemy[i]->m_fSpeed);
			tempmove->SetFrom(m_vecEnemy[i]->GetPosition());
			tempmove->SetTo(m_vecEnemyWay[i][1]);
			tempmove->SetRoationAngle(m_vecEnemy[i]->GetRoationAngle());
			m_vecEnemyCollisionMove[i]->AddAction(tempmove);
			for (int j = 1; j < m_vecEnemyWay[i].size(); j++)
			{
				if (j + 1 >= m_vecEnemyWay[i].size())continue;
				tempmove = new TeicCollisionMove;
				tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
				tempmove->SetSpeed(m_vecEnemy[i]->m_fSpeed);
				tempmove->SetFrom(m_vecEnemyWay[i][j]);
				tempmove->SetTo(m_vecEnemyWay[i][j + 1]);
				m_vecEnemyCollisionMove[i]->AddAction(tempmove);
			}
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
			tempmove->SetSpeed(m_vecEnemy[i]->m_fSpeed);
			tempmove->SetFrom(m_vecEnemyWay[i][m_vecEnemyWay[i].size() - 1]);
			tempmove->SetTo(m_pCharacter->GetPositionYZero());
			m_vecEnemyCollisionMove[i]->AddAction(tempmove);

			m_vecEnemyCollisionMove[i]->Start();

			m_vecEnemyWay[i].clear();
		}
	}
	m_EnemyTarget = m_pCharacter->GetPositionYZero();
}

void TeicJustTestScene::SlowUpdate()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetSlow())
		{
			m_vecEnemyCollisionMove[i]->SetSpeedAll(2.0f);
			if (TIMEMANAGER->getWorldTime() > m_vecEnemy[i]->GetSlowTime() + 1)
			{
				m_vecEnemy[i]->SetSlow(false);
				m_vecEnemyCollisionMove[i]->SetSpeedAll(m_vecEnemy[i]->m_fSpeed);
			}
		}
	}
	
}

void TeicJustTestScene::CleanHit()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{

		m_vecEnemy[i]->GetSkinnedMesh()->m_bHit = false;
	}
}

bool TeicJustTestScene::SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	if (abs(A.x - B.x) < 0.0001 &&
		abs(A.y - B.y) < 0.0001 &&
		abs(A.z - B.z) < 0.0001)
	{
		return true;
	}
	return false;
}

void TeicJustTestScene::CheckDie()
{
	int a = 0;
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetHP() <= 0)
		{
			if (m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo != NULL)
			{
				for (int j = 0; j < m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.size(); j++)
				{
					if (m_vecEnemy[i]->GetSkinnedMesh() == m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
					{
						m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.erase(
							m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.begin() + j);

					}
				}
			}


			if (m_pNode->m_vRow[m_vecEnemy[i]->GetNodeNum().y].m_vCol[m_vecEnemy[i]->GetNodeNum().x].m_pBoundInfo != NULL)
			{
				for (int j = 0; j < m_pNode->m_vRow[m_vecEnemy[i]->GetNodeNum().y].m_vCol[m_vecEnemy[i]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.size(); j++)
				{
					if (m_vecEnemy[i]->GetSkinnedMesh() == m_pNode->m_vRow[m_vecEnemy[i]->GetNodeNum().y].m_vCol[m_vecEnemy[i]->GetNodeNum().x].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
					{
						m_pNode->m_vRow[m_vecEnemy[i]->GetNodeNum().y].m_vCol[m_vecEnemy[i]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.erase(
							m_pNode->m_vRow[m_vecEnemy[i]->GetNodeNum().y].m_vCol[m_vecEnemy[i]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.begin() + j);

					}
				}
			}

			m_vecEnemy.erase(m_vecEnemy.begin() + i);
			m_vecEnemyWay.erase(m_vecEnemyWay.begin() + i);
			m_vecEnemyCollisionMove.erase(m_vecEnemyCollisionMove.begin() + i);
			a++;
		}
	}
	if (a != 0)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->SetCallbackfunction(bind(&TeicJustTestScene::CallbackOn, this, 10 + i));
		}
	}
}

float TeicJustTestScene::GetFireRate()
{
	switch (m_pCharacter->getWeaponType())
	{
	case Wp_Melee:
		break;
	case Wp_AA12:
		return DATABASE->GetItemFireRate("AA12");
		break;
	case Wp_AR6:
		return DATABASE->GetItemFireRate("AR6");
		break;
	case Wp_M4:
		return DATABASE->GetItemFireRate("M4");
		break;
	case Wp_MP5:
		return DATABASE->GetItemFireRate("MP5");
		break;
	case WP_FireGun:
		return DATABASE->GetItemFireRate("FireGun");
		break;
	default:
		break;
	}

	
	
}

float TeicJustTestScene::GetCallbackTime()
{
	switch (m_pCharacter->getWeaponType())
	{
	case Wp_Melee:
		break;
	case Wp_AA12:
		return DATABASE->GetCallbacktime("AA12");
		break;
	case Wp_AR6:
		return DATABASE->GetCallbacktime("AR6");
		break;
	case Wp_M4:
		return DATABASE->GetCallbacktime("M4");
		break;
	case Wp_MP5:
		return DATABASE->GetCallbacktime("MP5");
		break;
	case WP_FireGun:
		return DATABASE->GetCallbacktime("FireGun");
		break;
	default:
		break;
	}
}





void TeicJustTestScene::Push2(TeicEnemy * A, TeicEnemy * B)
{

	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
		if (m_pCollision->CheckCollision(A->GetBoundingSquare(), B->GetBoundingSquare()) == false)
			return;
		if (A->GetSlot() && B->GetSlot())
		{
			float Adist = D3DXVec3Length(&(A->GetPositionYzero() - m_EnemyTarget));
			float Bdist = D3DXVec3Length(&(B->GetPositionYzero() - m_EnemyTarget));

			/*D3DXMATRIX Rotation;
			float ran = RND->getFromFloatTo(-D3DX_PI / 2, D3DX_PI / 2);
			D3DXMatrixRotationY(&Rotation, ran);*/

			D3DXVECTOR3 A_B = D3DXVECTOR3(B->GetPositionYzero() - A->GetPositionYzero());
			if (D3DXVec3Length(&A_B) < 0.01)A_B = B->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
			D3DXVec3Normalize(&A_B, &A_B);
			A_B = A_B * 0.1;
			//		D3DXVec3TransformCoord(&A_B, &A_B, &Rotation);

			D3DXVECTOR3 B_A = D3DXVECTOR3(A->GetPositionYzero() - B->GetPositionYzero());
			if (D3DXVec3Length(&B_A) < 0.01)B_A = A->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
			D3DXVec3Normalize(&B_A, &B_A);
			B_A = B_A * 0.1;
			//		D3DXVec3TransformCoord(&B_A, &B_A, &Rotation);

			if (Adist < Bdist)
			{


				B->SetPosition(B->GetPositionYzero() + A_B);


			}
			else
			{


				A->SetPosition(A->GetPositionYzero() + B_A);


			}




		}
	}
	if (A->GetNodeNum().x == m_pCharacter->GetNodeNum().x &&
		A->GetNodeNum().y == m_pCharacter->GetNodeNum().y)
	{
		D3DXVECTOR3 temp = A->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
		D3DXVec3Normalize(&temp, &temp);
		A->SetPosition(A->GetPositionYzero() + temp*0.1);
	}
	if (B->GetNodeNum().x == m_pCharacter->GetNodeNum().x &&
		B->GetNodeNum().y == m_pCharacter->GetNodeNum().y)
	{
		D3DXVECTOR3 temp = B->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
		D3DXVec3Normalize(&temp, &temp);
		B->SetPosition(B->GetPositionYzero() + temp*0.1);
	}


}
