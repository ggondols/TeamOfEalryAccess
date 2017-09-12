#pragma once
#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
class iMap;


class cJustTestScene : public cGameNode
{
private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	iMap*					m_pMap;
	
	TeicCharacter*		m_pCharacter;

	      
	float					m_fTime;
public:
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