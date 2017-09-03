#pragma once
#include "cSingletonBase.h"
#include "cObject.h"
class cAutoReleasePool : public cSingletonBase<cAutoReleasePool>
{
protected:
	set<cObject*> m_setPool;
public:
	cAutoReleasePool();
	~cAutoReleasePool();

	void AddObject(cObject* pObject)
	{
		m_setPool.insert(pObject);
	}
	void Drain()
	{
		for each(auto p in m_setPool)
		{
			p->Release();
		}
		m_setPool.clear();
	}
};

