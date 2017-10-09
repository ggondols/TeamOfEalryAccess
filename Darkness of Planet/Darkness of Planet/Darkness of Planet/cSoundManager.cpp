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
	//���� �ý��� �����Ѵ�
	System_Create(&_system);

	//ä�μ� ����
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//ä�μ��� ���� ������ �����ش�!!
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];
	 
	//�ʱ�ȭ
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

	//�޸� �����
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

	//�ʿ� ���带 Ű���� �Բ� �־��ش�
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
			//�÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

//�Ͻ� ����
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

//�Ͻ� ������ ���� �ٽ� ���
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

//�Ͻ� ���� �Ǿ��°�?
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

//���� ������̳�?
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

//fmod �ý��� ����...
void cSoundManager::Update(void)
{
	//������ �ٲ�ٰų� ����� ���� ���带 ä�ο��� ���� ���� �پ��� �۾���
	//�ڵ����� ����
	_system->update();
}

//������ ���� �� �����´� (�и��겫 : 1000 == 1��)
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

//������ �������� �����Ѵ�
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

//������� ������ ���� ��ġ�� �����´�
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

