#include "StdAfx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager(void)
{
}

cSoundManager::~cSoundManager(void)
{
}

HRESULT cSoundManager::Setup(void)
{
	//사운드 시스템 생성한다
	System_Create(&_system);

	//채널수 설정
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//채널수와 사운드 갯수는 맞춰준다!!
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];
	 
	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	ZeroMemory(_channel, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	return S_OK;
}

void cSoundManager::Release(void)
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL) 
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지운다
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
void cSoundManager::addSound(string keyName, string fullPath, string soundName, bool background, bool loop)
{
	string fullPathName;

	fullPathName = fullPath + soundName;

	if (loop)
	{
		//bgm!!
		if (background)
		{
			
			_system->createStream(fullPathName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			
			_system->createSound(fullPathName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		_system->createSound(fullPathName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
	_mPlayList.insert(make_pair(_mPlayList.size(), keyName));
}

void cSoundManager::play(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			break;
		}
	}
}


void cSoundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

//일시 정지
void cSoundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

//일시 정지된 음악 다시 재생
void cSoundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

void cSoundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

//일시 정지 되었는가?
bool cSoundManager::isPauseSound(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	bool isPaused;
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}

//현재 재생중이냐?
bool cSoundManager::isPlaySound(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	bool isPlaying;
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlaying);
			break;
		}
	}

	return isPlaying;
}

//fmod 시스템 갱신...
void cSoundManager::Update(void)
{
	//볼륨이 바뀐다거나 재생이 끝난 사운드를 채널에서 빼는 등의 다양한 작업을
	//자동으로 해줌
	_system->update();
}

//음악의 길이 값 가져온다 (밀리쎄껀 : 1000 == 1초)
unsigned int cSoundManager::getLength(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return NULL;
}

//음악의 시작지점 설정한다
void cSoundManager::setPosition(string keyName, unsigned int ms)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(ms, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

//재생중인 음악의 현재 위치를 가져온다
unsigned int cSoundManager::getPosition(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			unsigned int position = 0;
			_channel[count]->getPosition(&position, FMOD_TIMEUNIT_MS);

			return position;
		}
	}

	return NULL;
}

std::string cSoundManager::GetSoundNamebyTrack(int trackNum)
{
	if (!_mPlayList.empty())
		return _mPlayList[trackNum];
	else
		return string();
}

