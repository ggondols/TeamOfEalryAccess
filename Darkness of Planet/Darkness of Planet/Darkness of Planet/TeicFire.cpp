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
	//앞
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	////뒤

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






	// 쉐이더 전역변수들을 설정


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

	// 셰이더 로딩
	LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

																									//fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(
		GETDEVICE,				// 디바이스
		szFileName,					// 불러올 셰이더 코드 파일이름
		NULL,						// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,						// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				// 셰이더 컴파일 플레그
		NULL,						// 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					// 로딩될 셰이더 Effect 포인터
		&pError						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
	);

	//셰이더 파일로딩에문재가 있다면..
	if (pError != NULL || pEffect == NULL) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}
