//=============================================================================
//    Arquivo:            Subregion.cpp
//.............................................................................
//    Descricao:          Implementacao da classe CSubRegion
//.............................................................................
//    Autores:
//                        Nancy Mieko Abe
//.............................................................................
//    Observacoes:
//
//
//=============================================================================

#include "subregion.h"
#include "levgroup.h"

////////////////////////
//The constructor function
////////////////////////
CSubRegion::CSubRegion()
{
    number = 0;
    modelRegionIndex = -1;
    localSouthFrontierIndex = -1;
}


////////////////////////
//The destructor function
////////////////////////
CSubRegion::~CSubRegion()
{
    vpFrontiers.clear();
}

//-----------------------------------------------------------------------------------------------------
//    IncludeFrontier()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:
//
//    par�metros:
//
//    retorno: numero de fronteiras da sub-regi�o
//
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:
//                    Nancy Mieko Abe
//    Data:            Mar�o/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CSubRegion::IncludeFrontier(CLevFrontier *f)
{
    int num_front, retorno;

    if (!vpFrontiers.size())
    {  // ainda n�o havia fronteira na lista
        vpFrontiers.push_back(f);
        num_front = vpFrontiers.size();
    }
    else
    {
        retorno = IsAFrontier(f);
        if (retorno == -1)
        {
           vpFrontiers.push_back(f);
        }
    }

    num_front = vpFrontiers.size();

    return num_front;

}

//-----------------------------------------------------------------------------------------------------
//    IsAFrontier()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:
//
//    par�metros:
//
// valor de retorno: -1 se fronteira *f n�o pertence � lista local
//                  >=0 indica posi��o na lista
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:
//                    Nancy Mieko Abe
//    Data:            Mar�o/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CSubRegion::IsAFrontier(CLevFrontier *f)
{
    CLevFrontier *pof_aux;
    int retorno = -1;
    for(unsigned int i=0; i<vpFrontiers.size(); i++) {
        if(vpFrontiers[i]->Status()) {
            pof_aux = vpFrontiers[i];
            if (vpFrontiers[i] == f) {
                retorno = i;
                break;
            }
        }
    }
    return retorno;
}


void CSubRegion::SetAssociatedModelRegionIndex(int ind)
{
    int i, num_front;
    CLevFrontier *pf;


    modelRegionIndex = ind;

    num_front = vpFrontiers.size();

    //atualiza tamb�m nas fronteiras
    for(i=0;i<num_front;i++)
    {
        pf = vpFrontiers[i];
        pf->SetSubRegionIndex(ind);
    }
}


int CSubRegion::GetAssociatedModelRegionIndex(void)
{
    return modelRegionIndex ;
}


void CSubRegion::Reinitialize(void)
{
    vpFrontiers.clear();

    number = 0;
    modelRegionIndex = -1;

}


bool CSubRegion::operator == (CSubRegion &s)
{
    unsigned int i;
    int ret = -1;
    bool retorno = true;

    if(GetNumFrontiers() == s.GetNumFrontiers()) {
        for(i=0;i<vpFrontiers.size() && retorno == true;i++) {
            if(vpFrontiers[i]->Status()) {
                ret = s.IsAFrontier(vpFrontiers[i]);
                if(ret==-1)
                    retorno = false;
            }
        }
    }
    else
        retorno = false;

    return retorno;
}


int CSubRegion::GetNumFrontiers(void)
{
    return vpFrontiers.size();
}


//-----------------------------------------------------------------------------------------------------
//    void CSubRegion::SortFrontiers(void)
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:
//        Ordena as fronteiras no sentido anti-hor�rio, em rela��o ao baricentro da regi�o.
//        A sequencia ordenada � armazenada por �ndice local das fronteiras da sub-regi�o.
//        Inicialmente ordena-se as primitivas das fronteiras por conex�es de pontos
//        (a sequencia ordenada de primitivas da fronteora s� ser� usada na grava��o dos pontos).
//        A ordena��o � realizada seguindo-se o procedimento:
//        1. Determina-se o baricentro da sub-regi�o;
//        2. Inicia-se com a fronteira na posi��o 0 do vetor de fronteiras;
//        3. Procura as duas fronteiras que estejam ligadas a primeira fronteira (pontos extremos em comum);
//        4. Verifica qual das fronteiras est� em sentido anti-hor�rio, usando o baricentro e pontos
//            internos �s fronteiras;
//        5. Insere a fronteira em sentido anti-hor�rio, inclui a outra fronteira que n�o est� ligada
//            a fronteira da posi��o 0 e finalmente, inclui a outra fronteira ligada
//
//    par�metros:
//
// valor de retorno:    0: sucesso
//                        1: existem mais de duas conex�es a uma primitiva
//                        2: existem mais do que quatro fronteiras
//                        3: inconsistencia de dados em alguma fronteira
//    Observa��es:
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:
//                    Nancy Mieko Abe
//    Data:            Abril/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CSubRegion::SortFrontiers(void)
{
    CLevPoint    p0, p1;
    vInt        vPrimConnectedAux;
    bool        isIncluded = false;
    unsigned int i, j, indexPrimNotIncluded;
    int            isCCW = -2;
    int            retorno = 0;
    unsigned int size = 0;
    int            codeReturn = 0;
    CLevPoint    o_point_aux0, o_point_aux1;

    ComputeBaricentro();

    // reinicializa vetor com o �ndice das primitivas ordenadas no sentido anti-hor�rio
    vIndexCCWSortedFrontiers.clear();

    size = vpFrontiers.size();

    if(size == 4) {

        // ordena as fronteiras em fun��o dos pontos de conex�o
        for(i=0;i<size && !codeReturn;i++)
        {
            codeReturn = vpFrontiers[i]->ConnectionPointsSortPrimitives();
            //0 : ordena��o realizada com sucesso
            //1 : existem primitivas desconectadas
            //2 : a fronteira forma um pol�gono
            //3 : inconsistencia de dados
        }

        // libera a altera��o das primitivas das fronteiras
        for(i=0;i<size; i++)
            for(j=0;j<vpFrontiers[i]->vpPrimitives.size();j++)
                vpFrontiers[i]->vpPrimitives[j]->FixEntity(false); // fixa a primitiva para que n�o seja alterada em passo posterior

        if(codeReturn)
            retorno =3;
        else {
            //insere a fronteira da posi��o 0
            vIndexCCWSortedFrontiers.push_back(0);

            //procura as outras duas fronteiras ligadas
            for(i=1;i<vpFrontiers.size();i++) {
                o_point_aux0 = *vpFrontiers[0]->GetPointOnePrimitive(0);
                o_point_aux1 = *vpFrontiers[0]->GetPointOnePrimitive(1);
                if(vpFrontiers[i]->IsAPoint(vpFrontiers[0]->GetPointOnePrimitive(0)))
                    vPrimConnectedAux.push_back(i);
                else if(vpFrontiers[i]->IsAPoint(vpFrontiers[0]->GetPointOnePrimitive(1)))
                    vPrimConnectedAux.push_back(i);
            }

            // verifica se foram encontradas duas fronteiras e
            // verifica qual delas deve ser inserida na sequencia (CCW)
            // insere a primitiva adequada na sequencia
            // insere a �nica primitiva que sobrou (n�o � a da posi��o 0 e nenhuma das duas ligadas)
            // insere a outra primitiva ligada � primitiva da posi��o 0
            if(vPrimConnectedAux.size() == 2) {
                p0 = vpFrontiers[0]->vpPrimitives[0]->GetMiddlePoint();
                p1 = vpFrontiers[vPrimConnectedAux[0]]->vpPrimitives[0]->GetMiddlePoint();
                isCCW = CCW(oBaricentro,p0,p1);

                if(isCCW == 1) { // est�o ordenadas no sentido anti-horario, inclui
                    vIndexCCWSortedFrontiers.push_back(vPrimConnectedAux[0]);
                    indexPrimNotIncluded = vPrimConnectedAux[1];
                } else {
                    // a primeira fronteira ligada encontrada n�o est� no sentido anti-hor�rio insere a outra
                    vIndexCCWSortedFrontiers.push_back(vPrimConnectedAux[1]);
                    indexPrimNotIncluded = vPrimConnectedAux[0];
                }

                for(i=1;i<4 && isIncluded == false;i++) {
                    if((signed)i != vIndexCCWSortedFrontiers[0] && (signed)i != vIndexCCWSortedFrontiers[1]
                        && i != indexPrimNotIncluded) {
                        vIndexCCWSortedFrontiers.push_back(i);
                        vIndexCCWSortedFrontiers.push_back(indexPrimNotIncluded);
                        isIncluded = true;
                    }
                }
            } else
                retorno = 1; // encontrou mais de duas conex�es a uma primitiva
        }
    } else
        retorno = 2; // existem mais do que 4 fronteiras

    if(retorno)
        vpFrontiers.clear();

    return retorno;
}


void CSubRegion::ComputeBaricentro(void)
{
    unsigned int  i, j;
    double        xb = 0., yb = 0.;
    double        xmax = 0., xmin = 0., ymax = 0., ymin = 0.;
    CLevEllipse *po_ellipse = NULL;
    CLevPoint    p0,p1;
    double        aux;

    //inicializa vari�veis com valores do ponto 0 da primeira primitiva da primeira fronteira
    xmin = xmax = vpFrontiers[0]->vpPrimitives[0]->vpPoints[0]->X();
    ymin = ymax = vpFrontiers[0]->vpPrimitives[0]->vpPoints[0]->Y();

    for(i=0;i<vpFrontiers.size();i++) {
        for(j=0;j<vpFrontiers[i]->vpPrimitives.size();j++) {
            switch(vpFrontiers[i]->vpPrimitives[j]->GeoEntityType()) {
                case LINELEV:
                    vpFrontiers[i]->vpPrimitives[j]->Limits(p0,p1);
                    break;
                case ELLIPSELEV:
                    po_ellipse = (CLevEllipse *) vpFrontiers[i]->vpPrimitives[j];
                    po_ellipse->Limits(p0,p1);
                    break;
            }

            aux = p0.X();
            if(p0.X() < xmin)
                xmin = p0.X();

            if(p0.X() > xmax)
                xmax = p0.X();

            aux = p0.Y();
            if(p0.Y() < ymin)
                ymin = p0.X();

            if(p0.Y() > ymax)
                ymax = p0.X();

            aux = p1.X();
            if(p1.X() < xmin)
                xmin = p1.X();

            if(p1.X() > xmax)
                xmax = p1.X();

            aux = p1.Y();
            if(p1.Y() < ymin)
                ymin = p1.Y();

            if(p1.Y() > ymax)
                ymax = p1.Y();
        }
    }

    xb = (xmax + xmin)/2;
    yb = (ymax + ymin)/2;

    oBaricentro.X(xb);
    oBaricentro.Y(yb);
}


//-----------------------------------------------------------------------------------------------------
//    CCW()
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:     Verifica qual o sentido (hor�rio, anti-hor�rio) em que se percorre
//                os pontos p0, p1, p2
//
//    par�metros:    p0, p1, p2
//
// valor de retorno: 1: se se percorre em sentido anti-hor�rio
//                     -1: se se percorre em sentido hor�rio
//
//                    no caso de os pontos serem colineares:
//                    1: p1 est� entre p0 e p2
//                    -1: p0 est� entre p2 e p1
//                    0: p2 est� entre p0 e p1
//
//    Observa��es: Algoritmo obtido do livro Algorithms in C, de Robert Sedgewick, Addison-Wesley
//                  cap�tulo 24, p�g. 350;
//
//......................................................................................................
//    Vers�o:            1.0
//    Autor:
//                    Nancy Mieko Abe
//    Data:            Mar�o/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CSubRegion::CCW(CLevPoint &p0, CLevPoint &p1, CLevPoint &p2)
{
    double dx1, dx2, dy1, dy2;

    dx1 = p1.X() - p0.X();
    dy1 = p1.Y() - p0.Y();

    dx2 = p2.X() - p0.X();
    dy2 = p2.Y() - p0.Y();

    if(dx1*dy2 > dy1*dx2)
        return 1;

    if(dx1*dy2 < dy1*dx2)
        return -1;

    if( (dx1*dx2<0) || (dy1*dy2<0))
        return -1;

    if( (dx1*dx1 + dy1*dy1) < (dx2*dx2 + dy2*dy2))
        return 1;

    return 0;
}


void CSubRegion::SetLocalSouthFrontierIndex(int ind)
{
    if(ind >=0 && ind<4)
        localSouthFrontierIndex = ind;
}


int CSubRegion::GetLocalSouthFrontierIndex(void)
{
    return localSouthFrontierIndex;
}


int CSubRegion::GetLocalEastFrontierIndex(void)
{
    unsigned int i;
    int index, indexCCW = 0;
    bool found=false;

    // identifica, no vetor vIndexCCWSortedFrontiers, a fronteira Sul

    for(i=0; i<vIndexCCWSortedFrontiers.size() && found == false; i++)
        if(vIndexCCWSortedFrontiers[i] == localSouthFrontierIndex) {
            found = true;
            indexCCW = i;
        }

    if(indexCCW == 3)
        index = vIndexCCWSortedFrontiers[0];
    else
        index = vIndexCCWSortedFrontiers[indexCCW+1];

    return index;
}


int CSubRegion::GetLocalWestFrontierIndex(void)
{
    unsigned int i;
    int index, indexCCW = 0;
    bool found=false;

    // identifica, no vetor vIndexCCWSortedFrontiers, a fronteira Sul
    for(i=0; i<vIndexCCWSortedFrontiers.size() && found == false; i++)
        if(vIndexCCWSortedFrontiers[i] == localSouthFrontierIndex) {
            found = true;
            indexCCW = i;
        }

    if(indexCCW == 0)
        index = vIndexCCWSortedFrontiers[3];
    else
        index = vIndexCCWSortedFrontiers[indexCCW-1];

    return index;
}


int CSubRegion::GetLocalNorthFrontierIndex(void)
{
    unsigned int i;
    int index, indexCCW = 0;
    bool found=false;

    // Identifica, no vetor vIndexCCWSortedFrontiers, a fronteira Sul

    for(i=0; i<vIndexCCWSortedFrontiers.size() && found == false; i++)
        if(vIndexCCWSortedFrontiers[i] == localSouthFrontierIndex) {
            found = true;
            indexCCW = i;
        }

    if(indexCCW == 2)
        index = vIndexCCWSortedFrontiers[0];
    else if(indexCCW == 3)
        index = vIndexCCWSortedFrontiers[1];
    else index = vIndexCCWSortedFrontiers[indexCCW+2];

    return index;
}


CLevFrontier * CSubRegion::GetLocalSouthFrontier(void)
{
    if(localSouthFrontierIndex >=0 )
        return vpFrontiers[localSouthFrontierIndex];
    else
        return NULL;
}


int CSubRegion::CheckFrontiersPredisConsistency(void)
{
    //0 : sucesso
    //1 : fronteiras Sul-Norte inconsistentes
    //2 : fronteiras Leste-Oeste inconsistentes

    int index_a, index_b;
    int retorno = 0;

    index_a = vIndexCCWSortedFrontiers[0];
    index_b = vIndexCCWSortedFrontiers[2];

    if(vpFrontiers[vIndexCCWSortedFrontiers[0]]->GetNumOfPredisSegments() != vpFrontiers[vIndexCCWSortedFrontiers[2]]->GetNumOfPredisSegments())
    {
        if(vIndexCCWSortedFrontiers[0] == localSouthFrontierIndex || vIndexCCWSortedFrontiers[2] == localSouthFrontierIndex)
            retorno = 1;
        else
            retorno = 2;
    }

    index_a = vIndexCCWSortedFrontiers[1];
    index_b = vIndexCCWSortedFrontiers[3];

    if(vpFrontiers[vIndexCCWSortedFrontiers[1]]->GetNumOfPredisSegments() != vpFrontiers[vIndexCCWSortedFrontiers[3]]->GetNumOfPredisSegments())
    {
        if(vIndexCCWSortedFrontiers[1] == localSouthFrontierIndex || vIndexCCWSortedFrontiers[3] == localSouthFrontierIndex)
            retorno = 1;
        else
            retorno = 2;
    }

    return retorno;
}


int CSubRegion::GetNumOfPredisSegmentsSouthFrontier(void)
{
    int num = vpFrontiers[localSouthFrontierIndex]->GetNumOfPredisSegments();
    return num;
}

int CSubRegion::GetNumOfPredisSegmentsEastFrontier(void)
{
    int num, east_index;

    east_index = GetLocalEastFrontierIndex();
    num = vpFrontiers[east_index]->GetNumOfPredisSegments();

    return num;
}


void CSubRegion::BuildFrontiersPrediscretizationVectors(void)
{
    unsigned i, j;
    CLevPoint    o_piA, o_pfA,o_piB, o_pfB;
    vCLevPoint    vPointsAux;
    CLevPoint    o_pp;
    int            indFrontierSouth, indFrontierEast,indFrontierNorth,indFrontierWest;
    bool        inverterSul = false, inverterLeste = false, inverterNorte = false, inverterOeste = false;

    // constroi o vetor vFrontierPoints de cada fronteira
    for(i=0;i<4;i++)
        vpFrontiers[i]->BuildPrediscretizationVector();

    //ordena��o dos pontos do vetor vPointsFrontier das fronteiras considerando que:
    // - come�a-se pela fronteira Sul:
    // - para a fronteira SUL, o ponto inicial � aquele que n�o � o ponto comum
    //         com a proxima fronteira (LESTE)
    // - para fronteira LESTE: o primeiro ponto � o ponto que � comum com a fronteira SUL
    // - para a fronteira NORTE: o primeiro ponto � o ponto comum com a fronteira LESTE
    // - para a fronteira OESTE: o primeiro ponto � o ponto comum com a fronteira NORTE


    // ordena��o dos pontos da fronteira SUL ==================================================
    indFrontierSouth = GetLocalSouthFrontierIndex();
    o_piA = vpFrontiers[indFrontierSouth]->vPointsFrontier[0];
    o_pfA = vpFrontiers[indFrontierSouth]->vPointsFrontier[vpFrontiers[indFrontierSouth]->vPointsFrontier.size()-1];

    indFrontierEast = GetLocalEastFrontierIndex();
    o_piB = vpFrontiers[indFrontierEast]->vPointsFrontier[0];
    o_pfB = vpFrontiers[indFrontierEast]->vPointsFrontier[vpFrontiers[indFrontierEast]->vPointsFrontier.size()-1];

    if(o_piA == o_piB || o_piA == o_pfB) inverterSul = true;

    if(inverterSul)
    {
        vPointsAux.clear();
        for(j=vpFrontiers[indFrontierSouth]->vPointsFrontier.size()-1; j>=0;j--)
        {
            o_pp = vpFrontiers[indFrontierSouth]->vPointsFrontier[j];
            vPointsAux.push_back(o_pp);
        }

        int num_p = vPointsAux.size();

        vpFrontiers[indFrontierSouth]->vPointsFrontier.clear();

        num_p = vPointsAux.size();

        for(j=0;j<vPointsAux.size();j++) {
            o_pp = vPointsAux[j];
            vpFrontiers[indFrontierSouth]->vPointsFrontier.push_back(o_pp);
        }
    }

    //obtem novamente o ponto final da sul
    o_pfA = vpFrontiers[indFrontierSouth]->vPointsFrontier[vpFrontiers[indFrontierSouth]->vPointsFrontier.size()-1];
    //obtem novamente o ponto final da leste
    o_pfB = vpFrontiers[indFrontierEast]->vPointsFrontier[vpFrontiers[indFrontierEast]->vPointsFrontier.size()-1];

    // ordena��o dos pontos da fronteira LESTE ==================================================
    if(o_pfA == o_pfB) inverterLeste = true;

    if(inverterLeste)
    {
        vPointsAux.clear();
        for(j=vpFrontiers[indFrontierEast]->vPointsFrontier.size()-1; j>=0;j--)
        {
            o_pp = vpFrontiers[indFrontierEast]->vPointsFrontier[j];
            vPointsAux.push_back(o_pp);
        }

        int num_p = vPointsAux.size();

        vpFrontiers[indFrontierEast]->vPointsFrontier.clear();

        num_p = vPointsAux.size();

        for(j=0;j<vPointsAux.size();j++)
        {
            o_pp = vPointsAux[j];
            vpFrontiers[indFrontierEast]->vPointsFrontier.push_back(o_pp);
        }
    }// fecha if(inverterLeste)
    //obtem novamente o ponto final da leste
    o_pfB = vpFrontiers[indFrontierEast]->vPointsFrontier[vpFrontiers[indFrontierEast]->vPointsFrontier.size()-1];

    // ordena��o dos pontos da fronteira NORTE ==================================================
    indFrontierNorth = GetLocalNorthFrontierIndex();
    o_piA = vpFrontiers[indFrontierNorth]->vPointsFrontier[0];
    o_pfA = vpFrontiers[indFrontierNorth]->vPointsFrontier[vpFrontiers[indFrontierNorth]->vPointsFrontier.size()-1];

    //o_pfB: final da leste o_pfA: final da Norte
    if(o_pfB == o_pfA) inverterNorte = true;

    if(inverterNorte)
    {
        vPointsAux.clear();
        for(j=vpFrontiers[indFrontierNorth]->vPointsFrontier.size()-1; j>=0;j--)
        {
            o_pp = vpFrontiers[indFrontierNorth]->vPointsFrontier[j];
            vPointsAux.push_back(o_pp);
        }

        int num_p = vPointsAux.size();

        vpFrontiers[indFrontierNorth]->vPointsFrontier.clear();

        num_p = vPointsAux.size();

        for(j=0;j<vPointsAux.size();j++)
        {
            o_pp = vPointsAux[j];
            vpFrontiers[indFrontierNorth]->vPointsFrontier.push_back(o_pp);
        }

    }// fecha if(inverterNorte)
    //obtem novamente o ponto final da Norte
    o_pfB = vpFrontiers[indFrontierNorth]->vPointsFrontier[vpFrontiers[indFrontierNorth]->vPointsFrontier.size()-1];

    // ordena��o dos pontos da fronteira OESTE ==================================================
    indFrontierWest = GetLocalWestFrontierIndex();
    o_piA = vpFrontiers[indFrontierWest]->vPointsFrontier[0];
    o_pfA = vpFrontiers[indFrontierWest]->vPointsFrontier[vpFrontiers[indFrontierWest]->vPointsFrontier.size()-1];

    if(o_pfA == o_pfB) inverterOeste = true;

    if(inverterOeste)
    {
        vPointsAux.clear();
        for(j=vpFrontiers[indFrontierWest]->vPointsFrontier.size()-1; j>=0;j--)
        {
            o_pp = vpFrontiers[indFrontierWest]->vPointsFrontier[j];
            vPointsAux.push_back(o_pp);
        }

        int num_p = vPointsAux.size();

        vpFrontiers[indFrontierWest]->vPointsFrontier.clear();

        num_p = vPointsAux.size();

        for(j=0;j<vPointsAux.size();j++)
        {
            o_pp = vPointsAux[j];
            vpFrontiers[indFrontierWest]->vPointsFrontier.push_back(o_pp);
        }

        //atualiza o ponto final
        o_pfB = vpFrontiers[indFrontierWest]->vPointsFrontier[vpFrontiers[indFrontierWest]->vPointsFrontier.size()-1];

    }// fecha if(inverterOeste)

} //end of void CSubRegion::BuildFrontiersPrediscretizationVectors(void)
