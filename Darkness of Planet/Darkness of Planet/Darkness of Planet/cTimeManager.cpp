#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
	:_showFrame(false)
{
}


cTimeManager::~cTimeManager()
{

}

HRESULT cTimeManager::Setup(void)
{
	_timer = new cTimer;
	_timer->Setup();

	//기본 폰트
	D3D::SetFont(m_pFont);
		

	return S_OK;

}


void cTimeManager::Release(void)
{
	if (_timer != NULL) SAFE_DELETE(_timer);
}

void cTimeManager::Update(float lock)
{
	if (_timer != NULL) _timer->tick(lock);
}

unsigned long cTimeManager::getFrame(void)
{
	return _timer->getFrameRate();
}

void cTimeManager::Render()
{
	//if(!_showFrame) return;

	char str[256];
	std::string strFrame;

	/*SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0, ));*/


#pragma warning(disable : 4996)
#ifdef _DEBUG
	if (_timer != NULL)
	{

		sprintf(str, "framePerSec : %d", _timer->getFrameRate());
		//TextOut(hdc, 0, 0, str, strlen(str));		
		SetRect(&_rc, 0, 0, 800, 600);
		m_pFont->DrawText(NULL, str, -1, &_rc, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));


		sprintf(str, "worldTime : %f", _timer->getWorldTime());
		//TextOut(hdc, 0, 20, str, strlen(str));
		SetRect(&_rc, 0, 20, 800, 600);
		m_pFont->DrawText(NULL, str, -1, &_rc, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));


		sprintf(str, "elapsedTime : %f", _timer->getElapsedTime());
		//TextOut(hdc, 0, 40, str, strlen(str));
		SetRect(&_rc, 0, 40, 800, 600);
		m_pFont->DrawText(NULL, str, -1, &_rc, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

#else
	if (_timer != NULL)
	{
		sprintf(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif
}