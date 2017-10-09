#pragma once
#include "stdafx.h"

class cUIObject;
class LDYCharacter;
class LDYCamera;

struct ST_INVENTORY_ITEM
{
	int	nItemCount;
	string sName;
	ITEM_TYPE stType;

	ST_INVENTORY_ITEM() {}
	ST_INVENTORY_ITEM(int count, string name) : nItemCount(count), sName(name), stType(ITEMTYPE_NONE){}
	ST_INVENTORY_ITEM(int count, ITEM_TYPE type, string name) : nItemCount(count), sName(name), stType(type) {}
	void ClearInventory()
	{
		nItemCount = 0;
		sName = "Empty";
		stType = ITEMTYPE_NONE;
	}
};

struct ST_EQUIPMENT_ITEM
{
	string sName;
	ITEM_TYPE stType;

	ST_EQUIPMENT_ITEM() {}
	ST_EQUIPMENT_ITEM(ITEM_TYPE type, string name) :stType(type), sName(name) {}
};

class Inventory
{
private:
	LDYCharacter*				m_pCharacter;

	vector<ST_INVENTORY_ITEM>	m_vecItems;
	vector<ST_EQUIPMENT_ITEM>	m_vecEquipments;
	cUIObject*					m_pSelectItem;
	POINT						m_ptPrevMouse;
	POINT						m_ptSavePosition;

	string						m_sFileAddress = "UI/Icon_";
	string						m_sFileType = ".png";

	void ChangeInventoryItem(int selectItem, int targetItem);
	void CombineInventoryItem(int selectItem, int targetItem, string combineName);
	bool UpgradeEquipmentItem(int targetItem, ITEM_TYPE type);
	void CheckCombineItem(string selectName, string targetName, int selectItem, int targetItem);
	void DeleteInventoryItem(int index);

public:
	Inventory();
	~Inventory();

	void Setup();
	void Update(LDYCharacter* character);
	void Render();

	void AddItem(string itemName, ITEM_TYPE type, int count = 1);
	void SubItem(string itemName, int count = 1);
	bool CheckHaveItem(string itemName);

	vector<ST_INVENTORY_ITEM>& GetItems() { return m_vecItems; }
	vector<ST_EQUIPMENT_ITEM>& GetEquipment() { return m_vecEquipments; }
};

