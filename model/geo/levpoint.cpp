
//=============================================================================
//	Arquivo:			LevPoint.cpp
//.............................................................................
//	Descricao:			Implementacao da classe CLevPoint
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//.............................................................................
//	Observacoes:
//
//=============================================================================


#include "levdef.h"
#include "levpoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


CLevPoint::CLevPoint()
{
	Reinitialize();
}


CLevPoint::~CLevPoint()
{ }


//-----------------------------------------------------------------------------------------------------
//	Move()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//					Anna Carolina S. Volpi (Adaptado de System_POI)
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevPoint::Move(double deltax, double deltay, double deltaz)
{
	if(!fixedEntity)
	{
		x += deltax;
		y += deltay;
		z += deltaz;
	}
}


//========================================================
//========================================================
void CLevPoint::ReflectionByX(void)
{
		///reflexao sobre o eixo x
		if(!fixedEntity)
		{
			x=x*1;
			y=y*(-1); //1
			z=z*1;
		//========================
			TakeLimits();
		}
}
void CLevPoint::ReflectionByY(void)
{
		//reflexao sobre o eixo y
		if(!fixedEntity)
		{
			x=x*(-1);
			y=y*1;
			z=z*1;
		//========================
			TakeLimits();
		}
}
//========================================================
//========================================================

/*-----------------------------------------------------------------------------------------------------
//	Rotation()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//					Anna Carolina S. Volpi (Adaptado de System_POI)
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------*/
void CLevPoint::Rotation(double angle, CLevPoint &pointc)
{
	double xc, yc, xlc, ylc, angulo_rad;

	if(!fixedEntity)
	{
		xc = x -  pointc.x;
		yc = y -  pointc.y;
		angulo_rad = 3.1415926535 * angle / 180;
		xlc = cos(angulo_rad) * xc - sin(angulo_rad) * yc;
		ylc = sin(angulo_rad) * xc + cos(angulo_rad) * yc;

		x = xlc + pointc.x;
		y = ylc + pointc.y;

	//	// *NMA*: Solu��o paliativa: 09/04/2002
		//*NMA*:	O trecho abaixo arredonda o valores at� 1e-6
		//			Se os valores forem mantidos com mais casas decimais
		//			pode ocorrer erro na gera��o da malha.
		x = x*1e6;
	//	x = ceil(x);
		x = floor(x);
		x = x*1e-6;

		y = y*1e6;
	//	y = ceil(y);
		y = floor(y);
		y = y*1e-6;
	}
}


/*-----------------------------------------------------------------------------------------------------
//	Reinitialize()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------*/
void CLevPoint::Reinitialize(void)
{
	x = y = z = 1e308;
	num_primitives = 0;
	num_ellipses = 0;
	geoEntityType = GEOM;

	CLevGeoEntity::Reinitialize();
}


////////////////////////
// function : Limits
////////////////////////
void CLevPoint::Limits(CLevPoint & p1, CLevPoint & p2)
{
	p1 = *this;
	p2 = *this;
}

double  CLevPoint::CalcDistance(CLevPoint &oOther)
{
	double dist=0;

	dist = sqrt( (x-oOther.x)*(x-oOther.x) + (y-oOther.y) *(y-oOther.y));

	return dist;

}


CLevPoint& CLevPoint::operator = (const CLevPoint &a)
{
	x				=	a.x;
	y				=	a.y;
	z				=	a.z;
	status			=	a.status;
	num_primitives  =   a.num_primitives;
	geoEntityType	=	a.geoEntityType;
	isOfGeometry	=	a.isOfGeometry;

	//propriedades f�sicas
	materialIndex		=	a.materialIndex;
	sourceIndex			=	a.sourceIndex;
	boundConditionIndex	=	a.boundConditionIndex;

	return *this;
}


bool CLevPoint::operator == (const CLevPoint &a)
{  // note-se que n�o s�o comparadas as vari�veis de controle num_lines e num_poligonos.
	// interessa-nos somente a compara��o dos atributos!
	bool ret = false;

	// *NMA*: para evitar que valores com diferen�as muito pequenas
	//        sejam considerados diferentes
	if (status && a.status)
	{
		if (fabs(x - a.x) < 1.e-8)
		{
			if (fabs(y - a.y) < 1.e-8)
			{
				if ( fabs(z - a.z) < 1.e-8) 
					ret = true;
			}
		}

	}


//	if (status && a.status)
//	{
//		if (x == xLocal)
//		{
//			if (y == yLocal)
//			{
//				if ( z ==  zLocal)
//					ret = true;
//			}
//		}
//	}

	return ret;
}

////////////////////////
//The operator!=() function
////////////////////////
bool CLevPoint::operator != (const CLevPoint &a)
{  // note-se que n�o s�o comparadas as vari�veis de controle num_lines e num_poligonos.
	// interessa-nos somente a compara��o dos atributos!

	bool ret = true;

	if (status && a.status)
	{
		if (fabs(x - a.x) < 1.e-8)
		{
			if (fabs(y - a.y) < 1.e-8)
			{
				if ( fabs(z - a.z) < 1.e-8) 
					ret = false;
		         }
	        }
	}
	return ret;
}

//*NMA*:13/06/2005: escalamento
void CLevPoint::Scale(double factor)
{
	if(!fixedEntity)
	{
		x = x*factor;
		y = y*factor;
		z = z*factor;

		fixedEntity = true;
		TakeLimits();

	}

}
