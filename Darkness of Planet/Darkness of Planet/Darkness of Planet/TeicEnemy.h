#pragma once
#include "TeicSkinnedMesh.h"
#include "HankcNode.h"
//###############################################
//#												#
//#		이동,위치,스케일,콜백이 적용된          #
//#		스킨드 메쉬 클래스를 포함하는           #
//#     에너미 클래스입니다.                    #
//#		2017-09-12								#	
//#     SetPosition으로 위치를					#
//#		SetRotationAngle로 회전 매트릭스 셋팅   #
//#		SetCallbackfunction로 콜백 셋팅			#	
//#												#
//#					Made by 태영				#
//###############################################
enum group
{
	Field,
	Rush
};
enum Mode
{
	Idle,
	Attack
};
class TeicEnemy
{

private:
	TeicSkinnedMesh*	m_pSkinnedMesh;
	CallbackBindFunction m_Callback;
	CallbackBindFunction m_AttackCallback;
	
public:
	void MakeBoundingBox();
	POINT m_PresentGrid;
	POINT m_PreviousGrid;
	float			 m_fBoundingSize;
	float			 m_fAttackRange;
	bool			 m_bAttackOn;
	bool			 m_bThreadCalOn;  ////쓰레드로 에이스타 계산이 완료되면
	bool			 m_bAnion;
	//bool			 m_bSlotOn;
	float			 m_fAngle;
	D3DXVECTOR3		 m_vPreviousPosition;
	float			 m_fSpeed;
	group			m_eGroup;
	Mode			 m_eMode;
public:
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
	TeicSkinnedMesh* GetSkinnedMesh() {return m_pSkinnedMesh;}
	void SetCollision(bool on);
	bool GetCollision();
	POINT GetNodeNum();
	void SetSpeed(float s) { m_fSpeed = s; }
	float CalBoundingSize();
	void SetAnimationPlay(bool on);
	void SetHP(int n);
	void SetAttack(int n);
	int GetHP();
	int GetAttack();
	bool GetSlow();
	void SetSlow(bool on);
	void SetSlowTime(float t);
	float GetSlowTime();
public:
	TeicEnemy();
	~TeicEnemy();
};

