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

// Especializa��o para fronteiras
int CLevFrontier::IsAPoint(CLevPoint  *p)
{
    // valor de retorno: 0 se ponto *p n�o pertence � nenhuma primitiva
    //                   1 se o ponto *p � um dos pontos extremos
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
//    Descri��o: Verifica a quantas primitivas pertence um dado ponto
//
//    par�metros:    Um ponteiro para um objeto CLevPoint
//
//    retorno:    n�mero de primitivas as quais o ponto pertence
//
//    Observa��es:
//......................................................................................................
//    Vers�o:            1.0
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
//    Descri��o: Ordena as primitivas em seguencia de liga��o de pontos
//
//    par�metros:    nenhum
//
//    retorno:    0 : ordena��o realizada com sucesso
//                1 : existem primitivas desconectadas
//                2 : a fronteira forma um pol�gono
//                3 : inconsistencia de dados
//
//    Observa��es:
//        Verifica inicialmente se todas as primitivas est�o conectadas.
//        Somente os dois pontos extremos da fronteira devem pertencer a somente uma primitiva,
//        No caso de existirem mais de dois pontos pertencentes a somente um primitiva, significa
//        que existem primitivas desconectadas.
//        No caso de n�o existir nenhum ponto pertencentes a somente um primitiva, significa que
//        a fronteira forma um pol�gono.
//......................................................................................................
//    Vers�o:            1.0
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
    vInt        vPrimIndex;        // vetor auxiliar usado para armazernar o �ndice, do vetor vpPrimitives, da primitiva que tem ponto extremo
	CLevPoint	*poPointAux;	// utilizado, quando necess�rio, para a troca da ordem dos pontos em linhas
    bool        isIncluded;        // flag que indica de um ponto j� foi inclu�do na lista de pontos pertencentes a somente uma primitiva
    short int num_connections;    // n�mero de conex�es de um ponto (ou seja, a quantas primitivas ele pertence)
    vpCLevPrimitive vpPrimAux;    // vetor auxiliar usado para armazernar os ponteiros para primitivas ordenadas
    vInt        vPrimIncluded;    // vetor auxiliar usado como flag para indicar se uma primitiva j�
                                // foi inclu�da na lista ordenada auxiliar
    int            ind;
	int			indEllipse		= -1; // posi��o da elipse no vetor vpPrimitives
	bool		thereIsAnEllipse = false; //flag para indicar se existe uma elipse na fronteira
	bool		isSequenceInverted; // flag que indica se a sequencia inicial armazenada em vpPrimAux
									// est� invertida, ou seja, existe pelo menos uma elipse o armazenamento
									// n�o est� seguindo a ordem p0 atual igual a p1 da primitiva anterior
									// Como n�o � poss�vel alterar p0 para p1 em elipses, ordena-se as primitivas
									// no sentido inverso (ou seja, comen�ando-se pelo outro ponto extremo)
	bool		jaInverteu;			// flag que indica que a sequencia j� foi invertida, ou seja, iniciou-se com o ponto
									// vpPointOnePrimitive[1]. A primeira tentativa � com vpPointOnePrimitive[0]
    CLevPoint    o_point_aux;
	int			aux=0;

    vpPointOnePrimitive.clear();

    // Identifica os pontos que pertencem a somente uma primitiva e o �ndice destas primitivas
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

    if(vpPointOnePrimitive.size() == 2 && !retorno) //n�o existem primitivas desconectadas
    {
        vpPrimAux.clear();

        //inicializa com flag 0 (n�o inclu�do) o vetor vPrimIncluded
        for(i=0;i<vpPrimitives.size();i++)
            vPrimIncluded.push_back(0);

        //recupera o indice da primitiva que cont�m o OnePrimitive ponto 0
        ind = vPrimIndex[0];
        //inclui a primitiva na lista
        vpPrimAux.push_back(vpPrimitives[ind]);
        //atualiza o flag de inclusao
        vPrimIncluded[ind] =1;

        // varre as outras primitivas procurando aquela que tem um ponto de liga��o com a
        // primitiva anterior.
        // a inclus�o deve parar quando o n�mero de primitivas inclu�das no vetor for igual
        // ao n�mero de primitivas da fronteira.

        num_prim = vpPrimitives.size();
        num_prim_included = 1;// a primitiva do OnePrimitive ponto 0 j� foi incluida

        do {
            for(i=0;i<num_prim;i++) {
                if(!vPrimIncluded[i]) { // a primitiva n�o foi incluida
                    //verifica se um dos pontos da primitiva � igual ao ponto da primitiva anterior
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
    // atualiza quando da pr�-discretiza��o de uma das primitivas
}


int    CLevFrontier::GetNumOfPredisSegments(void)
{
    unsigned int i;
    int num_predis_points;
    numPredisSegments = 0;

    for(i=0;i<vpPrimitives.size();i++) {
        // n�mero de segmentos = n�mero de pontos - 1
        num_predis_points = vpPrimitives[i]->oPredis.NPoints();

        // ainda n�o entrou na pre-discretiza��o
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
    // quando tudo est� certo, devem existir dois componentes neste vetor,
    // que s�o os pontos extremos da fronteira
    CLevPoint * po_aux = NULL;
    if(index >=0 && index < (signed)vpPointOnePrimitive.size())
        po_aux = vpPointOnePrimitive[index];
    return po_aux;
}


//-----------------------------------------------------------------------------------------------------
//    BuildPrediscretizationVector
//-----------------------------------------------------------------------------------------------------
//
//    Descri��o:  Preenche o vetor vpPoints, com os pontos de pre-discretiza��o da fronteira, a partir
//                dos pontos de pr�-discretiza��o de cada primitiva, incluindo os pontos v�rtices
//                com a ordena�ao de pontos em um determinado sentido
//                considerando todas as primitivas da fronteira
//
//    par�metros:    nenhum
//
//    retorno:
//
//    Observa��es:
//......................................................................................................
//    Vers�o:            1.0
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

    // inclus�o dos pontos ordenados
    //    partindo-se do ponto vpPointOnePrimitive[0] => para o ponto vpPointOnePrimitive[1] da fronteira
    //

    // aqui segue-se a sequencia do vetor vpPrimitives, sem se preocupar com a sequencia das primitivas
    // neste ponto, as primitivas j� devem estar na sequencia que se deseja

    // verifica se pertence  a vpPrimitives[0]
    //    verificando se � p0 ou p1 (tem que pertencer pois foi construido assim, se n�o pertence => erro)
    // come�a a inser��o pelo vpPointOnePrimitive[0]

    // obtem pontos de pre-discretiza��o da primitiva vpPrimitives[0]

    // verifica se os pontos est�o na ordem correta
    //        o primeiro ponto a ser inserido (do vetor vpredispoints) tem que estar mais perto
    //        do ponto vpPointOnePrimitive[0]
    //        do que do outro extremo da primitiva (acho que em alguns casos os pontos de pr�-discretiza��o
    //      s�o armazenados em ordem inversa da sequencia dos pontos p0-p1)

    // repete os passos at� varrer todas as primitivas da fronteira

    if(!vpPointOnePrimitive.size())
        ConnectionPointsSortPrimitives();

    onePrimitivePoint0 = *vpPointOnePrimitive[0];
    onePrimitivePoint1 = *vpPointOnePrimitive[1];
    //inclui primeiro ponto no vetor
    vPointsFrontier.push_back(*vpPointOnePrimitive[0]); //� um dos vertices da primitiva 0

    for(i=0;i<vpPrimitives.size() && !retorno;i++) {

        vpredispoints.clear();

        vpPrimitives[i]->PreDiscretize(vpredispoints);

        num_predis_points = vpPrimitives[i]->oPredis.NPoints()-2; //n�o inclui os dois vertices

        num_points = vPointsFrontier.size();

        //verifica qual � o indice, na primitiva, do ultimo ponto inserido
        if(vPointsFrontier[num_points-1] == *vpPrimitives[i]->vpPoints[0])
            indOther =1;
        else if(vPointsFrontier[num_points-1] == *vpPrimitives[i]->vpPoints[1])
            indOther =0;
        else
            retorno = 1; // se o ultimo ponto inserido n�o for vertice da primitiva => erro

        if(!retorno) {
            //verifica se o primeiro ponto do vetor vpredispoints est� mais proximo
            //   do vertice p0 ou p1
            oPointAux = *vpPrimitives[i]->vpPoints[indOther]; // � o outro vertice da primitiva
            oPointPredisAux = vpredispoints[0];
            //calcula distancia entre ultimo ponto inserido (primeiro vertice da primitiva que foi incluido)
            //e o primeiro ponto do vetor de pontos de pr�-discretiza��o
            dtA = vPointsFrontier[num_points-1].CalcDistance(oPointPredisAux);
            //calcula distancia entre o outro vertice da primitiva e primeiro ponto do vetor de pontos de pr�-discretiza��o
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

    // como s� os pontos extremos s�o verificados, poderia
    // comparado se os ponteiros s�o iguais. Mas para evitar problemas
    // preferi fazer a compara��o por coordenadas.
    for(i=0;i<vPointsFrontier.size() && isIncluded == false ;i++)
        if(vPointsFrontier[i].X() == p->X() && vPointsFrontier[i].Y() == p->Y())
            isIncluded = true;

    return isIncluded;
}
