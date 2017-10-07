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
cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{

}

HRESULT cMainGame::Setup()
{
	cGameNode::Setup(true);

	//�� �����ϰ�
	SCENEMANAGER->addScene("cJustTestScene", new cJustTestScene);
	SCENEMANAGER->addScene("TeicJustTestScene", new TeicJustTestScene);
	SCENEMANAGER->addScene("LDYcJustTestScene", new LDYcJustTestScene);
	SCENEMANAGER->addScene("LJHcJustTestScene", new LJHcJustTestScene);
	SCENEMANAGER->addScene("HankcDefferedRenderTest", new HankcDefferedRenderTest);
	SCENEMANAGER->addScene("cTestMain", new cTestMain);

	SCENEMANAGER->addScene("LoadingScene", new LoadingScene);
	
	//////////////////Ŀ�� ���� �׻� ����Ʈ �׽�Ʈ ������ �ٲټ���~~
	//���⼭ �� ��ü
	SCENEMANAGER->changeScene("HankcDefferedRenderTest");

	return S_OK;
}

void cMainGame::Release()
{
	cGameNode::Release();
}

void cMainGame::Update()
{
	cGameNode::Update();
	
	if (KEYMANAGER->isOnceKeyDown('1'))
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
	}
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

