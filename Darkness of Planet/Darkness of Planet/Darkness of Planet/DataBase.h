#pragma once
#include "cSingletonBase.h"

enum ITEM_TYPE
{
	ITEMTYPE_MATERIAL,
	ITEMTYPE_EXPENDABLE,
	ITEMTYPE_WEAPON,
	ITEMTYPE_HELMET,
	ITEMTYPE_ARMOR,
	ITEMTYPE_END
};

struct ST_ITEM_DATA
{
	ITEM_TYPE sType;
	float fValue;
};

class DataBase : public cSingletonBase<DataBase>
{
private:
	typedef map<string, ST_ITEM_DATA*> mapItemData;
	typedef map<string, ST_ITEM_DATA*>::iterator mapItemDataIter;

private:
	mapItemData	m_mapItemData;

public:
	DataBase();
	~DataBase();

	void LoadItemData();
	void Setup();
	void Destroy();
	float GetItemValue(string itemName);
};

