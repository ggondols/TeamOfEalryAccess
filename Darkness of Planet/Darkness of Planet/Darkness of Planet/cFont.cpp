#include "stdafx.h"
#include "cFont.h"


cFont::cFont()
	: m_pTarget(NULL)
{
}


cFont::~cFont()
{
}

void cFont::Setup(D3DXVECTOR3* vTarget, D3DXVECTOR3 vPos)
{
	m_pTarget = vTarget;
	m_vPos = vPos;
	m_fStartTime = TIMEMANAGER->getWorldTime();
}

void cFont::Update()
{
	m_vPos.y += 0.2f;
	if (m_fStartTime + 3.0f < TIMEMANAGER->getWorldTime())
	{

	}
	D3DXVECTOR3 vTarget, vMyPos;
	vMyPos = m_vPos;
	vTarget = *m_pTarget;
	vMyPos.y = vTarget.y;
	D3DXVECTOR3 vDirection = vMyPos - vTarget;

	if (D3DXVec3LengthSq(&vDirection) <= D3DX_16F_EPSILON)
		return;

	D3DXVec3Normalize(&vDirection, &vDirection);

	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);

	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;
}

void cFont::Render()
{
	HDC hdc = CreateCompatibleDC(0);
	// �۲��� Ư���� �����ϴ� LOGFONT ����ü�� ä���.
	LOGFONT Font;
	ZeroMemory(&Font, sizeof(LOGFONT));
	Font.lfHeight = 25;
	Font.lfWidth = 25;
	Font.lfWeight = 500;
	Font.lfItalic = false;
	Font.lfUnderline = false;
	Font.lfStrikeOut = false;
	Font.lfCharSet = DEFAULT_CHARSET;
	strcpy_s(Font.lfFaceName, "Times New Roman");
	// �۲� ��Ÿ�� 

	// ������ ����� ��ġ ���ؽ�Ʈ�� �۲��� �����Ѵ�.
	HFONT hFont;
	HFONT hFontOld;
	hFont = CreateFontIndirect(&Font);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	// �ؽ�Ʈ�� 3D �޽��� ������. 
	ID3DXMesh* Text = 0;
	D3DXCreateText(GETDEVICE, hdc, "100", 0.0001f, 0.1f, &Text, 0, 0);

	// �ٽ� ������ �۲��� �����ϰ� �ڿ��� �����Ѵ�.
	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);

	Text->DrawSubset(0);
}
