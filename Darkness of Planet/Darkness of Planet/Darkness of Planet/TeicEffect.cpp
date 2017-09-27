#include "stdafx.h"
#include "TeicEffect.h"




TeicEffect::TeicEffect()
{
	m_fTimeChange = 0.01;
}


TeicEffect::~TeicEffect()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pFlame);
	SAFE_RELEASE(m_pNoise);
	
}


void TeicEffect::Setup(char * Textfilename, float size, int Xnum, int Ynum)
{
	
	m_fSize = size;
	m_iSpriteXnum = Xnum;
	m_iSpriteYnum = Ynum;

	vector<ST_PNT_VERTEX>	m_vecVertex;
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	vecVertex.push_back(D3DXVECTOR3(-size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, -size, -1));
	vecVertex.push_back(D3DXVECTOR3(-size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, size, -1));
	vecVertex.push_back(D3DXVECTOR3(size, -size, -1));

	vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3(0, 0, -1));
	vecNormal.push_back(D3DXVECTOR3(0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3(0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3(0, -1, 0));

	vector<D3DXVECTOR2> vecTex;
	vecTex.push_back(D3DXVECTOR2(0, 1));
	vecTex.push_back(D3DXVECTOR2(0, 0));
	vecTex.push_back(D3DXVECTOR2(1, 0));
	vecTex.push_back(D3DXVECTOR2(0, 1));
	vecTex.push_back(D3DXVECTOR2(1, 0));
	vecTex.push_back(D3DXVECTOR2(1, 1));
	//앞
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	////뒤
	
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR3 n = vecNormal[i / 6];
		D3DXVECTOR2	t = vecTex[i];
		m_vecVertex.push_back(ST_PNT_VERTEX(p, n,t));
	}

	D3DXMATRIX ro;
	D3DXMatrixRotationY(&ro, D3DX_PI/2 /**4.0 / 3.0*/);
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVec3TransformCoord(&p, &p, &ro);
		D3DXVECTOR3 n = vecNormal[i / 6];
		D3DXVECTOR2	t = vecTex[i];
		m_vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
	}
	
	//D3DXMatrixRotationY(&ro, D3DX_PI*2.0 / 3.0);
	//for (size_t i = 0; i < vecIndex.size(); ++i)
	//{
	//	D3DXVECTOR3 p = vecVertex[vecIndex[i]];
	//	D3DXVec3TransformCoord(&p, &p, &ro);
	//	D3DXVECTOR3 n = vecNormal[i / 6];
	//	D3DXVECTOR2	t = vecTex[i];
	//	m_vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
	//}







	D3DXCreateMeshFVF(m_vecVertex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		GETDEVICE,
		&m_pMesh);

	ST_PNT_VERTEX* pV = 0;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = 0;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (int i = 0; i < m_vecVertex.size(); i++)
	{
		pI[i] = i;
	}
	
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (int i = 0; i < m_vecVertex.size()/3; i++)
	{
		if (i < vecIndex.size() / 3.0)
		{
			pA[i] = 0;
		}
		else if (i < vecIndex.size()*2.0 / 3.0)
		{
			pA[i] = 1;
		}
		else 
		{
			pA[i] = 2;
		}
	}

	m_pMesh->UnlockAttributeBuffer();


	vector<DWORD> vecAdj(m_vecVertex.size());
	m_pMesh->GenerateAdjacency(0, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);



	m_pEffect = LoadEffect("Fire2.fx");
	m_pFlame = TEXTUREMANAGER->GetTexture("sprites/Flame.tga");
	m_pNoise = TEXTUREMANAGER->GetTexture("sprites/NoiseVolume.dds");
	hTech = 0;
	hTech = m_pEffect->GetTechniqueByName("Inferno");
	
	GETDEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GETDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GETDEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	GETDEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



}

void TeicEffect::Update()
{
}

void TeicEffect::Render()
{
	
	
	m_fTimeChange += 0.001;


	D3DXMATRIXA16 matWorld,matView, matProjection;
	D3DXMatrixIdentity(&matWorld);


	GETDEVICE->GetTransform(D3DTS_VIEW, &matView);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);






	// 쉐이더 전역변수들을 설정


	//m_pEffect->SetTexture("base_Tex", m_pTexture);

	/*m_pEffect->SetMatrix("matWorld", &matWorld);
	m_pEffect->SetMatrix("matProjection", &matProjection);
	m_pEffect->SetMatrix("matViewProjection", &(matView*matProjection));
	m_pEffect->SetMatrix("matWorldViewProjection", &(matWorld*matView*matProjection));*/
	m_pEffect->SetFloat("time_0_X", 2);
	m_pEffect->SetFloat("Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X", m_fTimeChange);
	m_pEffect->SetFloat("timeSampleDist", m_fTimeChange);
	m_pEffect->SetTexture("Flame_Tex", m_pFlame);
	m_pEffect->SetTexture("Noise_Tex", m_pNoise);
	m_pEffect->SetMatrix("matWorldViewProjectionMatrix", &(matWorld*matView*matProjection));
	m_pEffect->SetTechnique(hTech);
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		
		m_pMesh->DrawSubset(0);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
	numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(1);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
	numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pMesh->DrawSubset(2);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
	
	





}



LPD3DXEFFECT TeicEffect::LoadEffect(const char* szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩
	LPD3DXBUFFER		pError = NULL;			//에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

														//fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(
		GETDEVICE,				// 디바이스
		szFileName,					// 불러올 셰이더 코드 파일이름
		NULL,						// 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,						// 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				// 셰이더 컴파일 플레그
		NULL,						// 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					// 로딩될 셰이더 Effect 포인터
		&pError						// 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
	);

	//셰이더 파일로딩에문재가 있다면..
	if (pError != NULL || pEffect == NULL) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pError->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pError->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pError);
		SAFE_DELETE_ARRAY(str);

		return NULL;
	}

	return pEffect;
}