#pragma once

#include "enginesettings.h"
#include "hitbox.h"

namespace _engineprivate
{
	class EngineLayer;
}

namespace _engineprivate
{
	using namespace _engineprivate;

	class CollisionBox:public Hitbox
	{
		friend class EngineLayer;

		private:
		CollisionBox(Object *obj,double px,double py,double w,double h);

		public:
	};
	class CollisionCircle:public Hitbox
	{
		friend class EngineLayer;

		private:
		CollisionCircle(Object *obj,double px,double py,double r);

		public:
	};
	class CollisionLine:public Hitbox
	{
		friend class EngineLayer;

		private:
		CollisionLine(Object *obj,double px,double py,double dx,double dy);

		public:
	};
	class CollisionPolygon:public Hitbox
	{
		friend class EngineLayer;

		private:
		CollisionPolygon(Object *obj,double px,double py,double px2,double py2,double px3,double py3);

		public:
	};
}
