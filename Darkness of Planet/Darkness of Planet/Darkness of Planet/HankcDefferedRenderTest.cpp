#include "stdafx.h"
#include "HankcDefferedRenderTest.h"
#include "iMap.h"
#include "TeicPhysicsCrtCtrl.h"
#include "cHeightMap.h"
#include "cCrtCtrl.h"
#include <fstream>
#include <sstream>
#include "cSoundLoader.h"


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
	//���콺 ����
	SetCapture(g_hWnd);

	//scene render state 
	
	m_pCamera = new Hank::cCamera;
	m_pGrid = new Hank::cGrid;
	m_pPhysicsCrtCtrl = new TeicPhysicsCrtCtrl;
	m_DummyCtrl = new cCrtCtrl;
	m_pMeshSpray = new cSprayMesh;

	m_ViewCamera = D3DXVECTOR3(0, 0, 0);

	m_pConsole = new cConsole;
	m_pConsole->Setup();

	m_mna = 0;
	
	m_pConsole->addValues("MNA", "int", &m_mna); //��� ����

	HEIGHTMAPMANAGER->AddHeightMap("terrain", "map/", "final5.raw", "final5.png", (DWORD)1);
	m_pMap = HEIGHTMAPMANAGER->GetHeightMap("terrain");
	m_pNode = NODEMANAGER->GetNode();
	//�������
	
	cSoundLoader ls;
	ls.LoadSound();


	
	/*SOUNDMANAGER->addSound(soundKey, soundName, false, false);
	SOUNDMANAGER->GetSoundNamebyTrack(0);*/

	//for(int i = 0; i < )


	m_pCamera->Setup(m_DummyCtrl->GetPosition());
	m_pGrid->Setup();

	// ��ũ��Ʈ�� ���ǵ� ��� �޽� ���ϰ� ��ġ ������ ����
	// obj�� ��ġ ������ ���� ����Ʈ
	// obj �� ���� model Ŭ���� �ȿ� �޽� ���� �����͸� ����
	m_meshList.ScriptLoader("Data/Script/ObjectList.txt", m_ObjNodes);
	m_pMeshSpray->MeshLoadFromScript("Data/Script/ObjectList.txt");
	m_pMeshSpray->SetTargetNode(NODEMANAGER->GetPickingNode()->GetPosition());

	for each(auto p in m_ObjNodes)
	{
		p->PutBoundBoxtoNodeByPosition(m_pNode);
	}

	/*for (auto i = m_ObjNodes.begin(); i != m_ObjNodes.end(); ++i)
	{
		cObjectNode* pObj = *i;
		float y = pObj->GetPosition().y;
		m_pMap->GetHeight(pObj->GetPosition().x, y, pObj->GetPosition().z);
		pObj->SetPosition(D3DXVECTOR3(pObj->GetPosition().x, y, pObj->GetPosition().z));
	}*/

	m_DummyCtrl->setSpeed(10);
	//m_pMap->GetHeight(x, y, z) //����Ʈ�� ��ġ ��� Y�� ������Ʈ
	//���۵� ���ҽ� �����ϱ�

	return S_OK;
}

void HankcDefferedRenderTest::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pMap);
	SAFE_RELEASE(m_pMeshSpray);
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

	//m_pCharacter->Update();

	UIOBJECTMANAGER->Update();

	m_pMeshSpray->Update();



	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		SOUNDMANAGER->play(SOUNDMANAGER->GetSoundNamebyTrack(0));
		string tt = SOUNDMANAGER->GetSoundNamebyTrack(0);
;	}
	else if (KEYMANAGER->isStayKeyDown('N'))
	{

	}
	else if (KEYMANAGER->isStayKeyDown('M'))
	{

	}

	SOUNDMANAGER->Update();
}

void HankcDefferedRenderTest::Render()
{
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGrid->Render();
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	GETDEVICE->SetTransform(D3DTS_WORLD, &I);

	if (m_pMap)m_pMap->frustumcullingRender();

	m_pConsole->Render();
	//m_pCharacter->UpdateAndRender();

	//������ ����
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
		//�븻��, �׸��� ����
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

	m_pMeshSpray->Render();
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}
