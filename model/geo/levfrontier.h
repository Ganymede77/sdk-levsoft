#ifndef __LEV_FRONTIER_H_
#define __LEV_FRONTIER_H_

//=============================================================================
//    Arquivo:            LevFrontier.h
//.............................................................................
//    Descricao:            Interface da classe CLevFrontier
//.............................................................................
//    Autores:            Nancy Mieko Abe
//                        Angelo Passaro
//
//-----------------------------------------------------------------------------
//    Classe CLevFrontier
//-----------------------------------------------------------------------------
//    Dados membros:
//
//    Associacoes:
//
//    Observacoes: Derivada de CLevGroup, para armazenar um conjunto de
//               primitivas que formam uma fronteira. Implementada para
//               modelagem grafica de dados de entrada para geracao de malha
//               estruturada (projeto com Francisco Rocamora)
//.............................................................................
//    Data:                Abril/2004
//    Modificado em:
//    Modificado por:
//    Motivo:
//=============================================================================


#include "levgroup.h"


class CLevFrontier : public CLevGroup
{
protected:

    int                numPredisSegments;
    vInt            vSubregionsIndex;
    vpCLevPoint        vpPointOnePrimitive; // vetor usado para armazernar os pontos que pertencem a somente uma primitiva (pontos extremos)
    vpCLevFrontier  vpRelatedFrontiers;

    bool            isInPredisVector(CLevPoint *);

public:
    vCLevPoint      vPointsFrontier; //vetor que armazena os pontos da fronteira discretiza��o + v�rtices)

    CLevFrontier(); //Constructor
    virtual ~CLevFrontier(); //Destructor

    int                ConnectionPointsSortPrimitives(void);
    void            UpdateNumOfPredisSegments(int ); //atualiza quando da pr�-discretiza��o de uma das primitivas
    int                GetNumOfPredisSegments(void );
    short int        VerifyConnections(CLevPoint  *p);
    int                IsAPoint(CLevPoint *p);//especializa��o para fronteiras
    void            SetSubRegionIndex(int );
    int                GetNumOfSubRegionIndex(void);
    CLevPoint *        GetPointOnePrimitive(int);
    void            BuildPrediscretizationVector(void);
    void            Reinitialize();
    void            ReinitializeAuxiliarData();

//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
    bool            operator ==    ( CLevFrontier &);
    CLevFrontier&    operator  =    (const CLevFrontier &);

};


#endif
