#pragma once
class LDYiMap
{
public:
	LDYiMap(void);
	virtual ~LDYiMap(void);

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
	virtual void Render() = 0;
	virtual LPD3DXMESH getMesh() = 0;
};

