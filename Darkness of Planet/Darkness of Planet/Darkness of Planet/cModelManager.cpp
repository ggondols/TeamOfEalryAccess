#include "stdafx.h"
#include "cModelManager.h"
#include "cModel.h"

cModelManager::cModelManager(LPDIRECT3DDEVICE9 pDevice)
{
	assert(pDevice && "Invalid Device");
	m_pDevice = pDevice;
}

cModelManager::~cModelManager()
{
	FreeAllResources();
}
void cModelManager::FreeAllResources()
{
	map< string, cModel* >::iterator i = m_modelMap.begin();

	while (i != m_modelMap.end())
	{
		SAFE_DELETE(i->second);
		i++;
	}
	m_modelMap.clear();
}


HRESULT cModelManager::LoadModel(const string& path, const string& name)
{
	HRESULT hr = S_OK;
	 
	// already loaded
	if (GetModel(name) != NULL)
		return S_OK;

	cModel*	pModel = new cModel;
	pModel->LoadModel(path, name, GETDEVICE);

	m_modelMap.insert(pair< string, cModel* >(name, pModel));
	return S_OK;
}


HRESULT	cModelManager::LoadSkybox(const std::string& skyName)
{
	HRESULT hr = S_OK;

	//아직 사용 불가
	/*if (GetModel(kSkyboxFileName) != NULL)
		return S_OK;*/
/*
	cModel* pSkybox = new cModel;
	V_RETURN(pSkybox->LoadModel(kSkyboxFileName, m_pDevice, true, skyName));

	m_modelMap.insert(pair< string, cModel* >(kSkyboxFileName, pSkybox));*/
	return S_OK;
}

cModel* cModelManager::GetModel(const string& name) const
{
	map< string, cModel* >::const_iterator i = m_modelMap.find(name);
	if (i == m_modelMap.end())
		return NULL;
	return i->second;
}
