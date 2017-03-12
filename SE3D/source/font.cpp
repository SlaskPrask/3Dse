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
	texture=NULL;
	charw=NULL;
	textures=0;
	characters=0;
	resource="";
	size=4;
	lineh=0;
	ratio=1;
	yratio=1;
	xoff=yoff=0;
}

Font::Font(const std::string &file,int s,unsigned int numchars,volatile bool threaded)
{
	init();
	load(file,s,numchars,threaded);
}

void Font::setData(int startc,int camount,float meas[3],float *charwidth,int texsizew,int texsizeh,int xoff,int yoff)
{
	for(int i=0;i<camount;i++)
	charw[i+startc]=charwidth[i+startc];
	descent=meas[0];
	lineh=fonth=meas[1];
	ascent=meas[2];
	w=texsizew;
	h=texsizeh;
	ratio=(double)w/16.0f/(double)size;
	yratio=(double)h/(double)w;
	this->xoff=-xoff;
	this->yoff=yoff;//(double)texsize/16.0f/3.0f; //-charsize/3.0f
}

void Font::load(const std::string &file,int s,unsigned int numchars,volatile bool threaded)
{
	unload();
	
	size=s;
	if (size<4)
	size=4;

	bool valid=1;

	textures=(unsigned int)ceil((double)numchars/(double)_FONT_SET_CHARACTERS);
	characters=numchars;
	charw=new double[textures*_FONT_SET_CHARACTERS];
	texture=new GLuint[textures];

	for(unsigned int i=0;i<textures;i++)
	{
		texture[i]=CallbackLoadFont(file,size,this,i*_FONT_SET_CHARACTERS,(i+1)*_FONT_SET_CHARACTERS,characters,threaded,&(texture[i]));
		if (texture[i]==0)
		valid=0;
	}
	
	if (!valid)
	unload();
	else
	{
		resource=file;
		EngineLayer::instance()->listLoadedFont(this);
	}
}

void Font::reload()
{
	bool valid=1;
	for (unsigned int i=0;i<textures;i++)
	{
		texture[i]=CallbackLoadFont(resource,size,this,i*_FONT_SET_CHARACTERS,(i+1)*_FONT_SET_CHARACTERS,characters,0,&(texture[i]));
		if (texture[i]==0)
			valid=0;
	}

	if (!valid)
	unload();
}

void Font::unload()
{
	if (texture!=NULL)
	{
		EngineLayer::instance()->unlistLoadedFont(this);

		for(unsigned int i=0;i<textures;i++)
		if (texture[i]!=0)
		glDeleteTextures(1,&(texture[i]));

		delete[] texture;
		texture=NULL;
	}
	if (charw!=NULL)
	{
		delete[] charw;
		charw=NULL;
	}
	textures=0;
	characters=0;

	resource="";
	w=h=0;
	size=4;
}

Font::~Font()
{
	unload();
}
