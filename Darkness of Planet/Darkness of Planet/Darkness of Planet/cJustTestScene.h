#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicSkinnedMesh.h"
#include "TeicCollisionMove.h"
class iMap;
class TeicPhysicsCrtCtrl;

class cJustTestScene : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	iMap*					m_pMap;
	TeicPhysicsCrtCtrl*		m_pCtrl;
	TeicSkinnedMesh*		m_pCharacter;

	TeicCollisionMove*      m_pCollisionMove;
	float					m_fTime;
public:
	vector<TeicSkinnedMesh*>	m_vecEnemy;
public:
	cJustTestScene();
	virtual ~cJustTestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void CallbackOn(int number);
};