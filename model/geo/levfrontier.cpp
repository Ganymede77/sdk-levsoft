//=============================================================================
//    Arquivo:            LevGroup.h
//.............................................................................
//    Descricao:          Interface da classe CLevGroup
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//
//-----------------------------------------------------------------------------
//    Classe CLevFrontier
//-----------------------------------------------------------------------------
//    Dados membros:
//
//    Associacoes:
//
//    Observacoes:
//
//.............................................................................
//    Data:                Abril/2002
//    Modificado em:
//    Modificado por:
//    Motivo:
//=============================================================================
#include "levfrontier.h"


CLevFrontier::CLevFrontier()
{
    numPredisSegments = 0;
}


CLevFrontier::~CLevFrontier()
{
    unsigned long i;

    vSubregionsIndex.clear();

    for(i=0; i<vpRelatedFrontiers.size(); i++)
        if(vpRelatedFrontiers[i]) {
            delete vpRelatedFrontiers[i]; vpRelatedFrontiers[i] = NULL;
        }
    vpRelatedFrontiers.clear();

    for(i=0; i<vpPointOnePrimitive.size(); i++)
        if(vpPointOnePrimitive[i]) {
            delete vpPointOnePrimitive[i]; vpPointOnePrimitive[i] = NULL;
        }
    vpPointOnePrimitive.clear();
}

// Especializaï¿½ï¿½o para fronteiras
int CLevFrontier::IsAPoint(CLevPoint  *p)
{
    // valor de retorno: 0 se ponto *p nï¿½o pertence ï¿½ nenhuma primitiva
    //                   1 se o ponto *p ï¿½ um dos pontos extremos
    int retorno = 0;

    for(unsigned int i=0; i<vpPointOnePrimitive.size(); i++) {
        if(vpPointOnePrimitive[i]->Status())
            if(vpPointOnePrimitive[i] == p) {
                retorno = 1;
                break;
            }
    }
    return retorno;
}


//-----------------------------------------------------------------------------------------------------
//    VerifyConnections
//-----------------------------------------------------------------------------------------------------
//
//    Descriï¿½ï¿½o: Verifica a quantas primitivas pertence um dado ponto
//
//    parï¿½metros:    Um ponteiro para um objeto CLevPoint
//
//    retorno:    nï¿½mero de primitivas as quais o ponto pertence
//
//    Observaï¿½ï¿½es:
//......................................................................................................
//    Versï¿½o:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Abril/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
short int CLevFrontier::VerifyConnections(CLevPoint  *p)
{
    short int num = 0;
    unsigned int i;

    for(i=0; i<vpPrimitives.size(); i++) {
        if(vpPrimitives[i]->vpPoints[0] == p)
            num++;
        else if(vpPrimitives[i]->vpPoints[1] == p)
            num++;
    }
    return num;
}


//-----------------------------------------------------------------------------------------------------
//    ConnectionPointsSortPrimitives
//-----------------------------------------------------------------------------------------------------
//
//    Descriï¿½ï¿½o: Ordena as primitivas em seguencia de ligaï¿½ï¿½o de pontos
//
//    parï¿½metros:    nenhum
//
//    retorno:    0 : ordenaï¿½ï¿½o realizada com sucesso
//                1 : existem primitivas desconectadas
//                2 : a fronteira forma um polï¿½gono
//                3 : inconsistencia de dados
//
//    Observaï¿½ï¿½es:
//        Verifica inicialmente se todas as primitivas estï¿½o conectadas.
//        Somente os dois pontos extremos da fronteira devem pertencer a somente uma primitiva,
//        No caso de existirem mais de dois pontos pertencentes a somente um primitiva, significa
//        que existem primitivas desconectadas.
//        No caso de nï¿½o existir nenhum ponto pertencentes a somente um primitiva, significa que
//        a fronteira forma um polï¿½gono.
//......................................................................................................
//    Versï¿½o:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Abril/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevFrontier::ConnectionPointsSortPrimitives(void)
{
    unsigned int i, j, num_prim, num_prim_included;
    int retorno = 0;
    vInt        vPrimIndex;        // vetor auxiliar usado para armazernar o ï¿½ndice, do vetor vpPrimitives, da primitiva que tem ponto extremo
	CLevPoint	*poPointAux;	// utilizado, quando necessário, para a troca da ordem dos pontos em linhas
    bool        isIncluded;        // flag que indica de um ponto jï¿½ foi incluï¿½do na lista de pontos pertencentes a somente uma primitiva
    short int num_connections;    // nï¿½mero de conexï¿½es de um ponto (ou seja, a quantas primitivas ele pertence)
    vpCLevPrimitive vpPrimAux;    // vetor auxiliar usado para armazernar os ponteiros para primitivas ordenadas
    vInt        vPrimIncluded;    // vetor auxiliar usado como flag para indicar se uma primitiva jï¿½
                                // foi incluï¿½da na lista ordenada auxiliar
    int            ind;
	int			indEllipse		= -1; // posição da elipse no vetor vpPrimitives
	bool		thereIsAnEllipse = false; //flag para indicar se existe uma elipse na fronteira
	bool		isSequenceInverted; // flag que indica se a sequencia inicial armazenada em vpPrimAux
									// está invertida, ou seja, existe pelo menos uma elipse o armazenamento
									// não está seguindo a ordem p0 atual igual a p1 da primitiva anterior
									// Como não é possível alterar p0 para p1 em elipses, ordena-se as primitivas
									// no sentido inverso (ou seja, començando-se pelo outro ponto extremo)
	bool		jaInverteu;			// flag que indica que a sequencia já foi invertida, ou seja, iniciou-se com o ponto
									// vpPointOnePrimitive[1]. A primeira tentativa é com vpPointOnePrimitive[0]
    CLevPoint    o_point_aux;
	int			aux=0;

    vpPointOnePrimitive.clear();

    // Identifica os pontos que pertencem a somente uma primitiva e o ï¿½ndice destas primitivas
    for(i=0; i<vpPrimitives.size(); i++) {
        //verifica ponto p0
        isIncluded = false;
        o_point_aux = *vpPrimitives[i]->vpPoints[0];
        num_connections = VerifyConnections(vpPrimitives[i]->vpPoints[0]);
        if(num_connections == 1) {
            for(j=0; j<vpPointOnePrimitive.size() && isIncluded == false; j++)
                if(vpPointOnePrimitive[j] == vpPrimitives[i]->vpPoints[0])
                    isIncluded = true;

            if(isIncluded == false) {
                vpPointOnePrimitive.push_back(vpPrimitives[i]->vpPoints[0]);
                vPrimIndex.push_back(i);
            }
        }

        // verifica ponto p1
        isIncluded = false;
        o_point_aux = *vpPrimitives[i]->vpPoints[1];
        num_connections = VerifyConnections(vpPrimitives[i]->vpPoints[1]);
        if(num_connections == 1) {
            for(j=0; j<vpPointOnePrimitive.size() && isIncluded == false; j++)
                if(vpPointOnePrimitive[j] == vpPrimitives[i]->vpPoints[1])
                    isIncluded = true;

            if(isIncluded == false) {
                vpPointOnePrimitive.push_back(vpPrimitives[i]->vpPoints[1]);
                vPrimIndex.push_back(i); //inclui o indice da primitiva que contem o OnePrimitive ponto
            }
        }
    }

    if(vpPointOnePrimitive.size() == 2 && !retorno) //nï¿½o existem primitivas desconectadas
    {
        vpPrimAux.clear();

        //inicializa com flag 0 (nï¿½o incluï¿½do) o vetor vPrimIncluded
        for(i=0;i<vpPrimitives.size();i++)
            vPrimIncluded.push_back(0);

        //recupera o indice da primitiva que contï¿½m o OnePrimitive ponto 0
        ind = vPrimIndex[0];
        //inclui a primitiva na lista
        vpPrimAux.push_back(vpPrimitives[ind]);
        //atualiza o flag de inclusao
        vPrimIncluded[ind] =1;

        // varre as outras primitivas procurando aquela que tem um ponto de ligaï¿½ï¿½o com a
        // primitiva anterior.
        // a inclusï¿½o deve parar quando o nï¿½mero de primitivas incluï¿½das no vetor for igual
        // ao nï¿½mero de primitivas da fronteira.

        num_prim = vpPrimitives.size();
        num_prim_included = 1;// a primitiva do OnePrimitive ponto 0 jï¿½ foi incluida

        do {
            for(i=0;i<num_prim;i++) {
                if(!vPrimIncluded[i]) { // a primitiva nï¿½o foi incluida
                    //verifica se um dos pontos da primitiva ï¿½ igual ao ponto da primitiva anterior
                    if( vpPrimitives[i]->vpPoints[0] == vpPrimAux[num_prim_included-1]->vpPoints[0] ||
                        vpPrimitives[i]->vpPoints[0] == vpPrimAux[num_prim_included-1]->vpPoints[1] ||
                        vpPrimitives[i]->vpPoints[1] == vpPrimAux[num_prim_included-1]->vpPoints[0] ||
                        vpPrimitives[i]->vpPoints[1] == vpPrimAux[num_prim_included-1]->vpPoints[1]  )
                    {
                        vpPrimAux.push_back(vpPrimitives[i]);
                        vPrimIncluded[i] =1;
                        num_prim_included++;
                    }
                }// fecha if(!vPrimIncluded[i])
            }//fecha for(i=1;i<num_prim;i++)

        }while(num_prim_included!=num_prim);


        if(!retorno) { // atualiza vpPrimitives com as primitivas ordenadas
            vpPrimitives.clear();
            for(i=0; i<vpPrimAux.size(); i++)
                vpPrimitives.push_back(vpPrimAux[i]);
        }
        vpPrimAux.clear();

    } else if(vpPointOnePrimitive.size() > 2)
        retorno = 1;
    else if(!retorno)
        retorno = 2;

    if(retorno) //ocorreu algum erro
        vpPointOnePrimitive.clear();

    return retorno;

}


void CLevFrontier::UpdateNumOfPredisSegments(int np)
{
    // atualiza quando da prï¿½-discretizaï¿½ï¿½o de uma das primitivas
}


int    CLevFrontier::GetNumOfPredisSegments(void)
{
    unsigned int i;
    int num_predis_points;
    numPredisSegments = 0;

    for(i=0;i<vpPrimitives.size();i++) {
        // nï¿½mero de segmentos = nï¿½mero de pontos - 1
        num_predis_points = vpPrimitives[i]->oPredis.NPoints();

        // ainda nï¿½o entrou na pre-discretizaï¿½ï¿½o
        if(!num_predis_points) {
            switch(vpPrimitives[i]->GeoEntityType()) {
                case LINELEV:
                    num_predis_points = 2;
                    break;
                case ELLIPSELEV:
                    num_predis_points = 3;
                    break;
            }
        }
        numPredisSegments += num_predis_points - 1;
    }
    return numPredisSegments;
}


void CLevFrontier::SetSubRegionIndex(int ind)
{
    vSubregionsIndex.push_back(ind);
}


int CLevFrontier::GetNumOfSubRegionIndex(void)
{
    return vSubregionsIndex.size();
}


CLevFrontier & CLevFrontier::operator = (const CLevFrontier & ro_frontier)
{
    if (this != &ro_frontier) {
        status                = ro_frontier.status;
        pt_minxminy            = ro_frontier.pt_minxminy;
        pt_maxxmaxy            = ro_frontier.pt_maxxmaxy;
        vpEllipses            = ro_frontier.vpEllipses;
        vpLines                = ro_frontier.vpLines;
        vpPoints            = ro_frontier.vpPoints;
        vpPrimitives        = ro_frontier.vpPrimitives;
        geoEntityType        = ro_frontier.geoEntityType;
        numPredisSegments    = ro_frontier.numPredisSegments;
        vpPointOnePrimitive = ro_frontier.vpPointOnePrimitive;
        vpRelatedFrontiers    = ro_frontier.vpRelatedFrontiers;
        vSubregionsIndex    = ro_frontier.vSubregionsIndex;
    }

    return *this;
}


bool CLevFrontier::operator == ( CLevFrontier &a)
{
    unsigned int i;
    int        ret = -1;
    bool    retorno = true;

    if(NumValidLines() == a.NumValidLines() && NumValidCurves() == a.NumValidCurves()) {
        for(i=0;i<vpPrimitives.size() && retorno == true;i++) {
            if(vpPrimitives[i]->Status()) {
                ret = a.IsAPrimitive(vpPrimitives[i]);
                if(ret==-1)
                    retorno = false;
            }
        }
    } else
        retorno = false;

    return retorno;
}


void CLevFrontier::Reinitialize()
{
    CLevGroup::Reinitialize();

    vpPointOnePrimitive.clear();
    vpRelatedFrontiers.clear();
    vSubregionsIndex.clear();

    numPredisSegments = 0;
}


void CLevFrontier::ReinitializeAuxiliarData()
{

    vpPointOnePrimitive.clear();
    vpRelatedFrontiers.clear();
    vSubregionsIndex.clear();
}


CLevPoint *    CLevFrontier::GetPointOnePrimitive(int index)
{
    // quando tudo estï¿½ certo, devem existir dois componentes neste vetor,
    // que sï¿½o os pontos extremos da fronteira
    CLevPoint * po_aux = NULL;
    if(index >=0 && index < (signed)vpPointOnePrimitive.size())
        po_aux = vpPointOnePrimitive[index];
    return po_aux;
}


//-----------------------------------------------------------------------------------------------------
//    BuildPrediscretizationVector
//-----------------------------------------------------------------------------------------------------
//
//    Descriï¿½ï¿½o:  Preenche o vetor vpPoints, com os pontos de pre-discretizaï¿½ï¿½o da fronteira, a partir
//                dos pontos de prï¿½-discretizaï¿½ï¿½o de cada primitiva, incluindo os pontos vï¿½rtices
//                com a ordenaï¿½ao de pontos em um determinado sentido
//                considerando todas as primitivas da fronteira
//
//    parï¿½metros:    nenhum
//
//    retorno:
//
//    Observaï¿½ï¿½es:
//......................................................................................................
//    Versï¿½o:            1.0
//    Autor:            Nancy Mieko Abe
//    Data:            Maio/2004
//    Modificado por:
//    Modificado em:
//    Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevFrontier::BuildPrediscretizationVector(void)
{
    unsigned int i, j;
    int            indOther;
    int            retorno = 0;
    unsigned int num_points, num_predis_points;
    double        dtA, dtB;
    vCLevPoint    vpredispoints;
	bool		isInserted = false;
	int			code;

	CLevPoint * poPoint = NULL;
    CLevPoint    o_p;
    CLevPoint    oPointAux, oPointPredisAux, onePrimitivePoint0,onePrimitivePoint1;

    vPointsFrontier.clear();

    // inclusï¿½o dos pontos ordenados
    //    partindo-se do ponto vpPointOnePrimitive[0] => para o ponto vpPointOnePrimitive[1] da fronteira
    //

    // aqui segue-se a sequencia do vetor vpPrimitives, sem se preocupar com a sequencia das primitivas
    // neste ponto, as primitivas jï¿½ devem estar na sequencia que se deseja

    // verifica se pertence  a vpPrimitives[0]
    //    verificando se ï¿½ p0 ou p1 (tem que pertencer pois foi construido assim, se nï¿½o pertence => erro)
    // comeï¿½a a inserï¿½ï¿½o pelo vpPointOnePrimitive[0]

    // obtem pontos de pre-discretizaï¿½ï¿½o da primitiva vpPrimitives[0]

    // verifica se os pontos estï¿½o na ordem correta
    //        o primeiro ponto a ser inserido (do vetor vpredispoints) tem que estar mais perto
    //        do ponto vpPointOnePrimitive[0]
    //        do que do outro extremo da primitiva (acho que em alguns casos os pontos de prï¿½-discretizaï¿½ï¿½o
    //      sï¿½o armazenados em ordem inversa da sequencia dos pontos p0-p1)

    // repete os passos atï¿½ varrer todas as primitivas da fronteira

    if(!vpPointOnePrimitive.size())
        ConnectionPointsSortPrimitives();

    onePrimitivePoint0 = *vpPointOnePrimitive[0];
    onePrimitivePoint1 = *vpPointOnePrimitive[1];
    //inclui primeiro ponto no vetor
    vPointsFrontier.push_back(*vpPointOnePrimitive[0]); //ï¿½ um dos vertices da primitiva 0

    for(i=0;i<vpPrimitives.size() && !retorno;i++) {

        vpredispoints.clear();

        vpPrimitives[i]->PreDiscretize(vpredispoints);

        num_predis_points = vpPrimitives[i]->oPredis.NPoints()-2; //nï¿½o inclui os dois vertices

        num_points = vPointsFrontier.size();

        //verifica qual ï¿½ o indice, na primitiva, do ultimo ponto inserido
        if(vPointsFrontier[num_points-1] == *vpPrimitives[i]->vpPoints[0])
            indOther =1;
        else if(vPointsFrontier[num_points-1] == *vpPrimitives[i]->vpPoints[1])
            indOther =0;
        else
            retorno = 1; // se o ultimo ponto inserido nï¿½o for vertice da primitiva => erro

        if(!retorno) {
            //verifica se o primeiro ponto do vetor vpredispoints estï¿½ mais proximo
            //   do vertice p0 ou p1
            oPointAux = *vpPrimitives[i]->vpPoints[indOther]; // ï¿½ o outro vertice da primitiva
            oPointPredisAux = vpredispoints[0];
            //calcula distancia entre ultimo ponto inserido (primeiro vertice da primitiva que foi incluido)
            //e o primeiro ponto do vetor de pontos de prï¿½-discretizaï¿½ï¿½o
            dtA = vPointsFrontier[num_points-1].CalcDistance(oPointPredisAux);
            //calcula distancia entre o outro vertice da primitiva e primeiro ponto do vetor de pontos de prï¿½-discretizaï¿½ï¿½o
            dtB = oPointAux.CalcDistance(oPointPredisAux);

            if(dtA < dtB) {
                for(j=0;j<num_predis_points;j++) {
                    o_p = vpredispoints[j];
                    vPointsFrontier.push_back(o_p);
                }
            } else {
                for(j=num_predis_points-1;j>=0;j--) {
                    o_p = vpredispoints[j];
                    vPointsFrontier.push_back(o_p);
                }
            }

            //insere o outro vertice da primitiva
            vPointsFrontier.push_back(oPointAux);
        }
    }
}


bool CLevFrontier::isInPredisVector( CLevPoint *p)
{
    unsigned int i;
    bool isIncluded = false;

    // como sï¿½ os pontos extremos sï¿½o verificados, poderia
    // comparado se os ponteiros sï¿½o iguais. Mas para evitar problemas
    // preferi fazer a comparaï¿½ï¿½o por coordenadas.
    for(i=0;i<vPointsFrontier.size() && isIncluded == false ;i++)
        if(vPointsFrontier[i].X() == p->X() && vPointsFrontier[i].Y() == p->Y())
            isIncluded = true;

    return isIncluded;
}
