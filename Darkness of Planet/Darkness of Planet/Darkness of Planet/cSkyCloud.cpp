#include "stdafx.h"
#include "cSkyCloud.h"


cSkyCloud::cSkyCloud()
	:m_pEffect(NULL)
	, m_pMesh(NULL)
	, m_dMatarial(0)
	, m_fDegree(0)
	, ftest(0.0f)
{
}


cSkyCloud::~cSkyCloud()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
}

void cSkyCloud::Setup()
{
	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;

	string filename = "shader/Skybox/dome.X";
	D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacency,
		&pMaterials,
		NULL,
		&m_dMatarial,
		&m_pMesh);


	m_pEffect = LoadEffect("shader/Skybox/CloudMapping.fx");
	m_hMatWorld = m_pEffect->GetParameterByName(0, "matWorld");
	m_hMatView = m_pEffect->GetParameterByName(0, "matView");
	m_hMatProjection = m_pEffect->GetParameterByName(0, "matProjection");
	m_hTexture = m_pEffect->GetParameterByName(0, "NoiseMap_Tex");
	m_hFSharpness = m_pEffect->GetParameterByName(0, "gSharpness");
	m_hFTime = m_pEffect->GetParameterByName(0, "gTime");

}

void cSkyCloud::Update()
{
	ftest += 0.001f;
}

void cSkyCloud::Render()
{
	D3DXMATRIXA16 matWorld, matView, matProjection;
	D3DXMatrixIdentity(&matWorld);

	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matS,matRY,matRX;
	D3DXMatrixScaling(&matS, 30.0f, 30.0f, 30.0f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, ftest);
	matWorld = matS*matRY*matWorld;

	m_pEffect->SetMatrix(m_hMatWorld, &matWorld);
	m_pEffect->SetMatrix(m_hMatView, &matView);
	m_pEffect->SetMatrix(m_hMatProjection, &matProjection);
	m_pEffect->SetFloat(m_hFSharpness, 1.0f);
	m_pEffect->SetFloat(m_hFTime, 1.0f);
	LPDIRECT3DTEXTURE9 texture;
	texture = TEXTUREMANAGER->GetTexture("shader/Skybox/Cloud1.jpg");
	m_pEffect->SetTexture(m_hTexture, texture);

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

LPD3DXEFFECT cSkyCloud::LoadEffect(const char * szFileName)
{

	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩
	LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
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
