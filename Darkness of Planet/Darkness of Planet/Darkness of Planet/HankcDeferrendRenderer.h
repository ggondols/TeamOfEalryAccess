#pragma once
#include "HankRenderer.h"


const unsigned int kiNumRTs = 3;

class HankcDeferrendRenderer : public HankRenderer
{
public:
	enum DebugOutputRT
	{
		DO_MRT0 = 0,
		DO_MRT1,
		DO_MRT2,
		DO_NORMAL,
		DO_TOTAL,
	};
	HankcDeferrendRenderer();
	~HankcDeferrendRenderer();

	//Initialise 
	HRESULT OneTimeInit(int nWidth, int nHeight, HWND hWnd, bool bWindowed, LPDIRECT3DDEVICE9 pDevice);

	// Fill the G-Buffer with scene sttributes. This is run prior to the lighting algorithms
	HRESULT FillGBuffer(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo, LPD3DXEFFECT pEffect);

	// LIGHTING METHODS ////////////////////////////////////////////////////

	// Draws a fullscreen quad that calculates the ambient term for the scene.  Does depth, too.
	HRESULT LightAmbient(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo,  LPD3DXEFFECT pEffect);

	// Additively blends each separate light into the light transport buffer
	HRESULT LightDirectionals(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow);

	// Additively blends each separate light into the light transport buffer
	HRESULT LightOmnis(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow);

	// Additively blends each separate light into the light transport buffer
	HRESULT LightSpots(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo, LPD3DXEFFECT pDeferred, LPD3DXEFFECT pShadow);



	/* Utility function.  Binds or unbinds the g-buffer renderable textures as render targets.
	If choosing to unbind, the function DOES NOT UNBIND RT 0. It is illegal to unbind RT 0.
	The user must manually bind their own desired RT 0 after calling this function */
	HRESULT BindGBufferAsRTs(bool bBindTexturesAsRT);

	/* Takes an enumerated RT value and displays the corresponding GBuffer RT in the viewport.
	E.g. DO_MRT0 will output RT0. DO_NORMAL disables outputting a specific RT and displays a normal image */
	HRESULT SetDebugOutputRT(DebugOutputRT eDebugOutputRT);


private:
	HRESULT DisplayRT(LPD3DXEFFECT pEffect, unsigned int iRT);

	RTT_s							m_GBufferRTs[kiNumRTs];

	// need to be able to generate a cone mesh too for spot lights
	DebugOutputRT					m_eDebugOutputRT;

	std::string						m_DeferredEffectName;
};