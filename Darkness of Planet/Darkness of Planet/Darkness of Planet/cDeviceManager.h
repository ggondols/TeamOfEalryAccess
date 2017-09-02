#pragma once

#include "cSingletonBase.h"

//#define g_pDeviceManager cDeviceManager::GetInstance()
//#define g_pD3DDevice cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager : public cSingletonBase <cDeviceManager>
{
	//SINGLETON(cDeviceManager);

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

public:

	cDeviceManager();

	virtual ~cDeviceManager();

	virtual void cDeviceManager::Setup(void);
	virtual LPDIRECT3DDEVICE9 GetDevice();
	virtual void Destroy(void);

};

