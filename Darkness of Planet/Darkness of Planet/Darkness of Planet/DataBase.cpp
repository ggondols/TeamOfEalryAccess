#include "stdafx.h"
#include "DataBase.h"

DataBase::DataBase()
{
}

DataBase::~DataBase()
{
}

void DataBase::Setup()
{
	LoadItemData();
	LoadAIData();
}

void DataBase::Destroy()
{
	for (mapItemDataIter it = m_mapItemData.begin(); it != m_mapItemData.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mapItemData.clear();
}

void DataBase::LoadItemData(void)
{
	FILE* fp = NULL;
	fopen_s(&fp, "Data/ItemData.txt", "r");
	
	string keyName;

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		char szPath[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		sscanf_s(szBuf, "%s", szPath, 1024);

		if (!strcmp(szPath, "name"))
		{
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			keyName = szPath;
			ST_ITEM_DATA* newData = new ST_ITEM_DATA;
			m_mapItemData.insert(make_pair(keyName, newData));
		}
		else if (!strcmp(szPath, "type"))
		{
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			mapItemDataIter it = m_mapItemData.find(keyName);
			if (it != m_mapItemData.end())
			{
				if (!strcmp(szPath, "weapon"))
				{
					it->second->sType = ITEMTYPE_WEAPON;
				}
				else if (!strcmp(szPath, "helmet"))
				{
					it->second->sType = ITEMTYPE_HELMET;
				}
				else if (!strcmp(szPath, "armor"))
				{
					it->second->sType = ITEMTYPE_ARMOR;
				}
				else if (!strcmp(szPath, "material"))
				{
					it->second->sType = ITEMTYPE_MATERIAL;
				}
				else if (!strcmp(szPath, "expendable"))
				{
					it->second->sType = ITEMTYPE_EXPENDABLE;
				}
			}
		}
		else if (!strcmp(szPath, "value"))
		{
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			mapItemDataIter it = m_mapItemData.find(keyName);
			if (it != m_mapItemData.end())
			{
				it->second->fValue = atof(szPath);
			}
		}
		else if (!strcmp(szPath, "fireRate"))
		{
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			mapItemDataIter it = m_mapItemData.find(keyName);
			if (it != m_mapItemData.end())
			{
				it->second->st_FireRate = atof(szPath);
			}
		}
		else if (!strcmp(szPath, "Callbacktime"))
		{
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			mapItemDataIter it = m_mapItemData.find(keyName);
			if (it != m_mapItemData.end())
			{
				it->second->st_CallbackTime = atof(szPath);
			}
		}

		
	}
	
	fclose(fp);
}

void DataBase::LoadAIData()
{
	FILE* fp = NULL;
	fopen_s(&fp, "Data/AIData.txt", "r");

	string keyName;
	keyName.clear();

	mapTimePositionData mapTimeToPosData;
	mapTimeToPosData.clear();

	float fTimeTemp;
	D3DXVECTOR3 vPosTemp;

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		char szPath[1024] = { '\0', };

		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		sscanf_s(szBuf, "%s", szPath, 1024);

		if (!strcmp(szPath, "name"))
		{
			if (keyName.empty())
			{
				sscanf_s(szBuf, "%*s %s", szPath, 1024);
				keyName = szPath;
			}
			else
			{
				m_mapAIData.insert(make_pair(keyName, mapTimeToPosData));
				keyName.clear();
				mapTimeToPosData.clear();
			}
		}
		else if (!strcmp(szPath, "time"))
		{
			sscanf_s(szBuf, "%*s %f", &fTimeTemp);
		}
		else if (!strcmp(szPath, "position"))
		{
			float x, y, z;
			sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
			vPosTemp = D3DXVECTOR3(x, y, z);

			mapTimeToPosData.insert(make_pair(fTimeTemp, vPosTemp));
		}
	}

	m_mapAIData.insert(make_pair(keyName, mapTimeToPosData));

	fclose(fp);
}

float DataBase::GetItemValue(string itemName)
{
	mapItemDataIter it = m_mapItemData.find(itemName);
	if (it != m_mapItemData.end())
	{
		return it->second->fValue;
	}
}

float DataBase::GetItemFireRate(string itemName)
{
	mapItemDataIter it = m_mapItemData.find(itemName);
	if (it != m_mapItemData.end())
	{
		return it->second->st_FireRate;
	}
}

float DataBase::GetCallbacktime(string itemName)
{
	mapItemDataIter it = m_mapItemData.find(itemName);
	if (it != m_mapItemData.end())
	{
		return it->second->st_CallbackTime;
	}
}

D3DXVECTOR3 DataBase::GetPosition(float time, mapTimePositionData& mapTimeData)
{
	mapTimePositionDataIter prevIter;
	if (time < mapTimeData.begin()->first) return mapTimeData.begin()->second;

	for (mapTimePositionDataIter iter = mapTimeData.begin(); iter != mapTimeData.end(); iter++)
	{
		if (time > iter->first)
		{
			prevIter = iter;
		}
		else if (time < iter->first)
		{
			float t = (time - prevIter->first) / (iter->first - prevIter->first);
			D3DXVECTOR3 result;
			D3DXVec3Lerp(&result, &prevIter->second, &iter->second, t);
			return result;
		}
		else if (time == iter->first)
		{
			return iter->second;
		}
	}

	if (time > prevIter->first)
	{
		return prevIter->second;
	}
}

D3DXVECTOR3 DataBase::GetTimeToPosition(string aiName, float time)
{
	mapAIDataIter AINameIter = m_mapAIData.find(aiName);
	if (AINameIter != m_mapAIData.end())
	{
		D3DXVECTOR3 vPosition;
		vPosition = GetPosition(time, AINameIter->second);
		return vPosition;
	}
}
