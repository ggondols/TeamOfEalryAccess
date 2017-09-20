#pragma once

#include "cGameNode.h"
#include "LDYCamera.h"
#include "HankcGrid.h"

#include "TeicEnemy.h"
#include "TeicCollisionMove.h"
#include "TeicMoveSequence.h"
#include "HankcNode.h"
#include "TeicAstar.h"


enum WeaponType {

	Wp_Melee,
	Wp_AA12,
	Wp_AR6,
	Wp_M4,
	Wp_MP5

};

enum StateType {

	ST_Idle,
	ST_Attack,
	ST_Move
};


class iMap;
class LDYCharacter;
class cUIObject;
class cSkyBoxCube;

class LDYcJustTestScene :public cGameNode
{

private:
	LDYCamera*							m_pCamera;
	Hank::cGrid*						m_pGrid;

	iMap*								m_pMap;


	cUIObject*							m_pUITest;
	cUIObject*							m_pUITest2;
	LPD3DXSPRITE						m_pSprite;

	float								m_fTime;   /// 쓰레드 시작
	float								m_fTime2;  ///  쓰레드 완료
	float								m_fTime3;
public:
	LDYCharacter*						m_pCharacter;
	HankcGrid*							m_pNode;
	TeicAstar*							m_pAstar;
	bool								m_bThread;
	vector<TeicEnemy*>					m_vecEnemy;
	vector< vector<D3DXVECTOR3>>		m_vecEnemyWay;
	vector<TeicMoveSequence*>			m_vecEnemyCollisionMove;
	D3DXVECTOR3							m_EnemyTarget;

	int									m_iBodyUpgrade;
	int									m_iHeadUpgrade;
	WeaponType							m_eWeaponType;
	StateType							m_eStateType;
	cSkyBoxCube*						m_pSkyBox;

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

