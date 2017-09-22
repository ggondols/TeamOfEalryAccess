#include "stdafx.h"
#include "cSkyCloud.h"


cSkyCloud::cSkyCloud()
	:m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_dMatarial(0)
	, m_fDegree(0)
	, ftest(0.0f)
	, m_trans(0.0f)
{
}


cSkyCloud::~cSkyCloud()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
}

void cSkyCloud::Setup()
{
	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;

	string filename = "shader/Skybox/dome.X";
	D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacency,
		&pMaterials,
		NULL,
		&m_dMatarial,
		&m_pMesh);


	m_pEffect = LoadEffect("shader/Skybox/CloudMapping.fx");
	m_hMatWorld = m_pEffect->GetParameterByName(0, "matWorld");
	m_hMatView = m_pEffect->GetParameterByName(0, "matView");
	m_hMatProjection = m_pEffect->GetParameterByName(0, "matProjection");
	m_hTexture = m_pEffect->GetParameterByName(0, "NoiseMap_Tex");
	m_hFSharpness = m_pEffect->GetParameterByName(0, "gSharpness");
	m_hFTime = m_pEffect->GetParameterByName(0, "gTime");

}

void cSkyCloud::Update()
{
	ftest += 0.001f;
}

void cSkyCloud::Render()
{
	D3DXMATRIXA16 matWorld, matView, matProjection;
	D3DXMatrixIdentity(&matWorld);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matS,matRY,matRX,matT;
	D3DXMatrixScaling(&matS, 650.0f, 650.0f, 650.0f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, ftest);
	D3DXMatrixTranslation(&matT, 128.5f, -m_trans, -128.5f);
	matWorld = matS*matRY*matT*matWorld;

	//�׽�Ʈ�浹
	m_pEffect->SetMatrix(m_hMatWorld, &matWorld);
	m_pEffect->SetMatrix(m_hMatView, &matView);
	m_pEffect->SetMatrix(m_hMatProjection, &matProjection);
	m_pEffect->SetFloat(m_hFSharpness, 1.0f);
	m_pEffect->SetFloat(m_hFTime, 1.0f);
	LPDIRECT3DTEXTURE9 texture;
	texture = TEXTUREMANAGER->GetTexture("shader/Skybox/Cloud1.jpg");
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

LPD3DXEFFECT cSkyCloud::LoadEffect(const char * szFileName)
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