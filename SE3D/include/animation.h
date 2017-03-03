#pragma once

#include "enginesettings.h"
#include "animationdata.h"
#include "sprite.h"

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Animation
	{
		private:
		AnimationData *data;
		Sprite *sprite;
		double pos;
		double speed;
		inline void updateFrame(int f)
		{
			frame=f;
		}
		int frame;
		
		public:
		inline Sprite* getSprite()
		{
			return sprite;
		}
		inline double getWidth()
		{
			return data->getWidth(frame);
		}
		inline double getHeight()
		{
			return data->getHeight(frame);
		}
		inline double getFromX()
		{
			return data->getFromX(frame)/sprite->getWidth();
		}
		inline double getFromY()
		{
			return data->getFromY(frame)/sprite->getHeight();
		}
		inline double getToX()
		{
			return data->getToX(frame)/sprite->getWidth();
		}
		inline double getToY()
		{
			return data->getToY(frame)/sprite->getHeight();
		}
		void setPosition(double p);
		Animation(Sprite *s,AnimationData *d);
		Animation();
		void setSprite(Sprite *s);
		void setData(AnimationData *d,bool reset=1);
		void run();
		double getPosition();
		int getFrame();
		void setSpeed(double s);
		inline double getSpeed()
		{
			return speed;
		}
	};
}
