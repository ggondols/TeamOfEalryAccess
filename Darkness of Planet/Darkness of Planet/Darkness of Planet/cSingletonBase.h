#pragma once

template <typename T>
class singletonBase
{
protected:
	static T* singleton;

	singletonBase(void) {};
	~singletonBase(void) {};

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
T* singletonBase<T>::singleton = 0;

//�� ��������
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;
	return singleton;
}

