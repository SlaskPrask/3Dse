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
			data=new Sprite(EngineLayer::instance()->resourceDirectory(file),threaded);
			break;
		case TypeSound:
			data=new Sound(EngineLayer::instance()->resourceDirectory(file),threaded);
			break;
		case TypeFont:
			data=new Font(EngineLayer::instance()->resourceDirectory(file),size,threaded);
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

void Resource::setSprite(const std::string &s)
{
	type=TypeSprite;
	file=s;
}

void Resource::setSound(const std::string &s)
{
	type=TypeSound;
	file=s;
}

void Resource::setFont(const std::string &s,int sz)
{
	type=TypeFont;
	size=sz;
	file=s;
}

void Resource::setEmpty()
{
	type=TypeEmpty;
	file="";
}
