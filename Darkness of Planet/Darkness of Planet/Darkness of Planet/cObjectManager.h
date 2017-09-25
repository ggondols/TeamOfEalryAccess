#pragma once
#include "cSingletonBase.h"
// Render Queue stuff to enable (crude) batching
struct RendererData_s
{
	RendererData_s() :
		m_pVB(NULL),
		m_pIB(NULL),
		m_pVertexDeclaration(NULL) {}

	RendererData_s(LPD3DXMATRIX pMatWorld, LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DINDEXBUFFER9 pIB,
		DWORD dwNumBytesPerVertex, LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration,
		LPD3DXATTRIBUTERANGE pAttrib, LPDIRECT3DTEXTURE9 pNormalTex, bool bEmissive)
		: m_matWorld(*pMatWorld),
		m_pVB(pVB),
		m_pIB(pIB),
		m_dwNumBytesPerVertex(dwNumBytesPerVertex),
		m_pVertexDeclaration(pVertexDeclaration),
		m_attribs(*pAttrib),
		m_normalMap(pNormalTex),
		m_bEmissive(bEmissive) { }

	~RendererData_s()
	{
		//SAFE_RELEASE( m_pVB ); 
		//SAFE_RELEASE( m_pIB );
	}

	D3DXMATRIX						m_matWorld;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	DWORD							m_dwNumBytesPerVertex;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;
	D3DXATTRIBUTERANGE				m_attribs;
	LPDIRECT3DTEXTURE9				m_normalMap;
	bool							m_bEmissive;
};

struct RendererBucket_s
{
	RendererBucket_s(RendererData_s* pRendererData)
	{
		m_RendererDataList.clear();
		m_RendererDataList.push_back(pRendererData);
	}

	~RendererBucket_s()
	{
		for (std::list< RendererData_s* >::iterator i = m_RendererDataList.begin(); i != m_RendererDataList.end(); i++)
			SAFE_DELETE((*i));
	}

	std::list< RendererData_s* >		m_RendererDataList;
};

class cObjectManager : public cSingletonBase <cObjectManager>
{
private:
	std::map<string, RendererData_s>					m_staticMesh;
	std::map<string, RendererData_s>					m_dinamicMesh;
	std::map< LPDIRECT3DTEXTURE9, RendererBucket_s* >	m_RendererBuckets;
	


public:

	cObjectManager();
	~cObjectManager();
	
	HRESULT AddObjectToList(LPDIRECT3DTEXTURE9 pTexture, RendererData_s* pRendererData);

	HRESULT ClearRenderBuckets();

};