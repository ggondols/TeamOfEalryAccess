#include "stdafx.h"
#include "TeicEffect.h"






TeicEffect::TeicEffect()
{
	m_fTimeChange = 0.01;
	m_pfire_opacity = NULL;
	m_bStart = false;
}


TeicEffect::~TeicEffect()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pFlame);
	SAFE_RELEASE(m_pNoise);
	SAFE_RELEASE(m_pfire_opacity);
}


void TeicEffect::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	Setup("", 10, 1, 1);

}

bool TeicEffect::IsRunning()
{
	return m_bStart;
}
void TeicEffect::Setup(char * Textfilename, float size, int Xnum, int Ynum)
{
	
	m_fSize = size;
	m_iSpriteXnum = Xnum;
	m_iSpriteYnum = Ynum;

	vector<ST_PNT_VERTEX>	m_vecVertex;
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	vecVertex.push_back(D3DXVECTOR3(-size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, -size, -1));

	vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3(0, 0, -1));
	vecNormal.push_back(D3DXVECTOR3(0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3(0, -1, 0));

	vector<D3DXVECTOR2> vecTex;
	vecTex.push_back(D3DXVECTOR2(0, 1));
	vecTex.push_back(D3DXVECTOR2(0, 0));
	vecTex.push_back(D3DXVECTOR2(1, 0));
	vecTex.push_back(D3DXVECTOR2(0, 1));
	vecTex.push_back(D3DXVECTOR2(1, 0));
	vecTex.push_back(D3DXVECTOR2(1, 1));
	//��
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	////��
	
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR3 n = vecNormal[i / 6];
		D3DXVECTOR2	t = vecTex[i];
		m_vecVertex.push_back(ST_PNT_VERTEX(p, n,t));
	}

	//D3DXMATRIX ro;
	//D3DXMatrixRotationY(&ro, D3DX_PI/2 /**4.0 / 3.0*/);
	//for (size_t i = 0; i < vecIndex.size(); ++i)
	//{
	//	D3DXVECTOR3 p = vecVertex[vecIndex[i]];
	//	D3DXVec3TransformCoord(&p, &p, &ro);
	//	D3DXVECTOR3 n = vecNormal[i / 6];
	//	D3DXVECTOR2	t = vecTex[i];
	//	m_vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
	//}
	
	//D3DXMatrixRotationY(&ro, D3DX_PI*2.0 / 3.0);
	//for (size_t i = 0; i < vecIndex.size(); ++i)
	//{
	//	D3DXVECTOR3 p = vecVertex[vecIndex[i]];
	//	D3DXVec3TransformCoord(&p, &p, &ro);
	//	D3DXVECTOR3 n = vecNormal[i / 6];
	//	D3DXVECTOR2	t = vecTex[i];
	//	m_vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
	//}







	D3DXCreateMeshFVF(m_vecVertex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		GETDEVICE,
		&m_pMesh);

	ST_PNT_VERTEX* pV = 0;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = 0;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < m_vecVertex.size(); i++)
	{
		pI[i] = i;
	}
	
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (int i = 0; i < m_vecVertex.size()/3; i++)
	{
		if (i < vecIndex.size() / 3.0)
		{
			pA[i] = 0;
		}
		else if (i < vecIndex.size()*2.0 / 3.0)
		{
			pA[i] = 1;
		}
		else 
		{
			pA[i] = 2;
		}
	}

	m_pMesh->UnlockAttributeBuffer();


	vector<DWORD> vecAdj(m_vecVertex.size());
	m_pMesh->GenerateAdjacency(0, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);



	m_pEffect = LoadEffect("Fire2.fx");
	m_pFlame = TEXTUREMANAGER->GetTexture("sprites/Flame.tga");
	m_pNoise = TEXTUREMANAGER->GetTexture("sprites/NoiseVolume.dds");
	m_pfire_opacity = TEXTUREMANAGER->GetTexture("sprites/FireOpacity.tga");
	hTech = 0;
	hTech = m_pEffect->GetTechniqueByName("Explosion");
	
	



}

void TeicEffect::Update()
{
	if (m_bStart == true)
		m_bStart = false;
}

void TeicEffect::Render()
{
	
	if (!m_bStart) return;
	m_fTimeChange += 0.01;


	D3DXMATRIXA16 matWorld,matView, matProjection;
//	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matWorld, 100, 40, -100);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);






	// ���̴� ������������ ����


	//m_pEffect->SetTexture("base_Tex", m_pTexture);

	/*m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetMatrix("matViewProjection", &(matView*matProjection));
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProjection));*/
	m_pEffect->SetFloat("time_0_X", 2);
	m_pEffect->SetFloat("Fire_Effects_Explosion_Single_Pass_Pixel_Shader_time_0_X", m_fTimeChange);
	m_pEffect->SetFloat("timeSampleDist", m_fTimeChange);
	m_pEffect->SetTexture("Flame_Tex", m_pFlame);
	m_pEffect->SetTexture("Noise_Tex", m_pNoise);
	m_pEffect->SetTexture("fire_opacity_Tex", m_pfire_opacity);
	m_pEffect->SetMatrix("matWorldViewProjectionMatrix", &(matWorld*matView*matProjection));
	m_pEffect->SetTechnique(hTech);
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pEffect->CommitChanges();
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
	/*numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(1);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
	numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(2);
		m_pEffect->EndPass();
	}
	m_pEffect->End();*/
	
	





}

void TeicEffect::Start()
{
}

void TeicEffect::Stop()
{
}



LPD3DXEFFECT TeicEffect::LoadEffect(const char* szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// ���̴� �ε�
	LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

														//fx ���Ϸ� ���� ���̴� ��ü ����
	D3DXCreateEffectFromFile(
		GETDEVICE,				// ����̽�
		szFileName,					// �ҷ��� ���̴� �ڵ� �����̸�
		NULL,						// ���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL,						// ���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				// ���̴� ������ �÷���
		NULL,						// ���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					// �ε��� ���̴� Effect ������
		&pError						// ���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
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

void TeicEffect::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
}
