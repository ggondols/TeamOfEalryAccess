#include "stdafx.h"
#include "TeicMonsterChill.h"


TeicMonsterChill::TeicMonsterChill()
{
	m_pParticle = NULL;
	
	m_bStart = false;
	m_fEndtiming = 0.0f;
	
}


TeicMonsterChill::~TeicMonsterChill()
{
	SAFE_DELETE(m_pParticle);
}

void TeicMonsterChill::Update()
{
	if (!m_bStart)return;

	m_pParticle->Update5();
	if (TIMEMANAGER->getWorldTime() > m_fEndtiming + 0.4f)
	{
		Stop();
	}
}

void TeicMonsterChill::Start()
{
	if (m_bStart)return;
	

	m_bStart = true;
	m_pParticle->Start2();
	m_fEndtiming = TIMEMANAGER->getWorldTime();

}

void TeicMonsterChill::Stop()
{
	m_bStart = false;
	
	m_pParticle->End();
}

void TeicMonsterChill::Render()
{
	if (!m_bStart)return;
	m_pParticle->Render();

}

void TeicMonsterChill::Setup(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{
	
	
	m_pParticle = new TeicParticleSystem;
	m_pParticle->Setup2(D3DXVECTOR3(position.x, position.y , position.z), D3DXVECTOR3(1, 1, 1),
		D3DXVECTOR3(characterpos.x, characterpos.y + 5, characterpos.z), D3DXVECTOR3(1, 1, 1)
		, 10, 0.5, 0.1, 0, D3DX_PI/2, 0, 0, 0, D3DX_PI/2,
		3.0f, 1.0f, D3DXVECTOR3(0, 0, 200), D3DXVECTOR3(50, 50, 50), D3DXVECTOR3(250, 250, 250), D3DXVECTOR3(10, 10, 10),
		"sprites/smoke.tga", 7, 0.5, false);
	

	
}

void TeicMonsterChill::Callbackon(int n)
{
	

}

void TeicMonsterChill::SetPosition(D3DXVECTOR3 position, D3DXVECTOR3 characterpos)
{

	

	m_pParticle->SetPosition(D3DXVECTOR3(position.x, position.y , position.z), D3DXVECTOR3(characterpos.x, characterpos.y + 10, characterpos.z));


}

bool TeicMonsterChill::IsRunning()
{
	return m_bStart;
}
