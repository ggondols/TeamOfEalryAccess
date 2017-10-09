#include "stdafx.h"
#include "TeicMonsterBlood.h"


TeicMonsterBlood::TeicMonsterBlood()
{
	m_iAlpha = 1;
	m_iDelta = 0;
	m_bStart = false;
	m_pEffect = NULL;
	
	m_pTexture= NULL;
	
}


TeicMonsterBlood::~TeicMonsterBlood()
{
	SAFE_RELEASE(m_pEffect);
	
		SAFE_RELEASE(m_pTexture);
	
}

void TeicMonsterBlood::Update()
{
	if (!m_bStart) return;
	m_iDelta++;
	if (m_iDelta > 2)
	{
		m_iDelta = 0;
		m_iAlpha++;
		if (m_iAlpha > 16)
			m_bStart = false;
	}
	
}

void TeicMonsterBlood::Start()
{
	if (m_bStart)return;
	m_bStart = true;
	m_iAlpha = 1;
	m_iDelta = 0;
}

void TeicMonsterBlood::Render()
{
	if (!m_bStart) return;

	m_stVertex[0].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 1);
	m_stVertex[1].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 0);
	m_stVertex[2].t = D3DXVECTOR2(0.0625 *m_iAlpha, 0);
	m_stVertex[3].t = D3DXVECTOR2(0.0625 *m_iAlpha, 0);
	m_stVertex[4].t = D3DXVECTOR2(0.0625 *m_iAlpha, 1);
	m_stVertex[5].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 1);

	UINT numPasses = 0;
	D3DXMATRIX view, projection;

	
	

	GETDEVICE->GetTransform(D3DTS_VIEW, &view);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &projection);
	m_pEffect->SetMatrix("matWorldViewProjection", &(m_World*view*projection));
	m_pEffect->SetTexture("base_Tex", m_pTexture);
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pEffect->CommitChanges();


				GETDEVICE->SetFVF(ST_PT_VERTEX::FVF);
				GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_stVertex[0], sizeof(ST_PT_VERTEX));

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();
}

void TeicMonsterBlood::Stop()
{
	m_bStart = false;
}



void TeicMonsterBlood::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_pEffect = LoadEffect("sprites/monsterBlood.fx");
	m_stVertex[0].p = D3DXVECTOR3(-1, -1, -1);
	m_stVertex[1].p = D3DXVECTOR3(-1, 1, -1);
	m_stVertex[2].p = D3DXVECTOR3(1, 1, -1);
	m_stVertex[3].p = D3DXVECTOR3(1, 1, -1);
	m_stVertex[4].p = D3DXVECTOR3(1, -1, -1);
	m_stVertex[5].p = D3DXVECTOR3(-1, -1, -1);
	m_stVertex[0].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 1);
	m_stVertex[1].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 0);
	m_stVertex[2].t = D3DXVECTOR2(0.0625 *m_iAlpha, 0);
	m_stVertex[3].t = D3DXVECTOR2(0.0625 *m_iAlpha, 0);
	m_stVertex[4].t = D3DXVECTOR2(0.0625 *m_iAlpha, 1);
	m_stVertex[5].t = D3DXVECTOR2(0.0625 *m_iAlpha - 0.0625, 1);

	m_pTexture = TEXTUREMANAGER->GetTexture("sprites/monsterblood2.png");
}
void TeicMonsterBlood::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	D3DXMATRIX trans,ro;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);

	D3DXMatrixLookAtLH(&ro,
		&CAMERA->getEye(),
		&position,
		&D3DXVECTOR3(0, 1, 0));
	ro._41 = ro._42 = ro._43 = 0;
	D3DXMatrixTranspose(&ro, &ro);
	m_World = ro *trans;


}
bool TeicMonsterBlood::IsRunning()
{
	return m_bStart;
}

LPD3DXEFFECT TeicMonsterBlood::LoadEffect(const char * szFileName)
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
