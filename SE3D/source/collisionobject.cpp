#include "../include/collisionobject.h"
#include "../include/enginelayer.h"
#include "../include/collision.h"
#include "../include/object.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

CollisionBox::CollisionBox(Object *obj,double px,double py,double w,double h)
{
	x=px;
	y=py;
	object=obj;
	type=Box;
	x2=px+w;
	y2=py+h;
	ox=&(object->x);
	oy=&(object->y);

	EngineLayer::instance()->getMasterCollision()->add(this);
}

CollisionCircle::CollisionCircle(Object *obj,double px,double py,double r)
{
	x=px;
	y=py;
	object=obj;
	type=Ellipse;
	x2=r;
	ox=&(object->x);
	oy=&(object->y);

	EngineLayer::instance()->getMasterCollision()->add(this);
}

CollisionLine::CollisionLine(Object *obj,double px,double py,double dx,double dy)
{
	x=px;
	y=py;
	object=obj;
	type=Line;
	x2=dx;
	y2=dy;
	ox=&(object->x);
	oy=&(object->y);

	EngineLayer::instance()->getMasterCollision()->add(this);
}

CollisionPolygon::CollisionPolygon(Object *obj,double px,double py,double px2,double py2,double px3,double py3)
{
	x=px;
	y=py;
	object=obj;
	type=Polygon;
	x2=px2;
	y2=py2;
	x3=px3;
	y3=py3;
	ox=&(object->x);
	oy=&(object->y);

	EngineLayer::instance()->getMasterCollision()->add(this);
}
