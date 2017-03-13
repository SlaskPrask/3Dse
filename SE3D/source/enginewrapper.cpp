#include "../include/enginewrapper.h"
#include "../include/enginelayer.h"
#include "../include/object.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

bool _engine::isObjectDestroyed(Object* o)
{
	return EngineLayer::instance()->isObjectDestroyed(o);
}

unsigned int _engineprivate::hexStrToInt(const std::string &s)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return x;
}

void _engine::generateTexture(GLuint *destination,int texwidth,int texheight,GLubyte *data,GLenum type)
{
	GLuint tex=0;
	glGenTextures(1,&tex);
	EngineLayer::printGLErrors("Texture creation");

	glBindTexture(GL_TEXTURE_2D,tex);
	DEBUGFUNC(EngineLayer::printGLErrors("Texture bind"));
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	DEBUGFUNC(EngineLayer::printGLErrors("Texture settings"));
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texwidth,texheight,0,type,GL_UNSIGNED_BYTE,data);
	EngineLayer::printGLErrors("Texture");
	if (destination)
	*destination=tex;
}
