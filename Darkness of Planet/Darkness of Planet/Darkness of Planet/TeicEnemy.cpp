#include "stdafx.h"
#include "TeicEnemy.h"


TeicEnemy::TeicEnemy()
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;

}


TeicEnemy::~TeicEnemy()
{
}


void TeicEnemy::Setup(char* Foldername, char* Filename)
{
	m_pSkinnedMesh = new TeicSkinnedMesh(Foldername, Filename);
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&TeicEnemy::CallbackOn, this, 0));


}

void TeicEnemy::CallbackOn(int n)
{
	if (m_Callback)
	{
		m_Callback();
	}

}

void TeicEnemy::SetCallbackfunction(CallbackBindFunction function)
{
	m_Callback = std::move(function);
}

void TeicEnemy::SetAttackCallbackfunction(CallbackBindFunction function)
{
	m_AttackCallback = std::move(function);
}

void TeicEnemy::UpdateAndRender()
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->UpdateAndRender();
}

void TeicEnemy::SetAnimationIndex(int nIndex)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimationIndex(nIndex);
}

void TeicEnemy::SetMove(D3DXMATRIX & move)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetMove(move);
}

void TeicEnemy::SetRotationAngle(float angle)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationAngle(angle);
}

void TeicEnemy::SetRotationMatrix(D3DXMATRIX rotation)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationMatrix(rotation);
}

D3DXVECTOR3 * TeicEnemy::GetPositionPointer()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPositionPointer();
	}
	return NULL;
}

D3DXVECTOR3 TeicEnemy::GetPosition()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPosition();
	}
	return D3DXVECTOR3();
}

void TeicEnemy::SetPosition(D3DXVECTOR3 position)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetPosition(position);
}

void TeicEnemy::SetAnimation(int num)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(num);
}

int TeicEnemy::GetAninum()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetAninum();
	}

	return 0;
}

void TeicEnemy::SetCollision(bool on)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->m_bCollision = on;
}
bool TeicEnemy::GetCollision()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->m_bCollision;
	}
	return false;
}

POINT TeicEnemy::GetNodeNum()
{
	POINT temp;
	if (m_pSkinnedMesh)
	{
		D3DXVECTOR3 pos = m_pSkinnedMesh->GetPosition();
		pos.y = 0;
		temp.x = pos.x;
		temp.y = -(pos.z);
		return temp;
	}

	temp.x = 0;
	temp.y = 0;
	return temp;
}

