#include "stdafx.h"
#include "HankcDeferrendRenderer.h"
#include "structs.h"

HankcDeferrendRenderer::HankcDeferrendRenderer()
{
}

HankcDeferrendRenderer::~HankcDeferrendRenderer()
{
}

HRESULT HankcDeferrendRenderer::OneTimeInit(int nWidth, int nHeight, HWND hWnd, bool bWindowed, LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = S_OK;

	V_RETURN(HankRenderer::OneTimeInit(nWidth, nHeight, hWnd, bWindowed, pDevice));

	m_eDebugOutputRT = DO_NORMAL;
	m_DeferredEffectName = "effects/deferred.fx";

	m_bInitialised = true;

	return true;
}

HRESULT HankcDeferrendRenderer::FillGBuffer(Hank::cCamera * pCamera, cSceneInfo * pSceneInfo, LPD3DXEFFECT pEffect)
{
	return E_NOTIMPL;
}

HRESULT HankcDeferrendRenderer::LightAmbient(Hank::cCamera * pCamera, cSceneInfo * pSceneInfo, LPD3DXEFFECT pEffect)
{
	
		HRESULT hr = S_OK;

		D3DXHANDLE hWVP = pEffect->GetParameterBySemantic(NULL, "WorldViewProjection");
		D3DXHANDLE hAmbientLighting = pEffect->GetTechniqueByName("AmbientLighting");
		assert(hAmbientLighting);

		pEffect->SetTechnique(hAmbientLighting);

		pEffect->SetTexture("diffuseRT_Tex", m_GBufferRTs[0].pTex);
		pEffect->SetTexture("positionRT_Tex", m_GBufferRTs[1].pTex);
		pEffect->SetTexture("normalRT_Tex", m_GBufferRTs[2].pTex);

		pEffect->SetVector("g_fvAmbientColour", &pSceneInfo->m_vecAmbientColor);


		UINT uiNumPasses;
		pEffect->Begin(&uiNumPasses, 0);

		for (UINT i = 0; i < uiNumPasses; i++)
		{
			pEffect->BeginPass(i);
			DrawFullScreenQuad(pEffect);
			pEffect->EndPass();
		}
		pEffect->End();

		return S_OK;
	
}

HRESULT HankcDeferrendRenderer::LightDirectionals(Hank::cCamera * pCamera, cSceneInfo * pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow)
{
	HRESULT hr = S_OK;

	D3DXHANDLE hWVP = pDeferred->GetParameterBySemantic(NULL, "WorldViewProjection");
	D3DXHANDLE hPhongDirectionalShadowed = pDeferred->GetTechniqueByName("PhongDirectionalShadowed");
	D3DXHANDLE hCastsShadows = pDeferred->GetParameterByName(NULL, "g_bCastsShadows");

	D3DXMATRIX matProjPersp;
	//pCamera->GetProjMatrix(&matProjPersp);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjPersp);

	D3DXMATRIX matView;
	//pCamera->GetViewMatrix(&matView);
	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matInvView;
	D3DXMatrixInverse(&matInvView, NULL, &matView);

	D3DXHANDLE hLightDirection = pDeferred->GetParameterByName(NULL, "g_fvViewSpaceLightDirection");
	D3DXHANDLE hLightColour = pDeferred->GetParameterByName(NULL, "g_fvLightColour");
	D3DXHANDLE hMatViewToLightProjSpace = pDeferred->GetParameterByName(NULL, "matViewToLightProjSpace");
	D3DXHANDLE hShadowMap = pDeferred->GetParameterByName(NULL, "shadowMapRT_Tex");


	UINT uiLightNum = 0;

	for (list< LightDirectional_s* >::iterator i = pSceneInfo->m_pLightDirectional.begin(); i != pSceneInfo->m_pLightDirectional.end(); i++, uiLightNum++)
	{
		LightDirectional_s* pLight = (*i);

		if (!pLight->m_bActive)
			continue;

		D3DXMATRIX matLightRot;
		D3DXMatrixRotationYawPitchRoll(&matLightRot, pLight->m_fYaw, pLight->m_fPitch, 0.0f);
		D3DXVECTOR3 worldSpaceLightDirection = D3DXVECTOR3(matLightRot._31, matLightRot._32, matLightRot._33);

		if (pLight->m_bCastsShadows)
		{
			D3DXMATRIX matLightViewProj;

		//	V(GenerateDirectionalShadowMatrix(&matLightViewProj, pLight, pSceneInfo->m_sceneFileName));

			// render scene to shadow map RT / Depth surface.  The DrawSceneToShadowMap function takes care of setting
			// the appropriate render target / surfs etc and also restoring the light accum buffer & regular depth buffer.
			if (m_bUse2xFp)
			{
				V(m_pDevice->SetRenderTarget(1, NULL));
			}

			// naive directional light shadow mapping.  Bruteforce it by rendering entire scene into shadow map
		//	V(DrawSceneToShadowMap(pScene, pShadow, matLightViewProj, uiLightNum, false, true));
			// DrawSceneToShadowMap함수 추가 부터

			if (m_bUse2xFp)
			{
				V(m_pDevice->SetRenderTarget(1, m_SceneB.pSurf));
			}

			// set the results of the shadow map rendering as our shadow map texture
			V(pDeferred->SetTexture(hShadowMap, m_ShadowMapRT.pTex));

			// now set the matrix that allows the lighting shader to move from view to lightViewProj space 
			D3DXMATRIX matViewToLightProjSpace;
			matViewToLightProjSpace = matInvView * matLightViewProj * m_matTexScaleBias;
			V(pDeferred->SetMatrix(hMatViewToLightProjSpace, &matViewToLightProjSpace));
		}
		// this stuff is common to whatever technique we use.				
		V(pDeferred->SetTechnique(hPhongDirectionalShadowed));

		D3DXVECTOR3 viewSpaceLightDirection;
		D3DXVec3TransformNormal(&viewSpaceLightDirection, &worldSpaceLightDirection, &matView);

		V(pDeferred->SetBool(hCastsShadows, pLight->m_bCastsShadows));
		V(pDeferred->SetVector(hLightColour, &pLight->m_colour));
		V(pDeferred->SetFloatArray(hLightDirection, (float*)&viewSpaceLightDirection, 3));

		UINT uiNumPasses;
		V(pDeferred->Begin(&uiNumPasses, 0));

		for (UINT i = 0; i < uiNumPasses; i++)
		{
			V(pDeferred->BeginPass(i));
			V(DrawFullScreenQuad(pDeferred));
			V(pDeferred->EndPass());
		}

		V(pDeferred->End());

	}

	return hr;
}

HRESULT HankcDeferrendRenderer::LightOmnis(Hank::cCamera * pCamera, cSceneInfo * pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow)
{
	return E_NOTIMPL;
}

HRESULT HankcDeferrendRenderer::LightSpots(Hank::cCamera * pCamera, cSceneInfo * pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow)
{
	return E_NOTIMPL;
}

HRESULT HankcDeferrendRenderer::BindGBufferAsRTs(bool bBindTexturesAsRT)
{
	return E_NOTIMPL;
}

HRESULT HankcDeferrendRenderer::SetDebugOutputRT(DebugOutputRT eDebugOutputRT)
{
	return E_NOTIMPL;
}
