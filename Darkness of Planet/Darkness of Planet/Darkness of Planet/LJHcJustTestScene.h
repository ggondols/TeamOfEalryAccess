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
#include "LDYCharacter.h"
#include "cSkyBoxCube.h"
#include "LDYCamera.h"

class iMap;
class cUIObject;
class Inventory;
class SkyDome;
class FieldItem;

class LJHcJustTestScene:public cGameNode
{

private:
	LDYCamera*				m_pCamera;
	//cSkyBoxCube*			m_pSkyBox;
	TeicBresenham*			m_pBresenham;
	Hank::cGrid*			m_pGrid;
	iMap*					m_pMap;

	Inventory*				m_pInventory;

	float					m_fTime;   /// 쓰레드 시작
	float					m_fTime2;  ///  쓰레드 완료
	float					m_fTime3;

	int						m_iBodyUpgrade;

	cSkyDome*				m_pSkyDome;
	FieldItem*				m_pItem;
public:
	HankcGrid*				m_pNode;
	LDYCharacter*			m_pCharacter;
	TeicAstar*				m_pAstar;
	TeicAstarShort*		    m_pAstarShort;
	bool					m_bThread;
	bool					m_bAstarThread;
	bool					m_bAttackOn;
	vector<TeicEnemy*>				m_vecEnemy;
	vector< vector<D3DXVECTOR3>>	m_vecEnemyWay;
	vector<TeicMoveSequence*>		m_vecEnemyCollisionMove;
	vector<POINT>					m_vecBresnhamNode;
	vector<bool>					m_vecAttackSlot;
	POINT							m_pAttackNode;
	D3DXVECTOR3						m_EnemyTarget;
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
	float EnemyEnemyDistance(TeicEnemy* A, TeicEnemy* B);
	float EnemyPlayerDistance(TeicEnemy* ene);
	bool TotalCollisionCheck();
	bool CheckSlot();
	bool ChangeCheckPoint();
	void AngleChange(TeicEnemy* A);
	void WayUpdate();
public:
	LJHcJustTestScene();
	~LJHcJustTestScene();
};

