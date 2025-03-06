/* 
 * File:   CLEV_ColorRGB.cpp
 * Author: alunos
 * 
 * Created on 14 de Setembro de 2012, 14:28
 */

#include <iostream>
#include "levcolorsRGB.h"

CLEV_ColorRGB::CLEV_ColorRGB(int r, int g, int b)
{
    
    //distribui os valores RGB para as variaveis
    red = r;
    green = g;
    blue = b;
    
}

//CLEV_ColorRGB::CLEV_ColorRGB(const CLEV_ColorRGB& orig)
//{
//}

CLEV_ColorRGB::~CLEV_ColorRGB()
{
}

int CLEV_ColorRGB::getRedCode()
{  
    return red;
}

int CLEV_ColorRGB::getGreenCode()
{
    return green;
}

int CLEV_ColorRGB::getBlueCode()
{
    return blue;
}

void CLEV_ColorRGB::setRedCode(int r)
{  
    red = r;
}

void CLEV_ColorRGB::setGreenCode(int g)
{
    green = g;
}

void CLEV_ColorRGB::setBlueCode(int b)
{
    blue = b;
}

void CLEV_ColorRGB::setRGB(int r, int g, int b)
{
    red = r;
    green = g;
    blue = b;
}


CLEV_ColorRGB& CLEV_ColorRGB::operator = (const CLEV_ColorRGB &a)
{
	red 			=	a.red;
	green			=	a.green;
	blue			=	a.blue;
	return *this;
}

////////////////////////
//The operator==() function
////////////////////////
bool CLEV_ColorRGB::operator == (const CLEV_ColorRGB &a)
{ 
	bool ret = false;

	//if (status && a.status)
	{
		if (red == a.red) 
		{
			if (green == a.green)
			{
				if (blue == a.blue)
					ret = true;
			}
		}

	}

	return ret;
}

////////////////////////
//The operator!=() function
////////////////////////
bool CLEV_ColorRGB::operator != (const CLEV_ColorRGB &a)
{ 
	bool ret = true;

	//if (status && a.status)
	{
		if (red == a.red)
		{
			if (green == a.green)
			{
				if (blue == a.blue) ret = false;
			}
		}
	}
	return ret;
}
