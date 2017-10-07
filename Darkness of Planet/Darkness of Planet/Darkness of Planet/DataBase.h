#pragma once
#include "cSingletonBase.h"

enum ITEM_TYPE
{
	ITEMTYPE_NONE,
	ITEMTYPE_PART,
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
	float st_FireRate;
	float st_CallbackTime;
};

class DataBase : public cSingletonBase<DataBase>
{
private:
	typedef map<string, ST_ITEM_DATA*> mapItemData;
	typedef map<string, ST_ITEM_DATA*>::iterator mapItemDataIter;
	typedef map<float, D3DXVECTOR3> mapTimePositionData;
	typedef map<float, D3DXVECTOR3>::iterator mapTimePositionDataIter;
	typedef map<string, mapTimePositionData> mapAIData;
	typedef map<string, mapTimePositionData>::iterator mapAIDataIter;

private:
	mapItemData	m_mapItemData;
	mapAIData	m_mapAIData;

	D3DXVECTOR3 GetPosition(float time, mapTimePositionData& mapTimeData);

public:
	DataBase();
	~DataBase();

	void LoadItemData();
	void LoadAIData();
	void Setup();
	void Destroy();
	float GetItemValue(string itemName);
	float GetItemFireRate(string itemName);
	float GetCallbacktime(string itemName);
	D3DXVECTOR3 GetTimeToPosition(string aiName, float time);
};

