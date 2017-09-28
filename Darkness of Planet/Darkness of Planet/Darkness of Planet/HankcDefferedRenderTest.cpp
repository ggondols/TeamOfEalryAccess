#include "stdafx.h"
#include "HankcDefferedRenderTest.h"
#include "iMap.h"
#include "cHeightMap.h"


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


	HEIGHTMAPMANAGER->AddHeightMap("terrain", "map/", "final5.raw", "final5.png", (DWORD)1);
	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");
	m_pNode = NODEMANAGER->GetNode();
	//여기부터
	//	m_pMap->GetHeight(x,y,z) 헤이트맵 수정

	m_pCamera->Setup();
	m_pGrid->Setup();


	m_meshList.ScriptLoader("Data/Script/ObjectList.txt", m_ObjNodes);


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
	m_pCamera->Update();
	//m_pCharacter->Update();
}

void HankcDefferedRenderTest::Render()
{
	m_pGrid->Render();
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	GETDEVICE->SetTransform(D3DTS_WORLD, &I);
	if (m_pMap)m_pMap->frustumcullingRender();
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
