#pragma once
#include "cAction.h"

//###############################################
//#												#
//#		�̵��ϴ� ���� �浹�� �Ͼ�� ���ߴ�    #
//#		move Ŭ���� �Դϴ�.			            #
//#		2017-09-11								#	
//#     SetPosition���� ��ġ��					#
//#		SetRotationAngle�� ȸ�� ��Ʈ���� ����   #
//#		SetCallbackfunction�� �ݹ� ����			#	
//#												#
//#					Made by �¿�				#
//###############################################

class TeicSkinnedMesh;

class TeicCollisionMove : public cAction
{

///�׼ǿ��� ��� �޾ƿ� �κ�
//protected:  
//	float m_fPassedActionTime;
//	SYNTHESIZE(float, m_fActionTime, ActionTime);
//	SYNTHESIZE(cGameObject*, m_pTarget, Target);
//	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);


private:
	SYNTHESIZE(D3DXVECTOR3, m_From, From);
	SYNTHESIZE(D3DXVECTOR3, m_To, To);
	SYNTHESIZE(float, m_Speed, Speed);
	SYNTHESIZE(TeicSkinnedMesh*, m_pSkinnedTarget, SkinnedTarget);
	D3DXVECTOR3 m_previous;
	D3DXVECTOR3 m_present;
//	D3DXMATRIX  m_RotationMat;

	bool		m_Start;
	float		m_fStarttime;
	float		m_fDistance;

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

