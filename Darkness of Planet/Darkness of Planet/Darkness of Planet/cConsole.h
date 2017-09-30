#pragma once
class cUIImageView;
class cConsole
{
	string m_input;

	vector<string>	m_values;
	vector<string>	m_type;
	vector<void*>	m_pDatas;

	string			m_outPut;
	vector<string>	m_log;

	cUIImageView* m_box;
	LPD3DXFONT	m_pFont;

	
public:

	void Setup();
	void Release();
	void Update();
	void DataInput(istringstream & iss, string & type, void* data);
	void Render();

	void addValues(string valueName, string type, void* data);

	cConsole();
	~cConsole();
};

