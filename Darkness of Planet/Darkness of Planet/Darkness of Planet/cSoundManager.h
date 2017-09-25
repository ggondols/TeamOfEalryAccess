#pragma once

#include "cSingletonBase.h"

#include <string>
#include <map>

//fmod hpp추가!!
#include "sound/inc/fmod.hpp"

//lib링크...
#pragma comment (lib, "sound/lib/fmodex_vc.lib")

using namespace FMOD;

#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 10

#define TOTALSOUNDBUFFER (EXTRACHANNELBUFFER + SOUNDBUFFER)

class cSoundManager : public cSingletonBase <cSoundManager>
{
private:
	typedef std::map<std::string, Sound**> arrSounds;
	typedef std::map<std::string, Sound**>::iterator arrSoundsIter;
	typedef std::map<std::string, Channel**> arrChannels;
	typedef std::map<std::string, Channel**>::iterator arrChannelsIter;
	
private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	HRESULT Setup(void);
	void Release(void);

	//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
	void addSound(string keyName, string soundName, bool background, bool loop);
	void play(string keyName);

	//이하는 숙제...
	void play(string keyName, float volume);
	void pause(string keyName);
	void resume(string keyName);
	void stop(string keyName);

	//일시 정지 되었는가?
	bool isPauseSound(string keyName);

	//현재 재생중이냐?
	bool isPlaySound(string keyName);

	//음악의 길이 값 가져온다 (밀리쎄껀 : 1000 == 1초)
	unsigned int getLength(string keyName);

	//음악의 시작지점 설정한다
	void setPosition(string keyName, unsigned int ms);

	//재생중인 음악의 현재 위치를 가져온다
	unsigned int getPosition(string keyName);

	//fmod 시스템 갱신...
	void Update(void);

	cSoundManager(void);
	~cSoundManager(void);
};

