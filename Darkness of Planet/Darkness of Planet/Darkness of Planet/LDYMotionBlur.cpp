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

	// Create two floating-point render targets with at least 2 channels.  These will be used to store 
	// velocity of each pixel (one for the current frame, and one for last frame).

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
	(g_pFullScreenRenderTarget->GetSurfaceLevel(0, &g_pFullScreenRenderTargetSurf));
	(g_pPixelVelocityTexture1->GetSurfaceLevel(0, &g_pPixelVelocitySurf1));
	(g_pPixelVelocityTexture2->GetSurfaceLevel(0, &g_pPixelVelocitySurf2));


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
		g_pScene1Object[iObject]->g_pMeshTexture = g_pMeshTexture1;
	}

	SetupFullscreenQuad();

	D3DXCOLOR colorWhite(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR colorBlack(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR colorAmbient(0.35f, 0.35f, 0.35f, 0);
	D3DXCOLOR colorSpecular(0.5f, 0.5f, 0.5f, 1.0f);
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

	g_hTechWorldWithVelocity = g_pEffect->GetTechniqueByName("WorldWithVelocityMRT");

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
	(GETDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE));

	LPDIRECT3DSURFACE9 pOriginalRenderTarget;
	(GETDEVICE->GetRenderTarget(0, &pOriginalRenderTarget));

	// Clear each RT
	(GETDEVICE->SetRenderTarget(0, g_pFullScreenRenderTargetSurf));
	(GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0));
	(GETDEVICE->SetRenderTarget(0, g_pLastFrameVelocitySurf));
	(GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0));
	(GETDEVICE->SetRenderTarget(0, g_pCurFrameVelocitySurf));
	(GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0));

	// Restore the orignal RT
	(GETDEVICE->SetRenderTarget(0, pOriginalRenderTarget));

	SAFE_RELEASE(pOriginalRenderTarget);
	int iObject;
	float fTime = TIMEMANAGER->getWorldTime();

	for (iObject = 0; iObject < NUM_OBJECTS; iObject++)
	{
		D3DXVECTOR3 vPos;

		float fRadius = 150.0f;
		/*if (iObject >= 30 && iObject < 41)
		{
			vPos.x = cosf(g_fObjectSpeed * 0.125f * fTime + 2 * D3DX_PI / 10 * (iObject - 30)) * fRadius;
			vPos.y = 10.0f;
			vPos.z = sinf(g_fObjectSpeed * 0.125f * fTime + 2 * D3DX_PI / 10 * (iObject - 30)) * fRadius -
				25.0f;
		}*/
		/*else if (iObject >= 20 && iObject < 31)
		{
			vPos.x = cosf(g_fObjectSpeed * 0.25f * fTime + 2 * D3DX_PI / 10 * (iObject - 20)) * fRadius;
			vPos.y = 10.0f;
			vPos.z = sinf(g_fObjectSpeed * 0.25f * fTime + 2 * D3DX_PI / 10 * (iObject - 20)) * fRadius - 5.0f;
		}
		else if (iObject >= 10 && iObject < 21)
		{
			vPos.x = cosf(g_fObjectSpeed * 0.5f * fTime + 2 * D3DX_PI / 10 * (iObject - 10)) * fRadius;
			vPos.y = 0.0f;
			vPos.z = sinf(g_fObjectSpeed * 0.5f * fTime + 2 * D3DX_PI / 10 * (iObject - 10)) * fRadius - 25.0f;
		}*/
		
		{
			vPos.x = cosf(g_fObjectSpeed * fTime + 2 * D3DX_PI / 10 * iObject) * fRadius;
			vPos.y = 0.0f;
			vPos.z = sinf(g_fObjectSpeed * fTime + 2 * D3DX_PI / 10 * iObject) * fRadius - 5.0f;
		}

		g_pScene1Object[iObject]->g_vWorldPos = vPos;

		// Store the last world matrix so that we can tell the effect file
		// what it was when we render this object
		g_pScene1Object[iObject]->g_mWorldLast = g_pScene1Object[iObject]->g_mWorld;

		// Update the current world matrix for this object
		D3DXMatrixTranslation(&g_pScene1Object[iObject]->g_mWorld, g_pScene1Object[iObject]->g_vWorldPos.x,
			g_pScene1Object[iObject]->g_vWorldPos.y, g_pScene1Object[iObject]->g_vWorldPos.z);
	}

	D3DXMATRIX matV, matP, matVP, matW;
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXMATRIXA16 mWorldViewProjectionLast;
	UINT iPass, cPasses;
	LPDIRECT3DSURFACE9 apOriginalRenderTarget[2] = { NULL, NULL };

	LPDIRECT3DTEXTURE9 pTempTex = g_pCurFrameVelocityTexture;
	g_pCurFrameVelocityTexture = g_pLastFrameVelocityTexture;
	g_pLastFrameVelocityTexture = pTempTex;

	LPDIRECT3DSURFACE9 pTempSurf = g_pCurFrameVelocitySurf;
	g_pCurFrameVelocitySurf = g_pLastFrameVelocitySurf;
	g_pLastFrameVelocitySurf = pTempSurf;

	CRenderTargetSet* pTempRTSet = g_pCurFrameRTSet;
	g_pCurFrameRTSet = g_pLastFrameRTSet;
	g_pLastFrameRTSet = pTempRTSet;

	(GETDEVICE->GetRenderTarget(0, &apOriginalRenderTarget[0]));

	g_pEffect->SetFloat("PixelBlurConst", g_fPixelBlurConst);

	g_pEffect->SetTexture(g_hCurFrameVelocityTexture, g_pCurFrameVelocityTexture);
	(g_pEffect->SetTexture(g_hLastFrameVelocityTexture, g_pLastFrameVelocityTexture));

	// Clear the velocity render target to 0
	(GETDEVICE->SetRenderTarget(0, g_pCurFrameVelocitySurf));
	(GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0));

	// Clear the full screen render target to the background color
	(GETDEVICE->SetRenderTarget(0, g_pFullScreenRenderTargetSurf));
	(GETDEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_dwBackgroundColor, 1.0f, 0));

	// Turn on Z for this pass
	(GETDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE));

	// For the first pass we'll draw the screen to the full screen render target
	// and to update the velocity render target with the velocity of each pixel
		// Set world drawing technique
		(g_pEffect->SetTechnique(g_hTechWorldWithVelocity));

		// Get the projection & view matrix from the camera class

		GETDEVICE->GetTransform(D3DTS_WORLD, &matW);
		GETDEVICE->GetTransform(D3DTS_VIEW, &matV);
		GETDEVICE->GetTransform(D3DTS_PROJECTION, &matP);
		matVP = matV*matP;

		// For each object, tell the effect about the object's current world matrix
		// and its last frame's world matrix and render the object. 
		// The vertex shader can then use both these matricies to calculate how 
		// much each vertex has moved.  The pixel shader then interpolates this 
		// vertex velocity for each pixel

		for (int iObject = 0; iObject < NUM_OBJECTS; iObject++)
		{
			mWorldViewProjection = g_pScene1Object[iObject]->g_mWorld * matVP;
			mWorldViewProjectionLast = g_pScene1Object[iObject]->g_mWorldLast * g_mViewProjectionLast;

			// Tell the effect where the camera is now
			(g_pEffect->SetMatrix(g_hWorldViewProjection, &mWorldViewProjection));
			(g_pEffect->SetMatrix(g_hWorld, &g_pScene1Object[iObject]->g_mWorld));

			// Tell the effect where the camera was last frame
			(g_pEffect->SetMatrix(g_hWorldViewProjectionLast, &mWorldViewProjectionLast));

			// Tell the effect the current mesh's texture
			(g_pEffect->SetTexture(g_hMeshTexture, g_pScene1Object[iObject]->g_pMeshTexture));

			(g_pEffect->Begin(&cPasses, 0));
			for (iPass = 0; iPass < cPasses; iPass++)
			{
				// Set the render targets here.  If multiple render targets are
				// supported, render target 1 is set to be the velocity surface.
				// If multiple render targets are not supported, the velocity
				// surface will be rendered in the 2nd pass.
				for (int rt = 0; rt < g_nRtUsed; ++rt)
					(GETDEVICE->SetRenderTarget(rt, g_pCurFrameRTSet->pRT[iPass][rt]));

				(g_pEffect->BeginPass(iPass));
				(g_pScene1Object[iObject]->g_pMesh->DrawSubset(0));
				(g_pEffect->EndPass());
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

		// Clear the render target
		(GETDEVICE->Clear(0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L));

		// Above we rendered to a fullscreen render target texture, and now we 
		// render that texture using a quad to apply a pixel shader on every pixel of the scene.
		(g_pEffect->SetTechnique(g_hPostProcessMotionBlur));

		(g_pEffect->Begin(&cPasses, 0));
		for (iPass = 0; iPass < cPasses; iPass++)
		{
			(g_pEffect->BeginPass(iPass));
			(GETDEVICE->SetFVF(SCREEN_VERTEX::FVF));
			(GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Vertex, sizeof(SCREEN_VERTEX)));
			(g_pEffect->EndPass());
		}
		(g_pEffect->End());


		

	
}

void LDYMotionBlur::SetupFullscreenQuad()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DSURFACE_DESC desc;

	g_pFullScreenRenderTargetSurf->GetDesc(&desc);

	// Ensure that we're directly mapping texels to pixels by offset by 0.5
	// For more info see the doc page titled "Directly Mapping Texels to Pixels"
	FLOAT fWidth5 = (FLOAT)rc.right - 0.5f;
	FLOAT fHeight5 = (FLOAT)rc.bottom - 0.5f;

	FLOAT fTexWidth1 = (FLOAT)rc.right / (FLOAT)desc.Width;
	FLOAT fTexHeight1 = (FLOAT)rc.bottom / (FLOAT)desc.Height;

	// Fill in the vertex values
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

	// ���̴� �ε�
	LPD3DXBUFFER		pError = NULL;			//���� ���� ( ���̴��� �������Ҷ� �߸� �� �����̳� ���������� �������ִ� ���� )
	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
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
