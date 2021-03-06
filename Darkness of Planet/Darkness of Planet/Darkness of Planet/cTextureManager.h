//////////////////////////////////////////////////////////////////////////////
////////////////////       2017/09/03 여현구     //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////// 텍스쳐매니져 기본적인 텍스쳐와 EX텍스쳐를 보관하고 가져오는 기능   ///////
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "cSingletonBase.h"

class cTextureManager : public cSingletonBase<cTextureManager>
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;
	map<string, D3DXIMAGE_INFO>		m_mapImageInfo;//텍스쳐 EX용 이미지 인포 저장

public:
	cTextureManager();
	~cTextureManager();

	LPDIRECT3DTEXTURE9 GetTexture(IN const char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(IN const string& szFullPath);

	LPDIRECT3DTEXTURE9 GetTextureEx(IN const char* szFullPath, OUT D3DXIMAGE_INFO* pImageInfo); 
	LPDIRECT3DTEXTURE9 GetTextureEx(IN const string& sFullPath, OUT D3DXIMAGE_INFO* pImageInfo);
	void Destroy();
};

