#include "../include/sound.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void Sound::init()
{
	loaded=0;
	playId=-1;
	loadedId=-1;
	streamed=0;
	file="";
}

void Sound::load(const std::string &file,volatile bool threaded,bool stream)
{
	if (loaded)
	unload();
	
	this->file=file;
	streamed=stream;
	loadedId=CallbackLoadSound(file,streamed);
	if (loadedId!=-1)
	{
		loaded=1;
		playId=-1;
		looping=0;
		#ifndef ANDROID
		EngineLayer::instance()->assignSoundToBuffer(loadedId,this);
		#endif
	}
	else
	loadedId=-1;
}
void Sound::unload()
{
	if (loaded)
	{
		file="";
		CallbackUnloadSound(loadedId);
		loaded=0;
		playId=-1;
		loadedId=-1;
		looping=0;
	}
}

void Sound::stop()
{
	if (looping)
	looping=0;
	if (playId!=-1)
	CallbackStopSound(playId);
	playId=-1;
}

Sound::Sound(const std::string &file,volatile bool threaded,bool stream)
{
	init();
	load(file,threaded,stream);
}

int Sound::play(double l,double r,int prio,bool loop,double speed)
{
	looping=loop?1:0;
	if (loadedId!=-1)
	playId=CallbackPlaySound(loadedId,(float)l,(float)r,prio,looping?-1:0,(float)speed);
	return playId;
}

Sound::Sound()
{
	init();
}

Sound::~Sound()
{
	unload();
}
