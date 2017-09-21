#pragma once
/*
170916 시험제작

*/
class HankcGrid;

enum LOADING_KIND
{
	LOADING_KIND_TEST,
	LOADING_KIND_HEIGHT,
	LOADING_KIND_WAY,
	LOADING_KIND_WAY2,
	LOADING_KIND_MESH,
	LOADING_KIND_END
};
enum MESHTYPE
{
	MESH_NORMAL,
	MESH_HEAD,
	MESH_WEAPON
};
struct tagTestResource
{
	string sKeyName;
	int fWidth;
	int fHeight;
};
struct tagHeight
{

	string keyName;
	string szFolder;
	string szFile;
	string szTexture;
	DWORD  dwBytesPerPixel;
};
struct tagWay
{
	
	string keyName;
	HankcGrid*	Node;
	int StartX;
	int StartZ;
	int LastX;
	int LastZ;
};
struct tagWay2
{

	string keyName;
	HankcGrid*	Node;
	D3DXVECTOR3	start;
	D3DXVECTOR3	last;
};
struct tagMesh
{

	string keyName;
	char   Foldername[256];
	char   Filename[256];
	MESHTYPE	type;
};
class LoadItem
{
private:
	LOADING_KIND m_kind;

	tagTestResource m_testResource;
	tagHeight	    m_stTagHeight;
	tagWay			m_stTagWay;
	tagWay2			m_stTagWay2;
	tagMesh			m_stMesh;
public:
	HRESULT AddForTestResource(string keyName, int width, int height);
	HRESULT InitForHeightMap(string keyName, string szFolder, string szFile, string szTexture, DWORD  dwBytesPerPixel =1);
	HRESULT InitForWay(string keyName, HankcGrid*	Node, int StartX, int StartZ, int LastX, int LastZ);
	HRESULT InitForWay2(string keyName, HankcGrid*	Node, D3DXVECTOR3 start, D3DXVECTOR3 last);
	HRESULT InitForMesh(string keyName, MESHTYPE type, char* foldername, char* filename);

	
	
	int StartZ;
	int LastX;
	int LastZ;

	void Release(void);

	//로딩종류 접근자
	LOADING_KIND GetLoadingKind(void) { return m_kind; }

	//테스트용 리소스 접근자
	tagTestResource GetTestResource(void) { return m_testResource; }
	tagHeight		GetHeightMapResource(void) { return m_stTagHeight; }
	tagWay			GetWayResource(void) { return m_stTagWay; }
	tagWay2			GetWayResource2(void) { return m_stTagWay2; }
	tagMesh			GetMeshResource(void) { return m_stMesh; }
	LoadItem(void);
	~LoadItem(void);
};

class Loading
{
private:
	typedef vector<LoadItem*> arrLoadItem;
	typedef vector<LoadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem m_vecLoadItems;

	cUIObject*	m_LoadingBarRoot;
	LPD3DXFONT	m_pFont;
	int m_nCurrent;
	char	str[512];
	void SetGauge(float current, float max);

public:
	void Setup(void);
	void Release(void);
	void Update(void);
	void Render(void);

	void LoadTestResource(string keyName, int width, int height);
	void LoadHeightMap(string keyName, string szFolder, string szFile, string szTexture, DWORD  dwBytesPerPixel = 1);
	void LoadWay(string keyName, HankcGrid*	Node, int StartX, int StartZ, int LastX, int LastZ);
	void LoadWay2(string keyName, HankcGrid*	Node, D3DXVECTOR3 start, D3DXVECTOR3 last);
	void LoadMesh(string keyName,MESHTYPE type, char* folername, char* filename);
	BOOL LoadNext(void);
	
	Loading();
	~Loading();
};

