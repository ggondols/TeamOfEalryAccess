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
#include "TeicBoss.h"

//��� ���
#include "cModel.h"
#include "cObjectNode.h"
#include "cStaticMeshLoader.h"
#include "cConsole.h"


class iMap;

class cUIObject;
class cSkyDome;
class cSkyCloud;
class Inventory;
class DarknessofPlanetMainScene:public cGameNode
{

private:

	cSkyDome*						m_pSkyDome;
	cSkyCloud*						m_pSkyCloud;
	TeicShoot*				m_pShoot;
	TeicObbCollision*		m_pCollision;
	//	LDYCamera*				m_pCamera;

	TeicBresenham*			m_pBresenham;
	Hank::cGrid*			m_pGrid;
	iMap*					m_pMap;
	LPD3DXFONT				m_pFont;

	float					m_fTime;   /// ������ ����
	float					m_fTime2;  ///  ������ �Ϸ�
	float					m_fTime3;
	float					m_fTime4; /// ����� �ڼ����� ���ƿ��� �ð�
	float					m_fTime5; /// �� ���� �ӵ� �ð�
	int						m_iBodyUpgrade;

	// �κ��丮 �߰�
	Inventory*				m_pInventory;

private:
	//Map Object Control
	std::list<cObjectNode*>	m_ObjNodes;
	cStaticMeshLoader		m_meshList;
	cConsole*				m_pConsole;
	

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
	TeicBoss*	m_pBoss;
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
	void SlowUpdate();
	void CleanHit();
	bool SameVector(D3DXVECTOR3 A, D3DXVECTOR3 B);  //A�� B�� ������?
	void CheckDie();
	float GetFireRate();
	float GetCallbackTime();
public:
	DarknessofPlanetMainScene();
	~DarknessofPlanetMainScene();
};
