#pragma once
#include "TeicEffectParent.h"
#include "TeicParticleSystem.h"
class TeicIceExplosion :public	TeicEffectParent
{



public:
	virtual void Update();
	virtual void Start();
	virtual void Render();




public:
	TeicIceExplosion();
	~TeicIceExplosion();
};

