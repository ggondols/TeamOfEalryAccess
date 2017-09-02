#include "stdafx.h"
#include "cSceneManager.h"
#include "cGameNode.h"

HRESULT cSceneManager::Setup(void)
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void cSceneManager::Release(void)
{
	mapSceneIter iter = _mSceneList.begin();

	for (; iter != _mSceneList.end();)
	{
		if (iter->second != NULL)
		{
			if (_currentScene == iter->second) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_currentScene = NULL;
	_mSceneList.clear();
}

void cSceneManager::Update(void)
{
	if (_currentScene) _currentScene->Update();
}

void cSceneManager::Render(void)
{
	if (_currentScene) _currentScene->Render();
}

cGameNode * cSceneManager::addScene(string sceneName, cGameNode * scene)
{
	if (!scene) return NULL;
	_mSceneList.insert(make_pair(sceneName, scene));
	return scene;
}

HRESULT cSceneManager::chageScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;

	if (SUCCEEDED(find->second->Setup()))
	{
		cGameNode *temp = _currentScene;

		_currentScene = find->second;

		if (temp)
		{
			temp->Release();
		}

		return S_OK;
	}
	return E_FAIL;
}

DWORD CALLBACK loadingThread(LPVOID prc)
{
	cSceneManager::_readyScene->Setup();


	cSceneManager::_currentScene = cSceneManager::_readyScene;

	cSceneManager::_loadingScene->Release();
	cSceneManager::_loadingScene = NULL;
	cSceneManager::_readyScene = NULL;

	return NULL;
}

cGameNode* cSceneManager::_currentScene = NULL;
cGameNode* cSceneManager::_loadingScene = NULL;
cGameNode* cSceneManager::_readyScene = NULL;



