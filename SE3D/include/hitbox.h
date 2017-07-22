#pragma once

#include "enginesettings.h"
#include <vector>

namespace _ENGINESPACE
{
	class Object;
}

namespace _engineprivate
{
	using namespace _ENGINESPACE;

	class Collision;
	class EngineLayer;
	
	class Hitbox
	{
		friend class Collision;
		friend class EngineLayer;
		
		private:
		inline double limit(double v,double min,double max)
		{
			return v<min?min:(v>max?max:v);
		}

		protected:
		Collision *map;
		double x,y,x2,y2,x3,y3;
		//x2=ellipse radius
		const double *ox,*oy;
		int type;
		enum ShapeType{Line,Polygon,Box,Ellipse};
		Object *object;
		inline void setMap(Collision *c)
		{
			map=c;
		}
		Hitbox();
		bool collide(std::vector<Object*> *vec);

		int where(Collision* c);
		/*0=top left
		1=top right
		2=bottom left
		3=bottom right
		-1=not in
		4=parent*/

		bool inside(Collision* c);
		/*true=fits
		false=won't fit*/

		int getExpansion(Collision* c);
		/*c will be * of parent
		0=top left
		1=top right
		2=bottom left
		3=bottom right*/

		bool getCollision(Hitbox *h);
		//virtual int inside(const double& px,const double& py,const double& px2,const double& py2) const;

		public:
		~Hitbox();
		double getRight();
		double getTop();
		double getLeft();
		double getBottom();

		inline double getX()
		{return x;}
		inline double getY()
		{return x;}
		inline double getX2()
		{return x2;}
		inline double getY2()
		{return y2;}
		inline double getX3()
		{return x3;}
		inline double getY3()
		{return y3;}
		inline double getOx()
		{return *ox;}
		inline double getOy()
		{return *oy;}
		inline int getType()
		{
			return type;
		}

		inline Object *getObject()
		{
			return object;
		}

		#ifdef DEBUG
		virtual void draw();
		#endif

		void move();
	};
}
