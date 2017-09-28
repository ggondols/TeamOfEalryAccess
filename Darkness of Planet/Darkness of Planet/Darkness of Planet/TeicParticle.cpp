#include "stdafx.h"
#include "TeicParticle.h"


TeicParticle::TeicParticle()
{
	m_Start = false;
	m_fUpDown = 0.0f;
	m_iUpDownPlus = 1;
}


TeicParticle::~TeicParticle()
{
}

void TeicParticle::Setup(ST_PC_VERTEX * vertex, float liftspan, float startsize, float finishsize, float emitterangle,
	float startrot, float endrot, float speed, D3DXVECTOR3 startcolor, D3DXVECTOR3 finishcolor, bool loop)
{
	m_pVertex = vertex;
	m_pVertexSample = *vertex;
	m_fLifeSpan = liftspan;          //
	m_fStartSize = startsize;		//
	m_fFinishSize = finishsize;		//
	m_fEmitterAngle = emitterangle;
	m_fStartRot = startrot;			//
	m_fEndRot = endrot;				//
	m_fSpeed = speed;
	m_dwStartColor = startcolor;
	m_dwFinishColor = finishcolor;

	//d3dcolor 선형보간 어렵다
	m_alpha = 255;

	m_delta = (255 * 0.016667) / m_fLifeSpan;
	m_loop = loop;
	m_distance = 0;
	m_deltadistance = 0.016667 * m_fSpeed;
	m_fTime = 0;
}

void TeicParticle::Setup2(ST_PC_VERTEX * vertex, float liftspan, D3DXVECTOR3 endposition, float angleX, float angleY, float angleZ, float speed, D3DXVECTOR3 startcolor, D3DXVECTOR3 finishcolor, bool loop)
{
	m_pVertex = vertex;
	m_pVertexSample = *vertex;
	m_fLifeSpan = liftspan;          
	

	
	m_vLastPosition = endposition;
	m_fCurveAngleX = angleX;
	m_fCurveAngleY = angleY;
	m_fCurveAngleZ = angleZ;

	m_fSpeed = speed;
	m_dwStartColor = startcolor;
	m_dwFinishColor = finishcolor;

	//d3dcolor 선형보간 어렵다
	m_alpha = 255;

	m_delta = (255 * 0.016667) / m_fLifeSpan;
	m_loop = loop;
	m_distance = 0;
	m_deltadistance = 0.016667 * m_fSpeed;
	m_fTime = 0;

}



void TeicParticle::Update()
{

	if (!m_Start)return;
	m_fTime += TIMEMANAGER->getElapsedTime();
	m_distance += m_deltadistance;
	D3DXMATRIX m_rotMat, m_rotMat2, m_transMat, matworld;
	D3DXMatrixRotationZ(&m_rotMat, m_fEmitterAngle);
	D3DXMatrixRotationX(&m_rotMat2, m_fEmitterAngle - D3DX_PI / 2);
	D3DXMatrixTranslation(&m_transMat, m_distance, 0, 0);
	matworld = m_transMat*m_rotMat*m_rotMat2;

	D3DXVec3TransformCoord(&m_pVertex->p, &m_pVertexSample.p, &matworld);
	m_alpha -= m_delta;
	D3DXVec3Lerp(&m_dwNowcolor, &m_dwStartColor, &m_dwFinishColor, m_fTime);
	(*m_pVertex).c = D3DCOLOR_ARGB((int)m_alpha, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
	if (m_alpha < 0)
	{
		if (!m_loop)
		{
			m_Start = false;
			(*m_pVertex).c = D3DCOLOR_ARGB(0, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
		}
		else
		{
			m_fTime = 0;
			m_distance = 0;
			m_alpha = 255;
		}

	}






}

void TeicParticle::Update2()
{
	if (!m_Start)return;
	m_fTime += TIMEMANAGER->getElapsedTime();

	D3DXMATRIX m_rotMatX, m_rotMatY, m_rotMatZ, matworld;
	
	D3DXMatrixRotationX(&m_rotMatX, m_fCurveAngleX);
	D3DXMatrixRotationY(&m_rotMatY, m_fCurveAngleY);
	D3DXMatrixRotationZ(&m_rotMatZ, m_fCurveAngleZ);
	matworld = m_rotMatX*m_rotMatY*m_rotMatZ;

	
	D3DXVec3Lerp(&m_pVertex->p, &m_pVertexSample.p, &m_vLastPosition, m_fTime / m_fLifeSpan);
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, -m_pVertexSample.p.x, -m_pVertexSample.p.y, -m_pVertexSample.p.z);
	D3DXVec3TransformCoord(&m_pVertex->p, &m_pVertex->p, &trans);
	D3DXVec3TransformCoord(&m_pVertex->p, &m_pVertex->p, &matworld);
	D3DXMatrixTranslation(&trans, m_pVertexSample.p.x, m_pVertexSample.p.y, m_pVertexSample.p.z);
	D3DXVec3TransformCoord(&m_pVertex->p, &m_pVertex->p, &trans);

	m_alpha -= m_delta;
	D3DXVec3Lerp(&m_dwNowcolor, &m_dwStartColor, &m_dwFinishColor, m_fTime);
	(*m_pVertex).c = D3DCOLOR_ARGB((int)m_alpha, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
	if (m_alpha < 0)
	{
		if (!m_loop)
		{
			m_Start = false;
			(*m_pVertex).c = D3DCOLOR_ARGB(0, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
		}
		else
		{
			m_fTime = 0;
			m_distance = 0;
			m_alpha = 255;
		}

	}


}

void TeicParticle::Update3()
{
	if (!m_Start)return;
	m_fUpDown += 0.1f *m_iUpDownPlus*m_fSpeed;

	if (m_fUpDown > 3)
	{
		m_iUpDownPlus = -1;
	}
	if (m_fUpDown < -3)
	{
		m_iUpDownPlus = 1;
	}
	m_distance += m_deltadistance;
	m_fTime += TIMEMANAGER->getElapsedTime();

	


	D3DXVec3Lerp(&m_pVertex->p, &m_pVertexSample.p, &m_vLastPosition, m_distance / 1);
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0, m_fUpDown, 0);
	D3DXVec3TransformCoord(&m_pVertex->p, &m_pVertex->p, &trans);
	


	m_alpha -= m_delta;
	D3DXVec3Lerp(&m_dwNowcolor, &m_dwStartColor, &m_dwFinishColor, m_fTime);
	(*m_pVertex).c = D3DCOLOR_ARGB((int)m_alpha, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
	if (m_alpha < 0)
	{
		if (!m_loop)
		{
			m_Start = false;
			(*m_pVertex).c = D3DCOLOR_ARGB(0, (int)m_dwNowcolor.x, (int)m_dwNowcolor.y, (int)m_dwNowcolor.z);
		}
		else
		{
			m_fTime = 0;
			m_distance = 0;
			m_alpha = 255;
		}

	}


}

void TeicParticle::Start()
{
	m_Start = true;
	m_fTime = 0;
	m_distance = 0;
	m_alpha = 255;
	m_fUpDown = 0;
	m_iUpDownPlus = 1;
	//m_fTime = TIMEMANAGER->getWorldTime();
}

void TeicParticle::End()
{
	m_Start = false;
}
