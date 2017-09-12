#pragma once
#include "TeicSkinnedMesh.h"
class TeicCharacter
{
private:
	TeicSkinnedMesh*	m_pSkinnedMesh;
	CallbackBindFunction m_Callback;
	CallbackBindFunction m_AttackCallback;

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

	
	
public:

	TeicCharacter();
	~TeicCharacter();
};

