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
#include "TeicObbCollision.h"
#include "TeicShoot.h"
#include "TeicIceExplosion.h"
class iMap;

class cUIObject;
class cSkyDome;
class cSkyCloud;
class Inventory;

class TeicJustTestScene:public cGameNode
{

private:
	TeicIceExplosion*				m_ptest;
	cSkyDome*						m_pSkyDome;
	cSkyCloud*						m_pSkyCloud;
	TeicShoot*				m_pShoot;
	TeicObbCollision*		m_pCollision;
	LDYCamera*				m_pCamera;
	
	TeicBresenham*			m_pBresenham;
	Hank::cGrid*			m_pGrid;
	iMap*					m_pMap;
	LPD3DXFONT				m_pFont;

	float					m_fTime;   /// ������ ����
	float					m_fTime2;  ///  ������ �Ϸ�
	float					m_fTime3;
	
	int						m_iBodyUpgrade;

	// �κ��丮 �߰�
	Inventory*				m_pInventory;
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
	TeicEnemy*  m_pTempEnemy;
	cSphere*	m_pTempSPhere;
	TeicEnemy*	m_pBoss[3];
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
	void CleanHit();
	bool SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B);  //A�� B�� ������?
public:
	TeicJustTestScene();
	~TeicJustTestScene();
};
















