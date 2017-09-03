#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_nRefCount(1)
{
	OBJECTPOOL->AddObject(this);
}


cObject::~cObject()
{
	assert(m_nRefCount <= 0 && "릴리즈 하지 않은 오브젝트가 있음");
	OBJECTPOOL->RemoveObject(this);
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
		delete this;
	}
}

cObject * cObject::AutoRelase()
{
	OBJECTPOOL->AddObject(this);
	return this;
}
