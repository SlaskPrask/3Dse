#pragma once

#include "platform.h"

#ifdef ANDROID
#else
#include <SFML/Audio.hpp>

namespace _engineprivate
{
	struct SoundInstance
	{
		sf::Sound *sound;
		void *soundres;
		int priority;
		unsigned long long int age;
		SoundInstance()
		{
			sound=NULL;
			priority=0;
			age=0;
			soundres=NULL;
		}
		SoundInstance(sf::Sound *sound,void *soundres,int priority,unsigned long long int age):sound(sound),priority(priority),age(age),soundres(soundres)
		{
		}
		~SoundInstance()
		{
			if (sound)
			{
				if (sound->getStatus()!=sf::SoundSource::Status::Stopped)
				sound->stop();
				delete sound;
			}
		}
	};

	struct SoundStorage
	{
		sf::SoundBuffer *buffer;
		void *sound;
		SoundStorage()
		{
			buffer=NULL;
			sound=NULL;
		}
		SoundStorage(sf::SoundBuffer *buffer,void *sound):buffer(buffer),sound(sound)
		{
		}
		~SoundStorage()
		{
			if (buffer)
			delete buffer;
		}
	};
}
#endif
