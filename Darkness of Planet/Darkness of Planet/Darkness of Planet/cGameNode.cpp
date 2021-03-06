#include "stdafx.h"
#include "Resource.h"
#include "cGameNode.h"

cGameNode::cGameNode()
	: _managerInit(FALSE)
	, m_pSceneInfo(NULL)
{
}

cGameNode::~cGameNode()
{

}

HRESULT cGameNode::Setup()
{
	return S_OK;
}


HRESULT cGameNode::Setup(bool managerInit)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	_managerInit = managerInit;

	//여기에 메니저 셋업
	if (_managerInit)
	{
		//DEVICEMANAGER->Setup();
		SCENEMANAGER->Setup();
		TIMEMANAGER->Setup();
		KEYMANAGER->Setup();
		MATERIALMANAGER->Setup();
		RND->Setup();
		UIOBJECTMANAGER->Setup();
		HEIGHTMAPMANAGER->Setup();
		WAYMANAGER->Setup();
		NODEMANAGER->Setup(210);
		DATABASE->Setup();
		MESHLOADER->Setup();
		SOUNDMANAGER->Setup();
		EFFECTMANAGER->Setup();
		MODELMANAGER->Setup();
		SKILLEFFECTMANAGER->Setup();
		CAMERA->Setup();
	}

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0, 3, -5),
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 1, 0));
	GETDEVICE->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom,
		1,
		3000);
	GETDEVICE->SetTransform(D3DTS_PROJECTION, &matProj);

	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

void cGameNode::Release()
{

	//메니저 릴리즈
	if (_managerInit)
	{
		DEVICEMANAGER->Destroy();
		DEVICEMANAGER->releaseSingleton();
	
		SCENEMANAGER->Release();
		SCENEMANAGER->releaseSingleton();

		TIMEMANAGER->Release();
		TIMEMANAGER->releaseSingleton();
		
		KEYMANAGER->Release();
		KEYMANAGER->releaseSingleton();

		MATERIALMANAGER->Release();
		MATERIALMANAGER->releaseSingleton();

		RND->Release();
		RND->releaseSingleton();

		TEXTUREMANAGER->Destroy();
		TEXTUREMANAGER->releaseSingleton();
		
		FONTMANAGER->Destroy();
		FONTMANAGER->releaseSingleton();

		UIOBJECTMANAGER->Destroy();
		UIOBJECTMANAGER->releaseSingleton();

		//HEIGHTMAPMANAGER->Destroy();  맵정보를 받아오는 곳에서 delete하면 안해줘도된다.
		HEIGHTMAPMANAGER->releaseSingleton();

		WAYMANAGER->Destroy();
		WAYMANAGER->releaseSingleton();

		NODEMANAGER->Destroy();
		NODEMANAGER->releaseSingleton();

		DATABASE->Destroy();
		DATABASE->releaseSingleton();

		MESHLOADER->Destroy();
		MESHLOADER->releaseSingleton();

		SOUNDMANAGER->Release();
		SOUNDMANAGER->releaseSingleton();

		EFFECTMANAGER->FreeAllResources();
		EFFECTMANAGER->releaseSingleton();

		MODELMANAGER->FreeAllResources();
		MODELMANAGER->releaseSingleton();

		SKILLEFFECTMANAGER->Destroy();
		SKILLEFFECTMANAGER->releaseSingleton();
		CAMERA->~LDYCamera();
		CAMERA->releaseSingleton();
	}
}

void cGameNode::Update()
{
}

void cGameNode::Render()
{
}

LRESULT cGameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
	{
		g_isLButtonDown = true;
		g_isLButtonUp = false;		
		
	}
	break;
	case WM_RBUTTONDOWN:
	{
		g_isRButtonDown = true;
		g_isRButtonUp = false;
		
	}
	break;
	case WM_LBUTTONUP:
	{
		g_isLButtonDown = false;
		g_isLButtonUp = true;
	}
	break;
	case WM_RBUTTONUP:
	{
		g_isRButtonDown = false;
		g_isRButtonUp = true;
	}
	break;
	case WM_MOUSEMOVE:
	{
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		g_MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
	}
	break;
	case WM_SETCURSOR:
	{
		SetCursor(NULL);
		GETDEVICE->ShowCursor(g_bCursor);
		return true;
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
			/*case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;*/
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
		}
	}
	break;
	//case WM_PAINT:
	//{
	//	PAINTSTRUCT ps;
	//	HDC hdc = BeginPaint(hWnd, &ps);
	//	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
	//	EndPaint(hWnd, &ps);
	//}
	//break;
	case WM_KEYDOWN:
		GETLPARAM = wParam;
		switch (wParam)
		{
		case VK_ESCAPE: //esc키로 종료한다
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	return LRESULT();
}
