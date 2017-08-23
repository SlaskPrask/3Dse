#pragma once

#include "enginesettings.h"

#include <vector>
#include <string>
#include "resource.h"


namespace _ENGINESPACE
{
	class ResourceSet;
	struct _SetPointer;
}

namespace _engineprivate
{
	class EngineLayer;
	class Loader;
	class _SetHelper
	{
		public:
		static void assign(_SetPointer *pointer,ResourceSet *set);
	};
}

namespace _ENGINESPACE
{
	class ResourceSet
	{
		friend class _engineprivate::EngineLayer;
		friend struct _SetPointer;
		
		private:
		std::vector<Resource*> sprites;
		std::vector<Resource*> sounds;
		std::vector<Resource*> fonts;
		int loads;
		unsigned int engine_id;
		void _unload(bool forced=0);
		bool _deathMark;
		inline Resource* getSpriteResource(unsigned int i)
		{
			if (i<0||i>=sprites.size())
			return NULL;
			return sprites[i];
		}
		inline Resource* getSoundResource(unsigned int i)
		{
			if (i<0||i>=sounds.size())
			return NULL;
			return sounds[i];
		}
		inline Resource* getFontResource(unsigned int i)
		{
			if (i<0||i>=fonts.size())
			return NULL;
			return fonts[i];
		}
		
		public:
		ResourceSet();
		virtual ~ResourceSet();
		Resource* addSprite(const std::string &s,bool smoothed=1);
		inline Resource* addSpriteSharp(const std::string &s)
		{
			return addSprite(s,0);
		}
		Resource* addSound(const std::string &s,bool stream=0);
		Resource* addMusic(const std::string &s);
		Resource* addFont(const std::string &s,int sz=_FONT_DEFAULT_SIZE,unsigned int characters=_FONT_SET_CHARACTERS,bool smoothed=1);
		void load(volatile bool threaded=0,volatile unsigned int *counter=NULL,volatile bool *quitter=0,Loader *loader=0);
		void unload();
		inline unsigned int id()
		{
			return engine_id;
		}
		inline Sprite* getSprite(unsigned int i)
		{
			if (i<0||i>=sprites.size())
			return NULL;
			return sprites[i]->getSprite();
		}
		inline Sound* getSound(unsigned int i)
		{
			if (i<0||i>=sounds.size())
			return NULL;
			return sounds[i]->getSound();
		}
		inline Font* getFont(unsigned int i)
		{
			if (i<0||i>=fonts.size())
			return NULL;
			return fonts[i]->getFont();
		}
		inline unsigned int getFonts()
		{
			return (unsigned int)fonts.size();
		}
		inline unsigned int getSounds()
		{
			return (unsigned int)sounds.size();
		}
		inline unsigned int getSprites()
		{
			return (unsigned int)sprites.size();
		}
		inline unsigned int getItems()
		{
			return (unsigned int)(sprites.size()+sounds.size()+fonts.size());
		}
		inline bool isLoaded()
		{
			return loads>=1;
		}
	};

	struct _SetPointer
	{
		friend class _engineprivate::_SetHelper;
		protected:
		ResourceSet *_s=NULL;
		public:
		operator unsigned int()
		{
			return _s->engine_id;
		}
		operator ResourceSet*()
		{
			return _s;
		}
	};
}
