#ifndef __LEV_ELLIPSE_H__
#define __LEV_ELLIPSE_H__

//=============================================================================
//    Arquivo:            LevEllipse.h
//.............................................................................
//    Descricao:            Interface da classe CLevEllipse
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//-----------------------------------------------------------------------------
//    Classe CLevEllipse
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

#include "levcurve.h"

class CLevEllipse : public CLevCurve
{

protected:
    double        xc;
    double        yc;
    double        x_semiAxis;
    double        y_semiAxis;

    void        Locate_Crossing_Coordinates(double angle, double &xCoord, double &yCoord);
    void        Locate_Extremity_Coordinates(double angle, double &xCoord, double &yCoord);

    double        Get_Point_Angle(CLevPoint *);
public:

    CLevEllipse();
    CLevEllipse(double xct,  double yct, double x_semiA, double y_semiA,
                double sAng, double eAng, vCLevPoint & vPoints); //Constructor

    CLevEllipse(CLevPoint *p1, CLevPoint *p2, CLevPoint *pCentral,
                CLevPoint *pAxisY,CLevPoint *pAxisX, double sAng, double eAng); //Constructor

    CLevEllipse(CLevPoint *p1, CLevPoint *p2, CLevPoint *pCentral,
                CLevPoint *pAxisY,CLevPoint *pAxisX); //Constructor

    virtual ~CLevEllipse();

    virtual CLevPrimitive* Clone() const override;

    //*NMA*
    // N�o esquecer de implementar o construtor de c�pia e a opera��o de atribui��o

    double        GetXCenter(void) { return xc;}
    double        GetYCenter(void) { return yc;}
    double        GetSemiX(void) { return x_semiAxis;}
    double        GetSemiY(void) { return y_semiAxis;}
    double        GetStartAngle(void) { return startAngle;}
    double        GetEndAngle(void) { return endAngle;}


//......................................................................................................
//................. fun��es de interface herdadas de CLevGeoEntity......................................
//......................................................................................................
    virtual void    Limits        (CLevPoint &, CLevPoint &);
    virtual void    Move        (double deltax, double deltay, double deltaz = 0);
    virtual void    Reinitialize(void);
    virtual void    Rotation    (double ang, CLevPoint &);
    virtual void    Scale        (double);
    virtual void    TakeLimits    (void);
    virtual void    ReflectionByX        (void);
    virtual void    ReflectionByY        (void);

//......................................................................................................
//................. interfaces herdadas de CLevPrimitive......................................
//......................................................................................................
    virtual bool    IsInBoundingBox(CLevPoint);
    virtual void    PreDiscretize(vCLevPoint &);
    virtual void    SetPoints(vpCLevPoint &) ;

    virtual CLevPoint GetMiddlePoint(void);

//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
    CLevEllipse & operator = (const CLevEllipse &);
    bool operator ==    (const CLevEllipse &a);
    bool operator !=    (const CLevEllipse &a);

//.............................................................................
//.......................Gravacao e Leitura....................................
//.............................................................................
    int    Save(FILE * parq, int unidade, FILENAME tipoArq);
    int Read(FILE * parq, int unidade, char *tp, FILENAME tipoArq);

//.............................................................................
//................... funcoes friend...........................................
//.............................................................................
    friend    unsigned int CrossingPoints( CLevLine  o_line, CLevEllipse  o_ellipse, vCLevPoint &vo_crosspoint);

    friend class CTabItemPropertiesEllipse;
};


#endif
