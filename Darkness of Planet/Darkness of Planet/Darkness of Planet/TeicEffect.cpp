#include "stdafx.h"
#include "TeicEffect.h"




TeicEffect::TeicEffect()
{
	m_fTimeChange = 0.01;
}


TeicEffect::~TeicEffect()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pFlame);
	SAFE_RELEASE(m_pNoise);
	
}


void TeicEffect::Setup(char * Textfilename, float size, int Xnum, int Ynum)
{
	m_fSize = size;
	m_iSpriteXnum = Xnum;
	m_iSpriteYnum = Ynum;

	vector<ST_PT_VERTEX>	m_vecVertex;


	vector<D3DXVECTOR3>	vecVertex;
	vector<D3DXVECTOR2> vecTex;
	vector<DWORD>		vecIndex;
	vecVertex.push_back(D3DXVECTOR3(-m_fSize, -m_fSize, -m_fSize));
	vecVertex.push_back(D3DXVECTOR3(-m_fSize, m_fSize, -m_fSize));
	vecVertex.push_back(D3DXVECTOR3(m_fSize, m_fSize, -m_fSize));
	vecVertex.push_back(D3DXVECTOR3(m_fSize, -m_fSize, -m_fSize));
	vecVertex.push_back(D3DXVECTOR3(-m_fSize, -m_fSize, m_fSize));
	vecVertex.push_back(D3DXVECTOR3(-m_fSize, m_fSize, m_fSize));
	vecVertex.push_back(D3DXVECTOR3(m_fSize, m_fSize, m_fSize));
	vecVertex.push_back(D3DXVECTOR3(m_fSize, -m_fSize, m_fSize));

	vecTex.push_back(D3DXVECTOR2( 0, 1));
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
	//vecIndex.push_back(4);
	//vecIndex.push_back(6);
	//vecIndex.push_back(5);
	//vecIndex.push_back(4);
	//vecIndex.push_back(7);
	//vecIndex.push_back(6);
	////��
	//vecIndex.push_back(4);
	//vecIndex.push_back(5);
	//vecIndex.push_back(1);
	//vecIndex.push_back(4);
	//vecIndex.push_back(1);
	//vecIndex.push_back(0);
	////��
	//vecIndex.push_back(3);
	//vecIndex.push_back(2);
	//vecIndex.push_back(6);
	//vecIndex.push_back(3);
	//vecIndex.push_back(6);
	//vecIndex.push_back(7);
	////��
	//vecIndex.push_back(1);
	//vecIndex.push_back(5);
	//vecIndex.push_back(6);
	//vecIndex.push_back(1);
	//vecIndex.push_back(6);
	//vecIndex.push_back(2);
	////��
	//vecIndex.push_back(4);
	//vecIndex.push_back(0);
	//vecIndex.push_back(3);
	//vecIndex.push_back(4);
	//vecIndex.push_back(3);
	//vecIndex.push_back(7);

	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR2	t = vecTex[i];
		m_vecVertex.push_back(ST_PT_VERTEX(p, t));
	}


	D3DXCreateMeshFVF(vecIndex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PT_VERTEX::FVF,
		GETDEVICE,
		&m_pMesh);

	ST_PT_VERTEX* pV = 0;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = 0;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();


	vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);



	m_pEffect = LoadEffect("Fire.fx");
	m_pFlame = TEXTUREMANAGER->GetTexture("sprites/Flame.tga");
	m_pNoise = TEXTUREMANAGER->GetTexture("sprites/NoiseVolume.dds");
	hTech = 0;
	hTech = m_pEffect->GetTechniqueByName("Inferno");
	
	
}

void TeicEffect::Update()
{
}

void TeicEffect::Render()
{

//	m_pMesh->DrawSubset(0);
	m_fTimeChange += 0.01;


	D3DXMATRIXA16 matWorld,matView, matProjection;
	D3DXMatrixIdentity(&matWorld);


	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);






	// ���̴� ������������ ����


	//m_pEffect->SetTexture("base_Tex", m_pTexture);

	/*m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetMatrix("matViewProjection", &(matView*matProjection));
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProjection));*/
	m_pEffect->SetFloat("time_0_X", INF);
	m_pEffect->SetFloat("timeSampleDist", m_fTimeChange);
	m_pEffect->SetTexture("Flame_Tex", m_pFlame);
	m_pEffect->SetTexture("Noise_Tex", m_pNoise);
	m_pEffect->SetTechnique(hTech);
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
		
	}

	m_pEffect->End();

	





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