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
#include "TeicAstarShort.h"
#include "TeicBresenham.h"
#include "cSphere.h"

class iMap;

class cUIObject;

class TeicJustTestScene:public cGameNode
{

private:
	
	TeicBresenham*			m_pBresenham;
	Hank::cCamera*			m_pCamera;
	Hank::cGrid*			m_pGrid;

	iMap*					m_pMap;


	cUIObject*			m_pUITest;
	cUIObject*			m_pUITest2;
	LPD3DXSPRITE		m_pSprite;

	float					m_fTime;   /// ������ ����
	float					m_fTime2;  ///  ������ �Ϸ�
	float					m_fTime3;
	
	
public:
	HankcGrid*				m_pNode;
	TeicCharacter*		m_pCharacter;
	TeicAstar*			m_pAstar;
	TeicAstarShort*     m_pAstarShort;
	bool					m_bThread;
	bool					m_bAstarThread;
	bool					m_bAttackOn;
	vector<TeicEnemy*>			m_vecEnemy;
	vector< vector<D3DXVECTOR3>> m_vecEnemyWay;
	vector<TeicMoveSequence*>  m_vecEnemyCollisionMove;
	D3DXVECTOR3				m_EnemyTarget;
	vector<POINT>				m_vecBresnhamNode;
	vector<bool>                m_vecAttackSlot;
	POINT						m_pAttackNode;
public:

	virtual HRESULT Setup();
	virtual void Release();
	virtual void Render();
	virtual void Update();
	void CallbackOn(int number);
	bool CollisionCheck(TeicEnemy* A, TeicEnemy* B);
	void Push2(TeicEnemy* A, TeicEnemy* B);
	void TotalPushCheck2();
	void ChangeGridInfo();
	void TargetOn();
	bool CheckStraight(TeicEnemy* A);
	float EnemyPlayerDistance(TeicEnemy* ene);
	bool TotalCollisionCheck();
	bool SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B);
	bool CheckSlot();
	bool ChangeCheckPoint();
	
public:
	TeicJustTestScene();
	~TeicJustTestScene();
};

