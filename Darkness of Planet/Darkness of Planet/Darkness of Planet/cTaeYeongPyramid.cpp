#include "stdafx.h"
#include "cTaeYeongPyramid.h"


cTaeYeongPyramid::cTaeYeongPyramid()
{
}


cTaeYeongPyramid::~cTaeYeongPyramid()
{
	SAFE_DELETE(vb);
}

void cTaeYeongPyramid::Setup(float x, float y, float z, float RotationZangle, float RotationYangle, DWORD color /*= D3DCOLOR_XRGB(255, 255, 255)*/)
{

	D3DXVECTOR3 crosstemp;

	//////////////기본 x축 방향
	///x축 1
	m_aPyramid[0].p = D3DXVECTOR3(0, 0, 0);
	m_aPyramid[1].p = D3DXVECTOR3(1, 1, -1);
	m_aPyramid[2].p = D3DXVECTOR3(1, -1, -1);
	//////2
	m_aPyramid[3].p = D3DXVECTOR3(0, 0, 0);
	m_aPyramid[4].p = D3DXVECTOR3(1, -1, 1);
	m_aPyramid[5].p = D3DXVECTOR3(1, 1, 1);
	/////3
	m_aPyramid[6].p = D3DXVECTOR3(0, 0, 0);
	m_aPyramid[7].p = D3DXVECTOR3(1, 1, 1);
	m_aPyramid[8].p = D3DXVECTOR3(1, 1, -1);
	///////4
	m_aPyramid[9].p = D3DXVECTOR3(0, 0, 0);
	m_aPyramid[10].p = D3DXVECTOR3(1, -1, -1);
	m_aPyramid[11].p = D3DXVECTOR3(1, -1, 1);
	/////5
	m_aPyramid[12].p = D3DXVECTOR3(1, 1, -1);
	m_aPyramid[13].p = D3DXVECTOR3(1, 1, 1);
	m_aPyramid[14].p = D3DXVECTOR3(1, -1, 1);
	/////6
	m_aPyramid[15].p = D3DXVECTOR3(1, 1, -1);
	m_aPyramid[16].p = D3DXVECTOR3(1, -1, 1);
	m_aPyramid[17].p = D3DXVECTOR3(1, -1, -1);



	for (int i = 0; i < 18; i = i + 3)
	{
		D3DXVec3Cross(&crosstemp, &(m_aPyramid[i + 1].p - m_aPyramid[i].p), &(m_aPyramid[i + 2].p - m_aPyramid[i].p));
		m_aPyramid[i].n = m_aPyramid[i + 1].n = m_aPyramid[i + 2].n = crosstemp;
	}


	/*for (int i = 0; i < 18; i++)
	{
	m_aPyramid[i].c = color;

	}*/
	//////
	D3DXMatrixIdentity(&matWorld);


	D3DXMATRIXA16 scalemat;
	D3DXMatrixScaling(&scalemat, x, y, z);
	D3DXMATRIXA16 rotationZ;
	D3DXMatrixRotationZ(&rotationZ, RotationZangle);
	D3DXMATRIXA16 rotationY;
	D3DXMatrixRotationY(&rotationY, RotationYangle);

	matWorld = scalemat * rotationY * rotationZ;



	GETDEVICE->CreateVertexBuffer(
		18 * sizeof(ST_PN_VERTEX)
		, 0,
		ST_PN_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&vb,
		0);




	ST_PN_VERTEX* vertices;
	vb->Lock(0, 0, (void**)&vertices, 0);
	for (int j = 0; j < 18; j++)
	{
		vertices[j] = m_aPyramid[j];
	}
	vb->Unlock();

}



void cTaeYeongPyramid::Render()
{
	//D3D->getD3DDevice()->SetRenderState(D3DRS_LIGHTING, true);

	GETDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	/*D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	D3DDevice->LightEnable(0, true);*/

	GETDEVICE->SetFVF(ST_PN_VERTEX::FVF);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);


	GETDEVICE->SetStreamSource(0, vb, 0, sizeof(ST_PN_VERTEX));
	GETDEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 18 / 3);
	/*D3D->getD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
	6,
	m_aPyramid,
	sizeof(ST_PN_VERTEX));
	*/
}

void cTaeYeongPyramid::Release()
{
}
