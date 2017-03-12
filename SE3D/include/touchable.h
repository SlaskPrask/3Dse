#pragma once

#include "enginesettings.h"
#include "object.h"

namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Touchable
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		Object* pointer;
		double depth;
		void init();
		void fixPosition();
		bool relative;
		double xr,yr,x,y,w,h;
		int mouse;
		bool inside;
		void hook(int m);
		bool enabled;
		
		public:
		Touchable();
		Touchable(double depth);
		Touchable(Object *o);
		void setDepth(double d);
		void setObject(Object *o);
		Touchable* setPosition(double xp,double yp);
		Touchable* setSize(double wp,double hp);
		double getX();
		double getY();
		inline Touchable* at(double xp,double yp)
		{
			return setPosition(xp,yp);
		}
		inline double getDepth()
		{
			return (pointer!=NULL?pointer->getDepth():depth);
		}
		inline bool isInside()
		{
			return inside;
		}
		inline void unsetRelative()
		{
			relative=0;
		}
		inline void setRelative(int r=1)
		{
			relative=1;
		}
		inline bool isRelative()
		{
			return relative;
		}
		inline bool isTouched()
		{
			return (mouse!=-1);
		}
		inline int getTouch()
		{
			return mouse;
		}
		inline bool isEnabled()
		{
			return enabled;
		}
		inline void enable()
		{
			enabled=1;
		}
		void disable();
		bool getPress();
		bool getRelease();
		bool getHeld();
		bool getIdle();
		double getTouchX();
		double getTouchY();
		double getWidth()
		{
			return w;
		}
		double getHeight()
		{
			return h;
		}
		inline bool getClick()
		{
			return getRelease()&&isInside();
		}
		~Touchable();

		#ifdef DEBUG
		void draw();
		#endif
	};
}
