#ifndef __LEVPRIMITIVE_H_
#define __LEVPRIMITIVE_H_

//=============================================================================
//    Arquivo:            LevPrimitive.h
//.............................................................................
//    Descricao:            Interface da classe CLevPrimitive
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//-----------------------------------------------------------------------------
//    Classe CLevPrimitive
//-----------------------------------------------------------------------------
//    Dados membros:
//
//    Associacoes:
//
//    Observacoes:
//.............................................................................
//    Data:                Junho/2000
//    Modificado em:
//    Modificado por:
//    Motivo:
//=============================================================================

#include "levgeoentity.h"
#include "levpoint.h"
#include "predis.h"
#include "levdef.h"
#include <stdio.h>

class CLevPrimitive : public CLevGeoEntity
{

protected:

    CLevPoint    pt_minxminy,pt_maxxmaxy;  // armazena os limites da primitiva
//    vCLevPoint    vDivisionPoints;

public:

    vpCLevPoint vpPredisPoints;
    int         num_polygons;
    CPredis     oPredis;            // *NMA* Dados de pre_discretizacao
    vInt        vIndexArestaGeometria;

    // *NMA*
    // Nao esquecer de implementar o construtor de copia e a operacao de atribuicao

    CLevPrimitive();
    virtual ~CLevPrimitive();

    // Metodo de clonagem polimórfica
    virtual CLevPrimitive* Clone() const = 0;

    //................. funcoes de interface herdadas de CLevGeoEntity
    virtual void FixEntity(bool fE);
    virtual bool FixEntity(void);
    virtual void Limits(CLevPoint &, CLevPoint &);
    virtual void Reinitialize(void);
    virtual void TakeLimits(void);

    //................. funcoes de interface desta classe......................................
    // virtual USINT    CrossingPoint(CLevPrimitive, vCLevPoint &vo_crosspoint) {return 0;};
    virtual bool IsInBoundingBox(CLevPoint)=0; //falta criar em curvas e poligonos
    virtual void PreDiscretize(vCLevPoint &) =0;
    virtual void SetPreDiscretizationData(int ,double , CLevPoint );
    virtual CPredis GetPreDiscretizationData(void);
    virtual void GetPredisPoints(vCLevPoint &);

    virtual void SetPoints(vpCLevPoint &)=0;

    virtual CLevPoint GetMiddlePoint(void)=0;

    virtual int  Save(FILE * parq, int unidade, FILENAME tipoArq)=0;
    virtual int  Read(FILE * parq, int unidade, char *tp, FILENAME tipoArq)=0;
};

#endif
