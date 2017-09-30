#pragma once

//###########
//	 콘솔 사용법
//  콘솔 선언후 Setup, Update, Render를 추가한다.
//  addValues 함수를 이용해 사용하고 싶은 변수를 추가 한다.
//  호출명, 변수형, 변수의 주소를 인자로 건내 준다.
//  인게임 내에서 콘솔창을 통해 [호출명 Data]를 입력하고 엔터
//  데이터가 변형되고 로그가 남는다
//  cConsole 내부 함수 수정을 통해 구조체와 클래스 같은 사용자 정의 함수또한 정의 가능하다
//  콜백 함수가 추가될 예정이다
//  addvalues 사용 예 : m_pConsole->addValues("MNA", "int", &m_mna); //사용 예시
//##########


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

	bool		m_bRender;
	
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

