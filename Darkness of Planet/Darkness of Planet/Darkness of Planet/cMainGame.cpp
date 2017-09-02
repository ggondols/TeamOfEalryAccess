#include "stdafx.h"
#include "cMainGame.h"
#include "cJustTestScene.h"


cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{

}

HRESULT cMainGame::Setup()
{
	cGameNode::Setup(true);

	//¾À »ı¼ºÇÏ°í
	SCENEMANAGER->addScene("cJustTestScene.h", new cJustTestScene);

	//¿©±â¼­ ¾À ±³Ã¼
	SCENEMANAGER->changeScene("cJustTestScene");

	return S_OK;
}

void cMainGame::Release()
{
	cGameNode::Release();
}

void cMainGame::Update()
{
	cGameNode::Update();

	SCENEMANAGER->Update();
	//AUTORELEASEPOOL->Drain();
}

void cMainGame::Render()
{
	GETDEVICE->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(47, 121, 112), 1.0f, 1);
	GETDEVICE->BeginScene();

	SCENEMANAGER->Render();
	//TIMEMANAGER->Render();

	GETDEVICE->EndScene();
	GETDEVICE->Present(0, 0, 0, 0);

}

