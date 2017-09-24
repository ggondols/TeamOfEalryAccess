#pragma once
#include "HankcCamera.h"
#include "cGameNode.h"

const unsigned int kiShadowMapDimensions = 512;
const unsigned int MAX_LUM_TEX = 3;
const unsigned int MAX_SAMPLES = 16;

// ####### NVIDIA STUFF #########
const unsigned int LUMINANCE_DOWNSAMPLE_LOG = 0;
const unsigned int LUMINANCE_DOWNSAMPLE = 1;
const unsigned int LUMINANCE_DOWNSAMPLE_EXP = 2;




// Render Queue stuff to enable (crude) batching
struct RendererData_s
{
	RendererData_s() :
		m_pVB(NULL),
		m_pIB(NULL),
		m_pVertexDeclaration(NULL) {}

	RendererData_s(LPD3DXMATRIX pMatWorld, LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DINDEXBUFFER9 pIB,
		DWORD dwNumBytesPerVertex, LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration,
		LPD3DXATTRIBUTERANGE pAttrib, LPDIRECT3DTEXTURE9 pNormalTex, bool bEmissive)
		: m_matWorld(*pMatWorld),
		m_pVB(pVB),
		m_pIB(pIB),
		m_dwNumBytesPerVertex(dwNumBytesPerVertex),
		m_pVertexDeclaration(pVertexDeclaration),
		m_attribs(*pAttrib),
		m_normalMap(pNormalTex),
		m_bEmissive(bEmissive) { }

	~RendererData_s()
	{
		//SAFE_RELEASE( m_pVB ); 
		//SAFE_RELEASE( m_pIB );
	}

	D3DXMATRIX						m_matWorld;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	DWORD							m_dwNumBytesPerVertex;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;
	D3DXATTRIBUTERANGE				m_attribs;
	LPDIRECT3DTEXTURE9				m_normalMap;
	bool							m_bEmissive;
};

struct RendererBucket_s
{
	RendererBucket_s(RendererData_s* pRendererData)
	{
		m_RendererDataList.clear();
		m_RendererDataList.push_back(pRendererData);
	}

	~RendererBucket_s()
	{
		for (std::list< RendererData_s* >::iterator i = m_RendererDataList.begin(); i != m_RendererDataList.end(); i++)
			SAFE_DELETE((*i));
	}

	std::list< RendererData_s* >		m_RendererDataList;
};

enum DebugOutputRT
{
	DO_MRT0 = 0,
	DO_MRT1,
	DO_MRT2,
	DO_NORMAL,
	DO_TOTAL,
};

// RTT structure represents a D3D Texture & its surface.
struct RTT_s
{
	// constructors
	RTT_s() : pTex(NULL),
		pSurf(NULL) { }

	RTT_s(LPDIRECT3DTEXTURE9 pRT, LPDIRECT3DSURFACE9 pRTSurf) : pTex(pRT),
		pSurf(pRTSurf) {}

	// destructor
	~RTT_s()
	{
		SAFE_RELEASE(pTex);
		SAFE_RELEASE(pSurf);
	}

	// for statically allocated objects, call this before program ends
	void Release()
	{
		SAFE_RELEASE(pTex);
		SAFE_RELEASE(pSurf);
	}

	LPDIRECT3DTEXTURE9	pTex;
	LPDIRECT3DSURFACE9	pSurf;
};


class HankRenderer
{
public:
	HankRenderer();
	
	//#######################
	// VIRTUAL
	//#######################
	virtual ~HankRenderer();


	// Initialiss the Renderer.
	virtual HRESULT OneTimeInit(int iWidth, int iHeight,
		HWND hWnd, bool bWindowed, LPDIRECT3DDEVICE9 pDevice) PURE;

	// Cleans up resources
	virtual HRESULT Cleanup() PURE;

	// Renders the Scene
	virtual HRESULT Render(Hank::cCamera* pCamera,  float fTimeDelta) PURE;


	// Draws a fullscreen quad that calculates the ambient term for the scene.  Does depth, too.
	HRESULT LightAmbient(Hank::cCamera* pCamera, cSceneInfo* pSceneInfo, LPD3DXEFFECT pEffect);


	// Additively blends each separate light into the light transport buffer
	HRESULT LightDirectionals(Hank::cCamera* pCamera, LPD3DXEFFECT pEffect);


	/* Utility function.  Binds or unbinds the g-buffer renderable textures as render targets.
	If choosing to unbind, the function DOES NOT UNBIND RT 0. It is illegal to unbind RT 0.
	The user must manually bind their own desired RT 0 after calling this function */
	HRESULT BindGBufferAsRTs(bool bBindTexturesAsRT);

	//Utility
	/* Takes an enumerated RT value and displays the corresponding GBuffer RT in the viewport.
	E.g. DO_MRT0 will output RT0. DO_NORMAL disables outputting a specific RT and displays a normal image */
	HRESULT SetDebugOutputRT(DebugOutputRT eDebugOutputRT);

	HRESULT DrawObjects(Hank::cCamera * pCamera, LPD3DXEFFECT pEffect, bool bDepthOnly);

	HRESULT DrawFullScreenQuad(LPD3DXEFFECT pEffect);

	//HRESULT DrawFullScreenQuad(LPD3DXEFFECT pEffect);

protected:
	IDirect3DDevice9*	m_pDevice;
	Hank::cCamera*			m_pCamera;

	// status vars
	bool			m_bInitialised;

	int				m_iWidth;
	int				m_iHeight;
	HWND			m_hWnd;
	bool			m_bWindowed;

	// Other RT stuff	

	// shadow mapping stuff
	float			m_fDepthBias;
	float			m_fBiasSlope;

	RTT_s			m_ShadowMapRT;
	RTT_s			m_ShadowMapColourRT;

	D3DXMATRIX		m_matTexScaleBias;

	bool			m_bDebugDisplayShadowMap;
	bool			m_bDebugDisplayLightWireframes;
	bool			m_bDebugDisplayMeshBoundingWireframes;

	std::string		m_SharedEffectName;

	// HDR stuff
	bool			m_bUseHDR;
	// batching stuff
	std::map< LPDIRECT3DTEXTURE9, RendererBucket_s* >	m_RendererBuckets;

	// Sphere mesh for omni point lights
	ID3DXMesh* 						m_pSphereMesh;
	IDirect3DVertexDeclaration9*	m_pSphereMeshVertexDeclaration;

	// TEMP #####################################################
	IDirect3DVertexBuffer9*			m_pQuadVB;

	D3DXMATRIX	m_matQuad;

	// Vertex decs
	IDirect3DVertexDeclaration9*	m_pQuadVertexDec;
	IDirect3DVertexDeclaration9*	m_pVertexDecFVF0;
	IDirect3DVertexDeclaration9*	m_pVertexDecFVF1;
	IDirect3DVertexDeclaration9*	m_pVertexDecFVF4;
	IDirect3DVertexDeclaration9*	m_pVertexDecFVF8;


	// ########## NVIDIA HDR STUFF ###############
	// Flags	
	bool					m_bUse2xFp;
	float                   m_fExposure;
	int                     m_iPreviousLuminance;

	int                     m_iCroppedWidth;
	int                     m_iCroppedHeight;
	int                     m_iShrinkWidth;
	int                     m_iShrinkHeight;

	UINT					m_uiNumVisibleObjects;
	UINT					m_uiNumVisibleLights;

	// back buffer & z buffer surfaces
	IDirect3DSurface9*	m_pBackBufferSurf;
	IDirect3DSurface9*	m_pZBufferSurf;

		// single render targets for 1xfp16 
	RTT_s					m_fpSceneF; 
	RTT_s					m_fp16ShrinkF;
	RTT_s					m_fp16ShrinkF_1;
	RTT_s					m_fp16ShrinkF_2;

	// Render targets and associated surfaces for 2xFP16 path
	RTT_s					m_SceneA;
	RTT_s					m_SceneB;
    	
	RTT_s					m_fp16ShrinkA;
	RTT_s					m_fp16ShrinkB;

	RTT_s					m_fp16ShrinkA_1;
	RTT_s					m_fp16ShrinkA_2;

	RTT_s					m_fp16Lum[MAX_LUM_TEX];
	RTT_s					m_fp32Lum[3];				// The 1x1 texture to use VTF for the luminance
	
	// Art assets
	//HDRTexture*             m_pHDREnvMap;
	//HDRTexture*             m_pHDRDifMap;
	//Skybox*                 m_pSkybox;
	//Model3D*                m_pModel3D;

	// Global variables for the sampling on downscale and blurring
	float                   m_gaussWeights[MAX_SAMPLES];
};


