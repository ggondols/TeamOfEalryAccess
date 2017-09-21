#include "stdafx.h"
#include "Loading.h"
#include "cUIImageView.h"

LoadItem::LoadItem(void)
{
}

LoadItem::~LoadItem(void)
{
}

HRESULT LoadItem::AddForTestResource(string keyName, int width, int height)
{
	m_kind = LOADING_KIND_TEST;

	memset(&m_testResource, 0, sizeof(tagTestResource));

	m_testResource.sKeyName = keyName;
	m_testResource.fWidth = width;
	m_testResource.fHeight = height;

	return S_OK;
}

HRESULT LoadItem::InitForHeightMap(string keyName, string szFolder, string szFile, string szTexture, DWORD dwBytesPerPixel  /*=1*/)
{
	m_kind = LOADING_KIND_HEIGHT;

	

	m_stTagHeight.keyName = keyName;
	m_stTagHeight.szFile = szFile;
	m_stTagHeight.szFolder = szFolder;
	m_stTagHeight.szTexture = szTexture;
	m_stTagHeight.dwBytesPerPixel = dwBytesPerPixel;

	return S_OK;
}

HRESULT LoadItem::InitForWay(string keyName, HankcGrid * Node, int StartX, int StartZ, int LastX, int LastZ)
{
	m_kind = LOADING_KIND_WAY;



	m_stTagWay.keyName = keyName;
	m_stTagWay.Node = Node;
	m_stTagWay.StartX = StartX;
	m_stTagWay.StartZ = StartZ;
	m_stTagWay.LastX = LastX;
	m_stTagWay.LastZ = LastZ;
	return S_OK;
}

void LoadItem::Release(void)
{
}

Loading::Loading(void)
	:m_nCurrent(0)
{
}

Loading::~Loading(void)
{
}

void Loading::Setup(void)
{
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	cUIImageView* pLoadingImageDown = new cUIImageView;
	pLoadingImageDown->SetTexture("./UI/lifeBarDown.bmp");
	pLoadingImageDown->SetPosition((viewport.Width / 2) - (pLoadingImageDown->GetSize().fWidth / 2), viewport.Height - 30);
	
	cUIImageView* pLoadingImageUp = new cUIImageView;
	pLoadingImageUp->SetTexture("./UI/lifeBarUp.bmp");
	pLoadingImageUp->SetSizeWidth(0);

	UIOBJECTMANAGER->AddRoot("LoadingBar", pLoadingImageDown, true);
	UIOBJECTMANAGER->AddChild("LoadingBar", pLoadingImageUp);
}

void Loading::Release(void)
{
	for (arrLoadItemIter iter = m_vecLoadItems.begin(); iter != m_vecLoadItems.end(); ++iter)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}

	UIOBJECTMANAGER->ReleaseRoot("LoadingBar");
}

void Loading::Update(void)
{
	UIOBJECTMANAGER->Update("LoadingBar");
}

void Loading::Render(void)
{
	UIOBJECTMANAGER->Render("LoadingBar");
}

void Loading::LoadTestResource(string keyName, int width, int height)
{
	LoadItem* item = new LoadItem;
	item->AddForTestResource(keyName, width, height);
	m_vecLoadItems.push_back(item);
}

void Loading::LoadHeightMap(string keyName, string szFolder, string szFile, string szTexture, DWORD dwBytesPerPixel /*=1*/)
{
	
	LoadItem* item = new LoadItem;
	item->InitForHeightMap(keyName, szFolder, szFile, szTexture, dwBytesPerPixel);
	m_vecLoadItems.push_back(item);
}

void Loading::LoadWay(string keyName, HankcGrid * Node, int StartX, int StartZ, int LastX, int LastZ)
{
	LoadItem* item = new LoadItem;
	item->InitForWay( keyName,  Node,  StartX,  StartZ,  LastX,  LastZ);
	m_vecLoadItems.push_back(item);
}

BOOL Loading::LoadNext(void)
{
	if (m_nCurrent >= m_vecLoadItems.size())
	{
		return false;
	}

	LoadItem* item = m_vecLoadItems[m_nCurrent];
	
	switch (item->GetLoadingKind())
	{
	case LOADING_KIND_TEST:
	{
		// 예시.. 
		//tagImageResource ir = item->getImageResource();
		//IMAGEMANAGER->addImage(ir.keyName, ir.width, ir.height);
		break;
	}
	case LOADING_KIND_HEIGHT:
	{
		tagHeight ir = item->GetHeightMapResource();
		HEIGHTMAPMANAGER->AddHeightMap(ir.keyName, ir.szFolder, ir.szFile, ir.szTexture, ir.dwBytesPerPixel);
		break;
	}
	case LOADING_KIND_WAY:
	{
		tagWay ir = item->GetWayResource();
		//WAYMANAGER->AddWay("SX숫자SZ숫자LX숫자LZ숫자")
		//HEIGHTMAPMANAGER->AddHeightMap(ir.keyName, ir.szFolder, ir.szFile, ir.szTexture, ir.dwBytesPerPixel);
		break;
	}
	//case LOADING_KIND_ADDIMAGE_01:
	//{
	//	tagImageResource ir = item->getImageResource();
	//	IMAGEMANAGER->addImage(ir.keyName, ir.fileName, ir.x, ir.y, ir.width, ir.height, ir.trans, ir.transColor);
	//}
	//break;
	}

	SetGauge((float)m_nCurrent, m_vecLoadItems.size());

	m_nCurrent++;

	return true;
}

void Loading::SetGauge(float current, float max)
{
	//_width = (current / max) * _gaugeDown->getWidth();
	cUIObject* currentGauge = UIOBJECTMANAGER->GetChildByTag("LoadingBar", 1);
	currentGauge->SetSizeWidth((current / max) * UIOBJECTMANAGER->FindRoot("LoadingBar")->GetSize().fWidth);
}