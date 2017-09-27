#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "cModel.h"

class HankcDefferedRenderTest : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	TeicCharacter*			m_pCharacter;

	cModel*					m_Tree;
	cModel*					pNewModel;
		
public:

	HankcDefferedRenderTest();
	~HankcDefferedRenderTest();

	void CallbackOn(int num);

	

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};
