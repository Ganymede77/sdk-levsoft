#ifndef __LEV_GEOENTITY_H_
#define __LEV_GEOENTITY_H_

//=============================================================================
//    Arquivo:            LevGeoEntity.h
//.............................................................................
//    Descricao:          Interface da classe CLevGeoEntity
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//
//-----------------------------------------------------------------------------
//    Classe CLevGeoEntity
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


#include "levenum.h"
#include "levtypedef.h"


class CLevPoint;


class CLevGeoEntity
{
    protected:

        int                status;
        GEOENTITYTYPE    geoEntityType; // *NMA*: ex. para pontos: GEOM,PRE,REF
        int                id;
        bool            fixedEntity;
        bool            isOfGeometry;  // se a primitiva ou ponto for definida como n�o sendo da geometria, ela pode ou n�o ser desenha com a geometria, � auxiliar

        // Propriedades F�sicas
        // *NMA*: Futuramente em classe derivada de CLevGeoEntity
        long            materialIndex;
        long            boundConditionIndex;
        long            sourceIndex;

    public:

        vpCLevPoint        vpPoints;
        vpCLevLine        vpLines;
        vpCLevEllipse    vpEllipses;
        vpCLevPrimitive    vpPrimitives;

        CLevGeoEntity();
        virtual ~CLevGeoEntity();

        //..................................................................
        //................. M�todos virtuais puros..........................
        //..................................................................
        virtual void    Limits(CLevPoint &, CLevPoint &) = 0;
        virtual void    Move        (double a, double b, double c = 0)    =0;
        virtual void    Rotation    (double ang, CLevPoint &)            =0;
        virtual void    Scale        (double factor)                        =0;
        virtual void    ReflectionByX        (void) = 0 ;
        virtual void    ReflectionByY        (void) = 0 ;
        virtual void    TakeLimits    (void) = 0;  // obtem os pontos v�rtices de um ret�ngulo que engloba a entidade

        //..................................................................
        //..................M�todos virtuais ...............................
        //..................................................................
        virtual void    Reinitialize(void)            { status = LEV_VALID; }
        virtual    void    FixEntity(bool fE)            { fixedEntity = fE; }
        virtual    bool    FixEntity(void)                { return fixedEntity; }

        //..................................................................
        //...................... M�todos ...................................
        //..................................................................
        int        Status   (void)                        {return status;};
        void    Status   (int s)                    {status = s;};

        int        GeoEntityType(void)                    {return geoEntityType;};
        void    GeoEntityType(GEOENTITYTYPE t)        {geoEntityType = t;};

        bool    IsOfGeometry(void)                    {return isOfGeometry;};
        void    IsOfGeometry(bool bIsOf)            {isOfGeometry=bIsOf;};

        int        Id(void)                            {return id;};
        void    Id(int ident)                        {id = ident;};

        int        NumPoints(void)                        {return vpPoints.size();};
        int        NumLines(void)                        {return vpLines.size();};
        int        NumEllipses(void)                    {return vpEllipses.size();};

        int        NumPrimitives(void)                    {return vpPrimitives.size();};

        //=========================== Propriedades F�sicas =================
        // *NMA*: Futuramente em classe derivada de CLevGeoEntity
        void    SetMaterial(long m)                  { materialIndex = m;}
        void    SetBoundaryCondition(long bc)        { boundConditionIndex = bc;}
        void    SetSource(long s )                   { sourceIndex = s;} //*NMA*: guarda  �ndice por causa das fontes vari�veis no tempo (ou no espa�o)
        long    GetMaterial(void)                    { return materialIndex;}
        long    GetBoundaryCondition(void)           { return boundConditionIndex;}
        long    GetSource(void)                      { return sourceIndex;}
};


#endif
