#pragma once

class D3D::Ray;


class cRayCaster
{
public:
	cRayCaster();
	~cRayCaster();

	D3D::Ray CalcuRay(int x, int y); //{ D3D::CalcPickingRay(x, y); }

	D3DXVECTOR3 GetMousePT(int x, int y, vector<D3DXVECTOR3>& vec);

	D3DXVECTOR3 GetMousePtFromNode(int x, int y, vector<D3DXVECTOR3>& vec);




};

