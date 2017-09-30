#pragma once
#include "TeicEffectParent.h"
class TeicCharacterBlood:public TeicEffectParent
{
public:
	virtual void Update();
		virtual void Start();
		virtual void Render();
		virtual void Stop() ;
		virtual void SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
		virtual void Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos);
		virtual bool IsRunning();
public:
public:
	TeicCharacterBlood();
	~TeicCharacterBlood();
};

