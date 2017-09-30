#include "stdafx.h"
#include "TeicIceblizzard.h"
#include "cHeightMap.h"

TeicIceblizzard::TeicIceblizzard()
{
	m_pParticle = NULL;
	for (int i = 0; i < 20; i++)
	{
		m_pMesh[i] = NULL;
	}

	m_fEndtiming = 0.0f;
	m_pEffect = NULL;
	m_fAlpha = 1;
	m_fDelta = (0.016667) / 2.0f;
}


TeicIceblizzard::~TeicIceblizzard()
{
	SAFE_DELETE(m_pParticle);
	SAFE_RELEASE(m_pEffect);
}

void TeicIceblizzard::Update()
{
	if (!m_bStart)return;
	m_pParticle->Update4();
	for (int i = 0; i < 20; i++)
	{
		if (!m_bOn[i])continue;
		if (m_pMesh[i]->GetPosition().y > m_fMeshHeight[i])
		{
			m_pMesh[i]->GetPositionPointer()->y -= 1* m_fSpeed[i];
		}
		else
		{
			if (m_pMesh[i]->GetAninum() != 0)
			{
				m_pMesh[i]->SetAnimation(0);
			}
		}
		

	}
	if (TIMEMANAGER->getWorldTime() > m_fEndtiming + 2.0f)
	{
		Stop();
	}
}

void TeicIceblizzard::Start()
{
	if (m_bStart)return;
	for (int i = 0; i < 20; i++)
	{

		m_pMesh[i]->SetAnimation(1);
		m_pMesh[i]->SetUpdateSpeed(1);
	}

	m_bStart = true;
	m_pParticle->Start();
	m_fEndtiming = TIMEMANAGER->getWorldTime();
	m_fAlpha = 1;
}

void TeicIceblizzard::Render()
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
				for (int j = 0; j < 20; j++)
				{
					if (m_fAlpha < 0.04*(20 - j) + 0.2)
					{
						m_bOn[j] = true;
						m_pMesh[j]->MeshRender(m_pEffect);
					}
				}

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();
	//m_pMesh->UpdateAndRender();
	m_pParticle->Render();
}

void TeicIceblizzard::Stop()
{

	m_bStart = false;
	for (int i = 0; i < 20; i++)
	{
		m_pMesh[i]->SetAnimation(1);
	}
	m_pParticle->End();
}

void TeicIceblizzard::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	m_pHeightmap = HEIGHTMAPMANAGER->GetHeightMap("terrain");
	for (int i = 0; i < 20; i++)
	{
		m_pMesh[i] = new TeicEnemy;
		m_pMesh[i]->Setup("sprites/", "ice_FallRock.X");
		m_pMesh[i]->SetScaleSize(RND->getFromFloatTo(0.05, 0.1));
		m_pMesh[i]->SetAnimation(1);
		m_pMesh[i]->SetRotationAngle(RND->getFloat(D3DX_PI * 2));
		m_pMesh[i]->SetCallbackfunction(bind(&TeicIceblizzard::Callbackon, this, i));

	}
	float dist = 100;
	
	for (int i = 0; i < 20; i++)
	{
		float angle = RND->getFloat(D3DX_PI * 2);
		float x = position.x +cosf(angle) * RND->getFromFloatTo(0, dist);
		float z = position.z -sinf(angle) * RND->getFromFloatTo(0, dist);
		m_pMesh[i]->SetPosition(D3DXVECTOR3(x,position.y +60,z));
		m_pHeightmap->GetHeight(x, m_fMeshHeight[i], z);
		m_bOn[i] = false;
		m_fSpeed[i] = RND->getFromFloatTo(2, 3);
	}



	m_pParticle = new TeicParticleSystem;
	m_pParticle->Setup2(D3DXVECTOR3(position.x, position.y + 60, position.z), D3DXVECTOR3(70, 1, 70),
		D3DXVECTOR3(characterpos.x, characterpos.y , characterpos.z), D3DXVECTOR3(100, 1, 100)
		, 80, 1.7f, 0.3, 0, 0, 0, 0, 0, 0,
		50, 30.0f, D3DXVECTOR3(200, 200, 200), D3DXVECTOR3(10, 10, 10), D3DXVECTOR3(250, 250, 250), D3DXVECTOR3(10, 10, 10),
		"sprites/snow.tga", 40, 1, false);
	m_pEffect = LoadEffectHpp("MultiAnimationEffect.hpp");
	

	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GETDEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	GETDEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}



void TeicIceblizzard::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	for (int i = 0; i < 20; i++)
	{
		m_pMesh[i]->SetScaleSize(RND->getFromFloatTo(0.05, 0.1));
		m_pMesh[i]->SetAnimation(1);
		m_pMesh[i]->SetRotationAngle(RND->getFloat(D3DX_PI * 2));
	}
	float dist = 100;

	for (int i = 0; i < 20; i++)
	{
		float angle = RND->getFloat(D3DX_PI * 2);
		float x = position.x + cosf(angle) * RND->getFromFloatTo(0, dist);
		float z = position.z - sinf(angle) * RND->getFromFloatTo(0, dist);
		m_pMesh[i]->SetPosition(D3DXVECTOR3(x, position.y + 60, z));
		m_pHeightmap->GetHeight(x, m_fMeshHeight[i], z);
		m_bOn[i] = false;
		m_fSpeed[i] = RND->getFromFloatTo(2, 3);
	}



	m_pParticle->SetPosition(D3DXVECTOR3(position.x, position.y + 60, position.z), D3DXVECTOR3(characterpos.x, characterpos.y , characterpos.z));

}





bool TeicIceblizzard::IsRunning()
{
	return m_bStart;
}



LPD3DXEFFECT TeicIceblizzard::LoadEffectHpp(const char * szFileName)
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

void TeicIceblizzard::Callbackon(int n)
{
	//m_pMesh[n]->SetAnimation(1);

}