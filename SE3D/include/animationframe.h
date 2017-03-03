#pragma once

#include "enginesettings.h"

namespace _engineprivate
{
	class AnimationFrame
	{
		private:
		double duration;
		AnimationFrame();
		
		public:
		double x1,y1,x2,y2;
		double w,h;
		AnimationFrame(double xx1,double yy1,double xx2,double yy2,double dur):x1(xx1),y1(yy1),x2(xx2),y2(yy2),duration(dur)
		{
			w=xx2-xx1;
			h=yy2-yy1;
		}
		
		inline void setDuration(double dur)
		{
			duration=dur;
		}
		
		inline double getDuration()
		{
			return (duration<0?0:duration);
		}
	};
}
