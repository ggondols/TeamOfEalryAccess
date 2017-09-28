#include "stdafx.h"
#include "TeicBoss.h"

TeicBoss::TeicBoss()
{
	m_Callback = NULL;
	m_pSkinnedMesh = NULL;
	POINT temp;
	temp.x = temp.y = 0;
	m_PresentGrid = temp;
	m_PreviousGrid = temp;
	m_fBoundingSize = 2.0f;
	m_fAttackRange = 8;
	m_bAttackOn = false;
	m_bThreadCalOn = false;
	/*m_bSlotOn = false;*/
	m_vPreviousPosition = D3DXVECTOR3(0, 0, 0);
	m_fAngle = 0.0f;
	m_fSpeed = 5.0f;
	m_ptest = NULL;
	m_eSkilltype = Skill_None;
}


TeicBoss::~TeicBoss()
{
	SAFE_DELETE(m_ptest);
}

void TeicBoss::MakeBoundingBox()
{
	vector<ST_PN_VERTEX>	m_vecVertex;
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	float fCubeSizeX = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeX;
	float fCubeSizeY = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeY;
	float fCubeSizeZ = m_pSkinnedMesh->m_pBoundingSquare.m_fSizeZ;
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

void TeicBoss::Update()
{
	//9876
	m_ptest->Update();
	if (KEYMANAGER->isOnceKeyDown('9'))
	{
		
		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(7);
		m_eSkilltype = Skill_Explosion;
	}
	if (KEYMANAGER->isOnceKeyDown('8'))
	{

		m_ptest->Start();
	}
	
}

void TeicBoss::MeshRender(LPD3DXEFFECT effect)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->ShaderMeshRender(effect);
	}

}

void TeicBoss::SetNextAni()
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetNextAni();
	}
}

void TeicBoss::SetScaleSize(float a)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->m_fScaleSize = a;
	}
}

void TeicBoss::SetSlot(bool on)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->m_bSlotOn = on;
	}
}

bool TeicBoss::GetSlot()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->m_bSlotOn;;
	}
}

BoundingSquare * TeicBoss::GetBoundingSquare()
{
	if (m_pSkinnedMesh)
	{
		return &m_pSkinnedMesh->m_pBoundingSquare;
	}
	return NULL;
}

void TeicBoss::Setup(char* Foldername, char* Filename)
{

	m_pSkinnedMesh = new TeicSkinnedMesh(Foldername, Filename);
	m_pSkinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pSkinnedMesh->SetRandomTrackPosition();
	m_pSkinnedMesh->SetCallbackfunction(bind(&TeicBoss::CallbackOn, this, 0));


	m_pSkinnedMesh->m_pBoundingSquare = SKINMANAGER->GetTeiBoundingSquare(Foldername, Filename);
	m_pSkinnedMesh->m_pBoundingSquare.m_pSkinnedObject = m_pSkinnedMesh;
	m_pSkinnedMesh->m_pBoundingSquare.st_Type = Bounding_Enemy;;
	m_pSkinnedMesh->m_pCopy = m_pSkinnedMesh->m_pBoundingSquare;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlX = 1.0f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlY = 0.3f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlZ = 0.0f;


	//m_fBoundingSize = CalBoundingSize();

	m_ptest = new TeicIceBreath;
	m_ptest->Setup(D3DXVECTOR3(150, 40, -150), D3DXVECTOR3(170,40,-170));
}

void TeicBoss::SetUpdateSpeed(float t)
{
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetUpdateSpeed(t);

	}
}

void TeicBoss::CallbackOn(int n)
{
	if (m_pSkinnedMesh)
	{
		if (m_pSkinnedMesh->GetAninum() == 9)
		{
			m_pSkinnedMesh->SetAnimation(8);
			return;
		}
		if (m_pSkinnedMesh->GetAninum() == 8)
		{
			m_pSkinnedMesh->SetAnimation(7);
			return;
		}
		if (m_pSkinnedMesh->GetAninum() == 7)
		{
			m_pSkinnedMesh->SetAnimation(6);
			return;
		}
		if (m_pSkinnedMesh->GetAninum() == 6)
		{
			m_pSkinnedMesh->SetAnimation(0);
		}
	}
	if (m_Callback)
	{
		m_Callback();
	}

}

void TeicBoss::SetCallbackfunction(CallbackBindFunction function)
{
	m_Callback = std::move(function);
}

void TeicBoss::SetAttackCallbackfunction(CallbackBindFunction function)
{
	m_AttackCallback = std::move(function);
}

void TeicBoss::UpdateAndRender()
{
	
	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->UpdateAndRender();

	}
	//MakeBoundingBox();
	m_ptest->Render();
}

void TeicBoss::SetAnimationIndex(int nIndex)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimationIndex(nIndex);
}

void TeicBoss::SetMove(D3DXMATRIX & move)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetMove(move);
}

void TeicBoss::SetRotationAngle(float angle)
{
	m_fAngle = angle;
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationAngle(m_fAngle);
}

float TeicBoss::GetRoationAngle()
{
	if (m_pSkinnedMesh)
	{
		m_fAngle = m_pSkinnedMesh->GetAngle();
		return m_fAngle;
	}
	return 0.0f;

}

void TeicBoss::SetRotationMatrix(D3DXMATRIX rotation)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetRotationMatrix(rotation);
}

D3DXVECTOR3 * TeicBoss::GetPositionPointer()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPositionPointer();
	}
	return NULL;
}

D3DXVECTOR3 TeicBoss::GetPosition()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetPosition();
	}
	return D3DXVECTOR3();
}

D3DXVECTOR3 TeicBoss::GetPositionYzero()
{
	if (m_pSkinnedMesh)
	{
		D3DXVECTOR3 temp = m_pSkinnedMesh->GetPosition();
		temp.y = 0;
		return temp;
	}
	return D3DXVECTOR3();
}

void TeicBoss::SetPosition(D3DXVECTOR3 position)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetPosition(position);
}

void TeicBoss::SetAnimation(int num)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(num);
}

int TeicBoss::GetAninum()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->GetAninum();
	}

	return 0;
}

void TeicBoss::SetCollision(bool on)
{
	if (m_pSkinnedMesh)m_pSkinnedMesh->m_bCollision = on;
}
bool TeicBoss::GetCollision()
{
	if (m_pSkinnedMesh)
	{
		return m_pSkinnedMesh->m_bCollision;
	}
	return false;
}

POINT TeicBoss::GetNodeNum()
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

float TeicBoss::CalBoundingSize()
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

