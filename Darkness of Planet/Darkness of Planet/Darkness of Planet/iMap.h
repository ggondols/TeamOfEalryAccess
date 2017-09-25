#pragma once
class iMap
{
public:
	iMap(void);
	virtual ~iMap(void);

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
	virtual void Render(D3DXVECTOR3 characterposition) = 0;
	virtual LPD3DXMESH getMesh() = 0;
};

