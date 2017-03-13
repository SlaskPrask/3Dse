#include "../include/hitbox.h"
#include "../include/enginelayer.h"
#include "../include/object.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

Hitbox::Hitbox()
{
	map=0;
	object=0;
}

int Hitbox::where(Collision* c)
{
	if (!inside(c))
	return -1;

	if (getLeft()>=(c->getLeft()+c->getRight())/2.0f)//right
	{
		if (getTop()>=(c->getBottom()+c->getTop())/2.0f)//bottom
		return 3;
		if (getBottom()<(c->getBottom()+c->getTop())/2.0f)//top
		return 1;
	}
	else
	if (getRight()<(c->getLeft()+c->getRight())/2.0f)//left
	{
		if (getTop()>=(c->getBottom()+c->getTop())/2.0f)//bottom
		return 2;
		if (getBottom()<(c->getBottom()+c->getTop())/2.0f)//top
		return 0;
	}
	return 4;
}

bool Hitbox::collide(std::vector<Object*> *vec)
{
	return map->collide(this,vec);
}

void Hitbox::move()
{
	map->move(this);
}

bool Hitbox::inside(Collision* c)
{
	return (getLeft()>=c->getLeft()
	&&      getTop()>=c->getTop()
	&&      getRight()<c->getRight()
	&&      getBottom()<c->getBottom());
}

int Hitbox::getExpansion(Collision* c)
{
	if (getLeft()<c->getLeft())//too far left
	{
		if (getBottom()<c->getBottom())//too far up
		return 3;
		else
		return 1;
	}
	else
	{
		if (getBottom()<c->getBottom())//too far up
		return 2;
		else
		return 0;
	}
}

bool Hitbox::getCollision(Hitbox *h)
{
	if (object==h->object)//no self object collision
	return 0;

	switch (type)
	{
		default:
		case Box:
			switch (h->getType())
			{
				default:
					return 0;
				case Line:
					//TODO
					return 0;
				case Polygon:
					//TODO
					return 0;
				case Box:
					if (*ox+x>=h->getOx()+h->getX2()
					||  *oy+y>=h->getOy()+h->getY2()
					||  *ox+x2<h->getOx()+h->getX()
					||  *oy+y2<h->getOy()+h->getY())
					return 0;
					return 1;
				case Ellipse:
					double dx=h->getOx()+h->getX()-limit(h->getOx()+h->getX(),*ox+x,*ox+x2);
					double dy=h->getOy()+h->getY()-limit(h->getOy()+h->getY(),*oy+y,*oy+y2);
					return dx*dx+dy*dy<h->getX2();
			}
			break;
		case Line:
			//TODO
			return 0;
		case Polygon:
			//TODO
			return 0;
		case Ellipse:
			//todo
			return 0;
	}
	return 0;
}

double Hitbox::getRight()
{
	switch (type)
	{
		default:
		case Box:
			return *ox+x2;
		case Line:
			//TODO
			return 0;
		case Polygon:
			//TODO
			return 0;
			return 1;
		case Ellipse:
			return *ox+x+x2;
	}
	return *ox+x2;
}

double Hitbox::getTop()
{
	switch (type)
	{
		default:
		case Box:
			return *oy+y;
		case Line:
			//TODO
			return 0;
		case Polygon:
			//TODO
			return 0;
			return 1;
		case Ellipse:
			return *oy+y-x2;
	}
	return *oy+y;
}

double Hitbox::getLeft()
{
	switch (type)
	{
		default:
		case Box:
			return *ox+x;
		case Line:
			//TODO
			return 0;
		case Polygon:
			//TODO
			return 0;
			return 1;
		case Ellipse:
			return *ox+x-x2;
	}
	return *ox+x;
}

double Hitbox::getBottom()
{
	switch (type)
	{
		default:
		case Box:
			return *oy+y2;
		case Line:
			//TODO
			return 0;
		case Polygon:
			//TODO
			return 0;
			return 1;
		case Ellipse:
			return *oy+y+x2;
	}
	return *oy+y2;
}

Hitbox::~Hitbox()
{
	if (map)
	map->remove(this);
}

