#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "HankcNode.h"
#include "TeicSkinnedMesh.h"
class iMap;
class TeicPhysicsCrtCtrl;

class cJustTestScene : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	iMap*					m_pMap;
	TeicPhysicsCrtCtrl*		m_pCtrl;
	TeicSkinnedMesh*		m_pSkinnedMesh;

	HankcGrid				m_pNodeGrid;

	
public:
	cJustTestScene();
	virtual ~cJustTestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void CallbackOn(int n);
};