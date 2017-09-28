#pragma once
#include "TeicSkinnedMesh.h"
#include "HankcNode.h"
#include "TeicIceBreath.h"
#include "TeicIceExplosion.h"
//###############################################
//#												#
//#		�̵�,��ġ,������,�ݹ��� �����          #
//#		��Ų�� �޽� Ŭ������ �����ϴ�           #
//#     ���� Ŭ�����Դϴ�.					    #
//#		2017-09-28								#	
//#     SetPosition���� ��ġ��					#
//#		SetRotationAngle�� ȸ�� ��Ʈ���� ����   #
//#		SetCallbackfunction�� �ݹ� ����			#	
//#     ���� ���ϰ� �ൿ����� �߰��˴ϴ�		#
//#												#
//#					Made by �¿�				#
//###############################################

enum Boss_Skill
{
	Skill_Explosion,
	Skill_None
};
class TeicBoss
{

private:
	TeicIceBreath*				m_ptest;
	TeicIceExplosion*			m_pExplosion;
	TeicSkinnedMesh*	m_pSkinnedMesh;
	CallbackBindFunction m_Callback;
	CallbackBindFunction m_AttackCallback;

public:
	Boss_Skill			m_eSkilltype;
	void MakeBoundingBox();
	POINT m_PresentGrid;
	POINT m_PreviousGrid;
	float			 m_fBoundingSize;
	float			 m_fAttackRange;
	bool			 m_bAttackOn;
	bool			 m_bThreadCalOn;  ////������� ���̽�Ÿ ����� �Ϸ�Ǹ�
									  //bool			 m_bSlotOn;
	float			 m_fAngle;
	D3DXVECTOR3		 m_vPreviousPosition;
	float			 m_fSpeed;
public:
	void Update();
	void MeshRender(LPD3DXEFFECT effect);
	void SetNextAni();
	void SetScaleSize(float a);
	void SetSlot(bool on);
	bool GetSlot();
	BoundingSquare* GetBoundingSquare();
	void Setup(char* Foldername, char* Filename);
	void SetUpdateSpeed(float t);
	void CallbackOn(int n);
	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move);
	void SetRotationAngle(float angle);
	float	GetRoationAngle();
	void SetRotationMatrix(D3DXMATRIX rotation);
	D3DXVECTOR3* GetPositionPointer();
	D3DXVECTOR3	 GetPosition();
	D3DXVECTOR3	 GetPositionYzero();
	void SetPosition(D3DXVECTOR3 position);
	void SetAnimation(int num);
	int GetAninum();
	TeicSkinnedMesh* GetSkinnedMesh() { return m_pSkinnedMesh; }
	void SetCollision(bool on);
	bool GetCollision();
	POINT GetNodeNum();
	void SetSpeed(float s) { m_fSpeed = s; }
	float CalBoundingSize();
	void SkillExplosion();
public:
	TeicBoss();
	~TeicBoss();
};

