#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"

class HankcDefferedRenderTest : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	TeicCharacter*			m_pCharacter;
		
public:

	HankcDefferedRenderTest();
	~HankcDefferedRenderTest();

	void CallbackOn(int num);

	

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};
