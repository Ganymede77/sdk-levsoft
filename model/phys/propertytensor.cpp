//=============================================================================
//	Arquivo:			PropertyTensor.cpp
//.............................................................................
//	Descricao:			Implementacao das classes
//						- CPropertyTensor 
//						- CCtePropertyTensor
//						- 
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//.............................................................................
//	Observacoes:
//
//=============================================================================

#include "propertytensor.h"
#include <stdio.h>

//===================== Class CPropertyType =======================================================
//===================================================================================================
////////////////////////
//The constructor function
////////////////////////
CPropertyTensor::CPropertyTensor()
{

}

////////////////////////
//The destructor function
////////////////////////
CPropertyTensor::~CPropertyTensor()
{

}



//===================== Class CCtePropertyTensor =======================================================
//===================================================================================================
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
//	Data:			Julho/2001
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
//The constructor function
////////////////////////
CCtePropertyTensor::CCtePropertyTensor()
{

	
}

////////////////////////
//The destructor function
////////////////////////
CCtePropertyTensor::~CCtePropertyTensor()
{

}

double CCtePropertyTensor::Get_Value(void)
{
	return property.Get_Value();
}

void CCtePropertyTensor::Set_Value(double v)
{
	property.Set_Value(v);
}
