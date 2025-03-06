//=============================================================================
//    Arquivo:            LevRegion.cpp
//.............................................................................
//    Descricao:            Implementacao da classe CLevRegion
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//.............................................................................
//    Observacoes:
//
//=============================================================================

#include "levregion.h"

////////////////////////
//The constructor function
////////////////////////
CLevRegion::CLevRegion()
{
    pt_minxminy.X(1e308); pt_minxminy.Y(1e308); pt_minxminy.Z(1e308);
    pt_maxxmaxy.X(-1e308); pt_maxxmaxy.Y(-1e308); pt_maxxmaxy.Z(1e308);

}


////////////////////////
//The destructor function
////////////////////////
CLevRegion::~CLevRegion()
{
    vpPoints.clear();
    vpLines.clear();
    vpEllipses.clear();

    vElementsRegion.clear();
    vElementsRegionRefined.clear();

}

void CLevRegion::IncludeElementsRegion(vInt & vEl)
{
    vElementsRegion.clear();

    vElementsRegion = vEl;
}

void CLevRegion::IncludeElementsRegionRefined(vInt & vEl)
{
    vElementsRegionRefined.clear();

    vElementsRegionRefined = vEl;
}


void CLevRegion::GetElementsRegion(vInt & vEl)
{
    vEl.clear();
    vEl = vElementsRegion;
}


void CLevRegion::GetElementsRegionRefined(vInt & vEl)
{
    vEl.clear();
    vEl = vElementsRegionRefined;
}


bool CLevRegion::operator == (CLevRegion &a)
{
    unsigned int i;
    bool retorno = true;
    int    ret = -1, num_prim = 0;

    if(NumValidLines() == a.NumValidLines() && NumValidCurves() == a.NumValidCurves()) {
        num_prim = vpPrimitives.size();
        for(i=0; i<vpPrimitives.size() && retorno == true; i++)
            if(vpPrimitives[i]->Status()) {
                ret = a.IsAPrimitive(vpPrimitives[i]);
                if(ret == -1)
                    retorno = false;
            }
    } else
        retorno = false;

    return retorno;
}

CLevRegion & CLevRegion::operator = (const CLevRegion & ro_region)
{

    if (this != &ro_region)
    {

        status                = ro_region.status;

        pt_minxminy            = ro_region.pt_minxminy;
        pt_maxxmaxy            = ro_region.pt_maxxmaxy;

        vpEllipses            = ro_region.vpEllipses;
        vpLines                = ro_region.vpLines;
        vpPoints            = ro_region.vpPoints;
        vpPrimitives        = ro_region.vpPrimitives;

        materialIndex        = ro_region.materialIndex;
        sourceIndex            = ro_region.sourceIndex;
        vElementsRegion        = ro_region.vElementsRegion;
        vPoints                = ro_region.vPoints;

        oPointClicked        = ro_region.oPointClicked;
        permanentMagnetAngle = ro_region.permanentMagnetAngle;

        geoEntityType        = ro_region.geoEntityType;

    }

    return *this;
}


void CLevRegion::SortPoints(void)
{
    unsigned int i,j;
    vInt vLineIncluded, vEllipseIncluded;
    CLevPoint o_point_aux;
    vCLevPoint vDivisionPoints;
    CLevEllipse o_ellipse;

    vPoints.clear();

    bool incluiuTodas, incluiu, incluiuLinha, incluiuEllipse;

    for(i=0; i<vpLines.size(); i++)
        vLineIncluded.push_back(0);

    for(i=0; i<vpEllipses.size(); i++)
        vEllipseIncluded.push_back(0);

    //come�ando com uma linha ou uma ellipse
    // inclui os dois pontos da linha
    if(vpLines.size()) {
        vPoints.push_back(*vpLines[0]->vpPoints[1]);
        vPoints.push_back(*vpLines[0]->vpPoints[0]);

        vLineIncluded[0] = 1;
    } else {
        if(vpEllipses.size()) {
            vPoints.push_back(*vpEllipses[0]->vpPoints[0]);

            o_ellipse = *vpEllipses[0];
            o_ellipse.SetPreDiscretizationData(40,1,o_point_aux);
            o_ellipse.PreDiscretize(vDivisionPoints);

            for(j=0; j<vDivisionPoints.size(); j++)
                vPoints.push_back(vDivisionPoints[j]);

            vPoints.push_back(*vpEllipses[0]->vpPoints[1]);

            vEllipseIncluded[0] = 1;
        }
    }

    incluiuTodas = false;

    do {
        do {
            incluiu = false;

            for(i=0; i<vpLines.size(); i++) {
                if(!vLineIncluded[i]) {
                    if(vPoints[vPoints.size()-1] == *vpLines[i]->vpPoints[0]) {
                        if(IsAPointRegion(*vpLines[i]->vpPoints[1]) == -1)
                            vPoints.push_back(*vpLines[i]->vpPoints[1]);

                        incluiu = true;
                        vLineIncluded[i] = 1;
                    } else {
                        if(vPoints[vPoints.size()-1] == *vpLines[i]->vpPoints[1]) {
                            if(IsAPointRegion(*vpLines[i]->vpPoints[0]) ==-1)
                                vPoints.push_back(*vpLines[i]->vpPoints[0]);

                            incluiu = true;
                            vLineIncluded[i] = 1;
                        }
                    }
                }
            }
        } while(incluiu == true);

        // Procura pelas ellipses
        if(!incluiu) {
            incluiu = false;
            do {
                for(i=0; i<vpEllipses.size() && !incluiu; i++) {
                    if(!vEllipseIncluded[i]) {
                        if(vPoints[vPoints.size()-1] == *vpEllipses[i]->vpPoints[0]) {
                            o_ellipse = *vpEllipses[i];
                            o_ellipse.SetPreDiscretizationData(40,1,o_point_aux);
                            o_ellipse.PreDiscretize(vDivisionPoints);

                            for(j=0; j<vDivisionPoints.size(); j++)
                                vPoints.push_back(vDivisionPoints[j]);

                            if(IsAPointRegion(*vpEllipses[i]->vpPoints[1]) ==-1)
                                vPoints.push_back(*vpEllipses[i]->vpPoints[1]);

                            incluiu = true;

                            vEllipseIncluded[i] = 1;
                        } else {
                            if(vPoints[vPoints.size()-1] == *vpEllipses[i]->vpPoints[1]) {
                                o_ellipse = *vpEllipses[i];
                                o_ellipse.SetPreDiscretizationData(40,1,o_point_aux);
                                o_ellipse.PreDiscretize(vDivisionPoints);

                                for(j=vDivisionPoints.size()-1; j>=0; j--)
                                    vPoints.push_back(vDivisionPoints[j]);

                                if(IsAPointRegion(*vpEllipses[i]->vpPoints[0]) ==-1)
                                    vPoints.push_back(*vpEllipses[i]->vpPoints[0]);

                                incluiu = true;

                                vEllipseIncluded[i] = 1;
                            }
                        }
                    }
                }
            } while(incluiu == true);
        }

        incluiuTodas = true;
        incluiuLinha = false;

        for(i=0; i<vpLines.size() && !incluiuLinha; i++) {
            // No caso de regiao com buraco
            if(vLineIncluded[i] == 0 ) {
                incluiuTodas = false;
                if(IsAPointRegion(*vpLines[i]->vpPoints[0]) == -1)
                    vPoints.push_back(*vpLines[i]->vpPoints[0]);
                if(IsAPointRegion(*vpLines[i]->vpPoints[1]) == -1)
                    vPoints.push_back(*vpLines[i]->vpPoints[1]);

                vLineIncluded[i] = 1;
                incluiuLinha = true;
            }
        }

        if(incluiuLinha == false) {
            incluiuEllipse = false;

            for(i=0; i<vpEllipses.size() && !incluiuEllipse; i++) {
                if(vEllipseIncluded[i] == 0 ) {
                    incluiuTodas = false;

                    if(IsAPointRegion(*vpEllipses[i]->vpPoints[0]) == -1)
                        vPoints.push_back(*vpEllipses[i]->vpPoints[0]);

                    o_ellipse = *vpEllipses[i];
                    o_ellipse.SetPreDiscretizationData(40,1,o_point_aux);
                    o_ellipse.PreDiscretize(vDivisionPoints);

                    for(j=0; j<vDivisionPoints.size(); j++)
                        vPoints.push_back(vDivisionPoints[j]);

                    if(IsAPointRegion(*vpEllipses[i]->vpPoints[1]) == -1)
                        vPoints.push_back(*vpEllipses[i]->vpPoints[1]);

                    vEllipseIncluded[i] = 1;

                    incluiuEllipse = true;
                }
            }
        }
    } while(incluiuTodas == false);
}


int CLevRegion::IsAPointRegion(CLevPoint& p)
{
    unsigned int i;
    int ret = -1;

    for(i=0; i<vPoints.size() && ret==-1; i++)
        if(vPoints[i] == p)
            ret = i;

    return ret;

}
void CLevRegion::SetIdentificationPoint(CLevPoint & p)
{
    oPointClicked = p;

}
void CLevRegion::GetIdentificationPoint(CLevPoint & p)
{
    p = oPointClicked;

}

double CLevRegion::GetPermanentMagnetAngle(void)
{
    return permanentMagnetAngle;
}

void CLevRegion::SetPermanentMagnetAngle(double a)
{
    permanentMagnetAngle = a;
}


bool CLevRegion::IsInBoundingBox(CLevPoint &o_point)
{
    bool codRetorno = false;
    if(o_point.X() >= pt_minxminy.X() && o_point.X() <= pt_maxxmaxy.X())
        if(o_point.Y() >= pt_minxminy.Y() && o_point.Y() <= pt_maxxmaxy.Y())
            codRetorno = true;
    return codRetorno;
}


void CLevRegion::Reinitialize(void)
{
    CLevGroup::Reinitialize();

    vElementsRegion.clear();
    vElementsRegionRefined.clear();
}
