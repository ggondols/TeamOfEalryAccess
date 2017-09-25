#pragma once
#include "stdafx.h"

class cUIObject;
class LDYCharacter;
class LDYCamera;

struct ST_INVENTORY_ITEM
{
	int nInventoryTag;
	int	nItemCount;
	string sName;
	ITEM_TYPE stType;

	ST_INVENTORY_ITEM() {}
	ST_INVENTORY_ITEM(int tag, int count, string name):nInventoryTag(tag), nItemCount(count), sName(name), stType(ITEMTYPE_END){}
	ST_INVENTORY_ITEM(int tag, int count, ITEM_TYPE type, string name) :nInventoryTag(tag), nItemCount(count), sName(name), stType(type) {}
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
	POINT						m_ptSavePosition;

public:
	Inventory();
	~Inventory();

	void Setup();
	void Update(LDYCamera* chamera, LDYCharacter* character);
	void Render();

	void AddItem(string itemName, int count = 1);
	void SubItem(string itemName, int count = 1);
	bool CheckHaveItem(string itemName);

	void ChangeEquipment(string partName, string addPartName);

	vector<ST_INVENTORY_ITEM>& GetItems() { return m_vecItems; }
	vector<ST_EQUIPMENT_ITEM>& GetEquipment() { return m_vecEquipments; }
};

