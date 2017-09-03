#include "stdafx.h"
#include "cTaeYeongHPbar.h"


cTaeYeongHPbar::cTaeYeongHPbar()
{
	m_fHeight = 0;
	m_vPosition = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity(&m_HPStatus);

}


cTaeYeongHPbar::~cTaeYeongHPbar()
{
}


void cTaeYeongHPbar::Setup(char * Fullpath, D3DXVECTOR3& position, float height)
{
	m_vPosition = position;
	m_fHeight = height;
	m_Vertex[0].p = D3DXVECTOR3(-1, 0, 0);
	m_Vertex[1].p = D3DXVECTOR3(-1, 1, 0);
	m_Vertex[2].p = D3DXVECTOR3(1, 1, 0);
	m_Vertex[3].p = D3DXVECTOR3(1, 1, 0);
	m_Vertex[4].p = D3DXVECTOR3(1, 0, 0);
	m_Vertex[5].p = D3DXVECTOR3(-1, 0, 0);

	m_Vertex[0].t = D3DXVECTOR2(0, 1);
	m_Vertex[1].t = D3DXVECTOR2(0, 0);
	m_Vertex[2].t = D3DXVECTOR2(1, 0);
	m_Vertex[3].t = D3DXVECTOR2(1, 0);
	m_Vertex[4].t = D3DXVECTOR2(1, 1);
	m_Vertex[5].t = D3DXVECTOR2(0, 1);
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, 0.8, 0.2, 1);

	m_fXcut = 0.8;
	for (int i = 0; i < 6; i++)
	{

		D3DXVec3TransformCoord(&m_Vertex[i].p, &m_Vertex[i].p, &Scale);
	}

	D3DXVECTOR3 crosstemp;
	for (int i = 0; i < 6; i = i + 3)
	{
		D3DXVec3Cross(&crosstemp, &(m_Vertex[i + 1].p - m_Vertex[i].p), &(m_Vertex[i + 2].p - m_Vertex[i].p));
		m_Vertex[i].n = m_Vertex[i + 1].n = m_Vertex[i + 2].n = crosstemp;
	}


	int totalnum = 6;

	D3DXCreateMeshFVF(totalnum / 3, totalnum, D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF, GETDEVICE, &Mesh);
	ST_PNT_VERTEX*	meshvertices = 0;
	Mesh->LockVertexBuffer(0, (void**)& meshvertices);

	for (int j = 0; j < 6; j++)
	{

		meshvertices[j] = m_Vertex[j];
	}

	Mesh->UnlockVertexBuffer();

	WORD* meshverticesI = 0;
	Mesh->LockIndexBuffer(0, (void**)& meshverticesI);
	for (int j = 0; j < 6; j++)
	{
		meshverticesI[j] = j;
	}
	Mesh->UnlockIndexBuffer();


	DWORD* meshverticesAttri = 0;
	int count = 0;
	Mesh->LockAttributeBuffer(0, &meshverticesAttri);
	for (int j = 0; j < 6; j += 3)
	{
		meshverticesAttri[count] = 0;
		count++;
	}

	Mesh->UnlockAttributeBuffer();

	vector<DWORD> adjacencyBuffer3(Mesh->GetNumFaces() * 3);
	Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer3[0]);

	Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&adjacencyBuffer3[0],
		0, 0, 0);
	D3DXCreateTextureFromFile(GETDEVICE,
		Fullpath,
		&m_pTexture);



	Whitematerial.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	Whitematerial.Power = 5.0f;

}

void cTaeYeongHPbar::Setup(char * Fullpath, D3DXVECTOR3 & position, float height, float xCut)
{
	m_vPosition = position;
	m_fHeight = height;
	m_Vertex[0].p = D3DXVECTOR3(0, 0, 0);
	m_Vertex[1].p = D3DXVECTOR3(0, 1, 0);
	m_Vertex[2].p = D3DXVECTOR3(1, 1, 0);
	m_Vertex[3].p = D3DXVECTOR3(1, 1, 0);
	m_Vertex[4].p = D3DXVECTOR3(1, 0, 0);
	m_Vertex[5].p = D3DXVECTOR3(0, 0, 0);

	m_Vertex[0].t = D3DXVECTOR2(0, 1);
	m_Vertex[1].t = D3DXVECTOR2(0, 0);
	m_Vertex[2].t = D3DXVECTOR2(1, 0);
	m_Vertex[3].t = D3DXVECTOR2(1, 0);
	m_Vertex[4].t = D3DXVECTOR2(1, 1);
	m_Vertex[5].t = D3DXVECTOR2(0, 1);
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, xCut, 0.2, 1);
	m_fXcut = xCut;
	for (int i = 0; i < 6; i++)
	{

		D3DXVec3TransformCoord(&m_Vertex[i].p, &m_Vertex[i].p, &Scale);
	}

	D3DXVECTOR3 crosstemp;
	for (int i = 0; i < 6; i = i + 3)
	{
		D3DXVec3Cross(&crosstemp, &(m_Vertex[i + 1].p - m_Vertex[i].p), &(m_Vertex[i + 2].p - m_Vertex[i].p));
		m_Vertex[i].n = m_Vertex[i + 1].n = m_Vertex[i + 2].n = crosstemp;
	}


	int totalnum = 6;

	D3DXCreateMeshFVF(totalnum / 3, totalnum, D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF, GETDEVICE, &Mesh);
	ST_PNT_VERTEX*	meshvertices = 0;
	Mesh->LockVertexBuffer(0, (void**)& meshvertices);

	for (int j = 0; j < 6; j++)
	{

		meshvertices[j] = m_Vertex[j];
	}

	Mesh->UnlockVertexBuffer();

	WORD* meshverticesI = 0;
	Mesh->LockIndexBuffer(0, (void**)& meshverticesI);
	for (int j = 0; j < 6; j++)
	{
		meshverticesI[j] = j;
	}
	Mesh->UnlockIndexBuffer();


	DWORD* meshverticesAttri = 0;
	int count = 0;
	Mesh->LockAttributeBuffer(0, &meshverticesAttri);
	for (int j = 0; j < 6; j += 3)
	{
		meshverticesAttri[count] = 0;
		count++;
	}

	Mesh->UnlockAttributeBuffer();

	vector<DWORD> adjacencyBuffer3(Mesh->GetNumFaces() * 3);
	Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer3[0]);

	Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&adjacencyBuffer3[0],
		0, 0, 0);
	D3DXCreateTextureFromFile(GETDEVICE,
		Fullpath,
		&m_pTexture);



	Whitematerial.Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Specular = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	Whitematerial.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	Whitematerial.Power = 5.0f;
}

void cTaeYeongHPbar::Render()
{
	D3DXMATRIXA16 World_matrix;
	D3DXMATRIX Trans;
	D3DXMATRIX rotation;
	D3DXMATRIX Transtwo;
	D3DXMatrixTranslation(&Transtwo, 1, 0, 0);
	D3DXMatrixIdentity(&World_matrix);
	D3DXMatrixTranslation(&Trans, m_vPosition.x, m_fHeight, m_vPosition.z);

	//float y = g_Camera->getAngleY() - D3DX_PI / 2;
	float y = 0;
	D3DXMatrixRotationY(&rotation, y);
	World_matrix = rotation*m_HPStatus*Trans;
	GETDEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);







	GETDEVICE->SetTexture(0, m_pTexture);
	GETDEVICE->SetMaterial(&Whitematerial);

	GETDEVICE->SetFVF(ST_PNT_VERTEX::FVF);

	GETDEVICE->SetTransform(D3DTS_WORLD, &World_matrix);

	Mesh->DrawSubset(0);

	GETDEVICE->SetTexture(0, NULL);
	for (int i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Render(rotation*Trans, m_HPStatus._11);
	}
}

void cTaeYeongHPbar::Render(D3DXMATRIX parent, float x)
{
	D3DXMATRIXA16 World_matrix;
	D3DXMATRIX Trans;

	D3DXMatrixIdentity(&World_matrix);
	D3DXMatrixTranslation(&Trans, m_vPosition.x, m_fHeight, m_vPosition.z);

	World_matrix = m_HPStatus*Trans*parent;
	GETDEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);


	GETDEVICE->SetTexture(0, m_pTexture);
	GETDEVICE->SetMaterial(&Whitematerial);

	GETDEVICE->SetFVF(ST_PNT_VERTEX::FVF);

	GETDEVICE->SetTransform(D3DTS_WORLD, &World_matrix);

	Mesh->DrawSubset(0);



	GETDEVICE->SetTexture(0, NULL);
}

void cTaeYeongHPbar::Update(D3DXVECTOR3 & position)
{
	m_vPosition = position;
}

void cTaeYeongHPbar::SetHP(float x)
{
	D3DXMATRIX Scal;
	D3DXMatrixScaling(&Scal, x, 1, x);

	m_HPStatus = Scal;
}


void cTaeYeongHPbar::AddChild(cTaeYeongHPbar * child)
{
	m_vecChild.push_back(child);
}