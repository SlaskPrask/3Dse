#include "../include/sound.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

inline void Sound::load(const std::string &file,volatile bool threaded)
{
	if (loaded)
	unload();
	
	loadedId=CallbackLoadSound(file);
	loaded=1;
	playId=0;
	looping=0;
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
	loaded=0;
	playId=0;
	loadedId=0;
}

Sound::~Sound()
{
	unload();
}
