//=============================================================================
//    Arquivo:            PcfDevice.cpp
//.............................................................................
//    Descricao:            Implementacao da classe CPcfDevice
//.............................................................................
//    Autores:
//.............................................................................
//    Observacoes:
//
//=============================================================================


#include "pcfdevice.h"


CPcfDevice::CPcfDevice()
{
    centerholeEdit = 0 ;
    pit = 0;

    vRadiusLayer.reserve(10);
    vNSubLayer.reserve(10);

    int i;

    for(i=0;i<10;i++)
        vRadiusLayer.push_back(0.0);

    for(i=0;i<10;i++)
        vNSubLayer.push_back(0);
}


CPcfDevice::~CPcfDevice()
{ }


void CPcfDevice::Set_Pcf_Last_Radius_Layer(double valor)
{
    lastRadiusLayer = valor;
}


void CPcfDevice::Set_Pcf_N_Total_Layers(int ntotallayers)
{
    NtotalLayers = ntotallayers;
}


void CPcfDevice::Set_Pcf_Pit(double valor)
{
    pit =valor;
}


void CPcfDevice::Set_Pcf_Radius_Center_Hole(double valor)
{
    centerholeEdit =valor;
}


#if 10
void CPcfDevice::Set_Pcf_Radius_Layer(double raio, int i)
{
    if(i < 0)
        vRadiusLayer.push_back(raio);
    else if(i<=(signed)vRadiusLayer.size())
        vRadiusLayer[i] = raio;
}


void CPcfDevice::Set_Pcf_N_Layer(int nCamadas, int i)
{
    if(i < 0)
        vNSubLayer.push_back(nCamadas);
    else if(i<=(signed)vNSubLayer.size())
        vNSubLayer[i] = nCamadas;
}


int CPcfDevice::Get_Pcf_N_Layer(int i)
{
    if(i<=(signed)vNSubLayer.size() && i>=0)
        return vNSubLayer[i];
    else
        return -1;
}


double CPcfDevice::Get_Pcf_Radius_Layer(int i)
{
    if(i<=(signed)vRadiusLayer.size() && i>=0)
        return vRadiusLayer[i];
    else
        return -1;
}


#endif


double CPcfDevice::Get_Pcf_Last_Radius_Layer(void)
{
    return lastRadiusLayer;
}
int    CPcfDevice::Get_Pcf_N_Total_Layers(void)
{
    return NtotalLayers;
}

double CPcfDevice::Get_Pcf_Pit(void)
{
    return pit ;
}
double CPcfDevice::Get_Pcf_Radius_Center_Hole(void)
{
    return centerholeEdit ;
}

