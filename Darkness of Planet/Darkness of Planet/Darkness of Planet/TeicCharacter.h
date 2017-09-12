#pragma once
#include "TeicSkinnedMesh.h"
class TeicPhysicsCrtCtrl;
//###############################################
//#												#
//#		�̵�,��ġ,������,�ݹ��� �����          #
//#		��Ų�� �޽� Ŭ������ �����ϴ�           #
//#     ĳ���� Ŭ�����Դϴ�.                    #
//#		2017-09-12								#	
//#     SetPosition���� ��ġ��					#
//#		SetRotationAngle�� ȸ�� ��Ʈ���� ����   #
//#		SetCallbackfunction�� �ݹ� ����			#	
//#												#
//#					Made by �¿�				#
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

