#include "stdafx.h"
#include "TeicIceExplosion.h"



TeicIceExplosion::TeicIceExplosion()
{
	m_pParticle = NULL;
	m_pMesh = NULL;
	m_fEndtiming = 2.0f;
}


TeicIceExplosion::~TeicIceExplosion()
{
	SAFE_DELETE(m_pParticle);
}



void TeicIceExplosion::Update()
{
	if (!m_bStart)return;
}

void TeicIceExplosion::Start()
{
	if (m_bStart)return;
	m_pMesh->SetAnimation(0);
	m_bStart = true;
}

void TeicIceExplosion::Stop()
{
	m_bStart = false;
	m_pMesh->SetAnimation(0);
}

void TeicIceExplosion::Render()
{
	if (!m_bStart)return;
	m_pMesh->UpdateAndRender();
}

void TeicIceExplosion::Setup(D3DXVECTOR3 position)
{
	m_pMesh = new TeicEnemy;
	m_pMesh->Setup("sprites/", "IceEffect.X");
	m_pMesh->SetPosition(position);
	m_pMesh->SetScaleSize(0.03);

	m_pParticle = new TeicParticleSystem;
//	m_pParticle->Setup(position,50, m_fEndtiming,0.2,)
}

