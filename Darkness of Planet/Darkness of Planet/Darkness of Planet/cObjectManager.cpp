#include "stdafx.h"
#include "cObjectManager.h"

cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{
}

HRESULT cObjectManager::AddObjectToList(LPDIRECT3DTEXTURE9 pTexture, RendererData_s * pRendererData)
{
	HRESULT hr = S_OK;

	map< LPDIRECT3DTEXTURE9, RendererBucket_s* >::iterator i = m_RendererBuckets.find(pTexture);

	// find the texture in the map.  
	if (i == m_RendererBuckets.end())
	{
		// couldn't find it, so create a new render entry 
		RendererBucket_s* pRendererBucket = new RendererBucket_s(pRendererData);
		m_RendererBuckets.insert(make_pair(pTexture, pRendererBucket));
		return S_OK;
	}

	// if we have a valid iterator, then we've already got a bucket with this texture, so append the new data to it
	(i->second)->m_RendererDataList.push_back(pRendererData);

	return S_OK;
}


HRESULT cObjectManager::ClearRenderBuckets()
{
	HRESULT hr = S_OK;

	// delete stuff in list and clear list
	map< LPDIRECT3DTEXTURE9, RendererBucket_s* >::iterator i;

	for (i = m_RendererBuckets.begin(); i != m_RendererBuckets.end(); i++)
	{
		SAFE_DELETE(i->second);
	}
	m_RendererBuckets.clear();

	return S_OK;

}