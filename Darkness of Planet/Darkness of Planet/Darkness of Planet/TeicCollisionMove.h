#pragma once
#include "cAction.h"

//###############################################
//#												#
//#		�̵��ϴ� ���� �浹�� �Ͼ�� ���ߴ�    #
//#		move Ŭ���� �Դϴ�.			            #
//#		2017-09-11								#	
//#     SetSkinnedTarget���� �ʱ�ȭ ���ְ�      #
//#     SetFrom,SetTo ���� Start�ϸ� �����Դϴ�	#
//#		����!! �������� 50���Ϸ� ��������       #
//#     �ȿ����Դϴ�                            #
//#		SetCallbackfunction�� �ݹ� ����			#	
//#     ��ũ�Ͻ� ���� �÷��� ������ ��Ų��޽���#
//#     ����ϱ� ������ ���� ������Ʈ�� �����ʰ�#
//#     ��Ų�� �޽��� ���� �־ ����Ͽ����ϴ�#
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
	D3DXVECTOR3 m_From;
	D3DXVECTOR3 m_To;
	SYNTHESIZE(float, m_Speed, Speed); //////////1�ʿ� �󸶳� ����?
	SYNTHESIZE(TeicSkinnedMesh*, m_pSkinnedTarget, SkinnedTarget);
	
	D3DXVECTOR3 m_previous;
	D3DXVECTOR3 m_present;
//	D3DXMATRIX  m_RotationMat;

	bool		m_bStart;

	float		m_fTotalDistance;
	float		m_fNowDistance;
	float		m_fElapsedDistance;
	CallbackBindFunction	m_Callback;
public:
	void SetCallback(CallbackBindFunction func);
	void CalRotation();
	void Stop();
	void SetFrom(D3DXVECTOR3 from);
	void SetTo(D3DXVECTOR3 to);
public:
	virtual void Start();
	virtual void Update();
public:
	TeicCollisionMove();
	virtual~TeicCollisionMove();
};
