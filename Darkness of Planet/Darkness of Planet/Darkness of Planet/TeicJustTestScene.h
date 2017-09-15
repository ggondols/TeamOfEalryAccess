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
#include "cSphere.h"
class iMap;

class cUIObject;

class TeicJustTestScene:public cGameNode
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
	vector<cSphere*>		m_vecSphere;
	LPD3DXFONT				m_pFont;
public:
	HankcGrid*				m_pNode;
	TeicCharacter*		m_pCharacter;
	TeicAstar*			m_pAstar;
	TeicAstarShort*     m_pAstarShort;
	bool					m_bThread;
	vector<TeicEnemy*>			m_vecEnemy;
	vector< vector<D3DXVECTOR3>> m_vecEnemyWay;
	vector<TeicMoveSequence*>  m_vecEnemyCollisionMove;
	D3DXVECTOR3					m_EnemyTarget;
	vector<POINT>m_vecSample;
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
	void DeleteSphere(int tilex, int tiley);
public:
	TeicJustTestScene();
	~TeicJustTestScene();
};

