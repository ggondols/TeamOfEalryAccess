#include "stdafx.h"
#include "TeicLaser.h"


TeicLaser::TeicLaser()
{
}


TeicLaser::~TeicLaser()
{
}

LPD3DXMESH TeicLaser::LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, GETDEVICE, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("�� �ε� ���� ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}
	return ret;
}