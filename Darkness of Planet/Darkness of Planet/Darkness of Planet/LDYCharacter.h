#pragma once
#include "LDYSkinnedMesh.h"
#include "LDYSkinnedMesh_Head.h"
#include "LDYSkinnedMesh_Weapon.h"
#include "LDYcJustTestScene.h"

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
	LDYSkinnedMesh_Head*		m_pHeroHead[2];

	LDYSkinnedMesh_Weapon*		m_pWeapon_AA12;
	LDYSkinnedMesh_Weapon*		m_pWeapon_AR6;
	LDYSkinnedMesh_Weapon*		m_pWeapon_M4;
	LDYSkinnedMesh_Weapon*		m_pWeapon_MP5;
	LDYSkinnedMesh_Weapon*		m_pWeapon_FireGun;

	LDYSkinnedMesh*				m_pHeroBody_MP5[4];
	LDYSkinnedMesh*				m_pHeroBody_Melee[4];
	LDYSkinnedMesh*				m_pHeroBody_Base[4];
	LDYSkinnedMesh*				m_pHeroBody_IdleBreak[4];
	LDYSkinnedMesh*				m_pHeroBody_SMG[4];


	CallbackBindFunction		m_Callback;
	CallbackBindFunction		m_AttackCallback;
	

	int							m_iHeadLv;
	int							m_iBodyLv;
	WeaponType					m_eWpType;
	StateType					m_eStType;
	bool						m_bRun;

	D3DXVECTOR3					m_afterPosMAX;
	D3DXVECTOR3					m_afterPosMIN;

public:
	TeicPhysicsCrtCtrl*			m_pCtrl;
	POINT				m_pPreviousPoint;

public:
	void Setup(char* Foldername, char* Filename);
	void CallbackOn(int n);
	void SetCallbackfunction(CallbackBindFunction function);
	void SetAttackCallbackfunction(CallbackBindFunction function);
	void UpdateAndRender();
	void MeshRender(LPD3DXEFFECT effect);
	void Render();
	void matUpdate();
	void SetAnimationIndex(int nIndex);
	void SetMove(D3DXMATRIX& move);
	void SetRotationAngle(float angle);
	void SetRotationMatrix(D3DXMATRIX rotation);
	D3DXVECTOR3* GetPositionPointer();
	D3DXVECTOR3	 GetPosition();
	void SetPosition(D3DXVECTOR3 position);
	void SetAnimation(int num);
	int GetAninum();
	void Update(float angle);
	void SetMoving(bool on);
	bool GetMoving();
	void SetAttacking(bool on);
	bool GetAttacking();
	POINT GetNodeNum();
	D3DXVECTOR3 GetPositionYZero();
	BoundingSquare * GetBoundingSquare();
	WeaponType getWeaponType() { return m_eWpType; }
	StateType getStateType() { return m_eStType; }
	//D3DXMATRIX getMatritxCrtCtrl() { return m_pCtrl->getCrtCtrlMatrix(); }

	TeicPhysicsCrtCtrl* getCrtCtrl() {return m_pCtrl;}

	// 인벤토리를 위한 임시함수..
	void SetWeaponType(WeaponType type) { m_eWpType = type; }
	void SetHeadLv(int HeadLv) { m_iHeadLv = HeadLv; }
	void SetBodyLv(int BodyLv) { m_iBodyLv = BodyLv; }

	//검광효과에 필요한 값들
	D3DXVECTOR3 getWeaponMaxPos();
	D3DXVECTOR3 getWeaponMinPos();

public:

	LDYCharacter();
	~LDYCharacter();
};

