//=============================================================================
//	Arquivo:			LevPrimitive.cpp
//.............................................................................
//	Descricao:			Implementacao da classe CLevPrimitive
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//.............................................................................
//	Observacoes:
//
//=============================================================================


#include "levprimitive.h"
#include <stdio.h>


CLevPrimitive::CLevPrimitive()
{
	Reinitialize();
}

CLevPrimitive::~CLevPrimitive()
{
	if(vpPoints.size()) vpPoints.clear();
	if(vpLines.size()) vpLines.clear();
}

void CLevPrimitive::Reinitialize()
{	
	if (vpPoints.size())
		vpPoints.clear();
	if (vpLines.size())
		vpLines.clear();
	num_polygons = 0;
	pt_minxminy.Reinitialize();
	pt_maxxmaxy.Reinitialize();
	pt_minxminy.X(1e308); pt_minxminy.Y(1e308); pt_minxminy.Z(1e308);
	pt_maxxmaxy.X(-1e308); pt_maxxmaxy.Y(-1e308); pt_maxxmaxy.Z(1e308);

	CLevGeoEntity::Reinitialize();
}

void CLevPrimitive::Limits(CLevPoint & p1, CLevPoint & p2)
{
	p1 = pt_minxminy;
	p2 = pt_maxxmaxy;
}

void CLevPrimitive::TakeLimits()
{
	unsigned int i;

	pt_minxminy.X(1e308);
	pt_minxminy.Y(1e308);
	pt_minxminy.Z(1e308);

	pt_maxxmaxy.X(-1e308);
	pt_maxxmaxy.Y(-1e308);
	pt_maxxmaxy.Z(1e308);

	for (i = 0; i < vpPoints.size(); i++) {
		if (vpPoints[i]->X() <= pt_minxminy.X()) pt_minxminy.X(vpPoints[i]->X());
		if (vpPoints[i]->X() >= pt_maxxmaxy.X()) pt_maxxmaxy.X(vpPoints[i]->X());

		if (vpPoints[i]->Y() <= pt_minxminy.Y()) pt_minxminy.Y(vpPoints[i]->Y());
		if (vpPoints[i]->Y() >= pt_maxxmaxy.Y()) pt_maxxmaxy.Y(vpPoints[i]->Y());
	}
}

void CLevPrimitive::SetPreDiscretizationData(int nPoints,double factor, CLevPoint clickedPoint)
{
	oPredis.SetPreDiscretizationData(nPoints,factor, clickedPoint.X(),clickedPoint.Y());
}

CPredis	CLevPrimitive::GetPreDiscretizationData(void)
{
	return oPredis;
}

void CLevPrimitive::GetPredisPoints(vCLevPoint & v)
{
	vCLevPoint vPredisPoints;

	oPredis.GetPredisPoints(vPredisPoints);

	if(vPredisPoints.size())
	{
		v = vPredisPoints;
	}
	else
	{
		PreDiscretize(v);
	}
}

//-----------------------------------------------------------------------------------------------------
//	FixEntity(bool )
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: 	Atribui vari�vel de controle para manter ou n�o a primtiva fixa,
//				ou seja, seus pontos n�o poder�o ser rotacionados nem movidos
//				Isto � �til em opera��es Move e Rotate de Grupo.
//
//	par�metros:	bool
//
//	retorno:	
//
//	Observa��es:
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Abril/20012
//	Modificado por:	
//	Modificado em:	
//	Motivo:			
//					
//					
//-----------------------------------------------------------------------------------------------------
void CLevPrimitive::FixEntity(bool fE)
{
	int i;
	int num;

	fixedEntity = fE;

	num = vpPoints.size();

	if(num>5) num = 5;
	for(i=0;i<num;i++) 
		vpPoints[i]->FixEntity(fE);
}

//-----------------------------------------------------------------------------------------------------
bool CLevPrimitive::FixEntity(void)
{
	return fixedEntity;
}
//-----------------------------------------------------------------------------------------------------
