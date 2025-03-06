//=============================================================================
//    Arquivo:            ModulatorDevice.cpp
//.............................................................................
//    Descricao:            Implementacao da classe CArrowDevice
//.............................................................................
//    Autores:
//.............................................................................
//    Observacoes:
//
//=============================================================================


#include "modulatordevice.h"


CModulatorDevice::CModulatorDevice()
{ }


CModulatorDevice::~CModulatorDevice()
{ }


#if 10
void CModulatorDevice::Set_Modulator_B(double valor)
{
    b=valor;
}
void CModulatorDevice::Set_Modulator_E(double valor)
{
    e=valor;
}
void CModulatorDevice::Set_Modulator_H(double valor)
{
    h=valor;
}
void CModulatorDevice::Set_Modulator_L(double valor)
{
    l=valor;
}

void CModulatorDevice::Set_Modulator_S1(double valor)
{
    s1=valor;
}
void CModulatorDevice::Set_Modulator_S2(double valor)
{
    s2=valor;
}
void CModulatorDevice::Set_Modulator_W1(double valor)
{
    w1=valor;
}

void CModulatorDevice::Set_Modulator_W2(double valor)
{
    w2=valor;
}
void CModulatorDevice::Set_Modulator_Wc(double valor)
{
    wc=valor;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


double CModulatorDevice::Get_Modulator_B(void)
{
    return b;
}
double CModulatorDevice::Get_Modulator_E(void)
{
    return e;
}
double CModulatorDevice::Get_Modulator_H(void)
{
    return h;
}
double CModulatorDevice::Get_Modulator_L(void)
{
    return l;
}
double CModulatorDevice::Get_Modulator_S1(void)
{
    return s1;
}
double CModulatorDevice::Get_Modulator_S2(void)
{
    return s2;
}
double CModulatorDevice::Get_Modulator_W1(void)
{
    return w1;
}
double CModulatorDevice::Get_Modulator_W2(void)
{
    return w2;
}
double CModulatorDevice::Get_Modulator_Wc(void)
{
    return wc;
}
#endif
