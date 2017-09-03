#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"

class cJustTestScene : public cGameNode
{
private:
	Hank::cCamera *m_pCamera;
	Hank::cGrid *m_pGrid;
public:
	cJustTestScene();
	virtual ~cJustTestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
};