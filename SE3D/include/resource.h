#pragma once

#include "enginesettings.h"

#include <string>
#include "sprite.h"
#include "sound.h"
#include "font.h"

namespace _ENGINESPACE
{
	class Resource;
	struct _ResPointer;
}

namespace _engineprivate
{
	class _ResHelper
	{
		public:
		static void assign(_ResPointer *pointer,Resource *res);
	};
}

namespace _ENGINESPACE
{
	class ResourceSet;
	
	class Resource
	{
		friend class _ENGINESPACE::ResourceSet;
		
		private:
		std::string file;
		void *data;
		int type;
		bool smooth;
		unsigned int fontchars;
		bool streamed;
		void load(volatile bool threaded=0);
		void unload();
		enum Types{TypeEmpty,TypeSprite,TypeSound,TypeFont};
		
		int size;
		
		public:
		Resource();
		virtual ~Resource();
		void setSprite(const std::string &s,bool smoothed);
		void setSound(const std::string &s,bool stream);
		void setFont(const std::string &s,int sz=_FONT_DEFAULT_SIZE,unsigned int chars=_FONT_SET_CHARACTERS,bool smoothed=1);
		void setEmpty();
		inline std::string getFile()
		{
			return file;
		}
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
	};

	struct _ResPointer
	{
		friend class _engineprivate::_ResHelper;
		protected:
		Resource *_r=NULL;
		const unsigned int _id;

		public:
		_ResPointer(unsigned int i):_id(i){}
		inline unsigned int id()
		{
			return _id;
		}
		operator Sprite*()
		{
			return _r->getSprite();
		}
		operator Sound*()
		{
			return _r->getSound();
		}
		operator Font*()
		{
			return _r->getFont();
		}
		operator unsigned int()
		{
			return _id;
		}
		_ResPointer* operator->()
		{
			return this;
		}
	};
}
