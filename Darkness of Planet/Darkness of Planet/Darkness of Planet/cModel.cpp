#include "stdafx.h"
#include "cModel.h"





cModel::~cModel()
{


	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pVertexDeclaration);
}

HRESULT cModel::LoadModel(const string& path, const string& name, LPDIRECT3DDEVICE9 pDevice, bool bSkybox)
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

	m_centerPos = m_vMin + m_vMax;	//두개 더해서 반으로 나눔
	m_centerPos = m_centerPos * 0.5; //센터점 찾기
	m_fSizeX = (m_vMax.x - m_vMin.x) / 2;	//사이즈 구함
	m_fSizeY = (m_vMax.y - m_vMin.y) / 2;
	m_fSizeZ = (m_vMax.z - m_vMin.z) / 2;


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

// deprecated
HRESULT cModel::Render(LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr = S_OK;
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, true);




	for (unsigned int i = 0; i < m_mtrls.size(); i++)
	{
		// deprecated
		GETDEVICE->SetMaterial(&m_mtrls[i]);
		GETDEVICE->SetTexture(0, m_pTextures[i]);
		m_pMesh->DrawSubset(i);
	}
	GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);


	/*float radius = FIndMax(m_fSizeX, m_fSizeY, m_fSizeZ);
	CheckRender(m_centerPos, radius);*/
	return hr;
}





void cModel::CheckRender(D3DXVECTOR3 center, float radius)
{



	if (CheckShow(center, radius) == true)
	{



		GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, true);




		for (unsigned int i = 0; i < m_mtrls.size(); i++)
		{
			// deprecated
			GETDEVICE->SetMaterial(&m_mtrls[i]);
			GETDEVICE->SetTexture(0, m_pTextures[i]);
			m_pMesh->DrawSubset(i);
		}
		GETDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		GETDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, false);

		//getAnotherMatrix(m_pRootFrame, NULL);

	}

}

bool cModel::CheckShow(D3DXVECTOR3 center, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float distance;
		distance = (D3DXPlaneDotCoord(&CAMERA->g_Plane[i], &center));
		if (distance - radius >= 0)
			return false;
	}

	return true;
}




float cModel::FIndMax(float x, float y, float z)
{
	if (x > y)
	{
		if (x > z)
		{
			return x;
		}
		else
		{
			return z;
		}

	}
	else
	{
		if (y > z)
		{
			return y;
		}
		else
		{
			return z;
		}
	}


}