#pragma once
#include "cSingletonBase.h"

class cEffectManager : public cSingletonBase <cEffectManager>
{
private:
	LPDIRECT3DDEVICE9			m_pDevice;
	std::map<std::string, ID3DXEffect*>	m_effectMap;


public:
	cEffectManager(LPDIRECT3DDEVICE9 pDevice = GETDEVICE);
	~cEffectManager();

	void FreeAllResources();

	// Loads an effect file and stores it ready for lookup
	HRESULT LoadEffect(const std::string& name);

	// Finds the effect file by name and returns a pointer to it.  If not found, returns NULL
	LPD3DXEFFECT GetEffect(const std::string& name) const;

	void Setup() {};
};