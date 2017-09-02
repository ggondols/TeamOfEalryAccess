#pragma once

template <typename T>
class singletonBase
{
protected:
	static T* singleton;

	singletonBase(void) {};
	~singletonBase(void) {};

public:
	//인스턴스 가져온다
	static T* getSingleton(void);

	//인스턴스 해제...
	//메모리 해제
	void releaseSingleton(void)
	{
		if (singleton)
		{
			delete singleton;
			singleton = NULL;
		}
	}
};

//싱글톤 초기화한다
template <typename T>
T* singletonBase<T>::singleton = 0;

//값 가져오기
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;
	return singleton;
}

