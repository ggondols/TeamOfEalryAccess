#pragma once
#include "cSingletonBase.h"
#include "time.h"
#include "cTimer.h"

//=============================================
//======== ## 20170902 timeManager ## =========
//=============================================


class cTimeManager : public cSingletonBase <cTimeManager>
{
private:
	cTimer *_timer;
	LPD3DXFONT m_pFont;
	RECT _rc;
	SYNTHESIZE(bool, _showFrame, ShowFrame);
public:
	cTimeManager();
	~cTimeManager();

	HRESULT Setup(void);
	void Release(void);

	void Update(float lock = 0.0f);

	unsigned long getFrame(void);

	void Render();

	inline float getWorldTime(void) const { return _timer->getWorldTime(); }

	inline float getElapsedTime(void) const { return _timer->getElapsedTime(); }


};

