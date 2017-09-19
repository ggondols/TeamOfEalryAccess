#include "stdafx.h"
#include "TeicCharacter.h"
#include "TeicPhysicsCrtCtrl.h"


TeicCharacter::TeicCharacter()
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
	m_pCtrl = NULL;
	m_pPreviousPoint = PointMake(0, 0);
}


TeicCharacter::~TeicCharacter()
{
	SAFE_DELETE(m_pCtrl);
	SAFE_DELETE(m_pSkinnedMesh);
}

void TeicCharacter::Setup(char* Foldername, char* Filename)
{
	m_pSkinnedMesh = new TeicSkinnedMesh(Foldername, Filename);
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&TeicCharacter::CallbackOn, this, 0));
	m_pCtrl = new TeicPhysicsCrtCtrl;
	
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

D3DXVECTOR3 TeicCharacter::GetPositionYZero()
{
	if (m_pSkinnedMesh)
	{
		D3DXVECTOR3 temp = m_pSkinnedMesh->GetPosition();
		temp.y = 0;
		return temp;
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

void TeicCharacter::Update()
{
	
	if (m_pCtrl != NULL)
	{
		m_pCtrl->Update();
		SetPosition(*m_pCtrl->GetPosition());
		SetRotationAngle(m_pCtrl->getAngle());


		if (m_pCtrl->getMoving())
		{

			if (GetAninum() != 1)
			{
				SetAnimation(1);
			}
		}
		else if (m_pCtrl->getAttacking())
		{
			if (GetAninum() != 3)
			{
				SetAnimation(3);
			}
		}
		else
		{
			if (GetAninum() != 0)
			{
				SetAnimation(0);
			}
		}
	}
}

void TeicCharacter::SetMoving(bool on)
{
	m_pCtrl->setMoving(on);
}

bool TeicCharacter::GetMoving()
{
	return m_pCtrl->getMoving();
}

void TeicCharacter::SetAttacking(bool on)
{
	m_pCtrl->setAttacking(on);
}

bool TeicCharacter::GetAttacking()
{
	return m_pCtrl->getAttacking();
}

POINT TeicCharacter::GetNodeNum()
{
	POINT temp;
	if (m_pSkinnedMesh)
	{
		D3DXVECTOR3 pos =  m_pSkinnedMesh->GetPosition();
		pos.y = 0;
		temp.x = pos.x/ NodeLength;
		temp.y = -(pos.z/ NodeLength);
		return temp;
	}
	
	temp.x = 0;
	temp.y = 0;
	return temp;
}

void TeicCharacter::SetSpeed(float f)
{
	if (m_pCtrl)m_pCtrl->SetSpeed(f);   /// ±âº» 1
}

