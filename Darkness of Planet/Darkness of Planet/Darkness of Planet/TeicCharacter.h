#pragma once
#include "TeicSkinnedMesh.h"
class TeicPhysicsCrtCtrl;
//###############################################
//#												#
//#		이동,위치,스케일,콜백이 적용된          #
//#		스킨드 메쉬 클래스를 포함하는           #
//#     캐릭터 클래스입니다.                    #
//#		2017-09-12								#	
//#     SetPosition으로 위치를					#
//#		SetRotationAngle로 회전 매트릭스 셋팅   #
//#		SetCallbackfunction로 콜백 셋팅			#	
//#												#
//#					Made by 태영				#
//###############################################
class TeicCharacter
{
private:
	TeicSkinnedMesh*	m_pSkinnedMesh;
	CallbackBindFunction m_Callback;
	CallbackBindFunction m_AttackCallback;
	TeicPhysicsCrtCtrl*		m_pCtrl;
public:
	void Setup(char* Foldername, char* Filename);
	void CallbackOn(int n);
	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move); 
	void SetRotationAngle(float angle);
	void SetRotationMatrix(D3DXMATRIX rotation); 
	D3DXVECTOR3* GetPositionPointer(); 
	D3DXVECTOR3	 GetPosition();
	void SetPosition(D3DXVECTOR3 position);
	void SetAnimation(int num);
	int GetAninum();
	void Update();
	void SetMoving(bool on);
	bool GetMoving();
	void SetAttacking(bool on);
	bool GetAttacking();
	POINT GetNodeNum();
public:

	TeicCharacter();
	~TeicCharacter();
};

