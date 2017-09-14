#pragma once

#include "cGameNode.h"
#include "HankcCamera.h"
#include "HankcGrid.h"
#include "TeicCharacter.h"
#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
#include "TeicMoveSequence.h"
#include "HankcNode.h"
#include "TeicAstar.h"
class iMap;

class cUIObject;
class LDYcJustTestScene:public cGameNode
{

private:
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	iMap*					m_pMap;


	cUIObject*			m_pUITest;
	cUIObject*			m_pUITest2;
	LPD3DXSPRITE		m_pSprite;

	float					m_fTime;   /// 쓰레드 시작
	float					m_fTime2;  ///  쓰레드 완료
	float					m_fTime3;
public:
	HankcGrid*				m_pNode;
	TeicCharacter*		m_pCharacter;
	TeicAstar*			m_pAstar;
	bool					m_bThread;
	vector<TeicEnemy*>			m_vecEnemy;
	vector< vector<D3DXVECTOR3>> m_vecEnemyWay;
	vector<TeicMoveSequence*>  m_vecEnemyCollisionMove;
	D3DXVECTOR3					m_EnemyTarget;
public:

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void CallbackOn(int number);
	bool CollisionCheck(TeicEnemy* A, TeicEnemy* B);
	void ChangeGridInfo();
	void TargetOn();
	float EnemyPlayerDistance(TeicEnemy* ene);
public:
	LDYcJustTestScene();
	~LDYcJustTestScene();
};

