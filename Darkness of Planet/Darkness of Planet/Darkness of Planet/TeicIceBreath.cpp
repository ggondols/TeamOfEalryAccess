#include "stdafx.h"
#include "TeicIceBreath.h"


TeicIceBreath::TeicIceBreath()
{
	m_pParticle = NULL;
	m_pMesh = NULL;
	m_fEndtiming = 0.0f;
	m_pEffect = NULL;
	m_fAlpha = 1;
	m_fDelta = (0.016667) / 2.0f;
}


TeicIceBreath::~TeicIceBreath()
{
	SAFE_DELETE(m_pParticle);
	SAFE_RELEASE(m_pEffect);
}


void TeicIceBreath::Update()
{
	if (!m_bStart)return;
	m_pParticle->Update2();
	if (TIMEMANAGER->getWorldTime() > m_fEndtiming + 2.0f)
	{
		Stop();
	}
}

void TeicIceBreath::Start()
{
	if (m_bStart)return;
	m_pMesh->SetAnimation(0);
	m_bStart = true;
	m_pParticle->Start();
	m_fEndtiming = TIMEMANAGER->getWorldTime();
	m_fAlpha = 1;
}

void TeicIceBreath::Stop()
{
	m_bStart = false;
	m_pMesh->SetAnimation(0);
	m_pParticle->End();
}

void TeicIceBreath::Render()
{
	if (!m_bStart)return;



	m_fAlpha -= m_fDelta;
	UINT numPasses = 0;
	m_pEffect->SetFloat("vAlpha", m_fAlpha);
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pEffect->CommitChanges();
				m_pMesh->MeshRender(m_pEffect);
			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();
	//m_pMesh->UpdateAndRender();
	m_pParticle->Render();
}

void TeicIceBreath::Setup(D3DXVECTOR3 position)
{
	m_pMesh = new TeicEnemy;
	m_pMesh->Setup("sprites/", "IceEffect.X");
	m_pMesh->SetPosition(position);
	m_pMesh->SetScaleSize(0.03);
	m_pMesh->SetAnimation(0);
	m_pMesh->SetUpdateSpeed(3);
	m_pParticle = new TeicParticleSystem;
	m_pParticle->Setup2(D3DXVECTOR3(position.x, position.y + 10, position.z), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(position.x, position.y + 10, position.z),
		D3DXVECTOR3(20, 20, 20), 5, 0.3f, 0.1, 0, D3DX_PI, 0, D3DX_PI, 0, D3DX_PI,
		1.0f, 0.1f, D3DXVECTOR3(200, 200, 200), D3DXVECTOR3(10, 10, 10), D3DXVECTOR3(250, 250, 250), D3DXVECTOR3(10, 10, 10),
		"sprites/alpha_tex.tga", 50, 1, false);
	m_pEffect = LoadEffectHpp("MultiAnimationEffect.hpp");
	m_hAlphahandle = m_pEffect->GetParameterByName(0, "vAlpha");

	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GETDEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	GETDEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

}

LPD3DXEFFECT TeicIceBreath::LoadEffectHpp(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	GETDEVICE->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(GETDEVICE,
		szFileName,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

