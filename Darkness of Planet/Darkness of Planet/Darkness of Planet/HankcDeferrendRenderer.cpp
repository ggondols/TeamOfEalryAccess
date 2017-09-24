#include "stdafx.h"
#include "HankcDeferrendRenderer.h"

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
	return E_NOTIMPL;
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
