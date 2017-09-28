#include "stdafx.h"
#include "TeicParticleSystem.h"


TeicParticleSystem::TeicParticleSystem()
{
}


TeicParticleSystem::~TeicParticleSystem()
{
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}
void TeicParticleSystem::Setup(D3DXVECTOR3 center, float Maxparticle, float lifespan, float lifespanVariance,
	float startsize, float startsizeVariance, float finishsize, float finishsizevariance,
	float emitterangle, float anglevariance, float startrot, float startrotvar, float endrot,
	float endrotvar, float speed, float speedvar, D3DXVECTOR3 startcolor, int startcolorvar,
	D3DXVECTOR3 endcolor, int endcolorvar,
	char* texture, float dotsize, float radius, bool loop)
{
	m_vecVertex.resize(Maxparticle);
	m_vecParticle.resize(Maxparticle);

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		float fRadius = rand() % 100 / (100 / radius);

		m_vecVertex[i].p = D3DXVECTOR3(0, 0, fRadius);


		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX matRX, matRY, matRZ, matT, matWorld;
		D3DXMatrixTranslation(&matT, center.x, center.y, center.z);
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ *matT;

		D3DXVec3TransformCoord(
			&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&matWorld);

		//	m_vecVertex[i].c = startcolor;
		m_vecParticle[i] = new TeicParticle;
		D3DXVECTOR3  startColV = D3DXVECTOR3(startcolor.x + RND->getFromIntTo(-startcolorvar, startcolorvar),
			startcolor.y + RND->getFromIntTo(-startcolorvar, startcolorvar),
			startcolor.z + RND->getFromIntTo(-startcolorvar, startcolorvar));

		D3DXVECTOR3  endColV = D3DXVECTOR3(endcolor.x + RND->getFromIntTo(-endcolorvar, endcolorvar),
			endcolor.y + RND->getFromIntTo(-endcolorvar, endcolorvar),
			endcolor.z + RND->getFromIntTo(-endcolorvar, endcolorvar));
		m_vecParticle[i]->Setup(&m_vecVertex[i], lifespan + RND->getFromFloatTo(-lifespanVariance, lifespanVariance),

			1, INF, emitterangle + RND->getFromFloatTo(-anglevariance, anglevariance), startrot + RND->getFromFloatTo(-startrotvar, startrotvar), endrot + RND->getFromFloatTo(-endrotvar, endrotvar)
			, speed + RND->getFromFloatTo(-speedvar, speedvar), startColV, endColV, loop);
		//m_vecParticle[i]->m_pVertex = &m_vecVertex[i];
		m_vecParticle[i]->Start();
	}
	m_text = texture;
	GETDEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 사이즈 설정
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDw(startsize + RND->getFromFloatTo(-startsizeVariance, startsizeVariance)));

	// 포인트 스케일링 Factor값 설정
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// 포인트 최소 크기
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// 포인트 최대 크기
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(1000.0f));

	// 포인트에 텍스쳐를 입힐 수 있게 해줌
	GETDEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	GETDEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

void TeicParticleSystem::Setup2(D3DXVECTOR3 start, D3DXVECTOR3 startVar, D3DXVECTOR3 end, D3DXVECTOR3 endVar, 
	float Maxparticle, float lifespan, float lifespanVariance, float angleX, float angleXvar, 
	float angleY, float angleYvar, float angleZ, float angleZvar, float speed, float speedvar, 
	D3DXVECTOR3 startcolor, D3DXVECTOR3 startcolorvar, D3DXVECTOR3 endcolor, D3DXVECTOR3 endcolorvar,
	char * texture, float dotsize, float dotsizevar, bool loop)
{
	m_vecVertex.resize(Maxparticle);
	m_vecParticle.resize(Maxparticle);

	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		

		m_vecVertex[i].p = D3DXVECTOR3(start.x+RND->getFromFloatTo(-startVar.x, startVar.x), 
			start.y+ RND->getFromFloatTo(-startVar.y, startVar.y), 
			start.z+ RND->getFromFloatTo(-startVar.z, startVar.z));


	

		

		//	m_vecVertex[i].c = startcolor;
		m_vecParticle[i] = new TeicParticle;
		D3DXVECTOR3  startColV = D3DXVECTOR3(startcolor.x + RND->getFromIntTo(-startcolorvar.x, startcolorvar.x),
			startcolor.y + RND->getFromIntTo(-startcolorvar.y, startcolorvar.y),
			startcolor.z + RND->getFromIntTo(-startcolorvar.z, startcolorvar.z));

		D3DXVECTOR3  endColV = D3DXVECTOR3(endcolor.x + RND->getFromIntTo(-endcolorvar.x, endcolorvar.x),
			endcolor.y + RND->getFromIntTo(-endcolorvar.y, endcolorvar.y),
			endcolor.z + RND->getFromIntTo(-endcolorvar.z, endcolorvar.z));
		m_vecParticle[i]->Setup2(&m_vecVertex[i], lifespan + RND->getFromFloatTo(-lifespanVariance, lifespanVariance),
			D3DXVECTOR3(end.x + RND->getFromFloatTo(-endVar.x, endVar.x),
				end.y + RND->getFromFloatTo(-endVar.y, endVar.y),
				end.z + RND->getFromFloatTo(-endVar.z, endVar.z)), angleX + RND->getFromFloatTo(-angleXvar, angleXvar),
			angleY + RND->getFromFloatTo(-angleYvar, angleYvar), angleX + RND->getFromFloatTo(-angleZvar, angleZvar),
			speed + RND->getFromFloatTo(-speedvar, speedvar), startColV, endColV, loop);
		
		m_vecParticle[i]->Start();
	}
	m_text = texture;
	GETDEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 사이즈 설정
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDw(dotsize + RND->getFromFloatTo(-dotsizevar, dotsizevar)));

	// 포인트 스케일링 Factor값 설정
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(1.0f));
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(1.0f));
	GETDEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// 포인트 최소 크기
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// 포인트 최대 크기
	GETDEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(1000.0f));

	// 포인트에 텍스쳐를 입힐 수 있게 해줌
	GETDEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GETDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	GETDEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);



	
}



void TeicParticleSystem::Update()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		m_vecParticle[i]->Update();
	}
}

void TeicParticleSystem::Update2()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		m_vecParticle[i]->Update2();
	}
}

void TeicParticleSystem::Update3()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		m_vecParticle[i]->Update3();
	}
}

void TeicParticleSystem::Render()
{
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);

	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);


	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	GETDEVICE->SetTexture(0, NULL);
	GETDEVICE->SetFVF(ST_PC_VERTEX::FVF);

	GETDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);

	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture(m_text));

	GETDEVICE->DrawPrimitiveUP(D3DPT_POINTLIST,
		m_vecVertex.size(),
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	GETDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void TeicParticleSystem::Release()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		delete m_vecParticle[i];
	}
}

void TeicParticleSystem::Start()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		m_vecParticle[i]->Start();
	}
}

void TeicParticleSystem::End()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		m_vecParticle[i]->End();
	}
}
