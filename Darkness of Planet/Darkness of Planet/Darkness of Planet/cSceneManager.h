#pragma once
#include "cSingleTonbase.h"
#include <map>
#include <string>

class cGameNode;

//============================================
//======= ## 20170902 sceneManager ## ======== 
//============================================

using namespace std;

class cSceneManager : public cSingletonBase <cSceneManager>
{
public:
	typedef map<string, cGameNode*> mapSceneList;
	typedef map<string, cGameNode*>::iterator mapSceneIter;
private:
	static cGameNode* _currentScene;
	static cGameNode* _loadingScene;
	static cGameNode* _readyScene;

	mapSceneList _mSceneList;

	DWORD _loadThreadID;


public:
	HRESULT Setup(void);
	void Release(void);
	void Update(void);
	void Render(void);

	cGameNode* addScene(string sceneName, cGameNode* scene);

	HRESULT chageScene(string sceneName);

	friend DWORD CALLBACK loadingThread(LPVOID prc);



	cSceneManager(void) {};
	~cSceneManager(void) {};
};

