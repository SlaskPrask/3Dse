#pragma once

#include "enginesettings.h"
#include "enginenamespace.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace _ENGINESPACE
{
	inline double degToRad(double a)
	{
		return a*M_PI/180.0f;
	}
	inline double radToDeg(double a)
	{
		return a*180.0f/M_PI;
	}
	double lengthdirX(double a,double x,double y);
	double lengthdirY(double a,double x,double y);
	template <class T,class F,class E>
	inline T limit(T a,F min,E max)
	{
		return (a<min?min:(a>max?max:a));
	}
	template <class T,class F>
	inline T max(T a,F b)
	{
		return (a>b?a:b);
	}
	template <class T,class F>
	inline T min(T a,F b)
	{
		return (a<b?a:b);
	}
	template <class T>
	inline T abs(T a)
	{
		return (a<0?-a:a);
	}
	template <class T>
	inline T sign(T a)
	{
		return (a>0?1:(a<0?-1:0));
	}
	inline double pi()
	{
		return M_PI;
	}
	inline double distance(double x1,double y1,double x2,double y2)
	{
		return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	}
}
