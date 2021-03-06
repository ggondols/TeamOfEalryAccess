#include "stdafx.h"
#include "Inventory.h"
#include "LDYCharacter.h"
#include "LDYCamera.h"
#include "DataBase.h"

Inventory::Inventory()
	: m_pSelectItem(NULL)
	, m_pCharacter(NULL)
	, m_sFileAddress("UI/Icon_")
	, m_sFileType(".png")
{
}

Inventory::~Inventory()
{
}

void Inventory::Setup()
{
	UIOBJECTMANAGER->AddRoot("inventory", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("inventory", "./UI/inventory.png");
	UIOBJECTMANAGER->SetAlpha("inventory", 200);
	UIOBJECTMANAGER->SetPosition("inventory", 0.05f, 0.15f);

	ST_SIZE inventorySize = UIOBJECTMANAGER->FindRoot("inventory")->GetSize();

	UIOBJECTMANAGER->AddRoot("equipment", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("equipment", "./UI/equipment.png");
	UIOBJECTMANAGER->SetAlpha("equipment", 200);
	//UIOBJECTMANAGER->SetPosition("equipment", 50.0f + inventorySize.fWidth, 20.0f);
	UIOBJECTMANAGER->SetPosition("equipment", 0.1f, 0.15f, inventorySize.fWidth);

	for (size_t row = 0; row < 5; row++)
	{
		for (size_t col = 0; col < 5; col++)
		{
			int index = (row * 5 + col) + 1;
			UIOBJECTMANAGER->AddChild("inventory", UITYPE_IMAGE);
			UIOBJECTMANAGER->SetPosition("inventory", index, col * 0.2f, row * 0.2f, 9.0f, 9.0f);
		}
	}

	for (size_t i = 1; i < 4; i++)
	{
		UIOBJECTMANAGER->AddChild("equipment", UITYPE_IMAGE);
		UIOBJECTMANAGER->SetPosition("equipment", i, 4 * 0.2f, (i - 1) * 0.2f, 9.0f, 9.0f);
	}
	
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_ARMOR, "FullArmor"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_WEAPON, "M4"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_HELMET, "Helmet"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_EXPENDABLE, "HealPack"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_EXPENDABLE, "HealPack"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_PART, "FullArmorPart1"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_PART, "FullArmorPart2"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_PART, "FullArmorPart3"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_PART, "HelmetPart1"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(1, ITEMTYPE_PART, "HelmetPart2"));

	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_HELMET, "Mask"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_ARMOR, "Armor"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_WEAPON, "MP5"));
}

void Inventory::Update(LDYCharacter* character)
{
	m_pCharacter = character;

	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		g_bCursor = !g_bCursor;
		bool showState = !UIOBJECTMANAGER->CheckShowState("inventory");
		UIOBJECTMANAGER->SetShowState("inventory", showState);
		UIOBJECTMANAGER->SetShowState("equipment", showState);
		if (g_bCursor == true)
		{
			int a = 0;
		}
	}

	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("inventory", i + 1, m_sFileAddress + m_vecItems[i].sName + m_sFileType);
	}

	for (size_t i = 0; i < m_vecEquipments.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("equipment", i + 1, m_sFileAddress + m_vecEquipments[i].sName + m_sFileType);
	}

	if (UIOBJECTMANAGER->CheckShowState("inventory") && KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		cUIObject* pSelectItem = UIOBJECTMANAGER->GetSelectChild("inventory");
		if (pSelectItem && m_vecItems[pSelectItem->GetTag() - 1].stType == ITEMTYPE_EXPENDABLE)
		{
			if (m_vecItems[pSelectItem->GetTag() - 1].sName == "HealPack")
			{
				m_pCharacter->m_iHP += 20;

				if (m_pCharacter->m_iHP >= m_pCharacter->m_iMaxHP) m_pCharacter->m_iHP = m_pCharacter->m_iMaxHP;
				if (m_pCharacter->m_iHP <= 0) m_pCharacter->m_iHP = 0;
				
				DeleteInventoryItem(pSelectItem->GetTag() - 1);
			}
		}
	}

	if (UIOBJECTMANAGER->CheckShowState("inventory") && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		m_pSelectItem = UIOBJECTMANAGER->GetSelectChild("inventory");
		if (m_pSelectItem)
		{
			m_ptPrevMouse = g_ptMouse;
			m_ptSavePosition = m_pSelectItem->GetPointPosition();
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) && m_pSelectItem)
	{
		cUIObject* pUpPositionInventory = UIOBJECTMANAGER->GetSelectChild("inventory", m_pSelectItem);
		cUIObject* pUpPositionEquipment = UIOBJECTMANAGER->GetSelectChild("equipment");
		if (pUpPositionInventory)
		{
			if (m_vecItems[m_pSelectItem->GetTag() - 1].stType == ITEMTYPE_PART &&
				m_vecItems[pUpPositionInventory->GetTag() - 1].stType == ITEMTYPE_PART)
			{
				CheckCombineItem(m_vecItems[m_pSelectItem->GetTag() - 1].sName,
					m_vecItems[pUpPositionInventory->GetTag() - 1].sName,
					m_pSelectItem->GetTag() - 1,
					pUpPositionInventory->GetTag() - 1);
			}
			else
			{
				ChangeInventoryItem(m_pSelectItem->GetTag() - 1, pUpPositionInventory->GetTag() - 1);
			}
		}
		
		if (pUpPositionEquipment)
		{
			if (m_vecItems[m_pSelectItem->GetTag() - 1].stType == m_vecEquipments[pUpPositionEquipment->GetTag() - 1].stType)
			{
				if (UpgradeEquipmentItem(pUpPositionEquipment->GetTag() - 1, m_vecItems[m_pSelectItem->GetTag() - 1].stType))
				{
					DeleteInventoryItem(m_pSelectItem->GetTag() - 1);
				}
				else
				{
					string itemName = m_vecItems[m_pSelectItem->GetTag() - 1].sName;
					m_vecItems[m_pSelectItem->GetTag() - 1].sName = m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName;
					m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName = itemName;
				}
			}

			for each(auto e in m_vecEquipments)
			{
				switch (e.stType)
				{
				case ITEMTYPE_WEAPON:
					if (e.sName == "AA12") m_pCharacter->SetWeaponType(Wp_AA12);
					else if (e.sName == "AR6") m_pCharacter->SetWeaponType(Wp_AR6);
					else if (e.sName == "M4") m_pCharacter->SetWeaponType(Wp_M4);
					else if (e.sName == "MP5") m_pCharacter->SetWeaponType(Wp_MP5);
					else if (e.sName == "FireGun") m_pCharacter->SetWeaponType(WP_FireGun);
					break;
				case ITEMTYPE_HELMET:
					if (e.sName == "Mask") m_pCharacter->SetHeadLv(0);
					else if (e.sName == "Helmet") m_pCharacter->SetHeadLv(1);
					break;
				case ITEMTYPE_ARMOR:
					if (e.sName == "Armor") m_pCharacter->SetBodyLv(0);
					else if (e.sName == "FullArmor1") m_pCharacter->SetBodyLv(1);
					else if (e.sName == "FullArmor2") m_pCharacter->SetBodyLv(2);
					else if (e.sName == "FullArmor3") m_pCharacter->SetBodyLv(3);
					break;
				}
			}
		}

		m_pSelectItem->SetPosition(m_ptSavePosition.x, m_ptSavePosition.y);
		m_pSelectItem = NULL;
	}

	if (m_pSelectItem)
	{
		POINT ptCurrentMouse;
		POINT ptAddPoint;

		ptAddPoint.x = (g_ptMouse.x - m_ptPrevMouse.x);
		ptAddPoint.y = (g_ptMouse.y - m_ptPrevMouse.y);
		m_pSelectItem->SetPosition(m_pSelectItem->GetPointPosition().x + ptAddPoint.x,
			m_pSelectItem->GetPointPosition().y + ptAddPoint.y);

		m_ptPrevMouse = g_ptMouse;
	}
}

void Inventory::Render()
{
	if (m_pCharacter && UIOBJECTMANAGER->CheckShowState("inventory")) m_pCharacter->UpdateAndRender();
}

void Inventory::AddItem(string itemName, ITEM_TYPE type, int count)
{
	if (m_vecItems.size() > 25) return;

	bool bHaveItem = false;

	//for (size_t i = 0; i < m_vecItems.size(); i++)
	//{
	//	if (m_vecItems[i].sName == itemName)
	//	{
	//		bHaveItem = true;
	//		m_vecItems[i].nItemCount += count;
	//		break;
	//	}
	//}

	if (!bHaveItem)
	{
		m_vecItems.push_back(ST_INVENTORY_ITEM(count, type, itemName));
	}
}

void Inventory::SubItem(string itemName, int count)
{
	if (!CheckHaveItem(itemName)) return;

	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		if (m_vecItems[i].sName == itemName)
		{
			m_vecItems[i].nItemCount -= count;
			break;
		}
	}
}

bool Inventory::CheckHaveItem(string itemName)
{
	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		if (m_vecItems[i].sName == itemName)
		{
			return true;
		}
	}
	return false;
}

void Inventory::ChangeInventoryItem(int selectItem, int targetItem)
{
	ST_INVENTORY_ITEM temp = m_vecItems[selectItem];
	m_vecItems[selectItem] = m_vecItems[targetItem];
	m_vecItems[targetItem] = temp;
}

void Inventory::CombineInventoryItem(int selectItem, int targetItem, string combineName)
{
	int min = selectItem;
	int max = targetItem;
	if (selectItem > targetItem)
	{
		min = targetItem;
		max = selectItem;
	}

	m_vecItems[min].sName = combineName;
	if (combineName == "FullArmor") m_vecItems[min].stType = ITEMTYPE_ARMOR;
	else if (combineName == "Helmet") m_vecItems[min].stType = ITEMTYPE_HELMET;
	m_vecItems[max].ClearInventory();
	DeleteInventoryItem(max);
}

bool Inventory::UpgradeEquipmentItem(int targetItem, ITEM_TYPE type)
{
	bool isUpgrade = true;

	switch (type)
	{
	case ITEMTYPE_HELMET:
		if (m_vecEquipments[targetItem].sName == "Mask")
		{
			m_vecEquipments[targetItem].sName = "Helmet";
		}
		break;
	case ITEMTYPE_ARMOR:
		if (m_vecEquipments[targetItem].sName == "Armor")
		{
			m_vecEquipments[targetItem].sName = "FullArmor1";
		}
		else if (m_vecEquipments[targetItem].sName == "FullArmor1")
		{
			m_vecEquipments[targetItem].sName = "FullArmor2";
		}
		else if (m_vecEquipments[targetItem].sName == "FullArmor2")
		{
			m_vecEquipments[targetItem].sName = "FullArmor3";
		}
		break;
	case ITEMTYPE_WEAPON:
		isUpgrade = false;
		break;
	}

	return isUpgrade;
}

void Inventory::CheckCombineItem(string selectName, string targetName, int selectItem, int targetItem)
{
	if (selectName == "FullArmorPart1")
	{
		if (targetName == "FullArmorPart2")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart1, 2");
		}
		else if (targetName == "FullArmorPart3")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart1, 3");
		}
		else if (targetName == "FullArmorPart2, 3")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "FullArmorPart2")
	{
		if (targetName == "FullArmorPart1")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart1, 2");
		}
		else if (targetName == "FullArmorPart3")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart2, 3");
		}
		else if (targetName == "FullArmorPart1, 3")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "FullArmorPart3")
	{
		if (targetName == "FullArmorPart1")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart1, 3");
		}
		else if (targetName == "FullArmorPart2")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmorPart2, 3");
		}
		else if (targetName == "FullArmorPart1, 2")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "FullArmorPart1, 2")
	{
		if (targetName == "FullArmorPart3")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "FullArmorPart1, 3")
	{
		if (targetName == "FullArmorPart2")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "FullArmorPart2, 3")
	{
		if (targetName == "FullArmorPart1")
		{
			CombineInventoryItem(selectItem, targetItem, "FullArmor");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "HelmetPart1")
	{
		if (targetName == "HelmetPart2")
		{
			CombineInventoryItem(selectItem, targetItem, "Helmet");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
	else if (selectName == "HelmetPart2")
	{
		if (targetName == "HelmetPart1")
		{
			CombineInventoryItem(selectItem, targetItem, "Helmet");
		}
		else
		{
			ChangeInventoryItem(selectItem, targetItem);
		}
	}
}

void Inventory::DeleteInventoryItem(int index)
{
	for (size_t i = index; i < m_vecItems.size() - 1; i++)
	{
		ChangeInventoryItem(i, i + 1);
	}

	UIOBJECTMANAGER->SetTexture("inventory", m_vecItems.size(), "null");
	m_vecItems.pop_back();
}
