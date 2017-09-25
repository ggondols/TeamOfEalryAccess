#pragma once

#define NUM_OBJECTS 40
#define NUM_WALLS 250
#define MOVESTYLE_DEFAULT 0

class LDYCharacter;

struct SCREEN_VERTEX
{
	D3DXVECTOR4 pos;
	DWORD clr;
	D3DXVECTOR2 tex1;

	static const DWORD FVF;
};
const DWORD                 SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct CRenderTargetSet
{
	IDirect3DSurface9* pRT[2][2];  // Two passes, two RTs
};

class LDYMotionBlur
{

private:

	ID3DXEffect*                g_pEffect = NULL;       // D3DX effect interface
	D3DFORMAT                   g_VelocityTexFormat;    // Texture format for velocity textures
	SCREEN_VERTEX				g_Vertex[4];

	LPDIRECT3DTEXTURE9          g_pFullScreenRenderTarget;
	LPDIRECT3DSURFACE9          g_pFullScreenRenderTargetSurf;

	LPDIRECT3DTEXTURE9          g_pPixelVelocityTexture1;
	LPDIRECT3DSURFACE9          g_pPixelVelocitySurf1;
	LPDIRECT3DTEXTURE9          g_pPixelVelocityTexture2;
	LPDIRECT3DSURFACE9          g_pPixelVelocitySurf2;

	LPDIRECT3DTEXTURE9          g_pLastFrameVelocityTexture;
	LPDIRECT3DSURFACE9          g_pLastFrameVelocitySurf;
	LPDIRECT3DTEXTURE9          g_pCurFrameVelocityTexture;
	LPDIRECT3DSURFACE9          g_pCurFrameVelocitySurf;

	FLOAT                       g_fChangeTime;
	bool                        g_bShowBlurFactor;
	bool                        g_bShowUnblurred;
	DWORD                       g_dwBackgroundColor;

	float                       g_fPixelBlurConst;
	float                       g_fObjectSpeed;
	float                       g_fCameraSpeed;

	DWORD                       g_dwMoveSytle;
	int                         g_nSleepTime;
	D3DXMATRIX                  g_mViewProjectionLast;
	int                         g_nCurrentScene;

	D3DXHANDLE                  g_hWorld;
	D3DXHANDLE                  g_hWorldLast;
	D3DXHANDLE                  g_hMeshTexture;
	D3DXHANDLE                  g_hWorldViewProjection;
	D3DXHANDLE                  g_hWorldViewProjectionLast;
	D3DXHANDLE                  g_hCurFrameVelocityTexture;
	D3DXHANDLE                  g_hLastFrameVelocityTexture;
	D3DXHANDLE                  g_hTechWorldWithVelocity;
	D3DXHANDLE                  g_hPostProcessMotionBlur;

	int                         g_nPasses = 0;          // Number of passes required to render
	int                         g_nRtUsed = 0;          // Number of render targets used by each pass
	CRenderTargetSet			g_aRTSet[2];            // Two sets of render targets
	CRenderTargetSet*           g_pCurFrameRTSet;      // Render target set for current frame
	CRenderTargetSet*           g_pLastFrameRTSet;     // Render target set for last frame

public:
	LDYMotionBlur();
	~LDYMotionBlur();

	void Setup();
	void Update();
	void Render();

	LPD3DXEFFECT LoadEffect(const char* szFileName);
};

