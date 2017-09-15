#pragma once
#include "LDYSkinnedMesh.h"
#include "LDYSkinnedMesh_Head.h"
#include "LDYSkinnedMesh_Weapon.h"

class TeicPhysicsCrtCtrl;
//###############################################
//#												#
//#		2017-09-15 주인공용으로 수정  #
//#											  #
//#											 #
//#											#	
//#											#
//#										  #
//#											#	
//#											#
//#					made by LDY				#
//###############################################

class LDYCharacter
{
private:
	LDYSkinnedMesh*				m_pSkinnedMesh;

	LDYSkinnedMesh_Head*		m_pHeroHead[2];

	LDYSkinnedMesh_Weapon*		m_pWeapon_AA12;
	LDYSkinnedMesh_Weapon*		m_pWeapon_AR6;
	LDYSkinnedMesh_Weapon*		m_pWeapon_M4;
	LDYSkinnedMesh_Weapon*		m_pWeapon_MP5;
	LDYSkinnedMesh_Weapon*		m_pWeapon_Pistol;

	LDYSkinnedMesh*				m_pHeroBody_MP5;
	LDYSkinnedMesh*				m_pHeroBody_Pistol;
	LDYSkinnedMesh*				m_pHeroBody_Melee;
	LDYSkinnedMesh*				m_pHeroBody_Base;
	LDYSkinnedMesh*				m_pHeroBody_IdleBreak;
	LDYSkinnedMesh*				m_pHeroBody_SMG;


	CallbackBindFunction		m_Callback;
	CallbackBindFunction		m_AttackCallback;
	TeicPhysicsCrtCtrl*			m_pCtrl;

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

	LDYCharacter();
	~LDYCharacter();
};

