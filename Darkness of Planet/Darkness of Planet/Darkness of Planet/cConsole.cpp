#include "stdafx.h"
#include "cConsole.h"
#include "cUIImageView.h"
#include <sstream>



void cConsole::Setup()
{
	D3DVIEWPORT9 viewport;
	GETDEVICE->GetViewport(&viewport);
	m_box = new cUIImageView;
	//m_box->SetTexture("./UI/Icon_Empty.png");
	//m_box->SetPosition(1,1);

	UIOBJECTMANAGER->AddRoot("ConsoleBar", UITYPE_IMAGE, true);
	UIOBJECTMANAGER->SetTexture("ConsoleBar", "./UI/Icon_Empty.png");
	UIOBJECTMANAGER->SetPosition("ConsoleBar", 0.5f, 0.85f);

	UIOBJECTMANAGER->AddChild("ConsoleBar", UITYPE_TEXT);
	UIOBJECTMANAGER->SetPosition("ConsoleBar", 1, 0, 0.5);	// 인풋 데이터
	UIOBJECTMANAGER->AddChild("ConsoleBar", UITYPE_TEXT);
	UIOBJECTMANAGER->SetPosition("ConsoleBar", 2, 0, -0.6); // 텍스트 로그
	UIOBJECTMANAGER->AddChild("ConsoleBar", UITYPE_TEXT);
	UIOBJECTMANAGER->SetPosition("ConsoleBar", 3, 0, -0.3); // 아웃풋 데이터
	
	m_pFont = FONTMANAGER->GetFont(cFontManager::E_NORMAL);
}

void cConsole::Release()
{
	UIOBJECTMANAGER->ReleaseRoot("ConsoleBar");
}

void cConsole::Update()
{
	if (GETLPARAM != 0)
	{
		char str = (GETLPARAM);

		if (str >= '0' && str <= 'Z')
		{
			m_input += str;
		}
		else if ((int)str == 32) // 스페이스
		{
			m_input.push_back(' ');
		}
		else if ((int)str == 8) // 백스페이스
		{
			m_input.pop_back();
		}
		else if ((int)str == 13) // 엔터
		{
			m_log.push_back(m_input);
			istringstream iss(m_input);

			string token;

			iss >> token;
			float num;

			for (int i = 0; i < m_values.size(); i++)
			{
				if (m_values[i] == token)
				{
					DataInput(iss, m_type[i], m_pDatas[i]);
				}
			}
			
		
			m_input.clear();
		}

		GETLPARAM = 0;
	}
}

void cConsole::DataInput(istringstream& iss, string& type, void* data)
{
	if (type == "int")
	{
		iss >> *(int*)data;
	}
	else if (type == "float")
	{
		iss >> *(float*)data;
	}
	else if (type == "D3DXVECTOR3")
	{
		float x, y, z;
		iss >> x >> y >> z;
		*(D3DXVECTOR3*)data = D3DXVECTOR3(x,y,z);
	}
	else if (type == "string")
	{
		string* temp = (string*)data;
		temp->clear();
		iss >> *temp;
	}
	else if (type == "char")
	{
		iss >> *(char*)data;
	}


}
void cConsole::Render()
{
	UIOBJECTMANAGER->Render("ConsoleBar");
	string str = m_input;
	//RECT rc = RectMake(200, 500, 1000, 1000);
	//m_pFont->DrawTextA(NULL, str.c_str(), str.size(), &rc, DT_CENTER, D3DCOLOR_XRGB(255, 255, 255));
	UIOBJECTMANAGER->SetText("ConsoleBar", 1, str);
	
	for (int i = 0; i < m_log.size(); i++)
	{
		UIOBJECTMANAGER->SetPosition("ConsoleBar", 2,0, -0.3 - -i*0.3);
		UIOBJECTMANAGER->SetText("ConsoleBar", 2, m_log[i]);
	}
}

void cConsole::addValues(string valueName, string type, void* data)
{
	m_values.push_back(valueName);
	m_type.push_back(type);
	m_pDatas.push_back(data);
}

cConsole::cConsole()
{
}


cConsole::~cConsole()
{
}
