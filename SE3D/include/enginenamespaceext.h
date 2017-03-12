#pragma once

#include "enginesettings.h"
#include "enginenamespace.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace _ENGINESPACE
{
	template <typename T>
	std::string toString(const T &value)
	{
		std::ostringstream os;
		os<<value;
		return os.str();
	}
	inline double degToRad(double a)
	{
		return a*M_PI/180.0f;
	}
	inline double radToDeg(double a)
	{
		return a*180.0f/M_PI;
	}
	double lengthdirX(double angle,double x,double y);
	double lengthdirY(double angle,double x,double y);
	template <class T,class F,class E>
	inline T limit(T a,F min,E max)
	{
		return (a<min?min:(a>max?max:a));
	}
	template <class T,class F>
	inline T max(F a,T b)
	{
		return (a>b?a:b);
	}
	template <class T,class F>
	inline T min(F a,T b)
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
	inline double angle(double x1,double y1,double x2,double y2)
	{
		return (x1-x2)==0?(y1-y2<0?270:90):radToDeg((x1-x2)>0?M_PI+atan((y2-y1)/(x1-x2)):((y2-y1<=0)?((y2-y1==0)?0:atan((y2-y1)/(x1-x2))):2*M_PI+atan((y2-y1)/(x1-x2))));
	}
	template<class T>
	inline char getByte(T i,unsigned int pos=0)
	{
		return (i>>(8*pos))&(0xFF);
	}
	template<class T>
	inline T setByte(T i,char value,unsigned int pos=0)
	{
		T v=(((T)value)&0xFF)<<(8*pos);
		T mask=~(0xFF<<(8*pos));
		return v|((i)&mask);
	}
	template<class T>
	bool writeBytes(std::string *str,T value,unsigned int bytes=0)
	{
		if (bytes==0)
		bytes=sizeof(T);

		for (unsigned int i=0;i<bytes;i++)
		*str+=(char)getByte(value,bytes-1-i);

		return 1;
	}
	template<class T>
	bool readBytes(std::string *str,T *value,unsigned int bytes=0)
	{
		if (bytes==0)
		bytes=sizeof(T);

		*value=0;
		for (unsigned int i=0;i<bytes;i++)
		{
			if (str->empty())
			return 0;
			*value=setByte(*value,(*str)[0],bytes-i-1);
			str->erase(0,1);
		}

		return 1;
	}

}
