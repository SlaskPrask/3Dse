#pragma once

#include "enginesettings.h"

#include <string>
#include "sprite.h"
#include "sound.h"
#include "font.h"

namespace _ENGINESPACE
{
	class ResourceSet;
}

namespace _engineprivate
{
	using namespace _ENGINESPACE;
	
	class Resource
	{
		friend class _ENGINESPACE::ResourceSet;
		
		private:
		std::string file;
		void *data;
		int type;
		void load(volatile bool threaded=0);
		void unload();
		enum Types{TypeEmpty,TypeSprite,TypeSound,TypeFont};
		
		int size;
		
		public:
		Resource();
		~Resource();
		void setSprite(const std::string &s);
		void setSound(const std::string &s);
		void setFont(const std::string &s,int sz=_FONT_DEFAULT_SIZE);
		void setEmpty();
		inline Sprite* getSprite()
		{
			return static_cast<Sprite*>(data);
		}
		inline Sound* getSound()
		{
			return static_cast<Sound*>(data);
		}
		inline Font* getFont()
		{
			return static_cast<Font*>(data);
		}
		
		operator Sprite*() const
		{
			return static_cast<Sprite*>(data);
		}
		operator Sound*() const
		{
			return static_cast<Sound*>(data);
		}
		operator Font*() const
		{
			return static_cast<Font*>(data);
		}
	};
}
