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
	FILE* fp = 0;
	fopen_s(&fp, "Data/ItemData.txt", "r");

	while (!feof(fp))
	{
		char szBuf[1024] = { '\0', };
		char szPath[1024] = { '\0', };
		string keyName;

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
	}
	
	fclose(fp);
}

float DataBase::GetItemValue(string itemName)
{
	return 0.0f;
}
