#include "stdafx.h"
#include "cShadowMapping.h"
#include "LDYiMap.h"
#include "LDYCharacter.h"
#include "LDYHeightMap.h"


cShadowMapping::cShadowMapping()
	:m_pCreateShadow(NULL)
	, m_pApplyShadow(NULL)
	, m_pDisc(NULL)
	, m_pTorus(NULL)
	, m_pShadowRenderTarget(NULL)
	, m_pShadowDepthStencil(NULL)
	, gRotationY(0.0f)
	,m_pMap(NULL)
	, m_pCharacter(NULL)
	, mapmesh(NULL)
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

	SAFE_DELETE(m_pMap);
	//SAFE_DELETE(m_pCharacter);
	SAFE_DELETE(m_pCharacter->m_pCtrl);

}

void cShadowMapping::Setup()
{
	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, 1);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(D3DXVECTOR3(20, 20, 20));

	const int shadowMapSize = 2048;
	if (FAILED(GETDEVICE->CreateTexture(shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL)))
	{

	}



	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if (FAILED(GETDEVICE->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowDepthStencil, NULL)))
	{
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
	D3DXMatrixTranslation(&matT, 40.0f, -40.0f, 40.0f);

	{
		D3DXVECTOR3 vEyePt(m_vec4LightPosition.x, m_vec4LightPosition.y, m_vec4LightPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	{
		RECT rc;
		GetClientRect(g_hWnd, &rc);
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 3000);
	}

	m_pMap = new LDYHeightMap;
	m_pMap->Load("map/","HeightMap.raw","terrian.jpg",1);

	D3DXMatrixScaling(&matWorld, 0.04f, 0.04f, 0.04f);
	//D3DXMatrixTranslation(&matWorld, 20.0f, 0.0f, -20.0f);

}

void cShadowMapping::Update()
{
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
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface);


	GETDEVICE->SetRenderTarget(0, pShadowSurface);
	GETDEVICE->SetDepthStencilSurface(m_pShadowDepthStencil);

	SAFE_RELEASE(pShadowSurface);

	// 저번 프레임에 그\렸던 그림자 정보를 지움
	GETDEVICE->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);


	// 그림자 만들기 쉐이더 전역변수들을 설정
	m_pCreateShadow->SetMatrix(m_hCmatWorld, &matWorld);
	m_pCreateShadow->SetMatrix(m_hCmatLightView, &matLightView);
	m_pCreateShadow->SetMatrix(m_hCmatLightProjection, &matLightProjection);
	
	// 그림자 만들기 쉐이더를 시작
	{
		UINT numPasses = 0;
		m_pCreateShadow->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pCreateShadow->BeginPass(i);
				{
					// 원환체를 그린다.
					m_pCharacter->UpdateAndRender();
				}
				m_pCreateShadow->EndPass();
			}
		}
		m_pCreateShadow->End();
	}
	
	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////
	//if (m_pMap) m_pMap->Render();

	////// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	GETDEVICE->SetRenderTarget(0, pHWBackBuffer);
	GETDEVICE->SetDepthStencilSurface(pHWDepthStencilBuffer);

	SAFE_RELEASE(pHWBackBuffer);
	SAFE_RELEASE(pHWDepthStencilBuffer);
	
	/*GETDEVICE->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 0, 0),
		1.0f, 0);*/


	// 그림자 입히기 쉐이더 전역변수들을 설정
	m_pApplyShadow->SetMatrix(m_hAmatWorld, &matWorld);		//원환체
	m_pApplyShadow->SetMatrix(m_hAmatViewProjection, &matViewProjection);
	m_pApplyShadow->SetMatrix(m_hAmatLightView, &matLightView);
	m_pApplyShadow->SetMatrix(m_hAmatLightProjection, &matLightProjection);

	m_pApplyShadow->SetVector(m_hAm_vec4LightPosition, &m_vec4LightPosition);
	m_pApplyShadow->SetVector(m_hAgObjectColor, &gObjectColor);

	m_pApplyShadow->SetTexture(m_hApplyTexture, m_pShadowRenderTarget);

	LPDIRECT3DTEXTURE9 tex;
	tex = TEXTUREMANAGER->GetTexture("map/terrain.jpg");
	m_pApplyShadow->SetTexture("heightMap_Tex", tex);

	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	m_pApplyShadow->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pApplyShadow->BeginPass(i);
			{
				// 원환체를 그린다.
				m_pCharacter->UpdateAndRender();

				// 디스크를 그린다.
				m_pApplyShadow->SetMatrix(m_hAmatWorld, &matCWorld);
				m_pApplyShadow->SetVector(m_hAgObjectColor, &gDiscColor);
				m_pApplyShadow->CommitChanges();
				//if (m_pMap) m_pMap->Render();
				//m_pDisc->DrawSubset(0);
				if (m_pMap) mapmesh=m_pMap->getMesh();
				mapmesh->DrawSubset(0);
			}
			m_pApplyShadow->EndPass();
		}
	}
	m_pApplyShadow->End();

	//if (m_pMap) m_pMap->Render();

	

}

LPD3DXEFFECT cShadowMapping::LoadEffect(const char * szFileName)
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
