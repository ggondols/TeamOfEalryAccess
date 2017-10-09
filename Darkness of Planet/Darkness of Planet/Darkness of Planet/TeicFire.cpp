#include "stdafx.h"
#include "TeicFire.h"


TeicFire::TeicFire()
{
	m_fTimeChange = 0.01;
	m_pfire_opacity = NULL;
	m_bStart = false;
	m_fEndtiming = 0.0f;
	m_fAlpha = 1.0f;
	m_fDelta = (0.016667)/ 1.0f;
}


TeicFire::~TeicFire()
{

	SAFE_RELEASE(m_pFlame);
	SAFE_RELEASE(m_pNoise);
	SAFE_RELEASE(m_pfire_opacity);
}

void TeicFire::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	Setup("", 5);
	m_vStart = characterpos;
	m_vFinish = position;
	
}	


void TeicFire::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_vStart = characterpos;
	m_vFinish = position;
	
	//getmuslepos;
}


bool TeicFire::IsRunning()
{
	return m_bStart;
}
void TeicFire::Setup(char * Textfilename, float size)
{

	m_fSize = size;



	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	vecVertex.push_back(D3DXVECTOR3(-size , -size, 0));
	vecVertex.push_back(D3DXVECTOR3(-size , size, 0));
	vecVertex.push_back(D3DXVECTOR3(size , size, 0));
	vecVertex.push_back(D3DXVECTOR3(size , -size, 0));
	vecVertex.push_back(D3DXVECTOR3(-size , -size, 0));
	vecVertex.push_back(D3DXVECTOR3(-size , size, 0));
	vecVertex.push_back(D3DXVECTOR3(size , size, 0));
	vecVertex.push_back(D3DXVECTOR3(size , -size, 0));

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
		m_vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
	}



	m_pEffect = LoadEffect("Fire2.fx");
	m_pFlame = TEXTUREMANAGER->GetTexture("sprites/Flame.tga");
	m_pNoise = TEXTUREMANAGER->GetTexture("sprites/NoiseVolume.dds");
	m_pfire_opacity = TEXTUREMANAGER->GetTexture("sprites/FireOpacity.tga");
	hTech = 0;
	hTech = m_pEffect->GetTechniqueByName("FireBall");

	

	D3DXMatrixLookAtLH(&m_matRo,
		&CAMERA->getEye(),
		&m_vFinish,
		&D3DXVECTOR3(0, 1, 0));
	m_matRo._41 = m_matRo._42 = m_matRo._43 = 0;
	D3DXMatrixTranspose(&m_matRo, &m_matRo);
}

void TeicFire::Update()
{
	if (!m_bStart)return;
	m_fNowTime += TIMEMANAGER->getElapsedTime();
	D3DXVECTOR3 temp;
	D3DXVec3Lerp(&temp, &m_vStart, &m_vFinish, m_fNowTime / 1.0f);
	D3DXMatrixTranslation(&m_matTrans, temp.x, temp.y, temp.z);
	if (TIMEMANAGER->getWorldTime() > m_fEndtiming + 1.5f)
	{
		Stop();
	}
}

void TeicFire::Render()
{

	if (!m_bStart)return;
	
	m_fTimeChange -= 0.01;
	if (m_fTimeChange < 0)
	{
		m_fTimeChange = 0;
	}
	m_fAlpha -= m_fDelta;
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran, -4,0, 0);
	D3DXMatrixLookAtLH(&m_matRo,
		&CAMERA->getEye(),
		&m_vFinish,
		&D3DXVECTOR3(0, 1, 0));
	m_matRo._41 = m_matRo._42 = m_matRo._43 = 0;
	D3DXMatrixTranspose(&m_matRo, &m_matRo);




	
	m_matWorld = tran*m_matRo *m_matTrans;






	D3DXMATRIXA16  matView, matProjection;
	//	D3DXMatrixIdentity(&matWorld);
//	D3DXMatrixTranslation(&matWorld, 100, 40, -100);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);






	// ���̴� ������������ ����


	//m_pEffect->SetTexture("base_Tex", m_pTexture);

	/*m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetMatrix("matViewProjection", &(matView*matProjection));
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProjection));*/
	m_pEffect->SetFloat("time_0_X", 2.0f);
	m_pEffect->SetFloat("Fire_Effects_FireBall_Single_Pass_Pixel_Shader_time_0_X", m_fTimeChange);
	m_pEffect->SetFloat("timeSampleDist", m_fTimeChange);
	m_pEffect->SetFloat("alpha", m_fAlpha);
	m_pEffect->SetTexture("Flame_Tex", m_pFlame);
	m_pEffect->SetTexture("Noise_Tex", m_pNoise);
	m_pEffect->SetTexture("fire_opacity_Tex", m_pfire_opacity);
	m_pEffect->SetMatrix("matWorldViewProjectionMatrix", &(m_matWorld*matView*matProjection));
	m_pEffect->SetTechnique(hTech);


	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GETDEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	GETDEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pEffect->CommitChanges();
		GETDEVICE->SetFVF(ST_PNT_VERTEX::FVF);
		GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));
		m_pEffect->EndPass();
	}
	m_pEffect->End();





	


}

void TeicFire::Start()
{
	if (m_bStart) return;
	m_bStart = true;
	m_fEndtiming = TIMEMANAGER->getWorldTime();
	m_fNowTime = 0;
	m_fTimeChange = 0.6;
	m_fAlpha = RND->getFromFloatTo(0.5,1);
}

void TeicFire::Stop()
{
	m_bStart = false;
}



LPD3DXEFFECT TeicFire::LoadEffect(const char* szFileName)
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
