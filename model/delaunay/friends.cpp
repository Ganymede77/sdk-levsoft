#include <limits>
#include "gam.hpp"
#include "aresta_geom.hpp"
#include "levline.h"
#include "levellipse.h"


//========================= Gerador de Malha ===============================
bool operator == (const FACE &a, const FACE &b)
{
    if ((a.V1 == b.V1 && a.V2 == b.V2) ||
        (a.V1 == b.V2 && a.V2 == b.V1))
        return 1;
    else return 0;
}


bool operator == (const Aresta_Geometria & a, const FACE &b)
{
    if ((a.g1 == b.V1 && a.g2 == b.V2) ||
        (a.g1 == b.V2 && a.g2 == b.V1))
        return 1;
    else return 0;
}


bool operator == ( FACE const&b, Aresta_Geometria const & a)
{
    return ( a == b);
}


//========================= Modelador ======================================
//--------------------------------------------------------------------------
//    CrossingPoint
//--------------------------------------------------------------------------
//
//    Descri��o: Verifica a existencia de intesec��o entre a linha e elipse.
//
//    par�metros:    Um objeto linha (primitiva), um objeto ellipse (primitiva)
//              e uma refer�ncia a um vector STL vCLevPoint para
//                retornar o(s) ponto(s) de cruzamento de existir
//
//    retorno:    0: n�o existe intersecc��o
//                1: existe um ponto de intersec��o
//                2: existem dois pontos de intersec��o
//
//    Observa��es:
//
//..........................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//                    Francisco Sircilli
//    Data:            Abril/2001
//    Modificado por:
//    Modificado em:
//    Motivo:
//--------------------------------------------------------------------------
unsigned int CrossingPoints(CLevLine o_line, CLevEllipse o_ellipse, vCLevPoint &vo_crosspoint)
{
    double angCoef=0;        // coeficiente angular da reta
    double linCoef=0;        // coeficiente y da reta
    double xSemiAxis = 0;    // semi-eixo em x da elipse
    double ySemiAxis = 0;    // semi-eixo em y da elipse
    double xc, yc;            // coordenadas do centro da elipse
    double aa, bb, cc;        // auxiliares
    double xSemiAxisSquare, ySemiAxisSquare; //auxiliares
    double delta;
    double x, y;
    int numCross = 0;
    int    lineType;
    double xr = 0.0, yr = 0.0, signal = 1.0;

    CLevPoint o_ponto;
    CLevPoint lxymin, lxymax, exymin, exymax; // limites m�ximos e m�nimos da linha e da elipse

    // Verifica se os limites da reta e da elipse est�o dentro de um retangulo definido
    // pelos pontos m�ximo e minimo uma da outra
    o_line.Limits(lxymin, lxymax);
    o_ellipse.Limits(exymin, exymax);

    if(lxymax.X() < exymin.X() || exymax.X() < lxymin.X())
        numCross = 0;
    else {
        if(lxymax.Y() < exymin.Y() || exymax.Y() < lxymin.Y())
            numCross = 0;
        else {
            // busca refinada
            xSemiAxis = o_ellipse.x_semiAxis;
            ySemiAxis = o_ellipse.y_semiAxis;

            xSemiAxisSquare = xSemiAxis * xSemiAxis;
            ySemiAxisSquare = ySemiAxis * ySemiAxis;

            xc = o_ellipse.xc;
            yc = o_ellipse.yc;

            angCoef = o_line.Angular_Coefficient();
            linCoef    = o_line.Linear_Coefficient();

            if(angCoef < 0 )
                signal = -1;

            if( fabs(angCoef-std::numeric_limits<double>::max()) < 1e-10 ) { // reta paralela ao eixo y
                xr = o_line.vpPoints[0]->X();
                aa = xSemiAxisSquare;
                bb = -2.0*xSemiAxisSquare*yc;
                cc = xSemiAxisSquare*(yc*yc-ySemiAxisSquare) + ySemiAxisSquare*(xr-xc)*(xr-xc);
                lineType = 0;
            } else if( fabs(angCoef) < 1e-10 ) { // reta paralela ao eixo x
                yr = o_line.vpPoints[0]->Y();
                aa = ySemiAxisSquare;
                bb = -2.0*ySemiAxisSquare*xc;
                cc = ySemiAxisSquare*(xc*xc - xSemiAxisSquare) + xSemiAxisSquare*(yr-yc)*(yr-yc);
                lineType = 1;
            } else { // reta inclinada
                aa = ySemiAxisSquare + xSemiAxisSquare*angCoef*angCoef;
                bb = -2.0*ySemiAxisSquare*xc + 2.0*xSemiAxisSquare*angCoef*linCoef - 2.0*xSemiAxisSquare*angCoef*yc;
                cc = ySemiAxisSquare*xc*xc + xSemiAxisSquare*linCoef*linCoef + xSemiAxisSquare*yc*yc - 2.0*xSemiAxisSquare*linCoef*yc - xSemiAxisSquare*ySemiAxisSquare;
                lineType = 2;
            }

            delta = bb*bb - 4.0*aa*cc;

            if(delta == 0) {
                switch(lineType) {
                    case 0: // reta paralela ao eixo y
                        x = xr;
                        y = -bb/(2.0*aa);
                        break;
                    case 1:    // reta paralela ao eixo x
                        x = -bb/(2.0*aa);
                        y = yr;
                        break;
                    case 2:    // reta inclinada
                        x = (-bb/(2.0*aa)) * signal;
                        y = angCoef*x + linCoef;
                        break;
                }

                o_ponto.X(x);
                o_ponto.Y(y);
                if(o_ellipse.IsInBoundingBox(o_ponto) && o_line.IsInBoundingBox(o_ponto)) {
                    vo_crosspoint[0].X(x);
                    vo_crosspoint[0].Y(y);
                    numCross++;
                }
            } else if(delta > 0) {

                switch(lineType) {
                    case 0: // reta paralela ao eixo y
                        x = xr;
                        y = (-bb + sqrt(delta))/(2*aa);
                        break;
                    case 1:    // reta paralela ao eixo x
                        x = (-bb + sqrt(delta))/(2*aa);
                        y = yr;
                        break;
                    case 2:    // reta inclinada
                        x = (-bb + sqrt(delta))/(2*aa);
                        y = angCoef*x + linCoef;
                        break;
                }

                o_ponto.X(x);
                o_ponto.Y(y);

                if(o_ellipse.IsInBoundingBox(o_ponto) && o_line.IsInBoundingBox(o_ponto)) {
                    vo_crosspoint[0].X(x);
                    vo_crosspoint[0].Y(y);
                    numCross++;
                }

                switch(lineType) {
                    case 0: // reta paralela ao eixo y
                        x = xr;
                        y = (-bb - sqrt(delta))/(2.0*aa);
                        break;
                    case 1:    // reta paralela ao eixo x
                        x = (-bb - sqrt(delta))/(2.0*aa);
                        y = yr;
                        break;
                    case 2:    // reta inclinada
                        x = (-bb - sqrt(delta))/(2.0*aa);
                        y = angCoef*x + linCoef;
                        break;
                }

                o_ponto.X(x);
                o_ponto.Y(y);

                if(o_ellipse.IsInBoundingBox(o_ponto) && o_line.IsInBoundingBox(o_ponto)) {
                    vo_crosspoint[numCross].X(x);
                    vo_crosspoint[numCross].Y(y);
                    numCross++;
                }
            }
        }
    }
    return numCross;
}
