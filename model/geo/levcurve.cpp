
//=============================================================================
//    Arquivo:            LevCurve.cpp
//.............................................................................
//    Descricao:            Implementacao da classe CLevCurve
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//.............................................................................
//    Observacoes:
//
//=============================================================================

#include "levcurve.h"
#include <stdio.h>


CLevCurve::CLevCurve()
{
    startAngle = 0;
    endAngle = 0;
    vpPoints.push_back(NULL);
    vpPoints.push_back(NULL);
}


CLevCurve::~CLevCurve()
{ }

