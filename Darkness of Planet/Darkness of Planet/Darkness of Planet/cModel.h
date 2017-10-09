#pragma once
#include <string>

const D3DVERTEXELEMENT9 elementsNormalMapping[] =
{
	{ 0, sizeof(float) * 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,		0 },
	{ 0, sizeof(float) * 3, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,		0 },
	{ 0, sizeof(float) * 5, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0 },
	{ 0, sizeof(float) * 8, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,		0 },
	{ 0, sizeof(float) * 11, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,		0 },
	D3DDECL_END()
};


class cModel
{
public:
	LPD3DXMESH						m_pMesh;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;

	bool							m_bHasNormals;
	bool							m_bHasEmissive;
	std::string						m_name;

	// Object space bounding sphere.  This must be transformed to world space by the parenting node for accurate collision detection
	//CBoundingSphere					m_ObjectSpaceBS;
	std::vector< D3DXVECTOR3 >		m_BoundingBoxVertices;

	std::vector< D3DMATERIAL9 >			m_mtrls;
	std::vector< LPDIRECT3DTEXTURE9 >	m_pTextures;
	std::vector< LPDIRECT3DTEXTURE9 >	m_pNormalTextures;

public:
	//Model infomation for bounding box
	D3DXVECTOR3 m_vMin, m_vMax; 
	D3DXVECTOR3 m_centerPos;
	float		m_fSizeX, m_fSizeY, m_fSizeZ;

public:
	cModel() : m_pMesh(NULL), m_pVertexDeclaration(NULL), m_bHasNormals(false), m_bHasEmissive(false)
	{
		m_mtrls.clear();
		m_pTextures.clear();
		m_pNormalTextures.clear();
		//m_ObjectSpaceBS.Empty();
		m_BoundingBoxVertices.resize(8);
	}

	~cModel();
public:
	
	HRESULT LoadModel(const string& path, const string & name, LPDIRECT3DDEVICE9 pDevice, bool bSkybox = false);
	HRESULT cModel::Render(LPDIRECT3DDEVICE9 pDevice);

	LPD3DXMESH GetMesh(void) { return m_pMesh; }
};


