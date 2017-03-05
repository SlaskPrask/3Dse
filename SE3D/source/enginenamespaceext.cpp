#include "../include/enginenamespaceext.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

double _ENGINESPACE::lengthdirX(double angle,double x,double y)
{
	return cos(degToRad(angle))*x+sin(degToRad(angle))*y;
}

double _ENGINESPACE::lengthdirY(double angle,double x,double y)
{
	return -sin(degToRad(angle))*x-cos(degToRad(angle))*y;
}
