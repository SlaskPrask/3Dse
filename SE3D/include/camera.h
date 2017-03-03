#pragma once

#include "enginesettings.h"
namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Camera
	{
		friend class _engineprivate::EngineLayer;
		private:
		double w,h;
		double boundsLeft,boundsRight,boundsTop,boundsBottom;
		double limitsLeft,limitsRight,limitsTop,limitsBottom;
		bool limits;
		const double *xfollow,*yfollow;
		void doFollow();
		void init();
		inline void checkXLimits()
		{
			if (limits)
			if (x<limitsLeft)
			x=limitsLeft;
			else
			if(x+w>limitsRight)
			x=limitsRight-w;
		}
		inline void checkYLimits()
		{
			if (limits)
			if (y<limitsTop)
			y=limitsTop;
			else
			if(y+h>limitsBottom)
			y=limitsBottom-h;
		}
		
		public:
		double x,y;
		
		Camera();
		Camera(double xp,double yp,double pw,double ph);
		void at(double xp,double yp);
		inline void setPosition(double xp,double yp)
		{
			at(xp,yp);
		}
		inline void setPositionCentered(double xp,double yp)
		{
			at(xp-w/2.0f,yp-h/2.0f);
		}
		inline void unfollow()
		{
			xfollow=yfollow=0;
		}
		inline bool isFollowing()
		{
			return (xfollow==0?0:1);
		}
		inline void follow(const double *xp,const double *yp)
		{
			xfollow=xp;
			yfollow=yp;
		}
		/*inline void follow(Object *obj)
		{
			xfollow=&(obj->x);
			yfollow=&(obj->y);
		}*/
		inline double getBoundsTop()
		{
			return boundsTop;
		}
		inline double getBoundsBottom()
		{
			return boundsBottom;
		}
		inline double getBoundsRight()
		{
			return boundsRight;
		}
		inline double getBoundsLeft()
		{
			return boundsLeft;
		}
		inline double getLimitsTop()
		{
			return limitsTop;
		}
		inline double getLimitsBottom()
		{
			return limitsBottom;
		}
		inline double getLimitsRight()
		{
			return limitsRight;
		}
		inline double getLimitsLeft()
		{
			return limitsLeft;
		}
		void setBounds(double left,double top,double right,double bottom);
		void setLimits(double left,double top,double right,double bottom);
		inline void setBounds(double hor,double ver)
		{
			setBounds(hor,ver,hor,ver);
		}
		inline void setLimits(double right,double bottom)
		{
			setLimits(0,0,right,bottom);
		}
		void setSize(double pw,double ph);
		void setExpandedSize(double pw,double ph,double internalWidth,double internalHeight,double positionRatio);
		void setExpandedSize(double pw,double ph,double internalWidth,double internalHeight);
		bool isActive();
		void activate();
		inline double getLeft()
		{
			return x;
		}
		inline double getTop()
		{
			return y;
		}
		inline double getRight()
		{
			return x+w;
		}
		inline double getBottom()
		{
			return y+h;
		}
		inline double getWidth()
		{
			return w;
		}
		inline double getHeight()
		{
			return h;
		}
		inline void unsetLimits()
		{
			limits=0;
		}
		
		~Camera();
	};
}

