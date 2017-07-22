#pragma once

#include "enginesettings.h"
#include "platform.h"
#include "platformgraphics.h"
#include <string>

namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Sprite
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		int w,h;
		GLuint texture;
		bool smooth;
		inline void bind()
		{
			glBindTexture(GL_TEXTURE_2D,texture);
		}
		std::string resource;
		void reload();
		inline GLuint getTexture()
		{
			return texture;
		}
		void load(const std::string &file,bool smoothed=1,volatile bool threaded=0);
		void unload();
		void init();
		
		public:
		Sprite();
		Sprite(const std::string &file,bool smoothed=1,volatile bool threaded=0);
		inline int getWidth()
		{
			return w;
		}
		inline int getHeight()
		{
			return h;
		}
		~Sprite();
	};
}
