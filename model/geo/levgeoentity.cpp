//==========================================================================
//	Arquivo:			LevGeoEntity.cpp
//..........................................................................
//	Descricao:			Implementacao da classe CLevGeoEntity
//..........................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//..........................................................................
//	Observacoes:
//
//==========================================================================


#include "levgeoentity.h"
#include "levdef.h"


//--------------------------------------------------------------------------
//	The constructor
//--------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//..........................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//--------------------------------------------------------------------------
CLevGeoEntity::CLevGeoEntity()
{
	status			= LEV_VALID;
	id				= -1;
	fixedEntity		= false;

	geoEntityType	= GEOUNDEFINED;

	materialIndex		= UNDEFINED;
	sourceIndex			= UNDEFINED;
	boundConditionIndex	= UNDEFINED;

	isOfGeometry		= true;
}


//--------------------------------------------------------------------------
//	The destructor
//--------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//..........................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//--------------------------------------------------------------------------
CLevGeoEntity::~CLevGeoEntity()
{
	vpPoints.clear();
	vpLines.clear();
	vpEllipses.clear();
	vpPrimitives.clear();
}

