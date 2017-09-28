#include "stdafx.h"
#include "LDYcJustTestScene.h"
#include "cSkyDome.h"
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "LDYCharacter.h"
#include "cSkyBoxCube.h"
#include "cSkyCloud.h"
#include "cShadowMapping.h"
#include "LDYMotionBlur.h"


static CRITICAL_SECTION cs;


static DWORD WINAPI ThFunc1(LPVOID lpParam)
{
	EnterCriticalSection(&cs);
	LDYcJustTestScene* temp = (LDYcJustTestScene*)lpParam;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			TeicEnemy* pSkinnedMesh = new TeicEnemy;
			pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(3 * i + 200, 0, -(100 + 3 * j)));
			pSkinnedMesh->SetCallbackfunction(bind(&LDYcJustTestScene::CallbackOn, temp, (i + 1) * 10 + j));
			temp->m_vecEnemy.push_back(pSkinnedMesh);
		}
	}

	temp->m_vecEnemyWay.resize(temp->m_vecEnemy.size());
	temp->m_vecEnemyCollisionMove.resize(temp->m_vecEnemy.size());

	for (int i = 0; i < temp->m_vecEnemyWay.size(); i++)
	{

		temp->m_vecEnemyWay[i] = temp->m_pAstar->FindWay(temp->m_vecEnemy[i]->GetNodeNum().x, temp->m_vecEnemy[i]->GetNodeNum().y,
			temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);
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
	LDYcJustTestScene* temp = (LDYcJustTestScene*)lpParam;
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


LDYcJustTestScene::LDYcJustTestScene()
	: m_pMap(NULL)
	, m_pNode(NULL)
	, m_bThread(false)
	, m_pAstar(NULL)
	, m_pAstarShort(NULL)
	, m_EnemyTarget(D3DXVECTOR3(0, 0, 0))
	, m_bAstarThread(false)
	, m_pCamera(NULL)
	,m_iBodyUpgrade(1)
	, m_pSkyDome(NULL)
	, m_pSkyCloud(NULL)
	, m_pSkyBox(NULL)
	, m_pShadow(NULL)
	, m_pCreateShadow(NULL)
	, m_pApplyShadow(NULL)
	, m_pHeightMapmesh(NULL)
	,lookz(0.0f)

{
	m_vecAttackSlot.resize(8, false);
}


LDYcJustTestScene::~LDYcJustTestScene()
{
	SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pAstarShort);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	//SAFE_DELETE(m_pCharacter);
	SAFE_DELETE(m_pSkyBox);
	SAFE_DELETE(m_pSkyDome);
	SAFE_DELETE(m_pSkyCloud);
	SAFE_DELETE(m_pShadow);

	SAFE_RELEASE(m_pCreateShadow);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pHeightMapmesh);

	//SAFE_DELETE(motionBlur);

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		SAFE_DELETE(m_vecEnemy[i]);
		SAFE_DELETE(m_vecEnemyCollisionMove[i]);

	}

}



HRESULT LDYcJustTestScene::Setup()
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

	cUIImageView* pInventoryImage = new cUIImageView;
	pInventoryImage->SetTexture("./UI/inventory.png");
	UIOBJECTMANAGER->AddRoot("inventory", pInventoryImage, false);



	///////////동윤
	/*motionBlur = new LDYMotionBlur;
	motionBlur->Setup();*/

	m_pShadow = new cShadowMapping;
	m_pShadow->Setup();

	m_pSkyBox = new cSkyBoxCube;
	m_pSkyBox->Setup();

	m_pSkyDome = new cSkyDome;
	m_pSkyDome->Setup();

	m_pSkyCloud = new cSkyCloud;
	m_pSkyCloud->Setup();


	m_pCamera = new LDYCamera;
	m_pGrid = new Hank::cGrid;

	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, m_iBodyUpgrade);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(D3DXVECTOR3(20, 20, 20));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&LDYcJustTestScene::CallbackOn, this, 0));


	/////////////태영
	m_pGrid->Setup();
	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");

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


	const int shadowMapSize = 2048;
	if (FAILED(GETDEVICE->CreateTexture(shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL)))
	{

	}



	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if (FAILED(GETDEVICE->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
	}

	m_pCreateShadow = LoadEffectHpp("MultiAnimationCreateShadow.hpp");
	m_pApplyShadow = LoadEffect("shader/shadow/ApplyShadow.fx");


	
	m_hCmatLightView = m_pCreateShadow->GetParameterByName(0, "matLightView");
	m_hCmatLightProjection = m_pCreateShadow->GetParameterByName(0, "gLightProjectionMatrix");


	m_hApplyTexture = m_pApplyShadow->GetParameterByName(0, "ShadowMap_Tex");
	m_hAmatWorld = m_pApplyShadow->GetParameterByName(0, "matWorld");
	m_hAmatLightView = m_pApplyShadow->GetParameterByName(0, "matLightView");
	m_hAmatLightProjection = m_pApplyShadow->GetParameterByName(0, "matLightProjection");
	m_hAm_vec4LightPosition = m_pApplyShadow->GetParameterByName(0, "m_vec4LightPosition");
	m_hAmatViewProjection = m_pApplyShadow->GetParameterByName(0, "matViewProjection");
	m_hAgObjectColor = m_pApplyShadow->GetParameterByName(0, "gObjectColor");




	D3DXMatrixIdentity(&matHeightWorld);


	return S_OK;
}

void LDYcJustTestScene::Release()
{
	m_pGrid->Release();

	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCharacter);


}

void LDYcJustTestScene::Update()
{
	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		UIOBJECTMANAGER->SetShowState("inventory", !UIOBJECTMANAGER->CheckShowState("inventory"));
	}

	//if (motionBlur)motionBlur->Update();
	if (m_pSkyBox) m_pSkyBox->Update();
	if (m_pSkyDome)m_pSkyDome->Update();
	if (m_pSkyCloud)m_pSkyCloud->Update();
	//if (m_pShadow)m_pShadow->Update();

	m_pCamera->Update(m_pCharacter->GetPosition());
	m_pCharacter->Update(m_pCamera->getAngleY());
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

		}
	}

	/*if (TIMEMANAGER->getWorldTime() > m_fTime + 5.0f)
	{
		m_fTime = INF;
		DWORD dwThID1;
		HANDLE hThreads;

		unsigned long ulStackSize = 0;
		dwThID1 = 0;
		hThreads = NULL;
		hThreads = CreateThread(NULL, ulStackSize, ThFunc1, this, CREATE_SUSPENDED, &dwThID1);
		ResumeThread(hThreads);
	}*/



	if (m_bThread)
	{
		CheckSlot();
		WayUpdate();
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (D3DXVec3Length(&(m_vecEnemy[i]->GetPositionYzero() - m_pCharacter->GetPositionYZero())) < m_vecEnemy[i]->m_fAttackRange)
			{
				if (!m_vecEnemy[i]->GetSlot())continue;
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
			D3DXMATRIX   skinnedRot;
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


	UIOBJECTMANAGER->Update();
}

void LDYcJustTestScene::CallbackOn(int number)
{
	//////0 캐릭터   10~29 적   

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

bool LDYcJustTestScene::CollisionCheck(TeicEnemy * A, TeicEnemy * B)
{
	
	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
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



void LDYcJustTestScene::TotalPushCheck2()
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

void LDYcJustTestScene::ChangeGridInfo()
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

void LDYcJustTestScene::TargetOn()
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


		}
	}

}

bool LDYcJustTestScene::CheckStraight(TeicEnemy *A)
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

float LDYcJustTestScene::EnemyEnemyDistance(TeicEnemy * A, TeicEnemy * B)
{
	float length;
	length = D3DXVec3Length(&(A->GetPositionYzero() - B->GetPositionYzero()));
	return length;
}

float LDYcJustTestScene::EnemyPlayerDistance(TeicEnemy *ene)
{
	float length;
	length = D3DXVec3Length(&(ene->GetPositionYzero() - m_pCharacter->GetPositionYZero()));
	return length;
}



void LDYcJustTestScene::Render()
{

	//if (motionBlur)motionBlur->Render();


	D3DXVECTOR3 light = m_pCharacter->GetPositionYZero();


	{
		m_vec4LightPosition = { light.x + 100.0f,light.y + 200.0f,light.z + 100.0f,1.0f };
		D3DXVECTOR3 vEyePt(m_vec4LightPosition.x, m_vec4LightPosition.y, m_vec4LightPosition.z);
		D3DXVECTOR3 vLookatPt = m_pCharacter->GetPositionYZero();
		vLookatPt.x += lookx;
		//vLookatPt.y += 5.0f;
		vLookatPt.z -= lookz;
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}
	

	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
	

	D3DXMATRIXA16 matWorld, matView, matProjection, matViewProjection;

	D3DXMatrixIdentity(&matWorld);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);

	matViewProjection = matView*matProjection;

	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	GETDEVICE->GetRenderTarget(0, &pHWBackBuffer);
	GETDEVICE->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface);


	GETDEVICE->SetRenderTarget(0, pShadowSurface);
	GETDEVICE->SetDepthStencilSurface(m_pShadowDepthStencil);

	SAFE_RELEASE(pShadowSurface);

	// 저번 프레임에 그\렸던 그림자 정보를 지움
	GETDEVICE->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);
	D3DXMATRIX scal;
	D3DXMatrixScaling(&scal, 0.04, 0.04, 0.04);
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_pCharacter->GetPosition().x, m_pCharacter->GetPosition().y, m_pCharacter->GetPosition().z);

	// 그림자 만들기 쉐이더 전역변수들을 설정

	m_pCreateShadow->SetMatrix(m_hCmatLightView, &matLightView);
	m_pCreateShadow->SetMatrix(m_hCmatLightProjection, &matLightProjection);

	// 그림자 만들기 쉐이더를 시작
	
	{
		UINT numPasses = 0;
		m_pCreateShadow->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pCreateShadow->BeginPass(i);
				{
					m_pCharacter->MeshRender(m_pCreateShadow);
				}
				m_pCreateShadow->EndPass();
			}
		}
		m_pCreateShadow->End();
	}

	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////

	////// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	GETDEVICE->SetRenderTarget(0, pHWBackBuffer);
	GETDEVICE->SetDepthStencilSurface(pHWDepthStencilBuffer);

	SAFE_RELEASE(pHWBackBuffer);
	SAFE_RELEASE(pHWDepthStencilBuffer);

	// 그림자 입히기 쉐이더 전역변수들을 설정

	m_pApplyShadow->SetMatrix(m_hAmatWorld, &matWorld);      //원환체
	m_pApplyShadow->SetMatrix(m_hAmatViewProjection, &matViewProjection);
	m_pApplyShadow->SetMatrix(m_hAmatLightView, &matLightView);
	m_pApplyShadow->SetMatrix(m_hAmatLightProjection, &matLightProjection);

	m_pApplyShadow->SetVector(m_hAm_vec4LightPosition, &m_vec4LightPosition);

	m_pApplyShadow->SetTexture(m_hApplyTexture, m_pShadowRenderTarget);

	LPDIRECT3DTEXTURE9 tex;
	tex = TEXTUREMANAGER->GetTexture("map/Terrain_Final_Map.png");
	m_pApplyShadow->SetTexture("heightMap_Tex", tex);

	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pApplyShadow->BeginPass(i);
			{
				// 원환체를 그린다.
				

				// 디스크를 그린다.
				
				if (m_pMap)m_pMap->MeshRender(m_pCharacter->GetPositionYZero());

			}
			m_pApplyShadow->EndPass();
		}
	}
	m_pApplyShadow->End();
	m_pCharacter->UpdateAndRender();
	/*if (m_pSkyDome)m_pSkyDome->Render();
	if (m_pSkyCloud)m_pSkyCloud->Render();
	if(m_pGrid)m_pGrid->Render();*/
	/*D3DCOLOR m_d3dFogColor = D3DCOLOR_XRGB(10, 100, 100);
	float start = 20.0f;
	float end = 100.0f;
	float m_fFogDensity = 0.05f;
	GETDEVICE->SetRenderState(D3DRS_FOGENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	GETDEVICE->SetRenderState(D3DRS_FOGCOLOR, m_d3dFogColor);
	GETDEVICE->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	GETDEVICE->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	GETDEVICE->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	GETDEVICE->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fFogDensity));*/
	//GETDEVICE->SetRenderState(D3DRS_RANGEFOGENABLE, true);
	/*if (m_pMap) m_pMap->Render(m_pCharacter->GetPositionYZero());
	if (m_pCharacter) m_pCharacter->UpdateAndRender();*/

	//if (m_pSkyBox)m_pSkyBox->Render(m_pCamera);

	if (m_bThread)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			m_vecEnemy[i]->UpdateAndRender();
		}
	}

	UIOBJECTMANAGER->Render();
}


bool LDYcJustTestScene::TotalCollisionCheck()
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



bool LDYcJustTestScene::CheckSlot()
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

bool LDYcJustTestScene::ChangeCheckPoint()
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

void LDYcJustTestScene::AngleChange(TeicEnemy * A)
{
	D3DXVECTOR3 A_B = A->GetPositionYzero() - m_pCharacter->GetPositionYZero();
	D3DXVec3Normalize(&A_B, &A_B);
	D3DXVECTOR3 base = D3DXVECTOR3(0, 0, 1);
	float targetangle = D3DXVec3Dot(&base, &A_B);
	/*D3DXVec3Normalize(&A_B, &A_B);
	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,

	D3DXMatrixTranspose(&matR, &matR);
	D3DXMATRIX   skinnedRot;
	D3DXMatrixRotationY(&skinnedRot, D3DX_PI / 2);
	matR = matR* skinnedRot;*/

	A->m_fAngle += 0.1;
	if (A->m_fAngle > targetangle) A->m_fAngle = targetangle;
	A->SetRotationAngle(A->m_fAngle);
}





void LDYcJustTestScene::Push2(TeicEnemy * A, TeicEnemy * B)
{


	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
		if (A->GetSlot() && B->GetSlot())
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


			if (EnemyPlayerDistance(A) > 3.0)
			{

				A->SetPosition(A->GetPositionYzero() + B_A);

			}

			if (EnemyPlayerDistance(B) > 3.0)
			{

				B->SetPosition(B->GetPositionYzero() + A_B);
			}

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

void LDYcJustTestScene::WayUpdate()
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
			tempmove = new TeicCollisionMove;
			tempmove->SetSkinnedTarget(m_vecEnemy[i]->GetSkinnedMesh());
			tempmove->SetSpeed(5);
			tempmove->SetFrom(m_vecEnemyWay[i][m_vecEnemyWay[i].size() - 1]);
			tempmove->SetTo(m_pCharacter->GetPositionYZero());
			m_vecEnemyCollisionMove[i]->AddAction(tempmove);

			m_vecEnemyCollisionMove[i]->Start();

			m_vecEnemyWay[i].clear();
		}
	}
}


LPD3DXEFFECT LDYcJustTestScene::LoadEffect(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩
	
	LPD3DXBUFFER      pError = NULL;         //에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD            dwShaderFlag = 0;      //셰이더 플레그 0 

#ifdef _DEBUG
	
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;      //셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif
														 //fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(
	
		GETDEVICE,            // 디바이스
		szFileName,               // 불러올 셰이더 코드 파일이름
		NULL,                  // 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,                  // 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,            // 셰이더 컴파일 플레그
		NULL,                  // 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,               // 로딩될 셰이더 Effect 포인터
		&pError                  // 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
	);

	//셰이더 파일로딩에문재가 있다면..
	if (pError != NULL || pEffect == NULL) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}

LPD3DXEFFECT LDYcJustTestScene::LoadEffectHpp(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	GETDEVICE->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(GETDEVICE,
		szFileName,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}
