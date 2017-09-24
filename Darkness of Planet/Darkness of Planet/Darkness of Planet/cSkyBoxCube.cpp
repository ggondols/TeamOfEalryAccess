#include "stdafx.h"
#include "cSkyBoxCube.h"
#include "LDYCamera.h"


cSkyBoxCube::cSkyBoxCube()
	:m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_dMatarial(0)
	, m_pCubeTexture(NULL)
{
}


cSkyBoxCube::~cSkyBoxCube()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pCubeTexture);

}

void cSkyBoxCube::Setup()
{
	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;

	string filename = "shader/Skybox/Cube.X";
	D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacency,
		&pMaterials,
		NULL,
		&m_dMatarial,
		&m_pMesh);

	D3DXCreateCubeTextureFromFile(
		GETDEVICE,
		"shader/Skybox/Texture2.dds",
		&m_pCubeTexture);

	m_pEffect = LoadEffect("shader/Skybox/Skybox.fx");
	m_hViewProjection = m_pEffect->GetParameterByName(0, "matViewProjection");
	m_hTexture = m_pEffect->GetParameterByName(0, "myCubemap_Tex");
	m_hvViewPosition = m_pEffect->GetParameterByName(0, "vViewPosition");

}

void cSkyBoxCube::Update()
{
}

void cSkyBoxCube::Render(LDYCamera * pCamera)
{
	D3DXMATRIXA16 matWorld, matView, matProjection, matViewProjection;
	D3DXMatrixIdentity(&matWorld);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);
	matViewProjection = matS*matView*matProjection;
	m_pEffect->SetMatrix(m_hViewProjection, &matViewProjection);
	m_pEffect->SetVector(m_hvViewPosition, &D3DXVECTOR4(pCamera->getEye(), 1));
	m_pEffect->SetTexture(m_hTexture, m_pCubeTexture);


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

LPD3DXEFFECT cSkyBoxCube::LoadEffect(const char * szFileName)
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
