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

	// 셰이더 로딩

	LPD3DXBUFFER      pError = NULL;         //에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD            dwShaderFlag = 0;      //셰이더 플레그 0 

#ifdef _DEBUG

	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;      //셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif
														 //fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(

		GETDEVICE,            // 디바이스
		szFileName,               // 불러올 셰이더 코드 파일이름
		NULL,                  // 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,                  // 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,            // 셰이더 컴파일 플레그
		NULL,                  // 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,               // 로딩될 셰이더 Effect 포인터
		&pError                  // 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
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
