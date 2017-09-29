#include "stdafx.h"
#include "cSkyDome.h"


cSkyDome::cSkyDome()
	:m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_dMatarial(0)
	, m_fNightvalue(0.0f)
{
}


cSkyDome::~cSkyDome()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
}

void cSkyDome::Setup()
{
	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;

	string filename = "shader/Skybox/skydome.X";
	D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacency,
		&pMaterials,
		NULL,
		&m_dMatarial,
		&m_pMesh);


	m_pEffect = LoadEffect("shader/Skybox/skydome.fx");
	m_hMatWorld = m_pEffect->GetParameterByName(0, "matWorld");
	m_hMatView = m_pEffect->GetParameterByName(0, "matView");
	m_hMatProjection = m_pEffect->GetParameterByName(0, "matProjection");
	m_hTexture = m_pEffect->GetParameterByName(0, "DiffuseMap_Tex");
	m_hfNight = m_pEffect->GetParameterByName(0, "night");
	
	UIOBJECTMANAGER->AddRoot("skydometime", UITYPE_TEXT, true);
	UIOBJECTMANAGER->SetPosition("skydometime", 0.9f, 0.05f);
	UIOBJECTMANAGER->SetSize("skydometime", 100, 30);
	UIOBJECTMANAGER->SetDrawTextFormat("skydometime", DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	UIOBJECTMANAGER->SetTime("skydometime", &m_fNightvalue);
}

void cSkyDome::Update()
{
	m_fNightvalue += 0.00001f;
}

void cSkyDome::Render()
{
	D3DXMATRIXA16 matWorld, matView, matProjection;
	D3DXMatrixIdentity(&matWorld);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DXMATRIXA16 matS,matR,matT;
	D3DXMatrixScaling(&matS, 3.0f, 3.0f, 3.0f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	D3DXMatrixTranslation(&matT, 512.0f, 0.0f, -512.0f);
	matWorld = matS*matR*matT*matWorld;

	m_pEffect->SetMatrix(m_hMatWorld, &matWorld);
	m_pEffect->SetMatrix(m_hMatView, &matView);
	m_pEffect->SetMatrix(m_hMatProjection, &matProjection);
	m_pEffect->SetFloat(m_hfNight, m_fNightvalue);
	LPDIRECT3DTEXTURE9 texture;
	texture = TEXTUREMANAGER->GetTexture("shader/Skybox/skydome1.jpg");
	m_pEffect->SetTexture(m_hTexture, texture);


	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

LPD3DXEFFECT cSkyDome::LoadEffect(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// ���̴� �ε�
	LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
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
