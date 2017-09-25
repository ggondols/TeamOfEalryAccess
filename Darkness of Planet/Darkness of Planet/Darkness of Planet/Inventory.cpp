#include "stdafx.h"
#include "Inventory.h"
#include "LDYCharacter.h"
#include "LDYCamera.h"

Inventory::Inventory()
	: m_pSelectItem(NULL)
	, m_pCharacter(NULL)
{
}

Inventory::~Inventory()
{
	//SAFE_DELETE(m_pCharacter);
}

void Inventory::Setup()
{
	m_pCharacter = new LDYCharacter;
	char* BodyName = "HeroBodyLv";
	char buff[1024];
	sprintf_s(buff, "%s%d", BodyName, 1);
	m_pCharacter->Setup("object/xFile/", "HeroBodyLv");
	m_pCharacter->SetPosition(D3DXVECTOR3(0, 0, 10));

	UIOBJECTMANAGER->AddRoot("inventory", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("inventory", "./UI/inventory.png");
	UIOBJECTMANAGER->SetAlpha("inventory", 200);
	UIOBJECTMANAGER->SetPosition("inventory", 50.0f, 20.0f);

	ST_SIZE inventorySize = UIOBJECTMANAGER->FindRoot("inventory")->GetSize();

	UIOBJECTMANAGER->AddRoot("equipment", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("equipment", "./UI/equipment.png");
	UIOBJECTMANAGER->SetAlpha("equipment", 200);
	UIOBJECTMANAGER->SetPosition("equipment", 50.0f + inventorySize.fWidth, 20.0f);

	for (size_t row = 0; row < 5; row++)
	{
		for (size_t col = 0; col < 5; col++)
		{
			int index = (row * 5 + col) + 1;
			UIOBJECTMANAGER->AddChild("inventory", UITYPE_IMAGE);
			UIOBJECTMANAGER->SetPosition("inventory", index, col * 100.0f + 9.0f, row * 100.0f + 9.0f);
		}
	}

	for (size_t i = 1; i < 4; i++)
	{
		UIOBJECTMANAGER->AddChild("equipment", UITYPE_IMAGE);
		UIOBJECTMANAGER->SetPosition("equipment", i, 4 * 100.0f + 9.0f, (i - 1) * 100.0f + 9.0f);
	}

	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_ARMOR, "FullArmor"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_WEAPON, "M4"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_HELMET, "Helmet"));

	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_HELMET, "Mask"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_ARMOR, "Armor"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_WEAPON, "MP5"));
}

void Inventory::Update(LDYCamera* camera, LDYCharacter* character)
{
	
	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		bool showState = !UIOBJECTMANAGER->CheckShowState("inventory");
		UIOBJECTMANAGER->SetShowState("inventory", showState);
		UIOBJECTMANAGER->SetShowState("equipment", showState);
	}

	string sFileAddress = "UI/Icon_";
	string sFileType = ".png";
	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("inventory", i + 1, sFileAddress + m_vecItems[i].sName + sFileType);
	}

	for (size_t i = 0; i < m_vecEquipments.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("equipment", i + 1, sFileAddress + m_vecEquipments[i].sName + sFileType);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
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
			ST_INVENTORY_ITEM temp = m_vecItems[m_pSelectItem->GetTag() - 1];
			m_vecItems[m_pSelectItem->GetTag() - 1] = m_vecItems[pUpPositionInventory->GetTag() - 1];
			m_vecItems[pUpPositionInventory->GetTag() - 1] = temp;
		}
		
		if (pUpPositionEquipment)
		{
			if (m_vecItems[m_pSelectItem->GetTag() - 1].stType == m_vecEquipments[pUpPositionEquipment->GetTag() - 1].stType)
			{
				string tempName = m_vecItems[m_pSelectItem->GetTag() - 1].sName;
				m_vecItems[m_pSelectItem->GetTag() - 1].sName = m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName;
				m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName = tempName;
			}

			for each(auto e in m_vecEquipments)
			{
				if (e.sName == "MP5") character->SetWeaponType(Wp_MP5);
				else if (e.sName == "M4") character->SetWeaponType(Wp_M4);
				else if (e.sName == "Mask") character->SetHeadLv(0);
				else if (e.sName == "Helmet") character->SetHeadLv(1);
				else if (e.sName == "Armor") character->SetBodyLv(0);
				else if (e.sName == "FullArmor") character->SetBodyLv(2);
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

	//for each(auto e in m_vecEquipments)
	//{
	//	if (e.sName == "MP5") character->SetWeaponType(Wp_MP5);
	//	else if (e.sName == "M4") character->SetWeaponType(Wp_M4);
	//	else if (e.sName == "Mask") character->SetHeadLv(0);
	//	else if (e.sName == "Helmet") character->SetHeadLv(1);
	//	else if (e.sName == "Armor") character->SetBodyLv(0);
	//	else if (e.sName == "Armor") character->SetBodyLv(2);
	//}
}

void Inventory::Render()
{
	//if (m_pCharacter && UIOBJECTMANAGER->CheckShowState("inventory")) m_pCharacter->UpdateAndRender();
}

void Inventory::AddItem(string itemName, int count)
{
	if (m_vecItems.size() >= 25) return;

	bool bHaveItem = false;

	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		if (m_vecItems[i].sName == itemName)
		{
			bHaveItem = true;
			m_vecItems[i].nItemCount += count;
			break;
		}
	}

	if (!bHaveItem)
	{
		m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), count, itemName));
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

void Inventory::ChangeEquipment(string partName, string addPartName)
{
}

void Inventory::Setup()
{
	UIOBJECTMANAGER->AddRoot("inventory", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("inventory", "./UI/inventory.png");
	UIOBJECTMANAGER->SetAlpha("inventory", 200);
	UIOBJECTMANAGER->SetPosition("inventory", 50.0f, 20.0f);

	ST_SIZE inventorySize = UIOBJECTMANAGER->FindRoot("inventory")->GetSize();

	UIOBJECTMANAGER->AddRoot("equipment", UITYPE_IMAGE, false);
	UIOBJECTMANAGER->SetTexture("equipment", "./UI/equipment.png");
	UIOBJECTMANAGER->SetAlpha("equipment", 200);
	UIOBJECTMANAGER->SetPosition("equipment", 50.0f + inventorySize.fWidth, 20.0f);

	for (size_t row = 0; row < 5; row++)
	{
		for (size_t col = 0; col < 5; col++)
		{
			int index = (row * 5 + col) + 1;
			UIOBJECTMANAGER->AddChild("inventory", UITYPE_IMAGE);
			UIOBJECTMANAGER->SetPosition("inventory", index, col * 100.0f + 9.0f, row * 100.0f + 9.0f);
		}
	}

	for (size_t i = 1; i < 4; i++)
	{
		UIOBJECTMANAGER->AddChild("equipment", UITYPE_IMAGE);
		UIOBJECTMANAGER->SetPosition("equipment", i, 4 * 100.0f + 9.0f, (i - 1) * 100.0f + 9.0f);
	}

	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_ARMOR, "FullArmor"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_WEAPON, "M4"));
	m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), 1, ITEMTYPE_HELMET, "Helmet"));

	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_HELMET, "Mask"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_ARMOR, "Armor"));
	m_vecEquipments.push_back(ST_EQUIPMENT_ITEM(ITEMTYPE_WEAPON, "MP5"));
}

void Inventory::Update(LDYCamera* camera, LDYCharacter* character)
{
	m_pCharacter = character;

	bool showState = UIOBJECTMANAGER->CheckShowState("inventory");
	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		UIOBJECTMANAGER->SetShowState("inventory", !showState);
		UIOBJECTMANAGER->SetShowState("equipment", !showState);
	}

	string sFileAddress = "UI/Icon_";
	string sFileType = ".png";
	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("inventory", i + 1, sFileAddress + m_vecItems[i].sName + sFileType);
	}

	for (size_t i = 0; i < m_vecEquipments.size(); i++)
	{
		UIOBJECTMANAGER->SetTexture("equipment", i + 1, sFileAddress + m_vecEquipments[i].sName + sFileType);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && showState)
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
			ST_INVENTORY_ITEM temp = m_vecItems[m_pSelectItem->GetTag() - 1];
			m_vecItems[m_pSelectItem->GetTag() - 1] = m_vecItems[pUpPositionInventory->GetTag() - 1];
			m_vecItems[pUpPositionInventory->GetTag() - 1] = temp;
		}
		
		if (pUpPositionEquipment)
		{
			if (m_vecItems[m_pSelectItem->GetTag() - 1].stType == m_vecEquipments[pUpPositionEquipment->GetTag() - 1].stType)
			{
				string tempName = m_vecItems[m_pSelectItem->GetTag() - 1].sName;
				m_vecItems[m_pSelectItem->GetTag() - 1].sName = m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName;
				m_vecEquipments[pUpPositionEquipment->GetTag() - 1].sName = tempName;
			}

			for each(auto e in m_vecEquipments)
			{
				if (e.sName == "MP5") m_pCharacter->SetWeaponType(Wp_MP5);
				else if (e.sName == "M4") m_pCharacter->SetWeaponType(Wp_M4);
				else if (e.sName == "Mask") m_pCharacter->SetHeadLv(0);
				else if (e.sName == "Helmet") m_pCharacter->SetHeadLv(1);
				else if (e.sName == "Armor") m_pCharacter->SetBodyLv(0);
				else if (e.sName == "FullArmor") m_pCharacter->SetBodyLv(2);
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

	//for each(auto e in m_vecEquipments)
	//{
	//	if (e.sName == "MP5") character->SetWeaponType(Wp_MP5);
	//	else if (e.sName == "M4") character->SetWeaponType(Wp_M4);
	//	else if (e.sName == "Mask") character->SetHeadLv(0);
	//	else if (e.sName == "Helmet") character->SetHeadLv(1);
	//	else if (e.sName == "Armor") character->SetBodyLv(0);
	//	else if (e.sName == "Armor") character->SetBodyLv(2);
	//}
}

void Inventory::Render()
{
	if (m_pCharacter && UIOBJECTMANAGER->CheckShowState("inventory"))
	{
		//UIOBJECTMANAGER->SetShowState("equipment", false);
		m_pCharacter->UpdateAndRender();
		//UIOBJECTMANAGER->SetShowState("equipment", true);
	}
}

void Inventory::AddItem(string itemName, int count)
{
	if (m_vecItems.size() >= 25) return;

	bool bHaveItem = false;

	for (size_t i = 0; i < m_vecItems.size(); i++)
	{
		if (m_vecItems[i].sName == itemName)
		{
			bHaveItem = true;
			m_vecItems[i].nItemCount += count;
			break;
		}
	}

	if (!bHaveItem)
	{
		m_vecItems.push_back(ST_INVENTORY_ITEM(m_vecItems.size(), count, itemName));
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

void Inventory::ChangeEquipment(string partName, string addPartName)
{
}
