#include "stdafx.h"
#include "TeicSkinnedMesh.h"
#include "cAllocateHierarchy.h"



TeicSkinnedMesh::TeicSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
	, m_fAngle(0.0f)

{
	/*TeicSkinnedMesh* pSkinnedMesh = SKINMANAGER->GetSkinnedMesh(szFolder, szFilename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Move);
	m_iCurrentAniNum = 0;
	m_Starttime = 0;
	m_Finishtime = 0;
	m_bBlending = false;
	m_fWeight = 0;
	m_fWeightDivide = 0;

	position = D3DXVECTOR3(0, 0, 0);
	m_Damaging = false;

	m_pAnimController->SetTrackEnable(0, TRUE);
	m_pAnimController->SetTrackEnable(1, TRUE);
	m_pAnimController->SetTrackSpeed(0, 1.0f);
	m_pAnimController->SetTrackSpeed(1, 1.0f);

	LPD3DXANIMATIONSET pAnimset = NULL;
	m_pAnimController->GetAnimationSet(4, &pAnimset);
	m_pAnimController->SetTrackAnimationSet(0, pAnimset);
	m_callback = NULL;
	m_iHp = 0;
	m_iShield = 0;
	m_iAttack = 0;
	m_fZealotdiffer = 0.0f;
	m_iNum = 0;*/
}
TeicSkinnedMesh::TeicSkinnedMesh()
{
}


TeicSkinnedMesh::~TeicSkinnedMesh()
{
}
