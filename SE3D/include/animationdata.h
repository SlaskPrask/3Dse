#pragma once

#include "enginesettings.h"
#include <vector>
#include "animationframe.h"

namespace _ENGINESPACE
{
	class Animation;
	
	using namespace _engineprivate;
	
	struct AnimationData
	{
		friend class _ENGINESPACE::Animation;
		
		private:
		bool loop;
		std::vector<AnimationFrame> frames;
		bool realTime;
		double duration;
		void recalculateDuration();
		
		inline double getWidth(int i)
		{
			return frames[i].w;
		}
		inline double getHeight(int i)
		{
			return frames[i].h;
		}
		inline double getFromX(int i)
		{
			return frames[i].x1;
		}
		inline double getFromY(int i)
		{
			return frames[i].y1;
		}
		inline double getToX(int i)
		{
			return frames[i].x2;
		}
		inline double getToY(int i)
		{
			return frames[i].y2;
		}
		
		public:
		AnimationData(bool rt=0);
		inline void setLoop(bool l=1)
		{
			loop=l;
		}
		inline void unsetLoop()
		{
			loop=0;
		}
		inline void setRealTime(bool r=1)
		{
			realTime=r;
		}
		inline void unsetRealTime()
		{
			realTime=0;
		}
		inline bool getLoop()
		{
			return loop;
		}
		inline bool getRealTime()
		{
			return realTime;
		}
		inline void addFrame(double x,double y,double w,double h,double duration=1)
		{
			frames.push_back(AnimationFrame(x,y,x+w,y+h,duration));
			recalculateDuration();
		}
		inline double getDuration()
		{
			return duration;
		}
		inline unsigned int getFrames()
		{
			return frames.size();
		}
		int getFrameAt(double pos);
		void addFrameStrip(double x,double y,double w,double h,int imagesx,int start,int length,double duration=1);
		void setFrameDuration(unsigned int i,double dur);
	};
}
