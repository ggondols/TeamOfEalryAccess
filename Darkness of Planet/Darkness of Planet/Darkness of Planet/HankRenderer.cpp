#include "stdafx.h"
#include "HankRenderer.h"

HankRenderer::HankRenderer()
{
}

HankRenderer::~HankRenderer()
{
}

HRESULT HankRenderer::OneTimeInit(int iWidth, int iHeight, HWND hWnd, bool bWindowed, LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = S_OK;

	//--------------------------------------------------------------//
	// Init Vars
	//--------------------------------------------------------------//
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_hWnd = hWnd;
	m_bWindowed = bWindowed;
	m_pDevice = pDevice;

	m_fDepthBias = 0.001f;
	m_fBiasSlope = 0.1f;

	m_bUse2xFp = false;
	m_bUseHDR = false;

	m_fExposure = 0.18f;
	m_iPreviousLuminance = 1;

	m_uiNumVisibleObjects = 0;
	m_uiNumVisibleLights = 0;

	//set special texture matrix for shadow mapping
	float fOffsetX = 0.5f + (0.5f / (float)kiShadowMapDimensions);
	float fOffsetY = 0.5f + (0.5f / (float)kiShadowMapDimensions);

	float fZScale = 1.0f;
	//float fZScale = (float)(1<<24);
	//float fZScale = 16777215;

	m_matTexScaleBias = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fZScale, 0.0f,
		fOffsetX, fOffsetY, 0.0f, 1.0f);

	m_bDebugDisplayShadowMap = false;
	m_bDebugDisplayLightWireframes = false;
	m_bDebugDisplayMeshBoundingWireframes = false;
	m_SharedEffectName = "effects/shared.fx";

	m_pVertexDecFVF0 = NULL;
	m_pVertexDecFVF1 = NULL;
	m_pVertexDecFVF4 = NULL;
	m_pVertexDecFVF8 = NULL;

	m_pQuadVB = NULL;
	m_pQuadVertexDec = NULL;

	m_pBackBufferSurf = NULL;
	m_pZBufferSurf = NULL;

	//--------------------------------------------------------------//
	// Set up FullScreenQuad vertex buffers & formats etc.
	//--------------------------------------------------------------//

	//setup quad matrix to render a fullscreen quad
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXMATRIX matViewProj;

	D3DXVECTOR3 const vEyePt = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 const vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 const   vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// Set World, View, Projection, and combination matrices.
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUp);
	D3DXMatrixOrthoLH(&matProj, 4.0f, 4.0f, 0.2f, 20.0f);
	matViewProj = matView * matProj;
	D3DXMatrixScaling(&matWorld, 2.0f, 2.0f, 1.0f);
	m_matQuad = matWorld * matViewProj;

	return S_OK;
}



HRESULT HankRenderer::LightDirectionals(Hank::cCamera * pCamera, LPD3DXEFFECT pEffect)
{
	return E_NOTIMPL;
}

HRESULT HankRenderer::BindGBufferAsRTs(bool bBindTexturesAsRT)
{
	return E_NOTIMPL;
}

HRESULT HankRenderer::SetDebugOutputRT(DebugOutputRT eDebugOutputRT)
{
	return E_NOTIMPL;
}

//
//HRESULT HankRenderer::DrawFullScreenQuad(LPD3DXEFFECT pEffect)
//{
//	HRESULT hr = S_OK;
//
//	D3DXHANDLE hWVP = pEffect->GetParameterBySemantic(0, "WorldViewProjection");
//	pEffect->SetMatrix(hWVP, &m_matQuad);
//	m_pDevice->SetVertexDeclaration(m_pQuadVertexDec);
//	m_pDevice->SetStreamSource(0, m_pQuadVB, 0, sizeof(posTexVertex));
//	pEffect->CommitChanges();
//	m_pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
//
//	return S_OK;
//}



HRESULT HankRenderer::DrawObjects(Hank::cCamera* pCamera, LPD3DXEFFECT pEffect, bool bDepthOnly)
{
	HRESULT hr = S_OK;
	// get handles to variables that will be constantly changed (faster than retrieving via name)
	//D3DXHANDLE hWorld		= pEffect->GetParameterBySemantic( NULL, "World" );
	D3DXHANDLE hWorldView = pEffect->GetParameterBySemantic(NULL, "WorldView");
	D3DXHANDLE hWVP = pEffect->GetParameterBySemantic(NULL, "WorldViewProjection");

	// Set effect params
	D3DXMATRIX matView, matProjPersp;

	if (!bDepthOnly)
	{
		
		//m_pCamera->GetViewMatrix(&matView);
		//pEffect->SetMatrix("matView", &matView);
		//pCamera->GetProjMatrix(&matProjPersp);
		
		GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
		pEffect->SetMatrix("matView", &matView);		
		GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjPersp);
	}

	D3DXHANDLE hDiffuseMap = pEffect->GetParameterByName(NULL, "base_Tex");
	D3DXHANDLE hNormalMap = pEffect->GetParameterByName(NULL, "normal_Tex");
	D3DXHANDLE hEmissive = pEffect->GetParameterByName(NULL, "g_bEmissive");

	map< LPDIRECT3DTEXTURE9, RendererBucket_s* >::iterator rendBucketItor;

	for (rendBucketItor = m_RendererBuckets.begin(); rendBucketItor != m_RendererBuckets.end(); rendBucketItor++)
	{
		LPDIRECT3DTEXTURE9 pTex = (rendBucketItor->first);
		RendererBucket_s* pBucket = (rendBucketItor->second);

		if (!bDepthOnly)
		{
			pEffect->SetTexture(hDiffuseMap, pTex);
		}

		list< RendererData_s* >::iterator rdIter;
		for (rdIter = pBucket->m_RendererDataList.begin(); rdIter != pBucket->m_RendererDataList.end(); rdIter++)
		{
			RendererData_s* pRD = (*rdIter);

			D3DXMATRIX matWorldView = pRD->m_matWorld * matView;
			D3DXMATRIX matWorldViewProj = matWorldView * matProjPersp;
			pEffect->SetMatrix(hWVP, &matWorldViewProj);

			if (!bDepthOnly)
			{
				pEffect->SetMatrix(hWorldView, &matWorldView);
				pEffect->SetTexture(hNormalMap, pRD->m_normalMap);
				pEffect->SetBool(hEmissive, pRD->m_bEmissive);
			}
			GETDEVICE->SetStreamSource(0, pRD->m_pVB, 0, pRD->m_dwNumBytesPerVertex);
			GETDEVICE->SetIndices(pRD->m_pIB);
			GETDEVICE->SetVertexDeclaration(pRD->m_pVertexDeclaration);

			pEffect->CommitChanges();

			GETDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0,
				pRD->m_attribs.VertexStart,
				pRD->m_attribs.VertexCount,
				pRD->m_attribs.FaceStart * 3,
				pRD->m_attribs.FaceCount);
		}
	}

	return S_OK;
}


HRESULT HankRenderer::DrawFullScreenQuad(LPD3DXEFFECT pEffect)
{
	HRESULT hr = S_OK;

	D3DXHANDLE hWVP = pEffect->GetParameterBySemantic(0, "WorldViewProjection");
	pEffect->SetMatrix(hWVP, &m_matQuad);
	GETDEVICE->SetVertexDeclaration(m_pQuadVertexDec);
	GETDEVICE->SetStreamSource(0, m_pQuadVB, 0, sizeof(ST_PT_VERTEX));
	pEffect->CommitChanges();
	GETDEVICE->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	return S_OK;
}