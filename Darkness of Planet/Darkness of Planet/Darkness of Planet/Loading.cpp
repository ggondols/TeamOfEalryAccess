#include "stdafx.h"
#include "Loading.h"
#include "cUIImageView.h"
#include "HankcNode.h"
#include "cSoundLoader.h"
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

HRESULT LoadItem::InitForWay2(string keyName, HankcGrid * Node, D3DXVECTOR3 start, D3DXVECTOR3 last)
{
	m_kind = LOADING_KIND_WAY2;



	m_stTagWay2.keyName = keyName;
	m_stTagWay2.Node = Node;
	m_stTagWay2.start = start;
	m_stTagWay2.last = last;

	return S_OK;
}

HRESULT LoadItem::InitForMesh(string keyName, MESHTYPE type, char * foldername, char * filename)
{
	m_kind = LOADING_KIND_MESH;
	m_stMesh.keyName = keyName;
	m_stMesh.type = type;
	strcpy_s(m_stMesh.Foldername,256, foldername);
	strcpy_s(m_stMesh.Filename, 256, filename);
	
	return S_OK;
}

HRESULT LoadItem::InitForSound(tagSoundList &sm)
{
	m_kind = LOADING_KIND_SOUND;
	m_stSound = sm;
	return S_OK;
}

void LoadItem::Release(void)
{
}

Loading::Loading(void)
	:m_nCurrent(0)
	, m_pFont(NULL)
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

	/*cUIImageView* pLodingBgImage = new cUIImageView;
	pLodingBgImage->SetTexture("./UI/loadingBg.png");
	UIOBJECTMANAGER->AddRoot("LoadingBg", pLodingBgImage, true);*/

	UIOBJECTMANAGER->AddRoot("LoadingBar", pLoadingImageDown, true);
	UIOBJECTMANAGER->AddChild("LoadingBar", pLoadingImageUp);

	UIOBJECTMANAGER->AddRoot("LoadingText", UITYPE_TEXT, true);
	UIOBJECTMANAGER->SetPosition("LoadingText", 0.5f, 0.85f);
	UIOBJECTMANAGER->SetDrawTextFormat("LoadingText", DT_CENTER | DT_TOP | DT_NOCLIP);
	UIOBJECTMANAGER->SetTextColor("LoadingText", D3DCOLOR_XRGB(218, 149, 31));
	//0, 0, 0
	//218, 149, 31
	//89, 231, 53
	UIOBJECTMANAGER->SetFontType("LoadingText", cFontManager::E_LODING);


	UIOBJECTMANAGER->AddRoot("StoryText", UITYPE_TEXT, true);
	UIOBJECTMANAGER->SetPosition("StoryText", 0.01f, 0.3f);
	UIOBJECTMANAGER->SetDrawTextFormat("StoryText", DT_LEFT | DT_TOP | DT_NOCLIP);
	UIOBJECTMANAGER->SetTextColor("StoryText", D3DCOLOR_XRGB(255, 255, 255));
	//0, 0, 0
	//218, 149, 31
	//89, 231, 53
	UIOBJECTMANAGER->SetFontType("StoryText", cFontManager::E_LODING);


	m_pFont = FONTMANAGER->GetFont(cFontManager::E_LODING);
	
	sprintf_s(m_cStory, "서기 2110년 우주로 진출한\n"
		"인류가 활발한 활동을 \n"
		"이어가는 가운데 주인공\n"
		"진철은 위험지정 생물\n"
		"E-T13호의 본 행성 운송을\n"
		"위한 항해 중 UT - 23성운의\n"
		"갑작스런 운석 충돌에 맞아\n"
		"고장 난 운송선에서 가까스로\n"
		"탈출용 포드를 타고\n"
		"탈출 하게 된다.");


	m_iLength = 0;
	m_fTime = 0;
	m_fTimeSound = 0;
}

void Loading::Release(void)
{
	for (arrLoadItemIter iter = m_vecLoadItems.begin(); iter != m_vecLoadItems.end(); ++iter)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}

//	UIOBJECTMANAGER->ReleaseRoot("LoadingBg");
	UIOBJECTMANAGER->ReleaseRoot("LoadingBar");
	UIOBJECTMANAGER->ReleaseRoot("LoadingText");
	UIOBJECTMANAGER->ReleaseRoot("StoryText");
}

void Loading::Update(void)
{
	//	UIOBJECTMANAGER->Update("LoadingBg");
	UIOBJECTMANAGER->Update("LoadingBar");
	UIOBJECTMANAGER->Update("LoadingText");
	
}

void Loading::Render(void)
{
//	UIOBJECTMANAGER->Render("LoadingBg");
	UIOBJECTMANAGER->Render("LoadingBar");
	UIOBJECTMANAGER->Render("LoadingText");
	UIOBJECTMANAGER->Render("StoryText");
	
	//RECT rc = RectMake(200, 500, 1000, 1000);
	//m_pFont->DrawTextA(NULL, str, strlen(str), &rc, DT_CENTER, D3DCOLOR_XRGB(0, 0, 0));
}

void Loading::UpdateStory()
{
	UIOBJECTMANAGER->Update("StoryText");
	if (TIMEMANAGER->getWorldTime() > m_fTime + 0.05)
	{
		m_fTime = TIMEMANAGER->getWorldTime();
		if (TIMEMANAGER->getWorldTime() > m_fTimeSound + 3)
		{
			SOUNDMANAGER->stop("Taja");
			m_fTimeSound = TIMEMANAGER->getWorldTime();
			SOUNDMANAGER->play("Taja");
		}
		
		m_iLength += 1;
		if (m_iLength > strlen(m_cStory))
		{
			SOUNDMANAGER->stop("Taja");
			m_iLength = strlen(m_cStory);
			m_fTime = INF;
		}
	}

	strncpy_s(m_cStoryShow, 1024, m_cStory, m_iLength);

	UIOBJECTMANAGER->SetText("StoryText", m_cStoryShow);
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

void Loading::LoadWay2(string keyName, HankcGrid * Node, D3DXVECTOR3 start, D3DXVECTOR3 last)
{
	LoadItem* item = new LoadItem;
	item->InitForWay2(keyName, Node, start,last);
	m_vecLoadItems.push_back(item);
}

void Loading::LoadMesh(string keyName,MESHTYPE type, char * folername, char * filename)
{
	LoadItem* item = new LoadItem;
	item->InitForMesh(keyName, type,folername,filename);
	m_vecLoadItems.push_back(item);
}

void Loading::LoadSound(tagSoundList& sm)
{
	LoadItem* item = new LoadItem;
	item->InitForSound(sm);
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
			//sprintf_s(str, "하이트 맵 로딩 중");
			sprintf_s(str, "HeightMap Loading...");
			tagHeight ir = item->GetHeightMapResource();
			HEIGHTMAPMANAGER->AddHeightMap(ir.keyName, ir.szFolder, ir.szFile, ir.szTexture, ir.dwBytesPerPixel);
			break;
		}
		case LOADING_KIND_WAY:
		{
			//sprintf_s(str, "몬스터 경로를 미리 찾아 놓는 중");
			sprintf_s(str, "Monster's Way Loading...");
			tagWay ir = item->GetWayResource();
			WAYMANAGER->AddWay(ir.keyName.c_str(), ir.Node, ir.StartX, ir.StartZ, ir.LastX, ir.LastZ);
			break;
		}
		case LOADING_KIND_WAY2:
		{
			//sprintf_s(str, "몬스터 경로를 미리 찾아 놓는 중");
			sprintf_s(str, "Monster's Way Loading...");
			tagWay2 ir = item->GetWayResource2();
			WAYMANAGER->AddWay2(ir.keyName.c_str(), ir.Node, ir.start, ir.last);
			break;
		}
		case LOADING_KIND_MESH:
		{
			//sprintf_s(str, "메쉬 불러오는 중");
			sprintf_s(str, "Mesh Loading...");
			tagMesh ir = item->GetMeshResource();
			if (ir.type == MESH_NORMAL)
			{
				MESHLOADER->AddSkinnedMesh(ir.keyName.c_str(), ir.Foldername, ir.Filename);
			}
			else if(ir.type ==MESH_HEAD)
			{
				MESHLOADER->AddSkinnedMeshHead(ir.keyName.c_str(), ir.Foldername, ir.Filename);
			}
			else if (ir.type == MESH_WEAPON)
			{
				MESHLOADER->AddSkinnedWeapon(ir.keyName.c_str(), ir.Foldername, ir.Filename);
			}
			else if (ir.type == MESH_TEI)
			{
				MESHLOADER->AddSkinnedMeshTei(ir.keyName.c_str(), ir.Foldername, ir.Filename);
			}
			break;
		}
		case LOADING_KIND_SOUND:
		{
			//sprintf_s(str, "사운드 불러오는 중");
			sprintf_s(str, "Sound Loading...");
			tagSoundList ir = item->GetSoundResource();
			
			SOUNDMANAGER->addSound(ir._soundKey, ir._soundPath, ir._soundName, ir._bgm, ir._loop);

			break;
		}
	}

	UIOBJECTMANAGER->SetText("LoadingText", str);

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