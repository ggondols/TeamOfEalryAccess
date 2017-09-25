#include "stdafx.h"
#include "LDYMotionBlur.h"


LDYMotionBlur::LDYMotionBlur()
{
	g_pFullScreenRenderTarget = NULL;
	g_pFullScreenRenderTargetSurf = NULL;
	g_pPixelVelocityTexture1 = NULL;
	g_pPixelVelocitySurf1 = NULL;
	g_pPixelVelocityTexture2 = NULL;
	g_pPixelVelocitySurf2 = NULL;
	g_pLastFrameVelocityTexture = NULL;
	g_pCurFrameVelocityTexture = NULL;
	g_pCurFrameVelocitySurf = NULL;
	g_pLastFrameVelocitySurf = NULL;
	g_pEffect = NULL;
	g_nSleepTime = 0;
	g_fPixelBlurConst = 1.0f;
	g_fObjectSpeed = 8.0f;
	g_fCameraSpeed = 20.0f;
	g_nCurrentScene = 1;

	g_fChangeTime = 0.0f;
	g_dwMoveSytle = MOVESTYLE_DEFAULT;
	D3DXMatrixIdentity(&g_mViewProjectionLast);

	g_hWorld = NULL;
	g_hWorldLast = NULL;
	g_hMeshTexture = NULL;
	g_hWorldViewProjection = NULL;
	g_hWorldViewProjectionLast = NULL;
	g_hCurFrameVelocityTexture = NULL;
	g_hLastFrameVelocityTexture = NULL;
	g_hTechWorldWithVelocity = NULL;
	g_hPostProcessMotionBlur = NULL;

	g_bShowBlurFactor = FALSE;
	g_bShowUnblurred = FALSE;
	g_dwBackgroundColor = 0x00003F3F;


}


LDYMotionBlur::~LDYMotionBlur()
{
	SAFE_RELEASE(g_pFullScreenRenderTargetSurf);
	SAFE_RELEASE(g_pFullScreenRenderTarget);
	SAFE_RELEASE(g_pPixelVelocitySurf1);
	SAFE_RELEASE(g_pPixelVelocityTexture1);
	SAFE_RELEASE(g_pPixelVelocitySurf2);
	SAFE_RELEASE(g_pPixelVelocityTexture2);

	SAFE_RELEASE(g_pEffect);

	SAFE_RELEASE(g_pFullScreenRenderTargetSurf);
	SAFE_RELEASE(g_pFullScreenRenderTarget);

}

void LDYMotionBlur::Setup()
{
	g_aRTSet[0].pRT[0][0] = g_pFullScreenRenderTargetSurf;
	g_aRTSet[0].pRT[0][1] = g_pPixelVelocitySurf1;
	g_aRTSet[0].pRT[1][0] = NULL;  // 2nd pass is not needed
	g_aRTSet[0].pRT[1][1] = NULL;  // 2nd pass is not needed

								   // Second frame
	g_aRTSet[1].pRT[0][0] = g_pFullScreenRenderTargetSurf;
	g_aRTSet[1].pRT[0][1] = g_pPixelVelocitySurf2;
	g_aRTSet[1].pRT[1][0] = NULL;  // 2nd pass is not needed
	g_aRTSet[1].pRT[1][1] = NULL;  // 2nd pass is not needed

}

void LDYMotionBlur::Update()
{

}

void LDYMotionBlur::Render()
{

}

LPD3DXEFFECT LDYMotionBlur::LoadEffect(const char * szFileName)
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
