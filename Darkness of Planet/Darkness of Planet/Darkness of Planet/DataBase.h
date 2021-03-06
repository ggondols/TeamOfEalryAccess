#pragma once
#include "cSingletonBase.h"

enum ITEM_TYPE
{
	ITEMTYPE_NONE,
	ITEMTYPE_PART,//업그레이드 조각
	ITEMTYPE_MATERIAL,//재료
	ITEMTYPE_EXPENDABLE,//소모품
	ITEMTYPE_WEAPON,//무기
	ITEMTYPE_HELMET,//헬멧
	ITEMTYPE_ARMOR,//방어구
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
	vector<string> m_vecString;

	D3DXVECTOR3 GetPosition(float time, mapTimePositionData& mapTimeData);

public:
	DataBase();
	~DataBase();

	void LoadItemData();
	void LoadAIData();

	void Setup();
	void Destroy();

	void AddVectorString(string sBuffer) { m_vecString.push_back(sBuffer); }

	void SaveVectorString(string fileName);

	float GetItemValue(string itemName);
	float GetItemFireRate(string itemName);
	float GetCallbacktime(string itemName);
	D3DXVECTOR3 GetTimeToPosition(string aiName, float time);
};

