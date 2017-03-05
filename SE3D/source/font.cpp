#include "../include/font.h"
#include "../include/enginecallback.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

Font::Font()
{
	init();
}

void Font::init()
{
	w=h=0;
	texture1=texture2=0;
	resource="";
	size=4;
	lineh=0;
	ratio=1;
	yratio=1;
	xoff=yoff=0;
}

Font::Font(const std::string &file,int s,volatile bool threaded)
{
	init();
	load(file,s,threaded);
}

void Font::setData(int startc,int camount,float *meas,float *charwidth,int texsize,int xoff,int yoff)
{
	for(int i=0;i<camount;i++)
	charw[i+startc]=charwidth[i+startc];
	descent=meas[0];
	lineh=fonth=meas[1];
	ascent=meas[2];
	w=h=texsize;
	ratio=(double)w/16.0f/(double)size;
	yratio=(double)h/16.0f/(double)fonth;
	this->xoff=-xoff;
	this->yoff=yoff;//(double)texsize/16.0f/3.0f; //-charsize/3.0f
}

void Font::load(const std::string &file,int s,volatile bool threaded)
{
	unload();
	
	size=s;
	if (size<4)
	size=4;
	texture1=CallbackLoadFont(file,size,this,0,_FONT_SET_CHARACTERS,threaded,&texture1);
	texture2=CallbackLoadFont(file,size,this,_FONT_SET_CHARACTERS,_FONT_SET_CHARACTERS,threaded,&texture2);
	
	if (texture1==0||texture2==0)
	unload();
	else
	{
		resource=file;
		EngineLayer::instance()->listLoadedFont(this);
	}
}

void Font::reload()
{
	texture1=CallbackLoadFont(resource,size,this,0,_FONT_SET_CHARACTERS,0,&texture1);
	texture2=CallbackLoadFont(resource,size,this,_FONT_SET_CHARACTERS,_FONT_SET_CHARACTERS,0,&texture1);
	
	if (texture1==0||texture2==0)
	unload();
}

void Font::unload()
{
	if (texture1!=0||texture2!=0)
	EngineLayer::instance()->unlistLoadedFont(this);
	if (texture1!=0)
	glDeleteTextures(1,&texture1);
	if (texture2!=0)
	glDeleteTextures(1,&texture2);
	resource="";
	w=h=0;
	size=4;
	texture1=texture2=0;
}

Font::~Font()
{
	unload();
}
