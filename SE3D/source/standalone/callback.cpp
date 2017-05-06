#include "../../include/enginelayer.h"
#include "../../include/enginecallback.h"
#include "../../include/androidnames.h"
#include "../../include/enginewrapper.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void CallbackSetTitle(const std::string &s)
{
	/*empty, handled on the inside*/
}

void _engineprivate::CallbackSetIcon(const std::string &s)
{
	sf::Window *window=EngineLayer::instance()->getWindow();
	if (!window)
	return;

	sf::Image image;
	if (!image.loadFromFile(s))
	{
		Log::error("Resources",std::string("Unable to load image file \"")+s+"\" for icon");
		return;
	}

	window->setIcon(image.getSize().x,image.getSize().y,image.getPixelsPtr());
}

void _engineprivate::CallbackStartAds(const std::string &adKey,int size,bool top)
{
	/*empty, not supported*/
}

void _engineprivate::CallbackEndAds()
{
	/*empty, not supported*/
}

bool _engineprivate::CallbackExistsFile(const std::string &f)
{
	bool ret=0;
	std::ifstream file(f);
	if (file.is_open())
	{
		ret=file.good();
		file.close();
	}
	return ret;
}

bool _engineprivate::CallbackDeleteFile(const std::string &f)
{
	return (!std::remove(f.c_str()));
}

bool _engineprivate::CallbackWriteFile(const std::string &f,const std::string &s)
{
	std::ofstream file;

	file.open(f,std::ios_base::binary);
	if (file.is_open())
	{
		file << s;
		file.close();
		return 1;
	}
	return 0;
}

bool _engineprivate::CallbackReadFile(const std::string &f,std::string *s)
{
	std::ifstream file;

	file.open(f);
	if (file.is_open(),std::ios_base::binary)
	{
		std::string content((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
		file.close();
		*s=content;
		return 1;
	}
	return 0;
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
		Log::error("Resources",std::string("Unable to load image file \"")+s+"\"");
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
		LOADERLOG(Log::log("Loader",std::string("7-Creating data for ")
						   +to_string(destination)+" "
						   +to_string(*width)+"x"
						   +to_string(*height)+" from "
						   +to_string((void*)data)+" of type "
						   +to_string(GL_RGBA)
						   ));
		EngineLayer::pushLoaderData(destination,*width,*height,data,GL_RGBA);
		LOADERLOG(Log::log("Loader",std::string("7-Data created")));
		return 1;//temp value
	}
	return 0;
}

GLuint _engineprivate::CallbackLoadFont(const std::string &s,int size,Font *fnt,int startc,int camount,int totalchars,bool threaded,GLuint *destination)
{
	FT_Library *lib=EngineLayer::instance()->getFontLib();
	if (!lib)
	{
		Log::error("Resources","Unable to load font library");
		return 0;
	}

	FT_Face face;
	if (FT_New_Face(*lib,s.c_str(),0,&face))
	{
		Log::error("Resources",std::string("Unable to load font file \"")+s+"\"");
		return 0;
	}

	FT_Set_Char_Size(face,size<<6,size<<6,72,72);//73.5,73.5
	float meas[3];//descent,lineh,ascent
	float *charwidth=new float[totalchars];

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
	for (int c=0;c<totalchars;c++)
	{
		if (FT_Load_Glyph(face,FT_Get_Char_Index(face,c),FT_LOAD_DEFAULT))
		{
			Log::error("Resources",std::string("Unable to load font character ")+to_string(c)+" from \""+s+"\" while initializing");
			return 0;
		}
		if (FT_Get_Glyph(face->glyph,&glyph))
		{
			Log::error("Resources",std::string("Unable to get font character ")+to_string(c)+" data from \""+s+"\" while initializing");
			return 0;
		}

		FT_Glyph_To_Bitmap(&glyph,ft_render_mode_normal,0,1);
		bmpg=(FT_BitmapGlyph)glyph;
		bmp=&bmpg->bitmap;

		charwidth[c]=(float)bmp->width;
		if (charwidth[c]>cwidth)
			cwidth=charwidth[c];
		/*if (bmp->rows>cheight)
			cheight=bmp->rows;*/ //doesn't do anything

		charwidth[c]=(GLfloat)(face->glyph->advance.x>>6);

		FT_Done_Glyph(glyph);
	}

	cheight=meas[1];

	int wborder=(int)(cwidth/4);
	int hborder=(int)(cheight/8);
	if (wborder<1)
	wborder=1;
	if (hborder<1)
	hborder=1;

	int cellw=(int)cwidth+wborder*2;
	int cellh=(int)cheight+hborder*2;

	int texsizew=(int)cellw*16;
	int texsizeh=(int)cellh*16;
	
	int xoff=wborder;
	int yoff=hborder;

	GLubyte *data=new GLubyte[texsizew*texsizeh*2];//double bytes because contains color byte and alpha byte (c,a)GL_LUMINANCE_ALPHA not like 4x (r,g,b,a)GL_RGBA

	for (int c=0;c<camount;c++)
	{
		if (FT_Load_Glyph(face,FT_Get_Char_Index(face,c+startc),FT_LOAD_DEFAULT))
		{
			Log::error("Resources",std::string("Unable to load font character ")+to_string(c+startc)+" from \""+s+"\"");
			return 0;
		}
		if (FT_Get_Glyph(face->glyph,&glyph))
		{
			Log::error("Resources",std::string("Unable to get font character ")+to_string(c+startc)+" data from \""+s+"\"");
			return 0;
		}

		FT_Glyph_To_Bitmap(&glyph,ft_render_mode_normal,0,1);
		bmpg=(FT_BitmapGlyph)glyph;
		bmp=&bmpg->bitmap;

		int offx=(int)((face->glyph->metrics.horiBearingX>>6)+xoff);
		int offy=(int)(cellh-meas[0]-(face->glyph->metrics.horiBearingY>>6)-yoff);

		int ox=offx+face->glyph->bitmap_left;
		int oy=offy+face->glyph->bitmap_top;
		int px=(int)((c%16)*cellw);
		int py=(c/16)*texsizew*(int)cellh;
		for (int j=0;j<cellh;j++)
		for (int i=0;i<cellw;i++)
		{
			//set two neighboring containers the same (c,a)
			if (i<ox||i>=ox+(int)bmp->width
			||  j<oy||j>=oy+(int)bmp->rows)
			{
				data[(py+px+i+j*(texsizew))*2]=0xFF;
				data[(py+px+i+j*(texsizew))*2+1]=0x00;
			}
			else
			{
				
				data[(py+px+i+j*(texsizew))*2]=0xFF;
				data[(py+px+i+j*(texsizew))*2+1]=bmp->buffer[i-ox+bmp->width*(j-oy)];
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

	EngineLayer::instance()->setFontData(fnt,startc,camount,meas,charwidth,texsizew,texsizeh,xoff,yoff);
	FT_Done_Face(face);

	delete[] charwidth;

	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,texsizew,texsizeh,data,GL_LUMINANCE_ALPHA);

		delete[] data;
		return tex;
	}
	else
	{
		LOADERLOG(Log::log("Loader",std::string("7-Creating font data for ")
						   +to_string(destination)+" "
						   +to_string(texsizew)+"x"
						   +to_string(texsizeh)+" from "
						   +to_string((void*)data)+" of type "
						   +to_string(GL_LUMINANCE_ALPHA)
						   ));
		EngineLayer::pushLoaderData(destination,texsizew,texsizeh,data,GL_LUMINANCE_ALPHA);
		LOADERLOG(Log::log("Loader",std::string("7-Data created")));
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

bool _engineprivate::CallbackOpenURL(const std::string &uri)
{
	//TODO
	return 0;
}

int _engineprivate::CallbackLoadSound(const std::string &s,bool stream)
{
	std::vector<SoundStorage*> *buf=EngineLayer::instance()->getAudioBuffers();
	unsigned int i=0;

	while(i<buf->size())//find free id
	{
		if ((*buf)[i]==NULL)
		break;

		i++;
	}

	SoundStorage *storage;

	if (stream)
	{
		storage=new SoundStorage(NULL,NULL);
	}
	else
	{
		sf::SoundBuffer *buffer=new sf::SoundBuffer;
		if (!buffer->loadFromFile(s))
		{
			delete buffer;
			Log::error("Resources",std::string("Unable to load audio file \"")+s+"\"");
			return -1;
		}

		storage=new SoundStorage(buffer,NULL);
	}

	if (i==buf->size())
	buf->push_back(storage);
	else
	(*buf)[i]=storage;

	return i;
}

void _engineprivate::CallbackUnloadSound(int i)
{
	std::vector<SoundStorage*> *buf=EngineLayer::instance()->getAudioBuffers();
	delete (*buf)[i];
	if (i==buf->size()-1)
	buf->pop_back();
	else
	(*buf)[i]=NULL;
}

int _engineprivate::CallbackPlaySound(int i,float l,float r,int prio,int loops,float speed)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	std::vector<SoundStorage*> *buf=EngineLayer::instance()->getAudioBuffers();
	unsigned int sndpos=0;
	int lowestprio=INT_MAX;
	unsigned long long int lowestage=ULLONG_MAX;
	int lowest=0;

	//vol
	float ratio=l>r?l:r;
	l/=ratio;
	r/=ratio;
	float pos=(l+r)/2.0f;
	sf::Vector3f pan(pos,0,pos<0?-pos-1.0f:pos-1.0f);

	while (sndpos<snd->size())//find free id
	{
		if ((*snd)[sndpos]==NULL)
		break;
		else
		if (((*snd)[sndpos]->priority<lowestprio)
		||  ((*snd)[sndpos]->priority==lowestprio&&((*snd)[sndpos]->age<lowestage)))
		{
			lowestprio=(*snd)[sndpos]->priority;
			lowestage=(*snd)[sndpos]->age;
			lowest=sndpos;
		}
		if (((*snd)[sndpos]->sound&&(*snd)[sndpos]->sound->getStatus()==sf::SoundSource::Status::Stopped)
		||	((*snd)[sndpos]->music&&(*snd)[sndpos]->music->getStatus()==sf::SoundSource::Status::Stopped))
		{
			delete (*snd)[sndpos];
			(*snd)[sndpos]=NULL;
			break;
		}

		sndpos++;
	}

	sf::Sound *sound=NULL;
	sf::Music *music=NULL;
	if (((Sound*)((*buf)[i]->sound))->isStreamed())
	{
		music=new sf::Music;
		if (!music->openFromFile(((Sound*)((*buf)[i]->sound))->getFile()))
		{
			Log::error("Resources",std::string("Unable to stream audio file \"")+((Sound*)((*buf)[i]->sound))->getFile()+"\"");
		}
		else
		{
			music->play();
			music->setLoop(loops!=0);
			music->setPitch(speed);
			music->setVolume(ratio*100.0f);
			if (sndpos==snd->size())
			{
				if (snd->size()>=255)
				{
					if ((*snd)[lowest]->sound)
					(*snd)[lowest]->sound->stop();
					if ((*snd)[lowest]->music)
					(*snd)[lowest]->music->stop();
					delete (*snd)[lowest];
					(*snd)[lowest]=new SoundInstance(music,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime());
				}
				else
				snd->push_back(new SoundInstance(music,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime()));
			}
			else
			(*snd)[sndpos]=new SoundInstance(music,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime());
		}
	}
	else
	{
		sound=new sf::Sound;
		sound->setBuffer(*((*buf)[i]->buffer));
		sound->play();
		sound->setLoop(loops!=0);
		sound->setPitch(speed);
		sound->setVolume(ratio*100.0f);

		if (sndpos==snd->size())
		{
			if (snd->size()>=255)
			{
				if ((*snd)[lowest]->sound)
				(*snd)[lowest]->sound->stop();
				if ((*snd)[lowest]->music)
				(*snd)[lowest]->music->stop();
				delete (*snd)[lowest];
				(*snd)[lowest]=new SoundInstance(sound,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime());
			}
			else
			snd->push_back(new SoundInstance(sound,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime()));
		}
		else
		(*snd)[sndpos]=new SoundInstance(sound,(*buf)[i]->sound,prio,EngineLayer::instance()->getPassedTime());
	}

	return sndpos;
}

void _engineprivate::CallbackPauseSound(int i)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	if ((*snd)[i]->sound)
	(*snd)[i]->sound->pause();
	if ((*snd)[i]->music)
	(*snd)[i]->music->pause();
}

void _engineprivate::CallbackUnpauseSound(int i)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	if ((*snd)[i]->sound)
	if ((*snd)[i]->sound->getStatus()==sf::SoundSource::Status::Paused)
	(*snd)[i]->sound->play();
	if ((*snd)[i]->music)
	if ((*snd)[i]->music->getStatus()==sf::SoundSource::Status::Paused)
	(*snd)[i]->music->play();
}

void _engineprivate::CallbackStopSound(int i)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	if ((*snd)[i]->sound)
	(*snd)[i]->sound->stop();
	if ((*snd)[i]->music)
	(*snd)[i]->music->stop();

	delete (*snd)[i];
	(*snd)[i]=NULL;

	if (i==snd->size()-1)
	{
		while(snd->size()>0&&(*snd)[snd->size()-1]==NULL)
		snd->pop_back();
	}
}

void _engineprivate::CallbackSpeedSound(int i,float spd)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	if ((*snd)[i]->sound)
	(*snd)[i]->sound->setPitch(spd);
	if ((*snd)[i]->music)
	(*snd)[i]->music->setPitch(spd);
}

void _engineprivate::CallbackVolumeSound(int i,float left,float right)
{
	std::vector<SoundInstance*> *snd=EngineLayer::instance()->getSounds();
	float ratio=left>right?left:right;
	left/=ratio;
	right/=ratio;
	float pos=(left+right)/2.0f;
	sf::Vector3f pan(pos,0,pos<0?-pos-1.0f:pos-1.0f);
	if ((*snd)[i]->sound)
	(*snd)[i]->sound->setVolume(ratio*100.0f);
	if ((*snd)[i]->music)
	(*snd)[i]->music->setVolume(ratio*100.0f);
}

void _engineprivate::CallbackCloseApplication()
{
	/*empty, handled on the inside*/
}
