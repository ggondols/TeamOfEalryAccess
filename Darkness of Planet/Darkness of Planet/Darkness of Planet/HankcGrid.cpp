#include "stdafx.h"
#include "HankcGrid.h"
namespace Hank
{

	cGrid::cGrid()
	{
	}


	cGrid::~cGrid()
	{
	}

	void cGrid::Release(void)
	{

	}

	void cGrid::Setup(int nNumLine, float fInterval)
	{

	//	pyramidR = new cPyramidLight;
	//	pyramidG = new cPyramidLight;
	//	pyramidB = new cPyramidLight;

	/*	pyramidR->Setup(D3DXVECTOR3(255, 0, 0));
		pyramidG->Setup(D3DXVECTOR3(0, 255, 0));
		pyramidB->Setup(D3DXVECTOR3(0, 0, 255));*/

		int nHalfNumLine = nNumLine / 2;
		float fMax = fabs(nHalfNumLine * fInterval);

		D3DCOLOR color = D3DCOLOR_XRGB(0, 0, 255);
		for (int i = 1; i <= nHalfNumLine; ++i)
		{
			color = D3DCOLOR_XRGB(100, 100, 100);
			if (i % 5 == 0)
			{
				color = D3DCOLOR_XRGB(255, 255, 255);
			}
			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, i*fInterval), color));
			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(fMax, 0, i*fInterval), color));

			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, -i*fInterval), color));
			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(fMax, 0, -i*fInterval), color));

			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(i * fInterval, 0, -fMax), color));
			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(i * fInterval, 0, fMax), color));

			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0, -fMax), color));
			m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0, fMax), color));
		}

		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, 0), D3DCOLOR_XRGB(255, 0, 0)));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(fMax, 0, 0), D3DCOLOR_XRGB(255, 0, 0)));

		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 0, -fMax), D3DCOLOR_XRGB(0, 0, 255)));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, 0, fMax), D3DCOLOR_XRGB(0, 0, 255)));

		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, -fMax, 0), D3DCOLOR_XRGB(0, 255, 0)));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(0, fMax, 0), D3DCOLOR_XRGB(0, 255, 0)));


		GETDEVICE->CreateVertexBuffer(
			m_vecVertex.size() * sizeof(ST_PC_VERTEX),
			0,
			ST_PC_VERTEX::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);


		vb->Lock(0, 0, (void**)&vertices, 0);
		for (int i = 0; i < m_vecVertex.size(); i++)
		{
			vertices[i] = m_vecVertex[i];
		}
		vb->Unlock();


	}

	void cGrid::Render()
	{
		//pyramidR->Render();
		//pyramidG->Render();
		//pyramidB->Render();

		GETDEVICE->SetTexture(0, NULL);
		GETDEVICE->SetRenderState(D3DRS_LIGHTING, false);
		GETDEVICE->LightEnable(0, false);
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);

		GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
		GETDEVICE->SetFVF(ST_PC_VERTEX::FVF);
		GETDEVICE->SetStreamSource(0, vb, 0, sizeof(ST_PC_VERTEX));

		D3DVERTEXBUFFER_DESC vbDesc;
		vb->GetDesc(&vbDesc);
		GETDEVICE->DrawPrimitive(D3DPT_LINELIST,
			0, vbDesc.Size / 2);

	}

}