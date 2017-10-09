#include "stdafx.h"
#include "cHeightMap.h"

cHeightMap::cHeightMap(void)

	:m_nTile(0)
{
	for (int i = 0; i < 4; i++)
	{
		m_pMesh[i] = NULL;
	}
}

cHeightMap::~cHeightMap(void)
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_RELEASE(m_pMesh[i]);
	}


}

void cHeightMap::Load(string szFolder,
	string szFile,
	string szTexture,
	DWORD dwBytesPerPixel/* = 1*/)
{
	string sPath = string(szFolder) + string(szFile);
	m_sTexture = string(szFolder) + string(szTexture);

	FILE* fp = 0;
	fopen_s(&fp, sPath.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	int nNumVertex = nFileSize / dwBytesPerPixel;
	int nRow = (int)sqrt(nNumVertex + FLT_EPSILON);
	if (nNumVertex != nRow * nRow)
	{
		fclose(fp);
		assert(false && "정사각형태의 RAW파일만 사용 가능합니다.");
	}
	fseek(fp, 0, SEEK_SET);

	int nCol = nRow;
	m_nTile = nRow - 1;
	vector<ST_PNT_VERTEX> vecVertex;
	m_vecVertex.resize(nNumVertex);
	vecVertex.resize(nNumVertex);
	//for (int z = 0; z < nRow; ++z)
	for (int z = nRow - 1; z >= 0; --z)
	{
		for (int x = 0; x < nCol; ++x)
		{
			unsigned char c = fgetc(fp);
			int i = z * nRow + x;
			//////////////////////이부분 바꿔서 크기바꿈 0.1f
			vecVertex[i].p = D3DXVECTOR3(x, (c*0.5) - 10.0f, (z - (nRow - 1)));
			vecVertex[i].n = D3DXVECTOR3(0, 1, 50);
			vecVertex[i].t = D3DXVECTOR2(x / (float)m_nTile, z / (float)m_nTile);

			m_vecVertex[i] = vecVertex[i].p;

			if (dwBytesPerPixel > 1)
			{
				fseek(fp, dwBytesPerPixel - 1, SEEK_CUR);
			}
		}
	}

	fclose(fp);

	//  ---U---
	//  |\ |\ |
	//  | \| \|
	//  L--I--R
	//  |\ |\ |
	//  | \| \|
	//  ---D---
	for (int z = 1; z < nRow - 1; ++z)
	{
		for (int x = 1; x < nCol - 1; ++x)
		{
			int l = (z + 0) * nCol + x - 1;
			int r = (z + 0) * nCol + x + 1;
			int u = (z + 1) * nCol + x + 0;
			int d = (z - 1) * nCol + x + 0;

			D3DXVECTOR3 lr = vecVertex[r].p - vecVertex[l].p;
			D3DXVECTOR3 du = vecVertex[u].p - vecVertex[d].p;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);

			vecVertex[z * nCol + x].n = n;
		}
	}

	//  1--3
	//  |\ |
	//  | \|
	//  0--2                          
	vector<DWORD> vecIndex;
	vecIndex.reserve(m_nTile * m_nTile * 2 * 3);
	for (int z = 0; z < m_nTile; ++z)
	{
		for (int x = 0; x < m_nTile; ++x)
		{
			int _0 = (z + 0) * nCol + x + 0;
			int _1 = (z + 1) * nCol + x + 0;
			int _2 = (z + 0) * nCol + x + 1;
			int _3 = (z + 1) * nCol + x + 1;
			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}
	m_vecVertexCopy.resize(vecVertex.size());
	for (int i = 0; i < 4; i++)
	{
		D3DXMATRIX temp;
		switch (i)
		{
		case	0:
			m_vecVertexCopy = vecVertex;
			break;
		case 1:
			D3DXMatrixTranslation(&temp, m_nTile, 0, 0);
			for (int a = 0; a < m_vecVertexCopy.size(); a++)
			{
				D3DXVec3TransformCoord(&m_vecVertexCopy[a].p, &vecVertex[a].p, &temp);
			}
			break;
		case 2:
			D3DXMatrixTranslation(&temp, 0, 0, -m_nTile);
			for (int a = 0; a < m_vecVertexCopy.size(); a++)
			{
				D3DXVec3TransformCoord(&m_vecVertexCopy[a].p, &vecVertex[a].p, &temp);
			}
			break;
		case 3:
			D3DXMatrixTranslation(&temp, m_nTile, 0, -m_nTile);
			for (int a = 0; a < m_vecVertexCopy.size(); a++)
			{
				D3DXVec3TransformCoord(&m_vecVertexCopy[a].p, &vecVertex[a].p, &temp);
			}
			break;
		}

		D3DXCreateMeshFVF(vecIndex.size() / 3,
			m_vecVertexCopy.size(),
			D3DXMESH_MANAGED | D3DXMESH_32BIT,
			ST_PNT_VERTEX::FVF,
			GETDEVICE,
			&m_pMesh[i]);

		ST_PNT_VERTEX* pV = 0;
		m_pMesh[i]->LockVertexBuffer(0, (LPVOID*)&pV);
		memcpy(pV, &m_vecVertexCopy[0], m_vecVertexCopy.size() * sizeof(ST_PNT_VERTEX));
		m_pMesh[i]->UnlockVertexBuffer();

		DWORD* pI = 0;
		m_pMesh[i]->LockIndexBuffer(0, (LPVOID*)&pI);
		memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
		m_pMesh[i]->UnlockIndexBuffer();

		/*DWORD* pA = 0;
		m_pMesh[i]->LockAttributeBuffer(0, &pA);
		ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
		m_pMesh[i]->UnlockAttributeBuffer();*/

		DWORD* meshverticesAttri = 0;
		int count = 0;
		m_pMesh[i]->LockAttributeBuffer(0, &meshverticesAttri);
		for (int j = 0; j < vecIndex.size(); j += 3)
		{
			int Cal = j / 6;
			int Row = Cal / (m_nTile);
			int Col = Cal % (m_nTile);
			int Subsetnumber;
			if (Row >= 0 && Row < (m_nTile) / 3.0)
			{
				if (Col >= 0 && Col < (m_nTile) / 3.0)
				{
					//Subsetnumber = 0;
					Subsetnumber = 6;
				}
				else if (Col >= (m_nTile) / 3.0 && Col < (m_nTile)* 2.0 / 3.0)
				{
					//Subsetnumber = 1;
					Subsetnumber = 7;
				}
				else if (Col >= (m_nTile) *2.0 / 3.0 && Col <= (m_nTile))
				{
					//Subsetnumber = 2;
					Subsetnumber = 8;
				}
			}
			else if (Row >= (m_nTile) / 3.0 && Row < (m_nTile)* 2.0 / 3.0)
			{
				if (Col >= 0 && Col < (m_nTile) / 3.0)
				{
					Subsetnumber = 3;
				}
				else if (Col >= (m_nTile) / 3.0 && Col < (m_nTile)* 2.0 / 3.0)
				{
					Subsetnumber = 4;
				}
				else if (Col >= (m_nTile) *2.0 / 3.0 && Col <= (m_nTile))
				{
					Subsetnumber = 5;
				}
			}
			else if (Row >= (m_nTile) *2.0/ 3.0 && Row <= (m_nTile))
			{
				if (Col >= 0 && Col < (m_nTile) / 3.0)
				{
					//Subsetnumber = 6;
					Subsetnumber = 0;
				}
				else if (Col >= (m_nTile) / 3.0 && Col < (m_nTile)* 2.0 / 3.0)
				{
					//Subsetnumber = 7;
					Subsetnumber = 1;
				}
				else if (Col >= (m_nTile) *2.0 / 3.0 && Col <= (m_nTile))
				{
					//Subsetnumber = 8;
					Subsetnumber = 2;
				}
			}
			meshverticesAttri[count] = Subsetnumber;
			count++;
		}



		vector<DWORD> vecAdj(vecIndex.size());
		m_pMesh[i]->GenerateAdjacency(0, &vecAdj[0]);

		m_pMesh[i]->OptimizeInplace(
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT |
			D3DXMESHOPT_VERTEXCACHE,
			&vecAdj[0], 0, 0, 0);



	}


	m_stMtl = MATERIALMANAGER->GetGrayWhite();

	GETDEVICE->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	GETDEVICE->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	GETDEVICE->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
}

bool cHeightMap::GetHeight(IN float x, OUT float& y, IN float z)
{
	/*if (x < 0 || z < 0 || x > m_nTile || z > m_nTile)
		return false;*/
	if (x < 0 || z > 0 || x > m_nTile*2 || z < -m_nTile*2)
		return false;

	if (x >= m_nTile)
	{
		x -= m_nTile;
	}
	if (z <= -m_nTile)
	{
		z += m_nTile;
	}
	//  1--3
	//  |\ |
	//  | \|
	//  0--2 
	int nX = x;
	int nZ = z + m_nTile - 1;

	float fDeltaX = x - nX;
	float fDeltaZ = (z + m_nTile - 1) - nZ;

	int _0 = (nZ + 0) * (m_nTile + 1) + nX + 0;
	int _1 = (nZ + 1) * (m_nTile + 1) + nX + 0;
	int _2 = (nZ + 0) * (m_nTile + 1) + nX + 1;
	int _3 = (nZ + 1) * (m_nTile + 1) + nX + 1;

	if (fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 _01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 _02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = (_01 * fDeltaZ + _02 * fDeltaX).y + m_vecVertex[_0].y;
	}
	else
	{
		D3DXVECTOR3 _31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 _32 = m_vecVertex[_2] - m_vecVertex[_3];
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		y = (_31 * fDeltaX + _32 * fDeltaZ).y + m_vecVertex[_3].y;
	}


	return true;
}

void cHeightMap::Render(D3DXVECTOR3 Characterposition)
{
	
	int centerX = Characterposition.x / (m_nTile/3.0f);
	int centerY = (-Characterposition.z) / (m_nTile / 3.0f);

	int num = centerX + (centerY * 6);

	//beginpass
	for (int i = -1; i < 2; i ++)
	{
		for (int j = -1	; j < 2; j++)
		{
			if (centerX + j < 0)continue;
			if (centerX + j > 5)continue;
			if (centerY +i < 0)continue;
			if (centerY + i > 5)continue;
		
			MeshRender(num + j + i*(6));
		}
	}
	//endpass
}

void cHeightMap::frustumcullingRender()
{
	DotWorldSpace();
	SetPlane();
	CheckRender();
}

void cHeightMap::MeshRender(int number)
{
	/*D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	GETDEVICE->SetTexture(0, TEXTUREMANAGER->GetTexture(m_sTexture));
	GETDEVICE->SetMaterial(&m_stMtl);

	switch (number)
	{
		case	0:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(0);
			break;
		case	1:	
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(1);
			break;
		case	2:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(2);
			break;
		case	3:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(0);
			break;
		case	4:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(1);
			break;
		case	5:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(2);
			break;
		case	6:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(3);
			break;
		case	7:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(4);
			break;
		case	8:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(5);
			break;
		case	9:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(3);
			break;
		case	10:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(4);
			break;
		case	11:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(5);
			break;
		case	12:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(6);
			break;
		case	13:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(7);
			break;
		case	14:
			GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
			m_pMesh[0]->DrawSubset(8);
			break;
		case	15:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(6);
			break;
		case	16:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(7);
			break;
		case	17:
			GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
			m_pMesh[1]->DrawSubset(8);
			break;
		case	18:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(0);
			break;
		case	19:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(1);
			break;
		case	20:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(2);
			break;
		case	21:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(0);
			break;
		case	22:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(1);
			break;
		case	23:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(2);
			break;
		case	24:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(3);
			break;
		case	25:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(4);
			break;
		case	26:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(5);
			break;
		case	27:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(3);
			break;
		case	28:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(4);
			break;
		case	29:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(5);
			break;
		case	30:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(6);
			break;
		case	31:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(7);
			break;
		case	32:
			GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
			m_pMesh[2]->DrawSubset(8);
			break;
		case	33:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(6);
			break;
		case	34:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(7);
			break;
		case	35:
			GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
			m_pMesh[3]->DrawSubset(8);
			break;

		default:
		break;
	}
*/
}



void cHeightMap::DotWorldSpace()
{
	//				   5---------6
	//				  /|	   / |
	//               / |      /  |
	//				/  4-----/---7
	//				1-------2   
	//				|		|
	//				|		|
	//				0-------3

	m_Dot[0] = D3DXVECTOR3(-1, -1, 0);
	m_Dot[1] = D3DXVECTOR3(-1, 1, 0);
	m_Dot[2] = D3DXVECTOR3(1, 1, 0);
	m_Dot[3] = D3DXVECTOR3(1, -1, 0);

	m_Dot[4] = D3DXVECTOR3(-1, -1, 1);
	m_Dot[5] = D3DXVECTOR3(-1, 1, 1);
	m_Dot[6] = D3DXVECTOR3(1, 1, 1);
	m_Dot[7] = D3DXVECTOR3(1, -1, 1);

	D3DXMATRIX PorjectionInverse;
	GETDEVICE->GetTransform(D3DTS_PROJECTION, &PorjectionInverse);
	D3DXMatrixInverse(&PorjectionInverse, NULL, &PorjectionInverse);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_Dot[i], &m_Dot[i], &PorjectionInverse);
	}
	D3DXMATRIX ViewSpace;
	GETDEVICE->GetTransform(D3DTS_VIEW, &ViewSpace);
	D3DXMatrixInverse(&ViewSpace, NULL, &ViewSpace);
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_Dot[i], &m_Dot[i], &ViewSpace);
	}
}

void cHeightMap::SetPlane()
{
	///앞쪽 평면    
	D3DXPlaneFromPoints(&m_Plane[0], &m_Dot[1], &m_Dot[2], &m_Dot[3]);
	//위쪽 평면
	D3DXPlaneFromPoints(&m_Plane[1], &m_Dot[1], &m_Dot[5], &m_Dot[6]);
	//오른쪽 평면
	D3DXPlaneFromPoints(&m_Plane[2], &m_Dot[2], &m_Dot[6], &m_Dot[7]);
	//아래쪽 평면
	D3DXPlaneFromPoints(&m_Plane[3], &m_Dot[0], &m_Dot[3], &m_Dot[7]);
	//왼쪽 평면
	D3DXPlaneFromPoints(&m_Plane[4], &m_Dot[5], &m_Dot[1], &m_Dot[0]);
	//뒤쪽 평면
	D3DXPlaneFromPoints(&m_Plane[5], &m_Dot[6], &m_Dot[5], &m_Dot[4]);
}

void cHeightMap::CheckRender()
{
	D3DXVECTOR3 CenterDot[36];
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			CenterDot[j + i * 6] = D3DXVECTOR3(m_nTile / 6.0 + m_nTile / 3.0 *j, 0, -m_nTile / 6.0 - m_nTile / 3.0 *i);
		}
	}
	float radius = D3DXVec3Length(&(CenterDot[0] - D3DXVECTOR3(0, 0, 0)));
	for (int i = 0; i < 36; i++)
	{
		if (CheckShow(CenterDot[i], radius) == true)
		{
			OnlyMeshRender(i);
		}
	}
}

bool cHeightMap::CheckShow(D3DXVECTOR3 center, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float distance;
		distance = (D3DXPlaneDotCoord(&m_Plane[i], &center));
		if (distance - radius >= 0)
			return false;
	}

	return true;
}

void cHeightMap::MeshRender(D3DXVECTOR3 Characterposition)
{
	int centerX = Characterposition.x / (m_nTile / 3.0f);
	int centerY = (-Characterposition.z) / (m_nTile / 3.0f);

	int num = centerX + (centerY * 6);

	//beginpass
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (num + j + i*(6) < 0)continue;
			if (num + j + i*(6) > 35)continue;
			OnlyMeshRender(num + j + i*(6));
		}
	}
	//endpass
}

void cHeightMap::OnlyMeshRender(int num)
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	//GETDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	switch (num)
	{
	case	0:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(0);
		break;
	case	1:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(1);
		break;
	case	2:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(2);
		break;
	case	3:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(0);
		break;
	case	4:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(1);
		break;
	case	5:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(2);
		break;
	case	6:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(3);
		break;
	case	7:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(4);
		break;
	case	8:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(5);
		break;
	case	9:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(3);
		break;
	case	10:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(4);
		break;
	case	11:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(5);
		break;
	case	12:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(6);
		break;
	case	13:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(7);
		break;
	case	14:
		GETDEVICE->SetFVF(m_pMesh[0]->GetFVF());
		m_pMesh[0]->DrawSubset(8);
		break;
	case	15:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(6);
		break;
	case	16:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(7);
		break;
	case	17:
		GETDEVICE->SetFVF(m_pMesh[1]->GetFVF());
		m_pMesh[1]->DrawSubset(8);
		break;
	case	18:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(0);
		break;
	case	19:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(1);
		break;
	case	20:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(2);
		break;
	case	21:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(0);
		break;
	case	22:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(1);
		break;
	case	23:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(2);
		break;
	case	24:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(3);
		break;
	case	25:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(4);
		break;
	case	26:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(5);
		break;
	case	27:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(3);
		break;
	case	28:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(4);
		break;
	case	29:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(5);
		break;
	case	30:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(6);
		break;
	case	31:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(7);
		break;
	case	32:
		GETDEVICE->SetFVF(m_pMesh[2]->GetFVF());
		m_pMesh[2]->DrawSubset(8);
		break;
	case	33:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(6);
		break;
	case	34:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(7);
		break;
	case	35:
		GETDEVICE->SetFVF(m_pMesh[3]->GetFVF());
		m_pMesh[3]->DrawSubset(8);
		break;

	default:
		break;
	}

}
