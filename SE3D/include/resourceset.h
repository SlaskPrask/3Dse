#pragma once

#include "enginesettings.h"

#include <vector>
#include <string>
#include "resource.h"

namespace _engineprivate
{
	class EngineLayer;
	class Loader;
}

namespace _ENGINESPACE
{
	class ResourceSet
	{
		friend class _engineprivate::EngineLayer;
		
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
			return sprites[i];
		}
		inline Resource* getSoundResource(unsigned int i)
		{
			return sounds[i];
		}
		inline Resource* getFontResource(unsigned int i)
		{
			return fonts[i];
		}
		
		public:
		ResourceSet();
		~ResourceSet();
		Resource* addSprite(const std::string &s);
		Resource* addSound(const std::string &s);
		Resource* addFont(const std::string &s,int sz=_FONT_DEFAULT_SIZE,unsigned int characters=_FONT_SET_CHARACTERS);
		void load(volatile bool threaded=0,volatile unsigned int *counter=NULL,volatile bool *quitter=0,Loader *loader=0);
		void unload();
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
}
