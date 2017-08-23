#pragma once

#include "enginesettings.h"

namespace _engineprivate
{
	#ifdef ANDROID
	static const char VERTEX_SHADER[]="#version 100\n\
	\n\
	attribute vec4 fpos;\n\
	attribute vec2 ftexcoord;\n\
	attribute float ftextured;\n\
	uniform vec2 foffset;\n\
	uniform vec2 ftrans;\n\
	uniform vec2 fhalfsize;\n\
	uniform mat4 fprojection;\n\
	uniform mat4 frotation;\n\
	varying vec2 vtexcoord;\n\
	void main()\n\
	{\n\
		gl_Position=fpos;\n\
		gl_Position.x-=fhalfsize.x;\n\
		gl_Position.y-=fhalfsize.y;\n\
		gl_Position=frotation*gl_Position;\n\
		gl_Position.x+=fhalfsize.x;\n\
		gl_Position.y+=fhalfsize.y;\n\
		gl_Position.x+=ftrans.x-foffset.x;\n\
		gl_Position.y+=ftrans.y-foffset.y;\n\
		gl_Position*=fprojection;\n\
		\n\
		vtexcoord=ftexcoord;\n\
	}";

	static const char FRAGMENT_SHADER[]="#version 100\n\
	\n\
	precision mediump float;\n\
	uniform vec4 fcolor;\n\
	uniform vec4 fcolorization;\n\
	uniform sampler2D ftex;\n\
	varying vec2 vtexcoord;\n\
	uniform int ftextured;\n\
	void main()\n\
	{\n\
		if (ftextured==0)\n\
		gl_FragColor=fcolor+fcolorization;\n\
		else\n\
		gl_FragColor=texture2D(ftex,vtexcoord)*fcolor+fcolorization;\n\
	}";
	static const char VERTEX_SHADER_INTERNAL_OUT[]="#version 100\n\
	\n\
	attribute vec4 fpos;\n\
	attribute vec2 ftexcoord;\n\
	varying vec2 vtexcoord;\n\
	void main()\n\
	{\n\
		gl_Position=fpos;\n\
		\n\
		vtexcoord=ftexcoord;\n\
	}";

	static const char FRAGMENT_SHADER_INTERNAL_OUT[]="#version 100\n\
	\n\
	precision mediump float;\n\
	uniform sampler2D ftex;\n\
	varying vec2 vtexcoord;\n\
	uniform int ftextured;\n\
	void main()\n\
	{\n\
		gl_FragColor=texture2D(ftex,vtexcoord);\n\
	}";
	#else
	static const char VERTEX_SHADER[]="#version 150\n\
	\n\
	in vec4 fpos;\n\
	in vec2 ftexcoord;\n\
	in float ftextured;\n\
	uniform vec2 foffset;\n\
	uniform vec2 ftrans;\n\
	uniform vec2 fhalfsize;\n\
	uniform mat4 fprojection;\n\
	uniform mat4 frotation;\n\
	out vec2 vtexcoord;\n\
	void main()\n\
	{\n\
		gl_Position=fpos;\n\
		gl_Position.x-=fhalfsize.x;\n\
		gl_Position.y-=fhalfsize.y;\n\
		gl_Position=frotation*gl_Position;\n\
		gl_Position.x+=fhalfsize.x;\n\
		gl_Position.y+=fhalfsize.y;\n\
		gl_Position.x+=ftrans.x-foffset.x;\n\
		gl_Position.y+=ftrans.y-foffset.y;\n\
		gl_Position*=fprojection;\n\
		\n\
		vtexcoord=ftexcoord;\n\
	}\n";
	static const char FRAGMENT_SHADER[]="#version 150\n\
	\n\
	#extension GL_ARB_explicit_attrib_location : require\n\
	#extension GL_ARB_explicit_uniform_location : require\n\
	layout(location=0) out vec3 color;\n\
	precision mediump float;\n\
	uniform vec4 fcolor;\n\
	uniform vec4 fcolorization;\n\
	uniform sampler2D ftex;\n\
	in vec2 vtexcoord;\n\
	uniform int ftextured;\n\
	out vec4 fragmentColor;\n\
	void main()\n\
	{\n\
		if (ftextured==0)\n\
		fragmentColor=fcolor+fcolorization;\n\
		else\n\
		fragmentColor=texture(ftex,vtexcoord)*fcolor+fcolorization;\n\
	}\n";
	static const char VERTEX_SHADER_INTERNAL_OUT[]="#version 150\n\
	\n\
	in vec4 fpos;\n\
	in vec2 ftexcoord;\n\
	out vec2 vtexcoord;\n\
	void main()\n\
	{\n\
		gl_Position=fpos;\n\
		\n\
		vtexcoord=ftexcoord;\n\
	}\n";
	static const char FRAGMENT_SHADER_INTERNAL_OUT[]="#version 150\n\
	\n\
	precision mediump float;\n\
	uniform sampler2D ftex;\n\
	in vec2 vtexcoord;\n\
	out vec4 fragmentColor;\n\
	void main()\n\
	{\n\
		fragmentColor=texture(ftex,vtexcoord);\n\
	}\n";
	#endif

}
