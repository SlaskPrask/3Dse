#pragma once

#include "platform.h"
#include "platformgraphics.h"
#include <string>

#include "enginesettings.h"

#define _FONT_SET_CHARACTERS (256)
#define _FONT_CHARACTERS (512)
#define _FONT_ALIGN_LEFT (0)
#define _FONT_ALIGN_MIDDLE (1)
#define _FONT_ALIGN_RIGHT (2)
#define _FONT_DEFAULT_SIZE (24)

namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Font
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		int size;
		double charw[_FONT_CHARACTERS];
		double lineh,fonth,descent,ascent;
		double ratio,yratio;
		double xoff,yoff;
		int cell;
		int w,h;
		GLuint texture1;
		GLuint texture2;
		inline void bind1()
		{
			glBindTexture(GL_TEXTURE_2D,texture1);
		}
		void bind2()
		{
			glBindTexture(GL_TEXTURE_2D,texture2);
		}
		std::string resource;
		void reload();
		inline GLuint getTexture1()
		{
			return texture1;
		}
		inline GLuint getTexture2()
		{
			return texture2;
		}
		void load(const std::string &file,int s=_FONT_DEFAULT_SIZE,volatile bool threaded=0);
		void unload();
		void init();
		void setData(int startc,int camount,float meas[3],float *charwidth,int texsize);
		
		public:
		Font();
		Font(const std::string &file,int s=_FONT_DEFAULT_SIZE,volatile bool threaded=0);
		~Font();
	};
}
