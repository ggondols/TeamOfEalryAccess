#include "stdafx.h"
#include "cMainGame.h"
#include "cJustTestScene.h"
#include "TeicJustTestScene.h"
#include "LDYcJustTestScene.h"
#include "LJHcJustTestScene.h"
#include "HankcJustTestScene.h"
#include "cTestMain.h"
#include "LoadingScene.h"
#include "HankcDefferedRenderTest.h"
#include "DarknessofPlanetMainScene.h"
#include "StartScene.h"
#include "EndingScene.h"
cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{

}

HRESULT cMainGame::Setup()
{
	cGameNode::Setup(true);

	//씬 생성하고
	SCENEMANAGER->addScene("cJustTestScene", new cJustTestScene);
	SCENEMANAGER->addScene("TeicJustTestScene", new TeicJustTestScene);
	SCENEMANAGER->addScene("LDYcJustTestScene", new LDYcJustTestScene);
	SCENEMANAGER->addScene("LJHcJustTestScene", new LJHcJustTestScene);
	SCENEMANAGER->addScene("HankcDefferedRederTest", new HankcDefferedRenderTest);
	SCENEMANAGER->addScene("cTestMain", new cTestMain);
	SCENEMANAGER->addScene("DarknessofPlanetMainScene", new DarknessofPlanetMainScene);
	
	SCENEMANAGER->addScene("LoadingScene", new LoadingScene);
	SCENEMANAGER->addScene("StartScene", new StartScene);
	SCENEMANAGER->addScene("EndingScene", new EndingScene);
	//////////////////커밋 전에 항상 저스트 테스트 씬으로 바꾸세요~~
	//여기서 씬 교체
	SCENEMANAGER->changeScene("LoadingScene");
	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GETDEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	GETDEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	return S_OK;
}

void cMainGame::Release()
{
	cGameNode::Release();
}

void cMainGame::Update()
{
	cGameNode::Update();
	
	if (KEYMANAGER->isToggleKey('0'))
	{
		TIMEMANAGER->SetShowFrame(true);
	}
	else
	{
		TIMEMANAGER->SetShowFrame(false);
	}
	/*if (KEYMANAGER->isOnceKeyDown('1'))
	{
		SCENEMANAGER->changeScene("HankcDefferedRenderTest");
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		SCENEMANAGER->changeScene("LDYcJustTestScene");
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		SCENEMANAGER->changeScene("LJHcJustTestScene");
	}
	if (KEYMANAGER->isOnceKeyDown('4'))
	{
		SCENEMANAGER->changeScene("TeicJustTestScene");
	}*/
	SCENEMANAGER->Update();
	//AUTORELEASEPOOL->Drain();
}

void cMainGame::Render()
{
	GETDEVICE->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(47, 121, 112), 1.0f, 1);
	GETDEVICE->BeginScene();

	SCENEMANAGER->Render();
	TIMEMANAGER->Render();

	GETDEVICE->EndScene();
	GETDEVICE->Present(0, 0, 0, 0);

}

