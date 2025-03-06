//=============================================================================
//    Arquivo:            ArrowDevice.cpp
//.............................................................................
//    Descricao:            Implementacao da classe CArrowDevice
//.............................................................................
//    Autores:
//.............................................................................
//    Observacoes:
//
//=============================================================================


#include "arrowdevice.h"


CArrowDevice::CArrowDevice()
{ }


CArrowDevice::~CArrowDevice()
{ }


void CArrowDevice::Set_Arrow_H(double valor, int i)
{
    if (i <0) vH.push_back(valor);
    else
    {
        vH[i] = valor;
    }
}


void CArrowDevice::Set_Arrow_W(double valor, int i)
{
    if (i <0) vW.push_back(valor);
    else
    {
        vW[i] = valor;
    }
}

double CArrowDevice::Get_Arrow_W(int i)
{
    return vW[i];
}

double CArrowDevice::Get_Arrow_H(int i)
{
    return vH[i];
}

