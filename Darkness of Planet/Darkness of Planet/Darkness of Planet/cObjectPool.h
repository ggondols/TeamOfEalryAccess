#pragma once
#include "cObject.h"
#include "cSingletonBase.h"
class cObjectPool : public cSingletonBase<cObjectPool>
{
private:
	set<cObject*> m_setObject;
public:
	cObjectPool();
	~cObjectPool();

	void AddObject(cObject* pObject);
	void RemoveObject(cObject* pObject);
	void Destroy();
};

