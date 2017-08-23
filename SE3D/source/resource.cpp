#include "../include/resource.h"
#include "../include/enginelayer.h"
#include <iostream>

using namespace _engineprivate;
using namespace _ENGINESPACE;

Resource::Resource()
{
	data=NULL;
	file="";
	type=TypeEmpty;
	size=_FONT_DEFAULT_SIZE;
	fontchars=_FONT_SET_CHARACTERS;
	smooth=1;
	streamed=0;
}

Resource::~Resource()
{
	unload();
}

void Resource::load(volatile bool threaded)
{
	if (data==NULL)
	switch (type)
	{
		default:
		case TypeEmpty:
			break;
		case TypeSprite:
			data=new Sprite(EngineLayer::instance()->resourceDirectory(file),smooth,threaded);
			break;
		case TypeSound:
			data=new Sound(EngineLayer::instance()->resourceDirectory(file),threaded,streamed);
			break;
		case TypeFont:
			data=new Font(EngineLayer::instance()->resourceDirectory(file),size,fontchars,smooth,threaded);
			break;
	}
}

void Resource::unload()
{
	if (data)
	{
		switch (type)
		{
			default:
			case TypeEmpty:
				//delete data;
				break;
			case TypeSprite:
				delete (Sprite*)data;
				break;
			case TypeSound:
				delete (Sound*)data;
				break;
			case TypeFont:
				delete (Font*)data;
				break;
		}
		data=NULL;
	}
}

void Resource::setSprite(const std::string &s,bool smoothed)
{
	type=TypeSprite;
	file=s;
	smooth=smoothed;
}

void Resource::setSound(const std::string &s,bool stream)
{
	type=TypeSound;
	streamed=stream;
	file=s;
}

void Resource::setFont(const std::string &s,int sz,unsigned int chars,bool smoothed)
{
	type=TypeFont;
	size=sz;
	file=s;
	fontchars=chars;
	smooth=smoothed;
}

void Resource::setEmpty()
{
	type=TypeEmpty;
	file="";
}

void _ResHelper::assign(_ResPointer *pointer,Resource *res)
{
	pointer->_r=res;
}
