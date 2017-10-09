#include "stdafx.h"
#include "TeicCharacterBlood.h"


TeicCharacterBlood::TeicCharacterBlood()
{
	m_fAlpha = 1;
	m_fDelta = 0.05;
	m_bStart = false;
	m_pEffect = NULL;

	m_pTexture = NULL;

}


TeicCharacterBlood::~TeicCharacterBlood()
{
	SAFE_RELEASE(m_pEffect);

	SAFE_RELEASE(m_pTexture);

}

void TeicCharacterBlood::Update()
{
	if (!m_bStart) return;
	m_fAlpha -= m_fDelta;

	if (m_fAlpha < 0)
	{
		m_bStart = false;
	}
}

void TeicCharacterBlood::Start()
{
	if (m_bStart)return;
	m_bStart = true;
	m_fAlpha = 0.8;
}

void TeicCharacterBlood::Render()
{
	if (!m_bStart) return;
	UINT numPasses = 0;
	m_pEffect->SetFloat("alpha", m_fAlpha);
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pEffect->SetTexture("base_Tex", m_pTexture);
				m_pEffect->CommitChanges();


				GETDEVICE->SetFVF(ST_PT_VERTEX::FVF);
				GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_stVertex[0], sizeof(ST_PT_VERTEX));

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();
}

void TeicCharacterBlood::Stop()
{
	m_bStart = false;
}



void TeicCharacterBlood::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_pEffect = LoadEffect("sprites/characterBlood.fx");
	m_stVertex[0].p = D3DXVECTOR3(-1, -1, 0);
	m_stVertex[1].p = D3DXVECTOR3(-1, 1, 0);
	m_stVertex[2].p = D3DXVECTOR3(1, 1, 0);
	m_stVertex[3].p = D3DXVECTOR3(1, 1, 0);
	m_stVertex[4].p = D3DXVECTOR3(1, -1, 0);
	m_stVertex[5].p = D3DXVECTOR3(-1, -1, 0);
	m_stVertex[0].t = D3DXVECTOR2(0, 1);
	m_stVertex[1].t = D3DXVECTOR2(0, 0);
	m_stVertex[2].t = D3DXVECTOR2(1, 0);
	m_stVertex[3].t = D3DXVECTOR2(1, 0);
	m_stVertex[4].t = D3DXVECTOR2(1, 1);
	m_stVertex[5].t = D3DXVECTOR2(0, 1);

	/*for (int i = 0; i < 1; i++)
	{
		char str[128];
		sprintf_s(str, "sprites/blood%d.jpg", i+1);
		D3DXCreateTextureFromFile(GETDEVICE, str, &m_pTexture[i]);
	}*/

	m_pTexture = TEXTUREMANAGER->GetTexture("sprites/bloodfinal.jpg");


}
void TeicCharacterBlood::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{


}
bool TeicCharacterBlood::IsRunning()
{
	return m_bStart;
}

LPD3DXEFFECT TeicCharacterBlood::LoadEffect(const char * szFileName)
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
