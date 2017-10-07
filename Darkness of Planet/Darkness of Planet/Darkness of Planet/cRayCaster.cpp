#include "stdafx.h"
#include "cRayCaster.h"


cRayCaster::cRayCaster()
{
}


cRayCaster::~cRayCaster()
{
}

D3D::Ray cRayCaster::CalcuRay(int x, int y)
{
	D3D::Ray ray = D3D::CalcPickingRay(g_ptMouse.x, g_ptMouse.y);

	D3DXMATRIX view;
	GETDEVICE->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	D3D::TransformRay(&ray, &viewInverse);

	return ray;
}

D3DXVECTOR3 cRayCaster::GetMousePT(int x, int y, vector<D3DXVECTOR3>& vec)
{
	//0---1
	//| / |
	//2---3
	//012 - 213;
	//∫§≈Õ 4∞≥

	D3D::Ray ray = CalcuRay(x, y);

	float dist;
	for (int i = 0; i < vec.size(); i += 3)
	{
		dist = 0;
		if (D3DXIntersectTri(&vec[i], &vec[i + 1], &vec[i + 2],
			&ray._origin, &ray._direction, NULL, NULL, &dist))
		{
			printf("\n ∞≈∏Æ %f ¥ÎªÛ i %d \n ", dist, i);
			break;
		}

	}
	return ray._origin + ray._direction * dist;
}

D3DXVECTOR3 cRayCaster::GetMousePtFromNode(int x, int y, vector<D3DXVECTOR3>& vec)
{
	//0---1
	//| / |
	//2---3
	//012 - 213;
	//∫§≈Õ 4∞≥

	if (vec.empty())
	{
		return D3DXVECTOR3(0,0,0);
	}

	D3D::Ray ray = CalcuRay(x, y);

	float dist;

	dist = 0;
	if (D3DXIntersectTri(&vec[0], &vec[1], &vec[2],
		&ray._origin, &ray._direction, NULL, NULL, &dist))
	{
		return ray._origin + ray._direction * dist;
	}
	else if (D3DXIntersectTri(&vec[2], &vec[1], &vec[3],
		&ray._origin, &ray._direction, NULL, NULL, &dist))
	{
		return ray._origin + ray._direction * dist;
	}



	return D3DXVECTOR3(0, 0, 0);
}


