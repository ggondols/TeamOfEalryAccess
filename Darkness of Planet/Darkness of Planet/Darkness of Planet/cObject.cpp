#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_nRefCount(1)
{
	//
	//g_pObjectPool->AddObject(this);
}


cObject::~cObject()
{
	assert(m_nRefCount <= 0 && "릴리즈 하지 않은 오브젝트가 있음");
	//
	//g_pObjectPool->RemoveObject(this);
}

void cObject::AddRef()
{
	++m_nRefCount;
}

void cObject::Release()
{
	--m_nRefCount;
	if (m_nRefCount <= 0)
	{
		//delete this;
	}
}

cObject * cObject::AutoRelase()
{
	//g_pAutoReleasePool->AddObject(this);
	return this;
}
