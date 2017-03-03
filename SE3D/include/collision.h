#pragma once

#include "enginesettings.h"
#include "hitbox.h"
#include <vector>

namespace _ENGINESPACE
{
	class Object;
}

namespace _engineprivate
{
	using namespace _ENGINESPACE;

	class Hitbox;

	class Collision
	{
		private:
		double x,y,x2,y2;
		Collision *parent;
		Collision *child[4];
		std::vector<Hitbox*> hitbox;
		int depth;

		inline bool children() const
		{
			return (child[0]!=0);
		}
		void split();
		void collideChildren(Hitbox *h,std::vector<Object*> *vec);
		void addHitbox(Hitbox *h);
		void removeHitbox(Hitbox *h);
		void expand(Hitbox *h);
		void giveChild(Collision* c,int index);
		void insert(Hitbox *h);
		void attemptCombine();

		public:
		Collision(double fx,double fy,double fx2,double fy2,int d,Collision *par=0);
		void reverseAdd(Hitbox *h);
		void add(Hitbox *h);
		bool collide(Hitbox *h,std::vector<Object*> *vec);
		void move(Hitbox* h);
		void remove(Hitbox* h);

		inline double getLeft() const
		{
			return x;
		}

		inline double getTop() const
		{
			return y;
		}

		inline double getRight() const
		{
			return x2;
		}

		inline double getBottom() const
		{
			return y2;
		}

		#ifdef DEBUG
		void draw(bool c=0);
		#endif
	};
}
