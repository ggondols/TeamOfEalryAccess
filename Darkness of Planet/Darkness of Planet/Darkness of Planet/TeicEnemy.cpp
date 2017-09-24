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
	m_fBoundingSize = 2;
	m_fAttackRange = 6;
	m_bAttackOn = false;
	m_bThreadCalOn = false;
	/*m_bSlotOn = false;*/
	m_vPreviousPosition = D3DXVECTOR3(0, 0, 0);
	m_fAngle = 0.0f;
	m_fSpeed = 5.0f;
}


TeicEnemy::~TeicEnemy()
{
}


void TeicEnemy::SetSlot(bool on)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->m_bSlotOn = on;
	}
}

bool TeicEnemy::GetSlot()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->m_bSlotOn;;
	}
}

BoundingSquare * TeicEnemy::GetBoundingSquare()
{
	if (m_pSkinnedMesh)
	{
		return &m_pSkinnedMesh->m_pBoundingSquare;
	}
	return NULL;
}

void TeicEnemy::Setup(char* Foldername, char* Filename)
{
	
	m_pSkinnedMesh = new TeicSkinnedMesh(Foldername, Filename);
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&TeicEnemy::CallbackOn, this, 0));
	
	
	m_pSkinnedMesh->m_pBoundingSquare = SKINMANAGER->GetTeiBoundingSquare(Foldername, Filename);
	m_pSkinnedMesh->m_pBoundingSquare.m_pSkinnedObject = m_pSkinnedMesh;
	m_pSkinnedMesh->m_pBoundingSquare.st_Type = Bounding_Enemy;;
	m_pSkinnedMesh->m_pCopy = m_pSkinnedMesh->m_pBoundingSquare;

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
	m_fAngle = angle;
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationAngle(m_fAngle);
}

float TeicEnemy::GetRoationAngle()
{
	if (m_pSkinnedMesh)
	{
		m_fAngle = m_pSkinnedMesh->GetAngle();
		return m_fAngle;
	}
	return 0.0f;
	
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

