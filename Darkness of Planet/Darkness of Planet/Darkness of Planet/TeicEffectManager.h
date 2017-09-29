#pragma once
#include "cSingletonBase.h"
class TeicEffectManager:public cSingletonBase<TeicEffectManager>
{
public:
	void Setup();
	void Destroy();
	void Render();
	void Update();
	TeicEffectManager();
	~TeicEffectManager();
};

