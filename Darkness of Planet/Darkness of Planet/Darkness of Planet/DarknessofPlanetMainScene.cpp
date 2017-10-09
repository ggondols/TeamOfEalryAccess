#include "stdafx.h"
#include "DarknessofPlanetMainScene.h"
//진짜 최고
//진짜 최고
//진짜 최고
#include "iMap.h"
#include "cHeightMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cSkyDome.h"
#include "cSkyCloud.h"
#include "Inventory.h"
#include "FieldItem.h"

DarknessofPlanetMainScene::DarknessofPlanetMainScene()
	: m_pMap(NULL)
	, m_pNode(NULL)
	, m_bThread(false)
	, m_pAstar(NULL)
	, m_pAstarShort(NULL)
	, m_EnemyTarget(D3DXVECTOR3(0, 0, 0))
	, m_bAstarThread(false)
	, m_bAstarThread2(false)
	/*, m_pCamera(NULL)*/
	, m_iBodyUpgrade(1)
	, m_fBossTime(0)
	, m_pFont(NULL)
	, m_pCollision(NULL)
	, m_pShoot(NULL)
	, m_pSkyDome(NULL)
	, m_pSkyCloud(NULL)
	, m_pInventory(NULL)
	, m_fTime5(0.0f)
	, m_pCreateShadow(NULL)
	, m_pApplyShadow(NULL)
	, m_pHeightMapmesh(NULL)
	, m_fStartTime(0.0f)
	, m_fEndTime(0.0f)
	, m_fCurrentTime(0.0f)
	, m_pBloomEffect(NULL)
	, m_pBloomRenderTarget(NULL)
	, m_pBloomDepthStencil(NULL)
	, m_fTime7(0)
	, m_iCameranum(0)
	, tex(NULL)
	, m_iSound(0)
{
	m_vecAttackSlot.resize(8, false);
	m_ObjNodes.clear();
}


DarknessofPlanetMainScene::~DarknessofPlanetMainScene()
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

	SAFE_RELEASE(m_pCreateShadow);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pHeightMapmesh);


	SAFE_RELEASE(m_pBloomEffect);
	SAFE_RELEASE(m_pBloomRenderTarget);
	SAFE_RELEASE(m_pBloomDepthStencil);

	for (size_t i = 0; i < m_vecItem.size(); i++)
	{
		SAFE_RELEASE(m_vecItem[i]);
	}

	SAFE_DELETE(m_pConsole);
	m_ObjNodes.clear();

}

static CRITICAL_SECTION cs;


static DWORD WINAPI ThFunc1(LPVOID lpParam)
{
	EnterCriticalSection(&cs);
	DarknessofPlanetMainScene* temp = (DarknessofPlanetMainScene*)lpParam;

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			TeicEnemy* pSkinnedMesh = new TeicEnemy;
			int a = RND->getInt(10);
			if (a < 6)
			{
				pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
				pSkinnedMesh->SetPosition(temp->m_pNode->m_vRow[45 + j].m_vCol[26 + i].m_vPosList->m_vCenterPos);
				pSkinnedMesh->SetAttack(5);
				pSkinnedMesh->SetHP(100);
				pSkinnedMesh->SetSpeed(5);
				pSkinnedMesh->m_eName = Wolf;
			}
			else if (a < 9)
			{
				pSkinnedMesh->Setup("object/xFile/tiger/", "tiger.X");
				pSkinnedMesh->SetPosition(temp->m_pNode->m_vRow[45 + j].m_vCol[26 + i].m_vPosList->m_vCenterPos);
				pSkinnedMesh->SetAttack(8);
				pSkinnedMesh->SetHP(200);
				pSkinnedMesh->SetSpeed(10);
				pSkinnedMesh->m_eName = Tiger;
			}
			else if (a < 10)
			{
				pSkinnedMesh->Setup("object/xFile/ArgoniteGiant/", "ArgoniteGiant.X");
				pSkinnedMesh->SetPosition(temp->m_pNode->m_vRow[45 + j].m_vCol[26 + i].m_vPosList->m_vCenterPos);
				pSkinnedMesh->SetAttack(15);
				pSkinnedMesh->SetHP(500);
				pSkinnedMesh->SetSpeed(15);
				pSkinnedMesh->SetScaleSize(0.1);
				pSkinnedMesh->m_eName = Argo;
			}

			pSkinnedMesh->m_eGroup = Rush;
			temp->m_vecMakingEnemy.push_back(pSkinnedMesh);

		}
	}



	temp->m_bThread = true;
	LeaveCriticalSection(&cs);

	return 0;
}


static DWORD WINAPI ThFunc2(LPVOID lpParam)
{


	EnterCriticalSection(&cs);
	DarknessofPlanetMainScene* temp = (DarknessofPlanetMainScene*)lpParam;
	for (int i = 0; i < temp->m_vecEnemy.size(); i++)
	{
		if (!temp->m_vecEnemy[i]->m_bThreadCalOn)continue;
		if (temp->m_vecEnemy[i]->m_bAttackOn)continue;
		if (temp->m_vecEnemy[i]->GetDie())continue;
		temp->m_pAttackNode.x = temp->m_pCharacter->GetNodeNum().x + RND->getFromIntTo(-1, 1);
		temp->m_pAttackNode.y = temp->m_pCharacter->GetNodeNum().y + RND->getFromIntTo(-1, 1);
		temp->m_vecEnemyWay[i] = temp->m_pAstarShort->FindWay(temp->m_vecEnemy[i]->GetNodeNum().x, temp->m_vecEnemy[i]->GetNodeNum().y,
			temp->m_pAttackNode.x, temp->m_pAttackNode.y, temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);



	}
	temp->m_bAstarThread = false;
	LeaveCriticalSection(&cs);



	return 0;
}

static DWORD WINAPI ThFunc3(LPVOID lpParam)
{


	EnterCriticalSection(&cs);
	DarknessofPlanetMainScene* temp = (DarknessofPlanetMainScene*)lpParam;
	for (int i = 0; i < temp->m_vecEnemy.size(); i++)
	{
		if (temp->m_vecEnemy[i]->m_eMode == Attack)continue;
		if (temp->m_vecEnemy[i]->m_eGroup == Field)continue;
		if (temp->m_vecEnemy[i]->GetDie())continue;
		temp->m_pAttackNode.x = temp->m_pCharacter->GetNodeNum().x + RND->getFromIntTo(-1, 1);
		temp->m_pAttackNode.y = temp->m_pCharacter->GetNodeNum().y + RND->getFromIntTo(-1, 1);
		temp->m_vecEnemyWay[i] = temp->m_pAstarShort->FindWay(temp->m_vecEnemy[i]->GetNodeNum().x, temp->m_vecEnemy[i]->GetNodeNum().y,
			temp->m_pAttackNode.x, temp->m_pAttackNode.y, temp->m_pCharacter->GetNodeNum().x, temp->m_pCharacter->GetNodeNum().y);

	}
	temp->m_bAstarThread2 = false;
	LeaveCriticalSection(&cs);



	return 0;
}




HRESULT DarknessofPlanetMainScene::Setup()
{
	//윈도우 옵션 MOUSEMOVE 전역으로
	//마우스 설정
	SetCapture(g_hWnd);

	////////// 대원
	//## 초기화 리스트
	//## StaticMeshLoader, Object List controler
	//## 메쉬를 불러와서, 컨트롤러에 저장하고 뿌린다.
	m_pNode = NODEMANAGER->GetNode();

	TeicEnemy* pSkinnedMesh = new TeicEnemy;
	pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
	pSkinnedMesh->SetPosition(m_pNode->m_vRow[145].m_vCol[126].m_vPosList->m_vCenterPos);
	pSkinnedMesh->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, 10));
	pSkinnedMesh->SetAttack(5);
	pSkinnedMesh->SetHP(100);
	pSkinnedMesh->m_eGroup = Rush;
	m_vecEnemy.push_back(pSkinnedMesh);

	m_vecEnemyCollisionMove.resize(1);
	m_vecEnemyWay.resize(1);
	char str[256];
	sprintf_s(str, "SX%dSZ%dLX%dLZ%d", 45 + 0, 26 + 0, 46, 116);
	m_vecEnemyWay[0] = WAYMANAGER->GetWay(str);
	m_vecEnemyCollisionMove[0] = new TeicMoveSequence;


	m_meshList.ScriptLoader("Data/Script/ObjectList.txt", m_ObjNodes);
	m_pConsole = new cConsole;
	m_pConsole->Setup();

	//////////정현
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	cUIImageView* pAimImage = new cUIImageView;
	pAimImage->SetTexture("./UI/aimNormal.png");
	pAimImage->SetPosition(viewport.Width / 2.0f - 15, viewport.Height / 2.0f - 15);
	pAimImage->SetIsCenter(true);
	pAimImage->SetScale(0.1f, 0.1f);

	UIOBJECTMANAGER->AddRoot("aimTest", pAimImage, true);

	m_pInventory = new Inventory;
	m_pInventory->Setup();

	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");

	for (size_t i = 0; i < 16; i++)
	{
		FieldItem* pItem = new FieldItem();
		pItem->Setup(RND->getInt(6));
		D3DXVECTOR3 vPos;
		switch (i)
		{
		case 0:
			vPos = D3DXVECTOR3(64.4186f, 35.0f, -95.7918f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 1:
			vPos = D3DXVECTOR3(430.622f, 35.0f, -37.3627f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 2:
			vPos = D3DXVECTOR3(529.199f, 35.0f, -128.479f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 3:
			vPos = D3DXVECTOR3(493.756f, 35.0f, -296.701f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 4:
			vPos = D3DXVECTOR3(605.995f, 39.0f, -679.417f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 5:
			vPos = D3DXVECTOR3(859.68f, 52.0f, -871.927f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 6:
			vPos = D3DXVECTOR3(209.549f, 39.0f, -525.958f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 7:
			vPos = D3DXVECTOR3(280.177f, 60.0f, -256.315f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 8:
			vPos = D3DXVECTOR3(425.924f, 39.0f, -391.909f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 9:
			vPos = D3DXVECTOR3(959.669f, 39.0f, -333.557f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 10:
			vPos = D3DXVECTOR3(654.76f, 39.0f, -522.515f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 11:
			vPos = D3DXVECTOR3(428.911f, 39.0f, -614.705f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 12:
			vPos = D3DXVECTOR3(402.471f, 39.0f, -619.421f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 13:
			vPos = D3DXVECTOR3(451.957f, 39.0f, -641.89f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 14:
			vPos = D3DXVECTOR3(506.443f, 39.0f, -609.262f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		case 15:
			vPos = D3DXVECTOR3(234.735f, 62.0f, -288.516f);
			m_pMap->GetHeight(vPos.x, vPos.y, vPos.z);
			pItem->SetPosition(vPos);
			break;
		}

		m_vecItem.push_back(pItem);
	}

	///////////동윤
	/*m_pCamera = new LDYCamera;*/
	m_pGrid = new Hank::cGrid;

	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, m_iBodyUpgrade);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(m_pNode->m_vRow[46].m_vCol[116].m_vPosList->m_vCenterPos);
	CAMERA->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, 0));

	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;



	// 렌더타깃을 만든다.
	RECT BloomRc;
	GetClientRect(g_hWnd, &BloomRc);

	GETDEVICE->CreateTexture(
		BloomRc.right,
		BloomRc.bottom,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBloomRenderTarget,
		NULL);

	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	GETDEVICE->CreateDepthStencilSurface(
		BloomRc.right,
		BloomRc.bottom,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pBloomDepthStencil,
		NULL);

	m_pBloomEffect = LoadEffect("shader/Shadow/blur.fx");


	m_vecVertex.resize(6);
	m_vecVertex[0].p = D3DXVECTOR4(0, 0, 0, 1);
	m_vecVertex[0].t = D3DXVECTOR2(0, 0);

	m_vecVertex[1].p = D3DXVECTOR4(BloomRc.right, 0, 0, 1);
	m_vecVertex[1].t = D3DXVECTOR2(1, 0);

	m_vecVertex[2].p = D3DXVECTOR4(BloomRc.right, BloomRc.bottom, 0, 1);
	m_vecVertex[2].t = D3DXVECTOR2(1, 1);

	m_vecVertex[3].p = D3DXVECTOR4(0, 0, 0, 1);
	m_vecVertex[3].t = D3DXVECTOR2(0, 0);

	m_vecVertex[4].p = D3DXVECTOR4(BloomRc.right, BloomRc.bottom, 0, 1);
	m_vecVertex[4].t = D3DXVECTOR2(1, 1);

	m_vecVertex[5].p = D3DXVECTOR4(0, BloomRc.bottom, 0, 1);
	m_vecVertex[5].t = D3DXVECTOR2(0, 1);


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

	///
	m_hCmatLightView = m_pCreateShadow->GetParameterByName(0, "matLightView");
	m_hCmatLightProjection = m_pCreateShadow->GetParameterByName(0, "gLightProjectionMatrix");
	m_hCTechnic = m_pCreateShadow->GetTechniqueByName("Skinning20");


	m_hApplyTexture = m_pApplyShadow->GetParameterByName(0, "ShadowMap_Tex");
	m_hAmatWorld = m_pApplyShadow->GetParameterByName(0, "matWorld");
	m_hAmatLightView = m_pApplyShadow->GetParameterByName(0, "matLightView");
	m_hAmatLightProjection = m_pApplyShadow->GetParameterByName(0, "matLightProjection");
	m_hAm_vec4LightPosition = m_pApplyShadow->GetParameterByName(0, "m_vec4LightPosition");
	m_hAmatViewProjection = m_pApplyShadow->GetParameterByName(0, "matViewProjection");
	m_hAgObjectColor = m_pApplyShadow->GetParameterByName(0, "gObjectColor");



	/////////////태영
	m_pFont = FONTMANAGER->GetFont(cFontManager::E_NORMAL);
	m_pGrid->Setup();

	//노드 추가 합니다.
	m_pNode = NODEMANAGER->GetNode();
	//대원 -> 노드가 필요해 부득이 이쪽에서 실행
	for each(auto p in m_ObjNodes)
	{
		p->PutBoundBoxtoNodeByPosition(m_pNode);
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
	m_fTime6 = 0.0f;

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


	m_pBoss->Setup("object/xFile/Valak/", "Valak.X");



	m_pBoss->SetPosition(m_pNode->m_vRow[174].m_vCol[74].m_vPosList->m_vCenterPos);


	m_pBoss->SetAnimation(31);

	m_pBoss->SetScaleSize(0.1);

	m_pBoss->SetHp(10000);
	m_pBoss->SetSpeed(20);


	m_pBoss->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, 5));
	m_pBossMove = new TeicCollisionMove;


	SKILLEFFECTMANAGER->AddEffect("Explosion", Skill_Explosion, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Breath", Skill_breath, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Blizzard", Skill_Blizzard, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Blood", Character_Blood, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 10);
	SKILLEFFECTMANAGER->AddEffect("MBlood", Monseter_Blood, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 20);
	SKILLEFFECTMANAGER->AddEffect("MChill", Monster_Chill, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 20);
	SKILLEFFECTMANAGER->AddEffect("Flame", Flame, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 30);
	SKILLEFFECTMANAGER->AddEffect("Laser", Laser, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);




	D3DXCreateTextureFromFile(GETDEVICE, "map/FFFFFFFFFF.png", &tex);

	for (int i = 0; i < 20; i++)
	{
		MakingFieldEnemy();
	}

	m_pSkyDome->m_fNowtime = 0;
	SOUNDMANAGER->play("BGM_Walk");
	return S_OK;
}

void DarknessofPlanetMainScene::Release()
{
	m_pGrid->Release();

	SAFE_DELETE(m_pMap);
	/*SAFE_DELETE(m_pCamera);*/
	SAFE_DELETE(m_pCharacter);



}

void DarknessofPlanetMainScene::Update()
{
	SOUNDMANAGER->Update();
	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);
	if (!m_pBoss->m_bAttackOn)
		m_pBossMove->Update();
	m_pMap->GetHeight(m_pBoss->GetPositionPointer()->x, m_pBoss->GetPositionPointer()->y, m_pBoss->GetPositionPointer()->z);
	m_pBoss->Update(m_pCharacter->GetPosition());
	BossAttack();

	if (m_pInventory) m_pInventory->Update(m_pCharacter);
	if (m_pSkyDome)m_pSkyDome->Update();
	if (m_pSkyCloud)m_pSkyCloud->Update();

	for (size_t i = 0; i < m_vecItem.size(); i++)
	{
		m_vecItem[i]->Update();

		if (m_vecItem[i]->GetIsNull() && m_vecItem[i]->GetNullTime() >= 8.0f)
		{
			m_vecItem[i]->SetIsNull(false);
			m_vecItem[i]->SetNameNumber(RND->getInt(5));
		}
		else if (!m_vecItem[i]->GetIsNull() && D3DXVec3Length(&(m_pCharacter->GetPosition() - m_vecItem[i]->GetPosition())) < 2.0f)
		{
			m_pInventory->AddItem(m_vecItem[i]->GetName(), m_vecItem[i]->GetItemType());
			m_vecItem[i]->SetIsNull(true);
		}
	}



	if (m_iCameranum == 0)
	{
		CAMERA->Update(&m_pCharacter->GetPosition(), 0);
	}
	else
	{
		CAMERA->Update(&m_pBoss->GetPosition(), 1);
	}

	m_pCharacter->Update(CAMERA->getAngleY());
	//bool check = ChangeCheckPoint();
	MakingEnemy();
	if (TIMEMANAGER->getWorldTime() > m_fTime7 + 20 && m_vecEnemy.size() < 30)
	{
		m_fTime7 = TIMEMANAGER->getWorldTime();
		MakingFieldEnemy();
	}



	/*if (check)
	{
		m_fTime2 = 0;
	}*/
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
	if (TIMEMANAGER->getWorldTime() > m_fTime6 + 5.0f)
	{
		m_fTime6 = TIMEMANAGER->getWorldTime();
		if (!m_bAstarThread2)
		{
			m_bAstarThread2 = true;
			DWORD dwThID3;
			HANDLE hThreads3;

			unsigned long ulStackSize = 0;
			dwThID3 = 1;
			hThreads3 = NULL;
			hThreads3 = CreateThread(NULL, ulStackSize, ThFunc3, this, CREATE_SUSPENDED, &dwThID3);
			ResumeThread(hThreads3);
		}

	}

	////////// 찾기
	if (m_pSkyDome->m_fNowtime > 1 && m_fTime < 1000 && m_vecEnemy.size() < 40)
	{
		m_fTime = INF;
		DWORD dwThID1;
		HANDLE hThreads;
		unsigned long ulStackSize = 0;
		dwThID1 = 0;
		hThreads = NULL;
		hThreads = CreateThread(NULL, ulStackSize, ThFunc1, this, CREATE_SUSPENDED, &dwThID1);
		ResumeThread(hThreads);
		SOUNDMANAGER->stop("BGM_Walk");
		SOUNDMANAGER->play("BGM_Suspence");
		m_iSound = 0;
	}

	if (m_pSkyDome->m_fNowtime < 1)
	{
		m_fTime = 0;
		if (m_iSound == 0)
		{
			SOUNDMANAGER->stop("BGM_Suspence");
			SOUNDMANAGER->play("BGM_Walk");
			m_iSound++;
		}
		
	}


	CleanHit();

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

					//m_pShoot->Shoot(m_pCharacter->getWeaponType());
					//CAMERA->rebound();
				}

			}
			else
			{
				m_fTime4 = TIMEMANAGER->getWorldTime();

				if (m_pCharacter->m_bTCallback)
				{
					if (m_pCharacter->getWeaponType() == WP_FireGun)
					{
						SOUNDMANAGER->play("Firegun");

						m_pCharacter->m_pCtrl->setAttacking(true);
						m_pShoot->Shoot(m_pCharacter->getWeaponType());
						D3DXVECTOR3 target = m_pShoot->GetStartPosition() + m_pShoot->GetDir() * 20;
						target.x += RND->getFromFloatTo(-2, 2);
						target.y += RND->getFromFloatTo(-2, 2);
						target.z += RND->getFromFloatTo(-2, 2);
						SKILLEFFECTMANAGER->play("Flame", target, m_pCharacter->getMuzzlePos());

					}
					else
					{
						switch (m_pCharacter->getWeaponType())
						{
						case Wp_Melee:
							break;
						case Wp_AA12:
							break;
						case Wp_AR6:
							SOUNDMANAGER->play("ArFire");
							break;
						case Wp_M4:
							SOUNDMANAGER->play("M4Fire");
							break;
						case Wp_MP5:
							SOUNDMANAGER->play("M5Fire");
							break;
						case WP_FireGun:
							break;
						default:
							break;
						}
						for (int i = 0; i < m_vecEnemy.size(); i++)
						{
							if (m_vecEnemy[i]->GetDie())continue;
							m_vecEnemy[i]->MakeBoundingBox();
						}
						m_pBoss->MakeBoundingBox();
						m_pCharacter->m_pCtrl->setAttacking(true);
						m_pShoot->Shoot(m_pCharacter->getWeaponType());
						CAMERA->rebound();
					}
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
		if (m_vecEnemy[i]->GetDie())continue;
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
				switch (m_vecEnemy[i]->m_eName)
				{
				case Wolf:
					SOUNDMANAGER->play("WolfAttack");
					break;
				case Argo:
					SOUNDMANAGER->play("ArgoAttack");
					break;
				case Tiger:
					SOUNDMANAGER->play("TigerAttack");
					break;
				default:
					break;
				}
			}
		}
	}


	TotalCollisionCheck();

	for (int i = 0; i < m_vecEnemyCollisionMove.size(); i++)
	{
		if (m_vecEnemy[i]->m_bAttackOn)continue;
		if (m_vecEnemy[i]->GetDie())continue;
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
	if (TIMEMANAGER->getWorldTime() > m_fTime3 + 0.1f)
	{
		m_fTime3 = TIMEMANAGER->getWorldTime();
		TargetOn();
	}
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (!m_vecEnemy[i]->m_bAttackOn)continue;
		if (m_vecEnemy[i]->GetDie())continue;
		AngleChange(m_vecEnemy[i]);

	}
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{

		m_pMap->GetHeight(m_vecEnemy[i]->GetPositionPointer()->x, m_vecEnemy[i]->GetPositionPointer()->y, m_vecEnemy[i]->GetPositionPointer()->z);
	}



	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
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


	m_pMap->GetHeight(m_pCharacter->GetPositionPointer()->x, m_pCharacter->GetPositionPointer()->y, m_pCharacter->GetPositionPointer()->z);

	ChangeGridInfo();


	UIOBJECTMANAGER->Update();

	m_pConsole->Update();



}

void DarknessofPlanetMainScene::MakingEnemy()
{
	if (m_bThread == true)
	{
		int num = m_vecEnemy.size();
		for (int i = 0; i < m_vecMakingEnemy.size(); i++)
		{
			m_vecMakingEnemy[i]->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, num + i + 10));
			m_vecEnemy.push_back(m_vecMakingEnemy[i]);
		}


		m_vecEnemyWay.resize(m_vecEnemy.size());
		m_vecEnemyCollisionMove.resize(m_vecEnemy.size());

		for (int i = 0; i < m_vecMakingEnemy.size(); i++)
		{

			char str[256];
			sprintf_s(str, "SX%dSZ%dLX%dLZ%d", 26 + i % 10, 45 + i / 10, 116, 46);
			m_vecEnemyWay[num + i] = WAYMANAGER->GetWay(str);



			m_vecEnemyCollisionMove[num + i] = new TeicMoveSequence;

			for (int j = 0; j < m_vecEnemyWay[num + i].size(); j++)
			{
				if (j + 1 >= m_vecEnemyWay[num + i].size())break;
				TeicCollisionMove* tempmove;
				tempmove = new TeicCollisionMove;
				tempmove->SetSkinnedTarget(m_vecEnemy[num + i]->GetSkinnedMesh());
				tempmove->SetSpeed(5);
				tempmove->SetFrom(m_vecEnemyWay[num + i][j]);
				tempmove->SetTo(m_vecEnemyWay[num + i][j + 1]);
				m_vecEnemyCollisionMove[num + i]->AddAction(tempmove);

			}
			m_vecEnemyCollisionMove[num + i]->Start();
			m_vecEnemyWay[num + i].clear();

		}

		m_vecMakingEnemy.clear();
		m_bThread = false;
	}

}
bool DarknessofPlanetMainScene::CollisionSkill()
{
	//m_pBoss->m_vecCheckCube;

	//0----1
	//|    |
	//2----3

	/*DWORD Index[6];
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 2;
	Index[5] = 3;*/



	D3DXVECTOR3 m_CollisionVec[4];
	m_CollisionVec[0] = D3DXVECTOR3(m_pBoss->m_vecCheckCube[0].p.x, m_pBoss->m_vecCheckCube[0].p.z, 0);
	m_CollisionVec[1] = D3DXVECTOR3(m_pBoss->m_vecCheckCube[1].p.x, m_pBoss->m_vecCheckCube[1].p.z, 0);
	m_CollisionVec[2] = D3DXVECTOR3(m_pBoss->m_vecCheckCube[2].p.x, m_pBoss->m_vecCheckCube[2].p.z, 0);
	m_CollisionVec[3] = D3DXVECTOR3(m_pBoss->m_vecCheckCube[5].p.x, m_pBoss->m_vecCheckCube[5].p.z, 0);


	D3DXVECTOR3 Mouse;
	Mouse.x = m_pCharacter->GetPosition().x;
	Mouse.y = m_pCharacter->GetPosition().z;
	Mouse.z = 0;
	if (!LineDotDown(m_CollisionVec[0], m_CollisionVec[1], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[1], m_CollisionVec[3], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[3], m_CollisionVec[2], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[2], m_CollisionVec[0], Mouse))
	{
		return false;
	}


	return true;




}

bool DarknessofPlanetMainScene::LineDotDown(D3DXVECTOR3 startDot, D3DXVECTOR3 finishDot, D3DXVECTOR3 targetDot)
{
	D3DXVECTOR3 A_B = finishDot - startDot;
	D3DXVECTOR3 A_C = targetDot - startDot;

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &A_B, &A_C);

	if (Cross.z <= 0)
		return true;
	else
		return false;

}

void DarknessofPlanetMainScene::CallbackOn(int number)
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
	else if (number == 5)
	{
		if (m_pBoss->GetAninum() == 40)
		{
			m_pBoss->m_eType = Boss_Attack;
			m_iCameranum = 0;
			CAMERA->m_fDistance = 8;
			SOUNDMANAGER->play("BGM_BattleRage");
			SOUNDMANAGER->stop("BGM_Walk");
			SOUNDMANAGER->stop("BGM_Suspence");
		}
		if (m_pBoss->GetAninum() == 6)
		{
			if (D3DXVec3Length(&(m_pBoss->m_vCharacterPos - m_pCharacter->GetPosition())) > 30)
			{
				return;
			}



			float body;
			switch (m_pCharacter->m_iBodyLv)
			{
			case 0:
				body = DATABASE->GetItemValue("Armor");
				break;
			case 1:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 2:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 3:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			default:
				break;
			}
			float helmet;
			switch (m_pCharacter->m_iHeadLv)
			{
			case 0:
				helmet = DATABASE->GetItemValue("Mask");
				break;
			case 1:
				helmet = DATABASE->GetItemValue("Helmet");
				break;
			default:
				break;
			}
			int damage = 50 - (body - helmet);
			if (damage < 1) damage = 1;
			m_pCharacter->m_iHP -= damage;
			SKILLEFFECTMANAGER->play("Blood", m_pCharacter->GetPosition(), m_pCharacter->GetPosition());
			SOUNDMANAGER->play("Hit");
		}
		if (m_pBoss->GetAninum() == 12)
		{
			if (D3DXVec3Length(&(m_pBoss->GetPosition() - m_pCharacter->GetPosition())) > 100)
			{
				return;
			}
			float body;
			switch (m_pCharacter->m_iBodyLv)
			{
			case 0:
				body = DATABASE->GetItemValue("Armor");
				break;
			case 1:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 2:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 3:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			default:
				break;
			}
			float helmet;
			switch (m_pCharacter->m_iHeadLv)
			{
			case 0:
				helmet = DATABASE->GetItemValue("Mask");
				break;
			case 1:
				helmet = DATABASE->GetItemValue("Helmet");
				break;
			default:
				break;
			}
			int damage = 30 - (body - helmet);
			if (damage < 1) damage = 1;
			m_pCharacter->m_iHP -= damage;
			SKILLEFFECTMANAGER->play("Blood", m_pCharacter->GetPosition(), m_pCharacter->GetPosition());
			SOUNDMANAGER->play("Hit");
		}
		if (m_pBoss->GetAninum() == 22)
		{
			if (CollisionSkill() == false)
				return;

			float body;
			switch (m_pCharacter->m_iBodyLv)
			{
			case 0:
				body = DATABASE->GetItemValue("Armor");
				break;
			case 1:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 2:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			case 3:
				body = DATABASE->GetItemValue("FullArmor");
				break;
			default:
				break;
			}
			float helmet;
			switch (m_pCharacter->m_iHeadLv)
			{
			case 0:
				helmet = DATABASE->GetItemValue("Mask");
				break;
			case 1:
				helmet = DATABASE->GetItemValue("Helmet");
				break;
			default:
				break;
			}
			int damage = 40 - (body - helmet);
			if (damage < 1) damage = 1;
			m_pCharacter->m_iHP -= damage;
			SKILLEFFECTMANAGER->play("Blood", m_pCharacter->GetPosition(), m_pCharacter->GetPosition());
			SOUNDMANAGER->play("Hit");
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

				float body;
				switch (m_pCharacter->m_iBodyLv)
				{
				case 0:
					body = DATABASE->GetItemValue("Armor");
					break;
				case 1:
					body = DATABASE->GetItemValue("FullArmor");
					break;
				case 2:
					body = DATABASE->GetItemValue("FullArmor");
					break;
				case 3:
					body = DATABASE->GetItemValue("FullArmor");
					break;
				default:
					break;
				}
				float helmet;
				switch (m_pCharacter->m_iHeadLv)
				{
				case 0:
					helmet = DATABASE->GetItemValue("Mask");
					break;
				case 1:
					helmet = DATABASE->GetItemValue("Helmet");
					break;
				default:
					break;
				}
				int damage = m_vecEnemy[i - 10]->GetAttack() - (body - helmet);
				if (damage < 1) damage = 1;
				m_pCharacter->m_iHP -= damage;
				SKILLEFFECTMANAGER->play("Blood", m_pCharacter->GetPosition(), m_pCharacter->GetPosition());
				SOUNDMANAGER->play("Hit");
			}

			if (m_vecEnemy[i - 10]->GetAninum() == 4)
			{
				m_vecEnemy[i - 10]->SetShow(false);
				if (m_pNode->m_vRow[m_vecEnemy[i - 10]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i - 10]->m_PreviousGrid.x].m_pBoundInfo != NULL)
				{
					for (int j = 0; j < m_pNode->m_vRow[m_vecEnemy[i - 10]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i - 10]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.size(); j++)
					{
						if (m_vecEnemy[i - 10]->GetSkinnedMesh() == m_pNode->m_vRow[m_vecEnemy[i - 10]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i - 10]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
						{
							m_pNode->m_vRow[m_vecEnemy[i - 10]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i - 10]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.erase(
								m_pNode->m_vRow[m_vecEnemy[i - 10]->m_PreviousGrid.y].m_vCol[m_vecEnemy[i - 10]->m_PreviousGrid.x].m_pBoundInfo->m_vecBounding.begin() + j);

						}
					}
				}


				if (m_pNode->m_vRow[m_vecEnemy[i - 10]->GetNodeNum().y].m_vCol[m_vecEnemy[i - 10]->GetNodeNum().x].m_pBoundInfo != NULL)
				{
					for (int j = 0; j < m_pNode->m_vRow[m_vecEnemy[i - 10]->GetNodeNum().y].m_vCol[m_vecEnemy[i - 10]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.size(); j++)
					{
						if (m_vecEnemy[i - 10]->GetSkinnedMesh() == m_pNode->m_vRow[m_vecEnemy[i - 10]->GetNodeNum().y].m_vCol[m_vecEnemy[i - 10]->GetNodeNum().x].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
						{
							m_pNode->m_vRow[m_vecEnemy[i - 10]->GetNodeNum().y].m_vCol[m_vecEnemy[i - 10]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.erase(
								m_pNode->m_vRow[m_vecEnemy[i - 10]->GetNodeNum().y].m_vCol[m_vecEnemy[i - 10]->GetNodeNum().x].m_pBoundInfo->m_vecBounding.begin() + j);

						}
					}
				}

				m_vecEnemy.erase(m_vecEnemy.begin() + (i - 10));
				m_vecEnemyWay.erase(m_vecEnemyWay.begin() + (i - 10));
				m_vecEnemyCollisionMove.erase(m_vecEnemyCollisionMove.begin() + (i - 10));

				for (int k = 0; k < m_vecEnemy.size(); k++)
				{
					m_vecEnemy[k]->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, 10 + k));
				}

			}
		}
	}

}

bool DarknessofPlanetMainScene::CollisionCheck(TeicEnemy * A, TeicEnemy * B)
{
	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
	{
		if (m_pCollision->CheckCollision(A->GetBoundingSquare(), B->GetBoundingSquare()) == false)
			return false;
		float Adist = D3DXVec3Length(&(A->GetPositionYzero() - m_pCharacter->GetPositionYZero()));
		float Bdist = D3DXVec3Length(&(B->GetPositionYzero() - m_pCharacter->GetPositionYZero()));


		if (Adist < Bdist)
		{
			if (B->m_eMode == Idle)
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
			if (A->m_eMode == Idle)
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



void DarknessofPlanetMainScene::TotalPushCheck2()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		for (int j = i + 1; j < m_vecEnemy.size(); j++)
		{
			if (m_vecEnemy[i]->GetDie())continue;
			if (m_vecEnemy[j]->GetDie())continue;
			Push2(m_vecEnemy[i], m_vecEnemy[j]);

		}
	}
	ChangeGridInfo();
	CheckSlot();
}

void DarknessofPlanetMainScene::ChangeGridInfo()
{

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		m_vecEnemy[i]->m_PresentGrid = m_vecEnemy[i]->GetNodeNum();
	}
	///비교
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
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



	//보스
	m_pBoss->m_PresentGrid = m_pBoss->GetNodeNum();
	/////////// 비교

	if (m_pBoss->m_PreviousGrid.x == m_pBoss->m_PresentGrid.x &&
		m_pBoss->m_PreviousGrid.y == m_pBoss->m_PresentGrid.y)return;

	for (int a = -1; a < 2; a++)
	{
		for (int b = -1; b < 2; b++)
		{
			if (m_pBoss->m_PreviousGrid.y + a < 0)continue;
			if (m_pBoss->m_PreviousGrid.x + b < 0)continue;
			if (m_pBoss->m_PresentGrid.y + a < 0)continue;
			if (m_pBoss->m_PresentGrid.x + b < 0)continue;
			if (m_pNode->m_vRow[m_pBoss->m_PreviousGrid.y + a].m_vCol[m_pBoss->m_PreviousGrid.x + b].m_pBoundInfo != NULL)
			{
				for (int j = 0; j < m_pNode->m_vRow[m_pBoss->m_PreviousGrid.y + a].m_vCol[m_pBoss->m_PreviousGrid.x + b].m_pBoundInfo->m_vecBounding.size(); j++)
				{
					if (m_pBoss->GetSkinnedMesh() == m_pNode->m_vRow[m_pBoss->m_PreviousGrid.y + a].m_vCol[m_pBoss->m_PreviousGrid.x + b].m_pBoundInfo->m_vecBounding[j]->m_pSkinnedObject)
					{
						m_pNode->m_vRow[m_pBoss->m_PreviousGrid.y + a].m_vCol[m_pBoss->m_PreviousGrid.x + b].m_pBoundInfo->m_vecBounding.erase(
							m_pNode->m_vRow[m_pBoss->m_PreviousGrid.y + a].m_vCol[m_pBoss->m_PreviousGrid.x + b].m_pBoundInfo->m_vecBounding.begin() + j);

					}
				}
			}

		}
	}

	for (int a = -1; a < 2; a++)
	{
		for (int b = -1; b < 2; b++)
		{
			if (m_pBoss->m_PreviousGrid.y + a < 0)continue;
			if (m_pBoss->m_PreviousGrid.x + b < 0)continue;
			if (m_pBoss->m_PresentGrid.y + a < 0)continue;
			if (m_pBoss->m_PresentGrid.x + b < 0)continue;
			if (m_pNode->m_vRow[m_pBoss->m_PresentGrid.y + a].m_vCol[m_pBoss->m_PresentGrid.x + b].m_pBoundInfo == NULL)
			{
				m_pNode->m_vRow[m_pBoss->m_PresentGrid.y + a].m_vCol[m_pBoss->m_PresentGrid.x + b].m_pBoundInfo = new nNodeBoundInfo;
				m_pNode->m_vRow[m_pBoss->m_PresentGrid.y + a].m_vCol[m_pBoss->m_PresentGrid.x + b].m_pBoundInfo->m_vecBounding.push_back(
					m_pBoss->GetBoundingSquare());
			}
			else
			{
				m_pNode->m_vRow[m_pBoss->m_PresentGrid.y + a].m_vCol[m_pBoss->m_PresentGrid.x + b].m_pBoundInfo->m_vecBounding.push_back(
					m_pBoss->GetBoundingSquare());
			}

		}
	}

	m_pBoss->m_PreviousGrid = m_pBoss->GetNodeNum();


}

void DarknessofPlanetMainScene::TargetOn()
{
	int a = 0;

	if (m_pBoss->m_eType == Boss_Idle)
	{
		if (D3DXVec3Length(&(m_pCharacter->GetPositionYZero() - m_pBoss->GetPositionYzero())) < 70.0f)
		{

			
			m_iCameranum = 1;
			m_pBoss->m_eType = Boss_Show;
			m_pBoss->SetAnimation(40);
			SOUNDMANAGER->stop("BGM_Walk");
			SOUNDMANAGER->stop("BGM_Suspence");
			SOUNDMANAGER->play("ValakVoice");
		}
	}
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		if (EnemyPlayerDistance(m_vecEnemy[i]) < 100.0f)
		{
			m_vecEnemy[i]->m_eMode = Attack;
			if (m_vecEnemy[i]->m_eGroup == Rush)
			{
				a++;
			}
		}
	}

	if (a != 0)
	{
		for (int i = 0; i < m_vecEnemy.size(); i++)
		{
			if (m_vecEnemy[i]->GetDie())continue;
			if (m_vecEnemy[i]->m_eGroup == Rush)
			{
				m_vecEnemy[i]->m_eMode = Attack;
			}
		}
	}

}

bool DarknessofPlanetMainScene::CheckStraight(TeicEnemy *A)
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

float DarknessofPlanetMainScene::EnemyEnemyDistance(TeicEnemy * A, TeicEnemy * B)
{
	float length;
	length = D3DXVec3Length(&(A->GetPositionYzero() - B->GetPositionYzero()));
	return length;
}

float DarknessofPlanetMainScene::EnemyPlayerDistance(TeicEnemy *ene)
{
	float length;
	length = D3DXVec3Length(&(ene->GetPositionYzero() - m_pCharacter->GetPositionYZero()));
	return length;
}


void DarknessofPlanetMainScene::Render()
{
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	////블룸용 변수들
	LPDIRECT3DSURFACE9 pHWBackBufferBloom = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBufferBloom = NULL;

	GETDEVICE->GetRenderTarget(0, &pHWBackBufferBloom);
	GETDEVICE->GetDepthStencilSurface(&pHWDepthStencilBufferBloom);

	LPDIRECT3DSURFACE9 pTempSurface = NULL;
	m_pBloomRenderTarget->GetSurfaceLevel(0, &pTempSurface);

	GETDEVICE->SetRenderTarget(0, pTempSurface);
	GETDEVICE->SetDepthStencilSurface(m_pBloomDepthStencil);

	GETDEVICE->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f, 0);

	//여기다가 그림그려야함 
	GETDEVICE->SetRenderState(D3DRS_RANGEFOGENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_FOGENABLE, false);

	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	if (m_pSkyDome)m_pSkyDome->Render();
	if (m_pSkyCloud)m_pSkyCloud->Render();

	for each(auto i in m_vecItem)
	{
		i->Render();
	}

	D3DXVECTOR3 light = m_pCharacter->GetPositionYZero();

	{
		m_vec4LightPosition = { light.x + 100.0f,light.y + 200.0f,light.z + 100.0f,1.0f };
		D3DXVECTOR3 vEyePt(m_vec4LightPosition.x, m_vec4LightPosition.y, m_vec4LightPosition.z);
		D3DXVECTOR3 vLookatPt = m_pCharacter->GetPositionYZero();
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


	// 저번 프레임에 그렸던 그림자 정보를 지움
	GETDEVICE->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

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


	D3DCOLOR m_d3dFogColor = D3DCOLOR_XRGB(217, 217, 217);
	float start = 0.0f;
	float end = 200.0f;
	float m_fFogDensity = 0.01f;
	GETDEVICE->SetRenderState(D3DRS_FOGENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	GETDEVICE->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	GETDEVICE->SetRenderState(D3DRS_RANGEFOGENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_FOGCOLOR, m_d3dFogColor);
	GETDEVICE->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	GETDEVICE->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&end));
	GETDEVICE->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fFogDensity));


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



	m_pApplyShadow->SetTexture("heightMap_Tex", tex);

	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pApplyShadow->BeginPass(i);
			{
				// 디스크를 그린다.
				m_pApplyShadow->CommitChanges();
				//if (m_pMap)m_pMap->MeshRender(m_pCharacter->GetPositionYZero());

				if (m_pMap)m_pMap->frustumcullingRender();
			}
			m_pApplyShadow->EndPass();
		}
	}
	m_pApplyShadow->End();

	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	if (m_pCharacter)m_pCharacter->UpdateAndRender();
	if (m_pInventory) m_pInventory->Render();


	for (int i = 0; i < m_vecEnemy.size(); i++)
	{

		m_vecEnemy[i]->UpdateAndRender();
	}



	char str[256];
	sprintf_s(str, "%d %d", m_pCharacter->GetNodeNum().x, m_pCharacter->GetNodeNum().y);
	LPD3DXFONT f;
	f = FONTMANAGER->GetFont(cFontManager::E_NORMAL);
	f->DrawTextA(NULL, str, strlen(str), &RectMake(100, 200, 100, 100), DT_NOCLIP | DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 0, 0));



	if (m_pBoss->m_eType != Boss_Idle)
		m_pBoss->UpdateAndRender();

	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, true);

	SKILLEFFECTMANAGER->Update();
	SKILLEFFECTMANAGER->Render();
	for (std::list<cObjectNode*>::iterator i = m_ObjNodes.begin(); i != m_ObjNodes.end(); ++i)
	{
		cObjectNode* pNode = *i;
		D3DXMATRIX matWorld;
		pNode->GetWorldMatrix(&matWorld);
		GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

		pNode->m_pModel->Render(GETDEVICE);
	}
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);


	AfterImage();

	GETDEVICE->SetRenderTarget(0, pHWBackBufferBloom);
	GETDEVICE->SetDepthStencilSurface(pHWDepthStencilBufferBloom);

	GETDEVICE->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xFFFFFFFF,
		1.0f, 0);

	D3DSURFACE_DESC destDesc;
	pTempSurface->GetDesc(&destDesc);

	float VBloomWeights[9];
	float VBloomOffsets[9];

	for (int i = 0; i < 9; i++)
	{
		VBloomOffsets[i] = (static_cast<float>(i) - 4.0f) * (1.0f / static_cast<float>(destDesc.Height));


		float x = (static_cast<float>(i) - 4.0f) / 4.0f;

		VBloomWeights[i] = 0.3f * ComputeGaussianValue(x, 0.0f, 0.2f);
	}

	m_pBloomEffect->SetFloatArray("VBloomWeights", VBloomWeights, 9);
	m_pBloomEffect->SetFloatArray("VBloomOffsets", VBloomOffsets, 9);


	float HBloomWeights[9];
	float HBloomOffsets[9];

	for (int i = 0; i < 9; i++)
	{
		HBloomOffsets[i] = (static_cast<float>(i) - 4.0f) * (1.0f / static_cast<float>(destDesc.Width));

		float x = (static_cast<float>(i) - 4.0f) / 4.0f;

		HBloomWeights[i] = 0.3f * ComputeGaussianValue(x, 0.0f, 0.2f);
	}

	m_pBloomEffect->SetFloatArray("HBloomWeights", HBloomWeights, 9);
	m_pBloomEffect->SetFloatArray("HBloomOffsets", HBloomOffsets, 9);

	m_pBloomEffect->SetTexture("TempRenderTargetTexture", m_pBloomRenderTarget);

	UINT uiPasses = 0;
	m_pBloomEffect->Begin(&uiPasses, 0);
	for (UINT i = 0; i < uiPasses; ++i)
	{
		m_pBloomEffect->BeginPass(i);

		/*	GETDEVICE->SetTexture(0, m_pBloomRenderTarget);*/
		GETDEVICE->SetFVF(ST_RHWT_VERTEX::FVF);
		GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_RHWT_VERTEX));

		m_pBloomEffect->EndPass();
	}

	m_pBloomEffect->End();



	SAFE_RELEASE(pTempSurface);
	SAFE_RELEASE(pHWBackBufferBloom);
	SAFE_RELEASE(pHWDepthStencilBufferBloom);











	UIOBJECTMANAGER->Render();

	m_pConsole->Render();



}


bool DarknessofPlanetMainScene::TotalCollisionCheck()
{
	bool check;
	check = false;
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		m_vecEnemy[i]->SetCollision(false);
	}

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		for (int j = i + 1; j < m_vecEnemy.size(); j++)
		{
			if (m_vecEnemy[i]->m_eMode == Field)continue;
			if (m_vecEnemy[j]->m_eMode == Field)continue;
			if (m_vecEnemy[i]->GetDie())continue;
			if (m_vecEnemy[j]->GetDie())continue;
			if (CollisionCheck(m_vecEnemy[i], m_vecEnemy[j]))
			{
				check = true;
			}
		}
	}
	return check;
}



bool DarknessofPlanetMainScene::CheckSlot()
{
	bool temp;
	int count = -1;
	for (int i = 0; i < m_vecAttackSlot.size(); i++)
	{
		m_vecAttackSlot[i] = false;
	}
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		m_vecEnemy[i]->SetSlot(false);
		if (m_vecEnemy[i]->m_eMode == Attack)
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
						if (m_pNode->m_vRow[m_pCharacter->GetNodeNum().y + i].m_vCol[m_pCharacter->GetNodeNum().x + j].m_pBoundInfo->m_vecBounding[a]->st_Type != Bounding_Object)
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

bool DarknessofPlanetMainScene::ChangeCheckPoint()
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

void DarknessofPlanetMainScene::AngleChange(TeicEnemy * A)
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

		if (targetangle - Angle > D3DX_PI)
		{
			Angle -= 0.05;
			if (Angle < 0)
			{
				Angle = D3DX_PI * 2 - Angle;
			}
		}
		else
		{
			Angle += 0.05;
			if (Angle > targetangle) Angle = targetangle;
		}

	}
	else
	{

		if (Angle - targetangle > D3DX_PI)
		{
			Angle += 0.05;
			if (Angle > D3DX_PI * 2)
			{
				Angle = Angle - D3DX_PI * 2;
			}
		}
		else
		{
			Angle -= 0.05;
			if (Angle < targetangle) Angle = targetangle;
		}
	}


	//if (targetangle > Angle)
	//{
	//	Angle += 0.05;
	//	if (Angle > targetangle) Angle = targetangle;
	//}
	//else
	//{
	//	Angle -= 0.05;
	//	if (Angle < targetangle) Angle = targetangle;
	//}
	A->SetRotationAngle(Angle);



}

void DarknessofPlanetMainScene::WayUpdate()
{
	for (int i = 0; i < m_vecEnemyWay.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
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

void DarknessofPlanetMainScene::SlowUpdate()
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

	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetFire())
		{

			if (TIMEMANAGER->getWorldTime() > m_vecEnemy[i]->GetFireTime() + 0.3)
			{
				m_vecEnemy[i]->SetFire(false);
			}
		}
	}

}

void DarknessofPlanetMainScene::CleanHit()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;

		


		if (m_vecEnemy[i]->GetSkinnedMesh()->m_bHit)
		{
			switch (m_vecEnemy[i]->m_eName)
			{
			case Wolf:
				SOUNDMANAGER->play("WolfDmg");
				break;
			case Argo:
				SOUNDMANAGER->play("ArgoDmg");
				break;
			case Tiger:
				SOUNDMANAGER->play("TigerDmg");
				break;
			default:
				break;
			}
			if (m_vecEnemy[i]->m_eGroup == Field)
			{
				int a = 0;
			}
			m_vecEnemy[i]->m_eMode = Attack;
			if (m_vecEnemy[i]->m_eGroup == Rush)
			{
				SetRushAttack();
			}
		}
	}


	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		m_vecEnemy[i]->GetSkinnedMesh()->m_bHit = false;
	}
}

bool DarknessofPlanetMainScene::SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	if (abs(A.x - B.x) < 0.0001 &&
		abs(A.y - B.y) < 0.0001 &&
		abs(A.z - B.z) < 0.0001)
	{
		return true;
	}
	return false;

}

void DarknessofPlanetMainScene::CheckDie()
{
	int a = 0;
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		if (m_vecEnemy[i]->GetHP() <= 0)
		{
			m_vecEnemy[i]->SetDIe(true);
			m_vecEnemy[i]->SetAnimation(4);
			switch (m_vecEnemy[i]->m_eName)
			{
			case Wolf:
				SOUNDMANAGER->play("WolfDeath");
				break;
			case Argo:
				SOUNDMANAGER->play("ArgoDeath");
				break;
			case Tiger:
				SOUNDMANAGER->play("TigerDeath");
				break;
			default:
				break;
			}
		}
	}







}

float DarknessofPlanetMainScene::GetFireRate()
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

float DarknessofPlanetMainScene::GetCallbackTime()
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

void DarknessofPlanetMainScene::SetRushAttack()
{
	for (int i = 0; i < m_vecEnemy.size(); i++)
	{
		if (m_vecEnemy[i]->GetDie())continue;
		if (m_vecEnemy[i]->m_eGroup == Rush)
		{
			m_vecEnemy[i]->m_eMode = Attack;
		}
	}
}

void DarknessofPlanetMainScene::BossAttack()
{
	if (m_pBoss->m_eType != Boss_Attack)return;
	float t = D3DXVec3Length(&(m_pCharacter->GetPositionYZero() - m_pBoss->GetPositionYzero()));
	if (t < 70)
	{
		m_pBossMove->Stop();

		if (TIMEMANAGER->getWorldTime() > m_fBossTime + 10.0f && !m_pBoss->m_bAttackOn)
		{
			m_fBossTime = TIMEMANAGER->getWorldTime();
			m_pBoss->m_bAttackOn = true;
			m_pBoss->SkillOn(m_pCharacter->GetPosition());
		}
		if (!m_pBoss->m_bAttackOn)
		{
			if (m_pBoss->GetAninum() != 31)
			{
				m_pBoss->SetAnimation(31);
			}
		}
	}
	else
	{
		if (!m_pBoss->m_bAttackOn)
		{
			if (m_pBoss->GetAninum() != 33)
			{
				m_pBoss->SetAnimation(33);
			}
			if (TIMEMANAGER->getWorldTime() > m_fBossTime + 2.0f)
			{
				m_pBossMove->SetSkinnedTarget(m_pBoss->GetSkinnedMesh());
				m_pBossMove->SetSpeed(m_pBoss->m_fSpeed);
				m_pBossMove->SetFrom(m_pBoss->GetPositionYzero());
				m_pBossMove->SetTo(m_pCharacter->GetPositionYZero());
				m_pBossMove->SetRoationAngle(m_pBoss->GetRoationAngle());
				m_pBossMove->Start();
			}
		}
	}
}

void DarknessofPlanetMainScene::AfterImage()
{
	//데큐 총용량 30개로제한 
	if (m_vecAfterImageMuzzle.size() > 21) {

		m_vecAfterImageMuzzle.pop_front();
	}

	if (m_vecAfterImageWeapon.size() > 21) {

		m_vecAfterImageWeapon.pop_front();
	}

	//검광 색깔흰색지정
	D3DCOLOR c = D3DCOLOR_XRGB(0, 0, 0);

	//무기위치벡터에넣음
	m_vecAfterImageMuzzle.push_back(m_pCharacter->getMuzzlePos());
	m_vecAfterImageWeapon.push_back(m_pCharacter->getWeaponPos());


	vector<ST_PC_VERTEX> vecMuzzlePos;
	if (m_vecAfterImageMuzzle.size() >= 20) {
		for (int i = 0; i < 19; ++i) {

			D3DXVECTOR3 result;

			float mull = (i + 1) / 19.0f;
			D3DXVec3CatmullRom(&result, &m_vecAfterImageMuzzle[i], &m_vecAfterImageMuzzle[i], &m_vecAfterImageMuzzle[i + 1], &m_vecAfterImageMuzzle[i + 1], mull);

			vecMuzzlePos.push_back(ST_PC_VERTEX(result, c));
		}

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
		GETDEVICE->SetFVF(ST_PC_VERTEX::FVF);
		GETDEVICE->DrawPrimitiveUP(
			D3DPT_LINESTRIP,
			vecMuzzlePos.size() - 1,
			&vecMuzzlePos[0],
			sizeof(ST_PC_VERTEX));
	}
}

float DarknessofPlanetMainScene::ComputeGaussianValue(float x, float mean, float std_deviation)
{
	return (1.0f / sqrt(2.0f * D3DX_PI * std_deviation * std_deviation))
		* expf((-((x - mean) * (x - mean))) / (2.0f * std_deviation * std_deviation));
}

LPD3DXEFFECT DarknessofPlanetMainScene::LoadEffect(const char * szFileName)
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

LPD3DXEFFECT DarknessofPlanetMainScene::LoadEffectHpp(const char * szFileName)
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

void DarknessofPlanetMainScene::MakingFieldEnemy()
{
	TeicEnemy* pSkinnedMesh = new  TeicEnemy;
	int a = RND->getInt(100);
	int where = RND->getInt(8);
	int num = m_vecEnemy.size();
	if (a < 60)
	{
		pSkinnedMesh->Setup("object/xFile/wolf/", "wolf.X");
		pSkinnedMesh->SetPosition(GetWhere(where));
		pSkinnedMesh->SetAttack(5);
		pSkinnedMesh->SetHP(100);
		pSkinnedMesh->SetSpeed(5);
		pSkinnedMesh->m_eName = Wolf;
	}
	else if (a < 95)
	{
		pSkinnedMesh->Setup("object/xFile/tiger/", "tiger.X");
		pSkinnedMesh->SetPosition(GetWhere(where));
		pSkinnedMesh->SetAttack(8);
		pSkinnedMesh->SetHP(200);
		pSkinnedMesh->SetSpeed(10);
		pSkinnedMesh->m_eName = Tiger;
	}
	else if (a < 100)
	{
		pSkinnedMesh->Setup("object/xFile/ArgoniteGiant/", "ArgoniteGiant.X");
		pSkinnedMesh->SetPosition(GetWhere(where));
		pSkinnedMesh->SetAttack(15);
		pSkinnedMesh->SetHP(500);
		pSkinnedMesh->SetSpeed(15);
		pSkinnedMesh->SetScaleSize(0.1);
		pSkinnedMesh->m_eName = Argo;
	}
	pSkinnedMesh->SetCallbackfunction(bind(&DarknessofPlanetMainScene::CallbackOn, this, num + 10));
	pSkinnedMesh->m_eGroup = Field;
	pSkinnedMesh->SetAnimation(0);
	m_vecEnemy.push_back(pSkinnedMesh);


	m_vecEnemyWay.resize(m_vecEnemy.size());
	m_vecEnemyCollisionMove.resize(m_vecEnemy.size());
	m_vecEnemyCollisionMove[m_vecEnemy.size() - 1] = new TeicMoveSequence;


}

D3DXVECTOR3 DarknessofPlanetMainScene::GetWhere(int n)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(0, 0, 0);
	//	892.892 / 37 / -121.117
//	207.94 / 39.5 / -169.153
	//843.88 / 50.5 / -878.838
//	178.974 / 37 / -886.544
//	798.869 / 37 / -526.444
//	256.194 / 51 / -719.275
//	191.122 / 37 / -378.23
//	726.537 / 53.5535 / -792.43						
	switch (n)
	{
	case 0:
		temp = D3DXVECTOR3(892.892 + RND->getFromFloatTo(-30, 30), 0, -121.117 + RND->getFromFloatTo(-30, 30));
		break;
	case 1:
		temp = D3DXVECTOR3(207.94 + RND->getFromFloatTo(-30, 30), 0, -169.153 + RND->getFromFloatTo(-30, 30));
		break;
	case 2:
		temp = D3DXVECTOR3(843.88 + RND->getFromFloatTo(-30, 30), 0, -878.838 + RND->getFromFloatTo(-30, 30));
		break;
	case 3:
		temp = D3DXVECTOR3(178.974 + RND->getFromFloatTo(-30, 30), 0, -886.544 + RND->getFromFloatTo(-30, 30));
		break;
	case 4:
		temp = D3DXVECTOR3(798.869 + RND->getFromFloatTo(-30, 30), 0, -526.444 + RND->getFromFloatTo(-30, 30));
		break;
	case 5:
		temp = D3DXVECTOR3(256.194 + RND->getFromFloatTo(-30, 30), 0, -719.275 + RND->getFromFloatTo(-30, 30));
		break;
	case 6:
		temp = D3DXVECTOR3(191.122 + RND->getFromFloatTo(-30, 30), 0, -378.23 + RND->getFromFloatTo(-30, 30));
		break;
	case 7:
		temp = D3DXVECTOR3(726.537 + RND->getFromFloatTo(-30, 30), 0, -792.43 + RND->getFromFloatTo(-30, 30));
		break;
	default:
		break;
	}
	return temp;
}







void DarknessofPlanetMainScene::Push2(TeicEnemy * A, TeicEnemy * B)
{
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
	if (EnemyEnemyDistance(A, B) < A->m_fBoundingSize + B->m_fBoundingSize)
	{


		if (A->GetSlot() && B->GetSlot())
		{
			if (m_pCollision->CheckCollision(A->GetBoundingSquare(), B->GetBoundingSquare()) == false)
				return;
			float Adist = D3DXVec3Length(&(A->GetPositionYzero() - m_pCharacter->GetPositionYZero()));
			float Bdist = D3DXVec3Length(&(B->GetPositionYzero() - m_pCharacter->GetPositionYZero()));

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




}
