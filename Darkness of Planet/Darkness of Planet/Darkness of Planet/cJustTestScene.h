#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
#include "HankcNode.h"

class iMap;

class cUIObject;

class cJustTestScene : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;
	HankcGrid*				m_pNode;
	iMap*					m_pMap;
	
	TeicCharacter*		m_pCharacter;
	cUIObject*			m_pUITest;
	cUIObject*			m_pUITest2;
	LPD3DXSPRITE		m_pSprite;
	      
	float					m_fTime;
	
public:
	bool					m_bThread;
	vector<TeicEnemy*>	m_vecEnemy;
	vector<TeicCollisionMove*>  m_vecEnemyCollisionMove;
public:
	cJustTestScene();
	virtual ~cJustTestScene();

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void CallbackOn(int number);
	bool CollisionCheck(TeicEnemy* A, TeicEnemy* B);
};