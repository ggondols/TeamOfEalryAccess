#pragma once
/*
170916 ��������

*/
enum LOADING_KIND
{
	LOADING_KIND_TEST,
	LOADING_KIND_END
};

struct tagTestResource
{
	string sKeyName;
	int fWidth;
	int fHeight;
};

class LoadItem
{
private:
	LOADING_KIND m_kind;

	tagTestResource m_testResource;

public:
	HRESULT AddForTestResource(string keyName, int width, int height);

	void Release(void);

	//�ε����� ������
	LOADING_KIND GetLoadingKind(void) { return m_kind; }

	//�׽�Ʈ�� ���ҽ� ������
	tagTestResource GetTestResource(void) { return m_testResource; }

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

	int m_nCurrent;

	void SetGauge(float current, float max);

public:
	void Setup(void);
	void Release(void);
	void Update(void);
	void Render(void);

	void LoadTestResource(string keyName, int width, int height);

	BOOL LoadNext(void);
	
	Loading();
	~Loading();
};

