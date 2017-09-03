#pragma once
#include "cSingletonBase.h"
class cObjectPool : public cSingletonBase<cObjectPool>
{
public:
	cObjectPool();
	~cObjectPool();
};

