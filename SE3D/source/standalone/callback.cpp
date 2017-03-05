#include "../../include/enginelayer.h"
#include "../../include/enginecallback.h"
#include "../../include/androidnames.h"
#include "../../include/enginewrapper.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void CallbackSetTitle(const std::string &s)
{
	/*enoty, handled on the inside*/
}

void _engineprivate::CallbackPrintLog(const std::string &s)
{
	std::cout << s;
}

bool _engineprivate::CallbackUnsetFullscreen()
{
	/*empty, handled on the inside*/
	return 0;
}

bool _engineprivate::CallbackSetFullscreen()
{
	/*empty, handled on the inside*/
	return 0;
}

void _engineprivate::CallbackSetOrientation(int orientation)
{
	/*empty, handled on the inside*/
}

GLuint _engineprivate::CallbackLoadPNG(const std::string &s,int *width,int *height,bool threaded,GLuint *destination)
{
	sf::Image image;
	if (!image.loadFromFile(s))
	{
		return 0;
	}
	*width = image.getSize().x;
	*height = image.getSize().y;

	/*//ARGB->ABGR
	for(int i=0;i<(*width)*(*height);i++)
	pixels[i]=(pixels[i]&0xFF00FF00)|((pixels[i]&0x000000FF)<<16)|((pixels[i]&0x00FF0000)>>16);*/
	/*
	glEnable(GL_TEXTURE_2D);
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
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,*width,*height,0,GL_RGBA,GL_UNSIGNED_BYTE,image.getPixelsPtr());
	EngineLayer::printGLErrors("Texture");
	*/
	GLubyte *data=new GLubyte[(*width)*(*height)*4];
	memcpy(data,image.getPixelsPtr(),(*width)*(*height)*4);


	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,*width,*height,data,GL_RGBA);

		delete[] data;
		return tex;
	}
	else
	{
		EngineLayer::pushLoaderData(destination,*width,*height,data,GL_RGBA);
		return 1;//temp value
	}
	return 0;
}

GLuint _engineprivate::CallbackLoadFont(const std::string &s,int size,Font *fnt,int startc,int camount,bool threaded,GLuint *destination)
{
	FT_Library *lib=EngineLayer::instance()->getFontLib();
	if (!lib)
		return 0;

	FT_Face face;
	if (FT_New_Face(*lib,s.c_str(),0,&face))
		return 0;

	FT_Set_Char_Size(face,size<<6,size<<6,77,77);//73.5,73.5
	float meas[3];//descent,lineh,ascent
	float charwidth[_FONT_CHARACTERS];

	meas[0]=-(float)(face->size->metrics.descender>>6);
	//meas[1]=(float)(face->size->metrics.height>>6);//bottom+top
												   //meas[1]=(face->bbox.yMax-face->bbox.yMin)>>6;//bottom+top
	meas[2]=(float)(face->size->metrics.ascender>>6);
	meas[1]=meas[0]+meas[2];

	double cwidth=0;
	double cheight=0;

	FT_Glyph glyph;
	FT_BitmapGlyph bmpg;
	FT_Bitmap *bmp;
	for (int c=0;c<_FONT_CHARACTERS;c++)
	{
		if (FT_Load_Glyph(face,FT_Get_Char_Index(face,c),FT_LOAD_DEFAULT))
			return 0;
		if (FT_Get_Glyph(face->glyph,&glyph))
			return 0;

		FT_Glyph_To_Bitmap(&glyph,ft_render_mode_normal,0,1);
		bmpg=(FT_BitmapGlyph)glyph;
		bmp=&bmpg->bitmap;

		charwidth[c]=(float)bmp->width;
		if (charwidth[c]>cwidth)
			cwidth=charwidth[c];
		if (bmp->rows>cheight)
			cheight=bmp->rows;

		charwidth[c]=(GLfloat)(face->glyph->advance.x>>6);

		FT_Done_Glyph(glyph);
	}

	cheight=meas[1];

	int cellw=(int)cwidth+2;
	int cellh=(int)cheight+2;

	int cellsize=(cellw>cellh?cellw:cellh);

	int charsize=1;
	while (charsize<cellsize)
		charsize*=2;
	int texsize=charsize*16;

	GLubyte *data=new GLubyte[texsize*texsize*2];//double bytes because contains color byte and alpha byte (c,a)GL_LUMINANCE_ALPHA not like 4x (r,g,b,a)GL_RGBA

	for (int c=0;c<_FONT_SET_CHARACTERS;c++)
	{
		if (FT_Load_Glyph(face,FT_Get_Char_Index(face,c),FT_LOAD_DEFAULT))
			return 0;
		if (FT_Get_Glyph(face->glyph,&glyph))
			return 0;

		FT_Glyph_To_Bitmap(&glyph,ft_render_mode_normal,0,1);
		bmpg=(FT_BitmapGlyph)glyph;
		bmp=&bmpg->bitmap;

		int offx=(int)((face->glyph->metrics.horiBearingX>>6)+charsize/3.0f);
		int offy=(int)(charsize-1-meas[0]-1-(face->glyph->metrics.horiBearingY>>6))-charsize/3.0f;

		int ox=1+offx+face->glyph->bitmap_left;
		int oy=1+offy+face->glyph->bitmap_top;
		int px=(c%16)*charsize;
		int py=(int)(c/16)*texsize*charsize;
		for (int j=0;j<charsize;j++)
		for (int i=0;i<charsize;i++)
		{
			//set two neighboring containers the same (c,a)
			if (i<ox||i>=ox+(int)bmp->width
				||  j<oy||j>=oy+(int)bmp->rows)
			{
				data[(py+px+i+j*(texsize))*2]=0xFF;
				data[(py+px+i+j*(texsize))*2+1]=0;
			}
			else
			{
				data[(py+px+i+j*(texsize))*2]=0xFF;
				data[(py+px+i+j*(texsize))*2+1]=bmp->buffer[i-ox+bmp->width*(j-oy)];
			}
		}

		FT_Done_Glyph(glyph);
	}

	/*//show background
	for (int i=0;i<texsize*texsize;i++)
	if (!data[i*2+1])
	{
		data[i*2]=0x00;
		data[i*2+1]=0x88;
	}
	else
	data[i*2+1]=0x88;
	//*/

	EngineLayer::instance()->setFontData(fnt,startc,camount,meas,charwidth,texsize);
	FT_Done_Face(face);

	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,texsize,texsize,data,GL_LUMINANCE_ALPHA);

		delete[] data;
		return tex;
	}
	else
	{
		EngineLayer::pushLoaderData(destination,texsize,texsize,data,GL_LUMINANCE_ALPHA);
		return 1;//temp value
	}
	return 0;
}

void _engineprivate::CallbackMessage(const std::string &s)
{
	//TODO
}

void _engineprivate::CallbackRequestInput(int type)
{
	/*empty, handled on the inside*/
}

void _engineprivate::CallbackSetFPS(int fps)
{
	/*empty, handled on the inside*/
}

int _engineprivate::CallbackLoadSound(const std::string &s)
{
	//TODO
	return -1;
}

void _engineprivate::CallbackUnloadSound(int i)
{
	//TODO
}

int _engineprivate::CallbackPlaySound(int i,float l,float r,int prio,int loops,float speed)
{
	return 0;
}

void _engineprivate::CallbackPauseSound(int i)
{
	//TODO
}

void _engineprivate::CallbackUnpauseSound(int i)
{
	//TODO
}

void _engineprivate::CallbackStopSound(int i)
{
	//TODO
}

void _engineprivate::CallbackSpeedSound(int i,float spd)
{
	//TODO
}

void _engineprivate::CallbackVolumeSound(int i,float left,float right)
{
	//TODO
}

void _engineprivate::CallbackCloseApplication()
{
	/*empty, handled on the inside*/
}
