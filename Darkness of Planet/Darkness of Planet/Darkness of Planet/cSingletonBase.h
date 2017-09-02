#pragma once

template <typename T>
class cSingletonBase
{
protected:
	static T* singleton;

	cSingletonBase(void) {};
	~cSingletonBase(void) {};

public:
	//�ν��Ͻ� �����´�
	static T* getSingleton(void);

	//�ν��Ͻ� ����...
	//�޸� ����
	void releaseSingleton(void)
	{
		if (singleton)
		{
			delete singleton;
			singleton = NULL;
		}
	}
};

//�̱��� �ʱ�ȭ�Ѵ�
template <typename T>
T* cSingletonBase<T>::singleton = 0;

//�� ��������
template <typename T>
T* cSingletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;
	return singleton;
}

