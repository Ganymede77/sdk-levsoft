//=============================================================================
//    Arquivo:            LevLine.cpp
//.............................................................................
//    Descricao:          Implementacao da classe CLevLine
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//.............................................................................
//    Observacoes:
//
//=============================================================================

#include "levline.h"
#include "levpoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits>

////////////////////////
//The constructor function
////////////////////////
CLevLine::CLevLine()
{
    vpPoints.reserve(10);
    vpLines.reserve(100);
    vpPoints.push_back(NULL);
    vpPoints.push_back(NULL);
    coefficients[0] = coefficients[1] = coefficients[2] = 0;

    // dados de pre-discretizacao
    oPredis.Reinitialize();
}

CLevLine::CLevLine(CLevPoint *p1, CLevPoint *p2)
{
    vpPoints.push_back(p1);
    vpPoints.push_back(p2);
    Line_Coefficients();
    vpLines.push_back(this);
}

////////////////////////
//The destructor function
////////////////////////
CLevLine::~CLevLine()
{
    vpPoints.clear();
    vpLines.clear();

}

//-----------------------------------------------------------------------------------------------------
//    Move()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:
//
//    par�metros:
//
//    retorno:
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//                    Anna Carolina S. Volpi (Adaptado de System_POI)
//    Data:            Junho/2000
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevLine::Move(double deltax, double deltay, double deltaz)
{
    vpPoints[0]->Move(deltax, deltay, deltaz);
    vpPoints[1]->Move(deltax, deltay, deltaz);
    Line_Coefficients();

    TakeLimits();

} // end of Move

void CLevLine::ReflectionByX(void)
{
    vpPoints[0]->ReflectionByX();
    vpPoints[1]->ReflectionByX();

    TakeLimits();
}
void CLevLine::ReflectionByY(void)
{
    vpPoints[0]->ReflectionByY();
    vpPoints[1]->ReflectionByY();

    TakeLimits();
}
////////////////////////
//The rotation() function
////////////////////////
void CLevLine::Rotation(double angle, CLevPoint &pointc)
{
    if(!fixedEntity)
    {
        vpPoints[0]->Rotation(angle, pointc);
        vpPoints[1]->Rotation(angle, pointc);
        Line_Coefficients();
    }

    TakeLimits();
}

////////////////////////
//The reinitialize() function
////////////////////////
// ATEN��O : *AP*  estudar se vpPoints[i] deve ser destru�do!
void CLevLine::Reinitialize(void)
{
    CLevPrimitive :: Reinitialize();
    coefficients[0] = coefficients[1] = coefficients[2] = 0;
}

///////////////////////////////////////////////
//*NMA*:Reinicializa��o do vetor de linhas (pr�-discretica��o)
///////////////////////////////////////////////
#if 0
void CLevLine::ReinitializeLines(void)
{
    CLevLine    *po_line;
    CLevPoint    *po_point;
    int            i;


    if(vpLines.size())
    {
        for(i=0;i<vpLines.size();i++)
        {
            po_line = vpLines[i];
            po_point = po_line->vpPoints[0];

            if(po_point->Type() == PRE) po_point->Status(LEV_INVALID);

            po_point = po_line->vpPoints[1];

            if(po_point->Type() == PRE) po_point->Status(LEV_INVALID);

            po_line->Status(LEV_INVALID);

        }

        vpLines.clear();

        oPredis.Reinitialize();
    }

}
#endif


void CLevLine::SetPoints(vpCLevPoint &vp)
{
    if(vpPoints.size())
    {
        vpPoints[0] = vp[0]; // ponto inicial
        vpPoints[1] = vp[1]; // ponto final
    }
    else
    {
        vpPoints.push_back(vp[0]);
        vpPoints.push_back(vp[1]);
    }

    Line_Coefficients();

}// end of SetPoints


//-----------------------------------------------------------------------------------------------------
//    operator =
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o: operador de atribui��o
//
//    par�metros:    refer�ncia a um objeto da mesma classe
//
//    retorno:    uma refer�ncia ao conte�do do pr�prio objeto
//
//    Observa��es: Nesta implementa��o, a opera��o de atribui��o copia
//               os dados membros que correspondem a atributos reais
//               do objeto e toma medidas convenientes quanto a dados
//               dados membros de controle ou de modo de opera��o.
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Julho/2000
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
CLevLine & CLevLine :: operator = (const CLevLine & ro_entrada)
{

    if (this != &ro_entrada)
    {
        // isto � copia dos dados de primitive
        //=====================================
        vpPoints     = ro_entrada.vpPoints;
        pt_maxxmaxy  = ro_entrada.pt_maxxmaxy;
        pt_minxminy  = ro_entrada.pt_minxminy;
        num_polygons = ro_entrada.num_polygons;
        status       = ro_entrada.status;
        isOfGeometry = ro_entrada.isOfGeometry;

        //propriedades f�sicas
        materialIndex     = ro_entrada.materialIndex;
        sourceIndex         = ro_entrada.sourceIndex;
        boundConditionIndex = ro_entrada.boundConditionIndex;

        vpLines.clear(); vpLines.push_back(this);
        //======================================

        //dados de pr�-discretiza��o
        oPredis        =    ro_entrada.oPredis;
        vIndexArestaGeometria = ro_entrada.vIndexArestaGeometria;

        memcpy(coefficients, ro_entrada.coefficients,3 * sizeof(double));
    }
    return *this;
} // fim operator =


////////////////////////
//The operator==() function
////////////////////////

//======================================================
//  Esta vers�o compara o ENDERE�O  dos pontos vertices
//  da linha.
//  Por completeza, a verifia��o � feita considerando
//  que a ordem dos pontos v�rtices pode estar invertida!
//======================================================


bool CLevLine::operator == (const CLevLine &a)
{  // note-se que n�o s�o comparadas a vari�vel de controle num_poligonos.
    // interessa-nos somente a compara��o dos objetos componentes!

    bool ret = false;

    if (status && a.status)
//    if (status && a.Status())
    {
        if (vpPoints[0] == a.vpPoints[0])
        {
            if (vpPoints[1] == a.vpPoints[1]) ret = true;
        }
        else
        {
            if (vpPoints[0] == a.vpPoints[1])
            {
                if (vpPoints[1] == a.vpPoints[0]) ret = true;
            }
        }
    }
    return ret;
}

/*
//======================================================
//  Esta vers�o compara o CONTE�DO dos pontos vertices
//  da linha.
//  Por completeza, a verifia��o � feita considerando
//  que a ordem dos pontos v�rtices pode estar invertida!
//======================================================

bool CPOI_Line::operator == (const CPOI_Line &a)
{
    bool ret = false;

    if (status && a.status)
    {
        if (*vpPoints[0] == *a.vpPoints[0])
        {
            if (*vpPoints[1] == *a.vpPoints[1]) ret = true;
        }
        else
        {
            if (*vpPoints[0] == *a.vpPoints[1])
            {
                if (*vpPoints[1] == *a.vpPoints[0]) ret = true;
            }
        }
    }
    return ret;
}
*/
//======================================================
//  Esta vers�o compara o ENDERE�O  dos pontos vertices
//  da linha.
//  Por completeza, a verifia��o � feita considerando
//  que a ordem dos pontos v�rtices pode estar invertida!
//======================================================

/*
bool CPOI_Line::operator != (const CPOI_Line &a)
{
    bool ret = true;

    if (status && a.status)
    {
        if (vpPoints[0] == a.vpPoints[0])
        {
            if (vpPoints[1] == a.vpPoints[1]) ret = false;
        }
        else
        {
            if (vpPoints[0] == a.vpPoints[1])
            {
                if (vpPoints[1] == a.vpPoints[0]) ret = false;
            }
        }
    }
    return ret;
}
*/

//======================================================
//  Esta vers�o compara o CONTE�DO dos pontos vertices
//  da linha.
//  Por completeza, a verifia��o � feita considerando
//  que a ordem dos pontos v�rtices pode estar invertida!
//======================================================

bool CLevLine::operator != (const CLevLine &a)
{
    bool ret = true;

    if (status && a.status)
    {
        if (*vpPoints[0] == *a.vpPoints[0])
        {
            if (*vpPoints[1] == *a.vpPoints[1]) ret = false;
        }
        else
        {
            if (*vpPoints[0] == *a.vpPoints[1])
            {
                if (*vpPoints[1] == *a.vpPoints[0]) ret = false;
            }
        }
    }
    return ret;
}


void CLevLine::Line_Coefficients()
{
    // defines the y coefficient
    if(vpPoints[0]->X() == vpPoints[1]->X()) // reta paralela ao eixo y
        coefficients[0] = 0;
    else
        coefficients[0] = 1;

    // defines the x coefficient (angular coefficient)
    if(vpPoints[0]->Y() == vpPoints[1]->Y()) // reta paralela ao eixo x
        coefficients[1] = 0;
    else {
        if(coefficients[0])
            coefficients[1] = (vpPoints[1]->Y() - vpPoints[0]->Y())/(vpPoints[1]->X() - vpPoints[0]->X());
        else
            coefficients[1] = std::numeric_limits<double>::max();
    }

    // defines the constant coefficient (linear coefficient)
    coefficients[2] = vpPoints[0]->Y() - coefficients[1] * vpPoints[0]->X();
    TakeLimits();
}


//-----------------------------------------------------------------------------------------------------
//    CrossingPoint
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o: Verifica a existencia de intesec��o entre a linha e outra linha dada.
//
//    par�metros:    Um objeto linha (primitiva) e uma refer�ncia a CLevPoint para
//                retornar o ponto de cruzamento de existir
//
//    retorno:    0: n�o existe intersecc��o
//                1: existe um ponto de intersec��o
//                2: linhas coincidentes com sobreposi��o
//
//    Observa��es: O c�digo para determina��o de pontos de interse��o (sem tratamento para
//                linhas coincidentes) foi transladado da fun��o LocateLineCrosses
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Dezembro/2001
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
lev_math::USINT CLevLine::CrossingPoint(CLevLine otherline, vCLevPoint &vo_crosspoint)
{
    lev_math::USINT codRetorno = 0;
    CLevPoint crosspoint;

    if(this != &otherline) {

        if( fabs(Angular_Coefficient()-otherline.Angular_Coefficient()) > 1e-10 ) { // linhas s�o concorrentes

            if( (*vpPoints[0] == *otherline.vpPoints[0]) || (*vpPoints[0] == *otherline.vpPoints[1]) )
                return codRetorno; // O ponto de cruzamento eh um dos v�rtices

            if(    (*vpPoints[1] == *otherline.vpPoints[0]) || (*vpPoints[1] == *otherline.vpPoints[1]) )
                return codRetorno; // O ponto de cruzamento eh um dos v�rtices

            if( !(int)Y_Coefficient() ) { // linha eh vertical
                crosspoint.X( vpPoints[0]->X() );
                crosspoint.Y( otherline.Angular_Coefficient() * vpPoints[0]->X() + otherline.Linear_Coefficient() );
            } else {
                if( !(int)otherline.Y_Coefficient() ) { // outra linha eh vertical
                    crosspoint.X( otherline.vpPoints[0]->X() );
                    crosspoint.Y( Angular_Coefficient() * crosspoint.X() + Linear_Coefficient() );
                } else { // as duas linhas s�o inclinadas
                    crosspoint.X( -(Linear_Coefficient() - otherline.Linear_Coefficient())/(Angular_Coefficient() - otherline.Angular_Coefficient()) );
                    crosspoint.Y( Angular_Coefficient() * crosspoint.X() + Linear_Coefficient() );
                }
            }

            if( IsInBoundingBox(crosspoint) && otherline.IsInBoundingBox(crosspoint) ) {
                vo_crosspoint[0].X(crosspoint.X());
                vo_crosspoint[0].Y(crosspoint.Y());
                codRetorno = 1;
            }

        } else { // linhas s�o paralelas

            bool crossing = false;
            // Verifica se existe sobreposi��o se:
            // - existe um ponto v�rtice comum �s duas linhas e o outro ponto est� dentro dos limites da outra reta
            //        (se existir um ponto v�rtice comum �s duas linhas e o outro ponto n�o est� dentros limites da
            //         outra linha, isto n�o deve ser considerado como sobreposi��o, sen�o o programa entra em loop,
            //         pois as duas linhas ser�o destru�das e reconstru�das com os mesmos pontos)
            // - se todos os pontos forem distintos e um deles estiver dentro dos limites da outra reta

            if( ((*vpPoints[0] == *otherline.vpPoints[0]) || (*vpPoints[0] == *otherline.vpPoints[1]) ) && (otherline.IsInBoundingBox(*vpPoints[1])) )
                crossing = true;
            else {
                if( ((*vpPoints[1] == *otherline.vpPoints[0]) || (*vpPoints[1] == *otherline.vpPoints[1]) ) && (otherline.IsInBoundingBox(*vpPoints[0])) )
                    crossing = true;
                else {
                    if(    ((*otherline.vpPoints[0] == *vpPoints[0]) || (*otherline.vpPoints[0] == *vpPoints[1]) ) && (IsInBoundingBox(*otherline.vpPoints[1])) )
                        crossing = true;
                    else {
                        if(    ((*otherline.vpPoints[1] == *vpPoints[0]) || (*otherline.vpPoints[1] == *vpPoints[1]) ) && (IsInBoundingBox(*otherline.vpPoints[0])) )
                            crossing = true;
                        else {
                            if(    ((*vpPoints[0] != *otherline.vpPoints[0]) && (*vpPoints[0] != *otherline.vpPoints[1]) )  &&  ((*vpPoints[1] != *otherline.vpPoints[0]) && (*vpPoints[1] != *otherline.vpPoints[1]) ) )
                                if( (otherline.IsInBoundingBox(*vpPoints[0])) || (otherline.IsInBoundingBox(*vpPoints[1]))  ||  (IsInBoundingBox(*otherline.vpPoints[1])) || (IsInBoundingBox(*otherline.vpPoints[0])) )
                                    crossing = true;
                        }
                    }
                }
            }

            if(crossing) {
                if( ((!(int)Y_Coefficient() && !(int)otherline.Y_Coefficient()))  ||  ((vpPoints[0]->Y()==vpPoints[1]->Y()) && (otherline.vpPoints[0]->Y()==otherline.vpPoints[1]->Y()) && (vpPoints[0]->Y()==otherline.vpPoints[0]->Y())) )
                    codRetorno = 2;
                else {
                    // linhas inclinadas: � necess�rio verificar se s�o coincidentes, at� aqui verificou-se que elas
                    // s�o paralelas e que um dos dos v�rtices de uma linha est� dentro do boundary box da outra
                    double coefang_linha_aux = 0.0;
                    double deltax, deltay;
                    // calculo de coeficiente angular de uma linha passando por um v�rtice de cada uma das linhas
                    if((vpPoints[0] != otherline.vpPoints[0])) {
                        deltax = vpPoints[0]->X() - otherline.vpPoints[0]->X();
                        deltay = vpPoints[0]->Y() - otherline.vpPoints[0]->Y();
                    } else {
                        deltax = vpPoints[0]->X() - otherline.vpPoints[1]->X();
                        deltay = vpPoints[0]->Y() - otherline.vpPoints[1]->Y();
                    }

                    coefang_linha_aux =(deltay)/(deltax);
                    if( fabs(coefang_linha_aux - Angular_Coefficient()) < 1e-10 )
                        codRetorno = 2;
                }
            }
        }
    }
    return codRetorno;
}



//-----------------------------------------------------------------------------------------------------
//    IsInBoundaryBox
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o: Verifica se um ponto est� dentro dos limites da linha.
//
//    par�metros:    Um objeto ponto
//
//
//    retorno:    false: o ponto n�o est� dentro dos limites da linha
//                true: o ponto est� dentro dos limites da linha
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Dezembro/2001
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
bool CLevLine::IsInBoundingBox(CLevPoint o_point)
{
    bool codRetorno = false;
    double res_x = 1e-5;
    double res_y = 1e-5;

    if( !(int)Y_Coefficient() ) { // linha eh vertical
        if( (o_point.Y()+res_y >= vpPoints[0]->Y() && o_point.Y()-res_y <= vpPoints[1]->Y())  ||  (o_point.Y()+res_y >= vpPoints[1]->Y() && o_point.Y()-res_y <= vpPoints[0]->Y()) )
            if( fabs(o_point.X() - vpPoints[0]->X()) < res_x )
                codRetorno = true;
    } else {
        if((int)coefficients[1] == 0) { // linha eh horizontal
            if( (o_point.X()+res_x >= vpPoints[0]->X() && o_point.X()-res_x <= vpPoints[1]->X())  ||  (o_point.X()+res_x >= vpPoints[1]->X() && o_point.X()-res_x <= vpPoints[0]->X()) )
                if( fabs(o_point.Y() - vpPoints[0]->Y()) < res_y )
                    codRetorno = true;
        } else { // linha inclinada
            if( (o_point.X()+res_x >= vpPoints[0]->X() && o_point.X()-res_x <= vpPoints[1]->X())  ||  (o_point.X()+res_x >= vpPoints[1]->X() && o_point.X()-res_x <= vpPoints[0]->X()) )
                if( (o_point.Y()+res_y >= vpPoints[0]->Y() && o_point.Y()-res_y <= vpPoints[1]->Y())  ||  (o_point.Y()+res_y >= vpPoints[1]->Y() && o_point.Y()-res_y <= vpPoints[0]->Y()) )
                    codRetorno = true;
        }
    }
    return codRetorno;
}


//-----------------------------------------------------------------------------------------------------
//    PreDiscretize()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:
//
//    par�metros:    Referencia a um vector (STL) de CLevPoints
//
//    retorno:
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Angelo Passaro
//                    Nancy Mieko Abe
//    Data:            Outubro/2001
//    Modificado por:    Nancy
//    Modificado em:    Janeiro/2002
//    Motivo:            A fun��o inicialmente foi criada em LevGeoData
//-----------------------------------------------------------------------------------------------------
void CLevLine::PreDiscretize(vCLevPoint &vo_ptPredis)
{
    double        xp1, yp1, xp2, yp2, xclicked,yclicked;
    double        divFactor;
    double        deltax, deltay;
    int            i,j;//insertpos;
    double        dt0,dt1;
    int            nPoints; //numero de pontos de pre-discretiza��o (inclui v�rtices)
    double        factor;  // fator de divis�o
    CLevPoint    clickedPoint, o_paux;;

    //---------------------------------------------------------------------
    //*NMA*
    //    limpa, redimensiona e inclui pontos na lista que s�o alterados na
    //  fun��o que calcula os pontos de pr�-disretiza��o
    nPoints            = oPredis.NPoints();
    factor            = oPredis.Factor();

    vo_ptPredis.clear();
    vo_ptPredis.reserve(oPredis.NPoints());

    for(j=0;j<nPoints-2;j++) //*NMA*: alterado em 17/05/2005: para que o vetor
    {                         // tenha o tamanho certo sem sobras
        vo_ptPredis.push_back(o_paux);
    }
    //---------------------------------------------------------------------

    clickedPoint.X(oPredis.XClicked());
    clickedPoint.Y(oPredis.YClicked());


    if(nPoints > 2 && factor)
    {
        xp1 = vpPoints[0]->X();
        yp1 = vpPoints[0]->Y();

        xp2 = vpPoints[1]->X();
        yp2 = vpPoints[1]->Y();

        // dependendo do fator sabe-se se � geometrica ou aritm�tica
        // fator = 1 : aritm�tica
        if(factor == 1.) {
            divFactor = (double) (nPoints-1);
            dt0 = dt1 = 1; //neste caso n�o � necess�rio calcular as dist�ncias
        } else {
            divFactor = (1.- pow(factor,(nPoints - 1)))/( 1.- factor);

            xclicked  = clickedPoint.X();
            yclicked  = clickedPoint.Y();

            // calcula distancia do ponto clicado aos pontos vertices
            dt0 = sqrt( (xclicked-xp1)*(xclicked-xp1) + (yclicked-yp1) *(yclicked-yp1));
            dt1 = sqrt( (xclicked-xp2)*(xclicked-xp2) + (yclicked-yp2) *(yclicked-yp2));
        }

        // calcula deltax e deltay
        deltax = (xp2 - xp1)/divFactor;
        deltay = (yp2 - yp1)/divFactor;

        if(dt0 > dt1) {
            // primeiro ponto
            vo_ptPredis[0].X(xp1);
            vo_ptPredis[0].Y(yp1);

            for(i=0;i<(nPoints-3);i++) {
                // calcula coordenadas do ponto
                vo_ptPredis[i].Move(deltax * pow(factor,i), deltay * pow(factor,i));

                // prepara pr�ximo ponto
                vo_ptPredis[i+1].X( vo_ptPredis[i].X());
                vo_ptPredis[i+1].Y( vo_ptPredis[i].Y());
            }

            //ultimo ponto
            vo_ptPredis[i].Move(deltax * pow(factor,i), deltay * pow(factor,i));
        } else {
            //ultimo ponto
            vo_ptPredis[(nPoints-3)].X(xp2);
            vo_ptPredis[(nPoints-3)].Y(yp2);

            for(i=(nPoints-3),j = 0; i>0;i--,j++) {
                // calcula coordenadas do ponto
                vo_ptPredis[i].Move(-deltax * pow(factor,j), -deltay * pow(factor,j));

                // prepara pr�ximo ponto
                vo_ptPredis[i-1].X( vo_ptPredis[i].X());
                vo_ptPredis[i-1].Y( vo_ptPredis[i].Y());
            }

            //primeiro ponto
            vo_ptPredis[i].Move(-deltax * pow(factor,j), -deltay * pow(factor,j));
        }
        if(vo_ptPredis.size())
            oPredis.SetPredisPoints(vo_ptPredis);
    } else {
        if(nPoints <= 2) {
            oPredis.SetPreDiscretizationData(2, oPredis.Factor(), oPredis.XClicked(),oPredis.YClicked() );
            vo_ptPredis.clear();
            vo_ptPredis.reserve(oPredis.NPoints());

            xp1 = vpPoints[0]->X();
            yp1 = vpPoints[0]->Y();

            xp2 = vpPoints[1]->X();
            yp2 = vpPoints[1]->Y();

            //primeiro ponto
            vo_ptPredis[0].X(xp1);
            vo_ptPredis[0].Y(yp1);

            //ultimo ponto
            vo_ptPredis[1].X(xp2);
            vo_ptPredis[1].Y(yp2);

            if(vo_ptPredis.size())
                oPredis.SetPredisPoints(vo_ptPredis);
        }
    }
}


//-----------------------------------------------------------------------------------------------------
//    Save
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o: grava dados de uma linha para um arquivo.
//
//    par�metros:    FILE * parq: ponteiro para arquivo
//                int unidade: fator de conversao para metro //unidade de comprimento
//                FILENAME tipoArq:    qual arquivo est� sendo gravado
//                                    - .ell: MODEL_CONNECTION
//                                    - .mgp: MODEL_GEOMETRY_PREDIS
//    retorno:    int: 0 - sucesso
//
//
//    Observa��es: O argumento FILENAME tipoArq � utilizado para diferencia��o na grava��o
//                de dados de pr�-discretiza��o dos arquivos .ell e .mgp. Quando a grava��o de
//                arquivos do tipo .ell forem abandonados, este par�metro pode ser eliminado.
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Fevereiro/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int    CLevLine::Save(FILE * parq, int unidade, FILENAME tipoArq)
{
    int retCod = 0;

    //Informa��es de Pre-discretiza��o
    if(tipoArq == MODEL_GEOMETRY_PREDIS) { // (.mgp) //*NMA*: arquivo novo (merge .ell + pre)
        if(isOfGeometry)
            fprintf(parq,"\n\n%s %d","reta-segmento ",Id());
        else
            fprintf(parq,"\n\n%s %d","reta-segmento_auxiliar ",Id());

        fprintf(parq,"\n\n%g", vpPoints[0]->X()*pow(10,unidade));
        fprintf(parq,"\n\n%g", vpPoints[0]->Y()*pow(10,unidade));
        fprintf(parq,"\n\n%g", vpPoints[1]->X()*pow(10,unidade));
        fprintf(parq,"\n\n%g", vpPoints[1]->Y()*pow(10,unidade));

        fprintf(parq,"\n\n%d", oPredis.NPoints());
        fprintf(parq,"\n\n%g", oPredis.Factor());
        fprintf(parq,"\n\n%g", oPredis.XClicked());
        fprintf(parq,"\n\n%g", oPredis.YClicked());
    } else if(tipoArq == MODEL_CONNECTION) { // (.ell)
        fprintf(parq,"\n\n%s %d","reta-segmento ",Id());
        fprintf(parq,"\n\n%g", vpPoints[0]->X());
        fprintf(parq,"\n\n%g", vpPoints[0]->Y());
        fprintf(parq,"\n\n%g", vpPoints[1]->X());
        fprintf(parq,"\n\n%g", vpPoints[1]->Y());

        fprintf(parq,"\n\n%d"  , oPredis.NPoints());   //numero de pontos de prediscretizacao
                                            // *NMA* deixado como 0, para manter consistencia para
                                            // leitura deste arquivo pelo edimaq. Nestea vers�o n�o
                                            // est�o sendo gravados dados para recupera��o de pre-discretiza��o
                                            // para o edimaq
    }

    return retCod;
}


//-----------------------------------------------------------------------------------------------------
//    Read
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o: l� dados de uma linha de um arquivo
//
//    par�metros:    FILE * parq: ponteiro para arquivo
//                int unidade: fator de conversao para metro//unidade de comprimento
//                char *tp   : string com tipo da primitiva
//
//    retorno:    int: 0 - sucesso
//
//
//    Observa��es: O vetor de ponteiros para pontos j� deve estar preenchido,
//                    para poder somente ser alterado.
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:            Nancy Mieko Abe
//                    Angelo Passaro
//    Data:            Fevereiro/2002
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevLine::Read(FILE * parq, int unidade, char *tp, FILENAME tipoArq)
{
    int            retCode = 0;
    double        xread0=0.0,yread0=0.0,xread1=0.0,yread1=0.0;
    CLevPoint    p0,p1;

    // vari�veis utilizadas na leitura dos dados de pr�-discretiza��o
    int            nPoints;
    double        x_clicked,y_clicked, factor;
    CLevPoint    o_pointclicked;

    char    reta[]   ="reta-segmento";
    char    reta_aux[]  ="reta-segmento_auxiliar";

//    if(!strcmp(tp,reta)) // inclui linhas
    if(!strcmp(tp,reta) || !strcmp(tp,reta_aux)) // inclui linhas
    {
        fscanf(parq,"%lf", &xread0);
        fscanf(parq,"%lf", &yread0);
        fscanf(parq,"%lf", &xread1);
        fscanf(parq,"%lf", &yread1);

        if(!strcmp(tp,reta_aux)) isOfGeometry = false;

        //leitura de pre-discretiza��o
        //Informa��es de Pre-discretiza��o
        if(tipoArq == MODEL_GEOMETRY_PREDIS)    //(.mgp) //*NMA*: arquivo novo (merge .ell + pre)
        {
            // *NMA: O vetor de ponteiros para os pontos deve ser
            // preenchido previamente para permitir a altera��o

            // *NMA: As coordenadas no arquivo MGP s�o armazenadas em metros
            vpPoints[0]->X(xread0*pow(10,-unidade));
            vpPoints[0]->Y(yread0*pow(10,-unidade));
            vpPoints[1]->X(xread1*pow(10,-unidade));
            vpPoints[1]->Y(yread1*pow(10,-unidade));

            vpPoints[0]->GeoEntityType(GEOM);
            vpPoints[1]->GeoEntityType(GEOM);

            fscanf(parq,"\n%d", &nPoints);
            fscanf(parq,"\n%lf", &factor);
            fscanf(parq,"\n%lf", &x_clicked);
            fscanf(parq,"\n%lf", &y_clicked);

            oPredis.SetPreDiscretizationData(nPoints,factor, x_clicked, y_clicked);
        }
        else
        if(tipoArq == MODEL_CONNECTION)        //(.ell)
        {

            // *NMA: O vetor de ponteiros para os pontos deve ser
            // preenchido previamente para permitir a altera��o

            // *NMA: As coordenadas no arquivo ELL s�o armazenadas na unidade especificada
            vpPoints[0]->X(xread0);
            vpPoints[0]->Y(yread0);
            vpPoints[1]->X(xread1);
            vpPoints[1]->Y(yread1);

            vpPoints[0]->GeoEntityType(GEOM);
            vpPoints[1]->GeoEntityType(GEOM);

            fscanf(parq,"\n\n%d"  , &nPoints);   //numero de pontos de prediscretizacao
                                            // *NMA* deixado como 0, para manter consistencia para
                                            // leitura deste arquivo pelo edimaq. Nestea vers�o n�o
                                            // est�o sendo gravados dados para recupera��o de pre-discretiza��o
            oPredis.SetPreDiscretizationData(nPoints,1.0, 0, 0);
                                            // para o edimaq
        }

    }

    return retCode;
}

CLevPoint CLevLine::GetMiddlePoint(void)
{
    CLevPoint middle;
    double    xm, ym;

    xm = (vpPoints[0]->X() + vpPoints[1]->X())/2.;
    ym = (vpPoints[0]->Y() + vpPoints[1]->Y())/2.;

    middle.X(xm);
    middle.Y(ym);

    return middle;

}

//*NMA*:13/06/2005: escalamento
void CLevLine::Scale(double factor)
{
    if(!fixedEntity)
    {
        vpPoints[0]->Scale(factor);
        vpPoints[1]->Scale(factor);

        TakeLimits();

        fixedEntity = true;
    }

}

