#include "stdafx.h"
#include "LDYSkinnedMesh_Head.h"
#include "cAllocateHierarchy.h"



LDYSkinnedMesh_Head::LDYSkinnedMesh_Head(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
	, m_fAngle(0.0f)
	, m_bCollision(false)

{
	LDYSkinnedMesh_Head* pSkinnedMesh = SKINMANAGER->GetLDYSkinnedMesh_Head(szFolder, szFilename);

	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;

		D3DXMatrixIdentity(&m_Move);
		m_iCurrentAniNum = 0;
		m_Starttime = 0;
		m_Finishtime = 0;
		m_bBlending = false;
		m_fWeight = 0;
		m_fWeightDivide = 0;

		position = D3DXVECTOR3(0, 0, 0);
		m_Damaging = false;

		m_callback = NULL;
		m_iHp = 0;
		m_iShield = 0;
		m_iAttack = 0;
		m_fZealotdiffer = 0.0f;
		m_iNum = 0;
		D3DXMatrixIdentity(&m_RotationMat);
	
}
LDYSkinnedMesh_Head::LDYSkinnedMesh_Head()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)

{
	D3DXMatrixIdentity(&m_Move);

	m_iNum = 0;
}


LDYSkinnedMesh_Head::~LDYSkinnedMesh_Head()
{
	SAFE_RELEASE(m_pAnimController);
}
void LDYSkinnedMesh_Head::SetNextAni()
{
	LPD3DXANIMATIONSET pAnimset = NULL;

	m_pAnimController->GetAnimationSet(m_iNum, &pAnimset);
	m_pAnimController->SetTrackAnimationSet(m_iCurrentAniNum, pAnimset);

	SAFE_RELEASE(pAnimset);


	m_iNum++;
	if (m_iNum > 95)
		m_iNum = 0;
}

void LDYSkinnedMesh_Head::MeshRender(ST_BONE* pBone, LPD3DXEFFECT effect)
{
	
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		

		
	

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			effect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

		
			

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here

			// set the current number of bones; this tells the effect which shader to use
			effect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			effect->SetTechnique("Skinning20");

			effect->CommitChanges();
			pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
			//UINT uiPasses, uiPass;

			//// run through each pass and draw
			//effect->Begin(&uiPasses, 0);
			//for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			//{
			//	effect->BeginPass(uiPass);
			//	pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
			//	effect->EndPass();
			//}
			//effect->End();
			
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		MeshRender((ST_BONE*)pBone->pFrameSibling,effect);
	}

	if (pBone->pFrameFirstChild)
	{
		MeshRender((ST_BONE*)pBone->pFrameFirstChild,effect);
	}


}

void LDYSkinnedMesh_Head::ShaderMeshRender(LPD3DXEFFECT effect)
{

	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(TIMEMANAGER->getElapsedTime(), NULL);
		Blending();
	}

	if (m_pRootFrame)
	{

		Update(m_pRootFrame, NULL);
		SetupWorldMatrix(m_pRootFrame, &m_matHead);
		MeshRender(m_pRootFrame,effect);
	}

	
}





void LDYSkinnedMesh_Head::SetCallbackfunction(CallbackBindFunction function)
{
	m_callback = std::move(function);
}

void LDYSkinnedMesh_Head::SetAttackCallbackfunction(CallbackBindFunction function)
{
	m_attackCallback = std::move(function);
}

void LDYSkinnedMesh_Head::Load(char* szDirectory, char* szFilename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetFolder(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		GETDEVICE,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
	{
		SetupBoneMatrixPtrs(m_pRootFrame);
	}
}

void LDYSkinnedMesh_Head::UpdateAndRender()
{
	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(TIMEMANAGER->getElapsedTime(), NULL);
		Blending();
	}

	if (m_pRootFrame)
	{

		Update(m_pRootFrame, NULL);
		SetupWorldMatrix(m_pRootFrame, &m_matHead);
		Render(m_pRootFrame);
	}
}

void LDYSkinnedMesh_Head::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
		GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		GETDEVICE->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

LPD3DXEFFECT LDYSkinnedMesh_Head::LoadEffect(char* szFilename)
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
		szFilename,
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

void LDYSkinnedMesh_Head::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}
}

void LDYSkinnedMesh_Head::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void LDYSkinnedMesh_Head::SetAnimationIndex(int nIndex)
{
	if (!m_pAnimController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);
}

void LDYSkinnedMesh_Head::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}

void LDYSkinnedMesh_Head::Blending()
{
	if (!m_bBlending)return;
	int Num = 1 + m_iCurrentAniNum;
	if (Num > 1) Num = 0;
	if (TIMEMANAGER->getWorldTime() <= m_Starttime + 0.3)
	{
		m_fWeight += m_fWeightDivide;
		if (m_fWeight > 1) m_fWeight = 1;
		m_pAnimController->SetTrackWeight(m_iCurrentAniNum, m_fWeight);
		m_pAnimController->SetTrackWeight(Num, 1 - m_fWeight);
	}
	else
	{

		m_pAnimController->SetTrackWeight(m_iCurrentAniNum, 1);
		m_pAnimController->SetTrackWeight(Num, 0);
		if (TIMEMANAGER->getWorldTime() > m_Finishtime)
		{
			m_bBlending = false;
			if (m_callback)
			{
				m_callback();
			}
		}

	}
}

void LDYSkinnedMesh_Head::SetupWorldMatrix(LPD3DXFRAME pFrame, D3DMATRIX * pParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->CombinedTransformationMatrix;

	if (pParent)
	{
		pBone->CombinedTransformationMatrix = (*pParent);
	}

	if (pBone->pFrameSibling)
	{
		SetupWorldMatrix(pBone->pFrameSibling, pParent);
	}
	if (pBone->pFrameFirstChild)
	{
		SetupWorldMatrix(pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}



void LDYSkinnedMesh_Head::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void LDYSkinnedMesh_Head::SetRotationAngle(float angle)
{

	D3DXMatrixRotationY(&m_RotationMat, angle);

}

void LDYSkinnedMesh_Head::SetAnimation(int num)
{
	m_iNum = num;////////////////
	m_Tracknum = num;
	m_Starttime = TIMEMANAGER->getWorldTime();
	m_fWeight = 0;
	m_iCurrentAniNum++;
	if (m_iCurrentAniNum > 1)
		m_iCurrentAniNum = 0;
	LPD3DXANIMATIONSET pAnimset = NULL;

	m_pAnimController->GetAnimationSet(num, &pAnimset);
	m_pAnimController->SetTrackAnimationSet(m_iCurrentAniNum, pAnimset);
	m_pAnimController->SetTrackPosition(m_iCurrentAniNum, 0);



	m_Finishtime = m_Starttime + pAnimset->GetPeriod();
	m_fWeightDivide = TIMEMANAGER->getElapsedTime() / 0.3;
	m_bBlending = true;
	SAFE_RELEASE(pAnimset);
}

void LDYSkinnedMesh_Head::ChangeAnimation(int num)
{
	m_Tracknum = num;
	m_Starttime = TIMEMANAGER->getWorldTime();
	m_fWeight = 0;

	m_iCurrentAniNum++;
	if (m_iCurrentAniNum > 1)
		m_iCurrentAniNum = 0;
	LPD3DXANIMATIONSET pAnimset = NULL;
	m_pAnimController->GetAnimationSet(num, &pAnimset);
	m_pAnimController->SetTrackAnimationSet(m_iCurrentAniNum, pAnimset);
	m_pAnimController->KeyTrackPosition(m_iCurrentAniNum, 0, m_Starttime);

	m_Finishtime = m_Starttime + pAnimset->GetPeriod()*0.8 - m_fZealotdiffer;
	m_Middletime = m_Starttime + pAnimset->GetPeriod() / 2 - 0.3;

	SAFE_RELEASE(pAnimset);
	m_fWeightDivide = TIMEMANAGER->getElapsedTime() / 0.3;
	m_bBlending = true;
}

