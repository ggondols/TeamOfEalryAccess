#include "stdafx.h"
#include "cTaeYeongCubeTexture.h"


cTaeYeongCubeTexture::cTaeYeongCubeTexture()
	:m_pParent(NULL)
	, movex(0)
	, m_Center(D3DXVECTOR3(0, 0, 0))
{
}


cTaeYeongCubeTexture::~cTaeYeongCubeTexture()
{
}
///////////future.png
void cTaeYeongCubeTexture::Setup(D3DXVECTOR3 Scale, D3DXVECTOR3 RotationXYZ, D3DXVECTOR3 Move, const char* imageNamePlusWhere, char* body)
{
	swingangle = 0;
	swingdir = 1;
	movez = 0;
	rotationyangle = 0;
	D3DXCreateTextureFromFile(GETDEVICE,
		imageNamePlusWhere,
		&m_pTexture);
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(imageNamePlusWhere, &info);

	m_sBody = body;
	float startx, starty;
	float divide1x, divide1y;
	float divide2x, divide2y;
	if (strncmp(body, "head", 128) == 0)
	{
		startx = 0.0f; starty = 0.0f;
		divide1x = 64;
		divide1y = 64;
		divide2x = 64;
		divide2y = 65;
	}
	if (strncmp(body, "body", 128) == 0)
	{
		startx = 128.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 64;
		divide2y = 96;
	}
	if (strncmp(body, "leftleg", 128) == 0)
	{
		startx = 0.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "rightleg", 128) == 0)
	{
		startx = 0.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "rightarm", 128) == 0)
	{
		startx = 321.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "leftarm", 128) == 0)
	{
		startx = 321.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}

	D3DXVECTOR3 temp0 = D3DXVECTOR3(-1, -1, -1);
	D3DXVECTOR3 temp1 = D3DXVECTOR3(-1, 1, -1);
	D3DXVECTOR3 temp2 = D3DXVECTOR3(1, 1, -1);
	D3DXVECTOR3 temp3 = D3DXVECTOR3(1, -1, -1);
	D3DXVECTOR3 temp4 = D3DXVECTOR3(-1, -1, 1);
	D3DXVECTOR3 temp5 = D3DXVECTOR3(-1, 1, 1);
	D3DXVECTOR3 temp6 = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 temp7 = D3DXVECTOR3(1, -1, 1);

	D3DXVECTOR3 crosstemp;

	///////앞
	m_aVertex[0].p = temp0;	m_aVertex[1].p = temp1;	m_aVertex[2].p = temp2;
	m_aVertex[3].p = temp0;	m_aVertex[4].p = temp2;	m_aVertex[5].p = temp3;




	m_aVertex[0].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[1].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[2].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);






	m_aVertex[3].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[4].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[5].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);

	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[0].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[1].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[2].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[3].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[4].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[5].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}


	////뒤
	m_aVertex[6].p = temp4; m_aVertex[7].p = temp6;	m_aVertex[8].p = temp5;
	m_aVertex[9].p = temp4; m_aVertex[10].p = temp7; m_aVertex[11].p = temp6;


	m_aVertex[6].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[7].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[8].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);



	m_aVertex[9].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[10].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[11].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);



	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[6].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[7].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[8].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[9].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[10].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[11].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);
	}

	//////왼쪽 측면
	m_aVertex[12].p = temp4; m_aVertex[13].p = temp5; m_aVertex[14].p = temp1;
	m_aVertex[15].p = temp4; m_aVertex[16].p = temp1; m_aVertex[17].p = temp0;


	m_aVertex[12].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[13].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[14].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);

	m_aVertex[15].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[16].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[17].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);


	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[12].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[13].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[14].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[15].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[16].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[17].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}


	////오른쪽 측면
	m_aVertex[18].p = temp3; m_aVertex[19].p = temp2; m_aVertex[20].p = temp6;
	m_aVertex[21].p = temp3; m_aVertex[22].p = temp6; m_aVertex[23].p = temp7;


	m_aVertex[18].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[19].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[20].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);

	m_aVertex[21].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[22].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[23].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);





	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[18].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[19].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[20].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[21].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[22].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[23].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}

	///상단
	m_aVertex[24].p = temp1; m_aVertex[25].p = temp5; m_aVertex[26].p = temp6;
	m_aVertex[27].p = temp1; m_aVertex[28].p = temp6; m_aVertex[29].p = temp2;

	m_aVertex[24].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[25].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty) / info.Height);
	m_aVertex[26].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);

	m_aVertex[27].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[28].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);
	m_aVertex[29].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);


	////하단
	m_aVertex[30].p = temp4; m_aVertex[31].p = temp0; m_aVertex[32].p = temp3;
	m_aVertex[33].p = temp4; m_aVertex[34].p = temp3; m_aVertex[35].p = temp7;


	m_aVertex[30].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[31].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);
	m_aVertex[32].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty) / info.Height);


	m_aVertex[33].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[34].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty) / info.Height);
	m_aVertex[35].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);


	D3DXMatrixIdentity(&matWorld);
	position = D3DXVECTOR3(0, 0, 0);


	D3DXMatrixScaling(&scaling, Scale.x, Scale.y, Scale.z);

	for (int i = 0; i < 36; i++)
	{
		D3DXVec3TransformCoord(&m_aVertex[i].p, &m_aVertex[i].p, &scaling);

	}
	D3DXMatrixIdentity(&scaling);


	//	D3DXMatrixScaling(&scalingInverse, 1 / Scale.x, 1 / Scale.y, 1 / Scale.z);

	/*if (strncmp(body, "leftleg", 128) == 0 || strncmp(body, "rightleg", 128) == 0 ||
	strncmp(body, "rightarm", 128) == 0)
	{

	}


	if (strncmp(body, "leftarm", 128) == 0)
	{
	startx = 321.0f; starty = 129.0;
	divide1x = 32;
	divide1y = 31;
	divide2x = 32;
	divide2y = 96;
	}*/



	D3DXMatrixRotationX(&rotationX, RotationXYZ.x);
	D3DXMatrixRotationY(&rotationY, RotationXYZ.y);
	D3DXMatrixRotationZ(&rotationZ, RotationXYZ.z);



	D3DXMatrixTranslation(&move, Move.x, Move.y, Move.z);
	m_matWorld2 = matWorld*rotationX *rotationY * rotationZ * move;
	matWorld = matWorld*scaling *rotationX *rotationY * rotationZ * move;


	position.x = Move.x;
	position.y = Move.y;
	position.z = Move.z;





	for (int i = 0; i < 36; i = i + 3)
	{
		D3DXVec3Cross(&crosstemp, &(m_aVertex[i + 1].p - m_aVertex[i].p), &(m_aVertex[i + 2].p - m_aVertex[i].p));
		m_aVertex[i].n = m_aVertex[i + 1].n = m_aVertex[i + 2].n = crosstemp;
	}



	/*D3DXVec3Cross(&crosstemp, &(m_aVertex[4].p - m_aVertex[3].p), &(m_aVertex[5].p - m_aVertex[3].p));
	m_aVertex[3].n = m_aVertex[4].n = m_aVertex[5].n = crosstemp;*/



	GETDEVICE->CreateVertexBuffer(
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
	vb->Unlock();

}

void cTaeYeongCubeTexture::Setup(D3DXVECTOR3 Scale, D3DXVECTOR3 RotationXYZ, D3DXVECTOR3 Move, cTaeYeongCubeTexture * parent, const char * imageNamePlusWhere, char * body)
{
	m_pParent = parent;
	swingangle = 0;
	//swingdir = -1;


	movez = 0;
	rotationyangle = 0;
	D3DXCreateTextureFromFile(GETDEVICE,
		imageNamePlusWhere,
		&m_pTexture);
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFileA(imageNamePlusWhere, &info);

	m_sBody = body;
	float startx, starty;
	float divide1x, divide1y;
	float divide2x, divide2y;
	if (strncmp(body, "head", 128) == 0)
	{
		startx = 0.0f; starty = 0.0f;
		divide1x = 64;
		divide1y = 64;
		divide2x = 64;
		divide2y = 65;
	}
	if (strncmp(body, "body", 128) == 0)
	{
		startx = 128.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 64;
		divide2y = 96;
	}
	if (strncmp(body, "leftleg", 128) == 0)
	{
		startx = 0.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "rightleg", 128) == 0)
	{
		startx = 0.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "rightarm", 128) == 0)
	{
		startx = 321.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}
	if (strncmp(body, "leftarm", 128) == 0)
	{
		startx = 321.0f; starty = 129.0;
		divide1x = 32;
		divide1y = 31;
		divide2x = 32;
		divide2y = 96;
	}

	D3DXVECTOR3 temp0 = D3DXVECTOR3(-1, -1, -1);
	D3DXVECTOR3 temp1 = D3DXVECTOR3(-1, 1, -1);
	D3DXVECTOR3 temp2 = D3DXVECTOR3(1, 1, -1);
	D3DXVECTOR3 temp3 = D3DXVECTOR3(1, -1, -1);
	D3DXVECTOR3 temp4 = D3DXVECTOR3(-1, -1, 1);
	D3DXVECTOR3 temp5 = D3DXVECTOR3(-1, 1, 1);
	D3DXVECTOR3 temp6 = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 temp7 = D3DXVECTOR3(1, -1, 1);


	D3DXVECTOR3 crosstemp;
	///////앞
	m_aVertex[0].p = temp0;	m_aVertex[1].p = temp1;	m_aVertex[2].p = temp2;
	m_aVertex[3].p = temp0;	m_aVertex[4].p = temp2;	m_aVertex[5].p = temp3;


	m_aVertex[0].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[1].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[2].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);


	m_aVertex[3].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[4].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[5].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);

	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[0].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[1].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[2].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[3].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[4].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[5].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}


	////뒤
	m_aVertex[6].p = temp4; m_aVertex[7].p = temp6;	m_aVertex[8].p = temp5;
	m_aVertex[9].p = temp4; m_aVertex[10].p = temp7; m_aVertex[11].p = temp6;


	m_aVertex[6].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[7].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[8].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);



	m_aVertex[9].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[10].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[11].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);



	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[6].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[7].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[8].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[9].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[10].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[11].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);
	}

	//////왼쪽 측면
	m_aVertex[12].p = temp4; m_aVertex[13].p = temp5; m_aVertex[14].p = temp1;
	m_aVertex[15].p = temp4; m_aVertex[16].p = temp1; m_aVertex[17].p = temp0;


	m_aVertex[12].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[13].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[14].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);

	m_aVertex[15].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[16].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[17].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);


	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[12].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[13].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[14].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[15].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[16].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[17].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}


	////오른쪽 측면
	m_aVertex[18].p = temp3; m_aVertex[19].p = temp2; m_aVertex[20].p = temp6;
	m_aVertex[21].p = temp3; m_aVertex[22].p = temp6; m_aVertex[23].p = temp7;


	m_aVertex[18].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[19].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[20].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);

	m_aVertex[21].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);
	m_aVertex[22].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[23].t = D3DXVECTOR2((startx + divide1x * 2 + divide2x) / info.Width, (starty + divide1y + divide2y) / info.Height);





	if (strncmp(body, "leftarm", 128) == 0 || strncmp(body, "leftleg", 128) == 0)
	{
		m_aVertex[18].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[19].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[20].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);


		m_aVertex[21].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y + divide2y) / info.Height);
		m_aVertex[22].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y) / info.Height);
		m_aVertex[23].t = D3DXVECTOR2((startx) / info.Width, (starty + divide1y + divide2y) / info.Height);
	}

	///상단
	m_aVertex[24].p = temp1; m_aVertex[25].p = temp5; m_aVertex[26].p = temp6;
	m_aVertex[27].p = temp1; m_aVertex[28].p = temp6; m_aVertex[29].p = temp2;

	m_aVertex[24].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[25].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty) / info.Height);
	m_aVertex[26].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);

	m_aVertex[27].t = D3DXVECTOR2((startx + divide1x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[28].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);
	m_aVertex[29].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);


	////하단
	m_aVertex[30].p = temp4; m_aVertex[31].p = temp0; m_aVertex[32].p = temp3;
	m_aVertex[33].p = temp4; m_aVertex[34].p = temp3; m_aVertex[35].p = temp7;


	m_aVertex[30].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[31].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty) / info.Height);
	m_aVertex[32].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty) / info.Height);


	m_aVertex[33].t = D3DXVECTOR2((startx + divide1x + divide2x) / info.Width, (starty + divide1y) / info.Height);
	m_aVertex[34].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty) / info.Height);
	m_aVertex[35].t = D3DXVECTOR2((startx + divide1x + divide2x * 2) / info.Width, (starty + divide1y) / info.Height);


	D3DXMatrixIdentity(&matWorld);
	//position = D3DXVECTOR3(0, 0, 0);

	position = D3DXVECTOR3(0, 0, 0);
	position.x = Move.x;
	position.y = Move.y;
	position.z = Move.z;

	D3DXMatrixScaling(&scaling, Scale.x, Scale.y, Scale.z);
	//D3DXMatrixScaling(&scalingInverse, 1 / Scale.x, 1 / Scale.y, 1 / Scale.z);
	for (int i = 0; i < 36; i++)
	{
		D3DXVec3TransformCoord(&m_aVertex[i].p, &m_aVertex[i].p, &scaling);

	}
	D3DXMatrixIdentity(&scaling);




	D3DXMatrixRotationX(&rotationX, RotationXYZ.x);
	D3DXMatrixRotationY(&rotationY, RotationXYZ.y);
	D3DXMatrixRotationZ(&rotationZ, RotationXYZ.z);


	D3DXMatrixTranslation(&move, Move.x, Move.y, Move.z);

	matWorld = matWorld*scaling *rotationX *rotationY * rotationZ * move * GetParentWorld(m_pParent);
	m_matWorld2 = matWorld *rotationX *rotationY * rotationZ * move *  GetParentWorld(m_pParent);


	////////시작






	////////끝



	if (m_sBody == "rightleg" || m_sBody == "leftarm")
	{
		swingdir = 1;
	}
	else
	{
		swingdir = -1;
	}


	for (int i = 0; i < 36; i = i + 3)
	{
		D3DXVec3Cross(&crosstemp, &(m_aVertex[i + 1].p - m_aVertex[i].p), &(m_aVertex[i + 2].p - m_aVertex[i].p));
		m_aVertex[i].n = m_aVertex[i + 1].n = m_aVertex[i + 2].n = crosstemp;

		/*m_aVertex[i].n = D3DXVECTOR3(rand() % 100, rand() % 100, rand() % 100);
		m_aVertex[i+1].n = D3DXVECTOR3(rand() % 100, rand() % 100, rand() % 100);
		m_aVertex[i+2].n = D3DXVECTOR3(rand() % 100, rand() % 100, rand() % 100);*/
	}
	//////0 3 17
	/*m_aVertex[0].n = D3DXVECTOR3(10, 0, 0);
	m_aVertex[3].n = D3DXVECTOR3(10, 0, 0);
	m_aVertex[17].n = D3DXVECTOR3(10, -0, 0);*/
	//D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	GETDEVICE->CreateVertexBuffer(
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
	vb->Unlock();

}

D3DXMATRIXA16 cTaeYeongCubeTexture::GetParentWorld(cTaeYeongCubeTexture* parent)
{
	D3DXMATRIXA16 temp;
	D3DXMatrixIdentity(&temp);
	if (!parent) return temp;
	return parent->GetWorldmatrixNoScaling();
}

void cTaeYeongCubeTexture::Render()
{


	GETDEVICE->SetTexture(0, m_pTexture);
	GETDEVICE->SetFVF(ST_PNT_VERTEX::FVF);

	m_CenterMoving = m_Center;

	D3DXVec3TransformCoord(&m_CenterMoving, &m_CenterMoving, &(matWorld));

	GETDEVICE->SetTransform(D3DTS_WORLD, &(matWorld));
	/*D3D->getD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
	12,
	m_aVertex,
	sizeof(ST_PNT_VERTEX));
	D3DDevice->SetTexture(0, NULL);*/



	GETDEVICE->SetStreamSource(0, vb, 0, sizeof(ST_PNT_VERTEX));
	GETDEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 36 / 3);

	for (int i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Render();
	}

}

void cTaeYeongCubeTexture::Update()
{

	m_Forwarddir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, rotationyangle);
	D3DXVec3TransformNormal(&m_Forwarddir, &m_Forwarddir, &matR);

	for (int i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->Update();
	}

}

void cTaeYeongCubeTexture::Release()
{
}

void cTaeYeongCubeTexture::Setscale(float x, float y, float z)
{
	D3DXMATRIXA16 anotherScale;
	D3DXMatrixScaling(&anotherScale, x, y, z);
	matWorld = matWorld * anotherScale;
}

void cTaeYeongCubeTexture::Addchild(cTaeYeongCubeTexture * child)
{
	m_vecChild.push_back(child);
	child->m_pParent = this;
}

