#include "stdafx.h"
#include "Resource.h"
#include "cGameNode.h"

cGameNode::cGameNode()
	:_managerInit(FALSE)
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
		1000);
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
