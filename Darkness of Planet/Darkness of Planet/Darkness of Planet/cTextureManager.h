#pragma once
#include "cSingletonBase.h"

class cTextureManager : public cSingletonBase<cTextureManager>
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:
	cTextureManager();
	~cTextureManager();

	LPDIRECT3DTEXTURE9 GetTexture(const char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(const string& szFullPath);
	void Destroy();
};

