#include "stdafx.h"
#include "TeicJustTestScene.h"


#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"

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
			//	pSkinnedMesh->SetPosition(D3DXVECTOR3(0,0,0));
			pSkinnedMesh->SetCallbackfunction(bind(&TeicJustTestScene::CallbackOn, temp, (i + 1) * 10 + j));
			temp->m_vecEnemy.push_back(pSkinnedMesh);
		}
	}

	temp->m_vecEnemyWay.resize(temp->m_vecEnemy.size());
	temp->m_vecEnemyCollisionMove.resize(temp->m_vecEnemy.size());








	/*vector<D3DXVECTOR3> wayway = temp->m_pAstar->FindWay(temp->m_vecEnemy[0]->GetNodeNum().x, temp->m_vecEnemy[0]->GetNodeNum().y,
		temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);*/

	/*vector<D3DXVECTOR3> wayway = temp->m_pAstarShort->FindWay(0,0,10,10);
	temp->m_EnemyTarget = wayway[wayway.size() - 1];*/

	for (int i = 0; i < temp->m_vecEnemyWay.size(); i++)
	{
		temp->m_vecEnemyWay[i] =temp->m_pAstar->FindWay(temp->m_vecEnemy[i]->GetNodeNum().x, temp->m_vecEnemy[i]->GetNodeNum().y,
		temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);
		//temp->m_vecEnemyWay[i] = wayway;
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

		temp->m_vecEnemyCollisionMove[i]->SetClear();


		if (temp->m_vecEnemyWay[i].size() <=1)continue;
		TeicCollisionMove* tempmove;
		tempmove = new TeicCollisionMove;
		tempmove->SetSkinnedTarget(temp->m_vecEnemy[i]->GetSkinnedMesh());
		tempmove->SetSpeed(5);
		tempmove->SetFrom(temp->m_vecEnemy[i]->GetPosition());
		tempmove->SetTo(temp->m_vecEnemyWay[i][1]);
		temp->m_vecEnemyCollisionMove[i]->AddAction(tempmove);
		for (int j = 1; j < temp->m_vecEnemyWay[i].size(); j++)
		{
			if (j + 1 >= temp->m_vecEnemyWay[i].size())continue;
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(temp->m_vecEnemy[i]->GetSkinnedMesh());
			tempmove->SetSpeed(5);
			tempmove->SetFrom(temp->m_vecEnemyWay[i][j]);
			tempmove->SetTo(temp->m_vecEnemyWay[i][j + 1]);
			temp->m_vecEnemyCollisionMove[i]->AddAction(tempmove);
		}
		tempmove = new TeicCollisionMove;
		tempmove->SetSkinnedTarget(temp->m_vecEnemy[i]->GetSkinnedMesh());
		tempmove->SetSpeed(5);
		tempmove->SetFrom(temp->m_vecEnemyWay[i][temp->m_vecEnemyWay[i].size()-1]);
		tempmove->SetTo(temp->m_pCharacter->GetPositionYZero());
		temp->m_vecEnemyCollisionMove[i]->AddAction(tempmove);

		temp->m_vecEnemyCollisionMove[i]->Start();



	}
	temp->m_bAstarThread = false;
	LeaveCriticalSection(&cs);



	return 0;
}


TeicJustTestScene::TeicJustTestScene()
	: m_pMap(NULL)
	, m_pNode(NULL)
	, m_bThread(false)
	, m_pUITest(NULL)
	, m_pSprite(NULL)
	, m_pAstar(NULL)
	, m_pAstarShort(NULL)
	, m_EnemyTarget(D3DXVECTOR3(0, 0, 0))
	, m_bAstarThread(false)


{
	m_vecAttackSlot.resize(8, false);
}


TeicJustTestScene::~TeicJustTestScene()
{
	SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pAstarShort);
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

}



HRESULT TeicJustTestScene::Setup()
{





	m_pCamera = new Hank::cCamera;
	m_pGrid = new Hank::cGrid;

	D3DXCreateSprite(GETDEVICE, &m_pSprite);
	cUIImageView* pImageView = new cUIImageView;
	pImageView->SetTexture("NULL");
	m_pUITest = pImageView;

	cUITextView* pTextView = new cUITextView;
	pTextView->SetText("UI 텍스트 출력 실험용..");
	pTextView->SetSize(ST_SIZE(312, 200));
	pTextView->SetPosition(100, 100);
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTag(0);
	m_pUITest->AddChild(pTextView);

	m_pCharacter = new TeicCharacter;
	m_pCharacter->Setup("object/xFile/tiger/", "tiger.X");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&TeicJustTestScene::CallbackOn, this, 0));

	
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
			m_pNode->m_vRow[i].m_vCol[j].InitPosition(NodeLength); // 생성된 노드를 기반으로 버텍스와 중점 좌상단점을 설정합니다.
			m_pNode->m_vRow[i].m_vCol[j].m_pAstarNode = new HankcAstarNode;
		}
	}

	//	m_pNode->m_vCol[0].m_vRow[0].m_pBoundInfo
	//////////////////여기서 부터 다시



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




	return S_OK;
}

void TeicJustTestScene::Release()
{
	m_pGrid->Release();

	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pUITest);
}

void TeicJustTestScene::Update()
{
	m_pCharacter->Update();
	bool check = ChangeCheckPoint();
	if (m_bThread)
	{
		if (check)
		{
			m_fTime2 = 0;
		}
		if (TIMEMANAGER->getWorldTime() > m_fTime2 + 2.0f)
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





		/*	for (int i = 0; i < m_vecEnemy.size(); i++)
			{
				if (!m_vecEnemy[i]->m_bThreadCalOn)continue;
				if (m_vecEnemy[i]->m_bAttackOn)continue;
				m_vecEnemyWay[i] = m_pAstarShort->FindWay(m_vecEnemy[i]->GetNodeNum().x, m_vecEnemy[i]->GetNodeNum().y,
					m_pAttackNode.x, m_pAttackNode.y, m_pCharacter->GetNodeNum().x, m_pCharacter->GetNodeNum().y);

				m_vecEnemyCollisionMove[i]->SetClear();


				if (m_vecEnemyWay[i].size() <= 1)continue;
				TeicCollisionMove* tempmove;
				tempmove = new TeicCollisionMove;
				tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
				tempmove->SetSpeed(5);
				tempmove->SetFrom(m_vecEnemy[i]->GetPosition());
				tempmove->SetTo(m_vecEnemyWay[i][1]);
				m_vecEnemyCollisionMove[i]->AddAction(tempmove);
				for (int j = 1; j < m_vecEnemyWay[i].size(); j++)
				{
					if (j + 1 >= m_vecEnemyWay[i].size())continue;
					tempmove = new TeicCollisionMove;
					tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
					tempmove->SetSpeed(5);
					tempmove->SetFrom(m_vecEnemyWay[i][j]);
					tempmove->SetTo(m_vecEnemyWay[i][j + 1]);
					m_vecEnemyCollisionMove[i]->AddAction(tempmove);
				}
				m_vecEnemyCollisionMove[i]->Start();



			}*/



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
		CheckSlot();
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (D3DXVec3Length(&(m_vecEnemy[i]->GetPositionYzero() - m_pCharacter->GetPositionYZero())) < m_vecEnemy[i]->m_fAttackRange)
			{
				if (!m_vecEnemy[i]->m_bSlotOn)continue;
				m_vecEnemy[i]->m_bAttackOn = true;
				m_vecEnemy[i]->m_bThreadCalOn = false;
			}
			else if (m_vecEnemy[i]->m_bAttackOn == false)
			{
				m_vecEnemy[i]->m_bAttackOn = false;
			}



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

				if (m_vecEnemy[i]->GetAninum() != 1)
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
			D3DXVECTOR3 A_B = m_vecEnemy[i]->GetPositionYzero() - m_pCharacter->GetPositionYZero();
			D3DXVec3Normalize(&A_B, &A_B);
			D3DXMATRIX matR;
			D3DXMatrixLookAtLH(&matR,
				&D3DXVECTOR3(0, 0, 0),
				&A_B,
				&D3DXVECTOR3(0, 1, 0));
			D3DXMatrixTranspose(&matR, &matR);
			D3DXMATRIX	skinnedRot;
			D3DXMatrixRotationY(&skinnedRot, D3DX_PI / 2);
			matR = matR* skinnedRot;
			//AngleChange(m_vecEnemy[i]);
			
		}
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_pMap->GetHeight(m_vecEnemy[i]->GetPositionPointer()->x, m_vecEnemy[i]->GetPositionPointer()->y, m_vecEnemy[i]->GetPositionPointer()->z);


		}
	}
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);

	ChangeGridInfo();
	if (m_pUITest) m_pUITest->Update();
	m_pCamera->Update();
}

void TeicJustTestScene::CallbackOn(int number)
{
	//////0 캐릭터   1 적   2  적 움직임

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
	for (int i = 10; i < 30; i++)
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
	if (D3DXVec3Length(&(A->GetPositionYzero() - B->GetPositionYzero())) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
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
				if (D3DXVec3Length(&(B->GetPositionYzero() - m_pCharacter->GetPositionYZero())) > 10*NodeLength)
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
				if (D3DXVec3Length(&(A->GetPositionYzero() - m_pCharacter->GetPositionYZero())) > 10 * NodeLength)
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
					if (m_vecEnemy[i] == m_pNode->m_vRow[m_vecEnemy[i]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding[j]->m_pEnemy)
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
					m_vecEnemy[i]->m_BoundingBox);
			}
			else
			{
				m_pNode->m_vRow[m_vecEnemy[i]->m_PresentGrid.y].m_vCol[m_vecEnemy[i]->m_PresentGrid.x].m_pBoundInfo->m_vecBounding.push_back(
					m_vecEnemy[i]->m_BoundingBox);
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
			if (m_vecEnemy[i]->m_bAttackOn)continue;
			if (m_vecEnemy[i]->m_bThreadCalOn)continue;
			


			
			m_vecEnemy[i]->m_bThreadCalOn = true;
			m_fTime3 = INF;
			/*if (CheckStraight(m_vecEnemy[i]))
			{
				m_vecEnemyCollisionMove[i]->SetClear();
				TeicCollisionMove* tempmove;
				tempmove = new TeicCollisionMove;
				tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
				tempmove->SetSpeed(5);
				tempmove->SetFrom(m_vecEnemy[i]->GetPosition());
				tempmove->SetTo(m_pCharacter->GetPosition());
				m_vecEnemyCollisionMove[i]->AddAction(tempmove);
				m_vecEnemyCollisionMove[i]->Start();

			}
			else
			{
				m_vecEnemy[i]->m_bThreadCalOn = true;

			}*/
			
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

float TeicJustTestScene::EnemyPlayerDistance(TeicEnemy *ene)
{
	float length;
	length = D3DXVec3Length(&(ene->GetPosition() - m_pCharacter->GetPosition()));
	return length;
}



void TeicJustTestScene::Render()
{

	/*vector<cSphere> m_vecSphere;
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if (m_pNode->m_vRow[i].m_vCol[j].m_pBoundInfo != NULL)
			{
				if (m_pNode->m_vRow[i].m_vCol[j].m_pBoundInfo->m_vecBounding.size() != 0)
				{
					cSphere tempsphere;
					tempsphere.Setup(m_pNode->m_vRow[i].m_vCol[j].m_vPosList->m_vCenterPos.x,
						-m_pNode->m_vRow[i].m_vCol[j].m_vPosList->m_vCenterPos.z);
					m_vecSphere.push_back(tempsphere);
				}
			}
		}
	}


	for (int i = 0; i < m_vecSphere.size(); i++)
	{
		m_vecSphere[i].Render();
	}
*/

	/*cSphere tempsphere;
	tempsphere.Setup(m_pNode->m_vRow[m_pCharacter->GetNodeNum().y].m_vCol[m_pCharacter->GetNodeNum().x].m_vPosList->m_vCenterPos.x,
		-m_pNode->m_vRow[m_pCharacter->GetNodeNum().y].m_vCol[m_pCharacter->GetNodeNum().x].m_vPosList->m_vCenterPos.z);
	tempsphere.SetRed(true);
	tempsphere.Render();*/
	/*if (m_bThread)
	{
		if (m_vecEnemy[0]->m_bThreadCalOn)
		{
			for (int i = 0; i < m_vecSphere.size(); i++)
			{
				SAFE_DELETE(m_vecSphere[i]);
			}
			for (int i = 0; i < m_vecEnemyWay[0].size(); i++)
			{
				for (int j = 0; j < m_vecEnemyWay[0].size(); j++)
				{
					cSphere* temp = new cSphere;
					temp->Setup(m_vecEnemyWay[0][j].x, -m_vecEnemyWay[0][j].z);
					m_vecSphere.push_back(temp);

				}
			}
			for (int i = 0; i < m_vecSphere.size(); i++)
			{
				m_vecSphere[i]->Render();
			}
		}
	}*/
	m_pGrid->Render();
	if (m_pMap) m_pMap->Render();
	if (m_pCharacter) m_pCharacter->UpdateAndRender();

	/*for each (auto p in m_vecEnemy)
	{
	p->UpdateAndRender();
	}*/
	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->UpdateAndRender();
		}
	}
	if (m_pUITest) m_pUITest->Render(m_pSprite);
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



bool TeicJustTestScene::SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	if (abs(A.x - B.x) < 0.0001 &&
		abs(A.z - B.z) < 0.0001)
	{
		return true;
	}

	return false;
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
		m_vecEnemy[i]->m_bSlotOn = false;
		if(m_bAttackOn)
		m_vecEnemy[i]->m_bThreadCalOn = true;
	}
	m_pAttackNode.x = m_pCharacter->GetNodeNum().x+1;
	m_pAttackNode.y = m_pCharacter->GetNodeNum().y+1;
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
			if (m_pCharacter->GetNodeNum().y + i >=m_pNode->m_vRow.size())continue;
			if (m_pCharacter->GetNodeNum().x + j < 0)continue;
			if (m_pCharacter->GetNodeNum().x + j >=m_pNode->m_vRow.size())continue;
			if (m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo != NULL)
			{
				if (m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding.size() != 0)
				{

					m_vecAttackSlot[count] = true;
					for (int a = 0; a < m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding.size(); a++)
					{
						m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding[a]->m_pEnemy->m_bSlotOn = true;
					}
					
				}
				else
				{
					
					
				}

			}


			
		}
	}
	/*int a = 0;
	for (int i = 0; i < m_vecAttackSlot.size(); i++)
	{
		if (m_vecAttackSlot[i] == false)
		{
			a = -1;
			if (i == 0)
			{
				if(m_pCharacter->GetNodeNum().x - 1>=0)
				m_pAttackNode.x = m_pCharacter->GetNodeNum().x - 1;
				if (m_pCharacter->GetNodeNum().y - 1 >= 0)
				m_pAttackNode.y = m_pCharacter->GetNodeNum().y - 1;
			}
			else if (i == 1)
			{
				
				m_pAttackNode.x = m_pCharacter->GetNodeNum().x ;
				if (m_pCharacter->GetNodeNum().y - 1 >= 0)
					m_pAttackNode.y = m_pCharacter->GetNodeNum().y - 1;
			}
			else if (i == 2)
			{
				if (m_pCharacter->GetNodeNum().x + 1 < m_pNode->m_vRow.size())
				m_pAttackNode.x = m_pCharacter->GetNodeNum().x+1;
				if (m_pCharacter->GetNodeNum().y - 1 >= 0)
					m_pAttackNode.y = m_pCharacter->GetNodeNum().y - 1;
			}
			else if (i == 3)
			{
				if (m_pCharacter->GetNodeNum().x - 1 >= 0)
					m_pAttackNode.x = m_pCharacter->GetNodeNum().x - 1;
				m_pAttackNode.y = m_pCharacter->GetNodeNum().y ;
			}
			
			else if (i == 4)
			{
				if (m_pCharacter->GetNodeNum().x + 1 < m_pNode->m_vRow.size())
					m_pAttackNode.x = m_pCharacter->GetNodeNum().x + 1;
				m_pAttackNode.y = m_pCharacter->GetNodeNum().y;
			}
			if (i == 5)
			{
				if (m_pCharacter->GetNodeNum().x - 1 >= 0)
					m_pAttackNode.x = m_pCharacter->GetNodeNum().x - 1;
				if (m_pCharacter->GetNodeNum().y + 1 < m_pNode->m_vRow.size())
					m_pAttackNode.y = m_pCharacter->GetNodeNum().y + 1;
			}
			if (i == 6)
			{
				m_pAttackNode.x = m_pCharacter->GetNodeNum().x;
				if (m_pCharacter->GetNodeNum().y + 1 < m_pNode->m_vRow.size())
					m_pAttackNode.y = m_pCharacter->GetNodeNum().y + 1;
			}
			if (i == 6)
			{
				if (m_pCharacter->GetNodeNum().x + 1 < m_pNode->m_vRow.size())
					m_pAttackNode.x = m_pCharacter->GetNodeNum().x + 1;
				if (m_pCharacter->GetNodeNum().y + 1 < m_pNode->m_vRow.size())
					m_pAttackNode.y = m_pCharacter->GetNodeNum().y + 1;
			}
			temp = true;
		}
	}
	if (a == 0)
	{
		m_pAttackNode.x = m_pCharacter->GetNodeNum().x;
		m_pAttackNode.y = m_pCharacter->GetNodeNum().y;
	}*/
	
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
	D3DXVECTOR3 A_B = A->GetPositionYzero() - m_pCharacter->GetPositionYZero();
	D3DXVec3Normalize(&A_B, &A_B);
	D3DXVECTOR3 base = D3DXVECTOR3(0, 0, 1);
	float targetangle = D3DXVec3Dot(&base, &A_B);
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

	A->m_fAngle += 0.1;
	if (A->m_fAngle > targetangle) A->m_fAngle = targetangle;
	A->SetRotationAngle(A->m_fAngle);
}





void TeicJustTestScene::Push2(TeicEnemy * A, TeicEnemy * B)
{
	
	if (D3DXVec3Length(&(A->GetPositionYzero() - B->GetPositionYzero())) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
		if (A->m_bSlotOn && B->m_bSlotOn)
		{
			float Adist = D3DXVec3Length(&(A->GetPositionYzero() - m_EnemyTarget));
			float Bdist = D3DXVec3Length(&(B->GetPositionYzero() - m_EnemyTarget));

			D3DXMATRIX Rotation;
			float ran = RND->getFromFloatTo(-D3DX_PI / 2, D3DX_PI / 2);
			D3DXMatrixRotationY(&Rotation, ran);





			D3DXVECTOR3 A_B = D3DXVECTOR3(B->GetPositionYzero() - A->GetPositionYzero());
			if (D3DXVec3Length(&A_B) < 0.01)A_B = B->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
			D3DXVec3Normalize(&A_B, &A_B);
			A_B = A_B * 0.3;
			D3DXVec3TransformCoord(&A_B, &A_B, &Rotation);

			D3DXVECTOR3 B_A = D3DXVECTOR3(A->GetPositionYzero() - B->GetPositionYzero());
			if (D3DXVec3Length(&B_A) < 0.01)B_A = A->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
			D3DXVec3Normalize(&B_A, &B_A);
			B_A = B_A * 0.3;
			D3DXVec3TransformCoord(&B_A, &B_A, &Rotation);


			if (D3DXVec3Length(&(A->GetPositionYzero() - m_pCharacter->GetPositionYZero())) > 3.0)
			{

				A->SetPosition(A->GetPositionYzero() + B_A);

			}

			if (D3DXVec3Length(&(B->GetPositionYzero() - m_pCharacter->GetPositionYZero())) > 3.0)
			{

				B->SetPosition(B->GetPositionYzero() + A_B);
			}


			//if (Adist < Bdist)
			//{
			//	//if (B->m_bSlotOn)
			//		B->SetPosition(B->GetPositionYzero() + A_B);
			//}
			//else
			//{
			//	//if (A->m_bSlotOn)
			//		A->SetPosition(A->GetPositionYzero() + B_A);

			//}
		}
	}
	if (A->GetNodeNum().x == m_pCharacter->GetNodeNum().x &&
		A->GetNodeNum().y == m_pCharacter->GetNodeNum().y)
	{
		D3DXVECTOR3 temp = A->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
		D3DXVec3Normalize(&temp, &temp);
		A->SetPosition(A->GetPositionYzero() + temp*0.3);
	}
	if (B->GetNodeNum().x == m_pCharacter->GetNodeNum().x &&
		B->GetNodeNum().y == m_pCharacter->GetNodeNum().y)
	{
		D3DXVECTOR3 temp = B->GetPositionYzero(); -m_pCharacter->GetPositionYZero();
		D3DXVec3Normalize(&temp, &temp);
		B->SetPosition(B->GetPositionYzero() + temp*0.3);
	}


}
