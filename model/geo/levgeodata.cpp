//===================================================================================================
//    Arquivo:            LevGeoData.h
//...................................................................................................
//    Descrição:            Implementação da classe CLevGeoData
//...................................................................................................
//    Autores:            Nancy Mieko Abe
//                        Angelo Passaro
//...................................................................................................
//    Observações:
//
//===================================================================================================
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "levgeodata.h"
#include "levdef.h"
#include "levtypedef.h"
#include "levregion.h"
#include "aresta_geom.hpp"
#include "elemento_2d.hpp"
#include "subregion.h"
#include "levenum.h"
#include <iostream>

// inicialização do dado membro estático
CLevGeoDataHistoryKeeper * CLevGeoData::historykeeper = NULL;

CLevGeoData::CLevGeoData() : project(nullptr)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    Reinitialize();
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

CLevGeoData::~CLevGeoData()
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    FreeMemory();
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

CLevGeoData::CLevGeoData(const CLevGeoData &other)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    // Copia os atributos primitivos
    numValidPrimitives = other.numValidPrimitives;
    numValidPoints = other.numValidPoints;
    numValidEdges = other.numValidEdges;
    isGeometryModified = other.isGeometryModified;
    original = other.original;
    meshfronteiraOk = other.meshfronteiraOk;
    modelregionsOk = other.modelregionsOk;
    modelprimitivesOk = other.modelprimitivesOk;

    // Copia os objetos complexos (vetores de ponteiros precisam ser copiados corretamente)
    project = other.project;
    vpObservers = other.vpObservers;

    // Copia profunda dos vetores de objetos
    vpPrimitives.clear();
    for (const auto& prim : other.vpPrimitives)
    {
        vpPrimitives.push_back(prim->Clone()); // polimórfico
    }

    vpPoints.clear();
    for (const auto& point : other.vpPoints)
    {
        vpPoints.push_back(new CLevPoint(*point));
    }

    vpInvisiblePoints.clear();
    for (const auto& point : other.vpInvisiblePoints)
    {
        vpInvisiblePoints.push_back(new CLevPoint(*point));
    }

    vpModelRegions.clear();
    for (const auto& region : other.vpModelRegions)
    {
        vpModelRegions.push_back(new CLevRegion(*region));
    }
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

// Operador de atribuição
CLevGeoData& CLevGeoData::operator=(const CLevGeoData &other)
{
    if (this != &other) // Evitar auto-atribuição
    {
        // Libera memória alocada previamente
        for (auto& prim : vpPrimitives) delete prim;
        vpPrimitives.clear();

        for (auto& point : vpPoints) delete point;
        vpPoints.clear();

        for (auto& point : vpInvisiblePoints) delete point;
        vpInvisiblePoints.clear();

        for (auto& region : vpModelRegions) delete region;
        vpModelRegions.clear();

        // Copia os atributos primitivos
        numValidPrimitives = other.numValidPrimitives;
        numValidPoints = other.numValidPoints;
        numValidEdges = other.numValidEdges;
        isGeometryModified = other.isGeometryModified;
        original = other.original;
        meshfronteiraOk = other.meshfronteiraOk;
        modelregionsOk = other.modelregionsOk;
        modelprimitivesOk = other.modelprimitivesOk;

        // Copia os ponteiros (cópia rasa)
        project = other.project;
        vpObservers = other.vpObservers;

        // Copia profunda dos vetores de objetos
        for (const auto& prim : other.vpPrimitives)
        {
        	vpPrimitives.push_back(prim->Clone()); // polimórfico
        }

        for (const auto& point : other.vpPoints)
        {
            vpPoints.push_back(new CLevPoint(*point));
        }

        for (const auto& point : other.vpInvisiblePoints)
        {
            vpInvisiblePoints.push_back(new CLevPoint(*point));
        }

        for (const auto& region : other.vpModelRegions)
        {
            vpModelRegions.push_back(new CLevRegion(*region));
        }
    }
    return *this;
}

//-----------------------------------------------------------------------------------------------------
//    FreeMemory()
//-----------------------------------------------------------------------------------------------------
//    Descrição:     Libera memória
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::FreeMemory(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    // project = NULL;

    unsigned long i;

    vArrowDevice.clear();
    vModulatorDevice.clear();
    vPcfDevice.clear();
    vGroups.clear();
    vEdges.clear();
    vElementos_fronteira.clear();
    vPontos_fronteira.clear();
    vArestasGeometria_fronteira.clear();

    for(i=0; i<vpModelRegions.size(); i++)
        if(vpModelRegions[i]) {
            delete vpModelRegions[i]; vpModelRegions[i] = NULL;
        }
    vpModelRegions.clear();

    for(i=0; i<vpPoints.size(); i++)
        if(vpPoints[i]) {
            delete vpPoints[i]; vpPoints[i] = NULL;
        }
    vpPoints.clear();

    for(i=0; i<vpPrimitives.size(); i++)
        if(vpPrimitives[i]) {
            delete vpPrimitives[i]; vpPrimitives[i] = NULL;
        }
    vpPrimitives.clear();

    vpObservers.clear();

    for(i=0; i<vpInvisiblePoints.size(); i++)
        if(vpInvisiblePoints[i]) {
            delete vpInvisiblePoints[i]; vpInvisiblePoints[i] = NULL;
        }
    vpInvisiblePoints.clear();
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

void CLevGeoData::CleanRegionsVector(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    unsigned long i;
    vpCLevRegion vpRegionsAux = vpModelRegions;

    vpModelRegions.clear();

    for(i=0;i<vpRegionsAux.size();i++)
        if(vpRegionsAux[i]->Status() == LEV_VALID)
            vpModelRegions.push_back(vpRegionsAux[i]);
        else if(vpRegionsAux[i]) {
            delete vpRegionsAux[i]; vpRegionsAux[i] = NULL;
        }
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

//-----------------------------------------------------------------------------------------------------
//    DeleteGroup()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Deleta primitivas que compõem o grupo da lista de primitivas
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Abril/2002
//    Modificado por: Nancy
//    Modificado em:  Novembro/2004
//    Motivo:            Atualização para a nova estrutura de dados
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::DeleteGroup(int pos)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    unsigned long i;
    int position;

    if (pos < 0 || pos >= vGroups.size())
    	return; // Proteção contra acesso inválido

    vGroups[pos].Status(LEV_INVALID);

    for(i=0; i<vGroups[pos].vpPrimitives.size(); i++)
        if(vGroups[pos].vpPrimitives[i]->Status())
        {
            position = GetPosition(vGroups[pos].vpPrimitives[i]);
            DeletePrimitive(position);
        }

    IsGeometryModified(lev_enum::YES);
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

//-----------------------------------------------------------------------------------------------------
//    Ungroup()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Desagrupa um grupo tornando-o inválido
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Abril/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::Ungroup(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vGroups[pos].Status(LEV_INVALID);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    IncludeGroup()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Abril/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::Include(CLevGroup &group)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int num;
    vGroups.push_back(group);
    vGroups.back().Status(LEV_GROUP);
    num = vGroups.size();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return num-1;
}

int CLevGeoData::IncludePcfDevice(CPcfDevice &pcf)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1,p2,p3, p4;
    CLevLine line;
    int pos,i,j;
    vpCLevPoint vpPoint;
    vpPoint.reserve(2);
    double deltax,deltay, centerx, centery, centerx1, centery1;
    double resto;
    int camadaAtual;
    deltax = sin(-10.47197551196597742)* pcf.Get_Pcf_Pit() ;
    deltay = 0.5 * pcf.Get_Pcf_Pit();

    IsGeometryModified(lev_enum::YES);

    double xaxis, yaxis;

    camadaAtual = 0;

    if(pcf.Get_Pcf_Radius_Center_Hole()) //*NMA*: verificação colocada em 04/08/2003
    {
        pos = Include(0.0,0.0,pcf.Get_Pcf_Radius_Center_Hole(),pcf.Get_Pcf_Radius_Center_Hole(),0.0,90.0);
        pcf.IncludeNextPrimitive(GetPrimitive(pos));
    }

    int p;
    for(p=0;p<pcf.Get_Pcf_N_Total_Layers();p++)
    {
        if(camadaAtual<pcf.Get_Pcf_N_Layer(0))
        {
            xaxis = 0.3;
            yaxis = 0.3;
        }
        else
        {
            if( pcf.Get_Pcf_N_Layer(0) <= camadaAtual && camadaAtual < pcf.Get_Pcf_N_Layer(0)+pcf.Get_Pcf_N_Layer(1))
            {
                xaxis = 0.6;
                yaxis = 0.4;
            }
            else
            {
                xaxis = 0.4;
                yaxis = 0.6;
            }

        }
        if (pcf.Get_Pcf_N_Layer(p)!= 0)
        {
            for(i=1;i<=pcf.Get_Pcf_N_Layer(p);i++)
            {
                pos = Include((camadaAtual+i)*deltax,(camadaAtual+i)*deltay,xaxis,yaxis,0.0,0.0);
                pcf.IncludeNextPrimitive(GetPrimitive(pos));

                for(j=1;j<=(camadaAtual+i);j++)
                {
                    centerx = ((camadaAtual+i)*deltax) - (j* (deltax));
                    centery = (camadaAtual+i)*deltay + j* deltay ;

                    if(centerx == 0.0)
                    {
                        pos = Include(centerx,centery,xaxis,yaxis,270.0,90.0);
                        pcf.IncludeNextPrimitive(GetPrimitive(pos));
                    }
                    else
                    {
                        pos = Include(centerx,centery,xaxis,yaxis,0.0,0.0);
                        pcf.IncludeNextPrimitive(GetPrimitive(pos));
                    }
                    centerx1 = ((camadaAtual+i)*deltax);
                    centery1 = ((camadaAtual+i)*deltay)- ((2*j)* deltay);

                    resto = (camadaAtual+i)%2;
                    if (centery1 == 0.0 && resto==0.0)
                    {
                        pos = Include(centerx1,centery1,xaxis,yaxis,0.0,180.0);
                        pcf.IncludeNextPrimitive(GetPrimitive(pos));
                    }
                    if(centery1 != 0.0 && centery1> 0.0)
                    {
                        pos = Include(centerx1,centery1,xaxis,yaxis,0.0,0.0);
                        pcf.IncludeNextPrimitive(GetPrimitive(pos));
                    }
                }
            }
        }
        camadaAtual = camadaAtual + pcf.Get_Pcf_N_Layer(p);
    }

    double y1, x2, y2, y3;
    p1.X(0.0);
    y1 = (camadaAtual*2*deltay)+ pcf.Get_Pcf_Last_Radius_Layer()+0.6;
    p1.Y(y1);
    x2 = (camadaAtual*deltax);
    p2.X(x2 );
    y2 = (camadaAtual*deltay)+pcf.Get_Pcf_Last_Radius_Layer()+0.6;
    p2.Y(y2);

    double m;

    p3.X((camadaAtual*deltax)+pcf.Get_Pcf_Last_Radius_Layer()+0.1);

    m = (p2.Y()-p1.Y() )/( p2.X()- p1.X());
    y3 = p2.Y() + m*( p3.X()-p2.X() );
    p3.Y(y3);

    pos = Include(p3,p1);
    pcf.IncludeNextPrimitive(GetPrimitive(pos));

    // linha horizontal em y = 0
    CLevPoint p5,p6;
    p5.X(0.0);
    p5.Y(0.0);
    p6.X((camadaAtual*deltax) + pcf.Get_Pcf_Last_Radius_Layer() + 0.1);
    p6.Y(0.0);

    pos = Include(p5,p6);
    pcf.IncludeNextPrimitive(GetPrimitive(pos));

    // linha vertical em x = 0
    p1.X(0.0);
    p1.Y(0.0);
    p2.X(0.0);
    p2.Y((camadaAtual*2*deltay)+pcf.Get_Pcf_Last_Radius_Layer()+0.15);

    pos = Include(p1,p2);
    pcf.IncludeNextPrimitive(GetPrimitive(pos));

    // linha vertical em x max
    p1.X((camadaAtual*deltax)+pcf.Get_Pcf_Last_Radius_Layer()+0.4);
    p1.Y(0.0);

    pos = Include(p1,p3);
    pcf.IncludeNextPrimitive(GetPrimitive(pos));

    // inclusao das primitivas no grupo (CPcfDevice)

    vPcfDevice.push_back(pcf);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

    return 0;
}


int CLevGeoData::IncludeModulatorDevice(CModulatorDevice &modulator)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1,p2,p3;
    int pos;

    IsGeometryModified(lev_enum::YES);

    p0.X(0.0);
    p0.Y(0.0);

    p1.X(modulator.Get_Modulator_L());
    p1.Y(0.0);

    p2.X(modulator.Get_Modulator_L());
    p2.Y(modulator.Get_Modulator_B()+modulator.Get_Modulator_H());

    p3.X(0.0);
    p3.Y(p2.Y());

    pos = Include(p0,p1);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p1,p2);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p2,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p0,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    p0.X(0.0);
    p0.Y(modulator.Get_Modulator_H());

    p1.X(modulator.Get_Modulator_L());
    p1.Y(modulator.Get_Modulator_H());

    pos = Include(p0,p1);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    p0.X((modulator.Get_Modulator_L() - (modulator.Get_Modulator_W1()+modulator.Get_Modulator_W2()+modulator.Get_Modulator_S1()+modulator.Get_Modulator_S2()+modulator.Get_Modulator_Wc()))/2);
    p0.Y(modulator.Get_Modulator_H()+modulator.Get_Modulator_B());

    p1.X(p0.X()+modulator.Get_Modulator_W1());
    p1.Y(p0.Y());

    p2.X(p1.X());
    p2.Y(p1.Y()+modulator.Get_Modulator_E());

    p3.X(p0.X());
    p3.Y(p2.Y());

    pos = Include(p1,p2);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p2,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p0,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    p0.X(((modulator.Get_Modulator_L() - (modulator.Get_Modulator_W1()+modulator.Get_Modulator_W2()+modulator.Get_Modulator_S1()+modulator.Get_Modulator_S2()+modulator.Get_Modulator_Wc()))/2)+modulator.Get_Modulator_W1()+modulator.Get_Modulator_S1());
    p0.Y(modulator.Get_Modulator_H()+modulator.Get_Modulator_B());

    p1.X(p0.X()+modulator.Get_Modulator_Wc());
    p1.Y(p0.Y());

    p2.X(p1.X());
    p2.Y(p1.Y()+modulator.Get_Modulator_E());

    p3.X(p0.X());
    p3.Y(p2.Y());

    pos = Include(p1,p2);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p2,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p0,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    p0.X(((modulator.Get_Modulator_L() - (modulator.Get_Modulator_W1()+modulator.Get_Modulator_W2()+modulator.Get_Modulator_S1()+modulator.Get_Modulator_S2()+modulator.Get_Modulator_Wc()))/2) +modulator.Get_Modulator_W1()+modulator.Get_Modulator_S1()+ modulator.Get_Modulator_Wc()+modulator.Get_Modulator_S2());
    p0.Y(modulator.Get_Modulator_H()+modulator.Get_Modulator_B());

    p1.X(p0.X()+modulator.Get_Modulator_W2());
    p1.Y(p0.Y());

    p2.X(p1.X());
    p2.Y(p1.Y()+modulator.Get_Modulator_E());

    p3.X(p0.X());
    p3.Y(p2.Y());

    pos = Include(p1,p2);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p2,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p0,p3);
    modulator.IncludeNextPrimitive(GetPrimitive(pos));

    vModulatorDevice.push_back(modulator);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

	return 0;
}


int CLevGeoData::IncludeArrowDevice(CArrowDevice &arrow)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1,p2,p3;
    int pos,j;
    pos= 0;
    double  paux,paux1;
    paux = paux1 = 0.0;

    p0.X(0.0);
    p0.Y(0.0);

    p1.X(arrow.Get_Arrow_W(0));
    p1.Y(0.0);

    p2.X(arrow.Get_Arrow_W(0));
    p2.Y(arrow.Get_Arrow_H(0));

    p3.X(0.0);
    p3.Y(arrow.Get_Arrow_H(0));

    pos = Include(p0,p1);
    arrow.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p1,p2);
    arrow.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p2,p3);
    arrow.IncludeNextPrimitive(GetPrimitive(pos));

    pos = Include(p0,p3);
    arrow.IncludeNextPrimitive(GetPrimitive(pos));

    for(unsigned int i=1;i< arrow.vW.size();i++)
    {
        p0.X((arrow.Get_Arrow_W(0)-arrow.Get_Arrow_W(i))/2);
        for(j=i ;j>=1;j--)
        {
            paux1 = arrow.Get_Arrow_H(j-1);
            paux = paux + paux1 ;
        }
        p0.Y(paux);

        paux1=0.0;
        paux=0.0;

        p1.X(p0.X()+arrow.Get_Arrow_W(i));
        p1.Y(p0.Y());

        p2.X(p1.X());
        p2.Y(p1.Y()+arrow.Get_Arrow_H(i));

        p3.X(p0.X());
        p3.Y(p2.Y());

        pos = Include(p0,p1);
        arrow.IncludeNextPrimitive(GetPrimitive(pos));

        pos = Include(p1,p2);
        arrow.IncludeNextPrimitive(GetPrimitive(pos));

        pos = Include(p2,p3);
        arrow.IncludeNextPrimitive(GetPrimitive(pos));

        pos = Include(p0,p3);
        arrow.IncludeNextPrimitive(GetPrimitive(pos));
    }

    vArrowDevice.push_back(arrow);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

    return 0; ///rever
}


#if 10
//-----------------------------------------------------------------------------------------------------
//    InterssectedLine()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Quebra uma linha em outras duas, no ponto de cruzamento dado.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Angelo    *AP*
//    Modificado em:  19/07/2000
//    Motivo:         atualização dos dados membros de crossp (em particular numlines)
//                  para a função que fez a chamada.
//    Modificado por: Nancy    *NMA*
//    Modificado em:  04/04/2002
//    Motivo:            Inclusão do parâmetro default verifyCrosses,
//                    se true: verifica se existem cruzamentos destas novas linhas
//                    se false: inclui sem verificação de cruzamentos destas novas linhas
//  Modificado por: Nancy    *NMA*
//  Modificado em:  16/05/2002
//  Motivo:            Inclusão de teste de status da linha. A linha pode ter sido dividida
//                    no processo de divisão da linha que a intercepta ( lembrando que
//                    LocateLineCrosses() é chamada quando da inclusão linhas)
//  Modificado em:  21/11/2004
//  Motivo:            Reengenharia
//                    - substituição de vPoints por vpPoints
//                    - substituição de IncludePoint por Include
//                    - substituição de vLines por vpPrimitives
//                    - substituição de Includes
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::InterssectedLine(int position, CLevPoint &crossp, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p1, p2, *pc = NULL;
    CLevLine newline;
    int retorno = -1;
    int pointpos;

    if (position < 0 || position >= vpPrimitives.size())
    	return 0;

    // Verifica o status da linha, pois ela já pode ter sido dividida no processo
    // de divisão da linha que a intercepta
    if(!vpPrimitives[position]->Status() || vpPrimitives[position]->GeoEntityType() != LINELEV)
        return 0;

    if (IsAPoint(crossp,pointpos, 1e-10, 1e-10)) pc = vpPoints[pointpos];
    else
    {
        pc = CLevGeoData::Include(crossp);
        IsAPoint(crossp,pointpos, 0, 0);
    }

    if (pc == vpPrimitives[position]->vpPoints[0] || pc == vpPrimitives[position]->vpPoints[1])
    {
        // neste caso é a mesma reta e nada será modificado
        retorno = 0;
    }
    else
    {
        DeletePrimitive(position);
        p1 = *vpPrimitives[position]->vpPoints[0];
        p2 = *vpPrimitives[position]->vpPoints[1];

        if(verifyCrosses) // Verifica outros cruzamentos em IncludeLine
        {
            Include(p1,*vpPoints[pointpos]);
            Include(p2,*vpPoints[pointpos]);
        }
        else // Não verifica outros cruzamentos em IncludeLine, esta sobrecarga não chama LocateLineCrosses
        {
            int pos;

            pos = Include(p1,*vpPoints[pointpos], false);
            pos = Include(p2,*vpPoints[pointpos], false);

        }
        crossp = *vpPoints[pointpos];  //*AP* atualiza crossp para função que fez a chamada
        retorno = 1;
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return retorno;
}


//-----------------------------------------------------------------------------------------------------
//    InterssectedLine() - Sobrecarga 1
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Quebra uma linha em outras três, nos pontos de cruzamento dados.
//
//    parâmetros:    int position        : posição da linha no vetor
//                vCLevPoint &crossp    : vetor de pontos de cruzamento
//                bool verifyCrosses    : variável que determina se é necessário verificar cruzamtnos das
//                                        novas retas criadas.
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy    *NMA*
//    Modificado em:  04/04/2002
//    Motivo:            Criação de sobrecarga para considerar dois pontos de cruzamento
//  Modificado por: Nancy    *NMA*
//  Modificado em:  16/05/2002
//  Motivo:            Inclusão de teste de status da linha. A linha pode ter sido dividida
//                    no processo de divisão da linha que a intercepta ( lembrando que
//                    LocateLineCrosses() é chamada quando da inclusão linhas)
//  Modificado em:  21/11/2004
//  Motivo:            Reengenharia
//                    - substituição de vPoints por vpPoints
//                    - substituição de IncludePoint por Include
//                    - substituição de vLines por vpPrimitives
//                    - substituição de Includes
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::InterssectedLine(int position, vCLevPoint &crossp, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint    p1, p2, *pcr = NULL, pcross1, pcross2;
    CLevLine    newline;
    int            retorno = -1;
    int            pointpos1, pointpos2;
    double        dt0, dt1, xv0,yv0;

    // Verifica o status da linha, pois ela já pode ter sido dividida no processo
    // de divisão da linha que a intercepta
    if(!vpPrimitives[position]->Status() || vpPrimitives[position]->GeoEntityType() != LINELEV)
        return 0;

    // Considera sempre dois pontos de cruzamento

    {
        if ( (*vpPrimitives[position]->vpPoints[0] == crossp[0] && *vpPrimitives[position]->vpPoints[1] == crossp[1] ) ||
             (*vpPrimitives[position]->vpPoints[1] == crossp[0] && *vpPrimitives[position]->vpPoints[0] == crossp[1] ) )
        {
            // neste caso é a mesma reta e nada será modificado
            retorno = 0;
        }
        else
        {
            xv0 = vpPrimitives[position]->vpPoints[0]->X();
            yv0 = vpPrimitives[position]->vpPoints[0]->Y();

            // calcula distância do vertice 0 aos dois pontos de cruzamento
            // os dois pontos de cruzamento estão sempre dentro dos limites da reta
            dt0 = sqrt( (xv0-crossp[0].X())*(xv0-crossp[0].X()) + (yv0-crossp[0].Y()) *(yv0-crossp[0].Y()));
            dt1 = sqrt( (xv0-crossp[1].X())*(xv0-crossp[1].X()) + (yv0-crossp[1].Y()) *(yv0-crossp[1].Y()));

            if(dt0==0)
            	retorno = InterssectedLine(position, crossp[1], verifyCrosses);
            else if(dt1==0)
            	retorno = InterssectedLine(position, crossp[0], verifyCrosses);
            else
            {
                DeletePrimitive(position);
                p1 = *vpPrimitives[position]->vpPoints[0];
                p2 = *vpPrimitives[position]->vpPoints[1];

                if (IsAPoint(crossp[0],pointpos1, 1e-10, 1e-10)) pcr = vpPoints[pointpos1];
                else
                {
                	pcr = CLevGeoData::Include(crossp[0]);IsAPoint(crossp[0],pointpos1, 0, 0);
                }

                if (IsAPoint(crossp[1],pointpos2, 1e-10, 1e-10)) pcr = vpPoints[pointpos2];
                else
                {
                	pcr = CLevGeoData::Include(crossp[1]);IsAPoint(crossp[1],pointpos2, 0, 0);
                }

                if(dt0 < dt1)
                {
                    pcross1 = *vpPoints[pointpos1];
                    pcross2 = *vpPoints[pointpos2];
                }
                else
                {
                    pcross1 = *vpPoints[pointpos2];
                    pcross2 = *vpPoints[pointpos1];
                }

                if(verifyCrosses) // Verifica outros cruzamentos em IncludeLine
                {
                    Include(p1,pcross1);
                    Include(pcross1,pcross2);
                    Include(p2,pcross2);
                }
                else // Não verifica outros cruzamentos em IncludeLine, esta sobrecarga não chama LocateLineCrosses
                {
                    Include(p1,pcross1,false);
                    Include(pcross1,pcross2,false);
                    Include(p2,pcross2,false);

                }
                retorno = 2;
            }
        }
    }

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

	return retorno;
}


//-----------------------------------------------------------------------------------------------------
//    InterssectedEllipse()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Quebra uma elipse em outras duas, no ponto de cruzamento dado.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril/2002
//
//  Modificado por: Nancy    *NMA*
//  Modificado em:  16/05/2002
//  Motivo:            Inclusão de teste de status da elipse. A elipse pode ter sido dividida
//                    no processo de divisão da linha que a intercepta ( lembrando que
//                    LocateCrosses() é chamada quando da inclusão linhas)
//  Modificado em:  21/11/2004
//  Motivo:            Reengenharia
//                    - substituição de vPoints por vpPoints
//                    - substituição de IncludePoint por Include
//                    - substituição de vLines por vpPrimitives
//                    - substituição de Includes
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::InterssectedEllipse(int position, CLevPoint &crossp, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint        p1, p2,pcentro, pxAxis, pyAxis, *pcr = NULL;
    int                retorno = -1;
    int                pointpos;

    if(!vpPrimitives[position]->Status() || vpPrimitives[position]->GeoEntityType() != ELLIPSELEV)
        return 0;

    if (IsAPoint(crossp,pointpos, 1e-10, 1e-10)) pcr = vpPoints[pointpos];
    else { pcr = CLevGeoData::Include(crossp);IsAPoint(crossp,pointpos, 0, 0);}

    CLevPoint pe1;
    CLevPoint pe2;
    pe1 = *vpPrimitives[position]->vpPoints[0];
    pe2 = *vpPrimitives[position]->vpPoints[1];

    if (pcr == vpPrimitives[position]->vpPoints[0] || pcr == vpPrimitives[position]->vpPoints[1])
    {
        // neste caso é a mesma elipse e nada será modificado
        retorno = 0;
    }
    else
    {
        DeletePrimitive(position);
        p1        = *vpPrimitives[position]->vpPoints[0];
        p2        = *vpPrimitives[position]->vpPoints[1];
        pcentro = *vpPrimitives[position]->vpPoints[2];
        pxAxis    = *vpPrimitives[position]->vpPoints[3];
        pyAxis    = *vpPrimitives[position]->vpPoints[4];

        if(p1.X()==p2.X() && p1.Y()==p2.Y() && p1.num_primitives ==0)
        {
            CLevEllipse        newellipse1(pcr,pcr,&pcentro,&pxAxis,&pyAxis);

            Include(newellipse1, verifyCrosses);
        }
        else
        {
            CLevEllipse        newellipse1(&p1,pcr,&pcentro,&pxAxis,&pyAxis);
            CLevEllipse        newellipse2(pcr,&p2,&pcentro,&pxAxis,&pyAxis);

            Include(newellipse1, verifyCrosses);
            Include(newellipse2, verifyCrosses);
        }
        retorno = 1;
        crossp = *vpPoints[pointpos];
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return retorno;
} // end of InterssectedEllipse


//-----------------------------------------------------------------------------------------------------
//    InterssectedEllipse() - Sobrecarga 1
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Quebra uma elipse em outras três, nos pontos de cruzamento dados.
//
//    parâmetros:    int position        : posição da linha no vetor
//                vCLevPoint &crossp    : vetor de pontos de cruzamento
//                bool verifyCrosses    : variável que determina se é necessário verificar cruzamtnos das
//                                        novas linhas criadas.
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril/2002
//    Modificado por: Nancy
//  Modificado em:  21/11/2004
//  Motivo:            Reengenharia
//                    - substituição de vPoints por vpPoints
//                    - substituição de IncludePoint por Include
//                    - substituição de vLines por vpPrimitives
//                    - substituição de Includes
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::InterssectedEllipse(int position, vCLevPoint &crossp, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint    p1, p2, *pcr1 = NULL,*pcr2 = NULL, pcross1, pcross2, *pcri = NULL,*pcrf = NULL;
    CLevPoint    pcentro, pxAxis, pyAxis;
    CLevLine    newline;
    int            retorno = -1;
    int            pointpos1, pointpos2;
    int            elpos1, elpos2,elpos3 ;
    double        dt0, dt1, xv0,yv0;
    bool        continua = true, divideInThree = false;

    if(!vpPrimitives[position]->Status() || vpPrimitives[position]->GeoEntityType() != ELLIPSELEV)
        return 0;

    // Considera sempre dois pontos de cruzamento
    CLevPoint ppCross0, ppCross1, ppEl0, ppEl1;

    ppCross0 = crossp[0];
    ppCross1 = crossp[1];
    ppEl0     = *vpPrimitives[position]->vpPoints[0] ;
    ppEl1     = *vpPrimitives[position]->vpPoints[1] ;

    //verifica se os pontos de cruzamento são os pontos vértices da elipse
    if( (*vpPrimitives[position]->vpPoints[0] == crossp[0] && *vpPrimitives[position]->vpPoints[1] == crossp[1] ) ||
        (*vpPrimitives[position]->vpPoints[1] == crossp[0] && *vpPrimitives[position]->vpPoints[0] == crossp[1] ) )
    {
		// os dois pontos de interseção são os próprios vértices da elipse,
		// não é necessário quebrá-la
		retorno = 0;
		continua = false;
    }

    if(continua)
    {
        //verifica se um dos vértice coincide com um dos pontos de cruzamento

        if(ppCross0 == ppEl0 || ppCross0 == ppEl1)
        {
            retorno = InterssectedEllipse(position, crossp[1], verifyCrosses);
            continua = false;
        }
        else
        {
            if(ppCross1 == ppEl0 || ppCross1 == ppEl1)
            {
                retorno = InterssectedEllipse(position, crossp[0], verifyCrosses);
                continua = false;
            }
        }
    }

    if(continua)
    {
        DeletePrimitive(position);
        p1 = *vpPrimitives[position]->vpPoints[0];
        p2 = *vpPrimitives[position]->vpPoints[1];
        pcentro = *vpPrimitives[position]->vpPoints[2];
        pxAxis    = *vpPrimitives[position]->vpPoints[3];
        pyAxis    = *vpPrimitives[position]->vpPoints[4];

        if (IsAPoint(crossp[0],pointpos1, 1e-10, 1e-10)) pcr1 = vpPoints[pointpos1];
        else { pcr1 = CLevGeoData::Include(crossp[0]);IsAPoint(crossp[0],pointpos1, 0, 0);}

        if (IsAPoint(crossp[1],pointpos2, 1e-10, 1e-10)) pcr2 = vpPoints[pointpos2];
        else { pcr2 = CLevGeoData::Include(crossp[1]);IsAPoint(crossp[1],pointpos2, 0, 0);}

        // calcula distância do vertice 0 aos dois pontos de cruzamento
        // os dois pontos de cruzamento estão sempre dentro dos limites da elipse
        xv0 = p1.X();
        yv0 = p1.Y();

        dt0 = sqrt( (xv0-crossp[0].X())*(xv0-crossp[0].X()) + (yv0-crossp[0].Y()) *(yv0-crossp[0].Y()));
        dt1 = sqrt( (xv0-crossp[1].X())*(xv0-crossp[1].X()) + (yv0-crossp[1].Y()) *(yv0-crossp[1].Y()));

        if(dt0 < dt1)
        {
            pcri = pcr1;
            pcrf = pcr2;
        }
        else
        {
            pcri = pcr2;
            pcrf = pcr1;
        }

        if(p1.X()==p2.X() && p1.Y()==p2.Y())
        {
            if( p1.num_primitives == 0  ||
                (p1.X()==pcri->X() && p1.Y()==pcri->Y())    ||
                (p1.X()==pcrf->X() && p1.Y()==pcrf->Y()))
            {
                CLevEllipse        newellipse1(pcri,pcrf,&pcentro,&pxAxis,&pyAxis);
                CLevEllipse        newellipse2(pcrf,pcri,&pcentro,&pxAxis,&pyAxis);

                elpos1 = Include(newellipse1, false);
                elpos2 = Include(newellipse2, false);
                if(verifyCrosses)
                {
                    LocateGivenEllipseAllLinesCrosses(elpos1);
                    LocateGivenEllipseAllLinesCrosses(elpos2);
                }
            }
            else divideInThree = true;
        }
        else divideInThree = true;

        if(divideInThree)
        {
            CLevEllipse        newellipse1(&p1,pcri,&pcentro,&pxAxis,&pyAxis);
            CLevEllipse        newellipse2(pcri,pcrf,&pcentro,&pxAxis,&pyAxis);
            CLevEllipse        newellipse3(pcrf,&p2,&pcentro,&pxAxis,&pyAxis);

            elpos1 = Include(newellipse1, false);
            elpos2 = Include(newellipse2, false);
            elpos3 = Include(newellipse3, false);

            if(verifyCrosses)
            {
                LocateGivenEllipseAllLinesCrosses(elpos1);
                LocateGivenEllipseAllLinesCrosses(elpos2);
                LocateGivenEllipseAllLinesCrosses(elpos3);
            }
        }

        crossp[0] = *vpPoints[pointpos1];
        crossp[1] = *vpPoints[pointpos2];
        retorno = 2;
    }

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return retorno;
} // end of InterssectedEllipse
#endif

//-----------------------------------------------------------------------------------------------------
//    IsAGroup()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Abril/2000
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IsAGroup(CLevPoint &screenPoint, CLevGroup & group, int &position, double deltax, double deltay)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    CLevPoint pt_minxminy, pt_maxxmaxy;

    for (i = 0; i < vGroups.size(); i++)
    {
        if (vGroups[i].Status())
        {
            vGroups[i].Limits(pt_minxminy,pt_maxxmaxy);

            if ((screenPoint.X() >= pt_minxminy.X()- deltax) && (screenPoint.X() <= pt_maxxmaxy.X()+deltax))
            {
                if ((screenPoint.Y() >= pt_minxminy.Y()- deltay) && (screenPoint.Y() < pt_maxxmaxy.Y()+ deltay))
                {
                    group = vGroups[i];
                    position = i;
                    return 1;
                }
            }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
} // end of IsAGroup

// *NMA*, 22/11/2004
// Reengenharia: Tratamento unificado de primitivas
CLevGroup CLevGeoData::FindPrimitivesInsideRect(CLevPoint &pmin, CLevPoint &pmax)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    CLevPoint o_pMinLimit,o_pMaxLimit;
    CLevGroup tempGroup;

    for(i=0;i<vpPrimitives.size();i++)
    {
        if(vpPrimitives[i] && vpPrimitives[i]->Status())
        {
            vpPrimitives[i]->Limits(o_pMinLimit,o_pMaxLimit);

            if(pmin.X() <= o_pMinLimit.X() && pmax.X() >= o_pMaxLimit.X())
                if(pmin.Y() <= o_pMinLimit.Y() && pmax.Y() >= o_pMaxLimit.Y())
                {
                    //aqui inclui a primitiva
                    tempGroup.IncludeNextPrimitive(vpPrimitives[i]->Clone()); // Cópia segura
                }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return tempGroup;
}

//-----------------------------------------------------------------------------------------------------
//    LocateLineCrosses()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Localiza pontos de intersecção entre uma dada linha e outras linhas existentes.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Angelo  *AP*
//    Modificado em:  06/Julho/2000, 19/07/2000
//    Motivo: -  correção da sequencia de cópia dos pontos referentes a uma reta que foi eliminada
//             quando da identificação de um cruzamento de linha. (06/07)
//          -  Inclusão explicita do ponto de cruzamento para permitir a atualização do mesmo
//                if (IsAPoint(crosspoint,pointpos, 1e-10, 1e-10)) pc = &vPoints[pointpos];
//                else { pc = IncludePoint(crosspoint); IsAPoint(crosspoint,pointpos,0,0); }  *AP* 19/07/2000
//    Modificado por: Nancy  *NMA*
//    Modificado em:  18/Dezembro/2001
//    Motivo:            Utilização de método de CLevLine para identificação dos pontos de intersecção
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::LocateLineCrosses(int insertionposition)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevLine    oline_i, oline_j;
    CLevPoint    crosspoint;
    vCLevPoint    vo_crosspoint;
    lev_math::USINT        codCrossing = 0;
    int            i, j ;
    int            num_primitives;

    // *NMA* reserva e inclui um ponto para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.reserve(1);
    vo_crosspoint.push_back(crosspoint);

    if (insertionposition == -1)
        return;

    i = insertionposition;
    if ( vpPrimitives[i]->Status() != LEV_VALID || vpPrimitives[i]->GeoEntityType() != LINELEV) return;

    oline_i = *(CLevLine*) vpPrimitives[i];
    // locate all possible line-line crosses
    num_primitives = vpPrimitives.size();
    for (j = 0; j < num_primitives; j++)
    {
        vo_crosspoint[0].Reinitialize();

        if ( i!=j )
        {
            //if (vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == LINELEV)
            if (vpPrimitives[i]->Status() && vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == LINELEV)
            {
                oline_j = *(CLevLine*)vpPrimitives[j];

                codCrossing = oline_i.CrossingPoint(oline_j,vo_crosspoint);

                switch(codCrossing)
                {
					case 1: //linhas concorrentes com interseção
					{    CLevPoint p1, p2, p3, p4, *pc = NULL;// pcrosspoint;
						int line1 = 0, line2 = 0;
						int pointpos;
						if (IsAPoint(vo_crosspoint[0],pointpos, 1e-8, 1e-8))
													 pc = vpPoints[pointpos];
						else {
													 pc = CLevGeoData::Include(vo_crosspoint[0]);
													 IsAPoint(vo_crosspoint[0],pointpos,0,0);
												}  //*AP*

						if (pc != vpPrimitives[j]->vpPoints[0] && pc != vpPrimitives[j]->vpPoints[1])
						{
							DeletePrimitive(j); line1 = 1;
							p1 = *vpPrimitives[j]->vpPoints[0];  //cópia feita apos o DeleteLine para
							p2 = *vpPrimitives[j]->vpPoints[1];  // garantir que o numero de linhas ligadas está atualizado
						}
						if (pc != vpPrimitives[i]->vpPoints[0] && pc != vpPrimitives[i]->vpPoints[1])
						{
							DeletePrimitive(i); line2 = 1;
							p3 = *vpPrimitives[i]->vpPoints[0];  //cópia feita apos o DeleteLine para
							p4 = *vpPrimitives[i]->vpPoints[1];  // garantir que o numero de linhas ligadas está atualizado
						}

						if (line1)
						{
							Include(p1,vo_crosspoint[0],false); // *AP* neste caso não é necessário verificar novos cruzamentos
							Include(p2,vo_crosspoint[0],false);
						}
						if (line2)
						{
							Include(p3,vo_crosspoint[0]); // *AP* neste caso é necessário verificar novos cruzamentos
							Include(p4,vo_crosspoint[0]);
						}
					}
						break;

					case 2: // Linhas coincidentes - ocorre sobreposição
						MergeLines(i,j);
						break;

                }
            }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}


//-----------------------------------------------------------------------------------------------------
//    LocateLineCrosses()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void  CLevGeoData::LocateLineCrosses()
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevLine    oline_i, oline_j;
    lev_math::USINT codCrossing = 0;
    unsigned int i, j;
    CLevPoint crosspoint;
    vCLevPoint vo_crosspoint;
    int loop;

    // *NMA* reserva e inclui um ponto para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.reserve(2);
    vo_crosspoint.push_back(crosspoint);
    vo_crosspoint.push_back(crosspoint);

    do
    {
        loop = false;
        // locate all possible line-line crosses
        for (i = 0; i < vpPrimitives.size(); i++)
        {
            vo_crosspoint[0].Reinitialize();

            if (vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
            {
                oline_i = *(CLevLine*) vpPrimitives[i];
                for (j = i+1; j < vpPrimitives.size(); j++)
                {
                    if ( vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
                    {
                        if (vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == LINELEV)
                        {
                            oline_j = *(CLevLine*) vpPrimitives[j];
                            codCrossing = oline_i.CrossingPoint(oline_j,vo_crosspoint);

                            switch(codCrossing)
                            {
								case 1:
									if (InterssectedLine(i, vo_crosspoint[0])) loop = true;
									if (InterssectedLine(j, vo_crosspoint[0])) loop = true;
									break;

								case 2:// Linhas coincidentes - ocorre sobreposição
									MergeLines(i,j);
									break;
                            }
                        }
                    }
                }
            }
        }
    } while (loop);

    vo_crosspoint.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::MergeLines(int i , int j )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p[4], p_ord[4],  *p_aux = NULL;

    //verifica se as duas primitivas são linhas
    if(vpPrimitives[i]->GeoEntityType() != LINELEV && vpPrimitives[j]->GeoEntityType() != LINELEV )
        return;

    DeletePrimitive(i);
    DeletePrimitive(j);

    // - Pode existir uma sobreposição total, ou seja os dois vértices de uma reta podem estar
    //        dentro do limite da outra, ou
    // - Somente um dos pontos de uma reta está dentro dos limites da outra

    // verifica se são linhas paralelas a x
    if(vpPrimitives[i]->vpPoints[0]->Y() == vpPrimitives[i]->vpPoints[1]->Y())
    {
        // ordena pontos da linha j
        if(vpPrimitives[j]->vpPoints[0]->X() < vpPrimitives[j]->vpPoints[1]->X())
        { p[0] = *vpPrimitives[j]->vpPoints[0]; p[1] = *vpPrimitives[j]->vpPoints[1];}
        else
        { p[0] = *vpPrimitives[j]->vpPoints[1]; p[1] = *vpPrimitives[j]->vpPoints[0];}

        // ordena pontos da linha i
        if(vpPrimitives[i]->vpPoints[0]->X() < vpPrimitives[i]->vpPoints[1]->X())
        { p[2] = *vpPrimitives[i]->vpPoints[0]; p[3] = *vpPrimitives[i]->vpPoints[1];}
        else
        { p[2] = *vpPrimitives[i]->vpPoints[1]; p[3] = *vpPrimitives[i]->vpPoints[0];}

        // Ordenação dos pontos considerando que ocorre sobreposição
        if(p[0].X() < p[2].X()) { p_ord[0] = p[0]; p_ord[1] = p[2];}
        else                    { p_ord[0] = p[2]; p_ord[1] = p[0];}

        if(p[1].X() < p[3].X()) { p_ord[2] = p[1]; p_ord[3] = p[3];}
        else                    { p_ord[2] = p[3]; p_ord[3] = p[1];}
    }
    else //linhas inclinadas ou paralelas a y
    {
        // ordena pontos da linha j
        if(vpPrimitives[j]->vpPoints[0]->Y() < vpPrimitives[j]->vpPoints[1]->Y())
        { p[0] = *vpPrimitives[j]->vpPoints[0]; p[1] = *vpPrimitives[j]->vpPoints[1];}
        else
        { p[0] = *vpPrimitives[j]->vpPoints[1]; p[1] = *vpPrimitives[j]->vpPoints[0];}

        // ordena pontos da linha i
        if(vpPrimitives[i]->vpPoints[0]->Y() < vpPrimitives[i]->vpPoints[1]->Y())
        { p[2] = *vpPrimitives[i]->vpPoints[0]; p[3] = *vpPrimitives[i]->vpPoints[1];}
        else
        { p[2] = *vpPrimitives[i]->vpPoints[1]; p[3] = *vpPrimitives[i]->vpPoints[0];}

        // Ordenação dos pontos considerando que ocorre sobreposição
        if(p[0].Y() < p[2].Y()) { p_ord[0] = p[0]; p_ord[1] = p[2];}
        else                    { p_ord[0] = p[2]; p_ord[1] = p[0];}

        if(p[1].Y() < p[3].Y()) { p_ord[2] = p[1]; p_ord[3] = p[3];}
        else                    { p_ord[2] = p[3]; p_ord[3] = p[1];}
    }
    //Inclusão dos pontos
    int pointpos;
    p_aux = CLevGeoData::Include(p_ord[0]); IsAPoint(p_ord[0],pointpos,0,0);
    p_aux = CLevGeoData::Include(p_ord[1]);    IsAPoint(p_ord[1],pointpos,0,0);
    p_aux = CLevGeoData::Include(p_ord[2]);    IsAPoint(p_ord[2],pointpos,0,0);
    p_aux = CLevGeoData::Include(p_ord[3]); IsAPoint(p_ord[3],pointpos,0,0);

    // Inclusão de linhas: é necessário verificar se ocorreram outros cruzamentos
    Include(p_ord[0],p_ord[1]);
    Include(p_ord[1],p_ord[2]);
    Include(p_ord[2],p_ord[3]);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    LocateEllipsesCrosses()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Localiza pontos de intersecção entre uma dada elipse e outras elipses
//                e linhas existentes.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void  CLevGeoData::LocateEllipsesCrosses(int insertionposition)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint    crosspoint;
    vCLevPoint    vo_crosspoint;
    lev_math::USINT        codCrossing = 0;
    int            i;

    // *NMA* reserva e inclui dois pontos para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.push_back(crosspoint);
    vo_crosspoint.push_back(crosspoint);

    if(insertionposition != -1)
    {
        i = insertionposition;
    }

    vo_crosspoint.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    LocateEllipsesCrosses()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Localiza pontos de intersecção entre cada uma das elipses e as outras elipses
//                e linhas existentes.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril/2002
//    Modificado por: Nancy
//    Modificado em:  22/11/2004
//    Motivo:            Reengenharia: tratamento unificado de primitivas
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::LocateEllipsesCrosses(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int i, size;

    size = vpPrimitives.size();
    for (i = 0; i < size; i++)
    {
        if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
        {
            LocateEllipsesCrosses(i);
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    LocateCrosses()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Verifica intersecções entre todas as retas e todas as elipses.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril/2002
//    Modificado por: Nancy
//    Modificado em:  22/11/2004
//    Motivo:            Reengenharia: tratamento unificado de primitivas
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::LocateCrosses(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    lev_math::USINT codCrossing = 0;
    unsigned int i, j;
    CLevPoint crosspoint;
    vCLevPoint vo_crosspoint;
    int loop;
    CLevLine o_line_i;
    CLevEllipse o_ellipse_j;

    // *NMA* reserva e inclui um ponto para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.reserve(2);
    vo_crosspoint.push_back(crosspoint);
    vo_crosspoint.push_back(crosspoint);

    do
    {
        loop = false;
        // locate all possible line-elipse crosses
        for (i = 0; i < vpPrimitives.size(); i++)
        {
            vo_crosspoint[0].Reinitialize();
            vo_crosspoint[1].Reinitialize();

            if(vpPrimitives[i]->GeoEntityType() == LINELEV && vpPrimitives[i]->Status())
            {
                o_line_i = *(CLevLine*)vpPrimitives[i];

                for (j = 0; j < vpPrimitives.size(); j++)
                {
                    if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV && vpPrimitives[i]->Status())
                    {
                        o_ellipse_j = *(CLevEllipse*)vpPrimitives[j];

                        codCrossing = CrossingPoints(o_line_i,o_ellipse_j,vo_crosspoint);

                        switch(codCrossing)
                        {
                            case 1: // um ponto de intersecção entre elipse e reta
                                if(InterssectedEllipse(j,vo_crosspoint[0],false)) loop = true;
                                if(InterssectedLine(i,vo_crosspoint[0],false)) loop = true;
                                break;
                            case 2: // dois pontos de interseção entre elipse e reta
                                if(InterssectedEllipse(j,vo_crosspoint,false))loop = true;
                                if(InterssectedLine(i,vo_crosspoint,false)) loop = true;
                                break;
                        }
                    }
                }

            }
        }
    } while (loop);

    vo_crosspoint.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    LocateGivenLineAllEllipeseCrosses
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Verifica intersecções entre uma reta e todas as elipses.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Junho/2002
//    Modificado por: Nancy
//    Modificado em:  22/11/2004
//    Motivo:            Reengenharia: tratamento unificado de primitivas
//-----------------------------------------------------------------------------------------------------
void  CLevGeoData::LocateGivenLineAllEllipsesCrosses(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    lev_math::USINT codCrossing = 0;
    int  j ;
    CLevPoint crosspoint;
    vCLevPoint vo_crosspoint;
    int loop;
    CLevLine o_line_pos;
    CLevEllipse o_ellipse_j;
    int size;
    int position = -1;
    double deltax, deltay;

    // *NMA* reserva e inclui um ponto para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.reserve(2);
    vo_crosspoint.push_back(crosspoint);
    vo_crosspoint.push_back(crosspoint);

    if(vpPrimitives[pos]->Status() && vpPrimitives[pos]->GeoEntityType() == LINELEV)
    {
        o_line_pos = *(CLevLine *) vpPrimitives[pos];

        size = vpPrimitives.size();

        // locate all possible crosses for a given line - all ellipses
        do {
            loop = false;

            vo_crosspoint[0].Reinitialize();
            vo_crosspoint[1].Reinitialize();

            for (j = 0; j < size; j++)
            {
                //if(vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == ELLIPSELEV)
                if(vpPrimitives[pos]->Status() && vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == ELLIPSELEV)
                {
                    o_ellipse_j = *(CLevEllipse *)vpPrimitives[j];

                    codCrossing = CrossingPoints(o_line_pos,o_ellipse_j,vo_crosspoint);

                    switch(codCrossing)
                    {
                    case 1: // um ponto de intersecção entre elipse e reta
                        Resolution(vo_crosspoint[0], deltax, deltay);
                        if (IsAPoint(vo_crosspoint[0], position,deltax,deltay))
                        {
                            if(position >= 0 && position < (signed)vpPoints.size())
                                vo_crosspoint[0] = *vpPoints[position];
                        }
                        InterssectedEllipse(j,vo_crosspoint[0],false);
                        InterssectedLine(pos,vo_crosspoint[0]); //*NMA* 2015-04-08: eh necessario verificar as interseccoes da nova linha
                        //InterssectedLine(pos,vo_crosspoint[0],false);
                        break;
                    case 2: // dois pontos de interseção entre elipse e reta
                        Resolution(vo_crosspoint[0], deltax, deltay);
                        if (IsAPoint(vo_crosspoint[0], position,deltax,deltay))
                        {
                            if(position >= 0 && position < (signed)vpPoints.size())
                                vo_crosspoint[0] = *vpPoints[position];
                        }

                        Resolution(vo_crosspoint[1], deltax, deltay);
                        if (IsAPoint(vo_crosspoint[1], position,deltax,deltay))
                        {
                            if(position >= 0 && position < (signed)vpPoints.size())
                                vo_crosspoint[1] = *vpPoints[position];
                        }
                        InterssectedEllipse(j,vo_crosspoint,false);
                        InterssectedLine(pos,vo_crosspoint);//*NMA* 2015-04-08: eh necessario verificar as interseccoes da nova linha
                        //InterssectedLine(pos,vo_crosspoint, false);
                        break;
                    }
                }
            }
        } while (loop);
    }
    vo_crosspoint.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    LocateGivenEllipseAllLinesCrosses
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Verifica intersecções entre uma ellipse e todas as retas.
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Junho/2002
//    Modificado por: Nancy
//    Modificado em:  22/11/2004
//    Motivo:            Reengenharia: tratamento unificado de primitivas
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::LocateGivenEllipseAllLinesCrosses(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    lev_math::USINT    codCrossing = 0;
    CLevPoint    crosspoint;
    vCLevPoint    vo_crosspoint;
    CLevLine    o_line_j;
    CLevEllipse o_ellipse_pos;
    int    j;
    int    loop;
    int    size;
    int position = -1;
    double deltax, deltay;

    // *NMA* reserva e inclui um ponto para evitar a necessidade de reinicializar o vetor (clear)
    // dentro do laço, o que teria que ser feito se a inclusão fosse feita em CrossingPoint().
    vo_crosspoint.push_back(crosspoint);
    vo_crosspoint.push_back(crosspoint);

    if(vpPrimitives[pos]->Status() && vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
    {
        o_ellipse_pos = *(CLevEllipse *)vpPrimitives[pos];
        do {
            loop = false;

            // locate all possible crosses for a given ellipse - all lines
            vo_crosspoint[0].Reinitialize();
            vo_crosspoint[1].Reinitialize();

            size = vpPrimitives.size();
            for(j=0; j<size; j++) {
                //if(vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == LINELEV) // *NMA*: o teste tambem eh necessario aqui, pois o status pode mudar ao longo do loop
                if(vpPrimitives[pos]->Status() && vpPrimitives[j]->Status() && vpPrimitives[j]->GeoEntityType() == LINELEV)  //*NMA*: o teste tamb� � necess�rio aqui, pois o status pode mudar ao longo do loop
                {
                    o_line_j = *(CLevLine *) vpPrimitives[j];

                    codCrossing = CrossingPoints(o_line_j, o_ellipse_pos, vo_crosspoint);

                    switch(codCrossing) {
                        case 1: // um ponto de intersecção entre elipse e reta
                            Resolution(vo_crosspoint[0], deltax, deltay);
                            if (IsAPoint(vo_crosspoint[0], position,deltax,deltay))
                                if(position >= 0 && position < (signed)vpPoints.size())
                                    vo_crosspoint[0] = *vpPoints[position];
                            InterssectedLine(j, vo_crosspoint[0], false);
                            InterssectedEllipse(pos, vo_crosspoint[0], false);
                            break;
                        case 2: // dois pontos de interseção entre elipse e reta
                            Resolution(vo_crosspoint[0], deltax, deltay);
                            if (IsAPoint(vo_crosspoint[0], position,deltax,deltay))
                                if(position >= 0 && position < (signed)vpPoints.size())
                                    vo_crosspoint[0] = *vpPoints[position];
                            Resolution(vo_crosspoint[1], deltax, deltay);
                            if (IsAPoint(vo_crosspoint[1], position,deltax,deltay))
                                if(position >= 0 && position < (signed)vpPoints.size())
                                    vo_crosspoint[1] = *vpPoints[position];
                            InterssectedLine(j,vo_crosspoint, false);
                            InterssectedEllipse(pos,vo_crosspoint,false);
                            break;
                    }
                }
            }
        } while (loop);
    }
    vo_crosspoint.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

CLevGroup* CLevGeoData::GetGroup(int ind)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return &vGroups[ind];
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

int CLevGeoData::GetNumValidPoints(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int pValid = 0;
    for(i=0;i<vpPoints.size();i++)
        if(vpPoints[i]->Status()) pValid++;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pValid;
}

int CLevGeoData::GetNumGroups(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return vGroups.size();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

int CLevGeoData::GetNumValidPrimitives(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int eValid = 0;
    for(i=0;i<vpPrimitives.size();i++)
        if(vpPrimitives[i]->Status()) eValid++;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return eValid;
}

void CLevGeoData::ReflectionLine(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    //vLines[pos].Reflection();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::MovePrimitive(int pos, double deltax, double deltay, double deltaz, bool locateCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vpPrimitives[pos]->Move(deltax, deltay);

    if(locateCrosses)
    {
        if(vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
            LocateEllipsesCrosses();
        else
            LocateLineCrosses();

        LocateCrosses();
    }

    IsGeometryModified(lev_enum::YES);

    Notify(MOVEOPERATION);

    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::MovePoint(int pos, double deltax, double deltay, double deltaz, bool locateCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vpPoints[pos]->Move(deltax, deltay);

    if(locateCrosses)
    {
        LocateLineCrosses();
        LocateEllipsesCrosses();
        LocateCrosses();
    }

    IsGeometryModified(lev_enum::YES);

    Notify(MOVEOPERATION);

    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}


void CLevGeoData::MoveGroup(int pos, double deltax, double deltay, double deltaz, bool locateCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;

    vGroups[pos].Move(deltax, deltay);

    //verifica se existe alguma linha que ficou com as coordenadas dos dois vértices iguais
    for(i=0;i<vpPrimitives.size();i++)
    {
        if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV )
        {
            if(vpPrimitives[i]->vpPoints[0]->X() == vpPrimitives[i]->vpPoints[1]->X())
            {
                if(vpPrimitives[i]->vpPoints[0]->Y() == vpPrimitives[i]->vpPoints[1]->Y())
                    DeletePrimitive(i);
            }
        }
    }

    if(locateCrosses)
    {
        LocateLineCrosses();
        LocateCrosses();
    }

    IsGeometryModified(lev_enum::YES);
    Notify(MOVEOPERATION);

    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::RotateGroup(int pos, double angle, CLevPoint & cp, bool locateCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vGroups[pos].Rotation(angle, cp);

    if(locateCrosses)
    {
        LocateLineCrosses();
        LocateCrosses();
    }

    IsGeometryModified(lev_enum::YES);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    CreateByRotation()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Cria e inclui no modelo nova linha a partir de rotação de uma linha dada.
//
//    parâmetros:    CLevLine line: a linha de referência
//                CLevPoint pc : ponto central (eixo) de rotação
//                double angle : angulo de rotação
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            abril/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::CreateByRotation(CLevLine line, CLevPoint &pc, double angle)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1;
    int pos;

    CLevPoint    o_clickedPoint;

    //*NMA: Reinicializa p0 e p1 para evitar carregar informações erradas
    p0.Reinitialize();
    p1.Reinitialize();

    p0.X(line.vpPoints[0]->X());
    p0.Y(line.vpPoints[0]->Y());

    p1.X(line.vpPoints[1]->X());
    p1.Y(line.vpPoints[1]->Y());

    // rotaciona os dois pontos vértices
    p0.Rotation(angle, pc);
    p1.Rotation(angle, pc);

    // rotaciona o ponto referência clicado
    o_clickedPoint.X(line.oPredis.XClicked());
    o_clickedPoint.Y(line.oPredis.YClicked());

    o_clickedPoint.Rotation(angle, pc);

    pos = Include(p0, p1,false);

    SetPrimitivePreDiscretizationData(pos,
                line.oPredis.NPoints(),line.oPredis.Factor(),
                o_clickedPoint);

    LocateGivenLineAllEllipsesCrosses(pos);
    LocateLineCrosses(pos);

    IsGeometryModified(lev_enum::YES);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//-----------------------------------------------------------------------------------------------------
//    CreateByRotation()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            abril/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::CreateByRotation(CLevEllipse ellipse, CLevPoint &pc, double angle)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1, p2;
    int pos;

    CLevPoint    o_clickedPoint;

    p0.Reinitialize();
    p1.Reinitialize();
    p2.Reinitialize();

    p0.X(ellipse.vpPoints[0]->X());
    p0.Y(ellipse.vpPoints[0]->Y());

    p1.X(ellipse.vpPoints[1]->X());
    p1.Y(ellipse.vpPoints[1]->Y());

    p2.X(ellipse.vpPoints[2]->X());
    p2.Y(ellipse.vpPoints[2]->Y());


    //rotaciona o centro e mantém os angulos inicial e final
    CLevEllipse newellipse(&p0,&p1,&p2,ellipse.vpPoints[3],ellipse.vpPoints[4]);

    newellipse.Rotation(angle, pc);

    pos = Include(newellipse, true);

    o_clickedPoint.X(ellipse.oPredis.XClicked());
    o_clickedPoint.Y(ellipse.oPredis.YClicked());
    o_clickedPoint.Rotation(angle, pc);

    SetPrimitivePreDiscretizationData(pos,
                ellipse.oPredis.NPoints(),ellipse.oPredis.Factor(),
                o_clickedPoint);

    //    LocateCrosses();
    LocateGivenEllipseAllLinesCrosses(pos);

    IsGeometryModified(lev_enum::YES);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//-----------------------------------------------------------------------------------------------------
//    CreateByDisplacement()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Cria e inclui no modelo nova linha a partir de translação de uma linha dada.
//
//    parâmetros:    CLevLine line: a linha de referência
//                double dx    : deslocamento em x
//                double dy    : deslocamento em y
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            junho/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::CreateByDisplacement(CLevLine line, double dx, double dy)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1;
    int pos;  // indicates the position of the line in the list (vector)
    CLevPoint    o_clickedPoint;

    p0.X(line.vpPoints[0]->X() + dx);
    p0.Y(line.vpPoints[0]->Y() + dy);

    p1.X(line.vpPoints[1]->X() + dx);
    p1.Y(line.vpPoints[1]->Y() + dy);

    o_clickedPoint.X(line.oPredis.XClicked() + dx);
    o_clickedPoint.Y(line.oPredis.YClicked() + dy);

    pos = Include(p0, p1,false);

    SetPrimitivePreDiscretizationData(pos,
                    line.oPredis.NPoints(),line.oPredis.Factor(),
                    o_clickedPoint);

    LocateGivenLineAllEllipsesCrosses(pos);
    LocateLineCrosses(pos);

    IsGeometryModified(lev_enum::YES);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//-----------------------------------------------------------------------------------------------------
//    CreateByDisplacement()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Cria e inclui no modelo nova elipse a partir de translação de uma elipse dada.
//
//    parâmetros:    CLevEllipse elipse: a elipse de referência
//                double dx    : deslocamento em x
//                double dy    : deslocamento em y
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            junho/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::CreateByDisplacement(CLevEllipse ellipse, double dx, double dy)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1, p2;
    int pos;

    CLevPoint    o_clickedPoint;

	//    p0.Reinitialize();
	//    p1.Reinitialize();
	//    p2.Reinitialize();

    p0.X(ellipse.vpPoints[0]->X());
    p0.Y(ellipse.vpPoints[0]->Y());

    p1.X(ellipse.vpPoints[1]->X());
    p1.Y(ellipse.vpPoints[1]->Y());

    p2.X(ellipse.vpPoints[2]->X());
    p2.Y(ellipse.vpPoints[2]->Y());

    CLevEllipse newellipse(&p0,&p1,&p2,ellipse.vpPoints[3],ellipse.vpPoints[4]);

    newellipse.Move(dx,dy);

    pos = Include(newellipse, true);

    o_clickedPoint.X(ellipse.oPredis.XClicked());
    o_clickedPoint.Y(ellipse.oPredis.YClicked());
    o_clickedPoint.Move(dx, dy);

    SetPrimitivePreDiscretizationData(pos,
                ellipse.oPredis.NPoints(),ellipse.oPredis.Factor(),
                o_clickedPoint);

    LocateGivenEllipseAllLinesCrosses(pos);

    IsGeometryModified(lev_enum::YES);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}


int CLevGeoData::CreateByDisplacement(CLevGroup grupo, double dx, double dy)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPrimitive * new_primitive = NULL;
    CLevLine    *newline    = NULL;
    CLevEllipse    *newellipse = NULL;
    CLevLine    o_newline;
    CLevEllipse    o_newellipse;

    CLevGroup newgroup;
    int position, posprim;
    int j;
    bool createnewGroup = true;

    newgroup.Reinitialize();
    createnewGroup = true;


    for(j = 0; j<grupo.NumPrimitives();j++)
    {
        switch(grupo.vpPrimitives[j]->GeoEntityType())
        {
			case LINELEV:
				newline   = (CLevLine *) grupo.vpPrimitives[j];
				o_newline = *newline;
				posprim = CreateByDisplacement(o_newline, dx, dy);
				new_primitive = GetPrimitive(posprim);
				//verifica se o status é válido, pois pode ter ocorrido uma interseção,
				//neste caso, o grupo não é mantido
				if(new_primitive->Status()) newgroup.IncludeNextPrimitive(new_primitive);
				else createnewGroup = false;
				break;
			case ELLIPSELEV:
				newellipse = (CLevEllipse *) grupo.vpPrimitives[j];
				o_newellipse = *newellipse;
				posprim = CreateByDisplacement(o_newellipse,dx, dy);
				new_primitive = GetPrimitive(posprim);
				//verifica se o status é válido, pois pode ter ocorrido uma interseção,
				//neste caso, o grupo não é mantido
				if(new_primitive->Status()) newgroup.IncludeNextPrimitive(new_primitive);
				else createnewGroup = false;
				break;
        }
    }

    if(createnewGroup)
    {
        position = Include(newgroup);
        IsGeometryModified(lev_enum::YES);
    }
    else
        position = -1;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return position;
}

//-----------------------------------------------------------------------------------------------------
//    ReadAuxiliarPrimitivesIndexes()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:     Leitura dos indices das primitivas auxiliares
//
//    par�metros:
//                CString nomePRX:  nome do arquivo com extens�o .prx com path completo

//
//    retorno:
//                0    - sucesso
//                100 - erro na abertura do arquivo
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Abril 2016
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::ReadAuxiliarPrimitivesIndexes(std::string nomePRX )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

    int        i;
    int        num_prim = 0;
    int        prim;
    vInt    vPrim;
    std::string versao;

    FILE  *        parqPRX = NULL;

    std::ifstream pFile(nomePRX.c_str());
    std::string line;
    std::string col1, col2;

    //le a versao do arquivo
    std::getline(pFile, line);
    std::istringstream ss(line);
    ss >> versao;

    if(versao == "v01.00")
    {
        //le a descricao do arquivo
        std::getline(pFile, line);

        //le a quantidade de indices
        std::getline(pFile, line);
        std::istringstream ss01(line);
        ss01 >> col1 >> col2;

        num_prim = atoi( col2.c_str() );

        for(i=0;i<num_prim;i++)
        {
            std::getline(pFile, line);

            std::istringstream ss02(line);
            ss02 >> col1;

            prim = atoi( col1.c_str() );

            vPrim.push_back(prim);
        }

        pFile.close();

        for (i = 0; i < vPrim.size(); i++)
        {
            vpPrimitives[vPrim[i]]->IsOfGeometry(false);
        }
    }

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    SaveAuxiliarPrimitivesIndexes()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:     Grava��o dos indices das primitivas auxiliares
//
//    par�metros:
//                CString nomePRX:  nome do arquivo com extens�o .prx com path completo
//
//    retorno:
//                0    - sucesso
//                100 - erro na abertura do arquivo
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                      Angelo Passaro
//    Data:             Abril 2016
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::SaveAuxiliarPrimitivesIndexes(std::string nomePRX )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
	std::string        versao = "v01.00";

	std::cout << "\n" << __FUNCTION__ << " [1] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

	int  i;
    int  num_prim = 0;
    int  prim;

    vInt vPrim;
    FILE *parqPRX = NULL;

	std::cout << "\n" << __FUNCTION__ << " [2] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    for (i = 0; i < vpPrimitives.size(); i++)
        if (vpPrimitives[i]->IsOfGeometry() == false)
        	if(vpPrimitives[i])
        		vPrim.push_back(vpPrimitives[i]->Id());

	std::cout << "\n" << __FUNCTION__ << " [3] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	std::cout << "\n" << nomePRX.c_str(); std::cout.flush();

    num_prim = vPrim.size();

    parqPRX = fopen(nomePRX.c_str(),"w+");
    if (!parqPRX)
    {
    	std::cerr << "Erro ao abrir o arquivo: " << nomePRX << std::endl;
    	std::perror("Motivo");  // Exibe o motivo do erro
        return 100;
    }

	fprintf(parqPRX, "%s", versao.c_str());
	fprintf(parqPRX, "\nDescription: Auxiliar Primitives Indexes");
	fprintf(parqPRX, "\nQuantity %d", num_prim);
	for(i=0;i<num_prim;i++)
	{
		fprintf(parqPRX,"\n%d ", vPrim[i]);
	}

	std::cout << "\n" << __FUNCTION__ << " [4] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    fclose(parqPRX);

	std::cout << "\n" << __FUNCTION__ << " [5] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    #ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    SaveMGP()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Gravação de primitivas de geometria com dados de pre-discretização
//
//    parâmetros:
//                CString nomeMGP:  nome do arquivo com extensão .mgp com path completo
//
//    retorno:
//                0    - sucesso
//                100 - erro na abertura do arquivo
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Fevereiro/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::SaveGeometricModel(std::string nomeMGP)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    std::string  versao = "01.00";
    char         simetria = 'P';
    int             length_conversion_factor;
    double         xmin,ymin,xmax,ymax; // Variaveis utilizadas para gravar retangulo englobante um pouco maior do que
    double         deltax, deltay;    // Os limites do desenho *NMA* 06/06/2000
    FILE  *         parqMGP = NULL;
    std::string  str_symmetry = project->Symmetry();
    int             id_primitive = 0;
    int index = project->GetCurrentPhenIndex();

    if(index < 0 || index >= (signed)project->vpPhenSpecificationVector.size() )
        length_conversion_factor = 0; // assume gravacao em metros
    else
        length_conversion_factor = project->vpPhenSpecificationVector[index]->Get_UnitFactorLength();
    
    if(str_symmetry == "plane")
        simetria = 'P';
    else if(str_symmetry == "axial")
        simetria = 'A';

    xmin = original.GetXMin() * pow(10,length_conversion_factor);
    ymin = original.GetYMin() * pow(10,length_conversion_factor);
    xmax = original.GetXMax() * pow(10,length_conversion_factor);
    ymax = original.GetYMax() * pow(10,length_conversion_factor);

    deltax = xmax - xmin;
    deltay = ymax - ymin;

    xmin = xmin - deltax/10;
    ymin = ymin - deltay/10;
    xmax = xmax + deltax/10;
    ymax = ymax + deltay/10;
    
    parqMGP = fopen(nomeMGP.c_str(),"w+");
    if (!parqMGP)
    {
        if(parqMGP)
            fclose(parqMGP);
        return 100;
    }
    else
    {
        fprintf(parqMGP, "%s\n", versao.c_str());
        fprintf(parqMGP, "%.7lf %.7lf %.7lf %.7lf\n", xmin, ymin, xmax, ymax);
        fprintf(parqMGP, "%d\t%c\n", length_conversion_factor, simetria);

        //*NMA*:importante: manter a sequencia linhas e depois elipses, na gravação para compatibilidade com
        // versões antigas (leitura de model regions!!!!)

        // Gravação das linhas
        for(i=0; i<vpPrimitives.size(); i++)
        {
            if (vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
            {
                vpPrimitives[i]->Save(parqMGP, length_conversion_factor, MODEL_GEOMETRY_PREDIS);
                vpPrimitives[i]->Id(id_primitive++);// para consistencia com o arquivo de primitivas com cc , no qual são gravados os indices das primitivas
            }
        }

        //gravação das elipses e círculos
        for(i=0; i<vpPrimitives.size(); i++)
        {
            if (vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
            {
                vpPrimitives[i]->Save(parqMGP,length_conversion_factor, MODEL_GEOMETRY_PREDIS);
                vpPrimitives[i]->Id(id_primitive++); // para consistencia com o arquivo de primitivas com cc, no qual são gravados os indices das primitivas
            }
        }


        fprintf(parqMGP,"\n" );
        putc(EOF,parqMGP);                                    /* coloca terminacao */
        fclose(parqMGP);
    }
    IsGeometryModified(lev_enum::NO);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 1;
}

int CLevGeoData::SaveModelRegions(std::string  fileName)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i,j;
    int        num_reg=0;
    int        num_lines=0, num_el=0;
    int        prim;
    long    ind_mat, ind_source;
    vInt    vPrim;
    vLong    vIndex;
    vInt    vPrimType;
    FILE *  parq;

    int        ind;
    vInt    vElFrontInd;

    num_reg = GetNumValidRegions();

    if(num_reg)
    {
        parq = fopen(fileName.c_str(),"w");

        if (!parq)
        {
            if(parq) fclose(parq);
            return 106;
        }
        else
        {
            fprintf(parq,"%d",num_reg);

            for(i=0;i<vpModelRegions.size();i++)
            {
                if(vpModelRegions[i]->Status())
                {
                //indice de material e de fonte
                    ind_mat        = vpModelRegions[i]->GetMaterial();
                    ind_source    = vpModelRegions[i]->GetSource();

                    fprintf(parq,"\n\n%ld %ld",ind_mat, ind_source);

                    //22/02/05:*NMA*: para manter consistencia com aplicatiovo para otimização genético
                    num_lines    = vpModelRegions[i]->NumValidLines();
                    num_el        = vpModelRegions[i]->NumValidCurves();

                    fprintf(parq,"\n\n%d %d",num_lines, num_el);

                    //22/02/05:*NMA*: para manter consistencia com aplicatiovo para otimização genético
                    //*NMA*: 22/11/2004: formato antigo de gravação para arquivos com extensão(.reg)
                    //para não alterar os arquivos e evitar tratamento de versão
                    //grava linhas
                    for(j=0;j<vpModelRegions[i]->vpPrimitives.size();j++)
                    {
                        if(vpModelRegions[i]->vpPrimitives[j]->GeoEntityType() == LINELEV)
                        {
                            prim = vpModelRegions[i]->vpPrimitives[j]->Id();
                            fprintf(parq,"\n%d", prim);
                        }
                    }

                    //grava elipses
                    for(j=0;j<vpModelRegions[i]->vpPrimitives.size();j++)
                    {
                        if(vpModelRegions[i]->vpPrimitives[j]->GeoEntityType() == ELLIPSELEV)
                        {
                            prim = vpModelRegions[i]->vpPrimitives[j]->Id();
                            fprintf(parq,"\n%d", prim);
                        }
                    }

                    //grava indice dos elementos da malha de fronteira

                    vpModelRegions[i]->GetElementsRegion(vElFrontInd);
                    fprintf(parq,"\n\n%d", vElFrontInd.size());
                    for(j=0;j<vElFrontInd.size();j++)
                    {
                        ind = vElFrontInd[j];
                        fprintf(parq,"\n%d", ind);
                    }
                }
            }
        }
        fclose(parq);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    ReadELL()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Leitura da linhas de geometria seguindo padrão antigo
//
//    parâmetros:
//                CString nomeELL:  nome do arquivo com extensão ell com path completo
//
//    retorno:
//                0    - sucesso
//                101 - erro na abertura do arquivo
//                102 - arquivo com primitiva não suportada por esta versão
//                102 - erro na inclusão de linha
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:
//    Modificado em:    Setembro/2001
//    Motivo:            Implementação em Lev077897
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::ReadELL(std::string nomeELL)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    std::string        versao,tipoPrimitiva ;
    std::string     str_unit;
    char        vers[500],tp[500];
    char        simetria ;
    int            unidade, indice_posicao ;
    int            retorno;
    char        c;
    CLevPoint    p0,p1;
    FILE  *parqell = NULL;
    int            unit=0;
    double        x_min, y_min,x_max, y_max;

    char    reta[] ="reta-segmento";
    char    elipse[] ="elipse";
    char    circ[]     ="circunferencia";

    parqell = fopen(nomeELL.c_str(),"r");
    if (!parqell)
    {
        if(parqell) fclose(parqell);
        return 101;
    }
    else
    {
        fscanf(parqell, "%s", &vers[0]);
        fscanf(parqell, "%lg %lg %lg %lg",  &x_min, &y_min, &x_max, &y_max);
        fscanf(parqell, "%d %c", &unidade, &simetria);

        original.SetXMin(x_min);
        original.SetYMin(y_min);
        original.SetXMax(x_max);
        original.SetYMax(y_max);

        switch(unidade)
        {
            case 0:
                unit = 0; //metro
                str_unit = "meter";
            break;

            case -2:
                unit = 1; //centímetro
                str_unit = "centimeter";
            break;

            case -3:
                unit = 2; // milímetro
                str_unit = "milimeter";
            break;

        }

        //project->SetUnit_of_Length(unit);
        int index = project->GetCurrentPhenIndex();
        project->vpPhenSpecificationVector[index]->LengthUnit(str_unit);

        pow(original.GetXMin(), -unidade);
        pow(original.GetXMax(), -unidade);
        pow(original.GetYMin(), -unidade);
        pow(original.GetYMax(), -unidade);

        c=getc(parqell);

        while(c!=EOF)
        {
            ungetc(c,parqell);
            fscanf(parqell,"%s %d",&tp[0],&indice_posicao);

            if(!strcmp(tp,reta)) // inclui linhas
            {
                CLevLine    oline;
                CLevPoint    p1,p2;

                //*NMA*: O vetor de ponteiros para pontos da linha deve
                //        estar preenchido antes da leitura, para que possa
                //        ser alterado pelo próprio objeto com as coordenadas lidas do arquivo.
                vpCLevPoint vpPt;
                vpPt.reserve(2);

                p1.GeoEntityType(GEOM);
                p2.GeoEntityType(GEOM);

                vpPt.push_back(&p1);
                vpPt.push_back(&p2);

                oline.SetPoints(vpPt);

                oline.Read(parqell,unidade,tp,MODEL_CONNECTION);

                retorno = CLevGeoData::Include(oline);

                if(retorno < 0)
                {
                    // mensagem erro
                }

            }
            else
            {
                if(!strcmp(tp,circ) || !strcmp(tp,elipse))
                {// inclui elipse ou circunferencia
                    CLevEllipse ellipse;
                    CLevPoint    p1,p2,pCentral,pAxisX,pAxisY;

                    //*NMA*: O vetor de ponteiros para pontos da elipse deve
                    //        estar preenchido antes da leitura, para que possa
                    //        somente ser alterado pela própria elipse.
                    vpCLevPoint vpPt;
                    vpPt.reserve(5);
                    vpPt.push_back(&p1);
                    vpPt.push_back(&p2);
                    vpPt.push_back(&pCentral);
                    vpPt.push_back(&pAxisX);
                    vpPt.push_back(&pAxisY);

                    ellipse.SetPoints(vpPt);

                    ellipse.Read(parqell,unidade,tp,MODEL_CONNECTION);

                    retorno = Include(ellipse,false);

                    if(retorno < 0)
                    {
                        // mensagem erro
                    }

                }
            }

            c=getc(parqell);
        }
        fclose(parqell);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    SaveModelMeshFrontier()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     grava dados da malha de fronteira para identificação de regiões fechadas do modelo
//                geométrico.
//
//    parâmetros:    CString fileName: nome do arquivo com path completo.
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Maio/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int    CLevGeoData::SaveModelMeshFrontier(std::string filename)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    FILE        *parqMMF = NULL;

    long int    i, num_elem_fronteira, num_pontos_fronteira, num_geom_fronteira;
    Elemento_2D triangulo;
    long        viz_ordenados[3];
    long        vertices[5] ;
    double        volume;

    Aresta_Geometria aresta;

    parqMMF = fopen(filename.c_str(),"w");
    if (!parqMMF)
    {
        if(parqMMF) fclose(parqMMF);
        return 200;
    }
    else
    {
       //------------- grava dados dos elementos da malha de fronteira-------------------
        num_elem_fronteira = vElementos_fronteira.size();

        fprintf (parqMMF,"%ld\n",num_elem_fronteira);

        // gravação da malha de fronteira como 0-based
        for(i=0;i<num_elem_fronteira;i++)
        {
            triangulo = vElementos_fronteira[i];
            triangulo.Ordena_Vizinhos(vElementos_fronteira, viz_ordenados);
            triangulo.Get_Vertices(vertices);
            volume = triangulo.Get_Volume();
            fprintf(parqMMF,"%ld %ld %ld %ld %ld %ld %lf\n",
                    vertices[0], vertices[1], vertices[2] ,
                    viz_ordenados[0], viz_ordenados[1], viz_ordenados[2],volume);
        }//fecha for elementos

        //------------------ grava dados das arestas de geometria da malha de fronteira------------
        num_geom_fronteira = vArestasGeometria_fronteira.size();

        fprintf (parqMMF,"%ld\n",num_geom_fronteira);

        for(i=0;i<num_geom_fronteira;i++)
        {
            aresta = vArestasGeometria_fronteira[i];
            fprintf(parqMMF,"%d %d %d %d\n",aresta.g1,aresta.g2, aresta.prim, aresta.primType);

        }//fecha for arestas

        // --------------------grava dados dos pontos da malha de fronteira----------------------
        num_pontos_fronteira = vPontos_fronteira.size();

        fprintf (parqMMF,"%ld\n",num_pontos_fronteira);

        for(i=0;i<num_pontos_fronteira;i++)
        {
            fprintf(parqMMF,"%g %g\n",vPontos_fronteira[i].X(),vPontos_fronteira[i].Y() );

        }//fecha for pontos

        fclose(parqMMF);
    }//fecha else: conseguiu abrir arquivo
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

void CLevGeoData::PreDiscretizePrimitive(int position, vCLevPoint &vo_ptPredis)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vpPrimitives[position]->PreDiscretize(vo_ptPredis);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::GetPrimitivePreDiscretization(int position, vCLevPoint &vo_ptPredis)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vpPrimitives[position]->GetPredisPoints(vo_ptPredis);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

////////////////////////////////////////////////////////////////
// Informa dados de pre_discretizacao de primitiva
////////////////////////////////////////////////////////////////
void CLevGeoData::SetPrimitivePreDiscretizationData(int position,
                            int nPoints,double factor,
                            CLevPoint clickedPoint)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vpPrimitives[position]->SetPreDiscretizationData(nPoints, factor, clickedPoint);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    IncludeEdge()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:  Basicamente é o código de IncludeLine, porém inserindo a linha
//                    na lista de arestas (vEdges)
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  Outubro/2001
//    Motivo:            Adaptação de IncludeLine para IncludeEdge
//-----------------------------------------------------------------------------------------------------
CLevLine* CLevGeoData::IncludeEdge(CLevPoint &ro_point0, CLevPoint &ro_point1,int & insertionposition )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
	// Códigos de erros a partir de 100000
    //  nesta rotina os erros são  100001, 100002 e  100003
    unsigned int i;
    CLevPoint    *po_point_i = NULL,
                *po_point_f = NULL;
    CLevLine    o_line0;

    CLevLine    * po_line = NULL;

    vpCLevPoint vpPt;
    vpPt.reserve(2);

    if (ro_point0 == ro_point1)
    {
        insertionposition = -1;
        return NULL;
    }
    else
    {
        for (i = 0; i < vpPoints.size(); i++)
        {
            if (ro_point0 == *vpPoints[i]) {po_point_i = vpPoints[i]; break;}
        }

        for (i = 0; i < vpPoints.size(); i++)
        {
            CLevPoint paxu = *vpPoints[i];

            if (ro_point1 == *vpPoints[i]) {po_point_f = vpPoints[i]; break;}
        }

        if (po_point_i && po_point_f)
        {
        }
        else
        {
            if (!po_point_i)
            {
                po_point_i = CLevGeoData::Include(ro_point0,PRE);
                if (!po_point_i) return NULL;
            }
            if (!po_point_f)
            {
                po_point_f = CLevGeoData::Include(ro_point1,PRE);
                if (!po_point_f) return NULL;
            }
        }

        if(po_point_i->GeoEntityType() != GEOM) po_point_i->GeoEntityType(PRE);
        if(po_point_f->GeoEntityType() != GEOM) po_point_f->GeoEntityType(PRE);

        //o_line0.SetVertices(po_point_i,po_point_f);
        vpPt.push_back(po_point_i);
        vpPt.push_back(po_point_f);
        o_line0.SetPoints(vpPt);

        for (i = 0; i < vEdges.size(); i++)
        {
            if (o_line0 == vEdges[i])
            {
                insertionposition = i;
                return 0; break;
            }
        }

        vEdges.push_back(o_line0);
        vEdges.back().Status(LEV_VALID);

        //Atualiza os pontos ro_point0 e ro_point1
        ro_point0 = *vEdges.back().vpPoints[0];
        ro_point1 = *vEdges.back().vpPoints[1];

        insertionposition = vEdges.size()-1;

    }

    po_line = &vEdges.back();

    vpPt.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return po_line;
}

//-------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//     SetIdToValidPoints()
//-----------------------------------------------------------------------------------
// *NMA*: Atribui o Id do ponto desconsiderando os pontos não válidos
//-----------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////
void CLevGeoData::SetIdToValidPoints(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int id_point = 0;
    unsigned int i;

    for(i=0;i<vpPoints.size();i++)
        if(vpPoints[i]->Status())
            vpPoints[i]->Id(id_point++);

    numValidPoints = id_point;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------
//     SetIdToValidLines()
//-----------------------------------------------------------------------------------
// *NMA*:
//        Atribui os Id's das Linhas desconsiderando as linhas não válidas .
//        Atribui os Id's das Elipses desconsiderando as elipses não válidas, a numeração
//         segue na seqüência da numeração das linhas.
// Observação: Os id's das linhas e elipses são usados como número da primitiva a que
//                pertencem as arestas de pré-discretização.
//-----------------------------------------------------------------------------------
void CLevGeoData::SetIdToValidPrimitives(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int id_primitives = 0;
    unsigned int i;

    for(i=0;i<vpPrimitives.size();i++)
        if(vpPrimitives[i]->Status())
            vpPrimitives[i]->Id(id_primitives++);

    numValidPrimitives = id_primitives;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------
//     SetIdToValidModelRegions()
//-----------------------------------------------------------------------------------
// *NMA*:
//        Atribui os Id's das regiões do modelo.
//        Esta informação é usada pelas sub-regiões
//-----------------------------------------------------------------------------------
void CLevGeoData::SetIdToValidModelRegions(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int num_reg = 0;

    // Atribui Id's às sub-regiões
    for(i=0;i<vpModelRegions.size();i++)
        if(vpModelRegions[i]->Status() == LEV_VALID)
            vpModelRegions[i]->Id(num_reg++);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------
// *NMA*, 13/12/2001
// void CLevGeoData::BuildEdges(void)
//
// Constroi lista de arestas de geometria
// Atribui o Id da Edge igual ao Id da linha de geometria
//
// *NMA*, 22/11/2004
// Reengenharia: Tratamento unificado de primitivas
//-----------------------------------------------------------------------------------
void CLevGeoData::BuildEdges(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vCLevPoint vp_ptPredis;
    CLevPoint o_xp1,o_xp2, o_paux;
    int insertion_pos, j;
    int contPoints;
    unsigned int i;

    numValidEdges = 0;

    if(vEdges.size())
        vEdges.clear();

    // Varredura da lista de pontos para invalidar pontos tipo PRE
    // que tiverem sido colocados na lista por gravação prévia
    for(i=0; i<vpPoints.size(); i++)
        if(vpPoints[i]->Status() && vpPoints[i]->GeoEntityType() == PRE)
        {
            vpPoints[i]->Status(LEV_INVALID);
            numValidPoints--;
        }

    //..................................................................
    // Inclui edges de discretização de primitivas
    //..................................................................
    for(i=0; i<vpPrimitives.size(); i++)
    {
        if(vpPrimitives[i]->Status())
        {
            //reinicializa vetor com indices de arestas de geometria
            vpPrimitives[i]->vIndexArestaGeometria.clear();

            //*NMA*:28/07/2005:reinicializa ponteiros para pontos de pré-discretização
            vpPrimitives[i]->vpPredisPoints.clear();

            o_xp1.X( vpPrimitives[i]->vpPoints[0]->X());
            o_xp1.Y( vpPrimitives[i]->vpPoints[0]->Y());

            o_xp2.X( vpPrimitives[i]->vpPoints[1]->X());
            o_xp2.Y( vpPrimitives[i]->vpPoints[1]->Y());

            o_xp1.GeoEntityType(GEOM);
            o_xp2.GeoEntityType(GEOM);

            if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
            {
                // Elipse completa
                if(o_xp1 == o_xp2)
                {
                    if(vpPrimitives[i]->oPredis.NPoints() < 4)
                        vpPrimitives[i]->oPredis.SetPreDiscretizationData(4, 1, 0, 0);
                }
                else if(vpPrimitives[i]->oPredis.NPoints() < 4)
                    vpPrimitives[i]->oPredis.SetPreDiscretizationData(4, 1, 0, 0);
            }

            if(vpPrimitives[i]->oPredis.NPoints() > 2)
            {
                if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
                    PreDiscretizePrimitive(i,vp_ptPredis);
                else if(vpPrimitives[i]->GeoEntityType() == LINELEV)
                    PreDiscretizePrimitive(i,vp_ptPredis);

                // Primeira aresta
                IncludeEdge(o_xp1,vp_ptPredis[0], insertion_pos);
                vEdges[insertion_pos].Id(i);
                vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                numValidEdges++;

                //*NMA*:28/07/2005:inclui ponto no vetor de ponteiros para pontos de pré-discretização
                if(*vEdges[insertion_pos].vpPoints[0] == vp_ptPredis[0])
                    vpPrimitives[i]->vpPredisPoints.push_back(vEdges[insertion_pos].vpPoints[0]);
                else
                    vpPrimitives[i]->vpPredisPoints.push_back(vEdges[insertion_pos].vpPoints[1]);

                if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV) {
                    // Elipse completa
                    if(o_xp1 == o_xp2)
                        contPoints = vpPrimitives[i]->oPredis.NPoints()-2;
                    else
                        contPoints = vpPrimitives[i]->oPredis.NPoints()-3;
                } else
                    contPoints = vpPrimitives[i]->oPredis.NPoints()-3;

                for(j=0; j<contPoints; j++)
                {
                    IncludeEdge(vp_ptPredis[j],vp_ptPredis[j+1], insertion_pos);
                    vEdges[insertion_pos].Id(i);
                    vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                    numValidEdges++;

                    //*NMA*:28/07/2005:inclui ponto no vetor de ponteiros para pontos de pré-discretização
                    if(*vEdges[insertion_pos].vpPoints[0] == vp_ptPredis[j+1])
                        vpPrimitives[i]->vpPredisPoints.push_back(vEdges[insertion_pos].vpPoints[0]);
                    else
                        vpPrimitives[i]->vpPredisPoints.push_back(vEdges[insertion_pos].vpPoints[1]);
                }

                // Ultima aresta
                IncludeEdge(vp_ptPredis[j],o_xp2, insertion_pos);
                vEdges[insertion_pos].Id(i);
                vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE) vpPrimitives[i]->GeoEntityType());
                numValidEdges++;
            }
            else
            {
                // Inclusão de aresta única
                IncludeEdge(o_xp1,o_xp2, insertion_pos);
                vEdges[insertion_pos].Id(i);
                vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                numValidEdges++;
            }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//..................................................................
//-----------------------------------------------------------------------------------
// *NMA*, 25/09/2002
// void CLevGeoData::BuildEdgesFronteira(void)
//
// Constrói lista de arestas de geometria para geração de malha de fronteira
// Atribui o Id da Edge igual ao Id da linha de geometria
//
// *NMA*, 22/11/2004
// Reengenharia: Tratamento unificado de primitivas
//-----------------------------------------------------------------------------------
void CLevGeoData::BuildEdgesFronteira(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    vCLevPoint    vp_ptPredis;
    CLevPoint    o_xp1,o_xp2, o_paux;
    int            insertion_pos, j;
    int            contPoints;
    int            numPointsEllipseOriginal;
    CLevEllipse o_ellipse;
    CLevPoint    o_point_aux;
    int            num_predis;
    CPredis     predisOriginal;
    int            num_predis_lines = 3;
    unsigned long i;

    numValidEdges = 0;
    if(vEdges.size()) vEdges.clear();

    // Varredura da lista de pontos para invalidar pontos tipo PRE
    //    que tiverem sido colocados na lista por gravação prévia
    for(i=0;i<vpPoints.size();i++)
        if(vpPoints[i]->Status() && vpPoints[i]->GeoEntityType() == PRE)
        {
            vpPoints[i]->Status(LEV_INVALID);
            numValidPoints--;
        }

    // TODO: RYT desativado
    if(0)
    {
        for(i=0;i<vpPrimitives.size();i++)
        {
            if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
            {
                o_xp1.X( vpPrimitives[i]->vpPoints[0]->X());
                o_xp1.Y( vpPrimitives[i]->vpPoints[0]->Y());

                o_xp2.X( vpPrimitives[i]->vpPoints[1]->X());
                o_xp2.Y( vpPrimitives[i]->vpPoints[1]->Y());

                o_xp1.GeoEntityType(GEOM);
                o_xp2.GeoEntityType(GEOM);

                //Altera pré-discretização do usuário para geração da malha de fronteira

                predisOriginal = vpPrimitives[i]->oPredis;

                vpPrimitives[i]->oPredis.SetPreDiscretizationData(num_predis_lines,1,0,0);

                PreDiscretizePrimitive(i,vp_ptPredis);

                //primeira aresta
                IncludeEdge(o_xp1,vp_ptPredis[0], insertion_pos);
                 if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                 {
                     vEdges[insertion_pos].Id(i);
                       vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                     numValidEdges++;
                 }

                contPoints = vpPrimitives[i]->oPredis.NPoints()-3;
                for(j=0;j<contPoints;j++)
                {
                    IncludeEdge(vp_ptPredis[j],vp_ptPredis[j+1], insertion_pos);
                    if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                    {
                         vEdges[insertion_pos].Id(i);
                           vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                         numValidEdges++;
                    }
                }

                //ultima aresta
                 IncludeEdge(vp_ptPredis[j],o_xp2, insertion_pos);
                 if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                 {
                     vEdges[insertion_pos].Id(i);
                       vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                     numValidEdges++;
                 }

                //restaura pré-discretização original
                 vpPrimitives[i]->oPredis.SetPreDiscretizationData(predisOriginal.NPoints(),predisOriginal.Factor(),predisOriginal.XClicked(),predisOriginal.YClicked());
                PreDiscretizePrimitive(i,vp_ptPredis);
            }
        }
    }
    else
    {
        for(i=0;i<vpPrimitives.size();i++)
        {
            if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
            {
                o_xp1.X( vpPrimitives[i]->vpPoints[0]->X());
                o_xp1.Y( vpPrimitives[i]->vpPoints[0]->Y());

                o_xp2.X( vpPrimitives[i]->vpPoints[1]->X());
                o_xp2.Y( vpPrimitives[i]->vpPoints[1]->Y());

                o_xp1.GeoEntityType(GEOM);
                o_xp2.GeoEntityType(GEOM);

                 IncludeEdge(o_xp1,o_xp2, insertion_pos);
                 if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                 {
                     vEdges[insertion_pos].Id(i);
                       vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                     numValidEdges++;
                 }
            }
        }
    }

    for(i=0;i<vpPrimitives.size();i++)
    {
        if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
        {
            o_xp1.X( vpPrimitives[i]->vpPoints[0]->X());
            o_xp1.Y( vpPrimitives[i]->vpPoints[0]->Y());

            o_xp2.X( vpPrimitives[i]->vpPoints[1]->X());
            o_xp2.Y( vpPrimitives[i]->vpPoints[1]->Y());

            o_xp1.GeoEntityType(GEOM);
            o_xp2.GeoEntityType(GEOM);

            numPointsEllipseOriginal = vpPrimitives[i]->oPredis.NPoints();

            if(o_xp1 == o_xp2) // elipse completa
            {
                num_predis = 15;
            }
            else num_predis = 5;

            //std::cout << "\n em CLevGeoData::BuildEdgesFronteira(void): num_predis = " << num_predis << "\n" << std::cout.flush();

            if(vpPrimitives[i]->oPredis.NPoints() < num_predis )
                vpPrimitives[i]->oPredis.SetPreDiscretizationData(num_predis = 15,1,0,0);

            if(vpPrimitives[i]->oPredis.NPoints() >= num_predis)
            {
                PreDiscretizePrimitive(i,vp_ptPredis);

                //primeira aresta
                IncludeEdge(o_xp1,vp_ptPredis[0], insertion_pos);
                if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                {
                    vEdges[insertion_pos].Id(i);
                      vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                    numValidEdges++;
                }

                if(o_xp1 == o_xp2) // elipse completa
                    contPoints = vpPrimitives[i]->oPredis.NPoints()-2;
                else contPoints = vpPrimitives[i]->oPredis.NPoints()-3;

                for(j=0;j<contPoints;j++)
                {
                    IncludeEdge(vp_ptPredis[j],vp_ptPredis[j+1], insertion_pos);
                     if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                     {
                         vEdges[insertion_pos].Id(i);
                           vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                         numValidEdges++;
                     }
                }

                //ultima aresta
                 IncludeEdge(vp_ptPredis[j],o_xp2, insertion_pos);
                 if(insertion_pos >=0 && insertion_pos < (signed)vEdges.size())
                 {
                     vEdges[insertion_pos].Id(i);
                       vEdges[insertion_pos].GeoEntityType((GEOENTITYTYPE)vpPrimitives[i]->GeoEntityType());
                     numValidEdges++;
                 }
            }

            vpPrimitives[i]->oPredis.SetPreDiscretizationData(numPointsEllipseOriginal,1,0,0);
            PreDiscretizePrimitive(i,vp_ptPredis);
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::BuildVetorArestasPrimitivas(vArestaGeometria &vBlocoGeometria)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int num, i;
    int    prim;

    for(i=0;i<vpPrimitives.size();i++)
        vpPrimitives[i]->vIndexArestaGeometria.clear();

    num = vBlocoGeometria.size();

    for(i=0;i<num;i++)
    {
        prim = vBlocoGeometria[i].prim;
        vpPrimitives[prim]->vIndexArestaGeometria.push_back(i);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::IsGeometryModified(lev_enum::LEV_BOOL b)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    isGeometryModified = b;

    if(isGeometryModified == lev_enum::YES)
        meshfronteiraOk = false;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::Reinitialize(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    meshfronteiraOk        = false;
    modelregionsOk        = false;
    modelprimitivesOk    = false;

    isGeometryModified    = lev_enum::NO;

    numValidPoints        = 0;
    numValidPrimitives    = 0;
    numValidEdges        = 0;

    original.SetXMin(0.);
    original.SetYMin(0.);
    original.SetXMax(1e-12);// *NMA* para permitir o redimensionamento da janela para os
    original.SetYMax(1e-12);// casos em que o usuário entra com valores muito pequenos

    project = NULL;
    
    FreeMemory();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

int CLevGeoData::CreateByReflection(CLevLine line, OPERATIONCONTROL mode,CLevPoint &Wfirstpointclicked ,CLevPoint &Wlastpointclicked )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1, aux, eixo_universal;
    int pos;
    double angle,m;

    CLevPoint    o_clickedPoint;

    //*NMA: Reinicializa p0 e p1 para evitar carregar informações erradas
    p0.Reinitialize();
    p1.Reinitialize();

    p0.X(line.vpPoints[0]->X());
    p0.Y(line.vpPoints[0]->Y());

    p1.X(line.vpPoints[1]->X());
    p1.Y(line.vpPoints[1]->Y());

    switch(mode)
    {

        case REFLECTIONXAXIS:
        case PRIMITIVELINEREFLECTIONXAXIS:
        case PRIMITIVEGROUPREFLECTIONXAXIS:
        {
            p0.ReflectionByX();
            p1.ReflectionByX();

            break;
        }
        case REFLECTIONYAXIS:
        case PRIMITIVELINEREFLECTIONYAXIS:
        case PRIMITIVEGROUPREFLECTIONYAXIS:
        {
            p0.ReflectionByY();
            p1.ReflectionByY();

            break;
        }
        case    REFLECTION2POINTS:
        case    REFLECTIONGIVENLINE:
        case     PRIMITIVELINEREFLECTION2POINTS:
        case     PRIMITIVEGROUPREFLECTION2POINTS:
        {
            //    verificação de pontos (o mais próximo da origem vai ser o firstpoint)

            if (Wfirstpointclicked.Y()<0 && Wlastpointclicked.Y()<0)
            {
                if (Wfirstpointclicked.Y()<Wlastpointclicked.Y())
                {
                    aux = Wfirstpointclicked;
                    Wfirstpointclicked = Wlastpointclicked;
                    Wlastpointclicked = aux;
                }
            }
            if (Wfirstpointclicked.Y()>Wlastpointclicked.Y())
            {
                aux = Wfirstpointclicked;
                Wfirstpointclicked = Wlastpointclicked;
                Wlastpointclicked = aux;

            }

            m = atan2((Wlastpointclicked.Y()-Wfirstpointclicked.Y()),(Wlastpointclicked.X()-Wfirstpointclicked.X()));
            angle= (180*m)/3.141592;

            eixo_universal.X(0.0);
            eixo_universal.Y(0.0);

            //translaçao para aorigem ()

            p0.Move(-Wfirstpointclicked.X(),-Wfirstpointclicked.Y(),0);//original
            p1.Move(-Wfirstpointclicked.X(),-Wfirstpointclicked.Y(),0);

            //rotação negativa do angulo do eixo

            p0.Rotation(-angle,eixo_universal );
            p1.Rotation(-angle,eixo_universal );

            // reflexao EM RELAÇÃO AO EIXO X
            p0.ReflectionByX();
            p1.ReflectionByX();

            //rotação positiva do angulo do eixo

            p0.Rotation(angle,eixo_universal);
            p1.Rotation(angle,eixo_universal);

            //translação positiva
            p0.Move(Wfirstpointclicked.X(),Wfirstpointclicked.Y(),0);//original
            p1.Move(Wfirstpointclicked.X(),Wfirstpointclicked.Y(),0);

            break;
        }
        default:
            break;
    }

	pos = Include(p0, p1,false);

	SetPrimitivePreDiscretizationData(pos,
	line.oPredis.NPoints(),line.oPredis.Factor(),
	o_clickedPoint);

	LocateLineCrosses();
	LocateCrosses();
	TakeLimits();

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif

    return pos;
}

int CLevGeoData::CreateByReflection(CLevEllipse ellipse, OPERATIONCONTROL mode,CLevPoint &Wfirstpointclicked ,CLevPoint &Wlastpointclicked )
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p0,p1,p2, aux, eixo_universal;
    int pos;
    double angle,m;

    CLevPoint    o_clickedPoint;

    //*NMA: Reinicializa p0 p1 p2 para evitar carregar informações erradas
    p0.Reinitialize();
    p1.Reinitialize();
    p2.Reinitialize();


    p0.X(ellipse.vpPoints[0]->X());
    p0.Y(ellipse.vpPoints[0]->Y());

    p1.X(ellipse.vpPoints[1]->X());
    p1.Y(ellipse.vpPoints[1]->Y());

    p2.X(ellipse.vpPoints[2]->X());
    p2.Y(ellipse.vpPoints[2]->Y());

    CLevEllipse newellipse(&p0,&p1,&p2,ellipse.vpPoints[3],ellipse.vpPoints[4]);
    switch(mode)
    {

        case REFLECTIONXAXIS:
        case PRIMITIVEELLIPSEREFLECTIONXAXIS:
        case PRIMITIVEGROUPREFLECTIONXAXIS:
            newellipse.ReflectionByX();
            break;
        case REFLECTIONYAXIS:
        case PRIMITIVEELLIPSEREFLECTIONYAXIS:
        case PRIMITIVEGROUPREFLECTIONYAXIS:
            newellipse.ReflectionByY();
            break;
        case REFLECTION2POINTS:
        case REFLECTIONGIVENLINE:
        case PRIMITIVEELLIPSEREFLECTION2POINTS:
        case PRIMITIVEGROUPREFLECTION2POINTS:
        {

            //_______________________________________________________________________
            //
            //    verificação de pontos (o mais próximo da origem vai ser o firstpoint)
            //________________________________________________________________________

            if (Wfirstpointclicked.Y()<0 && Wlastpointclicked.Y()<0)
			{
				if (Wfirstpointclicked.Y()<Wlastpointclicked.Y())
				{
					aux = Wfirstpointclicked;
					Wfirstpointclicked = Wlastpointclicked;
					Wlastpointclicked = aux;
				}
			}
			if (Wfirstpointclicked.Y()>Wlastpointclicked.Y())
			{
				aux = Wfirstpointclicked;
				Wfirstpointclicked = Wlastpointclicked;
				Wlastpointclicked = aux;

			}
			//________________________________________________________________________
			m = atan2((Wlastpointclicked.Y()-Wfirstpointclicked.Y()),(Wlastpointclicked.X()-Wfirstpointclicked.X()));
			angle= (180*m)/3.141592;

			eixo_universal.X(0.0);
			eixo_universal.Y(0.0);

            //translaçao para aorigem ()
            newellipse.Move(-Wfirstpointclicked.X(),-Wfirstpointclicked.Y(),0);

            //rotação negativa do angulo do eixo
            newellipse.Rotation(-angle,eixo_universal );

            // reflexao EM RELAÇÃO AO EIXO X
            newellipse.ReflectionByX();

            //rotação positiva do angulo do eixo
            newellipse.Rotation(angle,eixo_universal);

            //translação positiva
            newellipse.Move(Wfirstpointclicked.X(),Wfirstpointclicked.Y(),0);
            break;
        }
        default:
            break;
    }

	pos = CLevGeoData::Include(newellipse);

	SetPrimitivePreDiscretizationData(pos,
	ellipse.oPredis.NPoints(),ellipse.oPredis.Factor(),
	o_clickedPoint);

	LocateEllipsesCrosses();
	LocateCrosses();
	TakeLimits();

	//===================================================================================
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//=================================== reengenharia ===================================================
//-----------------------------------------------------------------------------------------------------
//    Include(CLevPoint &ro_point0, CLevPoint &ro_point1)
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  10/2004
//    Motivo:            reengenharia (sobrecargas de Includes sem especificar no nome da função o tipo de primitiva)
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::Include(CLevPoint &ro_point0, CLevPoint &ro_point1, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevLine o_line(&ro_point0, &ro_point1);

    int insertionpos = Include(o_line);

    if(insertionpos >= 0 && verifyCrosses) {
        LocateGivenLineAllEllipsesCrosses(insertionpos);
        LocateLineCrosses(insertionpos);
        IsGeometryModified(lev_enum::YES);
    }
    meshfronteiraOk = false;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionpos;
}


//////////////////////////////////////////////////////////////////////////////////
//int CLevGeoData::Include(CLevEllipse)
//////////////////////////////////////////////////////////////////////////////////
// retorno : -1   : problema na inclusão
//              >=0 : posição da elipse no vetor de primitivas
//////////////////////////////////////////////////////////////////////////////////
int CLevGeoData::Include(CLevEllipse &elipse, bool verifyCrosses)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int insertionposition = -1;
    insertionposition = CLevGeoData::Include(elipse);
    if(verifyCrosses && insertionposition>=0)
    {
        LocateGivenEllipseAllLinesCrosses(insertionposition);
        LocateLineCrosses();
        LocateEllipsesCrosses();
        LocateCrosses();
        TakeLimits();
    }
    meshfronteiraOk = false;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionposition;
}

//////////////////////////////////////////////////////////////////////////////////
//int CLevGeoData::Include(double xc, double yc, double xSemiA, double ySemiA,
//                                   CLevPoint p1, CLevPoint p2, CLevEllipse el)
//////////////////////////////////////////////////////////////////////////////////
// retorno : -1   : problema na inclusão
//              >=0 : posição da elipse no vetor de primitivas
//////////////////////////////////////////////////////////////////////////////////
int CLevGeoData::Include(double xc, double yc, double xSemiA, double ySemiA, double angleIni, double angleFim)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint p1,p2;
    int insertionposition = -1;

    ////////////////////////////////////////////////////////////////////////////
    // vetor de (dois) pontos preenchidos e devolvidos pelo construtor da elipse
    // com as coordenadas dos pontos vértices, calculados a partir dos angulos
    //  inicial e final. Os ponteiros para os pontos vértices
    // da elipse são obtidos após a inclusão dos pontos na lista vPoints
    vCLevPoint vPtVertices;
    vPtVertices.push_back(p1);
    vPtVertices.push_back(p2);

    vpCLevPoint vpPoints;

    //obtem coordenadas dos vertices p1 e p2
    CLevEllipse elipse(xc, yc, xSemiA, ySemiA, angleIni, angleFim, vPtVertices);

    CLevPoint *pp1,*pp2,*pp3,*pp4,*pp5;

    pp1 = new CLevPoint;
    pp2 = new CLevPoint;
    pp3 = new CLevPoint;
    pp4 = new CLevPoint;
    pp5 = new CLevPoint;

    *pp1 = vPtVertices[0];
    *pp2 = vPtVertices[1];

    pp3->X(xc);
    pp3->Y(yc);

    pp4->X(xSemiA);
    pp5->Y(ySemiA);

    vpPoints.push_back(pp1);
    vpPoints.push_back(pp2);
    vpPoints.push_back(pp3);
    vpPoints.push_back(pp4);
    vpPoints.push_back(pp5);

    elipse.SetPoints(vpPoints);

    insertionposition = Include(elipse);

    if(insertionposition>=0)
        LocateGivenEllipseAllLinesCrosses(insertionposition);

    meshfronteiraOk = false;

    if(pp1)
        delete pp1;
    if(pp2)
        delete pp2;
    if(pp3)
        delete pp3;
    if(pp4)
        delete pp4;
    if(pp5)
        delete pp5;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionposition;
}

//-----------------------------------------------------------------------------------------------------
//    IsAIsAPrimitive()
//-----------------------------------------------------------------------------------------------------
//    Descrição:     Verifica se as coordenadas fornecidas estão dentro da zona de influência de uma
//                das linhas armazenadas
//
//    parâmetros:    CLevPoint &screenPoint: coordenadas a serem verificadas
//                int position: posição no vetor de ponteiros de primitivas
//                double deltax, deltay: resolução (determinam a zona de influência dos pontos)
//
//    retorno:    1:     encontrou linha
//                0:    não encontrou linha
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:    Nancy
//    Modificado em:  14/10/2004
//    Motivo:            - Migração para CLevGeoData
//                    - alteração de vLines[i]. para vpPrimitives[i]-> devido à utilização de vetores de
//                        ponteiros de primitivas no lugar de vetor de objetos linhas e elipses
//                    - inclusão de teste para verificar se é uma linha (já que o ponteiro é para primitiva)
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IsAPrimitive(CLevPoint &screenPoint, CLevLine &line, int &position, double deltax, double deltay)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    int retorno = 0;
    int coarse_search;
    double coef_linear, coef_angular;
    double yline;
    CLevLine *pLine;

    for (i = 0; i < vpPrimitives.size(); i++)
    {
        if(vpPrimitives[i]->GeoEntityType() == LINELEV)
        {
            coarse_search = 0;
            if (vpPrimitives[i]->Status())
            {
                if (vpPrimitives[i]->vpPoints[0]->X() <= vpPrimitives[i]->vpPoints[1]->X())
                {
                    if (screenPoint.X() >= vpPrimitives[i]->vpPoints[0]->X() - deltax && screenPoint.X() <= vpPrimitives[i]->vpPoints[1]->X() + deltax)
                    {
                        if (vpPrimitives[i]->vpPoints[0]->Y() <= vpPrimitives[i]->vpPoints[1]->Y())
                        {
                            if (screenPoint.Y() >= vpPrimitives[i]->vpPoints[0]->Y() - deltay && screenPoint.Y() <= vpPrimitives[i]->vpPoints[1]->Y() + deltay)
                                coarse_search = 1;
                        }
                        else
                        {
                                if (screenPoint.Y() >= vpPrimitives[i]->vpPoints[1]->Y() - deltay && screenPoint.Y() <= vpPrimitives[i]->vpPoints[0]->Y() + deltay)
                                    coarse_search = 1;
                        }
                    }
                }
                else
                {
                    if (screenPoint.X() >= vpPrimitives[i]->vpPoints[1]->X()  - deltax && screenPoint.X() <= vpPrimitives[i]->vpPoints[0]->X() + deltax)
                    {
                        if (vpPrimitives[i]->vpPoints[0]->Y() <= vpPrimitives[i]->vpPoints[1]->Y())
                        {
                            if (screenPoint.Y() >= vpPrimitives[i]->vpPoints[0]->Y() - deltay && screenPoint.Y() <= vpPrimitives[i]->vpPoints[1]->Y() + deltay)
                                coarse_search = 1;
                        }
                        else
                        {
                            if (screenPoint.Y() >= vpPrimitives[i]->vpPoints[1]->Y() - deltay && screenPoint.Y() <= vpPrimitives[i]->vpPoints[0]->Y() + deltay)
                                coarse_search = 1;
                        }
                    }
                }
            }
            if (coarse_search)
            {
                double aux = vpPrimitives[i]->vpPoints[0]->X() - vpPrimitives[i]->vpPoints[1]->X();
                if (aux<0)
                    aux = -aux;
                if (aux > deltax)
                {
                    // calcular coeficientes da reta
                    coef_angular = ( vpPrimitives[i]->vpPoints[1]->Y() - vpPrimitives[i]->vpPoints[0]->Y() ) / ( vpPrimitives[i]->vpPoints[1]->X() - vpPrimitives[i]->vpPoints[0]->X() );
                    coef_linear  = vpPrimitives[i]->vpPoints[1]->Y() - coef_angular * vpPrimitives[i]->vpPoints[1]->X();
                    // calcular y da reta para o x do screenPoint
                    yline = coef_angular * screenPoint.X() + coef_linear;
                    // verificar se o ponto da reta coindice com o screenPoint
                    if ( (yline > screenPoint.Y() - deltay) && (yline < screenPoint.Y() + deltay))
                    {
                        if (!retorno)
                        {
                            pLine = (CLevLine *) vpPrimitives[i];
                            line = *pLine;
                            position = i;
                        }
                        retorno++;
                    }
                }
                else
                {
                    if (!retorno)
                    {
                        pLine = (CLevLine *) vpPrimitives[i];
                        line = *pLine;
                        position = i;
                    }
                    retorno++;
                }
            }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return retorno;
}

//-----------------------------------------------------------------------------------------------------
//    IsAPrimitive()
//-----------------------------------------------------------------------------------------------------
//    Descrição:     Identifica todas as elipses cujo ponto está dentro do bounding box, devolve
//            a que tem menor semi-eixo x ou semi-eixo y.
//
//    parâmetros:    CLevPoint &screenPoint: coordenadas a serem verificadas
//                int position: posição no vetor de ponteiros de primitivas
//                double deltax, deltay: resolução (determinam a zona de influência dos pontos)
//
//    retorno:    1:     encontrou elipse
//                0:    não encontrou elipse
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:    Nancy
//    Modificado em:  14/10/2004
//    Motivo:            - Migração para CLevGeoData
//                    - alteração de vEllipse[i]. para vpPrimitives[i]-> devido à utilização de vetores de
//                        ponteiros de primitivas no lugar de vetor de objetos linhas e elipses
//                    - inclusão de teste para verificar se é uma ellipse (já que o ponteiro é para primitiva)
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IsAPrimitive(CLevPoint &screenPoint, CLevEllipse &elipse, int &position, double deltax, double deltay)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    CLevEllipse * pEllipse = NULL;

    std::vector<int> vIdentified;

    for(i = 0; i < vpPrimitives.size(); i++)
        if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
            if (vpPrimitives[i]->Status())
                if(vpPrimitives[i]->IsInBoundingBox(screenPoint))
                    vIdentified.push_back(i);

    if(!vIdentified.empty())
    {
        CLevEllipse o_ellipseAux;
        int            auxPos;

        pEllipse = (CLevEllipse *) vpPrimitives[vIdentified[0]];
        o_ellipseAux = *pEllipse;
        auxPos = vIdentified[0];

        for(i=1; i<vIdentified.size();i++)
            if(vpPrimitives[vIdentified[i]]->vpPoints[3]->X() < o_ellipseAux.vpPoints[3]->X() ||
                vpPrimitives[vIdentified[i]]->vpPoints[4]->Y() < o_ellipseAux.vpPoints[4]->Y())
            {
                pEllipse = (CLevEllipse *) vpPrimitives[vIdentified[i]];
                o_ellipseAux = *pEllipse;
                auxPos = vIdentified[i];
            }

        elipse        = o_ellipseAux;
        position    =  auxPos;

        vIdentified.clear();
		#ifdef __DEBUG_MODE__
		std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
		#endif
        return 1;
    }
    else
    {
		#ifdef __DEBUG_MODE__
		std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
		#endif
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------
//    CreatePoint()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Cria um ponto (CLevPoint)
//
//    parâmetros:
//
//    retorno:    ponteiro para o ponto criado (NULL se o ponto nao tiver sido
//                     criado)
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevPoint* CLevGeoData::CreatePoint(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint * p = NULL;
    p = new  CLevPoint;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return p;
}

//-----------------------------------------------------------------------------------------------------
//    CreateLine()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Cria uma linha (CLevLine)
//
//    parâmetros:
//
//    retorno:    ponteiro para a linha criada
//                (NULL se a linha nao tiver sido
//                     criada)
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevLine* CLevGeoData::CreateLine(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevLine * l = NULL;
    l = new  CLevLine;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return l;
}

//-----------------------------------------------------------------------------------------------------
//    CreateEllipse()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Cria uma ellipse (CLevEllipse)
//
//    parâmetros:
//
//    retorno:    ponteiro para a elipse criada
//                (NULL se a elipse nao tiver sido
//                     criada)
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevEllipse* CLevGeoData::CreateEllipse(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevEllipse * e = NULL;
    e = new CLevEllipse;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return e;
}

//-----------------------------------------------------------------------------------------------------
//    CreateRegion()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Cria uma região fechada do modelo geométrico (CLevRegion)
//
//    parâmetros:
//
//    retorno:    ponteiro para a região criada
//                (NULL se a região nao tiver sido
//                     criada)
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            11/11/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevRegion* CLevGeoData::CreateRegion(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevRegion * r = NULL;
    r = new CLevRegion;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return r;
}

//-----------------------------------------------------------------------------------------------------
//    Include(CLevLine &ro_line)
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Inclui uma linha definida por dois pontos, verificando a existência dos pontos e da
//                linha.
//                A verificação da existência dos pontos leva em consideração a zona de influência dos
//                pontos. As coordenadas dos pontos vértices fornecidos como parâmetro são atualizadas
//                para "sincronizar" os valores.
//                Não faz verificação de intersecção de primitivas.
//
//    parâmetros:    - referencia para um objeto da classe CLevLine
//                - posição de inserção na lista de primitivas (retorno)
//
//    retorno:    -100001:  os dois pontos vértices são iguais
//                -100002:  problema na inclusão do primeiro vertice
//                -100003:  problema na inclusão do segundo vertice
//                -100004:  não conseguiu alocar memória para inclusão de linha
//                >0: inclusão com sucesso, posição na lista de primitivas
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  15/10/2004
//    Motivo:            - Migração para CLevGeoData
//                    - Adaptação para uso de vector de ponteiros para pontos
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::Include(CLevLine &ro_line)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    char         msg[1000];
    unsigned int i;
    int             insertionposition=0;
    bool         isInList = false;
    CLevPoint     p0, p1;
    CLevPoint    *po_point_i = NULL, *po_point_f = NULL;
    CLevLine     o_line;
    CLevLine    *po_line = NULL;

    vpCLevPoint vpPt;

    p0 = *ro_line.vpPoints[0];
    p1 = *ro_line.vpPoints[1];

    if(p0 == p1)
        return -100001;

    po_point_i = Include(p0, GEOM);
    if(!po_point_i)
        return -100002;

    po_point_f = Include(p1, GEOM);
    if(!po_point_f) {
        strcpy(msg, "Line inclusion (ERR100003) ");
        throw lev_math::CMemory_Exception(msg);
        return -100003;
    }

    po_point_i->GeoEntityType(GEOM);
    po_point_f->GeoEntityType(GEOM);

    vpPt.push_back(po_point_i);
    vpPt.push_back(po_point_f);

    o_line.SetPoints(vpPt);
    o_line.oPredis = ro_line.oPredis;

    o_line.IsOfGeometry(o_line.IsOfGeometry());

    for (i = 0; i < vpPrimitives.size() && isInList == false; i++)
        if(vpPrimitives[i]->GeoEntityType() == LINELEV)
            if(o_line == *(CLevLine *) vpPrimitives[i])
            {
                isInList = true;
                insertionposition = i;
            }

    if(!isInList)
    {
        po_line = CreateLine();
        if (!po_line)
        {
            strcpy(msg, "Line inclusion (ERR100004) ");
            throw lev_math::CMemory_Exception(msg);
            return -100004;
        }

        *po_line = o_line;

        po_line->Status(LEV_VALID);
        po_line->GeoEntityType(LINELEV);
        po_line->vpPoints[0]->num_primitives++;
        po_line->vpPoints[1]->num_primitives++;
        po_line->TakeLimits();

        vpPrimitives.push_back(po_line);

        //Atualiza os pontos ro_point0 e ro_point1
        p0 = *po_line->vpPoints[0];
        p1 = *po_line->vpPoints[1];

        insertionposition = vpPrimitives.size()-1;
        TakeLimits();

        IsGeometryModified(lev_enum::YES);
    }

    // *NMA* 31/05/2005: associa ponteiro da linha aos pontos vértices, para facilitar a
    //        verificação dos pontos que são auxiliares (pertencem somente a primitivas
    //        auxiliares)
    CLevPrimitive *pL = GetPrimitive(insertionposition);
    pL->vpPoints[0]->vpPrimitives.push_back(GetPrimitive(insertionposition));
    pL->vpPoints[1]->vpPrimitives.push_back(GetPrimitive(insertionposition));

    vpPt.clear();

    Notify(INCLUDEOPERATION);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionposition;
}

//-----------------------------------------------------------------------------------------------------
//    Include(CLevEllipse &)
//-----------------------------------------------------------------------------------------------------
//    Descrição: Inclui uma elipse na lista de primitivas. Não faz verificação de intersecção.
//
//    parâmetros: referencia a um objeto CLevEllipse
//
//    retorno:
//                -100020: problema indeterminado
//                -100021: problema na inclusão do ponto inicial
//                -100022: problema na inclusão do ponto final
//                -100023: problema na inclusão do ponto central (invisível)
//                -100024: problema na inclusão do ponto do semi-eixo x (invisível)
//                -100025: problema na inclusão do ponto do semi-eixo y (invisível)
//                -100026: problema na inclusão do da elipse
//               >= 0: posição na lista de primitivas
//
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::Include(CLevEllipse & ro_elipse)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    // Códigos de erros a partir de -100020
    unsigned int i;
    bool isInList = false;
    int  insertionposition = -100020;
    char msg[1000];

    CLevPoint  pAux;
    CLevPoint *pi = NULL, *pf = NULL;
    CLevPoint *pc = NULL, *px = NULL, *py = NULL;
    CLevEllipse *paux_ellipse = NULL;
    CLevPoint  p1, p2;
    vpCLevPoint vpPontos_ellipse;

    vpPontos_ellipse.reserve(5);

    // pontos inicial e final
    p1 = *ro_elipse.vpPoints[0];
    p2 = *ro_elipse.vpPoints[1];

    pi = Include(p1, GEOM);
    if(!pi)
    {
        strcpy(msg, "Ellipse inclusion (ERR100021) ");
        throw lev_math::CMemory_Exception(msg);
        return -100021;
    }
    vpPontos_ellipse.push_back(pi);

    pf = Include(p2, GEOM);
    if(!pf)
    {
        strcpy(msg, "Ellipse inclusion (ERR100022) ");
        throw lev_math::CMemory_Exception(msg);
        return -100022;
    }
    vpPontos_ellipse.push_back(pf);

    //ponto central
    pAux = *ro_elipse.vpPoints[2];

    pc = Include(pAux,POINTINVISIBLELEV);

    if(!pc) {
        strcpy(msg, "Ellipse inclusion (ERR100023) ");
        throw lev_math::CMemory_Exception(msg);
        return -100023;
    }

    vpPontos_ellipse.push_back(pc);

    //ponto semi-eixo x
    pAux = *ro_elipse.vpPoints[3];
    pAux.Y(pc->Y()); // para evitar que o ponto tenha as mesmas coordendas do ponto central
    px = Include(pAux,POINTINVISIBLELEV);
    if(!px)
    {
        strcpy(msg, "Ellipse inclusion (ERR100024) ");
        throw lev_math::CMemory_Exception(msg);
        return -100024;
    }
    vpPontos_ellipse.push_back(px);

    // ponto semi-eixo y
    pAux = *ro_elipse.vpPoints[4];
    pAux.X(pc->X()); // para evitar que o ponto tenha as mesmas coordendas do ponto central
    py = Include(pAux, POINTINVISIBLELEV);
    if(!py)
    {
        strcpy(msg, "Ellipse inclusion (ERR100025) ");
        throw lev_math::CMemory_Exception(msg);
        return -100025;
    }
    vpPontos_ellipse.push_back(py);

    pi->GeoEntityType(GEOM);
    pf->GeoEntityType(GEOM);

    CLevEllipse o_ellipse(pi,pf,pc,px,py);

    o_ellipse.oPredis = ro_elipse.oPredis;

    o_ellipse.IsOfGeometry(ro_elipse.IsOfGeometry());

    //verifica se a ellipse está na lista de primitivas
    for(i=0;i<vpPrimitives.size() && isInList == false;i++)
    {
        if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV && vpPrimitives[i]->Status())
        {
            if(o_ellipse == *(CLevEllipse *) vpPrimitives[i])
            {
                isInList = true;
                insertionposition  = i;
            }
        }
    }

    if(!isInList)
    {
        paux_ellipse = CreateEllipse();
        if(!paux_ellipse)
        {
            strcpy(msg, "Ellipse inclusion (ERR100026) ");
            throw lev_math::CMemory_Exception(msg);
            return -100026;
        }

        *paux_ellipse = o_ellipse;

        paux_ellipse->GeoEntityType(ELLIPSELEV);
        paux_ellipse->Status(LEV_VALID);
        paux_ellipse->TakeLimits();
        paux_ellipse->vpPoints[0]->num_primitives++;
        paux_ellipse->vpPoints[0]->num_ellipses++;

        if(pi != pf)
        {
            paux_ellipse->vpPoints[1]->num_primitives++;
            paux_ellipse->vpPoints[0]->num_ellipses++;
        }

        vpPrimitives.push_back(paux_ellipse);
        historykeeper->vpPrimitives.push_back(paux_ellipse);

        insertionposition = vpPrimitives.size()-1;
    }

    IsGeometryModified(lev_enum::YES);
    Notify(INCLUDEOPERATION);

    //*NMA* 31/05/2005: associa ponteiro da linha aos pontos vértices, para facilitar a
    //        verificação dos pontos que são auxiliares (pertencem somente a primitivas
    //        auxiliares)
    CLevPrimitive *pEl = GetPrimitive(insertionposition);
    pEl->vpPoints[0]->vpPrimitives.push_back(GetPrimitive(insertionposition));
    pEl->vpPoints[1]->vpPrimitives.push_back(GetPrimitive(insertionposition));

    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionposition;
}

//--------------------------------------------------------------------------
//    Include(CLevPoint &)
//--------------------------------------------------------------------------
//
//    Descrição: Inclui um ponto na lista de ponteiro de pontos ou na lista de ponteiro de
//                pontos invisíveis
//
//    parâmetros: referencia a um objeto CLevPoint
//
//    retorno:    ponteiro para o ponto incluido: se foi incluído com sucesso :
//                NULL: se ocorreu erro
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Junho/2000
//    Modificado por:    Angelo
//    Modificado em:    19/07/2000
//    Motivo:            Foi incluida uma verificação da pré existencia do ponto na lista:
//    int position = -1;
//    if (!IsAPoint(ro_point, position,0,0))
//    {
//        vPoints.push_back(ro_point);
//        vPoints.back().Status(LEV_VALID); // *NMA*:  vPoints.back() retorna uma referencia ao ultimo elemento da lista
//        po_point = &vPoints[vPoints.size()-1];
//    }
//    else po_point = &vPoints[position];
//    Modificado por:    Nancy
//    Modificado em:    18/10/2001
//    Motivo:            Foi incluido o tipo do ponto, parâmetro default
//    Modificado por:    Nancy
//    Modificado em:    13/10/2004
//    Motivo:            - Migração para a classe CLevGeoData
//                    - Inclusão de ponteiros para pontos nas listas
//-----------------------------------------------------------------------------------------------------
CLevPoint * CLevGeoData::Include(CLevPoint &ro_point, GEOENTITYTYPE type)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint * po_point = NULL;
    int position = -1;
    double deltax, deltay;

    if(type == POINTINVISIBLELEV)
    {

        po_point = CreatePoint();
        *po_point = ro_point;

        po_point->Status(LEV_VALID);
        po_point->GeoEntityType(type);

        vpInvisiblePoints.push_back(po_point);

        historykeeper->vpInvisiblePoints.push_back(po_point);

    }
    else
    {

        Resolution(ro_point, deltax, deltay);

        if (!IsAPoint(ro_point, position,deltax,deltay))
        {
            po_point = CreatePoint();
            *po_point = ro_point;

            po_point->Status(LEV_VALID);
            po_point->GeoEntityType(type);

            vpPoints.push_back(po_point);

            historykeeper->vpPoints.push_back(po_point);
        } else
            po_point = vpPoints[position];

        if(ro_point.X() < original.GetXMin())
            original.SetXMin(ro_point.X());
        else if(ro_point.X() > original.GetXMax())
            original.SetXMax(ro_point.X());

        if(ro_point.Y() < original.GetYMin())
            original.SetYMin(ro_point.Y());
        else if(ro_point.Y() > original.GetYMax())
            original.SetYMax(ro_point.Y());

        IsGeometryModified(lev_enum::YES);
    }
    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return po_point;
}

//--------------------------------------------------------------------------
//    Resolution
//--------------------------------------------------------------------------
//
//    Descrição: Determina a resolução deltax e deltay em que dois pontos
//               podem ser considerados o mesmo ponto (zona de influência dos
//             pontos)
//
//    parâmetros:
//
//    retorno:
//
//    Observações: Necessita ser revisado
//
//..........................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            13/10/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//--------------------------------------------------------------------------
void CLevGeoData::Resolution(CLevPoint o_point, double &deltax, double &deltay)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    bool continua = true;
    unsigned int i;
    double dx, dy;
    double xmax=0, xmin=0, ymax=0, ymin=0;

    if(GetNumValidPoints() >= 2) {
        // *NMA*: inicializa variaveis com os primeiros valores validos
        for(i=0; i<vpPoints.size() && continua; i++)
        {
            if(vpPoints[i]->Status() )
            {
                xmin = vpPoints[i]->X();
                xmax = vpPoints[i]->X();
                ymin = vpPoints[i]->Y();
                ymax = vpPoints[i]->Y();
                continua = false;
            }
        }

        //encontra os valores maximos e minimos
        for(i=0; i<vpPoints.size(); i++)
        {
            if(vpPoints[i]->Status() && vpPoints[i]->X() < xmin)
                xmin = vpPoints[i]->X();
            if(vpPoints[i]->Status() && vpPoints[i]->X() > xmax)
                xmax = vpPoints[i]->X();
            if(vpPoints[i]->Status() && vpPoints[i]->Y() < ymin)
                ymin = vpPoints[i]->Y();
            if(vpPoints[i]->Status() && vpPoints[i]->Y() > ymax)
                ymax = vpPoints[i]->Y();
        }

        dx = xmax * 1e-4;
        dy = ymax * 1e-4;

        deltax = fabs(dx);
        deltay = fabs(dy);
    }
    else
    {
        deltax = 0;
        deltay = 0;
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    IsAPoint()
//-----------------------------------------------------------------------------------------------------
//    Descrição:     Verifica se um ponto com as coordenadas fornecidas está dentro da zona de influência
//                de um dos pontos já incluidos
//
//    parâmetros:    CLevPoint &screenPoint: coordenadas a serem verificadas
//                int position: posição no vetor de ponteiros de pontos
//                double deltax, deltay: resolução (determinam a zona de influência do ponto)
//
//    retorno:    1:     encontrou ponto
//                0: não encontrou ponto
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:    Nancy
//    Modificado em:  14/10/2004
//    Motivo:            - Migração para CLevGeoData
//                    - alteração dos operadores de acesso devido à utilização de vetores de
//                        ponteiros de pontos no lugar de vetor de objetos ponto
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IsAPoint(CLevPoint &screenPoint, int &position, double deltax, double deltay)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    for (i = 0; i < vpPoints.size(); i++)
    {
        if (vpPoints[i]->Status())
        {
            if ((screenPoint.X() >= vpPoints[i]->X() - deltax) && (screenPoint.X() <= vpPoints[i]->X() + deltax))
            {
                if ((screenPoint.Y() >= vpPoints[i]->Y() - deltay) && (screenPoint.Y() <= vpPoints[i]->Y() + deltay))
                {
                    screenPoint = *vpPoints[i];
                    position = i;
                    return 1;
                }
            }
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    GetDomainSize()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Atualiza e retorna os limites do domínio
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  14/10/2004
//    Motivo:            Migração para CLevGeoData e retirei verificação se existem pontos válidos
//-----------------------------------------------------------------------------------------------------
CWorld_Window CLevGeoData::GetDomainSize(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    TakeLimits();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return original;
}

//-----------------------------------------------------------------------------------------------------
//    TakeLimits()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Atualiza os limites do domínio em original
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::TakeLimits(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    bool    existemPontos = false;
    CLevPoint op_Min, op_Max;

    CWorld_Window original_aux; //para guardar os limites atuais

    original_aux.SetXMin(original.GetXMin());
    original_aux.SetXMax(original.GetXMax());
    original_aux.SetYMin(original.GetYMin());
    original_aux.SetYMax(original.GetYMax());

    // valores alterados para permitir perceber um domínio menor do que o anterior
    if(vpPoints.size())
    {
        if (vpPoints[0]->Status())
        {
            original.SetXMin(vpPoints[0]->X());
            original.SetYMin(vpPoints[0]->Y());
            original.SetXMax(vpPoints[0]->X());
            original.SetYMax(vpPoints[0]->Y());
            existemPontos = true;
        }
        else
        {
            original.SetXMin(-1);
            original.SetYMin(-1);
            original.SetXMax(1);
            original.SetYMax(1);
        }

        for (i = 1; i < vpPoints.size(); i++)
        {
            if (vpPoints[i]->Status())
            {
                // *NMA*:Verifica max e min para permitir perceber um domínio menor do que o anterior

                if (vpPoints[i]->X() <= original.GetXMin()) original.SetXMin(vpPoints[i]->X());
                if (vpPoints[i]->X() >= original.GetXMax()) original.SetXMax(vpPoints[i]->X());

                if (vpPoints[i]->Y() <= original.GetYMin()) original.SetYMin(vpPoints[i]->Y());
                if (vpPoints[i]->Y() >= original.GetYMax()) original.SetYMax(vpPoints[i]->Y());
            }
            existemPontos = true;
        }

        for (i = 0; i < vpPrimitives.size(); i++)
        {
            if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
            {
                if (vpPrimitives[i]->Status())
                {
                    vpPrimitives[i]->Limits(op_Min, op_Max);

                    if (op_Min.X() <= original.GetXMin()) original.SetXMin(op_Min.X());
                    if (op_Max.X() >= original.GetXMax()) original.SetXMax(op_Max.X());

                    if (op_Min.Y() <= original.GetYMin()) original.SetYMin(op_Min.Y());
                    if (op_Max.Y() >= original.GetYMax()) original.SetYMax(op_Max.Y());
                }
            }
        }

        if(!existemPontos)
        {
            original.SetXMin(-1);
            original.SetYMin(-1);

            original.SetXMax(1);
            original.SetYMax(1);
        }
    }//close if(vpPoints.size())
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    IsARegion()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Verifica se um ponto está dentro de alguma das regiões fechadas do modelo geométrico
//
//    parâmetros:    CLevPoint &o_point: endereço de um obejto da classe CLevPoint
//
//    retorno:     -1: não encontrou região que contenha o ponto
//                >=0: número da região encontrada
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            11/11/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IsARegion(CLevPoint &o_point)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i, j;
    int            index = -1;
    bool        found = false;
    vInt        vElemRegion;
    Pt_Malha    o_Pt;
    Elemento_2D  o_el;

    o_Pt.X(o_point.X());
    o_Pt.Y(o_point.Y());

    for (i = 0; i < vpModelRegions.size() && (found == false); i++)
    {
        if(vpModelRegions[i]->Status())
        {
            vpModelRegions[i]->GetElementsRegion(vElemRegion);

            for(j=0;j<vElemRegion.size() && (found == false);j++)
            {
                o_el = vElementos_fronteira[vElemRegion[j]];

                if(o_el.Is_Point_Inside(vPontos_fronteira, o_Pt))
                {
                    index = i;
                    found = true;
                }
            }
        }
    }

    vElemRegion.clear();
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return index;
}

//-----------------------------------------------------------------------------------------------------
//    IsInside()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Verifica se um ponto com coordenadas (x,y) está dentro dos limites do modelo geométrico
//
//    parâmetros:    double x
//                double y
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Outubro/2004
//    Modificado por: Nancy
//    Modificado em:  Novembro/2004
//    Motivo:            Migração para CLevGeoData (de CLevGeoData)
//-----------------------------------------------------------------------------------------------------
bool CLevGeoData::IsInside(double x, double y)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint    point;
    bool        codeRet = false;
    int            indexRegion = -1;

    point.X(x);
    point.Y(y);

    indexRegion = IsARegion(point);

    if(indexRegion != -1)
        codeRet = true;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return codeRet;
}

double CLevGeoData::GetXMin(void)
{
    return original.GetXMin();
}

double CLevGeoData::GetXMax(void)
{
    return original.GetXMax();
}

double CLevGeoData::GetYMin(void)
{
    return original.GetYMin();
}

double CLevGeoData::GetYMax(void)
{
    return original.GetYMax();
}

void CLevGeoData::SetXMin(double k)
{
    original.SetXMin(k);
}

void CLevGeoData::SetXMax(double l)
{
    original.SetXMax(l);
}

void CLevGeoData::SetYMin(double m)
{
    original.SetYMin(m);
}

void CLevGeoData::SetYMax(double n)
{
    original.SetYMax(n);
}

//-----------------------------------------------------------------------------------------------------
//    GetPrimitive()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Retorna ponteiro para primitiva da posição pedida
//
//    parâmetros:    int ind: indice da primitiva na lista de primtivas
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembo/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevPrimitive* CLevGeoData::GetPrimitive(int ind)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPrimitive *p = NULL;

    if(ind>=0 && ind < (signed)vpPrimitives.size())
        p = vpPrimitives[ind];

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return p;
}

//-----------------------------------------------------------------------------------------------------
//    GetPoint()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Retorna ponteiro para o ponto da posição pedida
//
//    parâmetros:    int ind: indice do ponto na lista de pontos
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembo/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevPoint* CLevGeoData::GetPoint(int ind)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    CLevPoint *p = NULL;

    if(ind>=0 && ind < (signed)vpPoints.size())
        p = vpPoints[ind];

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return p;
}

//-----------------------------------------------------------------------------------------------------
//    GetNumPrimitives()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Retorna número de primitivas armazenadas na lista de ponteiros para primitivas
//    parâmetros:
//    retorno:    int
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembo/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::GetNumPrimitives(void)
{
    int num = vpPrimitives.size();
    return num;
}

//-----------------------------------------------------------------------------------------------------
//    GetNumLines()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Retorna número de linhas armazenadas na lista de ponteiros para primitivas
//    parâmetros:
//    retorno:    int
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembo/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::GetNumLines(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int num_lines = 0;

    for(i=0;i<vpPrimitives.size();i++)
        if(vpPrimitives[i]->GeoEntityType() == LINELEV) num_lines++;

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return num_lines;
}

int CLevGeoData::GetNumValidEllipses(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int num_valid_ellipses = 0;

    for(i=0;i<vpPrimitives.size();i++)
        if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
            num_valid_ellipses++;

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return num_valid_ellipses;
}

//-----------------------------------------------------------------------------------------------------
//    GetNumPoints()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição: Retorna número de pontos armazenados na lista de ponteiros para pontos
//    parâmetros:
//    retorno:    int
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembo/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::GetNumPoints(void)
{
    int num = vpPoints.size();
    return num;
}

//-----------------------------------------------------------------------------------------------------
//    ReadGeometricModel()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Leitura dos dados de primitivas e pre-discretização
//
//    parâmetros:
//                std::string nomeMGP:  nome do arquivo com extensão .mgp com path completo
//
//    retorno:
//                0    - sucesso
//                101 - erro na abertura do arquivo
//                102 - arquivo com primitiva não suportada por esta versão
//                102 - erro na inclusão de linha
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy Mieko Abe
//    Modificado em:    Setembro/2001
//    Motivo:            Implementação em Lev077897
//    Modificado por: Nancy Mieko Abe
//    Modificado em:    Fevereiro/2002
//    Motivo:            Leitura do dados usando as funções de leitura dos próprios objetos
//    Modificado em:    Novembro/2004
//    Motivo:            Migração para CLevGeoData, alteração de nome (antiga ReadMGP) e alteração do
//                    parâmetro de CString para std::string, para ser usado em outros módulos que não
//                    usam MFC
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::ReadGeometricModel(std::string nomeMGP)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    std::string        versao,tipoPrimitiva ;
    char        vers[500],tp[500];
    char        simetria ;
    int            unidade, indice_posicao ;
    int            retorno = 1;
    char        c;
    CLevPoint    p0,p1;
    FILE        *parqMGP = NULL;
    double        x_min, y_min,x_max, y_max;

    char        reta[]   ="reta-segmento";
    char        reta_aux[]  ="reta-segmento_auxiliar";
    char        elipse[] ="elipse";
    char        circ[]     ="circunferencia";
    char        elipse_aux[] ="elipse_auxiliar";
    char        circ_aux[]     ="circunferencia_auxiliar";

    parqMGP = fopen(nomeMGP.c_str(),"r");
    if (!parqMGP)
    {
        if(parqMGP) fclose(parqMGP);
        return 101;
    }
    else
    {
        fscanf(parqMGP, "%s", &vers[0]);
        fscanf(parqMGP, "%lg %lg %lg %lg",  &x_min, &y_min, &x_max, &y_max);
        fscanf(parqMGP, "%d %c", &unidade, &simetria);

        original.SetXMin(x_min);
        original.SetYMin(y_min);
        original.SetXMax(x_max);
        original.SetYMax(y_max);

        pow(original.GetXMin(), -unidade);
        pow(original.GetXMax(), -unidade);
        pow(original.GetYMin(), -unidade);
        pow(original.GetYMax(), -unidade);

        c=getc(parqMGP);

        while(c!=EOF && retorno >= 0)
        {
            ungetc(c,parqMGP);
            fscanf(parqMGP,"%s %d",&tp[0],&indice_posicao);

            if(!strcmp(tp,reta) || !strcmp(tp,reta_aux)) // inclui linhas
            {
                CLevLine    oline;
                CLevPoint    p1,p2;

                //*NMA*: O vetor de ponteiros para pontos da linha deve
                //        estar preenchido antes da leitura, para que possa
                //        ser alterado pelo próprio objeto com as coordenadas lidas do arquivo.
                vpCLevPoint vpPt;
                vpPt.reserve(2);
                vpPt.push_back(&p1);
                vpPt.push_back(&p2);

                oline.SetPoints(vpPt);

                oline.Read(parqMGP,unidade,tp,MODEL_GEOMETRY_PREDIS);

                retorno = Include(oline);

            }
            else
            {
                if(!strcmp(tp,circ) || !strcmp(tp,elipse) || !strcmp(tp,circ_aux) || !strcmp(tp,elipse_aux) )
                {// inclui elipse ou circunferencia
                    CLevEllipse ellipse;
                    CLevPoint    p1,p2,pCentral,pAxisX,pAxisY;

                    //*NMA*: O vetor de ponteiros para pontos da elipse deve
                    //        estar preenchido antes da leitura, para que possa
                    //        somente ser alterado pela própria elipse.
                    vpCLevPoint vpPt;
                    vpPt.reserve(5);
                    vpPt.push_back(&p1);
                    vpPt.push_back(&p2);
                    vpPt.push_back(&pCentral);
                    vpPt.push_back(&pAxisX);
                    vpPt.push_back(&pAxisY);

                    ellipse.SetPoints(vpPt);

                    ellipse.Read(parqMGP,unidade,tp,MODEL_GEOMETRY_PREDIS);

                    retorno = Include(ellipse);

                }//fecha if(!strcmp(tp,circ) || !strcmp(tp,elipse))

            }//fecha else

            c=getc(parqMGP);

        }//fecha while(c!=EOF && retorno >= 0)

        fclose(parqMGP);

    }//fecha else de if (!parqMGP)

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;//retorno;
}

//-----------------------------------------------------------------------------------------------------
//    ReadModelRegions()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Leitura dos dados de regiões fechadas do modelo geométrico
//
//    parâmetros:
//                std::string fileName:  nome do arquivo com extensão .mgp com path completo
//
//    retorno:
//                0    - sucesso
//                101 - erro na abertura do arquivo
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Junho/2000
//    Modificado por: Nancy Mieko Abe
//    Modificado em:    Novembro/2004
//    Motivo:            Migração para CLevGeoData de CLevGeoPhys,  alteração do
//                    parâmetro de CString para std::string, para ser usado em outros módulos que não
//                    usam MFC
//-----------------------------------------------------------------------------------------------------
int    CLevGeoData::ReadModelRegions(std::string fileName)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int        i,j;
    int        num_reg=0;
    int        num_lines=0, num_el=0;// para manter compatibilidade com arquivos .reg
    int        num_primitives = 0;
    int        primitive;
    long    ind_mat, ind_source;
    vInt    vPrim;
    vLong    vIndex;
    vInt    vPrimType;
    FILE *  parq;

    CLevRegion oRegionTemp;

    int                ind;
    vInt            vElFrontInd;
    int                prim;
    CLevPrimitive*    pPrim=NULL;

    num_primitives = vpPrimitives.size();

    //*NMA*: tenta abrir arquivo com extensão .rgn (formato novo)
    parq = fopen(fileName.c_str(),"r");

    if(parq) // se o arquivo .rgn existe, faz a leitura
    {
        fscanf(parq,"%d",&num_reg);

        for(i=0;i<num_reg;i++)
        {
            fscanf(parq,"\n\n%ld %ld",&ind_mat, &ind_source);

            oRegionTemp.Status(LEV_VALID);
            oRegionTemp.SetMaterial(ind_mat);
            oRegionTemp.SetSource(ind_source);

            //*NMA*:22/02/2005: para manter compatibilidade com a aplicação sem interface usado na otimização
            fscanf(parq,"\n\n%d %d",&num_lines, &num_el);

            for(j=0;j<num_lines;j++)
            {
                fscanf(parq,"\n%d", &prim);

                if(prim >= 0  && prim < (signed)vpPrimitives.size())
                {
                    pPrim = GetPrimitive(prim);

                    if(pPrim)
                    {
                        oRegionTemp.IncludeNextPrimitive(pPrim);
                    }
                }
            }

            for(j=0;j<num_el;j++)
            {
                fscanf(parq,"\n%d", &prim);

                // *NMA*: para evitar erro na leitura dos estudos novos
                // pois o numero da ellipse
                // gravado é o Id() que é numerado após as linhas incluindo o total de linhas
                // Isto deve ser mudado
                //prim = prim - numValidLines;
                if(prim >= 0  && prim < (signed)vpPrimitives.size())
                {
                    pPrim = GetPrimitive(prim);

                    if(pPrim)
                    {
                        oRegionTemp.IncludeNextPrimitive(pPrim);
                    }
                }

            }

            // so para versão 1
            //le indice dos elementos da malha de fronteira
            vElFrontInd.clear();
            fscanf(parq,"\n\n%d", &num_el);
            for(j=0;j<num_el;j++)
            {
                fscanf(parq,"\n%d", &ind);
                vElFrontInd.push_back(ind);
            }
            oRegionTemp.IncludeElementsRegion(vElFrontInd);

            IncludeRegion(oRegionTemp);

            oRegionTemp.Reinitialize();
        }
    }
    else //se não existe, tenta abrir arquivo com extensão antiga (.reg)
    {
        int x = fileName.find(".rgn");
        fileName.replace(x, 4, ".rgn");

        parq = fopen(fileName.c_str(),"r");

        if (!parq)
        {
            if(parq) fclose(parq);
            return 107;
        }
        else //conseguiu abrir arquivo .reg
        {
            fscanf(parq,"%d",&num_reg);

            for(i=0;i<num_reg;i++)
            {
                fscanf(parq,"\n\n%ld %ld",&ind_mat, &ind_source);

                oRegionTemp.Status(LEV_VALID);
                oRegionTemp.SetMaterial(ind_mat);
                oRegionTemp.SetSource(ind_source);

                fscanf(parq,"\n\n%d %d",&num_lines, &num_el);

                for(j=0;j<num_lines;j++)
                {
                    fscanf(parq,"\n%d", &primitive);
                    oRegionTemp.IncludeNextPrimitive(GetPrimitive(primitive));
                }

                for(j=0;j<num_el;j++)
                {
                    fscanf(parq,"\n%d", &primitive);

                    // *NMA*: para evitar erro na leitura dos estudos novos
                    // pois o numero da ellipse
                    // gravado é o Id() que é numerado após as linhas incluindo o total de linhas
                    // Isto deve ser mudado
//                    prim = prim - numValidLines;
//                    oRegionTemp.IncludeNextEllipse(GetEllipse(prim));
                    //Obs: para manter a consistencia com arquivos antigos (.reg), a ordem de gravação das
                    // primitivas continuará:primeiro as linhas e depois as elipses, os Ids, na nova versão
                    // são da lista de primitivas a qual inclui linhas e elipses
                    //*NMA*:22/11/2004: acho que o comentário anterior não tem sentido, pois não importa a
                    // sequencia de gravação, o problema, na verdade, é o indice prim que teria que ser corrigido
                    // por causa do uso de dois vetores para armazenamento de linhas e elipses.
                    // Na gravação dos arquivos, o índice das elipses vem depois do índice das linhas em sequencia
                    oRegionTemp.IncludeNextPrimitive(GetPrimitive(primitive));
                }

                IncludeRegion(oRegionTemp);
                oRegionTemp.Reinitialize();

            } //fecha for(i=0;i<num_reg;i++)
        } // fecha else //conseguiu abrir arquivo .reg
    }//fecha else //se não existe, tenta abrir arquivo com extensão antiga (.reg)

    num_reg = vpModelRegions.size();

    fclose(parq);

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    ReadModelMeshFrontier()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     lê dados da malha de fronteira para identificação de regiões fechadas do modelo
//                geométrico.
//
//    parâmetros:    CString fileName: nome do arquivo (.mmf) com path completo.
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Maio/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::ReadModelMeshFrontier(std::string filename)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    FILE        *parq = NULL;
    long int    i,j, num_elem_fronteira, num_pontos_fronteira, num_geom_fronteira;
    Elemento_2D triangulo;
    long        vert[3], viz[3];
    double        x=0.,y=0.;
    double        volume;

    Aresta_Geometria aresta;

    Pt_Malha ponto;


    vElementos_fronteira.clear();
    vArestasGeometria_fronteira.clear();
    vPontos_fronteira.clear();

    parq = fopen(filename.c_str(),"r");
    if (!parq)
    {
        if(parq) fclose(parq);
        return 200;
    }
    else
    {
        //le dados dos elementos
        fscanf(parq,"%ld",&num_elem_fronteira);
        vElementos_fronteira.reserve(num_elem_fronteira);

        for(i=0;i<num_elem_fronteira;i++)
        {
            triangulo.Set_N_Vizinhos(0);
            for (j=0; j<3; j++) triangulo.Set_Vizinho(0);
            triangulo.Set_N_Vizinhos(0);

            fscanf(parq, "%ld %ld %ld %ld %ld %ld %lf", &vert[0], &vert[1], &vert[2],
                                             &viz[0], &viz[1], &viz[2], &volume);
            triangulo.Set_Vertices(vert);
            triangulo.Set_Volume(volume);

            for (j=0; j<3; j++)
            {
                if (viz[j]>=0)
                    triangulo.Set_Vizinho(viz[j]);
            }

            vElementos_fronteira.push_back(triangulo);

        } // Fecha for elementos

        //le dados das arestas de geometria
        fscanf(parq,"%ld",&num_geom_fronteira);
        vArestasGeometria_fronteira.reserve(num_geom_fronteira);

        for(i=0;i<num_geom_fronteira;i++)
        {
            fscanf(parq,"%d %d %d %d",&aresta.g1,&aresta.g2, &aresta.prim, &aresta.primType);
            vArestasGeometria_fronteira.push_back(aresta);
        }

        // dados dos pontos
        fscanf(parq,"%ld",&num_pontos_fronteira);
        vPontos_fronteira.reserve(num_pontos_fronteira);

        for(i=0;i<num_pontos_fronteira;i++)
        {
            fscanf(parq,"%lf %lf\n",&x,&y);
            ponto.X(x);
            ponto.Y(y);
            vPontos_fronteira.push_back(ponto);
        }


        fclose(parq);
    }//fecha else: conseguiu abrir arquivo

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

//-----------------------------------------------------------------------------------------------------
//    Include(CLevRegion&)
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Inclusão de região (CLevRegion) na lista de ponteiros para regiões do modelo
//
//    parâmetros:
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Setembro/2002
//    Modificado por: Nancy
//    Modificado em:  Novembro/2004
//    Motivo:            Migração para CLevGeoData e adaptação para uso de ponteiros para região
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::IncludeRegion(CLevRegion &ro_region)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long   i;
    int                insertionposition = -1;
    bool            found = false, continua = true;
    int                ind =0;
    vInt            vElementsRegion;
    vpCLevRegion    vpRegionsAuxs;
    int                numPrimAux, numPrimInclude;
    CLevRegion        *po_region;
    int                num_regions, num_regions_keeper;

    num_regions = vpModelRegions.size();
    num_regions_keeper = historykeeper->vpModelRegions.size();

    for (i = 0; i < vpModelRegions.size() && (found == false); i++)
    {
        if (ro_region == *(vpModelRegions[i]))
        {  // a comparação é feita por conteudo
            if(vpModelRegions[i]->Status() ==  LEV_INVALID)
                vpModelRegions[i]->Status(LEV_VALID);
            ro_region.GetElementsRegion(vElementsRegion);
            vpModelRegions[i]->IncludeElementsRegion(vElementsRegion);
            insertionposition = i;
            found = true;   // já existe a regiao na lista
        }
    }

    if(!found)
    {
        po_region = CreateRegion();
        *po_region = ro_region;
        po_region->TakeLimits();

        //inclui já em ordem decrescente do número de primitivas
        for(i=0;i<vpModelRegions.size();i++)
            vpRegionsAuxs.push_back(vpModelRegions[i]);

        vpModelRegions.clear();

        numPrimInclude    = po_region->NumValidPrimitives();

        continua = true;
        for(i=0;i<vpRegionsAuxs.size() && continua;i++)
        {
            numPrimAux    = vpRegionsAuxs[i]->NumValidPrimitives();
            if(numPrimAux > numPrimInclude)
                vpModelRegions.push_back(vpRegionsAuxs[i]); //*NMA*: estes ponteiros já foram armazeandos no keeper
            else {
                continua = false;
                ind = i;
            }
        }

        po_region->Status(LEV_VALID);

        vpModelRegions.push_back(po_region);
        historykeeper->vpModelRegions.push_back(po_region);

        insertionposition = vpModelRegions.size()-1;

        num_regions = vpModelRegions.size();
        num_regions_keeper = historykeeper->vpModelRegions.size();

        if(!continua)
            for(i=ind;i<vpRegionsAuxs.size();i++)
                vpModelRegions.push_back(vpRegionsAuxs[i]);//*NMA*: estes ponteiros já foram armazeandos no keeper
    }

    vElementsRegion.clear();
    vpRegionsAuxs.clear();

    num_regions = vpModelRegions.size();
    num_regions_keeper = historykeeper->vpModelRegions.size();

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return insertionposition;
}

CLevRegion * CLevGeoData::GetRegion(int ind)
{
    return vpModelRegions[ind];
}

int CLevGeoData::GetNumRegions(void)
{
    return vpModelRegions.size();
}

int CLevGeoData::GetNumValidRegions(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    int num = 0;

    for(i=0;i<vpModelRegions.size();i++)
        if(vpModelRegions[i]->Status()) num++;

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
	return num;
}

//-----------------------------------------------------------------------------------------------------
//    GetPosition(CLevPoint *)
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Retorna posição do ponteiro para o ponto no vetor de ponteiros para pontos
//
//    parâmetros:    ponteiro do ponto
//
//    retorno:    >=0  : posição
//                = -1 : não encontrou ponteiro na lista
//
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembro/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::GetPosition(CLevPoint *p)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;
    int pos;

    pos = -1;

    for(i=0; i< vpPoints.size(); i++)
        if(p == vpPoints[i])
        {
            pos = i;
            break;
        }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//-----------------------------------------------------------------------------------------------------
//    GetPosition(CLevPrimitive *)
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Retorna posição do ponteiro para a primitiva no vetor de ponteiros para pontos
//
//    parâmetros:    ponteiro do ponto
//
//    retorno:    >=0  : posição
//                = -1 : não encontrou ponteiro na lista
//
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Novembro/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::GetPosition(CLevPrimitive *p)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    int pos;

    pos = -1;

    for(i=0; i< vpPrimitives.size(); i++)
        if(p == vpPrimitives[i])
        {
            pos = i;
            break;
        }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return pos;
}

//-----------------------------------------------------------------------------------------------------
//    DeletePoint()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Elimina ponto tornando o inválido (a eliminação completa tornaria o Undo mais complexo).
//                Torna inválida também a primitiva associada e o outro ponto vértice da primitiva, caso
//                este não seja ponto vértice de nenhuma outra primitiva
//
//    parâmetros:    posição na lista de ponteiros para pontos
//
//    retorno:
//
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//                    Anna Carolina Sabag Volpi (adaptado de SystemPOI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  Novembro/2004
//    Motivo:            Migração para CLevGeoData e adaptação a nova estrutura de dados
//-----------------------------------------------------------------------------------------------------
int  CLevGeoData::DeletePoint(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    int retorno                = 0;
    int primitivesdeleted    = 0;
    CLevPrimitive            *pPrimDebugLocal;

    int num_primitives        = vpPoints[pos]->num_primitives;
    CLevPoint *  pPoint     = vpPoints[pos];

    //varre a lista de Grupos em busca dos grupos que utilizam o ponto eliminado
    for(i=0;i<vGroups.size();i++)
        if(vGroups[i].IsAPoint(vpPoints[pos])!= -1)
            vGroups[i].Status(LEV_INVALID);

    //*NMA*: debug ---------------------------------
    pPrimDebug = vpPrimitives[2];
    pPointDebug = vpPoints[2];
    //----------------------------------------------

    //procura as primitivas ligadas ao ponto e as deleta
    for (i = 0; i < vpPrimitives.size(); i++) {
        pPrimDebugLocal = vpPrimitives[i];

        if (pPoint == vpPrimitives[i]->vpPoints[0]) {
            primitivesdeleted++;
            DeletePrimitive(i);
        } else if (pPoint == vpPrimitives[i]->vpPoints[1]) {
            primitivesdeleted++;
            DeletePrimitive(i);
        }
    }

    vpPoints[pos]->Status(LEV_INVALID);

    isGeometryModified    = lev_enum::YES;
    meshfronteiraOk     = false;

    if(primitivesdeleted != num_primitives)
        retorno = -1; // modelo corrompido, o número de primitivas deletadas não confere com o número de primitivas inicialmente ligadas ao ponto

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return retorno;
}

//-----------------------------------------------------------------------------------------------------
//    DeletePrimitive()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     Elimina primitiva tornanda a inválido (a eliminação completa tornaria o Undo mais complexo).
//                Torna inválida também os vertices da primitiva, caso
//                estes não sejam pontos vértices de nenhuma outra primitiva
//
//    parâmetros:    posição na lista de ponteiros para primitivas
//
//    retorno:
//
//    Observações:
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por: Nancy
//    Modificado em:  Novembro/2004
//    Motivo: Migração para CLevGeoData e adaptação a nova estrutura de dados
//-----------------------------------------------------------------------------------------------------
void CLevGeoData::DeletePrimitive(int pos)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    CLevPoint *pPointDebugLocal;

    //varre a lista de Grupos em busca dos grupos que utilizam a primitiva eliminada
    for(i=0;i<vGroups.size();i++)
        if(vGroups[i].IsAPrimitive(vpPrimitives[pos]) != -1)
            vGroups[i].Status(LEV_INVALID);

    //varre a lista de Regioes em busca das regioes que utilizam a linha eliminada
    for(i=0;i<vpModelRegions.size();i++)
        if(vpModelRegions[i]->IsAPrimitive(vpPrimitives[pos]) != -1)
            vpModelRegions[i]->Status(LEV_INVALID);

    //Retira regiões inválidas da lista de regiões
    CleanRegionsVector();

    //no update de physdata, varre os materiais e fontes em uso  e verifica se devem ser mantidas ou não

    Notify(DELETEOPERATION,vpPrimitives[pos]);

    if(vpPrimitives[pos]->vpPoints[0] != vpPrimitives[pos]->vpPoints[1])
    {
        //-------------- debug --------------------------
        pPointDebugLocal = vpPrimitives[pos]->vpPoints[0];
        //-------------- debug --------------------------

        vpPrimitives[pos]->vpPoints[0]->num_primitives--;
        if(vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
            vpPrimitives[pos]->vpPoints[0]->num_ellipses--;

        if (!vpPrimitives[pos]->vpPoints[0]->num_primitives)
            vpPrimitives[pos]->vpPoints[0]->Status(LEV_INVALID);

        //-------------- debug --------------------------
        pPointDebugLocal = vpPrimitives[pos]->vpPoints[1];
        //-------------- debug --------------------------

        vpPrimitives[pos]->vpPoints[1]->num_primitives--;
        if(vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
            vpPrimitives[pos]->vpPoints[1]->num_ellipses--;

        if (!vpPrimitives[pos]->vpPoints[1]->num_primitives)
            vpPrimitives[pos]->vpPoints[1]->Status(LEV_INVALID);
    }
    else
    {    //isto vale para elipses completas
        pPointDebugLocal = vpPrimitives[pos]->vpPoints[0];

        vpPrimitives[pos]->vpPoints[0]->num_primitives--;
        if(vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
            vpPrimitives[pos]->vpPoints[0]->num_ellipses--;

        if (!vpPrimitives[pos]->vpPoints[0]->num_primitives)
            vpPrimitives[pos]->vpPoints[0]->Status(LEV_INVALID);
    }

    if(vpPrimitives[pos]->GeoEntityType() == ELLIPSELEV)
        vpPrimitives[pos]->vpPoints[2]->Status(LEV_INVALID);

    vpPrimitives[pos]->Status(LEV_INVALID);

    isGeometryModified    = lev_enum::YES;
    meshfronteiraOk     = false;

    // aqui deve entrar a chamada a Notify(): malha estruturada, propriedades físicas (boundary conditions), ...
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

//-----------------------------------------------------------------------------------------------------
//    ReadReadStruturedMeshData()
//-----------------------------------------------------------------------------------------------------
//
//    Descrição:     lê arquivo de dados da malha de estruturada para geração de malha de Dealaunay (conversão)
//
//    parâmetros:    CString fileName: nome do arquivo (.mmf) com path completo.
//
//    retorno:
//
//    Observações:
//
//......................................................................................................
//    Versão:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Dexembro/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGeoData::ReadStruturedMeshData(std::string filename)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    FILE        *parq = NULL;
    long int    i,j;
    double        x=0.,y=0.;
    std::vector<double> vPressao;
    double        lixo, pressao;
    CLevPoint   point;

    parq = fopen(filename.c_str(),"r");
    if (!parq) {
        if(parq)
            fclose(parq);
        return 200;
    } else {
        for(i=0;i<52;i++) //fixo para ler um determinado arquivo
        {
            for(j=0;j<52;j++) {
                fscanf(parq,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &x, &y, &lixo, &lixo, &pressao, &lixo, &lixo, &lixo, &lixo, &lixo);
                if(x<1e-10) x = 0.;
                if(y<1e-10) y = 0.;
                point.X(x);
                point.Y(y);
                Include(point);
                vPressao.push_back(pressao);
            }
        }

        fclose(parq);

        //grava arquivo de pressao
        parq = fopen("vve.p1g","w");
        if (parq) {
            int num_points = vPressao.size();

            fprintf(parq,"%d\n", num_points);

            for(i=0;i<num_points;i++) {
                lixo = vPressao[i];
                fprintf(parq,"%g\n", lixo);
            }

            fclose(parq);
        }
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return 0;
}

void CLevGeoData::AttachObserver(CObserverBase *observer)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    bool found = false;

    for(i=0;i<vpObservers.size() && found == false;i++)
        if(vpObservers[i] == observer)
            found = true;
    if(!found)
        vpObservers.push_back(observer);
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::DetachObserver(CObserverBase *observer)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    int index = 0;
    bool found = false;

    for(i=0;i<vpObservers.size() ;i++) {
        if(vpObservers[i] == observer) {
            found = true;
            index = i;
        }
    }
    if(found) {
        vpObservers.erase(vpObservers.begin() + index);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

void CLevGeoData::Notify(GEOMETRICOPERATION operation,CLevPrimitive *pPrim)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned long i;
    for(i=0;i<vpObservers.size() ;i++)
        vpObservers[i]->Update(operation,pPrim);
    meshfronteiraOk = false;
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

// *NMA*: 01/06/2005: leitura de todos os arquivos necessários
int CLevGeoData::Read(void)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    int codeReturned = 0;

    codeReturned = ReadGeometricModel(project->GetFileName(MODEL_GEOMETRY_PREDIS));

    if(codeReturned == 101) //erro na abertura de MGP (pode não existir)
    {
        codeReturned = ReadELL(project->GetFileName(MODEL_CONNECTION));
    }

    if(!codeReturned)
    {
        ModelPrimitivesOk(true);
        codeReturned = ReadModelRegions(project->GetFileName(MODEL_REGIONS));
    }
    else ModelPrimitivesOk(false);

    if(!codeReturned)
    {
        ModelRegionsOk(true);
        codeReturned = ReadModelMeshFrontier(project->GetFileName(MODELMESHFRONTEIRA));
    }
    else ModelRegionsOk(false);

    if(!codeReturned && vElementos_fronteira.size())
    {
        MeshFronteiraOk(true);
    }

	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return codeReturned;
}

vCLevPoint CLevGeoData::GetElementVerticesCoordFronteira(int ind)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    Elemento_2D        element;
    vCLevPoint        vertices;
    long            vert[5];
    CLevPoint        p1, p2, p3;
    Pt_Malha        pt1,pt2,pt3;

    if(ind >=0 && ind < (signed)vElementos_fronteira.size())
    {
        element = vElementos_fronteira[ind];

        element.Get_Vertices(vert);

        pt1 = vPontos_fronteira[vert[0]];
        pt2 = vPontos_fronteira[vert[1]];
        pt3 = vPontos_fronteira[vert[2]];

        p1.X(pt1.X());
        p1.Y(pt1.Y());

        p2.X(pt2.X());
        p2.Y(pt2.Y());

        p3.X(pt3.X());
        p3.Y(pt3.Y());

        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    return vertices;
}

int    CLevGeoData::GetNumMeshElementsFronteira(void)
{
    return vElementos_fronteira.size();
}

//*NMA*: operação de escalamento
void CLevGeoData::Scale(double factor)
{
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
    unsigned int i;

    for(i=0; i<vpPrimitives.size(); i++)
        vpPrimitives[i]->Scale(factor);

    // Para escalar os pontos de pre-discretização da malha de fronteira
    for(i=0; i<vpPoints.size(); i++)
        vpPoints[i]->Scale(factor);

    for(i=0; i<vpInvisiblePoints.size(); i++)
        vpInvisiblePoints[i]->Scale(factor);

    for(i=0; i<vpPrimitives.size(); i++)
        vpPrimitives[i]->FixEntity(false);

    for(i=0; i<vpPoints.size(); i++)
        vpPoints[i]->FixEntity(false);

    for(i=0; i<vpInvisiblePoints.size(); i++)
        vpInvisiblePoints[i]->FixEntity(false);

    if(vPontos_fronteira.size())
    {
        for(i=0; i<vPontos_fronteira.size(); i++)
            vPontos_fronteira[i].Scale(factor);

        for(i=0; i<vPontos_fronteira.size(); i++)
            vPontos_fronteira[i].FixEntity(false);

        MeshFronteiraOk(true);
    }

    if(factor >1)
        TakeLimits();
    else
    {
        double xmax = original.GetXMax();
        double xmin = original.GetXMin();

        double ymax = original.GetYMax();
        double ymin = original.GetYMin();

        xmax *= factor;
        xmin *= factor;
        ymax *= factor;
        ymin *= factor;

        original.SetXMax(xmax);
        original.SetXMin(xmin);
        original.SetYMax(ymax);
        original.SetYMin(ymin);
    }
	#ifdef __DEBUG_MODE__
	std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
	#endif
}

