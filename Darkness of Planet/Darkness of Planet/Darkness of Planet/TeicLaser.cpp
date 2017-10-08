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
		OutputDebugString("모델 로딩 실패 ");
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


void TeicLaser::Callbackon(int n)
{
	

}


bool TeicLaser::IsRunning()
{
	return m_bStart;
}
