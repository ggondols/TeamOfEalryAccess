#include "stdafx.h"
#include "TeicEnemy.h"
#include "HankcNode.h"

TeicEnemy::TeicEnemy()
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
	POINT temp;
	temp.x = temp.y = 0;
	m_PresentGrid = temp;
	m_PreviousGrid = temp;
	m_fBoundingSize = 1;
	m_fAttackRange = 50;
	m_bAttackOn = false;
	m_bThreadCalOn = false;
	m_bSlotOn = false;
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
	m_BoundingBox = new BoundingSquare;
	m_BoundingBox->m_fSizeX = 1;
	m_BoundingBox->m_fSizeY = 1;
	m_BoundingBox->m_fSizeZ = 1;
	m_BoundingBox->m_vCenterPos = m_pSkinnedMesh->GetPosition();
	m_BoundingBox->st_Type = Bounding_Enemy;
	m_BoundingBox->m_pEnemy = this;
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
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->UpdateAndRender();
		m_BoundingBox->m_vCenterPos = m_pSkinnedMesh->GetPosition();
	}
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

D3DXVECTOR3 TeicEnemy::GetPositionYzero()
{
	if (m_pSkinnedMesh)
	{
		D3DXVECTOR3 temp = m_pSkinnedMesh->GetPosition();
		temp.y = 0;
		return temp;
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
		temp.x = pos.x / NodeLength;
		temp.y = -(pos.z / NodeLength);
		return temp;
	}

	temp.x = 0;
	temp.y = 0;
	return temp;
}

