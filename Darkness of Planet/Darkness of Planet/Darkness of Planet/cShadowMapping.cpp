#include "stdafx.h"
#include "cShadowMapping.h"


cShadowMapping::cShadowMapping()
	:m_pCreateShadow(NULL)
	, m_pApplyShadow(NULL)
	, m_pDisc(NULL)
	, m_pTorus(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_pShadowDepthStencil(NULL)
	, gRotationY(0.0f)
{
	D3DXMatrixIdentity(&matWorld);
}


cShadowMapping::~cShadowMapping()
{
	SAFE_RELEASE(m_pDisc);
	SAFE_RELEASE(m_pTorus);
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pShadowDepthStencil);
	SAFE_RELEASE(m_pApplyShadow);
	SAFE_RELEASE(m_pCreateShadow);

}

void cShadowMapping::Setup()
{
	const int shadowMapSize = 2048;
	if (FAILED(GETDEVICE->CreateTexture(shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL)))
	{
		int a = 0;
	}

	// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
	if (FAILED(GETDEVICE->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
		int a = 0;
	}

	LPD3DXBUFFER pAdjacencyTorus;
	LPD3DXBUFFER pMaterialsTorus;

	string filenameTorus = "shader/shadow/Torus.X";
	D3DXLoadMeshFromX(filenameTorus.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacencyTorus,
		&pMaterialsTorus,
		NULL,
		NULL,
		&m_pTorus);

	LPD3DXBUFFER pAdjacencyDisc;
	LPD3DXBUFFER pMaterialsDisc;

	string filenameDisc = "shader/shadow/Disc.X";
	D3DXLoadMeshFromX(filenameDisc.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacencyDisc,
		&pMaterialsDisc,
		NULL,
		NULL,
		&m_pDisc);


	

	m_pCreateShadow = LoadEffect("shader/shadow/CreateShadow.fx");
	m_pApplyShadow = LoadEffect("shader/shadow/ApplyShadow.fx");


	m_hCmatWorld = m_pCreateShadow->GetParameterByName(0, "matWorld");
	m_hCmatLightView = m_pCreateShadow->GetParameterByName(0, "matLightView");
	m_hCmatLightProjection = m_pCreateShadow->GetParameterByName(0, "gLightProjectionMatrix");


	m_hApplyTexture = m_pApplyShadow->GetParameterByName(0, "ShadowMap_Tex");
	m_hAmatWorld = m_pApplyShadow->GetParameterByName(0, "matWorld");
	m_hAmatLightView = m_pApplyShadow->GetParameterByName(0, "matLightView");
	m_hAmatLightProjection = m_pApplyShadow->GetParameterByName(0, "matLightProjection");
	m_hAm_vec4LightPosition = m_pApplyShadow->GetParameterByName(0, "m_vec4LightPosition");
	m_hAmatViewProjection = m_pApplyShadow->GetParameterByName(0, "matViewProjection");
	m_hAgObjectColor = m_pApplyShadow->GetParameterByName(0, "gObjectColor");

	

	 m_vec4LightPosition = { 500.0f,500.0f ,-500.0f ,1.0f };
	 gObjectColor = { 1.00f, 1.00f, 0.00f, 1.00f };
	 gDiscColor = { 0.00f, 1.00f, 1.00f, 1.00f };

	D3DXMatrixIdentity(&matCWorld);
	D3DXMatrixScaling(&matS, 2, 2, 2);
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, 0.0f, -40.0f, 0.0f);
	matCWorld = matS*matT;

	{
		D3DXVECTOR3 vEyePt(m_vec4LightPosition.x, m_vec4LightPosition.y, m_vec4LightPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	{
		RECT rc;
		GetClientRect(g_hWnd, &rc);
		//GETDEVICE->GetTransform(D3DTS_PROJECTION, &matLightProjection);
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 3000);
	}

}

void cShadowMapping::Update()
{
	gRotationY += 0.4f * D3DX_PI / 180.0f;
	if (gRotationY > 2 * D3DX_PI)
	{
		gRotationY -= 2 * D3DX_PI;
	}

	D3DXMatrixRotationY(&matWorld, gRotationY);
}

void cShadowMapping::Render()
{
	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);

	matViewProjection = matView*matProjection;

	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	GETDEVICE->GetRenderTarget(0, &pHWBackBuffer);
	GETDEVICE->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. �׸��� �����
	//////////////////////////////

	// �׸��� ���� ����Ÿ��� ���̹��۸� ����Ѵ�.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface);

	GETDEVICE->SetRenderTarget(0, pShadowSurface);
	GETDEVICE->SetDepthStencilSurface(m_pShadowDepthStencil);

	SAFE_RELEASE(pShadowSurface);

	// ���� �����ӿ� �׷ȴ� �׸��� ������ ����
	GETDEVICE->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);


	// �׸��� ����� ���̴� ������������ ����
	m_pCreateShadow->SetMatrix(m_hCmatWorld, &matWorld);
	m_pCreateShadow->SetMatrix(m_hCmatLightView, &matLightView);
	m_pCreateShadow->SetMatrix(m_hCmatLightProjection, &matLightProjection);
	
	// �׸��� ����� ���̴��� ����
	{
		UINT numPasses = 0;
		m_pCreateShadow->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pCreateShadow->BeginPass(i);
				{
					// ��ȯü�� �׸���.
					m_pTorus->DrawSubset(0);
				}
				m_pCreateShadow->EndPass();
			}
		}
		m_pCreateShadow->End();
	}

	
	//////////////////////////////
	// 2. �׸��� ������
	//////////////////////////////

	////// �ϵ���� �����/���̹��۸� ����Ѵ�.
	GETDEVICE->SetRenderTarget(0, pHWBackBuffer);
	GETDEVICE->SetDepthStencilSurface(pHWDepthStencilBuffer);

	SAFE_RELEASE(pHWBackBuffer);
	SAFE_RELEASE(pHWDepthStencilBuffer);
	

	// �׸��� ������ ���̴� ������������ ����
	m_pApplyShadow->SetMatrix(m_hAmatWorld, &matWorld);		//��ȯü
	m_pApplyShadow->SetMatrix(m_hAmatViewProjection, &matViewProjection);
	m_pApplyShadow->SetMatrix(m_hAmatLightView, &matLightView);
	m_pApplyShadow->SetMatrix(m_hAmatLightProjection, &matLightProjection);

	m_pApplyShadow->SetVector(m_hAm_vec4LightPosition, &m_vec4LightPosition);
	m_pApplyShadow->SetVector(m_hAgObjectColor, &gObjectColor);

	m_pApplyShadow->SetTexture(m_hApplyTexture, m_pShadowRenderTarget);


	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pApplyShadow->BeginPass(i);
			{
				// ��ȯü�� �׸���.
				m_pTorus->DrawSubset(0);

				// ��ũ�� �׸���.
				m_pApplyShadow->SetMatrix(m_hAmatWorld, &matCWorld);
				m_pApplyShadow->SetVector(m_hAgObjectColor, &gDiscColor);
				m_pApplyShadow->CommitChanges();
				m_pDisc->DrawSubset(0);
			}
			m_pApplyShadow->EndPass();
		}
	}
	m_pApplyShadow->End();

}

LPD3DXEFFECT cShadowMapping::LoadEffect(const char * szFileName)
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
