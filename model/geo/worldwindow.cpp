// World_Window.cpp: implementation of the CWorld_Window class.
//
//////////////////////////////////////////////////////////////////////

#include "worldwindow.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorld_Window::CWorld_Window()
{
	x_min = -1.0;
	x_max = 1.0;
	y_min = -1.0;
	y_max = 1.0;

}

CWorld_Window::~CWorld_Window()
{

}
double CWorld_Window::GetXMin()
{
	return x_min;
}
double CWorld_Window::GetXMax()
{
	return x_max;
}
double CWorld_Window::GetYMin()
{
	return y_min;
}
double CWorld_Window::GetYMax()
{
	return y_max;
}
void CWorld_Window::SetXMin(double posx)
{
	x_min = posx;
}
void CWorld_Window::SetXMax(double posx)
{
	x_max = posx;
}
void CWorld_Window::SetYMin(double posy)
{
	y_min = posy;
}
void CWorld_Window::SetYMax(double posy)
{
	y_max = posy;
}
