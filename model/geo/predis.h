//////////////////////////
//Predis.h
////////////////////////

//Class declaration

#ifndef __PREDIS_H__
#define __PREDIS_H__

#include "levtypedef.h"

class CPredis
{

protected:
    int            nPoints;
    double        factor;
    double        x_clicked,y_clicked;//*NMA* posicao "clickada" pelo usuario, utilizada para
                            // determinar o lado com maior concentracao de pontos no
                            // caso de divisao geometrica
    vCLevPoint  vPredisPoints;

public:

//    CPredis(){nPoints =0;factor =1;}; //Constructor
    CPredis(){Reinitialize();}; //Constructor
    ~CPredis(){}; //Destructor


    void SetPreDiscretizationData(int nP,double f, double xi, double yi)
    {
        nPoints        =    nP;
        factor        =    f;
        x_clicked    =    xi;
        y_clicked    =    yi;
    }

    void Reinitialize (void)
    {
        nPoints        =    0;
        factor        =    1;
        x_clicked    =    0;
        y_clicked    =    0;

        vPredisPoints.clear();
    }

    void SetPredisPoints(vCLevPoint v)
    {
        vPredisPoints = v;
    }

    void GetPredisPoints(vCLevPoint & v)
    {
        v = vPredisPoints;
    }

    int NPoints(void) { return nPoints; }
    double Factor(void) { return factor;}
    double XClicked(void) { return x_clicked;}
    double YClicked(void) { return y_clicked;}


};

#endif
