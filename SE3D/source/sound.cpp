#include "../include/sound.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void Sound::init()
{
	loaded=0;
	playId=0;
	loadedId=0;
}

void Sound::load(const std::string &file,volatile bool threaded)
{
	if (loaded)
	unload();
	
	loadedId=CallbackLoadSound(file);
	if (loadedId!=-1)
	{
		loaded=1;
		playId=0;
		looping=0;
		#ifndef ANDROID
		EngineLayer::instance()->assignSoundToBuffer(loadedId,this);
		#endif
	}
	else
	loadedId=0;
}
void Sound::unload()
{
	if (loaded)
	{
		CallbackUnloadSound(loadedId);
		loaded=0;
		playId=0;
		loadedId=0;
		looping=0;
	}
}

void Sound::stop()
{
	if (looping)
	looping=0;
	CallbackStopSound(playId);
}

Sound::Sound(const std::string &file,volatile bool threaded)
{
	init();
	load(file,threaded);
}

int Sound::play(double l,double r,int prio,bool loop,double speed)
{
	if (looping)
	CallbackStopSound(playId);
	looping=loop?1:0;
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
