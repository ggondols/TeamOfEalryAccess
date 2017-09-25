#pragma once

#include "cSingletonBase.h"

#include <string>
#include <map>

//fmod hpp�߰�!!
#include "sound/inc/fmod.hpp"

//lib��ũ...
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

	//���ϴ� ����...
	void play(string keyName, float volume);
	void pause(string keyName);
	void resume(string keyName);
	void stop(string keyName);

	//�Ͻ� ���� �Ǿ��°�?
	bool isPauseSound(string keyName);

	//���� ������̳�?
	bool isPlaySound(string keyName);

	//������ ���� �� �����´� (�и��겫 : 1000 == 1��)
	unsigned int getLength(string keyName);

	//������ �������� �����Ѵ�
	void setPosition(string keyName, unsigned int ms);

	//������� ������ ���� ��ġ�� �����´�
	unsigned int getPosition(string keyName);

	//fmod �ý��� ����...
	void Update(void);

	cSoundManager(void);
	~cSoundManager(void);
};

