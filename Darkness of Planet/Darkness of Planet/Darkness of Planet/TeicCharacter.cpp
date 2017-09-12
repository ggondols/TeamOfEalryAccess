#include "stdafx.h"
#include "TeicCharacter.h"



TeicCharacter::TeicCharacter()
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
}


TeicCharacter::~TeicCharacter()
{
	
}

void TeicCharacter::Setup(char* Foldername, char* Filename)
{
	m_pSkinnedMesh = new TeicSkinnedMesh(Foldername, Filename);
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&TeicCharacter::CallbackOn, this, 0));


}

void TeicCharacter::CallbackOn(int n)
{
	if (m_Callback)
	{
		m_Callback();
	}

}

void TeicCharacter::SetCallbackfunction(CallbackBindFunction function)
{
	m_Callback = std::move(function);
}

void TeicCharacter::SetAttackCallbackfunction(CallbackBindFunction function)
{
	m_AttackCallback = std::move(function);
}

void TeicCharacter::UpdateAndRender()
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->UpdateAndRender();
}

void TeicCharacter::SetAnimationIndex(int nIndex)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimationIndex(nIndex);
}

void TeicCharacter::SetMove(D3DXMATRIX & move)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetMove(move);
}

void TeicCharacter::SetRotationAngle(float angle)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationAngle(angle);
}

void TeicCharacter::SetRotationMatrix(D3DXMATRIX rotation)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationMatrix(rotation);
}

D3DXVECTOR3 * TeicCharacter::GetPositionPointer()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPositionPointer();
	}
	return NULL;
}

D3DXVECTOR3 TeicCharacter::GetPosition()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPosition();
	}
	return D3DXVECTOR3();
}

void TeicCharacter::SetPosition(D3DXVECTOR3 position)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetPosition(position);
}

void TeicCharacter::SetAnimation(int num)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(num);
}

int TeicCharacter::GetAninum()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetAninum();
	}

	return 0;
}

