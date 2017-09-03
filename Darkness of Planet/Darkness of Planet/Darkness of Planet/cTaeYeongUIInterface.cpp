#include "stdafx.h"
#include "cTaeYeongUIInterface.h"


cTaeYeongUIInterface::cTaeYeongUIInterface()
	:m_pSprite(NULL)
	, m_pTexture(NULL)
	, m_Type(NOTHING)
	, m_Drag(false)
	, m_pTarget(NULL)
	, m_typeTarget(NOTHING)
	, m_bClose(false)
{
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Local);
	m_Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	m_Rc = RectMake(0, 0, 0, 0);
	ZeroMemory(str, sizeof(str));
}


cTaeYeongUIInterface::~cTaeYeongUIInterface()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE(m_pTarget);
}



void cTaeYeongUIInterface::Setup(char * szFolder, char * szFilename, float Startx, float Starty, UItype type, D3DXMATRIX& Local)
{

	D3DXCreateSprite(GETDEVICE, &m_pSprite);
	char str[128];
	sprintf_s(str, "%s/%s", szFolder, szFilename);
	D3DXCreateTextureFromFileEx(
		GETDEVICE,
		str,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stImageInfo,
		NULL,
		&m_pTexture);
	m_Type = type;
	m_startX = Startx;
	m_startY = Starty;
	D3DXMatrixTranslation(&m_BasicTranslation, Startx, Starty, 0);
	m_Local = Local;
	SetRect(&m_Rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);



	if (m_Type == TEXT)
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 11;
		fd.Width = 11;
		fd.Weight = FW_NORMAL;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		//AddFontResource("umberto.ttf");
		strcpy_s(fd.FaceName, "1");	//글꼴 스타일
		D3DXCreateFontIndirect(GETDEVICE, &fd, &m_pFont);
	}
}
void cTaeYeongUIInterface::Setup(char * szFolder, char * szFilename, float Startx, float Starty, UItype type)
{

	D3DXCreateSprite(GETDEVICE, &m_pSprite);
	char str[128];
	sprintf_s(str, "%s/%s", szFolder, szFilename);
	D3DXCreateTextureFromFileEx(
		GETDEVICE,
		str,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stImageInfo,
		NULL,
		&m_pTexture);
	m_Type = type;
	m_startX = Startx;
	m_startY = Starty;
	D3DXMatrixTranslation(&m_BasicTranslation, Startx, Starty, 0);
	SetRect(&m_Rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	if (m_Type == TEXT)
	{
		D3DXFONT_DESC fd;
		ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
		fd.Height = 11;
		fd.Width = 11;
		fd.Weight = FW_NORMAL;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		//AddFontResource("umberto.ttf");
		strcpy_s(fd.FaceName, "1");	//글꼴 스타일
		D3DXCreateFontIndirect(GETDEVICE, &fd, &m_pFont);
	}
}
void cTaeYeongUIInterface::Update()
{
	if (m_Type == NOTHING)return;
	if (m_bClose) return;



	//if (m_Type == SHOW)
	//{
	//	if (b_LbuttonDown == 3)
	//	{

	//		if (Collision())
	//		{
	//			g_Camera->SetPause(true);
	//			m_Drag = true;
	//		}

	//	}

	//	if (b_LbuttonDown == 0)
	//	{
	//		m_Drag = false;
	//		g_Camera->SetPause(false);
	//	}

	//}
	//else if (m_Type == TEXT)
	//{
	//	Collision();
	//	if (b_LbuttonDown == 3)
	//	{

	//		if (Collision())
	//		{
	//			g_Camera->SetPause(true);
	//			m_Drag = true;
	//		}

	//	}

	//	if (b_LbuttonDown == 0)
	//	{
	//		m_Drag = false;
	//		g_Camera->SetPause(false);
	//	}
	//}
	//else if (m_Type == BUTTON)
	//{
	//	if (Collision())
	//	{

	//		m_Color = D3DCOLOR_ARGB(255, 200, 200, 200);

	//	}
	//	else
	//	{
	//		m_Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	//	}
	//	if (b_LbuttonDown == 3)
	//	{

	//		if (Collision())
	//		{
	//			g_Camera->SetPause(true);
	//			m_Color = D3DCOLOR_ARGB(255, 100, 100, 100);
	//			m_Drag = true;
	//		}

	//	}
	//	if (b_LbuttonDown == 2)
	//	{
	//		if (m_Drag)
	//		{
	//			if (Collision())
	//			{
	//				m_Color = D3DCOLOR_ARGB(255, 100, 100, 100);
	//			}
	//			else
	//			{
	//				m_Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	//			}
	//		}

	//	}
	//	if (b_LbuttonDown == 0)
	//	{
	//		if (m_Drag)
	//		{
	//			m_Color = D3DCOLOR_ARGB(255, 255, 255, 255);
	//			if (Collision())
	//			{
	//				if (CallBackF)
	//					CallBackF();
	//				/*if (m_pTarget)
	//				{
	//				if (m_typeTarget == TEXT)
	//				{
	//				m_pTarget->SetText(str);
	//				}
	//				if (m_typeTarget == DRAG)
	//				{
	//				m_pTarget->SetClose(true);
	//				}
	//				}*/
	//			}
	//			else
	//			{

	//			}
	//		}
	//		m_Drag = false;
	//		g_Camera->SetPause(false);
	//	}
	//}
	//else if (m_Type == DRAG)
	//{
	//	if (b_LbuttonDown == 3)
	//	{

	//		if (Collision())
	//		{
	//			g_Camera->SetPause(true);

	//			m_startX = m_BasicTranslation._41;
	//			m_startY = m_BasicTranslation._42;
	//			m_Point.x = _ptMouse.x - m_CollisionVec[0].x;
	//			m_Point.y = _ptMouse.y - m_CollisionVec[0].y;
	//			m_Drag = true;
	//		}

	//	}
	//	if (b_LbuttonDown == 2)
	//	{
	//		if (m_Drag)
	//		{
	//			m_FinishX = _ptMouse.x;
	//			m_FinishY = _ptMouse.y;
	//			float distanceX = m_FinishX - m_startX;
	//			float distanceY = m_FinishY - m_startY;
	//			m_BasicTranslation._41 = m_startX + distanceX - (m_Point.x);
	//			m_BasicTranslation._42 = m_startY + distanceY - (m_Point.y);
	//		}

	//	}
	//	if (b_LbuttonDown == 0)
	//	{
	//		m_Drag = false;
	//		g_Camera->SetPause(false);
	//	}
	//}


	for (int i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Update();
	}

}
void cTaeYeongUIInterface::Render(cTaeYeongUIInterface * parent)
{
	//GETDEVICE->SetTexture(0, NULL);
	if (m_Type == NOTHING)return;
	if (m_bClose) return;


	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	if (m_Type == TEXT)
	{
		m_Color.a = 0;
	}
	if (parent)
	{
		m_World = m_Local *  m_BasicTranslation*parent->m_World;
		m_pSprite->SetTransform(&m_World);
		m_pSprite->Draw(m_pTexture,
			&m_Rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			m_Color);
	}
	else
	{
		m_World = m_Local* m_BasicTranslation;
		m_pSprite->SetTransform(&m_World);
		m_pSprite->Draw(m_pTexture,
			&m_Rc,
			&D3DXVECTOR3(0, 0, 0),           ///////////이미지의 중심 (기본 0,0,0 은 왼쪽위)
			&D3DXVECTOR3(0, 0, 0),           //////////이미지의 중심이 어디에 있는가
			m_Color);
	}







	RECT TextRc = RectMake(m_CollisionVec[0].x, m_CollisionVec[0].y, m_stImageInfo.Width, m_stImageInfo.Height);
	TextRc = RectMake(0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
	if (m_Type == TEXT)
	{
		m_Color.a = 1;
		m_pFont->DrawTextA(m_pSprite,
			str,
			strlen(str),
			&TextRc,
			DT_LEFT | DT_TOP | DT_WORDBREAK,
			D3DCOLOR_ARGB(255, 255, 255, 0));
		//D3DCOLOR_XRGB(255, 255, 0));
	}

	GETDEVICE->SetTexture(0, NULL);
	m_pSprite->End();
	for (int i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Render(this);
	}


}

void cTaeYeongUIInterface::AddChild(cTaeYeongUIInterface * child)
{
	m_vecChild.push_back(child);
}

bool cTaeYeongUIInterface::Collision()
{
	m_CollisionVec[0] = D3DXVECTOR3(0, 0, 0);
	m_CollisionVec[1] = D3DXVECTOR3(0 + m_Rc.right, 0, 0);
	m_CollisionVec[2] = D3DXVECTOR3(0 + m_Rc.right, 0 + m_Rc.bottom, 0);
	m_CollisionVec[3] = D3DXVECTOR3(0, 0 + m_Rc.bottom, 0);

	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&m_CollisionVec[i], &m_CollisionVec[i], &m_World);
	}
	D3DXVECTOR3 Mouse;
	Mouse.x = g_ptMouse.x;
	Mouse.y = g_ptMouse.y;
	Mouse.z = 0;
	if (!LineDotDown(m_CollisionVec[0], m_CollisionVec[1], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[1], m_CollisionVec[2], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[2], m_CollisionVec[3], Mouse))
	{
		return false;
	}
	if (!LineDotDown(m_CollisionVec[3], m_CollisionVec[0], Mouse))
	{
		return false;
	}
	return true;
}

bool cTaeYeongUIInterface::LineDotDown(D3DXVECTOR3 startDot, D3DXVECTOR3 finishDot, D3DXVECTOR3 targetDot)
{
	D3DXVECTOR3 A_B = finishDot - startDot;
	D3DXVECTOR3 A_C = targetDot - startDot;

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &A_B, &A_C);

	if (Cross.z >= 0)
		return true;
	else
		return false;

}

void cTaeYeongUIInterface::SetText(char * text)
{
	sprintf_s(str, "%s", text);
}
