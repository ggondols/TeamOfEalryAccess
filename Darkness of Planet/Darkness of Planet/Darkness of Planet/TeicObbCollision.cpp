#include "stdafx.h"
#include "TeicObbCollision.h"


bool TeicObbCollision::CheckCollision(BoundingSquare * A, BoundingSquare * B)
{
	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;
	int i;
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	D3DXVECTOR3 diff = A->m_vCenterPos - B->m_vCenterPos;


	////////////////
	c[0][0] = D3DXVec3Dot(&A->m_vXdir, &B->m_vXdir);
	absC[0][0] = abs(c[0][0]);
	if(absC[0][0]>cutoff)
		existsParallelPair = true;

	c[0][1] = D3DXVec3Dot(&A->m_vXdir, &B->m_vYdir);
	absC[0][1] = abs(c[0][1]);
	if (absC[0][1]>cutoff)
		existsParallelPair = true;

	c[0][2] = D3DXVec3Dot(&A->m_vXdir, &B->m_vZdir);
	absC[0][2] = abs(c[0][2]);
	if (absC[0][2]>cutoff)
		existsParallelPair = true;

	
	d[0] = D3DXVec3Dot(&diff, &A->m_vXdir);
	r = abs(d[0]);
	r0 = A->m_fSizeX;
	r1 = B->m_fSizeX * absC[0][0] +B->m_fSizeY * absC[0][1] + B->m_fSizeZ * absC[0][2];

	if (r > r0 + r1)
		return FALSE;


////////////////////////
	c[1][0] = D3DXVec3Dot(&A->m_vYdir, &B->m_vXdir);
	absC[1][0] = abs(c[1][0]);
	if (absC[1][0]>cutoff)
		existsParallelPair = true;

	c[1][1] = D3DXVec3Dot(&A->m_vYdir, &B->m_vYdir);
	absC[1][1] = abs(c[1][1]);
	if (absC[1][1]>cutoff)
		existsParallelPair = true;

	c[1][2] = D3DXVec3Dot(&A->m_vYdir, &B->m_vZdir);
	absC[1][2] = abs(c[1][2]);
	if (absC[1][2]>cutoff)
		existsParallelPair = true;


	d[1] = D3DXVec3Dot(&diff, &A->m_vYdir);
	r = abs(d[1]);
	r0 = A->m_fSizeY;
	r1 = B->m_fSizeX * absC[1][0] + B->m_fSizeY * absC[1][1] + B->m_fSizeZ * absC[1][2];

	if (r > r0 + r1)
		return FALSE;

/////////////////////////////////
	c[2][0] = D3DXVec3Dot(&A->m_vZdir, &B->m_vXdir);
	absC[2][0] = abs(c[2][0]);
	if (absC[2][0]>cutoff)
		existsParallelPair = true;

	c[2][1] = D3DXVec3Dot(&A->m_vZdir, &B->m_vYdir);
	absC[2][1] = abs(c[2][1]);
	if (absC[2][1]>cutoff)
		existsParallelPair = true;

	c[2][2] = D3DXVec3Dot(&A->m_vZdir, &B->m_vZdir);
	absC[2][2] = abs(c[2][2]);
	if (absC[2][2]>cutoff)
		existsParallelPair = true;


	d[2] = D3DXVec3Dot(&diff, &A->m_vZdir);
	r = abs(d[2]);
	r0 = A->m_fSizeZ;
	r1 = B->m_fSizeX * absC[2][0] + B->m_fSizeY * absC[2][1] + B->m_fSizeZ * absC[2][2];

	if (r > r0 + r1)
		return FALSE;



	r = abs(D3DXVec3Dot(&diff, &B->m_vXdir));
	r0 = A->m_fSizeX* absC[0][0] + A->m_fSizeY* absC[1][0] + A->m_fSizeZ* absC[2][0];
	r1 = B->m_fSizeX;
	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &B->m_vYdir));
	r0 = A->m_fSizeX* absC[0][1] + A->m_fSizeY * absC[1][1] + A->m_fSizeZ * absC[2][1];
	r1 = B->m_fSizeY;
	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &B->m_vZdir));
	r0 = A->m_fSizeX * absC[0][2] + A->m_fSizeY * absC[1][2] + A->m_fSizeZ  * absC[2][2];
	r1 = B->m_fSizeZ;
	if (r > r0 + r1)
		return FALSE;

	if (existsParallelPair == true)
		return TRUE;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = A->m_fSizeY* absC[2][0] + A->m_fSizeZ* absC[1][0];
	r1 = B->m_fSizeY* absC[0][2] + B->m_fSizeZ* absC[0][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = A->m_fSizeY* absC[2][1] + A->m_fSizeZ* absC[1][1];
	r1 = B->m_fSizeX* absC[0][2] + B->m_fSizeZ* absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = A->m_fSizeY* absC[2][2] + A->m_fSizeZ* absC[1][2];
	r1 = B->m_fSizeX* absC[0][1] + B->m_fSizeY* absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = A->m_fSizeX* absC[2][0] + A->m_fSizeZ* absC[0][0];
	r1 = B->m_fSizeY* absC[1][2] + B->m_fSizeZ* absC[1][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = A->m_fSizeX* absC[2][1] + A->m_fSizeZ* absC[0][1];
	r1 = B->m_fSizeX* absC[1][2] + B->m_fSizeZ* absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = A->m_fSizeX* absC[2][2] + A->m_fSizeZ* absC[0][2];
	r1 = B->m_fSizeX* absC[1][1] + B->m_fSizeY* absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = A->m_fSizeX* absC[1][0] + A->m_fSizeY* absC[0][0];
	r1 = B->m_fSizeY* absC[2][2] + B->m_fSizeZ* absC[2][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = A->m_fSizeX* absC[1][1] + A->m_fSizeY* absC[0][1];
	r1 = B->m_fSizeX* absC[2][2] + B->m_fSizeZ* absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = A->m_fSizeX* absC[1][2] + A->m_fSizeY* absC[0][2];
	r1 = B->m_fSizeX* absC[2][1] + B->m_fSizeY* absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	return TRUE;
}

TeicObbCollision::TeicObbCollision()
{
}


TeicObbCollision::~TeicObbCollision()
{
}
