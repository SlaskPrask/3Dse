#pragma once

#include "platform.h"
#include "platformgraphics.h"
#include <string>

#include "enginesettings.h"

#define _FONT_SET_CHARACTERS (256)
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
		bool smooth;
		unsigned int characters,textures;
		double *charw;
		double lineh,fonth,descent,ascent;
		double ratio,yratio;
		double xoff,yoff;
		int cell;
		int w,h;
		GLuint *texture;
		inline void bind(int i=0)
		{
			glBindTexture(GL_TEXTURE_2D,texture[i]);
		}
		std::string resource;
		void reload();
		inline GLuint getTexture(int i=0)
		{
			return texture[i];
		}
		void load(const std::string &file,int s=_FONT_DEFAULT_SIZE,unsigned int numchars=_FONT_SET_CHARACTERS,bool smoothed=1,volatile bool threaded=0);
		void unload();
		void init();
		void setData(int startc,int camount,float meas[3],float *charwidth,int texsizew,int texsizeh,int xoff,int yoff);
		
		public:
		Font();
		Font(const std::string &file,int s=_FONT_DEFAULT_SIZE,unsigned int numchars=_FONT_SET_CHARACTERS,bool smoothed=1,volatile bool threaded=0);
		virtual ~Font();
	};
}
