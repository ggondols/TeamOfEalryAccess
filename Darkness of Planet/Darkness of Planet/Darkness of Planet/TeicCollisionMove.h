#pragma once
#include "cAction.h"

//###############################################
//#												#
//#		이동하는 도중 충돌이 일어나면 멈추는    #
//#		move 클래스 입니다.			            #
//#		2017-09-11								#	
//#     SetPosition으로 위치를					#
//#		SetRotationAngle로 회전 매트릭스 셋팅   #
//#		SetCallbackfunction로 콜백 셋팅			#	
//#												#
//#					Made by 태영				#
//###############################################

class TeicSkinnedMesh;

class TeicCollisionMove : public cAction
{

///액션에서 상속 받아온 부분
//protected:  
//	float m_fPassedActionTime;
//	SYNTHESIZE(float, m_fActionTime, ActionTime);
//	SYNTHESIZE(cGameObject*, m_pTarget, Target);
//	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);


private:
	SYNTHESIZE(D3DXVECTOR3, m_From, From);
	SYNTHESIZE(D3DXVECTOR3, m_To, To);
	SYNTHESIZE(float, m_Speed, Speed); //////////1초에 얼마나 가냐?
	SYNTHESIZE(TeicSkinnedMesh*, m_pSkinnedTarget, SkinnedTarget);
	D3DXVECTOR3 m_previous;
	D3DXVECTOR3 m_present;
//	D3DXMATRIX  m_RotationMat;

	bool		m_Start;

	float		m_fTotalDistance;
	float		m_fNowDistance;
	float		m_fElapsedDistance;
	CallbackBindFunction	m_Callback;
public:
	void SetCallback(CallbackBindFunction func);
	void CalRotation();
	void Stop();
public:
	virtual void Start();
	virtual void Update();
public:
	TeicCollisionMove();
	virtual~TeicCollisionMove();
};

