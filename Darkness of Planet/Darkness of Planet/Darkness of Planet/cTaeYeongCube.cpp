#include "stdafx.h"
#include "cTaeYeongCube.h"


cTaeYeongCube::cTaeYeongCube()
{
}


cTaeYeongCube::~cTaeYeongCube()
{
}

void cTaeYeongCube::Update()
{
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		rotationangle -= 0.1;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		rotationangle += 0.1;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		position.x += 0.3 * sinf(rotationangle);
		position.z += 0.3 * cosf(rotationangle);
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		position.x -= 0.3 * sinf(rotationangle);
		position.z -= 0.3 * cosf(rotationangle);
	}


	D3DXMATRIXA16 matWorldtemp1;
	D3DXMATRIXA16 matWorldtemp2;

	D3DXMatrixRotationY(&matWorldtemp1, rotationangle);
	D3DXMatrixTranslation(&matWorldtemp2, position.x, position.y, position.z);
	matWorld = matWorldtemp1* matWorldtemp2;
}

void cTaeYeongCube::Setup()
{

	ST_PNT_VERTEX temp0 = { D3DXVECTOR3(-1, -1, -1),D3DXVECTOR3(-1, -1, -1), D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp1 = { D3DXVECTOR3(-1, 1, -1),D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp2 = { D3DXVECTOR3(1, 1, -1), D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp3 = { D3DXVECTOR3(1, -1, -1),D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp4 = { D3DXVECTOR3(-1, -1, 1),D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp5 = { D3DXVECTOR3(-1, 1, 1), D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp6 = { D3DXVECTOR3(1, 1, 1),  D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };
	ST_PNT_VERTEX temp7 = { D3DXVECTOR3(1, -1, 1), D3DXVECTOR3(-1, -1, -1) ,D3DXVECTOR2(0,0) };

	m_aVertex[0] = temp0;	m_aVertex[1] = temp1;	m_aVertex[2] = temp2;
	m_aVertex[3] = temp0;	m_aVertex[4] = temp2;	m_aVertex[5] = temp3;
	///0~6 ¾Õ


	m_aVertex[6] = temp4; m_aVertex[7] = temp6;	m_aVertex[8] = temp5;
	m_aVertex[9] = temp4; m_aVertex[10] = temp7; m_aVertex[11] = temp6;




	m_aVertex[12] = temp4; m_aVertex[13] = temp5; m_aVertex[14] = temp1;
	m_aVertex[15] = temp4; m_aVertex[16] = temp1; m_aVertex[17] = temp0;



	m_aVertex[18] = temp3; m_aVertex[19] = temp2; m_aVertex[20] = temp6;
	m_aVertex[21] = temp3; m_aVertex[22] = temp6; m_aVertex[23] = temp7;



	m_aVertex[24] = temp1; m_aVertex[25] = temp5; m_aVertex[26] = temp6;
	m_aVertex[27] = temp1; m_aVertex[28] = temp6; m_aVertex[29] = temp2;



	m_aVertex[30] = temp4; m_aVertex[31] = temp0; m_aVertex[32] = temp3;
	m_aVertex[33] = temp4; m_aVertex[34] = temp3; m_aVertex[35] = temp7;


	D3DXMatrixIdentity(&matWorld);
	position = D3DXVECTOR3(0, 0, 0);
	rotationangle = 0;



	/*D3DDevice->CreateVertexBuffer(
	36 * sizeof(ST_PNT_VERTEX)
	, 0,
	ST_PNT_VERTEX::FVF,
	D3DPOOL_MANAGED,
	&vb,
	0);




	ST_PNT_VERTEX* vertices;
	vb->Lock(0, 0, (void**)&vertices, 0);
	for (int j = 0; j < 36; j++)
	{
	vertices[j] = m_aVertex[j];
	}
	vb->Unlock();*/
	SetMesh();
}

void cTaeYeongCube::Setup(char* fullpath, int x, int y)
{
	Setup();
	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, x - 0.5 + 1, 0.5, -y - 0.5);
	D3DXMATRIXA16 scale;
	D3DXMatrixScaling(&scale, 0.5, 1, 0.5);

	for (int i = 0; i < 36; i++)
	{
		D3DXVec3TransformCoord(&m_aVertex[i].p, &m_aVertex[i].p, &scale);
		D3DXVec3TransformCoord(&m_aVertex[i].p, &m_aVertex[i].p, &trans);
		if (i % 6 == 0)
		{
			m_aVertex[i].t = D3DXVECTOR2(0, 1);
		}
		else if (i % 6 == 1)
		{
			m_aVertex[i].t = D3DXVECTOR2(0, 0);
		}
		else if (i % 6 == 2)
		{
			m_aVertex[i].t = D3DXVECTOR2(1, 0);
		}
		else if (i % 6 == 3)
		{
			m_aVertex[i].t = D3DXVECTOR2(0, 1);
		}
		else if (i % 6 == 4)
		{
			m_aVertex[i].t = D3DXVECTOR2(1, 0);
		}
		else if (i % 6 == 5)
		{
			m_aVertex[i].t = D3DXVECTOR2(1, 1);
		}


	}


	D3DXCreateTextureFromFile(GETDEVICE,
		fullpath,
		&m_pTexture);


	//D3DDevice->CreateVertexBuffer(
	//	36 * sizeof(ST_PNT_VERTEX)
	//	, 0,
	//	ST_PNT_VERTEX::FVF,
	//	D3DPOOL_MANAGED,
	//	&vb,
	//	0);




	//ST_PNT_VERTEX* vertices;
	//vb->Lock(0, 0, (void**)&vertices, 0);
	//for (int j = 0; j < 36; j++)
	//{
	//	vertices[j] = m_aVertex[j];
	//}
	//vb->Unlock();

	SetMesh();
}

void cTaeYeongCube::Render()
{

	GETDEVICE->SetTexture(0, m_pTexture);
	GETDEVICE->SetFVF(ST_PNT_VERTEX::FVF);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	/*D3D->getD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
	12,
	m_aVertex,
	sizeof(ST_PNT_VERTEX));*/
	/*D3DDevice->SetStreamSource(0, vb, 0, sizeof(ST_PNT_VERTEX));
	D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 36 / 3);*/
	Mesh->DrawSubset(0);
}

void cTaeYeongCube::Release()
{
}

void cTaeYeongCube::SetMesh()
{

	int totalnum = 36;

	D3DXCreateMeshFVF(totalnum / 3, totalnum, D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF, GETDEVICE, &Mesh);
	ST_PNT_VERTEX*	meshvertices = 0;
	Mesh->LockVertexBuffer(0, (void**)& meshvertices);

	for (int j = 0; j < totalnum; j++)
	{

		meshvertices[j] = m_aVertex[j];
	}

	Mesh->UnlockVertexBuffer();

	WORD* meshverticesI = 0;
	Mesh->LockIndexBuffer(0, (void**)& meshverticesI);
	for (int j = 0; j < totalnum; j++)
	{
		meshverticesI[j] = j;
	}
	Mesh->UnlockIndexBuffer();


	DWORD* meshverticesAttri = 0;

	Mesh->LockAttributeBuffer(0, &meshverticesAttri);
	for (int j = 0; j < totalnum / 3; j++)
	{
		meshverticesAttri[j] = 0;
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

}
