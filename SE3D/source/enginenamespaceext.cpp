#include "../include/enginenamespaceext.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

double _ENGINESPACE::lengthdirX(double a,double x,double y)
{
	return cos(degToRad(a))*x+sin(degToRad(a))*y;
}

double _ENGINESPACE::lengthdirY(double a,double x,double y)
{
	return -sin(degToRad(a))*x-cos(degToRad(a))*y;
}
