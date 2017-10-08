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
	
	m_eSkilltype = Boss_Skill_None;
	
	m_bSkillCircleOn = false;
	m_pSkillCubeTexture = NULL;
	m_pSkillCubeEffect = NULL;
}


TeicBoss::~TeicBoss()
{
	
	SAFE_RELEASE(m_pSkillCubeTexture);
	SAFE_RELEASE(m_pSkillCubeEffect);
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

void TeicBoss::Update(D3DXVECTOR3	CharacterPos)
{
	//9876
	if (KEYMANAGER->isOnceKeyDown('7'))
	{
		SKILLEFFECTMANAGER->play("Blood", D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	}
	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		SKILLEFFECTMANAGER->play("MBlood", m_pSkinnedMesh->GetPosition(), D3DXVECTOR3(0, 0, 0));
	}




	if (m_pSkinnedMesh->GetAninum() == 0)
	{
		CalRotation(CharacterPos);
	}
	
	
	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		
		m_pSkinnedMesh->m_fAttacktiming = 2.4;
		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(12);

		m_eSkilltype = Boss_Skill_Blizzard;
		m_pSkillCubeTexture = TEXTUREMANAGER->GetTexture("sprites/circle2.png");
		SetSkillCube(100, 100);
		m_bSkillCircleOn = true;
		m_vCharacterPos = CharacterPos;
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		m_pSkinnedMesh->m_fAttacktiming = 1.4;
		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(6);
		
		m_eSkilltype = Boss_Skill_Explosion;
		m_pSkillCubeTexture = TEXTUREMANAGER->GetTexture("sprites/circle2.png");
		SetSkillCube(30, 30);
		m_bSkillCircleOn = true;
		m_vCharacterPos = CharacterPos;
		
	}
	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		m_pSkinnedMesh->m_fAttacktiming = 2.4;
		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(22);

		m_eSkilltype = Boss_Skill_Breath;
		m_pSkillCubeTexture = TEXTUREMANAGER->GetTexture("sprites/Rectangle.png");
		SetSkillCube(20, 60);
		m_bSkillCircleOn = true;
		m_vCharacterPos = CharacterPos;
	}
	if (KEYMANAGER->isOnceKeyDown('8'))
	{

		SKILLEFFECTMANAGER->play("Breath", D3DXVECTOR3(m_pSkinnedMesh->GetPosition()), D3DXVECTOR3(100, 40, -100));
	}



	/*if (KEYMANAGER->isOnceKeyDown('9'))
	{

		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(6);
		m_eSkilltype = Skill_Explosion;
	}
	if (KEYMANAGER->isOnceKeyDown('8'))
	{

		m_ptest->Start();
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{

		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(12);
		m_eSkilltype = Skill_Explosion;
	}
	if (KEYMANAGER->isOnceKeyDown('O'))
	{

		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(13);
		m_eSkilltype = Skill_Explosion;
	}
	if (KEYMANAGER->isOnceKeyDown('I'))
	{

		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(22);
		m_eSkilltype = Skill_Explosion;
	}*/

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
	m_pSkinnedMesh->SetAttackCallbackfunction(bind(&TeicBoss::CallbackOn, this, 1));

	m_pSkinnedMesh->m_pBoundingSquare = SKINMANAGER->GetTeiBoundingSquare(Foldername, Filename);
	m_pSkinnedMesh->m_pBoundingSquare.m_pSkinnedObject = m_pSkinnedMesh;
	m_pSkinnedMesh->m_pBoundingSquare.st_Type = Bounding_Enemy;;
	m_pSkinnedMesh->m_pCopy = m_pSkinnedMesh->m_pBoundingSquare;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlX = 1.0f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlY = 0.3f;
	m_pSkinnedMesh->m_pBoundingSquare.m_fControlZ = 0.0f;


	//m_fBoundingSize = CalBoundingSize();

	SKILLEFFECTMANAGER->AddEffect("Explosion", Skill_Explosion, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Breath", Skill_breath, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Blizzard", Skill_Blizzard, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 1);
	SKILLEFFECTMANAGER->AddEffect("Blood", Character_Blood, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 10);
	SKILLEFFECTMANAGER->AddEffect("MBlood", Monseter_Blood, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 20);
	SKILLEFFECTMANAGER->AddEffect("MChill", Monster_Chill, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 20);
	SKILLEFFECTMANAGER->AddEffect("Flame", Flame, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 30);
	SKILLEFFECTMANAGER->AddEffect("sample", sample, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 30);
	SKILLEFFECTMANAGER->play("sample", D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	m_pSkillCubeEffect = LoadEffect("sprites/SkillCircle.fx");
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
	if (n == 1)
	{
		if (m_pSkinnedMesh->GetAninum() == 6)
		{
			SKILLEFFECTMANAGER->play("Explosion", m_vCharacterPos, m_vCharacterPos);

		}
		if (m_pSkinnedMesh->GetAninum() == 12)
		{
			SKILLEFFECTMANAGER->play("Blizzard", m_pSkinnedMesh->GetPosition(), m_pSkinnedMesh->GetPosition());
		}
		if (m_pSkinnedMesh->GetAninum() == 22)
		{
			SKILLEFFECTMANAGER->play("Breath", m_pSkinnedMesh->GetPosition(), m_vCharacterPos);
		}
	}
	if (m_pSkinnedMesh)
	{
		
		 if (m_pSkinnedMesh->GetAninum() == 6)
		{
			//SkillExplosion();
			m_pSkinnedMesh->SetAnimation(0);
			m_bSkillCircleOn = false;
		}
		 if (m_pSkinnedMesh->GetAninum() == 12)
		 {
			 //SkillExplosion();
			 m_pSkinnedMesh->SetAnimation(0);
			 m_bSkillCircleOn = false;
		 }
		///////////
		else if (m_pSkinnedMesh->GetAninum() == 22)
		{
			m_pSkinnedMesh->SetAnimation(0);
			m_bSkillCircleOn = false;
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
	SKILLEFFECTMANAGER->Update();
	SKILLEFFECTMANAGER->Render();
	ShowSkillCube();
	
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

void TeicBoss::CalRotation(D3DXVECTOR3 position)
{
	D3DXVECTOR3 positionNoY = position;
	positionNoY.y = 0;
	//m_fAngle
	D3DXVECTOR3 vDirection = positionNoY -D3DXVECTOR3( m_pSkinnedMesh->GetPosition().x,0, m_pSkinnedMesh->GetPosition().z);

	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DXVECTOR3 base = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &base, &vDirection);
	float targetangle = acosf(D3DXVec3Dot(&base, &vDirection));
	if (Cross.y < 0) targetangle = D3DX_PI * 2 - targetangle;

	if (targetangle > m_fAngle)
	{
		m_fAngle += 0.05;
		if (m_fAngle > targetangle) m_fAngle = targetangle;
	}
	else
	{
		m_fAngle -= 0.05;
		if (m_fAngle < targetangle) m_fAngle = targetangle;
	}



	if (m_pSkinnedMesh)
	{
		m_pSkinnedMesh->SetRotationAngle(m_fAngle);
	}
}




void TeicBoss::ShowSkillCube()
{
	if (!m_bSkillCircleOn)return;
	D3DXMATRIX matWorld, matView, matProjection;
	switch (m_eSkilltype)
	{
	case Boss_Skill_Explosion:
	{
		D3DXMatrixTranslation(&matWorld, m_vCharacterPos.x, m_vCharacterPos.y + 0.1, m_vCharacterPos.z);
		break;
	}
	case Boss_Skill_Blizzard:
	{
		D3DXMatrixTranslation(&matWorld, m_pSkinnedMesh->GetPosition().x, m_pSkinnedMesh->GetPosition().y + 0.1, m_pSkinnedMesh->GetPosition().z);
		break;
	}
	case Boss_Skill_Breath:
	{
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, 0, 0, 60);

		/*D3DXVECTOR3 vDirection = characterpos - position;*/
		D3DXVECTOR3 vDirection =D3DXVECTOR3( m_vCharacterPos.x,0, m_vCharacterPos.z) 
			- D3DXVECTOR3(m_pSkinnedMesh->GetPosition().x,0, m_pSkinnedMesh->GetPosition().z);
		D3DXVec3Normalize(&vDirection, &vDirection);

		D3DXMATRIX matR;
		D3DXMatrixLookAtLH(&matR,
			&D3DXVECTOR3(0, 0, 0),
			&vDirection,
			&D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&matR, &matR);

		D3DXMATRIX FinalTrans;
		D3DXMatrixTranslation(&FinalTrans, m_pSkinnedMesh->GetPosition().x, m_pSkinnedMesh->GetPosition().y + 0.1, m_pSkinnedMesh->GetPosition().z);

		matWorld = trans * matR * FinalTrans;
		break;
	}
	case Boss_Skill_None:
	{
		break;
	}
	default:
	{
		break;
	}
	}
	
	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);
	matWorld = matWorld * matView * matProjection;
	m_pSkillCubeEffect->SetMatrix("matWorldViewProjection", &matWorld);
	m_pSkillCubeEffect->SetTexture("base_Tex", m_pSkillCubeTexture);
	UINT numPasses = 0;
	m_pSkillCubeEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pSkillCubeEffect->BeginPass(i);
		GETDEVICE->SetFVF(ST_PT_VERTEX::FVF);
		GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecSkillCube[0], sizeof(ST_PT_VERTEX));
		m_pSkillCubeEffect->EndPass();
	}
	m_pSkillCubeEffect->End();
}

void TeicBoss::SetSkillCube(float x, float y)
{
	m_vecSkillCube.clear();
	D3DXVECTOR3 vertex[4];
	vertex[0] = D3DXVECTOR3(-x, 0, y);
	vertex[1] = D3DXVECTOR3(x, 0, y);
	vertex[2] = D3DXVECTOR3(-x, 0, -y);
	vertex[3] = D3DXVECTOR3(x, 0, -y);


	//0----1
	//|    |
	//2----3

	DWORD Index[6];
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 2;
	Index[3] = 2;
	Index[4] = 1;
	Index[5] = 3;

	D3DXVECTOR2 t[6];
	t[0] = D3DXVECTOR2(0, 0);
	t[1] = D3DXVECTOR2(1, 0);
	t[2] = D3DXVECTOR2(0, 1);
	t[3] = D3DXVECTOR2(0, 1);
	t[4] = D3DXVECTOR2(1, 0);
	t[5] = D3DXVECTOR2(1, 1);
	for (int i = 0; i < 6; i++)
	{
		m_vecSkillCube.push_back(ST_PT_VERTEX(vertex[Index[i]], t[i]));
	}
	//m_vecSkillCircle


}


LPD3DXEFFECT TeicBoss::LoadEffect(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩

	LPD3DXBUFFER      pError = NULL;         //에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD            dwShaderFlag = 0;      //셰이더 플레그 0 

#ifdef _DEBUG

	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;      //셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif
														 //fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(

		GETDEVICE,            // 디바이스
		szFileName,               // 불러올 셰이더 코드 파일이름
		NULL,                  // 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,                  // 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,            // 셰이더 컴파일 플레그
		NULL,                  // 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,               // 로딩될 셰이더 Effect 포인터
		&pError                  // 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
	);

	//셰이더 파일로딩에문재가 있다면..
	if (pError != NULL || pEffect == NULL) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}