#include "stdafx.h"
#include "TeicCharacterBlood.h"


TeicCharacterBlood::TeicCharacterBlood()
{
}


TeicCharacterBlood::~TeicCharacterBlood()
{
}

void TeicCharacterBlood::Update()
{
}

void TeicCharacterBlood::Start()
{
}

void TeicCharacterBlood::Render()
{
}

void TeicCharacterBlood::Stop()
{
}

void TeicCharacterBlood::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
}

void TeicCharacterBlood::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_pEffect = LoadEffect();
}

bool TeicCharacterBlood::IsRunning()
{
	return false;
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
