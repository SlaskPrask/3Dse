#include "../include/camera.h"
#include "../include/enginelayer.h"
#include "../include/platformgraphics.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void Camera::init()
{
	xfollow=yfollow=NULL;
	boundsLeft=boundsRight=boundsTop=boundsBottom=0;
	limitsLeft=limitsRight=limitsTop=limitsBottom=0;
	limits=0;
}

Camera::Camera()
{
	x=y=0;
	w=h=1;
	init();
}

Camera::Camera(double xp,double yp,double pw,double ph):x(xp),y(yp),w(pw),h(ph)
{
	init();
}

void Camera::setExpandedSize(double pw,double ph,double internalWidth,double internalHeight,double positionRatio)
{
	if (ph/pw<internalHeight/internalWidth)
	{//wider
		setSize(pw/ph*internalHeight,internalHeight);
		at(-positionRatio*(w-internalWidth),0);
	}
	else
	{
		setSize(internalWidth,ph/pw*internalWidth);
		at(0,-positionRatio*(h-internalHeight));
	}
}

void Camera::setExpandedSize(double pw,double ph,double internalWidth,double internalHeight)
{
	if (ph/pw<internalHeight/internalWidth)
	{//wider
		setSize(pw/ph*internalHeight,internalHeight);
	}
	else
	{
		setSize(internalWidth,ph/pw*internalWidth);
	}
}

void Camera::at(double xp,double yp)
{
	x=xp;
	y=yp;
	checkXLimits();
	checkYLimits();
}

void Camera::doFollow()
{
	if (isFollowing())
	{
		if (boundsLeft+boundsRight>=w)
		{
			x=*xfollow-w/2.0f;
			checkXLimits();
		}
		else
		if (*xfollow<x+boundsLeft)
		{
			x=*xfollow-boundsLeft;
			checkXLimits();
		}
		else
		if (*xfollow>x+w-boundsRight)
		{
			x=*xfollow-w+boundsRight;
			checkXLimits();
		}
		
		if (boundsTop+boundsBottom>=h)
		{
			h=*yfollow-h/2.0f;
			checkYLimits();
		}
		else
		if (*yfollow<y+boundsTop)
		{
			y=*yfollow>y-boundsTop;
			checkYLimits();
		}
		else
		if (*yfollow>y+h-boundsBottom)
		{
			y=*yfollow-h+boundsBottom;
			checkYLimits();
		}
	}
}

void Camera::setSize(double pw,double ph)
{
	if (w==pw&&h==ph)
	return;
	w=pw;
	h=ph;
	if (isActive())
	EngineLayer::instance()->refreshCamera();
	//check limits?
}

void Camera::setBounds(double left,double top,double right,double bottom)
{
	boundsLeft=left;
	boundsRight=right;
	boundsTop=top;
	boundsBottom=bottom;
}

void Camera::setLimits(double left,double top,double right,double bottom)
{
	limitsLeft=left;
	limitsRight=right;
	limitsTop=top;
	limitsBottom=bottom;
	limits=1;
	checkXLimits();
	checkYLimits();
}

bool Camera::isActive()
{
	return (EngineLayer::instance()->getCamera()==this);
}

void Camera::activate()
{
	EngineLayer::instance()->setCamera(this);
}

Camera::~Camera()
{
	if (isActive())
	EngineLayer::instance()->setCamera(NULL);
}
