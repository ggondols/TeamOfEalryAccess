#pragma once
#include "cSingletonBase.h"
#define KEYMAX 256
#include <bitset>
/////////////////////////////////////////////////////////
/////// 2017.09.03  ���¿� //////////////////////////////
////////// Ű �Ŵ��� ///////////////////////////////////
//////Ű �����°� �����ϴ� �Ŵ��� �Դϴ� ///////////////
////////////////////////////////////////////////////////
class cKeyManager:public cSingletonBase <cKeyManager>
{
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;
public:
	void Release();
	HRESULT Setup(void);
	void release(void);

	//�ѹ��� ��������..
	bool isOnceKeyDown(int key);

	//�ѹ��� ��������..
	bool isOnceKeyUp(int key);

	//��� ������ ������..
	bool isStayKeyDown(int key);

	//���� on ���� off ~~~~~ (���� ����ġ ����...)
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	
	cKeyManager();
	~cKeyManager();
};

