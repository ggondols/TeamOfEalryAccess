#include "stdafx.h"
#include "cJustTestScene.h"

cJustTestScene::cJustTestScene()
{
}

cJustTestScene::~cJustTestScene()
{
}

HRESULT cJustTestScene::Setup()
{
	m_pCamera = new Hank::cCamera;
	m_pCamera->Setup();
	//�� ���� �� 1�� gu        ////////////ã�ҽ��ϴ�  ���¿�
	return S_OK;
}

void cJustTestScene::Release()
{
}

void cJustTestScene::Update()
{
	m_pCamera->Update();
}

void cJustTestScene::Render()
{
	
}

