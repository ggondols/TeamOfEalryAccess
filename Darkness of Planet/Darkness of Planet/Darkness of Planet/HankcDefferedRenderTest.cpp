#include "stdafx.h"
#include "HankcDefferedRenderTest.h"
#include "iMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cHeightMap.h"
#include "cCrtCtrl.h"


HankcDefferedRenderTest::HankcDefferedRenderTest()
{
	m_ObjNodes.clear();
}

HankcDefferedRenderTest::~HankcDefferedRenderTest()
{
}

void HankcDefferedRenderTest::CallbackOn(int num)
{

}

HRESULT HankcDefferedRenderTest::Setup()
{
	m_pCamera = new Hank::cCamera;
	m_pGrid = new Hank::cGrid;
	m_pPhysicsCrtCtrl = new TeicPhysicsCrtCtrl;
	m_DummyCtrl = new cCrtCtrl;

	m_ViewCamera = D3DXVECTOR3(0, 0, 0);

	m_pConsole = new cConsole;
	m_pConsole->Setup();
	
	//m_pConsole->addValues("MNA", "int", &m_mna); //사용 예시

	HEIGHTMAPMANAGER->AddHeightMap("terrain", "map/", "final5.raw", "final5.png", (DWORD)1);
	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");
	m_pNode = NODEMANAGER->GetNode();
	//여기부터


	m_pCamera->Setup(m_DummyCtrl->GetPosition());
	m_pGrid->Setup();


	m_meshList.ScriptLoader("Data/Script/ObjectList.txt", m_ObjNodes);

	for (auto i = m_ObjNodes.begin(); i != m_ObjNodes.end(); ++i)
	{
		cObjectNode* pObj = *i;
		float y = pObj->GetPosition().y;
		m_pMap->GetHeight(pObj->GetPosition().x, y, pObj->GetPosition().z);
		pObj->SetPosition(D3DXVECTOR3(pObj->GetPosition().x, y, pObj->GetPosition().z));
	}

	m_DummyCtrl->setSpeed(10);
	//m_pMap->GetHeight(x, y, z) //헤이트맵 위치 기반 Y값 업데이트
	//디퍼드 리소스 설정하기

	return S_OK;
}

void HankcDefferedRenderTest::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pMap);
}

void HankcDefferedRenderTest::Update()
{
	//m_pPhysicsCrtCtrl->Update(m_pCamera->getAngleY());
	//float y = m_DummyCtrl->GetPosition()->y;
	//m_pMap->GetHeight(m_DummyCtrl->GetPosition()->x, y, m_DummyCtrl->GetPosition()->z);
	//m_DummyCtrl->setPosY(y);

	D3DXVECTOR3 viewPos;
	viewPos.x = m_pCamera->getDir().x;
	viewPos.y = m_pCamera->getDir().y;
	viewPos.z = m_pCamera->getDir().z;
	D3DXVec3Normalize(&viewPos, &viewPos);

	m_pCamera->Update();
	m_DummyCtrl->UpdateByDir(viewPos);

	m_pConsole->Update();

	
	/*if (m_mna != 0)
	{
		int a = 0;
	}*/

	//m_pCharacter->Update();

	UIOBJECTMANAGER->Update();
}

void HankcDefferedRenderTest::Render()
{
	m_pGrid->Render();
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	GETDEVICE->SetTransform(D3DTS_WORLD, &I);
	if (m_pMap)m_pMap->frustumcullingRender();

	m_pConsole->Render();
	//m_pCharacter->UpdateAndRender();

	//렌더링 예시
	/*D3DXMATRIX matWRD,matT, matS;
	D3DXMatrixScaling(&matS, 0.1, 0.1, 0.1);
	D3DXMatrixTranslation(&matT, 1, 10, 1);
	matWRD = matT * matS;
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWRD);
	pNewModel->Render(GETDEVICE);*/



	for (std::list<cObjectNode*>::iterator i = m_ObjNodes.begin(); i != m_ObjNodes.end(); ++i)
	{
		cObjectNode* pNode = *i;

		//if (!pNode->m_bVisible)
		//	continue;


		D3DXMATRIX matWorld;
		pNode->GetWorldMatrix(&matWorld);

		/*
		//노말맵, 그림자 조명
		//// new stuff for normal mapping
		//D3DXMATRIX matWorldInverse;
		//D3DXMatrixInverse(&matWorldInverse, NULL, &matWorld);

		//D3DXVECTOR3 objectSpaceLightDir;
		//D3DXVec3TransformNormal(&objectSpaceLightDir, &worldSpaceLightDirection, &matWorldInverse);
		//V(pForward->SetFloatArray(hObjectSpaceLightDirection, (float*)&objectSpaceLightDir, 3));

		//D3DXVECTOR3 objectSpaceEyePosition;
		//D3DXVECTOR3 eyePos = pCamera->GetPosition();
		//D3DXVec3TransformCoord(&objectSpaceEyePosition, &eyePos, &matWorldInverse);
		//V(pForward->SetFloatArray(hObjectSpaceEyePosition, (float*)&objectSpaceEyePosition, 3));


		//D3DXMATRIX matWVP = matWorld * matView * matProj;
		//V(pForward->SetMatrix(hWVP, &matWVP));

		//if (pLight->m_bCastsShadows)
		//{
		//	// now set the matrix that allows the lighting shader to move from object to lightViewProj space
		//	D3DXMATRIX matObjectToLightProjSpace;
		//	matObjectToLightProjSpace = matWorld * matLightViewProj * m_matTexScaleBias;
		//	V(pForward->SetMatrix(hMatObjectToLightProjSpace, &matObjectToLightProjSpace));
		//}
		*/

		GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

		pNode->m_pModel->Render(GETDEVICE);
	}

}
