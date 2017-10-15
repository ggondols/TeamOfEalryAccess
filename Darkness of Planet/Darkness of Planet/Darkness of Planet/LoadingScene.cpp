#include "stdafx.h"
#include "LoadingScene.h"
#include "Loading.h"
#include "cSoundLoader.h"
#include "cModel.h"
#include "TeicCollisionMove.h"
LoadingScene::LoadingScene()
	: m_pLoading(NULL)

{
	m_vcenter = D3DXVECTOR3(0, 0, 0);
	m_pEffect = NULL;
	m_fMovetime = 0;
	m_fAttacktime = 0;
	m_iNum = 0;
}

LoadingScene::~LoadingScene()
{

}

static DWORD WINAPI threadFunc1(LPVOID lpParam)
{
	LoadingScene* temp = (LoadingScene*)lpParam;
	EnterCriticalSection(&temp->cs);
	while (temp->m_bIsthreading)
	{

		temp->m_pLoading->Update();
		temp->m_iNum++;
		if (!temp->m_pLoading->LoadNext())
		{
			temp->m_bIsthreading = false;
		}

	}
	LeaveCriticalSection(&temp->cs);
	return NULL;


}


void LoadingScene::CallbacOn(int n)
{
	m_vecEnemyCollisionMove[n]->SetClear();
	TeicCollisionMove* tempmove;
	tempmove = new TeicCollisionMove;
	tempmove->SetSkinnedTarget(m_pEnemy[n]->GetSkinnedMesh());
	tempmove->SetSpeed(RND->getFromFloatTo(5, 10));
	tempmove->SetFrom2(D3DXVECTOR3(10 + RND->getFromFloatTo(-5, 5), 10 + RND->getFromFloatTo(-5, 5), 0));
	tempmove->SetTo2(D3DXVECTOR3(-10 + RND->getFromFloatTo(-5, 5), -10 + RND->getFromFloatTo(-5, 5), 0));
	m_vecEnemyCollisionMove[n]->AddAction(tempmove);
	m_vecEnemyCollisionMove[n]->Start();

}

HRESULT LoadingScene::Setup()
{
	SOUNDMANAGER->addSound("Taja", "Sound/BGM/", "Taja.mp3", 0, 0);
	LoadModel("object/xFile/SpaceShip/", "spaceship.X", GETDEVICE, false);
	m_pCamera = new Hank::cCamera;

	m_pCamera->Setup(&m_vcenter);
	m_pLoading = new Loading;
	m_pLoading->Setup();
	m_bIsthreading = true;
	thread1 = NULL;
	dwThread1 = 1;
	InitializeCriticalSection(&cs);



	unsigned long ulStackSize = 0;

	thread1 = CreateThread(NULL, ulStackSize, threadFunc1, this, CREATE_SUSPENDED, &dwThread1);
	ResumeThread(thread1);







	D3DXVECTOR3 temp = D3DXVECTOR3(200, 0, -100);
	/*char str[256];
	sprintf_s(str, "S(%d,%d,%d)L(0,0,0)", 200, 0, -100);
	m_pLoading->LoadWay2(str, NODEMANAGER->GetNode(), temp, D3DXVECTOR3(0, 0, 0));*/
	/*for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			char str[256];
			sprintf_s(str, "SX%dSZ%dLX%dLZ%d", 26+j,45+i, 116, 46);
			m_pLoading->LoadWay(str, NODEMANAGER->GetNode(), 26 + j, 45 + i, 116, 46);

		}
	}*/


	//m_pLoading->LoadMesh("", MESH_NORMAL, "object/xFile/", "MP5_Aim_Anims/HeroBodyLv",i,"_MP5.X");

	m_pLoading->LoadMesh("ice_FallRock.X", MESH_TEI, "sprites/", "ice_FallRock.X");
	m_pLoading->LoadMesh("ice_Boom.X", MESH_TEI, "sprites/", "ice_Boom.X");
	m_pLoading->LoadMesh("IceEffect.X", MESH_TEI, "sprites/", "IceEffect.X");
	m_pLoading->LoadMesh("wolf.X", MESH_TEI, "object/xFile/wolf/", "wolf.X");
	m_pLoading->LoadMesh("tiger.X", MESH_TEI, "object/xFile/tiger/", "tiger.X");
	m_pLoading->LoadMesh("ArgoniteGiant.X", MESH_TEI, "object/xFile/ArgoniteGiant/", "ArgoniteGiant.X");
	m_pLoading->LoadMesh("Valak.X", MESH_TEI, "object/xFile/Valak/", "Valak.X");
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv1.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv1.X");
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv2.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv2.X");
	m_pLoading->LoadMesh("HeroHead/HeroHeadLv4.X", MESH_HEAD, "object/xFile/", "HeroHead/HeroHeadLv4.X");
	for (int i = 1; i < 5; i++)
	{
		char str[128];
		sprintf_s(str, "MP5_Aim_Anims/HeroBodyLv%d_MP5.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "Pistol_Aim_Anims/HeroBodyLv%d_Pistol.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "Player_Melee/HeroBodyLv%d_Player_Melee.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "PR_Base_Anims/HeroBodyLv%d_Base.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "PR_IdleBreak_Anims/HeroBodyLv%d_IdleBreak.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);
		sprintf_s(str, "SMG_Aim_Anims/HeroBodyLv%d_SMG.X", i);
		m_pLoading->LoadMesh(str, MESH_NORMAL, "object/xFile/", str);

	}
	m_pLoading->LoadMesh("Weapon/Wp_AA12.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_AA12.X");
	m_pLoading->LoadMesh("Weapon/Wp_AR6.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_AR6.X");
	m_pLoading->LoadMesh("Weapon/Wp_M4.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_M4.X");
	m_pLoading->LoadMesh("Weapon/Wp_MP5.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_MP5.X");
	m_pLoading->LoadMesh("Weapon/Wp_Pistol.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_Pistol.X");
	m_pLoading->LoadMesh("Weapon/Wp_FireGun.X", MESH_WEAPON, "object/xFile/", "Weapon/Wp_FireGun.X");


	//m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");
	m_pLoading->LoadHeightMap("terrain", "map/", "final5.raw", "final5.png");

	//m_pLoading->LoadHeightMap("terrain", "map/", "HeightMap.raw", "terrain.jpg");

	m_pLoading->LoadTestResource("test", 10, 10);
	m_pLoading->LoadTestResource("test", 10, 10);

	//soudn Loading
	cSoundLoader ls;
	ls.LoadSound();
	for (int i = 0; i < ls._wholeSound.size(); i++)
	{
		m_pLoading->LoadSound(ls._wholeSound[i]);
	}


	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursor.png", &m_cursortex);
	D3DXCreateTextureFromFile(GETDEVICE, "UI/cursor/cursorHit.png", &m_cursortex2);
	m_cursortex->GetSurfaceLevel(0, &surfcursor);
	GETDEVICE->SetCursorProperties(0, 0, surfcursor);


	SKILLEFFECTMANAGER->AddEffect("sample", sample, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 2);




	m_pEffect = LoadEffect("sprites/Space.fx");
	m_pSpace = TEXTUREMANAGER->GetTexture("sprites/galaxy.bmp");
	m_stVertex[0].p = D3DXVECTOR3(-20, -20, 5);
	m_stVertex[1].p = D3DXVECTOR3(-20, 20, 5);
	m_stVertex[2].p = D3DXVECTOR3(20, 20, 5);
	m_stVertex[3].p = D3DXVECTOR3(20, 20, 5);
	m_stVertex[4].p = D3DXVECTOR3(20, -20, 5);
	m_stVertex[5].p = D3DXVECTOR3(-20, -20, 5);
	m_stVertex[0].t = D3DXVECTOR2(0, 1);
	m_stVertex[1].t = D3DXVECTOR2(0, 0);
	m_stVertex[2].t = D3DXVECTOR2(1, 0);
	m_stVertex[3].t = D3DXVECTOR2(1, 0);
	m_stVertex[4].t = D3DXVECTOR2(1, 1);
	m_stVertex[5].t = D3DXVECTOR2(0, 1);
	for (int i = 0; i < 10; i++)
	{
		m_pEnemy[i] = new TeicEnemy;
		m_pEnemy[i]->Setup("sprites/", "Meteo.X");
		m_pEnemy[i]->SetScaleSize(0.005);
		//m_pEnemy[i]->SetAnimation(0);
		m_pEnemy[i]->SetPosition(D3DXVECTOR3(10 + RND->getFromFloatTo(-5, 5), 10 + RND->getFromFloatTo(-5, 5), 0));
		m_pEnemy[i]->SetUpdateSpeed(1);


		m_vecEnemyCollisionMove[i] = new TeicMoveSequence;
		TeicCollisionMove* tempmove;
		tempmove = new TeicCollisionMove;
		tempmove->SetSkinnedTarget(m_pEnemy[i]->GetSkinnedMesh());
		tempmove->SetSpeed(RND->getFromFloatTo(5, 10));
		tempmove->SetFrom2(m_pEnemy[i]->GetPosition());
		tempmove->SetTo2(D3DXVECTOR3(-10 + RND->getFromFloatTo(-5, 5), -10 + RND->getFromFloatTo(-5, 5), 0));
		m_vecEnemyCollisionMove[i]->AddAction(tempmove);
		//m_vecEnemyCollisionMove[i]->Start();
		m_vecEnemyCollisionMove[i]->SetCallback(bind(&LoadingScene::CallbacOn, this, i));
	
	}
	
	return S_OK;
}

void LoadingScene::Release()
{
	m_pLoading->Release();
	SAFE_DELETE(m_pLoading);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pSpace);
	SAFE_RELEASE(m_pMesh);
	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE(m_pEnemy[i]);
		SAFE_DELETE(m_vecEnemyCollisionMove[i]);
	}
	

	for (int i = 0; i < m_pTextures.size(); i++)
	{
		SAFE_RELEASE(m_pTextures[i]);
	}
	for (int i = 0; i < m_pNormalTextures.size(); i++)
	{
		SAFE_RELEASE(m_pNormalTextures[i]);
	}
	SAFE_RELEASE(m_pVertexDeclaration);
	




}

void LoadingScene::Update()
{
	m_pLoading->UpdateStory();

	if (m_iNum == 30)
	{
		for (int i = 0; i < 10; i++)
		{
			m_vecEnemyCollisionMove[i]->Start();
		}
		m_pCamera->m_bTwist = true;
	}

	/*if (TIMEMANAGER->getWorldTime() > m_fAttacktime + 10.0f)
	{
		m_fAttacktime = INF;
		for (int i = 0; i < 10; i++)
		{
			m_vecEnemyCollisionMove[i]->Start();
		}
	}*/
	for (int i = 0; i < 10; i++)
	{
		m_vecEnemyCollisionMove[i]->Update();
	}
	m_pCamera->Update();
	SKILLEFFECTMANAGER->play("sample",D3DXVECTOR3(-2.05+ 1.5,0.45,0), D3DXVECTOR3(0, 0, 0));
	SKILLEFFECTMANAGER->play("sample", D3DXVECTOR3(-2.05+ 1.5, -0.45, 0), D3DXVECTOR3(0, 0, 0));
	SKILLEFFECTMANAGER->Update();
	if (!m_bIsthreading)
	{
		SCENEMANAGER->changeScene("DarknessofPlanetMainScene");
	}
}

void LoadingScene::Render()
{
	for (int i = 0; i < 10; i++)
	{
		m_pEnemy[i]->UpdateAndRender();
	}
	
	m_fMovetime += 0.001;
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pLoading->Render();
	SKILLEFFECTMANAGER->Render();
	D3DXMATRIX world,temp;
	D3DXMatrixScaling(&temp, 0.002, 0.002, 0.002);
	world = temp;
	D3DXMatrixRotationY(&temp, -D3DX_PI / 2);
	world *= temp;
	D3DXMatrixTranslation(&temp, 1.5, 0, 0);
	world *= temp;
	GETDEVICE->SetTransform(D3DTS_WORLD, &world);

	D3DXMATRIX world2, view, proj;
	D3DXMatrixIdentity(&world2);
	GETDEVICE->GetTransform(D3DTS_VIEW, &view);
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &proj);
	m_pEffect->SetFloat("deltatime", m_fMovetime);
	m_pEffect->SetMatrix("matWorldVIewProj", &(world2*view*proj));
	
	m_pEffect->SetTexture("base_Tex",m_pSpace);
	UINT numPasses = 0;
	m_pEffect->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);
			{
				m_pEffect->CommitChanges();
				GETDEVICE->SetFVF(ST_PT_VERTEX::FVF);
				GETDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_stVertex[0], sizeof(ST_PT_VERTEX));

			}
			m_pEffect->EndPass();
		}
	}
	m_pEffect->End();



	for (unsigned int i = 0; i < m_mtrls.size(); i++)
	{
		// deprecated
		GETDEVICE->SetMaterial(&m_mtrls[i]);
		GETDEVICE->SetTexture(0, m_pTextures[i]);
		m_pMesh->DrawSubset(i);
	}
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}





HRESULT LoadingScene::LoadModel(const string& path, const string& name, LPDIRECT3DDEVICE9 pDevice, bool bSkybox)
{
	HRESULT hr = S_OK;

	if (m_pMesh)
	{
		return S_OK;
	}

	// get the directory path.  
	string pathName = path + name;
	m_name = name;
	//
	// Load the XFile data.
	//


	ID3DXBuffer* pAdjBuffer = 0;
	ID3DXBuffer* pMtrlBuffer = 0;
	DWORD        numMtrls = 0;

	V_RETURN(D3DXLoadMeshFromX(
		pathName.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		&pAdjBuffer,
		&pMtrlBuffer,
		0,
		&numMtrls,
		&m_pMesh));

	m_mtrls.resize(numMtrls);
	m_pTextures.resize(numMtrls);
	m_pNormalTextures.resize(numMtrls);

	//
	// Extract the materials, load textures.
	//

	if (pMtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

		for (int i = 0; i < (int)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			m_mtrls[i] = mtrls[i].MatD3D;

			// check if the ith material has an associative texture
			if (mtrls[i].pTextureFilename != 0)
			{
				// the filename of each texture is relative to the mesh by default, so add on the pathname 

				// if it's a skybox model then we want to rename the textures to skyname_originalFilename.extension
				// so we can make one skybox model and swap the materials easily without messing with modelling packages etc.
				string textureFileName = mtrls[i].pTextureFilename;

				if (bSkybox)
				{
					//	textureFileName = string(skyName + "_" + textureFileName);
				}

				D3DXIMAGE_INFO tempInfo;

				string diffuseTexName = path + textureFileName;
				//TEXTUREMANAGER->GetTextureEx(diffuseTexName, &tempInfo);
				LPDIRECT3DTEXTURE9 pDiffuseTex = TEXTUREMANAGER->GetTextureEx(diffuseTexName, &tempInfo);

				// find out what the file extension is.  Assume that all file extensions are .xyz where xyz is some arbitrary combination of 3 letters
				string extension = diffuseTexName.substr(diffuseTexName.length() - 4, 4);

				// remove the diffuse file extension and add the normal string + extension (assume extension is of the same type for normal)
				// note that I'm using "_bumpmap" rather than "_normal" solely because the textures in the D3D / NVidia SDK folders use this extension. 
				// and it's easier to just use it as is than rename everything.

				string normalTexName = diffuseTexName.substr(0, diffuseTexName.length() - 4) + "_bumpmap" + extension;

				// don't bother double checking this.  If it's flagged as an error, it'll show up in the log anyway.  
				// Not all meshes have normal maps so it's not really an error, just a means of checking whether we should use normal mapping shaders
				//TEXTUREMANAGER->GetTextureEx(normalTexName);
				LPDIRECT3DTEXTURE9 pNormalTex = TEXTUREMANAGER->GetTextureEx(normalTexName, &tempInfo);

				m_bHasNormals = pNormalTex ? true : false;

				if (m_bHasNormals)	// if there's a normal map, check to see whether it has four channels.  If it does, then it has an emissive bit
				{
					LPDIRECT3DSURFACE9 pSurf = NULL;
					V(pNormalTex->GetSurfaceLevel(0, &pSurf));
					D3DSURFACE_DESC desc;
					V(pSurf->GetDesc(&desc));

					if (desc.Format == D3DFMT_A8R8G8B8 || desc.Format == D3DFMT_DXT2 || desc.Format == D3DFMT_DXT3 || desc.Format == D3DFMT_DXT4 || desc.Format == D3DFMT_DXT5)
					{
						m_bHasEmissive = true;
					}
					else
					{
						m_bHasEmissive = false;
					}

					SAFE_RELEASE(pSurf);
				}
				else
				{
					m_bHasEmissive = false;
				}

				// save the loaded texture
				m_pTextures[i] = pDiffuseTex;
				m_pNormalTextures[i] = pNormalTex;
			}
			else
			{
				// no texture for the ith subset
				m_pTextures[i] = NULL;
			}
		}
	}
	SAFE_RELEASE(pMtrlBuffer); // done w/ buffer

							   // Clone the mesh using the normal mapping vertex declaration.  Many D3DX meshes don't have binormals/tangents so we must generate them
	LPD3DXMESH pNewMesh = NULL;
	V(m_pMesh->CloneMesh(D3DXMESH_MANAGED, elementsNormalMapping, pDevice, &pNewMesh));
	SAFE_RELEASE(m_pMesh);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pAdjBuffer);
		SAFE_RELEASE(pNewMesh);
		return hr;
	}

	m_pMesh = pNewMesh;

	// bonkers function with a zillion parameters.  Doing it this way ensures we get the right result.  I tried to clone & clean the mesh
	// previously, but it didn't work in a lot of cases.  Using computeTangentFrameEx seems to work exactly as desired
	V(D3DXComputeTangentFrameEx(m_pMesh, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_BINORMAL, 0, D3DDECLUSAGE_TANGENT, 0, D3DDECLUSAGE_NORMAL,
		0, 0, (DWORD*)pAdjBuffer->GetBufferPointer(), 0.01f, 0.25f, 0.01f, &pNewMesh, NULL));

	SAFE_RELEASE(m_pMesh);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pAdjBuffer);
		SAFE_RELEASE(pNewMesh);
		return hr;
	}

	m_pMesh = pNewMesh;

	// double check vertex format, then store it with the mesh.
	D3DVERTEXELEMENT9 vertexDeclaration[MAX_FVF_DECL_SIZE];

	V(m_pMesh->GetDeclaration(vertexDeclaration));
	V(pDevice->CreateVertexDeclaration(vertexDeclaration, &m_pVertexDeclaration));

	if (FAILED(hr))
	{
		SAFE_RELEASE(m_pMesh);
		SAFE_RELEASE(pAdjBuffer);
		return hr;
	}

	V(m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjBuffer->GetBufferPointer(),
		0, 0, 0));

	SAFE_RELEASE(pAdjBuffer); // done w/ buffer

							  //여기부터 수정하세요
							  /*
							  //바운드 박스 메이커
							  V_RETURN(m_ObjectSpaceBS.ComputeFromMesh(m_pMesh));

							  CAABB MeshBB;
							  V_RETURN(MeshBB.ComputeFromMesh(m_pMesh));

							  MeshBB.GetVertices((D3DXVECTOR3*)&m_BoundingBoxVertices[0]);

							  */
							  //충돌체 만들기
	BYTE* v = 0;
	m_pMesh->LockVertexBuffer(0, (void**)&v);
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_pMesh->GetDeclaration(decl);
	UINT vertSize = D3DXGetDeclVertexSize(decl, 0);


	D3DXComputeBoundingBox((D3DXVECTOR3*)v,
		m_pMesh->GetNumVertices(),
		vertSize,
		&m_vMin,
		&m_vMax);

	m_pMesh->UnlockVertexBuffer();




	//이밑은 아님
	/*
	for( UINT i = 0; i < 8; i++ )
	{
	g_Log.Write( string( "Vertex ") + toString(i) + string(" X: ") + toString( m_BoundingBoxVertices[i].x )
	+ string(" Y: ") +toString( m_BoundingBoxVertices[i].y)  + string(" Z: ") +toString( m_BoundingBoxVertices[i].z ) );
	}
	*/

	return hr;
}



LPD3DXEFFECT LoadingScene::LoadEffect(const char * szFileName)
{
	LPD3DXEFFECT pEffect = NULL;

	// 셰이더 로딩

	LPD3DXBUFFER      pError = NULL;         //에러 버퍼 ( 셰이더를 컴파일할때 잘못 된 문법이나 오류정보를 리턴해주는 버퍼 )
	DWORD            dwShaderFlag = 0;      //셰이더 플레그 0 

#ifdef _DEBUG

	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;      //셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif
														 //fx 파일로 부터 셰이더 객체 생성
	D3DXCreateEffectFromFile(

		GETDEVICE,            // 디바이스
		szFileName,               // 불러올 셰이더 코드 파일이름
		NULL,                  // 셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL,                  // 셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,            // 셰이더 컴파일 플레그
		NULL,                  // 셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,               // 로딩될 셰이더 Effect 포인터
		&pError                  // 셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
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
