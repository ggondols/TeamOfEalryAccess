#include "stdafx.h"
#include "cDeviceManager.h"

cDeviceManager::cDeviceManager()
	:m_pD3D(NULL)
	, m_pD3DDevice(NULL)
{
	this->Setup();
}

void cDeviceManager::Setup(void)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 cp;
	int ver;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &cp);

	if (cp.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		ver = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else ver = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS dpp;
	//ZeroMemory(&dpp, sizeof(dpp));
	//dpp.SwapEffect= D3DSWAPEFFECT_DISCARD;
	//dpp.Windowed = FALSE;        ///true���� �ٲ�
	//dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  ///D3DFMT_UNKNOWN���� �ٲ�
	//dpp.EnableAutoDepthStencil = TRUE;
	//dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//dpp.BackBufferWidth = 1920;       ///���Ӱ� �߰���
	//dpp.BackBufferHeight = 1080;		////���Ӱ� �߰���

	ZeroMemory(&dpp, sizeof(dpp));
	dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                      //����
	dpp.Windowed = true;        ///true���� �ٲ�
	dpp.BackBufferFormat = D3DFMT_UNKNOWN;  ///D3DFMT_UNKNOWN���� �ٲ�
	dpp.EnableAutoDepthStencil = TRUE;
	dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	     ///���Ӱ� �߰���
		////���Ӱ� �߰���


	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		ver | D3DCREATE_MULTITHREADED,
		&dpp,
		&m_pD3DDevice);

}


cDeviceManager::~cDeviceManager()
{
	Destroy();
}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

void cDeviceManager::Destroy(void)
{

	SAFE_RELEASE(m_pD3D);
	ULONG ul = m_pD3DDevice->Release(); /*SAFE_RELEASE(m_pD3DDevice);
	assert(ul == 0 && "�������� ���� COM ��ü�� �ֽ��ϴ�.");	*/																											
}
