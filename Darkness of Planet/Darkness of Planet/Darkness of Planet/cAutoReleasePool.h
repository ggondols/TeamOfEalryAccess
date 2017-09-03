#pragma once
#include "cSingletonBase.h"
class cAutoReleasePool : public cSingletonBase<cAutoReleasePool>
{
public:
	cAutoReleasePool();
	~cAutoReleasePool();
};

