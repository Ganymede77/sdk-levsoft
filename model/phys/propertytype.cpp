//=============================================================================
//    Arquivo:            PropertyType.cpp
//.............................................................................
//    Descricao:          Implementacao das classes
//                        - CPropertyType
//                        - CCteProperty
//                        -
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//.............................................................................
//    Observacoes:
//
//=============================================================================

#include "propertytype.h"
#include <stdio.h>

//===================== Class CPropertyType ===================================
//=============================================================================
////////////////////////
//The constructor function
////////////////////////
CPropertyType::CPropertyType()
{ }

////////////////////////
//The destructor function
////////////////////////
CPropertyType::~CPropertyType()
{ }



//===================== Class CCteProperty =======================================================
//===================================================================================================
//
//    Descri��o:
//
//    par�metros:
//
//    retorno:
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Julho/2001
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
//The constructor function
////////////////////////
CCteProperty::CCteProperty()
{

    value = 0.0;
}

////////////////////////
//The destructor function
////////////////////////
CCteProperty::~CCteProperty()
{

}

double CCteProperty::Get_Value(void)
{
    return value;
}

void CCteProperty::Set_Value(double v)
{
    value = v;
}
