#include "stdafx.h"
#include "cTaeYeongTile.h"


cTaeYeongTile::cTaeYeongTile()
{
}


cTaeYeongTile::~cTaeYeongTile()
{
	SAFE_DELETE(vb);
}


void cTaeYeongTile::Setup(int num, float distance)
{
	m_vecTile.reserve(num * 5);
	if (num <= 0) return;
	int half = num / 2;
	float max = half * distance;
	for (int i = 1; i < half + 1; i++)
	{
		ST_PC_VERTEX temp;
		temp.p = D3DXVECTOR3(i*distance, 0, max);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);

		temp.p = D3DXVECTOR3(i*distance, 0, -max);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);

		temp.p = D3DXVECTOR3(-i*distance, 0, max);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);


		temp.p = D3DXVECTOR3(-i*distance, 0, -max);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);

		///////////////////////////////////////////////////////////////////

		temp.p = D3DXVECTOR3(max, 0, i*distance);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);


		temp.p = D3DXVECTOR3(-max, 0, i*distance);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);

		temp.p = D3DXVECTOR3(max, 0, -i*distance);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);



		temp.p = D3DXVECTOR3(-max, 0, -i*distance);
		temp.c = D3DCOLOR_XRGB(137, 137, 137);
		if (i % 5 == 0)
			temp.c = D3DCOLOR_XRGB(255, 255, 255);
		m_vecTile.push_back(temp);
	}

	ST_PC_VERTEX temp;
	temp.p = D3DXVECTOR3(0, 0, max);
	temp.c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecTile.push_back(temp);

	temp.p = D3DXVECTOR3(0, 0, -max);
	temp.c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecTile.push_back(temp);

	temp.p = D3DXVECTOR3(max, 0, 0);
	temp.c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecTile.push_back(temp);

	temp.p = D3DXVECTOR3(-max, 0, 0);
	temp.c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecTile.push_back(temp);
	///////////////yÃà Ãß°¡
	temp.p = D3DXVECTOR3(0, max, 0);
	temp.c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecTile.push_back(temp);

	temp.p = D3DXVECTOR3(0, -max, 0);
	temp.c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecTile.push_back(temp);



	GETDEVICE->CreateVertexBuffer(
		m_vecTile.size() * sizeof(ST_PC_VERTEX)
		, 0,
		ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&vb,
		0);




	ST_PC_VERTEX* vertices;
	vb->Lock(0, 0, (void**)&vertices, 0);
	for (int j = 0; j < m_vecTile.size(); j++)
	{
		vertices[j] = m_vecTile[j];
	}
	vb->Unlock();



}

void cTaeYeongTile::Render()
{
	GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	//D3DDevice->LightEnable(0, true);
	//D3D->getD3DDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GETDEVICE->SetFVF(ST_PC_VERTEX::FVF);
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	GETDEVICE->SetStreamSource(0, vb, 0, sizeof(ST_PC_VERTEX));
	GETDEVICE->DrawPrimitive(D3DPT_LINELIST, 0, m_vecTile.size() / 2);

	//D3D->getD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST, m_vecTaeYeongTile.size() / 2, &m_vecTaeYeongTile[0], sizeof(ST_PC_VERTEX));

}

void cTaeYeongTile::Release()
{

	m_vecTile.clear();
}
