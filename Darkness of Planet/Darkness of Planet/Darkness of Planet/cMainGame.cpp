#include "stdafx.h"
#include "cMainGame.h"
#include "cJustTestScene.h"
#include "TeicJustTestScene.h"
#include "LDYcJustTestScene.h"
#include "LJHcJustTestScene.h"
#include "HankcJustTestScene.h"
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
	SCENEMANAGER->addScene("HankcJustTestScene", new HankcJustTestScene);


	//////////////////커밋 전에 항상 저스트 테스트 씬으로 바꾸세요~~
	//여기서 씬 교체
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
	TIMEMANAGER->Render();

	GETDEVICE->EndScene();
	GETDEVICE->Present(0, 0, 0, 0);

}

