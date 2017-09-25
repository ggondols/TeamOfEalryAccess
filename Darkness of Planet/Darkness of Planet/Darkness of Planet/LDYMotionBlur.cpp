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
