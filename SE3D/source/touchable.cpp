#include "../include/touchable.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void Touchable::init()
{
	EngineLayer::instance()->addTouchable(this);
	relative=0;
	mouse=-1;
	inside=0;
	enabled=1;
}

Touchable::Touchable()
{
	x=y=xr=yr=w=h=0;
	depth=0;
	pointer=NULL;
	init();
}

Touchable::Touchable(double depth)
{
	x=y=xr=yr=w=h=0;
	depth=depth;
	pointer=NULL;
	init();
}

Touchable::Touchable(Object *o)
{
	x=y=xr=yr=w=h=0;
	depth=0;
	pointer=o;
	init();
}

void Touchable::disable()
{
	mouse=-1;
	inside=0;
	enabled=0;
}

Touchable* Touchable::setPosition(double xp,double yp)
{
	xr=xp;
	yr=yp;
	if (pointer==NULL)
	{
		x=xr;
		y=yr;
	}
	return this;
}

Touchable* Touchable::setSize(double wp,double hp)
{
	w=wp;
	h=hp;
	return this;
}

void Touchable::fixPosition()
{
	if (relative&&pointer!=NULL)
	{
		x=pointer->x+xr;
		y=pointer->y+yr;
	}
	else
	{
		x=xr;
		y=yr;
	}
}

void Touchable::setDepth(double d)
{
	depth=d;
	pointer=NULL;
}

void Touchable::setObject(Object *o)
{
	pointer=o;
}

void Touchable::hook(int m)
{
	if (mouse==-1||m==-1)
	{
		mouse=m;
		inside=(m!=-1);
	}
}

double Touchable::getX()
{
	if (relative&&pointer!=NULL)
	return pointer->x+xr;
	else
	return x;
}
double Touchable::getY()
{
	if (relative&&pointer!=NULL)
	return pointer->y+yr;
	else
	return y;
}

bool Touchable::getPress()
{
	if (mouse!=-1)
	return EngineLayer::instance()->getMousePress(mouse);
	else
	return 0;
}

bool Touchable::getRelease()
{
	if (mouse!=-1)
	return EngineLayer::instance()->getMouseRelease(mouse);
	else
	return 0;
}

bool Touchable::getHeld()
{
	if (mouse!=-1)
	return EngineLayer::instance()->getMouseHeld(mouse);
	else
	return 0;
}

bool Touchable::getIdle()
{
	if (mouse==-1)
	return 1;
	else
	return EngineLayer::instance()->getMouseUnheld(mouse);
}

double Touchable::getTouchX()
{
	if (mouse==-1)
	return getX()+w/2.0f;
	else
	return EngineLayer::instance()->getMouseTranslatedX(mouse)-getX();
}

double Touchable::getTouchY()
{
	if (mouse==-1)
	return getY()+h/2.0f;
	else
	return EngineLayer::instance()->getMouseTranslatedY(mouse)-getY();
}

Touchable::~Touchable()
{
	EngineLayer::instance()->removeTouchable(this);
}

#ifdef DEBUG
void Touchable::draw()
{
	if (getPress())
	EngineLayer::instance()->drawRectangle(getX(),getY(),w,h,0,0,0,1,.30);
	else
	if (getRelease())
	EngineLayer::instance()->drawRectangle(getX(),getY(),w,h,0,1,0,1,.30);
	else
	if (getHeld())
	EngineLayer::instance()->drawRectangle(getX(),getY(),w,h,0,1,1,0,.15);
	else
	EngineLayer::instance()->drawRectangle(getX(),getY(),w,h,0,0,1,0,.15);
}
#endif
