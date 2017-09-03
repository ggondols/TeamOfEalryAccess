#pragma once
#include "cSingletonBase.h"
#include <time.h>
class cRandomFunction :
	public cSingletonBase<cRandomFunction>
{
public:
	cRandomFunction()
	{
		//		srand((unsigned int)time(NULL)); //1초에 한번씩 갱신
		srand((unsigned int)GetTickCount()); //20분의 1초에 한번씩 갱신..
											 //밀리쎄컨...!!!! 컴퓨터가 부팅하자마사 재기 시작하는 시간..

	}
	~cRandomFunction()
	{
	}
	void Setup() {}
	void Release() {}
	//getInt
	int getInt(int num) { return rand() % num; }
	int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}

	//숙제 1번!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//getFloat 범위 : 0.0 ~ 1.0 실수
	float getFloat(void)
	{
		return ((float)rand() / (float)RAND_MAX);
	}

	//범위 : 예 100 -> 0 ~ 100.0 실수
	float getFloat(float num)
	{
		return ((float)rand() / (float)RAND_MAX) * num;
	}

	float getFromFloatTo(float fromFloat, float toFloat)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return (rnd * (toFloat - fromFloat) + fromFloat);
	}
};

