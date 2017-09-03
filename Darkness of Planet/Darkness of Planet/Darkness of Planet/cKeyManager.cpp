#include "stdafx.h"
#include "cKeyManager.h"



cKeyManager::cKeyManager()
{
}


cKeyManager::~cKeyManager()
{
}

HRESULT cKeyManager::Setup(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
		this->getKeyDown().set(i, false);
	}
	return S_OK;
}


//�ѹ��� ��������..
bool cKeyManager::isOnceKeyDown(int key)
{
	//GetKeyState : �����͸� �˻� ����
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeyDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

//�ѹ��� ��������..
bool cKeyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}
	return false;
}

//��� ������ ������..
bool cKeyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//���� on ���� off ~~~~~ (���� ����ġ ����...)
bool cKeyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}


void cKeyManager::Release()
{
	//���� �ʿ� ���°� �����ϴ�?
}
