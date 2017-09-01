#include "StdAfx.h"
#include "cNode.h"


cNode::cNode(int nNodeIndex, eNodeType e)
	: F(0.0f)
	, G(0.0f)
	, H(0.0f)
	, m_pParent(NULL)
	, m_isClose(false)
	, m_isOpen(false)
	, m_nRow(nNodeIndex / TILE_N)
	, m_nCol(nNodeIndex % TILE_N)
	, m_eNodeType(e)
{
	SetRect(&m_rc, 
		m_nCol * TILE_SIZE, 
		m_nRow * TILE_SIZE,
		(m_nCol + 1) * TILE_SIZE,
		(m_nRow + 1) * TILE_SIZE);
}

cNode::~cNode(void)
{
}

void cNode::Render( HDC hdc )
{
	COLORREF BkColor = RGB(255,255,255);
	switch(m_eNodeType)
	{
	case E_EMPTY:
		break;
	case E_WALL:
		BkColor = RGB(80,80,80);
		break;
	case E_START:
		break;
	case E_DEST:
		break;
	case E_PATH:
		BkColor = RGB(255,255,0);
		break;
	case E_OPEN:
		BkColor = RGB(0,255,0);
		break;
	case E_CLOSE:
		BkColor = RGB(255,0,0);
		break;
	default:
		break;
	}

	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(BkColor);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);

	if(F > 0.0f)
	{
		SetBkMode(hdc, TRANSPARENT);

		char szBuf[16];

		sprintf(szBuf, "%.1f", F);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_LEFT | DT_TOP);

		sprintf(szBuf, "%.1f", G);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);

		sprintf(szBuf, "%.1f", H);
		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);

		if(m_pParent)
		{
			POINT ptMyCenter;
			ptMyCenter.x = (m_rc.left + m_rc.right) / 2;
			ptMyCenter.y = (m_rc.top + m_rc.bottom) / 2;

			POINT ptParentCenter;
			ptParentCenter.x = (m_pParent->m_rc.left + m_pParent->m_rc.right) / 2;
			ptParentCenter.y = (m_pParent->m_rc.top + m_pParent->m_rc.bottom) / 2;

			POINT ptLineEnd;
			ptLineEnd.x = (ptMyCenter.x + ptParentCenter.x) / 2;
			ptLineEnd.y = (ptMyCenter.y + ptParentCenter.y) / 2;

			MoveToEx(hdc, ptMyCenter.x, ptMyCenter.y, NULL);
			LineTo(hdc, ptLineEnd.x, ptLineEnd.y);

			hBrush = CreateSolidBrush(BkColor);
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, 
				ptMyCenter.x - 3,
				ptMyCenter.y - 3,
				ptMyCenter.x + 3,
				ptMyCenter.y + 3);

			SelectObject(hdc, hOldBrush);
			DeleteObject(hBrush);
		}
	}
}
