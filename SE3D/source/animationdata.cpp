#include "../include/animationdata.h"
#include "../include/platform.h"
#include "../include/log.h"
#include "../include/enginewrapper.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

AnimationData::AnimationData(bool rt):realTime(rt)
{
	loop=1;
	duration=0;
}

void AnimationData::recalculateDuration()
{
	duration=0;
	for(unsigned int i=0;i<frames.size();i++)
	duration+=frames[i].getDuration();
}

void AnimationData::addFrameStrip(double x,double y,double w,double h,int imagesx,int start,int length,double duration)
{
	for(int i=0;i<length;i++)
	{
		int xpos=(i+start)%imagesx;
		int ypos=((i+start)-((i+start)%imagesx))/imagesx;
		addFrame(x+xpos*w,y+ypos*h,w,h,duration);
	}
	recalculateDuration();
}

void AnimationData::setFrameDuration(unsigned int i,double dur)
{
	if (i<0||i>=frames.size())
	{
		DEBUGFUNC(Log::notify(std::string("Animation","Attempting to set duration of unavailable frame ")+to_string(i)));
		return;
	}
	
	frames[i].setDuration(dur);
	recalculateDuration();
}

int AnimationData::getFrameAt(double pos)
{
	if (pos>=duration)
	return getFrames()-1;
	double buffer=0;
	for (unsigned int i=0;i<frames.size();i++)
	{
		buffer+=frames[i].getDuration();
		if (pos<buffer)
		return i;
	}
	return getFrames()-1;
}
