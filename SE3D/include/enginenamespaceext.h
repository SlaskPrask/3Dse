#pragma once

#include "enginesettings.h"
#include <string>
#include <climits>
#include <cmath>
#include "enginelayer.h"
#include "font.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	inline double transform(double rat,double val1,double val2)
	{
		return val1*(1-rat)+val2*rat;
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
	inline char getByte(const T i,unsigned int pos=0)
	{
		return (i>>(8*(T)pos))&((T)0xFF);
	}
	template<>
	inline char getByte<bool>(const bool i,unsigned int pos)
	{
		return i?0x01:0x00;
	}
	template<class T>
	inline T setByte(T i,char value,unsigned int pos=0)
	{
		T v=(((T)value)&0xFF)<<(8*pos);
		T n=((T)0xFF<<(8*(T)pos))^(~((T)0x00));
		T mask=(~((T)0))&n;
		return v|((i)&mask);
	}
	template<>
	inline bool setByte<bool>(bool i,char value,unsigned int pos)
	{
		return value!=0x00;
	}
	template<class T>
	inline bool writeBytes(std::string *str,const T value,unsigned int bytes=0)
	{
		if (bytes==0)
		bytes=sizeof(T);

		for (unsigned int i=0;i<bytes;i++)
		*str+=(char)getByte(value,bytes-1-i);

		return 1;
	}
	template<>
	inline bool writeBytes<std::string>(std::string *str,const std::string value,unsigned int bytes)
	{
		if (bytes==0)
		bytes=(unsigned int)(value.length());

		for (unsigned int i=0;i<bytes;i++)
		*str+=value[i];

		return 1;
	}
	inline bool ignoreBytes(std::string *str,unsigned int bytes=0)
	{
		if (str->length()<bytes)
		{
			*str="";
			return 0;
		}
		else
		{
			str->erase(0,bytes);
			return 1;
		}
	}
	template<class T>
	inline bool readBytes(std::string *str,T *value,unsigned int bytes=0)
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
	template<>
	inline bool readBytes<std::string>(std::string *str,std::string *value,unsigned int bytes)
	{
		if (bytes==0)
		bytes=1;

		*value="";
		for (unsigned int i=0;i<bytes;i++)
		{
			if (str->empty())
			return 0;
			*value+=(*str)[0];
			str->erase(0,1);
		}

		return 1;
	}
}
