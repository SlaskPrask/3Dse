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
		sf::Music *music;
		void *soundres;
		int priority;
		unsigned long long int age;
		bool streamed;
		SoundInstance()
		{
			sound=NULL;
			music=NULL;
			priority=0;
			age=0;
			soundres=NULL;
		}
		SoundInstance(sf::Sound *sound,void *soundres,int priority,unsigned long long int age):sound(sound),priority(priority),age(age),soundres(soundres)
		{
			music=NULL;
			streamed=0;
		}
		SoundInstance(sf::Music *music,void *soundres,int priority,unsigned long long int age):music(music),priority(priority),age(age),soundres(soundres)
		{
			sound=NULL;
			streamed=1;
		}
		virtual ~SoundInstance()
		{
			if (sound)
			{
				if (sound->getStatus()!=sf::SoundSource::Status::Stopped)
				sound->stop();
				delete sound;
			}
			if (music)
			{
				if (music->getStatus()!=sf::SoundSource::Status::Stopped)
				music->stop();
				delete music;
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
		virtual ~SoundStorage()
		{
			if (buffer)
			delete buffer;
		}
	};
}
#endif
