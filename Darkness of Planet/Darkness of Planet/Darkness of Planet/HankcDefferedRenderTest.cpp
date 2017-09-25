#include "stdafx.h"
#include "HankcDefferedRenderTest.h"

HankcDefferedRenderTest::HankcDefferedRenderTest()
{
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

	m_pCharacter = new TeicCharacter;
	m_pCharacter->Setup("object/xFile/tree/", "Tree_01.X");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pCharacter->SetCallbackfunction(bind(&HankcDefferedRenderTest::CallbackOn, this, 0));
	m_pCharacter->SetSpeed(5);

	m_pCamera->Setup(m_pCharacter->GetPositionPointer());
	m_pGrid->Setup();
	//추가하면


	//디퍼드 리소스 설정하기



	return S_OK;
}

void HankcDefferedRenderTest::Release()
{
	m_pGrid->Release();
	SAFE_DELETE(m_pCamera);
	
}

void HankcDefferedRenderTest::Update()
{
	m_pCamera->Update();
	m_pCharacter->Update();
}

void HankcDefferedRenderTest::Render()
{
	m_pGrid->Render();
	m_pCharacter->UpdateAndRender();
}
