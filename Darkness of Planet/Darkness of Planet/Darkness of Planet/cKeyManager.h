#pragma once
#include "cSingletonBase.h"
#define KEYMAX 256
#include <bitset>
/////////////////////////////////////////////////////////
/////// 2017.09.03  김태영 //////////////////////////////
////////// 키 매니저 ///////////////////////////////////
//////키 누르는거 관리하는 매니저 입니다 ///////////////
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

	//한번만 눌렀을때..
	bool isOnceKeyDown(int key);

	//한번만 떼었을때..
	bool isOnceKeyUp(int key);

	//계속 누르고 있을때..
	bool isStayKeyDown(int key);

	//딸깍 on 딸깍 off ~~~~~ (전등 스위치 개념...)
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	
	cKeyManager();
	~cKeyManager();
};

