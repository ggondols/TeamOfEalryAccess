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
	
	m_bSkillCircleOn = false;
	m_pSkillCubeTexture = NULL;
	m_pSkillCubeEffect = NULL;
}


TeicBoss::~TeicBoss()
{
	SAFE_DELETE(m_ptest);
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

	//��
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	//��
	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);
	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);
	//��
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);
	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);
	//��
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);
	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);
	//��
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);
	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);
	//��
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
	m_pExplosion->Update();
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		m_pSkinnedMesh->m_fAttacktiming = 1.4;
		if (m_pSkinnedMesh)m_pSkinnedMesh->SetAnimation(6);
		
		m_eSkilltype = Skill_Explosion;
		m_pSkillCubeTexture = TEXTUREMANAGER->GetTexture("sprites/circle2.png");
		SetSkillCube(30, 30);
		m_bSkillCircleOn = true;
		
	}
	if (KEYMANAGER->isOnceKeyDown('8'))
	{

		m_ptest->Start();
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

	m_ptest = new TeicIceBreath;
	m_ptest->Setup(D3DXVECTOR3(150, 40, -150), D3DXVECTOR3(130, 40, -130));
	m_pExplosion = new TeicIceExplosion;
	m_pExplosion->Setup(D3DXVECTOR3(150, 40, -150));
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
			SkillExplosion();

		}
	}
	if (m_pSkinnedMesh)
	{
		if (m_pSkinnedMesh->GetAninum() == 9)
		{
			m_pSkinnedMesh->SetAnimation(8);

		}
		else if (m_pSkinnedMesh->GetAninum() == 8)
		{
			m_pSkinnedMesh->SetAnimation(7);

		}
		else if (m_pSkinnedMesh->GetAninum() == 7)
		{
			m_pSkinnedMesh->SetAnimation(6);

		}
		else if (m_pSkinnedMesh->GetAninum() == 6)
		{
			//SkillExplosion();
			m_pSkinnedMesh->SetAnimation(0);
			m_bSkillCircleOn = false;
		}
		///////////
		else if (m_pSkinnedMesh->GetAninum() == 12)
		{
			m_pSkinnedMesh->SetAnimation(0);
		}
		else if (m_pSkinnedMesh->GetAninum() == 13)
		{
			m_pSkinnedMesh->SetAnimation(0);
		}
		////////////
		else if (m_pSkinnedMesh->GetAninum() == 22)
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
	m_pExplosion->Render();
	ShowSkillCube(D3DXVECTOR3(150, 40, -150));
	
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

void TeicBoss::SkillExplosion()
{

	m_pExplosion->Start();
}

void TeicBoss::ShowSkillCube(D3DXVECTOR3 position)
{
	if (!m_bSkillCircleOn)return;
	D3DXMATRIX matWorld, matView, matProjection;
	D3DXMatrixTranslation(&matWorld, position.x, position.y+0.01, position.z);
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

	// ���̴� �ε�

	LPD3DXBUFFER      pError = NULL;         //���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD            dwShaderFlag = 0;      //���̴� �÷��� 0 

#ifdef _DEBUG

	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;      //���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif
														 //fx ���Ϸ� ���� ���̴� ��ü ����
	D3DXCreateEffectFromFile(

		GETDEVICE,            // ����̽�
		szFileName,               // �ҷ��� ���̴� �ڵ� �����̸�
		NULL,                  // ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL,                  // ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,            // ���̴� ������ �÷���
		NULL,                  // ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,               // �ε��� ���̴� Effect ������
		&pError                  // ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
	);

	//���̴� ���Ϸε������簡 �ִٸ�..
	if (pError != NULL || pEffect == NULL) {

		//������ ������ ���� ���ڿ��� Ȯ��
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str�� ���ۿ��ִ� ������ �����Ѵ�.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//���������� ��������� �������� ����
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}