#include "stdafx.h"
#include "TeicLaser.h"


TeicLaser::TeicLaser()
{
	
	m_pMesh = NULL;
	

	m_fEndtiming = 0.0f;
	m_pEffect = NULL;
	m_fAlpha = 1;
	m_fDelta = (0.016667) / 2.0f;
	m_fTime = 0;
	m_pTexture = NULL;
}


TeicLaser::~TeicLaser()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTexture);
}

LPD3DXMESH TeicLaser::LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, GETDEVICE, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("�� �ε� ���� ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}
	return ret;
}


void TeicLaser::Update()
{
	if (!m_bStart)return;
	m_fTime += TIMEMANAGER->getElapsedTime();
	if (TIMEMANAGER->getWorldTime() > m_fEndtiming + 0.5f)
	{
		Stop();
	}
}

void TeicLaser::Start()
{
	if (m_bStart)return;
	

	m_bStart = true;
	m_fEndtiming = TIMEMANAGER->getWorldTime();
	m_fAlpha = 1;
}

void TeicLaser::Stop()
{
	m_bStart = false;

}

void TeicLaser::Render()
{
	if (!m_bStart)return;
	
	/*GETDEVICE->SetTransform(D3DTS_WORLD, &m_Matworld);

	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture("sprites/laser3.jpg"));*/
	D3DXMATRIX view, proj;
	GETDEVICE->GetTransform(D3DTS_VIEW, &view);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &proj);
	m_fAlpha -= m_fDelta;
	UINT numPasses = 0;
	m_pEffect->SetMatrix("gWorldMatrix",&m_Matworld);
	m_pEffect->SetMatrix("gViewMatrix", &view);
	m_pEffect->SetMatrix("gProjectionMatrix", &proj);
	m_pEffect->SetFloat("gTime",m_fTime);
	m_pEffect->SetTexture("DiffuseMap_Tex", m_pTexture);
	/*m_pEffect->SetFloat("vAlpha", m_fAlpha);*/
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pEffect->CommitChanges();
				m_pMesh->DrawSubset(0);

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();
	//m_pMesh->UpdateAndRender();

}

void TeicLaser::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_pMesh = LoadModel("sprites/laser.X");
	m_pEffect = LoadEffect("sprites/UVAnimation2.fx");
	
	m_pTexture = TEXTUREMANAGER->GetTexture("sprites/B_light_beam001_emis.tga");


}
void TeicLaser::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	//matWorld;
	D3DXMatrixTranslation(&m_Matworld, 50, 0, 0);
	D3DXMATRIX scal, ro, tran;
	D3DXMatrixScaling(&scal, 1, 0.01, 0.01);
	D3DXMatrixRotationY(&ro, -D3DX_PI / 2);
	m_Matworld = m_Matworld*scal *ro;

	D3DXMatrixLookAtLH(&ro,
		&CAMERA->getEye(),
		&characterpos,
		&D3DXVECTOR3(0, 1, 0));
	ro._41 = ro._42 = ro._43 = 0;
	D3DXMatrixTranspose(&ro, &ro);
	m_Matworld = m_Matworld*ro;
	D3DXMatrixTranslation(&tran, position.x, position.y, position.z);
	m_Matworld = m_Matworld * tran;
	
}

LPD3DXEFFECT TeicLaser::LoadEffect(const char* szFileName)
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


void TeicLaser::Callbackon(int n)
{
	

}


bool TeicLaser::IsRunning()
{
	return m_bStart;
}
