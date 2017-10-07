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
	m_fBoundingSize = 5.0f;
	m_fAttackRange = 8;
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


void TeicEnemy::MakeBoundingBox()
{
	vector<ST_PN_VERTEX>	m_vecVertex;
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	float fCubeSizeX = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeX ;
	float fCubeSizeY = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeY ;
	float fCubeSizeZ = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeZ ;
	D3DXVECTOR3	center = m_pSkinnedMesh->m_pBoundingSquare.m_vCenterPos;

	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y - fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y + fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y + fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y - fCubeSizeY, center.z - fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y - fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x - fCubeSizeX, center.y + fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y + fCubeSizeY, center.z + fCubeSizeZ));
	vecVertex.push_back(D3DXVECTOR3(center.x + fCubeSizeX, center.y - fCubeSizeY, center.z + fCubeSizeZ));

	

	vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3(0, 0, -1));
	vecNormal.push_back(D3DXVECTOR3(0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3(0, -1, 0));

	//앞
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	//뒤
	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);
	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);
	//좌
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);
	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);
	//우
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);
	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);
	//상
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);
	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);
	//하
	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);
	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR3 n = vecNormal[i / 6];
		m_vecVertex.push_back(ST_PN_VERTEX(p, n));
	}
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, -center.x, -center.y, -center.z);
	D3DXMATRIX ro;
	D3DXMatrixRotationY(&ro, m_pSkinnedMesh->GetAngle());
	matWorld = trans*ro;
	D3DXMatrixTranslation(&trans, center.x, center.y, center.z);
	matWorld = matWorld* trans;
	//D3DXMatrixRotationY(&matWorld, m_pSkinnedMesh->GetAngle());
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &m_vecVertex[0], sizeof(ST_PN_VERTEX));

}

void TeicEnemy::MeshRender(LPD3DXEFFECT effect)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->ShaderMeshRender(effect);
	}

}

void TeicEnemy::SetNextAni()
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetNextAni();
	}
}

void TeicEnemy::SetScaleSize(float a)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->m_fScaleSize = a;
	}
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
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlX = 1.0f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlY = 0.3f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlZ = 0.0f;

	
	//m_fBoundingSize = CalBoundingSize();
}

void TeicEnemy::SetUpdateSpeed(float t)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetUpdateSpeed(t);

	}
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
	//MakeBoundingBox();
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

float TeicEnemy::CalBoundingSize()
{
	float A = 0;
	if (m_pSkinnedMesh)
	{
		if (A < m_pSkinnedMesh->m_pCopy.m_fSizeX * m_pSkinnedMesh->m_fScaleSize)
		{
			A = m_pSkinnedMesh->m_pCopy.m_fSizeX * m_pSkinnedMesh->m_fScaleSize;
		}
		if (A < m_pSkinnedMesh->m_pCopy.m_fSizeY * m_pSkinnedMesh->m_fScaleSize)
		{
			A = m_pSkinnedMesh->m_pCopy.m_fSizeY * m_pSkinnedMesh->m_fScaleSize;
		}
		if (A < m_pSkinnedMesh->m_pCopy.m_fSizeZ * m_pSkinnedMesh->m_fScaleSize)
		{
			A = m_pSkinnedMesh->m_pCopy.m_fSizeZ * m_pSkinnedMesh->m_fScaleSize;
		}

		return A;
	}
	return 0.0f;
}

void TeicEnemy::SetAnimationPlay(bool on)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetAnimationPlay(on);

	}
}

void TeicEnemy::SetHP(int n)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetHP(n);

	}
}



void TeicEnemy::SetAttack(int n)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetAttack(n);

	}
}

int TeicEnemy::GetHP()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetHP();

	}
}

int TeicEnemy::GetAttack()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetAttack();

	}
}
