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

	int iObject;
	for (iObject = 0; iObject < NUM_OBJECTS; iObject++)
	{
		g_pScene1Object[iObject] = new OBJECT;
		if (g_pScene1Object[iObject] == NULL)
			return;
		ZeroMemory(g_pScene1Object[iObject], sizeof(OBJECT));
	}

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

	int iObject;
	for (iObject = 0; iObject < NUM_OBJECTS; iObject++)
		SAFE_DELETE(g_pScene1Object[iObject]);

}

void LDYMotionBlur::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);


	GETDEVICE->CreateTexture(rc.right,
		rc.bottom,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_pFullScreenRenderTarget,
		NULL);

	GETDEVICE->CreateTexture(rc.right,
		rc.bottom,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_pPixelVelocityTexture1,
		NULL);

	GETDEVICE->CreateTexture(rc.right,
		rc.bottom,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&g_pPixelVelocityTexture2,
		NULL);

	// Store pointers to surfaces so we can call SetRenderTarget() later
	g_pFullScreenRenderTarget->GetSurfaceLevel(0, &g_pFullScreenRenderTargetSurf);
	g_pPixelVelocityTexture1->GetSurfaceLevel(0, &g_pPixelVelocitySurf1);
	g_pPixelVelocityTexture2->GetSurfaceLevel(0, &g_pPixelVelocitySurf2);


	g_aRTSet[0].pRT[0][0] = g_pFullScreenRenderTargetSurf;
	g_aRTSet[0].pRT[0][1] = g_pPixelVelocitySurf1;
	g_aRTSet[0].pRT[1][0] = NULL;  // 2nd pass is not needed
	g_aRTSet[0].pRT[1][1] = NULL;  // 2nd pass is not needed

								   // Second frame
	g_aRTSet[1].pRT[0][0] = g_pFullScreenRenderTargetSurf;
	g_aRTSet[1].pRT[0][1] = g_pPixelVelocitySurf2;
	g_aRTSet[1].pRT[1][0] = NULL;  // 2nd pass is not needed
	g_aRTSet[1].pRT[1][1] = NULL;  // 2nd pass is not needed

	LPD3DXBUFFER pAdjacency;
	LPD3DXBUFFER pMaterials;

	string filename = "shader/Skybox/Sphere.X";
	D3DXLoadMeshFromX(filename.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&pAdjacency,
		&pMaterials,
		NULL,
		NULL,
		&g_pMesh1);

	g_pEffect = LoadEffect("shader/Shadow/PixelMotionBlur.fx");

	g_pCurFrameVelocityTexture = g_pPixelVelocityTexture1;
	g_pLastFrameVelocityTexture = g_pPixelVelocityTexture2;
	g_pCurFrameVelocitySurf = g_pPixelVelocitySurf1;
	g_pLastFrameVelocitySurf = g_pPixelVelocitySurf2;
	g_pCurFrameRTSet = &g_aRTSet[0];
	g_pLastFrameRTSet = &g_aRTSet[1];

	g_pMeshTexture1 = TEXTUREMANAGER->GetTexture("shader/Skybox/earth.bmp");
	int iObject;
	for (iObject = 0; iObject < NUM_OBJECTS; iObject++)
	{
		g_pScene1Object[iObject]->g_pMesh = g_pMesh1;
		//g_pScene1Object[iObject]->g_pMeshTexture = g_pMeshTexture1;
	}

	SetupFullscreenQuad();

	D3DXCOLOR colorWhite(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR colorBlack(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR colorAmbient(0.7f, 0.7f, 0.7f, 0);
	D3DXCOLOR colorSpecular(0.7f, 0.7f, 0.7f, 1.0f);
	g_pEffect->SetVector("MaterialAmbientColor", (D3DXVECTOR4*)&colorAmbient);
	g_pEffect->SetVector("MaterialDiffuseColor", (D3DXVECTOR4*)&colorWhite);
	g_pEffect->SetTexture("RenderTargetTexture", g_pFullScreenRenderTarget);

	D3DSURFACE_DESC desc;
	g_pFullScreenRenderTargetSurf->GetDesc(&desc);
	g_pEffect->SetFloat("RenderTargetWidth", (FLOAT)desc.Width);
	g_pEffect->SetFloat("RenderTargetHeight", (FLOAT)desc.Height);


	float fVelocityCapInPixels = 3.0f;
	float fVelocityCapNonHomogeneous = fVelocityCapInPixels * 2 / rc.right;
	float fVelocityCapSqNonHomogeneous = fVelocityCapNonHomogeneous * fVelocityCapNonHomogeneous;

	g_pEffect->SetFloat("VelocityCapSq", fVelocityCapSqNonHomogeneous);
	g_pEffect->SetFloat("ConvertToNonHomogeneous", 1.0f / rc.right);

	g_hTechWorldWithVelocity = g_pEffect->GetTechniqueByName("WorldWithVelocityTwoPasses");

	g_hPostProcessMotionBlur = g_pEffect->GetTechniqueByName("PostProcessMotionBlur");
	g_hWorld = g_pEffect->GetParameterByName(NULL, "mWorld");
	g_hWorldLast = g_pEffect->GetParameterByName(NULL, "mWorldLast");
	g_hWorldViewProjection = g_pEffect->GetParameterByName(NULL, "mWorldViewProjection");
	g_hWorldViewProjectionLast = g_pEffect->GetParameterByName(NULL, "mWorldViewProjectionLast");
	g_hMeshTexture = g_pEffect->GetParameterByName(NULL, "MeshTexture");
	g_hCurFrameVelocityTexture = g_pEffect->GetParameterByName(NULL, "CurFrameVelocityTexture");
	g_hLastFrameVelocityTexture = g_pEffect->GetParameterByName(NULL, "LastFrameVelocityTexture");

}

void LDYMotionBlur::Update()
{

}

void LDYMotionBlur::Render()
{
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);

	LPDIRECT3DSURFACE9 pOriginalRenderTarget;
	GETDEVICE->GetRenderTarget(0, &pOriginalRenderTarget);

	// Clear each RT
	GETDEVICE->SetRenderTarget(0, g_pFullScreenRenderTargetSurf);
	GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	GETDEVICE->SetRenderTarget(0, g_pLastFrameVelocitySurf);
	GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	GETDEVICE->SetRenderTarget(0, g_pCurFrameVelocitySurf);
	GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);

	// Restore the orignal RT
	GETDEVICE->SetRenderTarget(0, pOriginalRenderTarget);

	SAFE_RELEASE(pOriginalRenderTarget);

	int iObject;
	float fTime = TIMEMANAGER->getWorldTime();

	for (iObject = 0; iObject < 1; iObject++)
	{
		D3DXVECTOR3 vPos;

		float fRadius = 150.0f;

		{
			vPos.x = cosf(g_fObjectSpeed * fTime + 2 * D3DX_PI / 10 * iObject) * fRadius;
			vPos.y = 0.0f;
			vPos.z = sinf(g_fObjectSpeed * fTime + 2 * D3DX_PI / 10 * iObject) * fRadius - 5.0f;
			/*vPos.x = 0.0f;
			vPos.y = 0.0f;
			vPos.z = 0.0f;*/
		}

		g_pScene1Object[iObject]->g_vWorldPos = vPos;

		g_pScene1Object[iObject]->g_mWorldLast = g_pScene1Object[iObject]->g_mWorld;

		D3DXMatrixTranslation(&g_pScene1Object[iObject]->g_mWorld, g_pScene1Object[iObject]->g_vWorldPos.x,
			g_pScene1Object[iObject]->g_vWorldPos.y, g_pScene1Object[iObject]->g_vWorldPos.z);
	}

	D3DXMATRIX matV, matP, matVP, matW;
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXMATRIXA16 mWorldViewProjectionLast;
	UINT iPass, cPasses;
	LPDIRECT3DSURFACE9 apOriginalRenderTarget[2] = { NULL, NULL };


	(GETDEVICE->GetRenderTarget(0, &apOriginalRenderTarget[0]));

	g_pEffect->SetFloat("PixelBlurConst", g_fPixelBlurConst);

	g_pEffect->SetTexture(g_hCurFrameVelocityTexture, g_pCurFrameVelocityTexture);
	g_pEffect->SetTexture(g_hLastFrameVelocityTexture, g_pLastFrameVelocityTexture);

	GETDEVICE->SetRenderTarget(0, g_pCurFrameVelocitySurf);
	GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);

	GETDEVICE->SetRenderTarget(0, g_pFullScreenRenderTargetSurf);
	GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_dwBackgroundColor, 1.0f, 0);

	GETDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);


	g_pEffect->SetTechnique(g_hTechWorldWithVelocity);

	GETDEVICE->GetTransform(D3DTS_WORLD, &matW);
	GETDEVICE->GetTransform(D3DTS_VIEW, &matV);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matP);
	matVP = matV*matP;


	for (int iObject = 0; iObject < NUM_OBJECTS; iObject++)
	{
		mWorldViewProjection = g_pScene1Object[iObject]->g_mWorld * matVP;
		mWorldViewProjectionLast = g_pScene1Object[iObject]->g_mWorldLast * g_mViewProjectionLast;

		g_pEffect->SetMatrix(g_hWorldViewProjection, &mWorldViewProjection);
		g_pEffect->SetMatrix(g_hWorld, &g_pScene1Object[iObject]->g_mWorld);

		g_pEffect->SetMatrix(g_hWorldViewProjectionLast, &mWorldViewProjectionLast);

		g_pEffect->SetTexture(g_hMeshTexture, g_pScene1Object[iObject]->g_pMeshTexture);

		g_pEffect->Begin(&cPasses, 0);
		for (iPass = 0; iPass < cPasses; iPass++)
		{

			for (int rt = 0; rt < g_nRtUsed; ++rt)
			{
				GETDEVICE->SetRenderTarget(rt, g_pCurFrameRTSet->pRT[iPass][rt]);
			}
			g_pEffect->BeginPass(iPass);
			g_pScene1Object[iObject]->g_pMesh->DrawSubset(0);
			g_pEffect->EndPass();
		}
		(g_pEffect->End());
	}
	g_mViewProjectionLast = matVP;

	for (int rt = 0; rt < g_nRtUsed; ++rt)
	{
		GETDEVICE->SetRenderTarget(rt, apOriginalRenderTarget[rt]);
	}

	SAFE_RELEASE(apOriginalRenderTarget[0]);
	GETDEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);

	GETDEVICE->Clear(0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);

	g_pEffect->SetTechnique(g_hPostProcessMotionBlur);

	g_pEffect->Begin(&cPasses, 0);
	for (iPass = 0; iPass < cPasses; iPass++)
	{
		(g_pEffect->BeginPass(iPass));
		(GETDEVICE->SetFVF(SCREEN_VERTEX::FVF));
		(GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Vertex, sizeof(SCREEN_VERTEX)));
		(g_pEffect->EndPass());
	}
	g_pEffect->End();

	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
}

void LDYMotionBlur::SetupFullscreenQuad()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DSURFACE_DESC desc;

	g_pFullScreenRenderTargetSurf->GetDesc(&desc);

	FLOAT fWidth5 = (FLOAT)rc.right - 0.5f;
	FLOAT fHeight5 = (FLOAT)rc.bottom - 0.5f;

	FLOAT fTexWidth1 = (FLOAT)rc.right / (FLOAT)desc.Width;
	FLOAT fTexHeight1 = (FLOAT)rc.bottom / (FLOAT)desc.Height;

	g_Vertex[0].pos = D3DXVECTOR4(fWidth5, -0.5f, 0.0f, 1.0f);
	g_Vertex[0].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
	g_Vertex[0].tex1 = D3DXVECTOR2(fTexWidth1, 0.0f);

	g_Vertex[1].pos = D3DXVECTOR4(fWidth5, fHeight5, 0.0f, 1.0f);
	g_Vertex[1].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
	g_Vertex[1].tex1 = D3DXVECTOR2(fTexWidth1, fTexHeight1);

	g_Vertex[2].pos = D3DXVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f);
	g_Vertex[2].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
	g_Vertex[2].tex1 = D3DXVECTOR2(0.0f, 0.0f);

	g_Vertex[3].pos = D3DXVECTOR4(-0.5f, fHeight5, 0.0f, 1.0f);
	g_Vertex[3].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
	g_Vertex[3].tex1 = D3DXVECTOR2(0.0f, fTexHeight1);
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
