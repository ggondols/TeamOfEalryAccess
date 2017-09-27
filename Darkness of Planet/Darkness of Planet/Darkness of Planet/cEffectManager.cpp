#include "stdafx.h"
#include "cEffectManager.h"

cEffectManager::cEffectManager(LPDIRECT3DDEVICE9 pDevice)
{
	assert(pDevice && "Invalid Device");
	m_pDevice = pDevice;
}

cEffectManager::~cEffectManager()
{
	FreeAllResources();
}

void cEffectManager::FreeAllResources()
{
	map< string, ID3DXEffect* >::iterator i = m_effectMap.begin();
	while (i != m_effectMap.end())
	{
		SAFE_RELEASE(i->second);
		i++;
	}
	m_effectMap.clear();
}


HRESULT cEffectManager::LoadEffect(const string& name)
{
	HRESULT hr = S_OK;

	// check to see whether it's already loaded
	if (GetEffect(name) != NULL)
		return S_OK;

	// it ain't there, so load it
	ID3DXEffect* pEffect = NULL;
	ID3DXBuffer* pErrors = NULL;

//#if defined( DEBUG_PS )
//	V_RETURN(D3DXCreateEffectFromFile(m_pDevice, name.c_str(), 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 0, &pEffect, &pErrors));
//#else
//	hr = D3DXCreateEffectFromFile(m_pDevice, name.c_str(), 0, 0, 0, 0, &pEffect, &pErrors);
//	//D3DERR_INVALIDCALL
//	//V_RETURN( D3DXCreateEffectFromFile( m_pDevice, name.c_str(), 0, 0, 0, 0, &pEffect, &pErrors ));
//#endif
//

	hr = D3DXCreateEffectFromFile(m_pDevice, name.c_str(), 0, 0, 0, 0, &pEffect, &pErrors);

	//셰이더 파일로딩에문재가 있다면..
	if (hr != S_OK) {

		//문제의 내용이 뭔지 문자열로 확인
		int size = pErrors->GetBufferSize();
		char* str = new char[size];

		//str에 버퍼에있는 내용을 저장한다.
		sprintf_s(str, size, (const char*)pErrors->GetBufferPointer());

		OutputDebugString(str);
		//오류내용을 출력했으니 오류버퍼 해제
		SAFE_RELEASE(pErrors);
		SAFE_DELETE_ARRAY(str);

		return hr;
	}

	m_effectMap.insert(pair< string, ID3DXEffect* >(name, pEffect));
	return hr;
}
LPD3DXEFFECT cEffectManager::GetEffect(const string& name) const
{
	map< string, ID3DXEffect* >::const_iterator i = m_effectMap.find(name);
	if (i == m_effectMap.end())
		return NULL;
	return i->second;
}

