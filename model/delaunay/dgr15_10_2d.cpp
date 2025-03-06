//****************************************************************************


//


//============================================================================

// S U B S I S T E M A    :
//
//              G E R A D O R    A U T O M A T I C O    DE    M A L H A
//
//                    D E    E L E M E N T O S    F I N I T O S




//============================================================================


// modulo    :                          dgr15_10_2D.cpp


// versao de :                          21/12/1999

// programador :                        Angelo Passaro




// Modificado por: Nancy Mieko Abe
// Data:    Marco/2002
// Motivo:  Inclusao do gerador de malha no projeto LEV077897
// Alteracoes:
//        Delaunay::nuvem_pontos
//      Delaunay::bloco_pts 
//  - passaram a ser objetos vectors da STL (antes eram ponteiros para listas Lo_Pt_Malha)
//    - passaram a ser objetos internos ao gerador de malha
//  - foi criada uma uma funcao para retornar os pontos gerados Delaunay2D::GetMeshPoints(),
//        a qual atualmente esta retonando o endereco de bloco_pts.
//        Verificar se isto pode causar algum problema, pois bloco_pts s� existe enquanto
//         o objeto Delaunay2D existir;
//

// Modificado por: Nancy Mieko Abe
// Data:    Abril/2002
// Motivo:  Inclusao do gerador de malha no projeto LEV077897 e no EspfisVC
// Alteracoes:
//        Delaunay2D::bloco_geometria
//  - passa a ser vector da STL (antes era lista Lo_Aresta_Geometria)
//  - foi criada uma uma funcao para retornar as arestas de geometria ap�s
//        a geracao de malha Delaunay2D::GetGeometryEdges(),
//        a qual atualmente esta retonando o endereco de bloco_geometria.
//        Verificar se isto pode causar algum problema, pois bloco_geometria s� existe enquanto
//         o objeto Delaunay2D existir;
//
// Modificado por: Nancy Mieko Abe
// Data: 15/05/2002
// Motivo: 
//    A lista de elementos 2D (Delaunay2D::bloco_elem_2D) passara a ser
//    uma lista interna ao gerador de malha (antes era fornecido um ponteiro
//    para um objeto criado externamente).
//
// Alteracoes:
//    Para evitar grandes alteracoes no codigo, em relacao a sintaxe ( troca de -> por .)
//    decidiu-se por fazer uma alocacao dinamica da lista no construtor de Delaunay2D
//    e sua desalocacao no destructor.


// ***************************************************************************


#include <algorithm> 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <memory.h>
#include <math.h>
#include "dgr15_10_2d.hpp"
 
//#define CHECA_RESTAURACAO // impressao da restauracao de geometria

    #define MAXIMOFACES     5000
    #define MAXELEMELIM     5000
    #define MAXVIZINHOSDIST 5000
    #define MAXDESLOCAMENTOS 50000


//void Erro(Delaunay2D *,Lo_Pt_Malha *lp,Lista_Elementos_2D_D *le,long);
void Erro(Delaunay2D *,vCLevPtMalha &lp,Lista_Elementos_2D_D *le,long);

// Physical_Display Vid1;
// Viewport jNumero   (&Vid1);

//#if defined INTERFACE_GRAFICA
//    Mensagem Msg;
//#endif
//double SIGMA = -1e-7;    // esta variavel e usada por outros objetos
//double SIGMA =  0;
//const float deslcontrol = 1.5e-7;    //4e-6;

double deslcontrol;


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// variaveis globais no ambito deste fonte para armazenamento das
// coordenadas originais devido a perturbacoes originadas por ocorrencia
// de casos degenerados e volume igual a zero
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

long   pontodalista = 10;


lev_math::USINT              controle_degenerados = 0;
long               deslocamentos[MAXDESLOCAMENTOS]      ;
double              coordx[MAXDESLOCAMENTOS]             ;
double              coordy[MAXDESLOCAMENTOS]             ;
double              coordz[MAXDESLOCAMENTOS]             ;
long  Vizinhos_Distantes[MAXVIZINHOSDIST];// vetor contendo o indice dos
                                                             // elementos vizinhos distantes
                                                             // (identificados em localiza_vizinhos)
long  ind_elem_elim     [MAXELEMELIM];    // vetor contendo o indice dos
                                                             // elementos a serem eliminados
                                                             // (identificados em localiza_vizinhos)
lev_math::USINT num_vizinhos_distantes, num_elem_elim;

FACE  *faces             ; // faces do poliedro de insercao dimensionado no construtor de Delaunay2D


//===========================================================================
//               class Delaunay2D :: Dimensiona_Englobante
//===========================================================================
//
//   descricao : procura  na lista de pontos pelo maior e menor valor
//               de x e y, e a partir destes define o paralelepipedo
//               englobante. E utilizado um fator_englobante para
//               dimensionar as dimensoes maximas  deste paralelepipedo.
//
//   data da �ltima modifica��o: 17/03/2001
//
//     modificada por: Guilherme Moreira Magalh�es
//
//===========================================================================

void Delaunay2D :: Dimensiona_Englobante(void)
{
     long   i;
     Pt_Malha  pt;
     double xmax , ymax, xmin, ymin;
     xmax = ymax = 0;
     xmin = ymin = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  DESLOCAMENTO DE OITO COMPONENTES DEVIDO AO POSICIONAMENTO NO INICIO DA
//  LISTA DOS quatro PONTOS VERTICES DO RETANGULO ENGLOBANTE
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//     for (i = 0 ; i < nuvem_pontos->Get_Total_Componentes(); i++) {
     for (i = 0 ; i < nuvem_pontos.size(); i++) {
         pt = nuvem_pontos[i];
//         pt = nuvem_pontos->ptr_pontos[i];

         if (xmax < pt.X()) xmax = pt.X();
         if (ymax < pt.Y()) ymax = pt.Y();

         if (xmin > pt.X()) xmin = pt.X();
         if (ymin > pt.Y()) ymin = pt.Y();


    }

    double difx, dify;
    difx = xmax - xmin;
    dify = ymax - ymin;
    difx *=.3;
    dify *=.3;
    xmin -= difx; ymin -= dify;
    xmax += difx; ymax += dify;


//    bloco_pts->Inclui(xmin,ymin,0l);
//    bloco_pts->Inclui(xmax,ymin,1l);
//    bloco_pts->Inclui(xmax,ymax,2l);
//    bloco_pts->Inclui(xmin,ymax,3l);

/////////////////////////////////////// STL /////////////////////////////////////////
    Pt_Malha p1;

    // tipos atribu�dos por Inclui
    p1.Set_Tipo(0);
    p1.Set_TipoCC(0);

    p1.X(xmin); p1.Y(ymin);
//    bloco_pts->Inclui(xmin,ymin,0l);
    bloco_pts.push_back(p1);

    p1.X(xmax); p1.Y(ymin);
//    bloco_pts->Inclui(xmax,ymin,1l);
    bloco_pts.push_back(p1);

    p1.X(xmax); p1.Y(ymax);
//    bloco_pts->Inclui(xmax,ymax,2l);
    bloco_pts.push_back(p1);

    p1.X(xmin); p1.Y(ymax);
//    bloco_pts->Inclui(xmin,ymax,3l);
    bloco_pts.push_back(p1);

//////////////////////////////////////////////////////////////////////////////////////
}





//===========================================================================
//               class Delaunay :: Delaunay
//===========================================================================
//
Delaunay :: Delaunay (void) {
     Delaunay_Ok      = lev_enum::YES ;
     simetria         = PLANE;
     limite_superior_pontos = 0;
     fator_englobante = 1.3 ;

     // *RYT* 2015-09-03
     // It was global before
     SIGMA = 0.0;
     //SIGMA = -1e-7;

//     bloco_pts        = NULL;
//     IndMov           = NULL;
}

Delaunay :: ~Delaunay (void) {
     Delaunay_Ok      = lev_enum::NO;
//     if (bloco_pts) delete bloco_pts;
}

//lev_math::USINT    Delaunay :: Altera_Malha(long, long) {}

//void     Delaunay ::  Associa(Indicador_Movel *i) { IndMov = i;}

//void     Delaunay ::  Associa(Lo_Pt_Malha *obj) { bloco_pts = obj;}
void     Delaunay ::  Associa(vCLevPtMalha &obj) { bloco_pts = obj;}

void     Delaunay ::  Dimensiona_Englobante  (void)     {}  //virtual
void     Delaunay ::  Elimina_Englobante     (void)     {}  //virtual
void     Delaunay ::  Gera_Elementos_Partida (void)     {}  //virtual
//long     Delaunay ::  Discretiza             (void)     {}  //virtual
//lev_math::USINT    Delaunay ::  Refina                 (void)     {}  //virtual

void Delaunay :: Set_Simetria(SYMMETRYTYPE sim)  {
    if (sim == PLANE || sim == AXIAL || sim == ASSIMETRIC  || sim == QUADRANTE) simetria = sim;
}

long  Delaunay :: Get_Limite_Pontos(void) { return limite_superior_pontos;}

//===========================================================================
//                  Delaunay :: SET_LIMITE_PONTOS
// ==========================================================================
// Descricao    :  Define para objeto  o numero maximo de pontos que deve ser
//                 utilizado para a geracao da malha de elementos finitos.
//
// Parametros   :  limite     : limite de pontos;
//
// retorno      :  limite     : eco do valor sugerido para indicar que o limite
//                          foi aceito e alterado
//                             : caso a sugestao fornecida seja menor que
//                           o numero minimo de pontos permitidos o limite minimo e'
//                           definido  e devolvido
//                             : caso a sugestao fornecida seja maior que
//                           o numero maximo de pontos permitidos  o valor maximo e
//                           assumido e devolvido
//                 0               : se a lista de pontos nao tiver sido definida atraves
//                                   de Associa.
//
// programador  : Angelo Passaro
// alterado em  : 14/1/97
//===========================================================================
long  Delaunay :: Set_Limite_Pontos(long n)
{

    // *NMA* : qual � o limite de pontos para STL???
#if 0
    if (bloco_pts)
    {
        if (n > bloco_pts->Get_N_Max_Com_Abs()) limite_superior_pontos = bloco_pts->Get_N_Max_Com_Abs();
        else
        {
            if (n <= bloco_pts->Get_Total_Componentes()) limite_superior_pontos = bloco_pts->Get_Total_Componentes();
            else limite_superior_pontos = n;
        }
    }
    else limite_superior_pontos = 0;
    return limite_superior_pontos;
#endif
    return 100000;
}



//===========================================================================






//===========================================================================
//               class Delaunay2D  
//===========================================================================
//
Delaunay2D :: Delaunay2D (void) : Delaunay() 
{
     Delaunay2D_Ok    = lev_enum::YES ;
     bloco_elem_2D    = NULL;

    // Aloca��o de bloco_elem_2D que passa a ser lista interna ao gerador de malha
     // ver coment�rios no in�cio do arquivo
    bloco_elem_2D = new Lista_Elementos_2D_D;
    if (!bloco_elem_2D){std::cout << "\n incapaz de gerar lista de elementos (Delaunay2D) . Execucao interrompida ..."; exit(0);}

//===================STL ==================
//     bloco_geometria  = NULL;
//=========================================
     faces            = NULL;
     n_vertices = 3;
     faces = new FACE[MAXIMOFACES];
     if (!faces){std::cout << "\n incapaz de gerar vetor de faces . Execucao interrompida ..."; exit(0);}


     geoData = NULL;
}

Delaunay2D :: ~Delaunay2D()   
{
     Delaunay2D_Ok      = lev_enum::NO;

    // desaloca��o de bloco_elem_2D que passa a ser lista interna ao gerador de malha
     // ver coment�rios no in�cio do arquivo
     if (bloco_elem_2D) delete bloco_elem_2D;

}

//===========================================================================
//               class Delaunay2D :: Acessa_Capacidade
//===========================================================================
//
//  Descricao          : Permite o acesso ao dados sobre  a capacidade maxima
//                       da malha de elementos finitos.
//
//
//  parametros         : maior bloco     : maior bloco de memoria extendida
//                                         disponivel atualmente (em Kb)
//                       total_memoria   : memoria extendida total identificada (em Kb)
//
//                       num_max_pontos  : numero maximo de pontos da malha
//
//                       num_max_elementos:numero maximo de elementos da malha
//
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 12/08/94
//
//===========================================================================
void Delaunay2D :: Acessa_Capacidade (unsigned &maior_bloco,unsigned &total_memoria,long &num_max_pontos, long &num_max_elementos)
{

    // *NMA* : isto ainda vale ???

#if 0
 if (bloco_pts && bloco_elem_2D)
 {
    num_max_pontos    = bloco_pts->Get_N_Max_Com_Abs();
    num_max_elementos = bloco_elem_2D->Get_N_Max_Com_Abs();
//#if defined  __DPMI32__
//    lev_math::Uchar status = 0;
//    maior_bloco = total_memoria = coreleft()/1024;
//#else
//    lev_math::Uchar status = bloco_pts->Get_Free_Mem(&maior_bloco, &total_memoria);
//#endif
//    if (status) {maior_bloco = total_memoria = 0;}
 }
#endif

} // fim do servico acessa_capacidade


//===========================================================================
//               class Delaunay2D :: Altera_Malha
//===========================================================================
//
//  Descricao          : Dado o ponto a ser inserido da nuvem de Delaunay
//                       altera a malha de elementos atuais utilizando o
//                       algoritmo de Watson com modificacoes inspiradas em
//                       Correc
//
//  observacao         : os valores de retorno indicam aa Discretiza se deve
//                       ou nao, haver uma perturbacao das coordenadas do
//                       ponto sendo inserido.
//
//  parametros         : indice_elemento : indice do elemento que "envolve"
//                                         o ponto
//                       indice_ponto    : indice do ponto sendo incluido.
//
//  retorno            : status de termino de execucao :  0 OK
//                                                       20 volume de um elemento igual a zero
//                                                       30 caso degenerado
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 28/03/2001
//  por Guilherme Moreira Magalh�es
//
//===========================================================================
lev_math::USINT Delaunay2D::Altera_Malha(long indice_elemento, long indice_ponto)
{
    lev_math::Uchar retorno2;                   // retorno do status de inclusao de elemento
    lev_math::USINT retorno                = 0; // valor de retorno de altera malha
    lev_math::USINT elementos_introduzidos = 0; // utilizado para acumular o numero de
                        // de elementos realmente introduzidos
                        // na malha de elementos finitos devido a inclusao do ponto
    lev_math::USINT i, n_faces_total;    // numero total de faces do poliedro de insercao
    long k, j, l;
    FACE faces[MAXIMOFACES]; // faces do poliedro de insercao
    Elemento_2D el;
    Pt_Malha ponto;
    int quebra = 0;

    ponto = bloco_pts[indice_ponto];
    if(retorno) {
        retorno =  105;
        Informa_Erro("Erro 105: Dados Inconsistentes. Execucao interrompida...",retorno);
    }

    n_faces_total = num_vizinhos_distantes = 0;
    num_elem_elim = 1;
    ind_elem_elim[0] = indice_elemento;

    retorno = bloco_elem_2D->Localiza_Vizinhos(ind_elem_elim, &num_elem_elim, &Vizinhos_Distantes[0], &num_vizinhos_distantes, &ponto, SIGMA);

    #if 10
    if (num_elem_elim > MAXELEMELIM)
    {
        Informa_Erro("ERRO : numero de elementos eliminados excedeu o maximo. O sistema provavelmente ficou instavel. Reinicialize-o!",0);
    }

    if (num_vizinhos_distantes  > MAXVIZINHOSDIST) {
        char lixo[200];
        sprintf(lixo,"%ld  ERRO : numero de vizinhos distantes excedeu o maximo. O sistema provavelmente ficou instavel. Reinicialize-o!",num_vizinhos_distantes);
        Informa_Erro(lixo,0);
    }
    #endif

    if(!retorno) {

        // ===========================================================================
        //       grava_elem_elim(ponto, ind_elem_elim, num_elem_elim);
        // ===========================================================================
        n_faces_total = Define_P_Insercao(faces, ind_elem_elim, num_elem_elim);

        if (n_faces_total >= MAXIMOFACES) {
            retorno = 116;
            Informa_Erro("Erro 116: Execucao interrompida...", retorno);
        }

        // ===========================================================================
        //        grava_faces(faces, n_faces_total);
        // ===========================================================================
        quebra = Verifica_Quebra_Geometria(faces, n_faces_total);

        l = bloco_elem_2D->Get_Total_Componentes()-1;//numero de elementos da malha
        // antes da construcao dos novos elementos

        j=0;
        quebra = 0;  // verificacao de quebra de geometria

        for (i = 0; i < n_faces_total && !quebra; i++) {

            if(faces[i].Valido == LEV_VALID) {
                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // este trecho garante a ordenacao da numeracao dos vertices
                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                if (indice_ponto <faces[i].V1)
                    retorno2=bloco_elem_2D->Inclui(bloco_pts,indice_ponto,faces[i].V1,faces[i].V2);
                else if (indice_ponto <faces[i].V2) {
                    retorno2=bloco_elem_2D->Inclui(bloco_pts,faces[i].V1,indice_ponto,faces[i].V2);
                } else {
                    retorno2=bloco_elem_2D->Inclui(bloco_pts,faces[i].V1,faces[i].V2,indice_ponto);
                }

                k=bloco_elem_2D->Get_Total_Componentes()-1;

                if(!retorno2) {

                    #if 0     // desligado para efeito de teste do comportamento do gerador de malha autom�tico
                    bloco_elem_2D->Get_Componente(k, el);

                    quebra = Mantem_Geometria(ind_elem_elim, num_elem_elim, indice_ponto);

                    if(quebra == 1 || quebra == -1)
                    {
                        for(j=elementos_introduzidos; j>=0; j--)
                        {
                            bloco_elem_2D->Elimina(k-j);
                        }
                        if (quebra == -1)
                            bloco_pts->Elimina_Ultimo();
                    } else {
                        long lixao = 0;
                        lixao = Recupera_Propriedades(ind_elem_elim, &num_elem_elim,el);
                        if (lixao)
                            lixao = k;

                        bloco_elem_2D->Modifica(k, el);
                        elementos_introduzidos++;  // para indicar aa atribui_vizinhanca
                                                    // o numero total de elementos realmente introduzidos
                    }
                    #else
                    /* so para teste do gerador autom�tico */
                    elementos_introduzidos++;   // para indicar aa atribui_vizinhanca
                                                // o numero total de elementos realmente introduzidos
                    #endif
                } else {
                    // elimina os ultimos elementos introduzidos e devolve status para
                    // o servico de chamada:
                    // discretiza:
                    //      desloca controladamente o ultimo ponto introduzido e depois
                    //      recomeca o processo
                    // refina :
                    //      ignora o ponto e continua o processo
                    for (j = bloco_elem_2D->Get_Total_Componentes(); j > bloco_elem_2D->Get_Total_Componentes() - elementos_introduzidos; j--)
                         bloco_elem_2D->Elimina(j-1);

                    retorno = retorno2;
                    break;  // quebro o laco i;
                } // fim do else de !retorno2
            } // fim do teste de validade de faces ======================
        } // fim do for i =====================================================

        if(!quebra) {
            if(!retorno2) {
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // mensagem para Lista_Elementos_2D_D para atribuir a vizinhanca aos ultimos
                // n_faces_total tetraedros
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                for (i=0; i<num_elem_elim; i++)
                    bloco_elem_2D->Elimina(ind_elem_elim[i]);

                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // mensagem para Lista_Elementos_2D_D para atribuir a vizinhanca aos ultimos
                // n_faces_total tetraedros
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                if ((retorno = bloco_elem_2D->Atribui_Vizinhanca(Vizinhos_Distantes, num_vizinhos_distantes, elementos_introduzidos))!=0)
                {
                    if (retorno == 306) {
                        DBG_Salva_Lista(indice_ponto,&Vizinhos_Distantes[0], num_vizinhos_distantes);
                        Informa_Erro("Erro 306 : Execucao Interrompida!", retorno);
                    }
                }
                // ============================================================
                // *AP*   alteracao em 26/07/94 para atualizar a lista, se necessario,
                // *AP*   antes de localizar vizinhos.
                // *AP*   Este atribuicao dada aa altera_malha eh devida aa sistematica de
                // *AP*   identifica'c~ao de vizinhos do novo ponto sendo incluido e da
                // *AP*   inclusao dos novos tetraedros. (Caderno2 pgs 38-39-40)
                bloco_elem_2D->Compacta_Lista();//----------------------------------------------
            } // fim do if
        } // fim do teste de quebra de geometria
        else if (quebra == 1) {
            retorno = 31;
        } else if (quebra == -1)
            retorno = 0;  // ponto ignorado
    }// fim do teste de retorno de Localiza_Vizinhos

    return retorno;
}



//void     Delaunay2D ::  Associa(Indicador_Movel *obj)      { Delaunay :: Associa(obj);}
void     Delaunay2D ::  Associa(Lista_Elementos_2D_D *obj) { bloco_elem_2D = obj;}

//*NMA*: para fornecer uma lista de pontos com estrutura de dados de lista_ordenada
void    Delaunay2D ::  Set_Nuvem_Pontos(lev_math::Lo_Templ<Point*,long> &rlo_points)
{
    long i;
    double x, y;
    Pt_Malha            o_ponto;
    int                    tipo_ponto;

    nuvem_pontos.clear();

    for(i=0;i<rlo_points.Get_Total_Componentes();i++)
    {

        x = rlo_points[i]->X();
        x = x*1e6;
        x = ceil(x);
        x = x*1e-6;

        y = rlo_points[i]->Y();
        y = y*1e6;
        y = ceil(y);
        y = y*1e-6;

        o_ponto.Set_Coord(x, y);

        tipo_ponto = rlo_points[i]->Origin();

        o_ponto.Set_Tipo((lev_math::Uchar) tipo_ponto);

        nuvem_pontos.push_back(o_ponto);
    }

}// end of Set_Nuvem_Pontos(lev_math::Lo_Templ<Point*,long> &rlo_points)

// *NMA*: esta opera��o seria usada para a gera��o de malha de primeira ordem a partir de
//        dos pontos e arestas gerada por higher_order para permitir a explora��o de resultados
/*
void    Delaunay2D ::  Set_Arestas_Geometria(lev_math::Lo_Templ<Aresta,long> &rlo_arestas)
{
    Aresta_Geometria    geom;
    long                    n1,n2,prim;
    long                    primType; // por enquanto n�o est� fornecendo

    int                    i;

    int j = 4; // se fosse malha autoadaptativa, come�aria em 0


    for(i=0;i<rlo_arestas.Get_Total_Componentes();i++)
    {
        rlo_arestas[i].Get_Vertices(n1,n2);

        rlo_arestas[i].Get_Prim(prim);

        geom.g1            = n1 + j;
        geom.g2            = n2 + j;
        geom.prim        = prim;

        bloco_geometria.push_back(geom);
    }


}
*/
//............. STL..................................................................
//void     Delaunay2D ::  Associa(Lo_Aresta_Geometria *obj) {bloco_geometria = obj;}
//void     Delaunay2D ::  Associa(vArestaGeometria &obj)
//{
//    bloco_geometria.reserve(MAXPTSTRN);
//    bloco_geometria = obj;
//}

//void     Delaunay2D ::  Associa(Lo_Pt_Malha *obj, Lo_Pt_Malha *obj1)
//void Delaunay2D ::  Associa(vCLevPtMalha &obj, vCLevPtMalha &obj1)
//{ 
//    bloco_pts.reserve(MAXPTSTRN);
//    nuvem_pontos.reserve(MAXPTSTRN);
//    bloco_pts        = obj;
//    nuvem_pontos    = obj1;
//}
//.......................................................................................

void Delaunay2D::Associa(CLevGeoData * geoPhys)
{
    geoData = geoPhys;
}

vCLevPtMalha & Delaunay2D ::  GetMeshPoints(void)
{
    return bloco_pts;
}

//.......................................................................................
vArestaGeometria & Delaunay2D ::  GetGeometryEdges(void)
{
    return bloco_geometria;
}

//.......................................................................................
void Delaunay2D ::  GetMeshElements(vElemento2D & vElem2D)
{
    Elemento_2D elemento;
    int            i;

    bloco_elem_2D->Compacta_Lista(1);

    for(i=0;i< bloco_elem_2D->Get_Total_Componentes();i++)
    {
        bloco_elem_2D->Get_Componente(i,elemento);

        vElem2D.push_back(elemento);
    }

}


// *NMA*: Como agora bloco_elem_2D n�o � mais fornecido por meio 
//            de Associa, foi necess�rio criar este m�todo para
//            inicializ�-la (15/05/2002)
void Delaunay2D::Set_N_Max_Mesh_Elements(long n_max)
{

    bloco_elem_2D->Set_N_Max_Com_Abs(n_max);

}


// *NMA*: Constroi a partir dos dados do modelo, os vetores nuvem pontos e bloco pontos
int Delaunay2D::BuildInitialData(int tipo_malha)
{
    Pt_Malha            o_ponto;
    CLevPoint*          po_lev_point;
	CLevLine*			po_lev_line;
	CLevEllipse*		po_lev_ellipse;
    Aresta_Geometria    geom;
    int                    tipo_ponto;
    int                    i;
    int                    n1,n2,prim;
    int                    primType;

    if(tipo_malha == FRONTEIRA)
        geoData->BuildEdgesFronteira();
    else
        geoData->BuildEdges();

    geoData->SetIdToValidPoints();

    nuvem_pontos.clear();

    // Constroi a nuvem de pontos
    for(i=0; i<geoData->GetNumPoints(); i++)
    {
        po_lev_point = geoData->GetPoint(i);

        if (po_lev_point->Status() && (po_lev_point->GeoEntityType() != POINTINVISIBLELEV))
        {
            //*NMA*:    O trecho abaixo arredonda o valores at� 1e-6
            //            Se os valores forem mantidos com mais casas decimais
            //            pode ocorrer erro na gera��o da malha.
            double x = po_lev_point->X();
            x = x*1e6;
            x = ceil(x);
            x = x*1e-6;
            double y = po_lev_point->Y();
            y = y*1e6;
            y = ceil(y);
            y = y*1e-6;

//            o_ponto.Set_Coord(po_lev_point->X(), po_lev_point->Y());
            o_ponto.Set_Coord(x, y);

            tipo_ponto = (enum GEOENTITYTYPE) po_lev_point->GeoEntityType();

            o_ponto.Set_Tipo((lev_math::Uchar) tipo_ponto);

            nuvem_pontos.push_back(o_ponto);

        }
    }

    int j;

    switch (tipo_malha)
    {
        case FRONTEIRA:
        case ARITHMETIC:
        case GEOMETRIC:
        case HARMONIC:
        case SQUARED_HARMONIC:    j =  4;break;  //*NMA*: Os indices come�am em 0
        case SELF_ADAPTIVE:        j =  0; break;
        default: ;
    }

    //Constroi bloco_geometria: A numera��o dos Id come�a em zero, assim como no gerador de malha
        for (i = 0 ;i < geoData->vEdges.size() ; i++)
        {
            if (geoData->vEdges[i].Status())
            {

                CLevPoint * pp = geoData->vEdges[i].vpPoints[0];

                n1            = geoData->vEdges[i].vpPoints[0]->Id();
                n2            = geoData->vEdges[i].vpPoints[1]->Id();
                prim        = geoData->vEdges[i].Id();
                primType    = geoData->vEdges[i].GeoEntityType();

                geom.g1            = n1 + j;
                geom.g2            = n2 + j;
                geom.prim        = prim;
                geom.primType    = primType;

                bloco_geometria.push_back(geom);


#if 0
                int posAresta = bloco_geometria.size()-1;
                if(primType == LINE)
                {
                    po_lev_line = geoData->GetLine(prim);
                    po_lev_line->vIndexArestaGeometria.push_back(posAresta);
                }
                else
                {
                    if(primType == ELLIPSE)
                    {
                        po_lev_ellipse = geoData->GetEllipse(prim);
                        po_lev_ellipse->vIndexArestaGeometria.push_back(posAresta);
                    }
                }
#endif

                nuvem_pontos[n1].Set_Tipo(0);
                nuvem_pontos[n2].Set_Tipo(0);

            }

        }

    //    geoData->BuildVetorArestasPrimitivas(bloco_geometria);


    return nuvem_pontos.size();

}

//===========================================================================
//               class Delaunay2D :: Define_P_Insercao
//===========================================================================
//
//  Descricao          : Dado um elemento cuja esfera circunscrita envolva
//                       o ponto a ser incluido constroi o poliedro de
//                       insercao, gerando suas faces. Os elementos que
//                       participam da geracao do poliedro sao marcados como
//                       INLEV_VALID pelo metodo Elimina
//
//  parametros         : faces : ponteiro para as faces construidas
//                       ind_elem_elim :  ponteiro para o vetor que contem o
//                                        indice dos elementos que envolvem o
//                                        ponto eliminado
//                       num_elem_elim :  numero dos elementos do vetor
//                                        ind_elem_elim
//
//  retorno            : numero de faces que compoem o poliedro envolvente
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 12/08/94
//
//===========================================================================


lev_math::USINT Delaunay2D :: Define_P_Insercao(FACE *faces, long  *ind_elem_elim, lev_math::USINT num_elem_elim)  {

        lev_math::USINT n_faces_total  ;   // visibilidade local
        lev_math::USINT n_faces_inicial;   // evita o teste entre faces de um mesmo tetraedro;
                                 // deveria fazer parte do objeto numa
                                 // estrutura mais completa.
        lev_math::USINT i,j,k, inclusao;

        lev_math::USINT status         ;

        FACE         face_interm;// face que sera utilizada para execucao dos testes
//    Pt_Malha       *ponto      ;// mantem os dados do ponto sendo incluido
        Elemento_2D  atual_el   ;// mantem os dados do elemento em teste
        lev_math::USINT n_faces = atual_el.Get_N_Vertices()    ;   // numero de faces de um tri�ngulo.


        status = bloco_elem_2D->Get_Componente(ind_elem_elim[0],atual_el);
        if (status) {
         status =  106;
//     std::cout << "\a\a\n\n\nErro "<< status <<": Dados Inconsistentes. Execucao interrompida...";
         Informa_Erro("Erro 106: Dados Inconsistentes. Execucao interrompida...",status);
        }

        atual_el.Monta_Faces(faces);

     n_faces_total = n_faces;


     // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     // O laco i comeca em 1 porque o indice zero foi utilizado para construir
     // as primeira n_faces faces
     // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

     for (i =1; i < num_elem_elim; i++)
     { //--------------------------------
         n_faces_inicial = n_faces_total; // evita busca nas outras faces do
                                                                            // mesmo tetraedro
         status = bloco_elem_2D->Get_Componente(ind_elem_elim[i],atual_el);
         if (status) {
            status =  106;
            Informa_Erro("Erro 106: Dados Inconsistentes. Execucao interrompida...",status);
         }
         for (j=0; j < n_faces; j++)
         {  //-----------------------------
             switch (j)
             { // particularizado para n_faces = 3 ------
                 case 0   :
                         face_interm.V1 = atual_el.Get_V1();
                         face_interm.V2 = atual_el.Get_V2();
                         break;
                 case 1   :
                         face_interm.V1 = atual_el.Get_V2();
                         face_interm.V2 = atual_el.Get_V3();
                         break;

                 case 2   :
                         face_interm.V1 = atual_el.Get_V1();
                         face_interm.V2 = atual_el.Get_V3();
                         break;

             } // fim do switch j ----------------------------------
//             inclusao = VERDADEIRO;
             inclusao = lev_enum::YES;

             for (k = 0; k < n_faces_inicial; k++)
             { //-------------
//                 if     ((face_interm.V1 == faces[k].V1)||(face_interm.V1 == faces[k].V2))
//                     if ((face_interm.V2 == faces[k].V2)||(face_interm.V2 == faces[k].V1))
                 if (face_interm == faces[k])
                     {
                        //inclusao = FALSO;
                        inclusao = lev_enum::NO;
                        break;
                     }
             } // fim do for k -------------------------------------

             if (inclusao) { //-------------------------------------
//                 faces[n_faces_total].V1 = face_interm.V1;
//                 faces[n_faces_total].V2 = face_interm.V2;
                 faces[n_faces_total] = face_interm;
                 faces[n_faces_total++].Valido = LEV_VALID;

             } //---------------------------------------------------

            // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 // faces comuns entre os elementos marcados para eliminacao indicam
            // que estas nao sao faces do poliedro de insercao
                 // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

             else faces[k].Valido = LEV_INVALID;
         } // fim do for j --------------------------------------------


     } // fim do for i -----------------------------------------------------

     return n_faces_total;

} // fim do metodo Define_P_Insercao ========================================


//===========================================================================
//               class Delaunay2D :: Discretiza
//===========================================================================
//
//  Descricao          : Divide o dominio de estudo em elementos finitos.
//                       Nesta implementacao utiliza-se o algoritmo de Watson
//                       com modificacoes inspiradas em Correc.
//                       A lista de pontos eh varrida e para cada ponto
//                       introduzido a malha de elementos finitos eh alterada.
//                       Os elementos nao LEV_VALIDs sao marcados. Para cada novo
//                       elementos introduzido, armazena os coeficientes da
//                       figura circunscrita e o volume dos tetraedros.
//
//  observacao         : nesta versao, se localizado um caso degenerado o
//                       ponto tem sua posicao,na direcao x, alterada com o
//                       intuito de quebrar a degenerescencia . Todos os indices
//                       de pontos deslocados sao armazenados no vetor
//                       deslocamentos  e o numero de deslocamentos eh
//                       armazenada em controle_degenerados.
//
//  valor de retorno   : numero de elementos v�lidos gerados
//
//  erros: 
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 13/02/2001 por Guilherme M Magalh�es
//
//===========================================================================

long Delaunay2D::Discretiza(void)
{
    if (bloco_elem_2D)
    {
        long           ii;  // contador do laco de inclusao de pontos
        long           num_pts, retorno; // numero total de pontos e valor de retorno de
                                                                         // altera malha
        Pt_Malha p; //objeto ponto para a c�pia da lista nuvem_pontos para a lista
                    // bloco_pts

        for (ii = 0, num_pts = nuvem_pontos.size(); ii<num_pts; ii++)
        {
            p = nuvem_pontos[ii];
            bloco_pts.push_back(p);

            retorno = Inclui_1_Ponto(bloco_pts.size() - 1);

            if (retorno == 108)
            {
                Informa_Erro("Erro 108: Dados Inconsistentes. Execucao interrompida...",retorno);
            }
            else if (retorno == 109)
            {
                Informa_Erro("Erro 109: Dados Inconsistentes. Execucao interrompida...",retorno);
            }
            else if (retorno == 120)
            {
                Informa_Erro("Erro 120 (Delaunay2D::Discretiza): Base de dados corrompida. Execucao interrompida...",retorno);
            }
            else if (retorno == 14)
            {
                Informa_Erro("Erro 14: Memoria extendida inexistente. Execucao interrompida...",retorno);
            }
            else if (retorno == 15)
            {
                Informa_Erro("Erro 15: Atingido o limite maximo de elementos do sistema. Tente aumentar a quantidade de memoria extendida disponivel!", retorno);
            }

        }
        nuvem_pontos.clear();
        return bloco_elem_2D->Get_N_Comp_Validos_Absoluto();
    } else
        return 0;    // algum objeto -pontos ou elementos3D- nao foi associado
}



//===========================================================================
//               class Delaunay2D :: Dimensiona_Capacidade
//===========================================================================
//
//   descricao : dimensiona o numero de pontos e elementos em funcao
//               da capacidade de memoria extendida disponivel.
//
//===========================================================================

void Delaunay2D :: Dimensiona_Capacidade(void){
    //long  max_pts, max_elem;
//  lev_math::USINT  nb_el,nb_pt,nb_pp; // prefixo nb == numero de bytes
    double fator_pt_el = 6.7;   // razao entre o numero de elementos e o de pontos
                                         // segundo a literatura = 5.3
//#if !defined __DPMI32__
//    lev_math::USINT status = bloco_pts->XMS_Driver_Detect();
//#else
//    lev_math::USINT status = 1;
//#endif
//    if (status )
//    {
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                             XMS driver detected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#if !defined __DPMI32__
//        unsigned maior_bloco = 0, mem_total = 0;
//        status  = bloco_pts->Get_Free_Mem(&maior_bloco, &mem_total);
//#else
//    unsigned long maior_bloco = 0, mem_total = 0;
//    maior_bloco = mem_total = coreleft()/1024;
//    status = 0;
//#endif
//      if (!status)
//      {
//         nb_pt    = sizeof(Pt_Malha); nb_el = sizeof(Elemento_2D);
//         nb_pp    = nb_pt + fator_pt_el*nb_el;
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Deixa folga para criacao de vetores auxiliares na lista de elementos 3D
// (OU SEJA, ISTO NAO ESTA CONVENIENTEMENTE ENCAPSULADO )
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//         max_pts  = ceil (maior_bloco * .85 * 1024/nb_pp);  // em bytes.
//         max_elem = ceil (fator_pt_el * max_pts);
//         bloco_pts    ->Set_N_Max_Com_Abs(max_pts);
//       limite_superior_pontos = max_pts;  // *AP* em 14/01/97 (ver Delaunay)
//         bloco_elem_2D->Set_N_Max_Com_Abs(max_elem);
//        }
//      else
//      {
//         status = 114;
//         Informa_Erro("ERRO 114: Memoria Extendida. Execucao Interrompida ...",status);
//        }
//    }

//    else
//    {
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   MEMORIA EXTENDIDA NAO DISPONIVEL! DEFINE O NUMERO MAXIMO DE PONTOS
//   EM FUNCAO DO NUMERO MAXIMO DE ELEMENTOS PERMITIDOS NO BLOCO DE 64k
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//        max_pts  = floor((bloco_elem_2D->Get_N_Max_Com_Abs()-1)/fator_pt_el);
//        bloco_pts->Set_N_Max_Com_Abs(max_pts);
//
//    }


}







void Delaunay2D :: Elimina_Achatados(void)
{

    FILE *arq1;   //  para gerar arquivo novo
    arq1 = fopen("remach.dat", "w");
    fclose(arq1);


    int                    num_elem_viz;
    int                    num_vertices_marcados ;
    int                  k, kk, kkk, kkkk;
    int                    continua_k, continua_kk, continua_kkk;
    int           retorno,status0;
    long          j;
    long          num_achatados;
    long          num_removidos;
    long          vertices_achatado [num_vertices]; // vertices do tetraedro achatado
    long          vertices_vizinho  [num_vertices]; // vertices de vizinho
    long          vertices_marcados [num_vertices]; // vertices nao comuns ao achatado
                                                                                                    // pode conter so o vertice comum
                                                                                                    // ou 3 ou 4 vertices, se nao houver comum
    long          elementos_marcados[num_vertices]; // elementos vizinhos
                                                                                                    // se existe um vertice comum, a dois vizinhos
                                                                                                    // o ultimo vetor nao eh igual ao de
                                                                                                    // elementos_vizinho, contendo somente dois componentes
    long        * elementos_vizinhos = NULL;
    double        rinsc, rcirc2;
    Elemento_2D el_atual;
    Elemento_2D el_viz;

    num_achatados = 0;
    num_removidos = 0;

    for (j = 0; j < bloco_elem_2D->Get_Total_Componentes(); j++)
    {
        status0 = bloco_elem_2D->Get_Componente(j, el_atual);
        if (status0)
        {
            Informa_Erro("Erro ?? : ",status0);
        }
        else
        {
            // _________________________________________________________________
         if (el_atual.Get_Validade())
         {
            rinsc  = el_atual.Calc_R_Insc(bloco_pts);
            rcirc2    = el_atual.Get_Raio_Circ(); //get raio_circ retorna o raio circunscrito

            if (rinsc/rcirc2 < 1.e-4)
            {
                num_achatados++;

//________________________________________________________________________

fprintf (arq1, "\n\n numero de achatados : %ld", num_achatados);


//________________________________________________________________________

                vertices_achatado[0] = el_atual.Get_V1();
                vertices_achatado[1] = el_atual.Get_V2();
                vertices_achatado[2] = el_atual.Get_V3();
                elementos_vizinhos   = el_atual.Get_Vizinhos();
                num_elem_viz         = el_atual.Get_N_Vizinhos();
//                num_vertices_marcados = Verifica_Tipo_Achatado(el_atual, &vertices_marcados[0], &elementos_marcados[0]);


                for (k = 0, continua_k = lev_enum::YES, num_vertices_marcados = 0; k <el_atual.Get_N_Vizinhos() && continua_k; k++)
//                k = 0; continua_k = YES; num_vertices_marcados = 0;l = 0;
//                while ( l <el_atual.Get_N_Vizinhos() && continua_k)
                {
//                    if (elementos_vizinhos[k]>=0)
//                    {    *GMM*  modifica��o no la�o para trabalhar com vizinhos ordenados
//                        l++;

                    status0 = bloco_elem_2D->Get_Componente(elementos_vizinhos[k],el_viz);
                    if (status0)
                    {
                        Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status0);
                    }
                    vertices_vizinho[0] = el_viz.Get_V1();
                    vertices_vizinho[1] = el_viz.Get_V2();
                    vertices_vizinho[2] = el_viz.Get_V3();
                    // vertices_vizinho[3] = el_viz.Get_V4();
                    // laco kk : varredura dos vertices dos elementos vizinhos ao achatado
                    for (kk = 0, continua_kk = lev_enum::YES; kk < num_vertices/*global*/ && continua_kk; kk++)
                    {

                    // laco kkk : varredura dos vertices do elemento achatado
                        for (kkk = 0, continua_kkk = lev_enum::YES; kkk < num_vertices /*global*/ && continua_kkk; kkk++)
                            if (vertices_achatado[kkk] == vertices_vizinho[kk])
                                continua_kkk = lev_enum::NO;

                        if (continua_kkk) // vertice do vizinho nao e comum ao achatado
                        {
                            if (num_vertices_marcados > 0)
                            {
                                for(kkkk = 0; kkkk < num_vertices_marcados; kkkk++)
                                {
                                    if (vertices_vizinho[kk] == vertices_marcados[kkkk])
                                    {
                                        vertices_marcados[0] = vertices_vizinho  [kk];
                                        elementos_marcados[1] = elementos_marcados[kkkk];
                                        elementos_marcados[0] = elementos_vizinhos[k];
                                        num_vertices_marcados = 1;
                                        continua_k = continua_kk = lev_enum::NO;
                                        break;     // quebra o laco kkkk
                                    } // fim do teste de vertice presente na lista de marcados
                                }   // fim do laco kkkk
                                if (continua_k) // significa que nao foi encontrado um vertice comum
                                {
                                    vertices_marcados[num_vertices_marcados]   = vertices_vizinho[kk];
                                    elementos_marcados[num_vertices_marcados++] = elementos_vizinhos[k];
                                    continua_kk = lev_enum::NO;
                                }
                            }
                            else
                            {
                                vertices_marcados [num_vertices_marcados]   = vertices_vizinho[kk];
                                elementos_marcados[num_vertices_marcados++] = elementos_vizinhos[k];
                                continua_kk = lev_enum::NO;
                            }     // fim do teste de numero de vertices marcados

                        }       // fim do teste de coincidencia de vertices com el. achatado
                    }         // fim do laco kk
                //    } //fim do if
                // k++;
                }           // fim do laco k








//____________________________________________________________________________
fprintf(arq1, "\n        numero de vertices marcados : %d", num_vertices_marcados);
//____________________________________________________________________________


                if (num_vertices_marcados == 1)
                {
//                    num_removidos+=Elimina_Achatado_TipoI(j, num_elem_viz, &elementos_vizinhos[0], &vertices_achatado[0], &vertices_marcados[0], &elementos_marcados[0]);
/* *AP* */                    arq1 = fopen("remach.dat", "a+");
                    fprintf(arq1,"\n          numero de elementos removidos acumulado : %ld", num_removidos);
                    fclose(arq1);

#if 0     // substituido pela funcao acima
                    // ===================================================== inicio [2]
                    // este bloco eh responsavel pela eliminacao do tetraedro achatado
                    // e reconstrucao do esquema de vizinhanca consistentemente
                    // =============================================================
                    // constroi arestas dos tetraedros vizinhos que nao contem o ponto marcado como vertice
                    Elemento_2D  el_vizA;
                    struct {long a1, a2;} arestas1[3], arestas2[3];
                    long vertices_vizinhoA[num_vertices];
                    long vertice_troca    [2];
                    long vertice_resto    [2];
                    int  ii,ij,ik, identificado = NO;

fprintf(arq1, "\n===================================================================");
fprintf(arq1,"\nnumero de elementos vizinhos = %d", num_elem_viz);
fprintf(arq1,"\nElementos vizinhos : %ld  %ld  %ld  %ld",elementos_vizinhos[0],elementos_vizinhos[1],elementos_vizinhos[2],elementos_vizinhos[3]);
                    bloco_elem_2D->Elimina(j);     // elimina o achatado da lista

                    vertice_troca[0]=vertice_troca[1]=vertice_resto[0]=vertice_resto[1]=0;

                    status0 = bloco_elem_2D->Get_Componente(elementos_marcados[0],el_viz);
                    status1 = bloco_elem_2D->Get_Componente(elementos_marcados[1],el_vizA);
                    if (status0 || status1)
                    {
                        Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status0);
                    }
                    vertices_vizinho[0] = el_viz.Get_V1()  ; vertices_vizinho[1] = el_viz.Get_V2();
                    vertices_vizinho[2] = el_viz.Get_V3()  ; vertices_vizinho[3] = el_viz.Get_V4();

                    vertices_vizinhoA[0] = el_vizA.Get_V1(); vertices_vizinhoA[1] = el_vizA.Get_V2();
                    vertices_vizinhoA[2] = el_vizA.Get_V3(); vertices_vizinhoA[3] = el_vizA.Get_V4();

                    // monta as arestas que nao contem o vertice comum dos tetraedros vizinhos
//                    for (ii = 0, ik = 0; ii < num_vertices-1/*global*/; ii++)
//                        for (ij = ii+1; ij <num_vertices-1/*global*/; ij++)
                    for (ii = 0, ik = 0; ii < num_vertices/*global*/; ii++)
                        for (ij = ii+1; ij <num_vertices/*global*/; ij++)
                            if   (vertices_vizinho[ii] != vertices_marcados[0])
                                if (vertices_vizinho[ij] != vertices_marcados[0])
                                {
                                    arestas1[ik]  .a1 = vertices_vizinho[ii];
                                    arestas1[ik++].a2 = vertices_vizinho[ij];
                                }

//                    for (ii = 0, ik = 0; ii < num_vertices-1/*global*/; ii++)
//                        for (ij = ii+1; ij <num_vertices-1/*global*/; ij++)
                    for (ii = 0, ik = 0; ii < num_vertices/*global*/; ii++)
                        for (ij = ii+1; ij <num_vertices/*global*/; ij++)
                            if   (vertices_vizinhoA[ii] != vertices_marcados[0])
                                if (vertices_vizinhoA[ij] != vertices_marcados[0])
                                {
                                    arestas2[ik]  .a1 = vertices_vizinhoA[ii];
                                    arestas2[ik++].a2 = vertices_vizinhoA[ij];
                                }

                    // verifica aresta comum e associa os vertices da aresta comum vom vertice_troca
                    // e os outros dois vertices do elemento achatado a vertice_resto
                    int continua_i, continua_j;
                    for   (ii = 0, continua_i = YES; ii < n_vertices && continua_i; ii++)
                        for (ij = 0, continua_j = YES; ij < n_vertices && continua_j; ij++)
                        {
                            if (arestas1[ii].a1 == arestas2[ij].a1 && arestas1[ii].a2 == arestas2[ij].a2)
                                identificado = YES;
                            else if (arestas1[ii].a1 == arestas2[ij].a2 && arestas1[ii].a2 == arestas2[ij].a1)
                                identificado = YES;
                            if (identificado)
                            {
                                vertice_troca[0] = arestas1[ii].a1;
                                vertice_troca[1] = arestas1[ii].a2;
                                for (ik = 0, kk = 0; ik < num_vertices; ik++)
                                {
                                    if (vertice_troca[0] != vertices_achatado[ik] && vertice_troca[1] != vertices_achatado[ik])
                                        vertice_resto[kk++] = vertices_achatado[ik];
                                }  // fim do laco for(ik)
                                continua_i = continua_j = NO;
                            }    // fim do teste(identificado)

                        }    // fim do laco for(ij)


// __________________________________________________________________________
        fprintf(arq1,"\n        vertice comum : %ld", vertices_marcados[0]);
        for (ii = 0; ii < n_vertices; ii++)
        {
            fprintf(arq1,"\n    prim.vizinho -> vertices da aresta %d :  %ld    %ld  " , ii, arestas1[ii].a1,arestas1[ii].a2);
            fprintf(arq1,"\n    sec..vizinho -> vertices da aresta %d :  %ld    %ld  " , ii, arestas2[ii].a1,arestas2[ii].a2);
        }
        fprintf(arq1, "\n vertices do tetraedro achatado %ld:",j);
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_achatado[0],
                         vertices_achatado[1], vertices_achatado[2], vertices_achatado[3]);

        fprintf(arq1, "\n vertices da aresta comum :    %ld    %ld",vertice_troca[0], vertice_troca[1]);
        fprintf(arq1, "\n vertices restantes       :    %ld    %ld",vertice_resto[0], vertice_resto[1]);
        fprintf(arq1, "\n vertices do tetraedro vizinho1 antes da modificacao %ld:",elementos_marcados[0]);
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho2 antes da modificacao %ld:",elementos_marcados[1]);
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
//        fclose(arq1);
// __________________________________________________________________________
                    // PROVIDENCIA A TROCA DE 1 VERTICE PARA O PRIMEIRO VIZINHO
                    for (ii = 0, continua_i = YES; ii < num_vertices && continua_i; ii++)
                    {
                        if (vertices_vizinho[ii] == vertice_resto[0])
                        {
                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
                            {
                                if (vertices_vizinho[ij] == vertice_troca[0])
                                { vertices_vizinho[ij] = vertice_resto[1]; continua_j = NO; }
                            }
                            continua_i = NO;
                        }
                        else if (vertices_vizinho[ii] == vertice_resto[1])
                        {
                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
                            {
                                if (vertices_vizinho[ij] == vertice_troca[0])
                                { vertices_vizinho[ij] = vertice_resto[0]; continua_j = NO; }
                            }
                            continua_i = NO;
                        }
                    }   // fim do laco for (ii)

                    // PROVIDENCIA A TROCA DE 1 VERTICE PARA O SEGUNDO VIZINHO
                    for (ii = 0, continua_i = YES; ii < num_vertices && continua_i; ii++)
                    {
                        if (vertices_vizinhoA[ii] == vertice_resto[0])
                        {
                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
                            {
                                if (vertices_vizinhoA[ij] == vertice_troca[1])
                                {
                                    vertices_vizinhoA[ij] = vertice_resto[1];
                                    continua_j = NO;
                                }
                            }
                            continua_i = NO;
                        }
                        else if (vertices_vizinhoA[ii] == vertice_resto[1])
                        {
                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
                            {
                                if (vertices_vizinhoA[ij] == vertice_troca[1])
                                {    vertices_vizinhoA[ij] = vertice_resto[0];    continua_j = NO; }
                            }
                            continua_i = NO;
                        }
                    }   // fim do laco for (ii)

                    // ===============================================================
                    // neste ponto, faz-se a atribuicao da vizinhanca
                    // ===============================================================

                    Elemento_2D elalter1, elalter2;  // vao armazenar momentaneamente
                                                                                         // as caracteristicas dos elementos
                                                                                         // que compartilham vertice comum
                    Elemento_2D el_vizB;
                    long vertices_aux[4];
                    int contador_vertices_iguais;
//                    std::cout << "\n -----------------------------------------------";
//                    std::cout << "\n elalter1 logo apos declaracao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos declaracao (vizinhos) "<< elalter2.Get_N_Vizinhos();

                    elalter1.Set_Vertices(&vertices_vizinho[0]);
                    elalter1.Calc_Circuns(bloco_pts);
fprintf(arq1,"\n           numero de vizinhos elalter1 (antes mutuo): %d",elalter1.Get_N_Vizinhos());
                    elalter1.Set_Vizinho(elementos_marcados[1]);

                    elalter2.Set_Vertices(&vertices_vizinhoA[0]);
                    elalter2.Calc_Circuns(bloco_pts);
fprintf(arq1,"\n           numero de vizinhos elalter2 (antes mutuo) : %d",elalter2.Get_N_Vizinhos());
                    elalter2.Set_Vizinho(elementos_marcados[0]);

//                    std::cout << "\n elalter1 logo apos 1.atribuicao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos 1.atribuicao (vizinhos) "<< elalter2.Get_N_Vizinhos();
//                    std::cout << "\n numero de vizinhos "<< num_elem_viz;

                    for (ii = 0; ii < num_elem_viz; ii++)
                    {
                        if (elementos_vizinhos[ii] != elementos_marcados[0] &&
                                elementos_vizinhos[ii] != elementos_marcados[1] )
                        {
                            status0 = bloco_elem_2D->Get_Componente(elementos_vizinhos[ii], el_vizB);
                            if (status0)
                            {
                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
                            }
                            vertices_aux[0] = el_vizB.Get_V1();
                            vertices_aux[1] = el_vizB.Get_V2();
                            vertices_aux[2] = el_vizB.Get_V3();
                            vertices_aux[3] = el_vizB.Get_V4();
                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
                            {
                                for (ik = 0; ik < num_vertices        ; ik++)
                                {
                                    if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
                                    {                                            // corresponde a elementos_marcados[0]
                                        contador_vertices_iguais++;
                                        break; // laco for ik
                                    }

                                }
                            }
                            if (contador_vertices_iguais == 3)
                            {
fprintf(arq1,"\n           numero de vizinhos elalter1 (!=vizinhos) : %d",elalter1.Get_N_Vizinhos());
                                elalter1.Set_Vizinho(elementos_vizinhos[ii]);
fprintf(arq1,"\n           numero de vizinhos de el_vizB (!=vizinhos) : %d",el_vizB.Get_N_Vizinhos());
                                el_vizB .Set_Vizinho(elementos_marcados[0] );
                            }
                            else
                            {

                                for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
                                {
                                    for (ik = 0; ik < num_vertices        ; ik++)
                                    {
//                                        if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
                                        if (vertices_aux[ij]== vertices_vizinhoA[ik]) // lembrando que vertices_vizinho
                                        {                                            // corresponde a elementos_marcados[0]
                                            contador_vertices_iguais++;
                                            break; // laco for ik
                                        }

                                    }
                                }
                                if (contador_vertices_iguais == 3)
                                {
fprintf(arq1,"\n           numero de vizinhos elalter2 (!=vizinhos) : %d",elalter2.Get_N_Vizinhos());
                                    elalter2.Set_Vizinho(elementos_vizinhos[ii]);
fprintf(arq1,"\n           numero de vizinhos de el_vizB (!=vizinhos) : %d",el_vizB.Get_N_Vizinhos());
                                    el_vizB .Set_Vizinho(elementos_marcados[1] );
                                }
                            }
                            // modificada somente a vizinhanca.
                            // Nao ha mudanca de pontos vertices e volume nao precisa ser recalculado
                            bloco_elem_2D->Modifica(elementos_vizinhos[ii],&el_vizB);
                        }
                    }
//                    std::cout << "\n elalter1 logo apos 2.atribuicao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos 2.atribuicao (vizinhos) "<< elalter2.Get_N_Vizinhos();

                    //================================================================
                    // constroi o vetor de vizinhos dos dois elementos cujos vertices
                    // foram alterados, sem contar com o elemento achatado que foi
                    // eliminado no inicio deste processo
                    // ===============================================================
//                    std::cout << "\natribuicao de vizinhanca dos alterados";
                    long  *vizinhos_distantes; // contem a lista dos vizinhos originais dos
                                                                         // elementos cujos vertices foram alterados
                                                                         //  (el_viz e el_vizA)
                    int    num_viz_dist;       // contador do numero de vizinhos distantes
                    long   vizinhos_busca[NUM_MAX_VIZINHOS];   // contem os vizinhos somados
                                                                         // dos elementos alterados
                    int    num_viz_busca=0 ;   // contador do numero de vizinhos do 2 elemento
                    vizinhos_distantes = el_viz.Get_Vizinhos();
                    num_viz_dist       = el_viz.Get_N_Vizinhos();

                    for (ii = 0; ii < num_viz_dist; ii++)
                    {
                        if (vizinhos_distantes[ii] != elementos_marcados[1])
                                        vizinhos_busca[num_viz_busca++] = vizinhos_distantes[ii];
                    }
                    vizinhos_distantes = el_vizA.Get_Vizinhos();
                    num_viz_dist       = el_vizA.Get_N_Vizinhos();

                    for (ii = 0; ii < num_viz_dist; ii++)
                    {
                        if (vizinhos_distantes[ii] != elementos_marcados[0])
                                        vizinhos_busca[num_viz_busca++] = vizinhos_distantes[ii];
                    }



// __________________________________________________________________________
//std::cout << "\n numero de vizinhos para varredura :  " << num_viz_busca;
//for (ii = 0; ii < num_viz_busca; ii++)
//{
//    std::cout << "\n            numero do indice do vizinho :  " << vizinhos_busca[ii] <<"\n" ;
//}

//std::cout << "\n (marcados[0], marcados[1]) : ("<<elementos_marcados[0] <<" , " << elementos_marcados[1];
// __________________________________________________________________________
#if 10
                    for (ii = 0; ii < num_viz_busca; ii++)
                    {
                        status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
                        if (status0)
                        {
                            Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
                        }
                        el_vizB.Elimina_Vizinhanca(elementos_marcados[0]);
                        el_vizB.Elimina_Vizinhanca(elementos_marcados[1]);
                        vertices_aux[0] = el_vizB.Get_V1();
                        vertices_aux[1] = el_vizB.Get_V2();
                        vertices_aux[2] = el_vizB.Get_V3();
                        vertices_aux[3] = el_vizB.Get_V4();
                        for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
                        {
                            for (ik = 0; ik < num_vertices        ; ik++)
                            {
                                if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
                                {                                            // corresponde a elementos_marcados[0]
                                    contador_vertices_iguais++;
                                    break; // laco for ik
                                }
                            }
                        }
                        if (contador_vertices_iguais == 3)
                        {
//                                    std::cout << "\n elalter1 1   " << elalter1.Get_N_Vizinhos();
                            elalter1.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 1   " << el_vizB.Get_N_Vizinhos();
                            el_vizB .Set_Vizinho(elementos_marcados[0] );
                        }
                        else
                        {
                            elalter2.Set_Vizinho(vizinhos_busca[ii]);
                            el_vizB .Set_Vizinho(elementos_marcados[1] );
                        }
                        bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
                    }

                    elalter1.Set_Validade(LEV_VALID);
                    elalter2.Set_Validade(LEV_VALID);
                    bloco_elem_2D->Modifica(elementos_marcados[0],&elalter1);
                    bloco_elem_2D->Modifica(elementos_marcados[1],&elalter2);
                    num_removidos++;
// __________________________________________________________________________
        fprintf(arq1, "\n vertices do tetraedro vizinho1 apos a modificacao:");
                    vertices_vizinho[0] = elalter1.Get_V1();
                    vertices_vizinho[1] = elalter1.Get_V2();
                    vertices_vizinho[2] = elalter1.Get_V3();
                    vertices_vizinho[3] = elalter1.Get_V4();
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho2 apos a modificacao:");
                    vertices_vizinhoA[0] = elalter2.Get_V1();
                    vertices_vizinhoA[1] = elalter2.Get_V2();
                    vertices_vizinhoA[2] = elalter2.Get_V3();
                    vertices_vizinhoA[3] = elalter2.Get_V4();
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
        fprintf(arq1,"\n          numero de elementos removidos acumulado : %ld", num_removidos);
// __________________________________________________________________________





#else  // implementacao sem Elemento_2D::Elimina_Vizinhanca()
                    long  *vizinhos_aux ;
                    int num_viz_auxiliar;
                    // primeiro elimina da lista de vizinhos os elementos_marcados[0 e 1]

                    for (ii = 0; ii < num_viz_busca; ii++)
                    {
                        status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
                        if (status0)
                        {
                            Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
                        }
                        vizinhos_aux = el_vizB.Get_Vizinhos();
                        num_viz_auxiliar  = el_vizB.Get_N_Vizinhos();
//___________________________________________________________________________
//                        std::cout << "\n"<< ii;
//                        std::cout << "\n   ";
//                        for (ij = 0; ij < num_viz_auxiliar; ij++)
//                            std::cout << "   " << vizinhos_aux[ij] ;
//___________________________________________________________________________
                        vertices_aux[0] = el_vizB.Get_V1();
                        vertices_aux[1] = el_vizB.Get_V2();
                        vertices_aux[2] = el_vizB.Get_V3();
                        vertices_aux[3] = el_vizB.Get_V4();
                        Elemento_2D elC;
                        elC.Set_Vertices(&vertices_aux[0]);
                        elC.Set_Validade(LEV_VALID);
                        for (ij = 0; ij < num_viz_auxiliar; ij++)
                        {
//                            std::cout <<"-";
                            if (vizinhos_aux[ij] != elementos_marcados[0] &&
                                    vizinhos_aux[ij] != elementos_marcados[1]  )
                                    elC.Set_Vizinho(vizinhos_aux[ij]);

                        }
                        elC.Calc_Circuns(bloco_pts);

                        bloco_elem_2D->Modifica(vizinhos_busca[ii],&elC);
                        vizinhos_aux = elC.Get_Vizinhos(); num_viz_auxiliar = elC.Get_N_Vizinhos();
//___________________________________________________________________________
//                        std::cout << "\n   ";
//                        for (ij = 0; ij < num_viz_auxiliar; ij++)
//                            std::cout << "   " << vizinhos_aux[ij] ;
//___________________________________________________________________________
                    }
//___________________________________________________________________________
//                        std::cout << "\n modificados os vizinhos dos elementos que compartilham vertice comum";
//___________________________________________________________________________


                    for (ii = 0; ii < num_viz_busca; ii++)
                    {
                            status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
                            if (status0)
                            {
                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
                            }
                            vertices_aux[0] = el_vizB.Get_V1();
                            vertices_aux[1] = el_vizB.Get_V2();
                            vertices_aux[2] = el_vizB.Get_V3();
                            vertices_aux[3] = el_vizB.Get_V4();
                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
                            {
                                for (ik = 0; ik < num_vertices        ; ik++)
                                {
                                    if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
                                    {                                            // corresponde a elementos_marcados[0]
                                        contador_vertices_iguais++;
                                        break; // laco for ik
                                    }

                                }
                            }
                            if (contador_vertices_iguais == 3)
                            {
//                                    std::cout << "\n elalter1 1   " << elalter1.Get_N_Vizinhos();
                                    elalter1.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 1   " << el_vizB.Get_N_Vizinhos();
                                    el_vizB .Set_Vizinho(elementos_marcados[0] );
                                    bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
                            }

                    }
                    for (ii = 0; ii < num_viz_busca; ii++)
                    {
                            status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
                            if (status0)
                            {
                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
                            }
                            vertices_aux[0] = el_vizB.Get_V1();
                            vertices_aux[1] = el_vizB.Get_V2();
                            vertices_aux[2] = el_vizB.Get_V3();
                            vertices_aux[3] = el_vizB.Get_V4();
                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
                            {
                                for (ik = 0; ik < num_vertices        ; ik++)
                                {
                                    if (vertices_aux[ij]== vertices_vizinhoA[ik]) // lembrando que vertices_vizinho
                                    {                                            // corresponde a elementos_marcados[0]
                                        contador_vertices_iguais++;
                                        break; // laco for ik
                                    }

                                }
                            }
                            if (contador_vertices_iguais == 3)
                            {
//                                    std::cout << "\n elalter2 " << elalter2.Get_N_Vizinhos();
                                    elalter2.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 2 " << el_vizB.Get_N_Vizinhos() ;
                                    el_vizB .Set_Vizinho(elementos_marcados[1] );
                                    bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
                            }
                    }

                    elalter1.Set_Validade(LEV_VALID);
                    elalter2.Set_Validade(LEV_VALID);
                    bloco_elem_2D->Modifica(elementos_marcados[0],&elalter1);
                    bloco_elem_2D->Modifica(elementos_marcados[1],&elalter2);
                    num_removidos++;
// __________________________________________________________________________
        fprintf(arq1, "\n vertices do tetraedro vizinho1 apos a modificacao:");
                    vertices_vizinho[0] = elalter1.Get_V1();
                    vertices_vizinho[1] = elalter1.Get_V2();
                    vertices_vizinho[2] = elalter1.Get_V3();
                    vertices_vizinho[3] = elalter1.Get_V4();
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho2 apos a modificacao:");
                    vertices_vizinhoA[0] = elalter2.Get_V1();
                    vertices_vizinhoA[1] = elalter2.Get_V2();
                    vertices_vizinhoA[2] = elalter2.Get_V3();
                    vertices_vizinhoA[3] = elalter2.Get_V4();
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
        fprintf(arq1,"\n          numero de elementos removidos acumulado : %ld", num_removidos);
// __________________________________________________________________________
#endif
#endif // fim do trecho transformado em funcao Elimina_achatado_TipoI
                // ========================================================== fim [2]
                }
                else if (num_vertices_marcados <= num_vertices/*global*/)
                {
                    // ===================================================== inicio [3]
                    // este bloco eh responsavel pela mudanca do tetraedro achatado
                    // e reconstrucao do esquema de vizinhanca consistentemente
                    // =============================================================

                    retorno = 0;
                // ========================================================== fim [3]
                }
                else retorno = num_vertices_marcados; //*AP* avisar do erro
//#endif
            }

        }
     }   // fim do teste de validade do elemento
    }    // fim do laco for(j)
    fclose(arq1);

    char aux [40];
    sprintf(aux,"numero de tetraedros achatados : %d (%d)",num_achatados,num_removidos);
}  // fim do servico de eliminacao de tetraedros achatados


#if 10
//=============================================================================

//int Delaunay2D :: Elimina_Achatado_TipoI(long indice, int num_elem_viz, long *elementos_vizinhos, long *vertices_achatado, long *vertices_marcados,long *elementos_marcados)
//{
//
//    FILE *arq1;
//    arq1 = fopen("remach.dat", "a+");
                    // ===================================================== inicio [2]
                    // este bloco eh responsavel pela eliminacao do tetraedro achatado
                    // e reconstrucao do esquema de vizinhanca consistentemente
                    // =============================================================
                    // constroi arestas dos tetraedros vizinhos que nao contem o ponto marcado como vertice
//    int                  kk;
//    int           status0, status1;
//    int           i;
//    long          j;
//    long         * elementos_vizinhos = NULL;
//    Elemento_2D  el_viz;
//    Elemento_2D  el_vizA;
//    struct {long a1, a2;} arestas1[3], arestas2[3];
//    long vertices_vizinho [num_vertices];
//    long vertices_vizinhoA[num_vertices];
//    long vertice_troca    [2];
//    long vertice_resto    [2];
//    int  ii,ij,ik, identificado = NO;


//fprintf(arq1, "\n===================================================================");
//fprintf(arq1,"\nnumero de elementos vizinhos = %d", num_elem_viz);
//fprintf(arq1,"\nElementos vizinhos : %ld  %ld  %ld  %ld",elementos_vizinhos[0],elementos_vizinhos[1],elementos_vizinhos[2],elementos_vizinhos[3]);


//    bloco_elem_2D->Elimina(indice);     // elimina o achatado da lista

//    vertice_troca[0]=vertice_troca[1]=vertice_resto[0]=vertice_resto[1]=0;

//    status0 = bloco_elem_2D->Get_Componente(elementos_marcados[0],el_viz);
//    status1 = bloco_elem_2D->Get_Componente(elementos_marcados[1],el_vizA);
//    if (status0 || status1)
//    {
//        Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status0);
//    }
//    vertices_vizinho[0]  = el_viz.Get_V1() ; vertices_vizinho[1]  = el_viz.Get_V2();
//    vertices_vizinho[2]  = el_viz.Get_V3() ; vertices_vizinho[3]  = el_viz.Get_V4();
//    vertices_vizinhoA[0] = el_vizA.Get_V1(); vertices_vizinhoA[1] = el_vizA.Get_V2();
//    vertices_vizinhoA[2] = el_vizA.Get_V3(); vertices_vizinhoA[3] = el_vizA.Get_V4();
//
                    // monta as arestas que nao contem o vertice comum dos tetraedros vizinhos
//                    for (ii = 0, ik = 0; ii < num_vertices-1/*global*/; ii++)
//                        for (ij = ii+1; ij <num_vertices-1/*global*/; ij++)
//    for (ii = 0, ik = 0; ii < num_vertices/*global*/; ii++)
//        for (ij = ii+1; ij <num_vertices/*global*/; ij++)
//            if   (vertices_vizinho[ii] != vertices_marcados[0])
//                if (vertices_vizinho[ij] != vertices_marcados[0])
//                {
//                    arestas1[ik]  .a1 = vertices_vizinho[ii];
//                    arestas1[ik++].a2 = vertices_vizinho[ij];
//                }
//
//                    for (ii = 0, ik = 0; ii < num_vertices-1/*global*/; ii++)
//                    for (ij = ii+1; ij <num_vertices-1/*global*/; ij++)
//    for (ii = 0, ik = 0; ii < num_vertices/*global*/; ii++)
//        for (ij = ii+1; ij <num_vertices/*global*/; ij++)
//            if   (vertices_vizinhoA[ii] != vertices_marcados[0])
//                if (vertices_vizinhoA[ij] != vertices_marcados[0])
//                {
//                    arestas2[ik]  .a1 = vertices_vizinhoA[ii];
//                    arestas2[ik++].a2 = vertices_vizinhoA[ij];
//                }
//
                    // verifica aresta comum e associa os vertices da aresta comum vom vertice_troca
                    // e os outros dois vertices do elemento achatado a vertice_resto
//                    int continua_i, continua_j;
//                    for   (ii = 0, continua_i = YES; ii < 3 && continua_i; ii++)
//                        for (ij = 0, continua_j = YES; ij < 3 && continua_j; ij++)
//                        {
//                            if (arestas1[ii].a1 == arestas2[ij].a1 && arestas1[ii].a2 == arestas2[ij].a2)
//                                identificado = YES;
//                            else if (arestas1[ii].a1 == arestas2[ij].a2 && arestas1[ii].a2 == arestas2[ij].a1)
//                                identificado = YES;
//                            if (identificado)
//                            {
//                                vertice_troca[0] = arestas1[ii].a1;
//                                vertice_troca[1] = arestas1[ii].a2;
//                                for (ik = 0, kk = 0; ik < num_vertices; ik++)
//                                {
//                                    if (vertice_troca[0] != vertices_achatado[ik] && vertice_troca[1] != vertices_achatado[ik])
//                                        vertice_resto[kk++] = vertices_achatado[ik];
//                                }  // fim do laco for(ik)
//                                continua_i = continua_j = NO;
//                            }    // fim do teste(identificado)
//
//                        }    // fim do laco for(ij)
//

// __________________________________________________________________________
//        fprintf(arq1,"\n\n        vertice comum : %ld", vertices_marcados[0]);
//        for (ii = 0; ii <3; ii++)
//        {
//            fprintf(arq1,"\n    prim.vizinho -> vertices da aresta %d :  %ld    %ld  " , ii, arestas1[ii].a1,arestas1[ii].a2);
//            fprintf(arq1,"\n    sec..vizinho -> vertices da aresta %d :  %ld    %ld  " , ii, arestas2[ii].a1,arestas2[ii].a2);
//        }
//        fprintf(arq1, "\n vertices do tetraedro achatado %ld:",indice);
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_achatado[0],
//                         vertices_achatado[1], vertices_achatado[2], vertices_achatado[3]);
//
//        fprintf(arq1, "\n vertices da aresta comum :    %ld    %ld",vertice_troca[0], vertice_troca[1]);
//        fprintf(arq1, "\n vertices restantes       :    %ld    %ld",vertice_resto[0], vertice_resto[1]);
//        fprintf(arq1, "\n vertices do tetraedro vizinho1 antes da modificacao %ld:",elementos_marcados[0]);
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
//                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
//        fprintf(arq1, "\n vertices do tetraedro vizinho2 antes da modificacao %ld:",elementos_marcados[1]);
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
//                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
//        fclose(arq1);
// __________________________________________________________________________
                    // PROVIDENCIA A TROCA DE 1 VERTICE PARA O PRIMEIRO VIZINHO
//                    for (ii = 0, continua_i = YES; ii < num_vertices && continua_i; ii++)
//                    {
//                        if (vertices_vizinho[ii] == vertice_resto[0])
//                        {
//                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
//                            {
//                                if (vertices_vizinho[ij] == vertice_troca[0])
//                                { vertices_vizinho[ij] = vertice_resto[1]; continua_j = NO; }
//                            }
//                            continua_i = NO;
//                        }
//                        else if (vertices_vizinho[ii] == vertice_resto[1])
//                        {
//                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
//                            {
//                                if (vertices_vizinho[ij] == vertice_troca[0])
//                                { vertices_vizinho[ij] = vertice_resto[0]; continua_j = NO; }
//                            }
//                            continua_i = NO;
//                        }
//                    }   // fim do laco for (ii)
//
//                    // PROVIDENCIA A TROCA DE 1 VERTICE PARA O SEGUNDO VIZINHO
//                    for (ii = 0, continua_i = YES; ii < num_vertices && continua_i; ii++)
//                    {
//                        if (vertices_vizinhoA[ii] == vertice_resto[0])
//                        {
//                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
//                            {
//                                if (vertices_vizinhoA[ij] == vertice_troca[1])
//                                {
//                                    vertices_vizinhoA[ij] = vertice_resto[1];
//                                    continua_j = NO;
//                                }
//                            }
//                            continua_i = NO;
//                        }
//                        else if (vertices_vizinhoA[ii] == vertice_resto[1])
//                        {
//                            for (ij = 0, continua_j = YES; ij < num_vertices && continua_j; ij++)
//                            {
//                                if (vertices_vizinhoA[ij] == vertice_troca[1])
//                                {    vertices_vizinhoA[ij] = vertice_resto[0];    continua_j = NO; }
//                            }
//                            continua_i = NO;
//                        }
//                    }   // fim do laco for (ii)
//
//                    // ===============================================================
//                    // neste ponto, faz-se a atribuicao da vizinhanca
//                    // ===============================================================
//
//                    Elemento_2D elalter1, elalter2;  // vao armazenar momentaneamente
//                                                                                         // as caracteristicas dos elementos
//                                                                                         // que compartilham vertice comum
//                    Elemento_2D el_vizB;
//                    long vertices_aux[4];
//                    int contador_vertices_iguais;
//                    std::cout << "\n -----------------------------------------------";
//                    std::cout << "\n elalter1 logo apos declaracao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos declaracao (vizinhos) "<< elalter2.Get_N_Vizinhos();

//                    elalter1.Set_Vertices(&vertices_vizinho[0]);
//                    elalter1.Calc_Circuns(bloco_pts);
//fprintf(arq1,"\n           numero de vizinhos elalter1 (antes mutuo): %d",elalter1.Get_N_Vizinhos());
//                    elalter1.Set_Vizinho(elementos_marcados[1]);
//fprintf(arq1,"\n           numero de vizinhos elalter1 (apos mutuo) : %d",elalter1.Get_N_Vizinhos());
//
//                    elalter2.Set_Vertices(&vertices_vizinhoA[0]);
//                    elalter2.Calc_Circuns(bloco_pts);
//fprintf(arq1,"\n           numero de vizinhos elalter2 (antes mutuo) : %d",elalter2.Get_N_Vizinhos());
//                    elalter2.Set_Vizinho(elementos_marcados[0]);
//fprintf(arq1,"\n           numero de vizinhos elalter2 (apos mutuo)  : %d",elalter2.Get_N_Vizinhos());
//                    std::cout << "\n elalter1 logo apos 1.atribuicao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos 1.atribuicao (vizinhos) "<< elalter2.Get_N_Vizinhos();
//                    std::cout << "\n numero de vizinhos "<< num_elem_viz;
//fprintf(arq1,"\nPONTO1  <---");

//                    for (ii = 0; ii < num_elem_viz; ii++)
//                    {
//                        if (elementos_vizinhos[ii] != elementos_marcados[0] &&
//                                elementos_vizinhos[ii] != elementos_marcados[1] )
//                        {
//                            status0 = bloco_elem_2D->Get_Componente(elementos_vizinhos[ii], el_vizB);
//                            if (status0)
//                            {
//                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
//                            }
//                            vertices_aux[0] = el_vizB.Get_V1();
//                            vertices_aux[1] = el_vizB.Get_V2();
//                            vertices_aux[2] = el_vizB.Get_V3();
//                            vertices_aux[3] = el_vizB.Get_V4();
//                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
//                            {
//                                for (ik = 0; ik < num_vertices        ; ik++)
//                                {
//                                    if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
//                                    {                                            // corresponde a elementos_marcados[0]
//                                        contador_vertices_iguais++;
//                                        break; // laco for ik
//                                    }
//
//                                }
//                            }
//                            if (contador_vertices_iguais == 3)
//                            {
//fprintf(arq1,"\n           numero de vizinhos elalter1 (!=vizinhos) : %d",elalter1.Get_N_Vizinhos());
//                                elalter1.Set_Vizinho(elementos_vizinhos[ii]);
//fprintf(arq1,"\n           numero de vizinhos de el_vizB (!=vizinhos) : %d",el_vizB.Get_N_Vizinhos());
//                                el_vizB .Set_Vizinho(elementos_marcados[0] );
//                            }
//                            else
//                            {
//
//                                for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
//                                {
//                                    for (ik = 0; ik < num_vertices        ; ik++)
//                                    {
//                                        if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
//                                        if (vertices_aux[ij]== vertices_vizinhoA[ik]) // lembrando que vertices_vizinho
//                                        {                                            // corresponde a elementos_marcados[0]
//                                            contador_vertices_iguais++;
//                                            break; // laco for ik
//                                        }
//
//                                    }
//                                }
//                                if (contador_vertices_iguais == 3)
//                                {
//fprintf(arq1,"\n           numero de vizinhos elalter2 (!=vizinhos) : %d",elalter2.Get_N_Vizinhos());
//                                    elalter2.Set_Vizinho(elementos_vizinhos[ii]);
//fprintf(arq1,"\n           numero de vizinhos de el_vizB (!=vizinhos) : %d",el_vizB.Get_N_Vizinhos());
//                                    el_vizB .Set_Vizinho(elementos_marcados[1] );
//                                }
//                            }
//                            // modificada somente a vizinhanca.
//                            // Nao ha mudanca de pontos vertices e volume nao precisa ser recalculado
//                            bloco_elem_2D->Modifica(elementos_vizinhos[ii],&el_vizB);
//                        }
//                    }
//                    std::cout << "\n elalter1 logo apos 2.atribuicao (vizinhos) "<< elalter1.Get_N_Vizinhos();
//                    std::cout << "\n elalter2 logo apos 2.atribuicao (vizinhos) "<< elalter2.Get_N_Vizinhos();
//fprintf(arq1,"\nPONTO2  <---");

                    //================================================================
                    // constroi o vetor de vizinhos dos dois elementos cujos vertices
                    // foram alterados, sem contar com o elemento achatado que foi
                    // eliminado no inicio deste processo
                    // ===============================================================
//                    std::cout << "\natribuicao de vizinhanca dos alterados";
//                    long  *vizinhos_distantes; // contem a lista dos vizinhos originais dos
//                                                                         // elementos cujos vertices foram alterados
//                                                                         //  (el_viz e el_vizA)
//                    int    num_viz_dist;       // contador do numero de vizinhos distantes
//                    long   vizinhos_busca[NUM_MAX_VIZINHOS];   // contem os vizinhos somados
//                                                                         // dos elementos alterados
//                    int    num_viz_busca=0 ;   // contador do numero de vizinhos do 2 elemento
//                    vizinhos_distantes = el_viz.Get_Vizinhos();
//                    num_viz_dist       = el_viz.Get_N_Vizinhos();
//
//                    for (ii = 0; ii < num_viz_dist; ii++)
//                    {
//                        if (vizinhos_distantes[ii] != elementos_marcados[1])
//                                        vizinhos_busca[num_viz_busca++] = vizinhos_distantes[ii];                    }
//                    vizinhos_distantes = el_vizA.Get_Vizinhos();
//                    num_viz_dist       = el_vizA.Get_N_Vizinhos();
//
//                    for (ii = 0; ii < num_viz_dist; ii++)
//                    {
//                        if (vizinhos_distantes[ii] != elementos_marcados[0])
//                                        vizinhos_busca[num_viz_busca++] = vizinhos_distantes[ii];
//                    }
//
//
//
// __________________________________________________________________________
//std::cout << "\n numero de vizinhos para varredura :  " << num_viz_busca;
//for (ii = 0; ii < num_viz_busca; ii++)
//{
//    std::cout << "\n            numero do indice do vizinho :  " << vizinhos_busca[ii] <<"\n" ;
//}

//std::cout << "\n (marcados[0], marcados[1]) : ("<<elementos_marcados[0] <<" , " << elementos_marcados[1];
// __________________________________________________________________________
//#if 10

//fprintf(arq1,"\nPONTO3  <---");
//                    for (ii = 0; ii < num_viz_busca; ii++)
//                    {
//                        status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
//                        if (status0)
//                        {
//                            Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
//                        }
//                        el_vizB.Elimina_Vizinhanca(elementos_marcados[0]);
//                        el_vizB.Elimina_Vizinhanca(elementos_marcados[1]);
//                        vertices_aux[0] = el_vizB.Get_V1();
//                        vertices_aux[1] = el_vizB.Get_V2();
//                        vertices_aux[2] = el_vizB.Get_V3();
//                        vertices_aux[3] = el_vizB.Get_V4();
//                        for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
//                        {
//                            for (ik = 0; ik < num_vertices        ; ik++)
//                            {
//                                if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
//                                {                                            // corresponde a elementos_marcados[0]
//                                    contador_vertices_iguais++;
//                                    break; // laco for ik
//                                }
//                            }
//                        }
//                        if (contador_vertices_iguais == 3)
//                        {
//                                    std::cout << "\n elalter1 1   " << elalter1.Get_N_Vizinhos();
//fprintf(arq1,"\n           numero de vizinhos elalter1 (!=vizinhos) : %d",elalter2.Get_N_Vizinhos());
//                            elalter1.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 1   " << el_vizB.Get_N_Vizinhos();
//                            el_vizB .Set_Vizinho(elementos_marcados[0] );
//                        }
//                        else
//                        {
//                            elalter2.Set_Vizinho(vizinhos_busca[ii]);
//                            el_vizB .Set_Vizinho(elementos_marcados[1] );
//                        }
//                    bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
//                    }
//
//                    elalter1.Set_Validade(LEV_VALID);
//                    elalter2.Set_Validade(LEV_VALID);
//                    bloco_elem_2D->Modifica(elementos_marcados[0],&elalter1);
//                    bloco_elem_2D->Modifica(elementos_marcados[1],&elalter2);
//                    num_removidos++;
// __________________________________________________________________________
//        fprintf(arq1, "\n vertices do tetraedro vizinho1 apos a modificacao:");
//                    vertices_vizinho[0] = elalter1.Get_V1();
//                    vertices_vizinho[1] = elalter1.Get_V2();
//                    vertices_vizinho[2] = elalter1.Get_V3();
//                    vertices_vizinho[3] = elalter1.Get_V4();
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
//                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
//        fprintf(arq1, "\n vertices do tetraedro vizinho2 apos a modificacao:");
//                    vertices_vizinhoA[0] = elalter2.Get_V1();
//                    vertices_vizinhoA[1] = elalter2.Get_V2();
//                    vertices_vizinhoA[2] = elalter2.Get_V3();
//                    vertices_vizinhoA[3] = elalter2.Get_V4();
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
//                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
// __________________________________________________________________________


    // ========================================================== fim [2]

//#else  // implementacao sem Elemento_2D::Elimina_Vizinhanca()
//                    long  *vizinhos_aux ;
//                    int num_viz_auxiliar;
//                    // primeiro elimina da lista de vizinhos os elementos_marcados[0 e 1]
//
//                    for (ii = 0; ii < num_viz_busca; ii++)
//                    {
//                        status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
//                        if (status0)
//                        {
//                            Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
//                        }
//                        vizinhos_aux = el_vizB.Get_Vizinhos();
//                        num_viz_auxiliar  = el_vizB.Get_N_Vizinhos();
//___________________________________________________________________________
//                        std::cout << "\n"<< ii;
//                        std::cout << "\n   ";
//                        for (ij = 0; ij < num_viz_auxiliar; ij++)
//                            std::cout << "   " << vizinhos_aux[ij] ;
//___________________________________________________________________________
//                        vertices_aux[0] = el_vizB.Get_V1();
//                        vertices_aux[1] = el_vizB.Get_V2();
//                        vertices_aux[2] = el_vizB.Get_V3();
//                        vertices_aux[3] = el_vizB.Get_V4();
//                        Elemento_2D elC;
//                        elC.Set_Vertices(&vertices_aux[0]);
//                        elC.Set_Validade(LEV_VALID);
//                        for (ij = 0; ij < num_viz_auxiliar; ij++)
//                        {
//                            std::cout <<"-";
//                            if (vizinhos_aux[ij] != elementos_marcados[0] &&
//                                    vizinhos_aux[ij] != elementos_marcados[1]  )
//                                    elC.Set_Vizinho(vizinhos_aux[ij]);
//
//                        }
//                        elC.Calc_Circuns(bloco_pts);
//
//                        bloco_elem_2D->Modifica(vizinhos_busca[ii],&elC);
//                        vizinhos_aux = elC.Get_Vizinhos(); num_viz_auxiliar = elC.Get_N_Vizinhos();
//___________________________________________________________________________
//                        std::cout << "\n   ";
//                        for (ij = 0; ij < num_viz_auxiliar; ij++)
//                            std::cout << "   " << vizinhos_aux[ij] ;
//___________________________________________________________________________
//                    }
//___________________________________________________________________________
//                        std::cout << "\n modificados os vizinhos dos elementos que compartilham vertice comum";
//___________________________________________________________________________
//
//
//                    for (ii = 0; ii < num_viz_busca; ii++)
//                    {
//                            status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
//                            if (status0)
//                            {
//                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
//                            }
//                            vertices_aux[0] = el_vizB.Get_V1();
//                            vertices_aux[1] = el_vizB.Get_V2();
//                            vertices_aux[2] = el_vizB.Get_V3();
//                            vertices_aux[3] = el_vizB.Get_V4();
//                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
//                            {
//                                for (ik = 0; ik < num_vertices        ; ik++)
//                                {
//                                    if (vertices_aux[ij]== vertices_vizinho[ik]) // lembrando que vertices_vizinho
//                                    {                                            // corresponde a elementos_marcados[0]
//                                        contador_vertices_iguais++;
//                                        break; // laco for ik
//                                    }
//
//                                }
//                            }
//                            if (contador_vertices_iguais == 3)
//                            {
//                                    std::cout << "\n elalter1 1   " << elalter1.Get_N_Vizinhos();
//                                    elalter1.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 1   " << el_vizB.Get_N_Vizinhos();
//                                    el_vizB .Set_Vizinho(elementos_marcados[0] );
//                                    bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
//                            }
//
//                    }
//                    for (ii = 0; ii < num_viz_busca; ii++)
//                    {
//                            status0 = bloco_elem_2D->Get_Componente(vizinhos_busca[ii], el_vizB);
//                            if (status0)
//                            {
//                                Informa_Erro("Erro ?? : Vizinho nao localizado em Elimina_Achatados. Execucao Interrompida !", status0);
//                            }
//                          vertices_aux[0] = el_vizB.Get_V1();
//                            vertices_aux[1] = el_vizB.Get_V2();
//                            vertices_aux[2] = el_vizB.Get_V3();
//                            vertices_aux[3] = el_vizB.Get_V4();
//                            for(ij = 0, contador_vertices_iguais = 0; ij < num_vertices /*global*/; ij++)
//                            {
//                                for (ik = 0; ik < num_vertices        ; ik++)
//                                {
//                                    if (vertices_aux[ij]== vertices_vizinhoA[ik]) // lembrando que vertices_vizinho
//                                    {                                            // corresponde a elementos_marcados[0]
//                                        contador_vertices_iguais++;
//                                        break; // laco for ik
//                                    }
//
//                                }
//                            }
//                            if (contador_vertices_iguais == 3)
//                            {
//                                    std::cout << "\n elalter2 " << elalter2.Get_N_Vizinhos();
//                                    elalter2.Set_Vizinho(vizinhos_busca[ii]);
//                                    std::cout << "\n elvizB 2 " << el_vizB.Get_N_Vizinhos() ;
//                                    el_vizB .Set_Vizinho(elementos_marcados[1] );
//                                    bloco_elem_2D->Modifica(vizinhos_busca[ii],&el_vizB);
//                            }
//                    }
//
//                    elalter1.Set_Validade(LEV_VALID);
//                    elalter2.Set_Validade(LEV_VALID);
//                    bloco_elem_2D->Modifica(elementos_marcados[0],&elalter1);
//                    bloco_elem_2D->Modifica(elementos_marcados[1],&elalter2);
//                    num_removidos++;
// __________________________________________________________________________
//        fprintf(arq1, "\n vertices do tetraedro vizinho1 apos a modificacao:");
//                    vertices_vizinho[0] = elalter1.Get_V1();
//                    vertices_vizinho[1] = elalter1.Get_V2();
//                    vertices_vizinho[2] = elalter1.Get_V3();
//                    vertices_vizinho[3] = elalter1.Get_V4();
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
//                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
//        fprintf(arq1, "\n vertices do tetraedro vizinho2 apos a modificacao:");
//                    vertices_vizinhoA[0] = elalter2.Get_V1();
//                    vertices_vizinhoA[1] = elalter2.Get_V2();
//                    vertices_vizinhoA[2] = elalter2.Get_V3();
//                    vertices_vizinhoA[3] = elalter2.Get_V4();
//        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
//                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
// __________________________________________________________________________
//#endif
//    fclose(arq1);
//    return 1;
//}
#endif


#if 0
//===========================================================================
//               class Delaunay2D :: Verifica_Tipo_Achatado
//===========================================================================
//
//  Descricao          : Uma vez identificado um elemento achatado, verifica
//                       se dois de seus vizinhos compartilham um vertice. Se
//                       compartilham, informa a funcao requisitante que o
//                       tetraedro e removivel.
//  Parametros         :
//                       el       -> o elemento achatado (input)
//                       vertmarc -> lista dos vertices marcados(tantos quanto
//                                   for o valor de retorno);
//                       elemmarc -> lista do indice dos elementos marcados
//                                   (util somente se valor e retorno = 1, e,
//                                   neste caso contem sempre dois componentes)
//
//
//  Retorno            : 1  -> se dois tetraedros compartilham um vertice
//                       n  -> caso contrario, o numero de vertices pesquisados
//                             que nao pertencem ao achatado(2, 3 ou 4).
//                       11000 ->erro
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/02/95
//
//===========================================================================
short int  Delaunay2D :: Verifica_Tipo_Achatado(Elemento_2D &el, long *vertmarc, long *elemmarc)
{       // ====================================================== inicio [1]
                // este bloco e responsavel por identificar o tipo de tetraedro
                // achatado (removivel ou nao), pela procura da ocorrencia de um
                // vertice compartilhado entre os vizinhos do achatado
                // ==============================================================

                Elemento_2D el_viz;
                long      vertices_achatado[num_vertices/*global*/];
                long      vertices_vizinho[num_vertices];
                long    * elementos_vizinhos = NULL;
                int              k, kk, kkk, kkkk, l;
                int              continua_k, continua_kk, continua_kkk;
                int       status0, retorno;
                short int num_vertices_marcados = 0;

                vertices_achatado[0] = el.Get_V1();
                vertices_achatado[1] = el.Get_V2();
                vertices_achatado[2] = el.Get_V3();
                vertices_achatado[3] = el.Get_V4();
                elementos_vizinhos   = el.Get_Vizinhos();

                for (k = 0, continua_k = YES; k <el.Get_N_Vizinhos() && continua_k; k++)
//                k = 0; continua_k = YES; l = 0; *GMM*  modifica��o no la�o para trabalhar com vizinhos ordenados
//                while(l <el.Get_N_Vizinhos() && continua_k)
                {
//                    if (elementos_vizinhos[k] >= 0)
//                    {
//                        l++;


                    status0 = bloco_elem_2D->Get_Componente(elementos_vizinhos[k],el_viz);
                    if (status0)
                    {
                        Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status0);
                    }
                    vertices_vizinho[0] = el_viz.Get_V1();
                    vertices_vizinho[1] = el_viz.Get_V2();
                    vertices_vizinho[2] = el_viz.Get_V3();
                    vertices_vizinho[3] = el_viz.Get_V4();
                    // laco kk : varredura dos vertices dos elementos vizinhos ao achatado
                    for (kk = 0, continua_kk = YES; kk < num_vertices/*global*/ && continua_kk; kk++)
                    {

                    // laco kkk : varredura dos vertices do elemento achatado
                        for (kkk = 0, continua_kkk = YES; kkk < num_vertices /*global*/ && continua_kkk; kkk++)
                            if (vertices_achatado[kkk] == vertices_vizinho[kk])
                                continua_kkk = NO;

                        if (continua_kkk) // vertice do vizinho nao e comum ao achatado
                        {
                            if (num_vertices_marcados > 0)
                            {
                                for(kkkk = 0; kkkk < num_vertices_marcados; kkkk++)
                                {
                                    if (vertices_vizinho[kk] == vertmarc[kkkk])
                                    {
                                        vertmarc[0] = vertices_vizinho  [kk];
                                        elemmarc[1] = elemmarc[kkkk];
                                        elemmarc[0] = elementos_vizinhos[k];
                                        num_vertices_marcados = 1;
                                        continua_k = continua_kk = NO;
                                        break;     // quebra o laco kkkk
                                    } // fim do teste de vertice presente na lista de marcados
                                }   // fim do laco kkkk
                                if (continua_k) // significa que nao foi encontrado um vertice comum
                                {
                                    vertmarc[num_vertices_marcados]   = vertices_vizinho[kk];
                                    elemmarc[num_vertices_marcados++] = elementos_vizinhos[k];
                                    continua_kk = NO;
                                }
                            }
                            else
                            {
                                vertmarc [num_vertices_marcados]   = vertices_vizinho[kk];
                                elemmarc [num_vertices_marcados++] = elementos_vizinhos[k];
                                continua_kk = NO;
                            }     // fim do teste de numero de vertices marcados

                        }       // fim do teste de coincidencia de vertices com el. achatado
                    }         // fim do laco kk
                //    }        //fim if
                //    k++;
                }           // fim do laco k



} // ========================================================== fim [1]

#endif

//===========================================================================
//               class Delaunay2D :: Elimina_Englobante
//===========================================================================
//
//  Descricao          : Dada a malha de elementos finitos em toda a regiao
//                       do paralelepipedo envolvente elimina os vertices desse
//                       paralelepipedo e os elementos ligados aos vertices
//                         Foi incluida uma nova funcionalidade, a elimina��o
//                         de elementos indesejados em geometrias quase convexas
//                       (um "C", por exemplo)
//
//  versao             : 1.2
//  programador        : Angelo Passaro
//    modificado por     : Guilherme Moreira Magalh�es
//  ultima modificacao : 29/06/2000
//
//===========================================================================

void Delaunay2D :: Elimina_Englobante(void) {
    lev_math::USINT  status;
    long   i;
    Elemento_2D el_atual;

    for (i = 0; i < bloco_elem_2D->Get_Total_Componentes(); i++) { //========
        status = bloco_elem_2D->Get_Componente(i, el_atual);
        if (status) {
         status =  110;
         Informa_Erro("Erro 110: Dados Inconsistentes. Execucao interrompida...",status);
        }

        if (el_atual.Get_Validade())  // elemento LEV_VALID
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// verifica se pelo menos um dos vertices pertence ao paralelepipedo
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (el_atual.Get_V1() < 4 || el_atual.Get_V2() < 4 ||
                    el_atual.Get_V3() < 4)
                bloco_elem_2D->Elimina(i);
    } // fim do for i ------------------------------------------------------
} // fim do metodo Elimina_Englobante =======================================




//===========================================================================
//               class Delaunay2D :: Estatistica
//===========================================================================
//
//  Descricao          : Calcula o valor da media e desvio padrao do volume
//                       dos elementos tetraedricos de primeira ordem gerados
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/09/94
//
//===========================================================================

void  Delaunay2D :: Estatistica (double & media, double &desvio) {
     Elemento_2D   EEE;
     long          elementos_total,
                                 i              ;
     lev_math::USINT         status         ;
     elementos_total = bloco_elem_2D->Get_Total_Componentes();
   media = desvio = 0;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CALCULA O VOLUME MEDIO DOS ELEMENTOS LEV_VALIDS E O DESVIO PADRAO
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     for (i = 0; i < elementos_total; i++) {
         status = bloco_elem_2D->Get_Componente(i, EEE);
         if (status) {
             Informa_Erro(" Erro em Estatistica. Execucao Interrompida",999);
         } // fim do teste de status de Get_Componente
         else {
             if (!EEE.Get_Validade()) ;  // nao faz nada e passa para o proximo
             else { // elemento eh LEV_VALID
                    media += fabs(EEE.Get_Volume());
             }
      }
   }
     media /= bloco_elem_2D->Get_N_Comp_Validos_Absoluto();

     for (i = 0; i < elementos_total; i++) {
         status = bloco_elem_2D->Get_Componente(i, EEE);
         if (!EEE.Get_Validade()) ;  // nao faz nada e passa para o proximo
         else { // elemento eh LEV_VALID
                 desvio += (fabs(EEE.Get_Volume())- media) * (fabs(EEE.Get_Volume())- media) ;
         }
     }

     desvio = sqrt(desvio/bloco_elem_2D->Get_N_Comp_Validos_Absoluto());


} // fim do servico Estatistica ---------------------------------------------






//===========================================================================
//               class Delaunay2D :: Gera_Elementos_Partida
//===========================================================================
//
//  Descricao          : Dado o paralelepipedo envolvente divide-o em 5
//                       elementos tetraedricos de primeira ordem
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 22/04/94
//
//===========================================================================

void  Delaunay2D :: Gera_Elementos_Partida (void) {
    lev_math::Uchar droga;
    lev_math::USINT status = 0;

    Dimensiona_Englobante();

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // inicializa os 2 primeiros tri�ngulos
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    droga = bloco_elem_2D -> Inclui (bloco_pts,0,1,2) ;
    if (droga) { Informa_Erro("Erro na inclusao do primeiro elemento ...",droga);}
    droga = bloco_elem_2D -> Inclui (bloco_pts,0,2,3) ;

    if (droga) { Informa_Erro("Erro na inclusao do segundo elemento ...",droga);}
    if ((status = bloco_elem_2D -> Atribui_Vizinhanca(NULL,0, 2))!=0)
    {
            if (status == 306)
            {
                Informa_Erro("Erro 306 : Execucao Interrompida!", 306);
            }
    }
} // fim do metodo Gera_Elementos_Partida====================================


// ==========================================================================
//               class Delaunay2D ::  Gets
// ==========================================================================
//
//  Descricao          : informam alguns dos parametros da lista de
//                       pontos e da lista de elementos
//
// ==========================================================================

#if 0 // substituida pelo servico de mesmo nome em Delaunay (14/01/97)
long Delaunay2D :: Get_Limite_Pontos(void)
{
    return bloco_pts->Get_Limite_Superior();
}
#endif // delimita a funcao Get_limite_pontos

long Delaunay2D :: Get_N_Pontos(void)
{
//    return bloco_pts->Get_Total_Componentes();
    return bloco_pts.size();
}


long Delaunay2D :: Get_N_Elementos_2D(void)
{
    return bloco_elem_2D->Get_Total_Componentes();
}





//===========================================================================
//               class Delaunay2D :: Refina_Media
//===========================================================================
//
//  Descricao          : Dada a  malha de elementos finitos inicial, gerada
//                       pelo metodo de Delaunay (Discretiza) verifica a
//                       necessidade de refinar um determinado tetraedro
//                       atraves da comparacao com os elementos vizinhos.
//
//  Observacoes        : Este servico tem dois comportamentos distintos para
//                       a geracao de pontos, sendo que o primeiro e utilizado
//                       somente na primeira passada pela lista de elementos ja
//                       existentes. Depois disso, um elemento so e dividido se
//                       nao satisfaz um criterio de comparacao de volume com
//                       o medio dos vizinhos.
//
//  Retorno            : 0    Ok
//                       115  alcancado o limite de numero de pontos permissivel
//                            na lista
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 22/12/94
//
//===========================================================================

lev_math::USINT  Delaunay2D :: Refina_Media (void) {

 lev_math::USINT  status = 0, retorno;

 const double fator_volume= .1;//.1     // define o valor minimo do volume de um tetraedro
                                        // para que ele sofra um refinamento em cada ciclo
 const double fator_vol_medio = 1.5;//1.5 // fator que define quando um tetraedro deve ser
                                          // dividido em funcao dos volume medio dos vizinhos

 if (bloco_elem_2D)
 {
    long elementos_total;  // numero de total, atual, de elementos da lista de
                                                 // de elementos 3D

    long i             ,                // contador dos lacos de varredura de elementos
                contador      ,
                *viz          ;   // vetor de vizinhos de um elemento

    Elemento_2D EEE;       // utilizado para atualizar o status dos elementos
                                                 // que foram alterados durante o processo.
    Elemento_2D el_atual;
    Pt_Malha       PP, P1, P2;
    Pt_Malha       pt_atual;
    double  xb, yb;   // coordenadas do baricentro do elemento escolhido
    double volume_medio ,   // volume medio dos elementos
                 volume_minimo,   // volume minimo dos elementos para os quais sera incluido um ponto
                 desvio_volume,   // desvio padrao dos volumes dos elementos
                 rinsc        ,
                 rcirc2       ;

    double maior_volume, absvolume;
    lev_math::USINT    j       ,
            n_viz   ;  // numero de vizinhos de um dado elemento
    lev_enum::LEV_BOOL  continua;
    FACE faces_el_atual[3];//armazena faces de um simplexo
    double comprimento = 0, aux_comprimento, menor, comprimentos[3];//armazena comprimento de aresta
    int maior_aresta, menor_aresta;//indice das arestas  maior e menor

    double aux_x, aux_y;//auxiliares para calculo de comprimento
    long k;
    double fator;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Varre toda a lista de elementos e inclui os pontos dos baricentros na lista
    // de pontos. Em seguida aplica altera malha. Note-se que a cada novo ponto
    // incluido e feita a atualizacao da lista, de forma que os elementos que tive-
    // sido eliminados com a inclusao do novo ponto sera considerado eliminado e
    // nao sofrera a divisao.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    continua = lev_enum::YES;

     volume_medio  = 0;
     desvio_volume = 0;

     Estatistica(volume_medio, desvio_volume);

     volume_minimo = volume_medio - desvio_volume;
     if (volume_minimo <volume_medio  * fator_volume) volume_minimo = volume_medio * fator_volume;



    while(continua) {
        contador = 0;
        elementos_total = bloco_elem_2D->Get_Total_Componentes();
        for (i = 0; i < elementos_total; i++) {

         status =  bloco_elem_2D->Get_Componente(i,el_atual);
         if (status ) {};
         if (el_atual.Get_Validade()) {
            if (fabs(el_atual.Get_Volume()) > volume_minimo) {

            rinsc  = el_atual.Calc_R_Insc(bloco_pts);
            rcirc2    = el_atual.Get_Raio_Circ(); //get raio_circ retorna o raio circunscrito

            if (rinsc/rcirc2 > 1.e-2) // *AP* em teste  02/06/95 1.e-3
            {
                viz = el_atual.Get_Vizinhos();
                n_viz = el_atual.Get_N_Vizinhos();
                maior_volume = 0;
                xb = yb = 0;   // inicializando as coordenadas do baricentro
                for (j = 0, volume_medio = 0; j < n_viz; j++) {
                 status = bloco_elem_2D->Get_Componente(viz[j],EEE);

                 absvolume = EEE.Get_Volume();
                 (absvolume > 0)? absvolume : absvolume = -absvolume;
                 volume_medio += absvolume;
                 if (maior_volume < absvolume) maior_volume = absvolume;

                }  // fim do laco j
                volume_medio /= n_viz;
                absvolume = el_atual.Get_Volume();
                (absvolume < 0)? absvolume : absvolume = -absvolume;
                if (absvolume >  maior_volume && absvolume > fator_vol_medio*volume_medio)  {

                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //  ESTE TRECHO E IGUAL AO TRECHO ACIMA NA PRIMEIRA DIVISAO DOS TETRAEDROS
                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    PP = bloco_pts[el_atual.Get_V1()];
                    if (status) {
                         Informa_Erro("Erro em Refina. Execucao Interrompida", 999);
                         return status;
                    } else {
                         xb += PP.X(); yb += PP.Y();
                    }
                    PP = bloco_pts[el_atual.Get_V2()];
                    if (status) {
                         Informa_Erro("Erro em Refina. Execucao Interrompida", 999);
                         return status;
                    }
                    else {

                         xb += PP.X(); yb += PP.Y();
                    }
//                    status = bloco_pts->Get_Componente(el_atual.Get_V3(),PP);
                    PP = bloco_pts[el_atual.Get_V3()];
                    if (status) {
                         Informa_Erro("Erro em Refina. Execucao Interrompida", 999);
                         return status;
                    }
                    else {

                         xb += PP.X(); yb += PP.Y();
                    }

                    xb /= 3; yb /= 3;// CALCULO DA COORDENADA DO NOVO PONTO

/////////////////////////////////  STL ///////////////////////////////////////////////
//                    status = bloco_pts->Inclui(xb,yb,0,(lev_math::Uchar)REF);
                    Pt_Malha pontob;

                    pontob.X(xb); pontob.Y(yb);
                    pontob.Set_Tipo((lev_math::Uchar)REF);
                    bloco_pts.push_back(pontob);
//////////////////////////////////////////////////////////////////////////////////////
                    if (status){
                         status = 115;
                         return status;
                    }
                    else {  // ponto incluido normalmente
                    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    // Nesta situacao, qualquer ponto incluido garantidamente nao gera nenhuma
                    // degenerescencia ou volume igual a zero
                    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        //if (!(bloco_pts.size()%50))
                        //{
                        //   char aux [15];
                        //    std::cout <<bloco_pts.size() <<aux <<10;
                        //}

                        contador++;
                        retorno = Altera_Malha(i, bloco_pts.size() - 1);

                        // ocorrencia de caso degenerado
                        if (retorno == 20 || retorno == 30) {
                            bloco_pts.pop_back(); // elimina o componente recem gerado
                            contador--;
                        } else if (retorno == 15) {
                            bloco_pts.pop_back(); // elimina o componente recem gerado
                            Informa_Erro("Erro 15: Atingido o limite maximo de elementos do sistema. Tente aumentar a quantidade de memoria extendida disponivel!", retorno);
                        }

                        if(i >= bloco_elem_2D->Get_Total_Componentes()) // interrompe laco i (houve compactacao da lista) // *AP* teste somente
                        {
                            // este teste visa evitar um possivel erro ocasionado pela compactacao do
                            // vetor de elementos em Altera_Malha. Se a compactacao ocorrer, o indice do
                            // proximo elememento podera ser menor que o numero de eleemntos existente
                            // anteriormente na lista (elementos_total. Nestas condicoes optou-se por
                            // interromper o processo, pura e simplesmente.
                            break;
                        }
                    } // fim do teste de inclusao de pontos
                }  // fim do teste de volume do elemento contra os vizinhos
            } else {
            //------------------------  *GMM* ------------------------------------
                el_atual.Monta_Faces(faces_el_atual);
                comprimento = 0;
                menor = 1.e12;
                for (k=0; k<n_vertices; k++)// procura pela maior aresta de um simplexo
                {                   // e tamb�m pela menor.
                    P1 = bloco_pts[faces_el_atual[k].V1];
                    P2 = bloco_pts[faces_el_atual[k].V2];
                    if (status) {
                         Informa_Erro("Erro em Refina. Execucao Interrompida", 999);
                         return status;
                    }
                    aux_x = P1.X()-P2.X();
                    aux_x *= aux_x;
                    aux_y = P1.Y()-P2.Y();
                    aux_y *= aux_y;
                    aux_comprimento = sqrt(aux_x + aux_y);
                    comprimentos[k] = aux_comprimento;//guarda comprimentos para calculos
                    if (aux_comprimento > comprimento)
                    {
                        comprimento = aux_comprimento;
                        maior_aresta = k;
                    }
                    if (aux_comprimento < menor)
                    {
                        menor = aux_comprimento;
                        menor_aresta = k;
                    }
                }
                P1 = bloco_pts[faces_el_atual[maior_aresta].V1];
                P2 = bloco_pts[faces_el_atual[maior_aresta].V2];

                //P1 sempre ser� a origem
                if ( (faces_el_atual[menor_aresta].V1 == faces_el_atual[maior_aresta].V1) ||
                     (faces_el_atual[menor_aresta].V2 == faces_el_atual[maior_aresta].V1)  )
                {
                    P1 = bloco_pts[faces_el_atual[maior_aresta].V1];
                    P2 = bloco_pts[faces_el_atual[maior_aresta].V1];
                } else if ( (faces_el_atual[menor_aresta].V1 == faces_el_atual[maior_aresta].V2) ||
                          (faces_el_atual[menor_aresta].V2 == faces_el_atual[maior_aresta].V2) )
                {
                    P2 = bloco_pts[faces_el_atual[maior_aresta].V1];
                    P1 = bloco_pts[faces_el_atual[maior_aresta].V2];
                }

                if (status)
                {
                     Informa_Erro("Erro em Refina. Execucao Interrompida", 999);
                     return status;
                }

                switch(maior_aresta)
                {
                case 0:
                    switch(menor_aresta)
                    {
                    case 1:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[2]);
                    break;
                    case 2:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[1]);
                    break;
                    }
                break;
                case 1:
                    switch(menor_aresta)
                    {
                    case 0:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[2]);
                    break;
                    case 2:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[0]);
                    break;
                    }
                break;
                case 2:
                    switch(menor_aresta)
                    {
                    case 0:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[1]);
                    break;
                    case 1:
                        fator =    comprimentos[menor_aresta]/(comprimentos[menor_aresta]+comprimentos[0]);
                    break;
                    }
                break;
                }
                Pt_Malha pCentroMaiorAresta;
                pCentroMaiorAresta.X(P1.X() + (P2.X() - P1.X()) * fator);
                pCentroMaiorAresta.Y(P1.Y() + (P2.Y() - P1.Y()) * fator);
                pCentroMaiorAresta.Set_Tipo((lev_math::Uchar)REF);
                bloco_pts.push_back(pCentroMaiorAresta);
///////////////////////////////////////////////////////////////////////////////////////////////////

                if (status)
                {
                     status = 115;
                     return status;
                }
                else {  // ponto incluido normalmente
                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // Nesta situacao, qualquer ponto incluido garantidamente nao gera nenhuma
                // degenerescencia ou volume igual a zero
                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    //if (!(bloco_pts.size()%50))
                    //{
                    //   char aux [15];
                    //    std::cout <<bloco_pts.size() <<aux <<10;
                    //}
                    contador++;
                    retorno = Altera_Malha(i, bloco_pts.size() - 1);

                    if (retorno == 20 || retorno == 30)
                    {                                  // ocorrencia de caso degenerado
                        bloco_pts.pop_back(); // elimina o componente recem gerado
                        contador--;
                    } else if (retorno == 15) {
                        bloco_pts.pop_back(); // elimina o componente recem gerado
                        Informa_Erro("Erro 15: Atingido o limite maximo de elementos do sistema. Tente aumentar a quantidade de memoria extendida disponivel!", retorno);
                    }

                    if(i >= bloco_elem_2D->Get_Total_Componentes()) // interrompe laco i (houve compactacao da lista) // *AP* teste somente
                    {
                        // este teste visa evitar um possivel erro ocasionado pela compactacao do
                        // vetor de elementos em Altera_Malha. Se a compactacao ocorrer, o indice do
                        // proximo elememento podera ser menor que o numero de eleemntos existente
                        // anteriormente na lista (elementos_total. Nestas condicoes optou-se por
                        // interromper o processo, pura e simplesmente.
                        break;
                    }
                } // fim do teste de inclusao de pontos
            }// fim do teste de tetraedro achatado
            }  // fim do teste de volume_minimo
         }   // fim do teste de validade do elemento

        }     // fim do laco i
        if (!contador)
            continua = lev_enum::NO;
    }       // fim do laco monitorado por continua
 }        // fim do teste de associacao de bloco_pontos e bloco_Elementos_2D
 return status;
} // fim do servico refina ==================================================


void Delaunay2D :: Reinicializa(void)
{
//    if (bloco_pts && bloco_elem_2D)
    if (bloco_elem_2D)
    {
//        bloco_pts->Apaga();
        bloco_pts.clear();
        bloco_elem_2D->Apaga();
    }
} // fim de reinicializa


#if 0

int Delaunay2D :: Remove_Achatado(Elemento_2D & el_achat)
{
    Elemento_2D el_viz;
    long vertices_achatado [num_vertices]; // vertices do tetraedro achatado
    long vertices_vizinho  [num_vertices]; // vertices de vizinho
    long vertices_marcados [num_vertices]; // vertices nao comuns ao achatado
    long elementos_marcados[num_vertices]; // elementos vizinhos
                                                                                 // se existe um vertice comum, a dois vizinhos
                                                                                 // o ultimo vetor nao eh igual ao de
                                                                                 // elementos_vizinho
    long *elementos_vizinhos = NULL;
    int  num_vertices_marcados;
    int  k, kk, kkk, kkkk, l;
    int  continua_k, continua_kk, continua_kkk;
    int  retorno,status;

    num_vertices_marcados = 0;
    vertices_achatado[0] = el_achat.Get_V1();
    vertices_achatado[1] = el_achat.Get_V2();
    vertices_achatado[2] = el_achat.Get_V3();
    vertices_achatado[3] = el_achat.Get_V4();
    elementos_vizinhos   = el_achat.Get_Vizinhos();

    for (k = 0, continua_k = YES; ; k++)
//    l = 0;k = 0; continua_k = YES;
//    while(l <el_achat.Get_N_Vizinhos() && continua_k)
    {
//        if (elementos_vizinhos[k]>=0)
//        {
//            l++;

        status = bloco_elem_2D->Get_Componente(elementos_vizinhos[k],el_viz);
        if (status)
        {
            Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status);
        }
        vertices_vizinho[0] = el_viz.Get_V1();
        vertices_vizinho[1] = el_viz.Get_V2();
        vertices_vizinho[2] = el_viz.Get_V3();
        vertices_vizinho[3] = el_viz.Get_V4();
        // laco kk : varredura dos vertices dos elementos vizinhos ao achatado
        for (kk = 0, continua_kk = YES; kk < num_vertices/*global*/ && continua_kk; kk++)
        {

            // laco kkk : varredura dos vertices do elemento achatado
            for (kkk = 0, continua_kkk = YES; kkk < num_vertices /*global*/ && continua_kkk; kkk++)
                if (vertices_achatado[kkk] == vertices_vizinho[kk])
                    continua_kkk = NO;

            if (continua_kkk) // vertice do vizinho nao e comum ao achatado
            {
                if (num_vertices_marcados > 0)
                {
                    for(kkkk = 0; kkkk < num_vertices_marcados; kkkk++)
                    {
                        if (vertices_vizinho[kk] == vertices_marcados[kkkk])
                        {
                            vertices_marcados [0] = vertices_vizinho  [kk];
                            elementos_marcados[1] = elementos_marcados[kkkk];
                            elementos_marcados[0] = elementos_vizinhos[k];
                            num_vertices_marcados = 1;
                            continua_k = continua_kk = NO;
                            break;
                        } // fim do teste de vertice presente na lista de marcados
                    }   // fim do laco kkkk
                    if (continua_k) // significa que nao foi encontrado um vertice comum
                    {
                        vertices_marcados[num_vertices_marcados]    = vertices_vizinho[kk];
                        elementos_marcados[num_vertices_marcados++] = elementos_vizinhos[k];
                        continua_kk = NO;
                    }
                }
                else
                {
                    vertices_marcados [num_vertices_marcados]   = vertices_vizinho[kk];
                    elementos_marcados[num_vertices_marcados++] = elementos_vizinhos[k];
                    continua_kk = NO;
                }     // fim do teste de numero de vertices marcados

            }       // fim do teste de coincidencia de vertices com el. achatado
        }         // fim do laco kk
    //    }
    //    k++;
    }           // fim do laco k
    if (num_vertices_marcados == 1) // significa que foi encontrado um vertice comum
    {
        // constroi arestas dos tetraedros vizinhos que nao contem o ponto marcado como vertice
        Elemento_2D  el_vizA;
        struct {long a1, a2;} arestas1[3], arestas2[3];
        long vertices_vizinhoA[num_vertices];
        long vertice_troca    [2];
        long vertice_resto    [2];
        int i,j,k, identificado = NO;

        vertice_troca[0]=vertice_troca[1]=vertice_resto[0]=vertice_resto[1]=0;

        status = bloco_elem_2D->Get_Componente(elementos_marcados[0],el_viz);
        if (status)
        {
            Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status);
        }
        vertices_vizinho[0] = el_viz.Get_V1();
        vertices_vizinho[1] = el_viz.Get_V2();
        vertices_vizinho[2] = el_viz.Get_V3();
        vertices_vizinho[3] = el_viz.Get_V4();

        status = bloco_elem_2D->Get_Componente(elementos_marcados[1],el_vizA);
        if (status)
        {
            Informa_Erro("Erro ?? : Vizinho nao localizado em Remove_Achatado. Execucao Interrompida !", status);
        }
        vertices_vizinhoA[0] = el_vizA.Get_V1();
        vertices_vizinhoA[1] = el_vizA.Get_V2();
        vertices_vizinhoA[2] = el_vizA.Get_V3();
        vertices_vizinhoA[3] = el_vizA.Get_V4();

// __________________________________________________________________________
        FILE *arq1;
        arq1 = fopen("remach.dat", "a+");
        fprintf(arq1, "\n vertices do tetraedro achatado :");
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_achatado[0],
                         vertices_achatado[1], vertices_achatado[2], vertices_achatado[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho1 antes da modificacao:");
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho2 antes da modificacao:");
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
// __________________________________________________________________________

        // monta as arestas que nao contem o vertice comum dos tetraedros vizinhos
        for (i = 0, k = 0; i < num_vertices-1/*global*/; i++)
            for (j = i+1; j <num_vertices-1/*global*/; j++)
                if   (vertices_vizinho[i] != vertices_marcados[0])
                    if (vertices_vizinho[j] != vertices_marcados[0])
                    {
                        arestas1[k]  .a1 = vertices_vizinho[i];
                        arestas1[k++].a2 = vertices_vizinho[j];
                    }

        for (i = 0, k = 0; i < num_vertices-1/*global*/; i++)
            for (j = i+1; j <num_vertices-1/*global*/; j++)
                if   (vertices_vizinhoA[i] != vertices_marcados[0])
                    if (vertices_vizinhoA[j] != vertices_marcados[0])
                    {
                        arestas2[k]  .a1 = vertices_vizinhoA[i];
                        arestas2[k++].a2 = vertices_vizinhoA[j];
                    }

        // verifica aresta comum e associa os vertices da aresta comum vom vertice_troca
        // e os outros dois vertices do elemento achatado a vertice_resto
        int continua_i, continua_j;
        for   (i = 0, continua_i = YES; i < n_vertices && continua_i; i++)
            for (j = 0, continua_j = YES; j < n_vertices && continua_j; j++)
            {
                if (arestas1[i].a1 == arestas2[j].a1 && arestas1[i].a2 == arestas2[j].a2)
                    identificado = YES;
                else if (arestas1[i].a1 == arestas2[j].a2 && arestas1[i].a2 == arestas2[j].a1)
                    identificado = YES;
                if (identificado)
                {
                    vertice_troca[0] = arestas1[i].a1;
                    vertice_troca[1] = arestas1[i].a2;
                    for (k = 0, kk = 0; k < num_vertices; k++)
                    {
                        if (vertice_troca[0] != vertices_achatado[k] && vertice_troca[1] != vertices_achatado[k])
                            vertice_resto[kk++] = vertices_achatado[k];
                    }  // fim do laco for(k)
                    continua_i = continua_j = NO;
                }    // fim do teste(identificado)

            }    // fim do laco for(j)

        // PROVIDENCIA A TROCA DE 1 VERTICE PARA O PRIMEIRO VIZINHO
        for (i = 0, continua_i = YES; i < num_vertices && continua_i; i++)
        {
            if (vertices_vizinho[i] == vertice_resto[0])
            {
                for (j = 0, continua_j = YES; j < num_vertices && continua_j; j++)
                {
                    if (vertices_vizinho[j] == vertice_troca[0])
                    {
                        vertices_vizinho[j] = vertice_resto[1];
                        continua_j = NO;
                    }
                }
                continua_i = NO;
            }
            else if (vertices_vizinho[i] == vertice_resto[1])
            {
                for (j = 0, continua_j = YES; j < num_vertices && continua_j; j++)
                {
                    if (vertices_vizinho[j] == vertice_troca[0])
                    {
                        vertices_vizinho[j] = vertice_resto[0];
                        continua_j = NO;
                    }
                }
                continua_i = NO;
            }
        }   // fim do laco for (i)

        // PROVIDENCIA A TROCA DE 1 VERTICE PARA O SEGUNDO VIZINHO
        for (i = 0, continua_i = YES; i < num_vertices && continua_i; i++)
        {
            if (vertices_vizinhoA[i] == vertice_resto[0])
            {
                for (j = 0, continua_j = YES; j < num_vertices && continua_j; j++)
                {
                    if (vertices_vizinhoA[j] == vertice_troca[1])
                    {
                        vertices_vizinhoA[j] = vertice_resto[1];
                        continua_j = NO;
                    }
                }
                continua_i = NO;
            }
            else if (vertices_vizinhoA[i] == vertice_resto[1])
            {
                for (j = 0, continua_j = YES; j < num_vertices && continua_j; j++)
                {
                    if (vertices_vizinhoA[j] == vertice_troca[1])
                    {
                        vertices_vizinhoA[j] = vertice_resto[0];
                        continua_j = NO;
                    }
                }
                continua_i = NO;
            }
        }   // fim do laco for (i)


        el_viz.Set_Vertices(&vertices_vizinho[0]);
        el_viz.Calc_Circuns(bloco_pts);
        bloco_elem_2D->Modifica(elementos_marcados[0],&el_viz);

        el_vizA.Set_Vertices(&vertices_vizinhoA[0]);
        el_vizA.Calc_Circuns(bloco_pts);
        bloco_elem_2D->Modifica(elementos_marcados[1],&el_vizA);

// __________________________________________________________________________
        fprintf(arq1, "\n vertices do tetraedro vizinho1 apos a modificacao:");
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinho[0],
                         vertices_vizinho[1], vertices_vizinho[2], vertices_vizinho[3]);
        fprintf(arq1, "\n vertices do tetraedro vizinho2 apos a modificacao:");
        fprintf(arq1, "\n       %ld  %ld  %ld  %ld", vertices_vizinhoA[0],
                         vertices_vizinhoA[1], vertices_vizinhoA[2], vertices_vizinhoA[3]);
        fclose(arq1);
// __________________________________________________________________________

        retorno = 1;
    }
    else
    {

        retorno = 0;
    }
    return retorno;

}             // fim do servico de remocao de achatados

#endif

void Delaunay2D :: Restitui_Degenerados(void)
{
    lev_math::USINT i,status;
    long  ii;
    Pt_Malha      pt_atual;
    Elemento_2D el_atual;
//    Vid1.Set_Hue(cor_anterior);


    if (controle_degenerados) {

//        char aux1[10];
        char aux [40]  /*= "numero de casos degenerados : "*/;
//        strcat(aux,itoa(controle_degenerados,aux1,10));

//        IndMov->Mensagem(aux);
//        jNumero.Window_Open();
//        jNumero.Put_Text_Gra(5, 3,AZUL,ltoa(bloco_pts->Get_Total_Componentes(),aux,10));
//        std::cout <<bloco_pts->Get_Total_Componentes() <<aux <<10;
        //std::cout <<bloco_pts.size() <<aux <<10;
//        jNumero.Window_Close();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  *AP* para restituir os deslocamentos efetuados
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


        for ( i = controle_degenerados;i > 0; i--) {

//            status = bloco_pts->Get_Componente(deslocamentos[i-1], pt_atual);
            pt_atual = bloco_pts[deslocamentos[i-1]];
            if (status) {
             status =  108;
             Informa_Erro("Erro 108: Dados Inconsistentes. Execucao interrompida...",status);
            }

//          pt_atual.Set_Coord( coordx[i-1],coordy[i-1], pt_atual.Z());

            pt_atual.Set_Coord( coordx[i-1],coordy[i-1]);

//            bloco_pts->Modifica(deslocamentos[i-1], &pt_atual);
            bloco_pts[deslocamentos[i-1]] = pt_atual;

        }

        controle_degenerados = 0;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Recalcula o volume de todos os elementos da lista (incluindo XMS)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        double vol_teste;
        for (ii = 0; ii < bloco_elem_2D->Get_Total_Componentes(); ii++) {

            status = bloco_elem_2D->Get_Componente(ii,el_atual);
            if (status) {
             status =  109;
             Informa_Erro("Erro 109: Dados Inconsistentes. Execucao interrompida...",status);
             exit (status);
            }

            if (el_atual.Get_Validade()) {
                el_atual.Calc_Circuns(bloco_pts);
                vol_teste = el_atual.Get_Volume();
                if (vol_teste<4e-11 && vol_teste>-4e-11)  {  // *AP* 15/02/95 teste de Rat de alin2

/*                 fprintf (avol0,"\n\n elemento: %ld  vertices : %ld %ld %ld %ld      %lg",ii,el_atual.Get_V1()-7+1,el_atual.Get_V2()-7+1,el_atual.Get_V3()-7+1,el_atual.Get_V4()-7+1, vol_teste);//*AP*
                 Pt_Malha pppt;
                 bloco_pts->Get_Componente(el_atual.Get_V1(), pppt);
                 fprintf(avol0,"\n    %.7g   %.7g    %.7g",pppt.X(),pppt.Y(),pppt.Z());
                 bloco_pts->Get_Componente(el_atual.Get_V2(), pppt);
                 fprintf(avol0,"\n    %.7g   %.7g    %.7g",pppt.X(),pppt.Y(),pppt.Z());
                 bloco_pts->Get_Componente(el_atual.Get_V3(), pppt);
                 fprintf(avol0,"\n    %.7g   %.7g    %.7g",pppt.X(),pppt.Y(),pppt.Z());
                 bloco_pts->Get_Componente(el_atual.Get_V4(), pppt);
                 fprintf(avol0,"\n    %.7g   %.7g    %.7g",pppt.X(),pppt.Y(),pppt.Z());
*/
                 bloco_elem_2D->Elimina(ii); // os elementos vizinhos ja estao com as faces corretamente definidas
                }
                else
                {
                    bloco_elem_2D->Modifica(ii,&el_atual);
                }
            }

        }

    } // FIM DO TESTE DE CONTROLE_DEGENERADOS
} // fim do servico de Restituicao de degenerados


#if 0  // substituida pelo servido de mesmo nome em Delaunay (14/01/97)
//===========================================================================
//                  Delaunay2D :: SET_LIMITE_PONTOS
// ==========================================================================
// Descricao    :  Define para objeto  o numero maximo de pontos que deve ser
//                 utilizado para a geracao da malha de elementos finitos
//                 Se a sugestao fornecida nao estiver dentro dos limites
//                 permitidos, o limite original nao eh alterado.
//
// Parametros   :  limite : limite de pontos;
//
// retorno      :  limite : eco do valor sugerido para indicar que o limite
//                          foi aceito e alterado
//                 limite_inferior : caso a sugestao fornecida seja menor que
//                                   o numero minimo de pontos permitidos
//                 limite_superior : caso a sugestao fornecida seja maior que
//                                   o numero maximo de pontos permitidos
//                 0               : se pelo menos uma das lista (pontos ou
//                                   elementos) nao tiver sido definida atraves
//                                   de Associa.
//
// programador  : Angelo Passaro
// alterado em  : 19/11/94
//===========================================================================


long Delaunay2D :: Set_Limite_Pontos(long limite)
{
    long retorno;
    if (bloco_pts && bloco_elem_2D)
    {
        if      (limite > bloco_pts->Get_N_Max_Com_Abs())
            retorno =  bloco_pts->Get_N_Max_Com_Abs();
        else if (limite < bloco_pts->Get_Total_Componentes())
            retorno = bloco_pts->Get_Total_Componentes();
        else
        {
                bloco_pts->Set_Limite_Superior(limite);
                retorno = limite;
        }

//        if (limite < bloco_pts->Get_N_Max_Com_Abs())
//        {
//                bloco_pts->Set_Limite_Superior(limite);
//                retorno = limite;
//        }
//        else  retorno = bloco_pts->Get_N_Max_Com_Abs();
    }
    else retorno = 0;
    return retorno;

}

#endif  // delimita a funcaoo SET_LIMite_PONTOS



//===========================================================================
//               class Delaunay2D :: Restaura_Geometria
//===========================================================================
//
//  Descricao          : Verifica se, apos o processo de geracao da malha de
//                       elementos finitos, todos os elementos 1d sao aresta de
//                       elementos 3D. Se negativo, sao gerados pontos no meio
//                       do segmento de reta dos elementos quebrados e realizado
//                       um processo de inclus�o desses pontos na malha. O algoritmo
//                       para essa inclus�o e o mesmo utilizado na Discretiza.
//
//  retorno            : 0   execucao normal
//                       1   verificacao nao executada ou parcialmente executada
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 13/07/95
//
//===========================================================================

lev_math::USINT Delaunay2D :: Restaura_Geometria(void)
{
    Elemento_2D el2D;                // conexao de ocorrencia
    Aresta_Geometria el1D;          // conexao de ocorrencia
    Pt_Malha ptnovo;                // conexao de ocorrencia
    long   num_1D_total = 0,        // armazena o total de elementos 1D
             num_2D_total = 0;        // armazena o total de elementos 2D
    long   num_el_1D_zero;          // numero de elementos 1D para os quais ainda nao foi associada
                                            // uma aresta de elementos 3D
    long   vertice_inicial,         // vertice inicial original do elemento 1D que foi quebrado
             vertice_final  ,       // vertice final original do elemento 1D que foi quebrado
             vertice_novo   ;       // novo vertice, colocado no centro do segmento que representa
                                            // o elemento 1D original
    long   E1, E2,                  // vertices do elemento 1D
             e[4]  ;                // vertices do elemento 3D
    int   prim;                     // primitiva
    long  *num_elementos;           // ponteiro para vetor que indica se existem (>0) ou
                                    // nao (==0) elementos 3D cuja aresta corresponde ao
                                    // elemento 1d
    long   i,j,k;                   // contadores
    lev_math::USINT  retorno;
    int    v1_pertence, v2_pertence; // flag  para indicar se os vertices 1 e 2 correspondem
                                                // a vertices do Elemento 3D

    ////////////////////////////////////  STL //////////////////////////////////////////////////
    //    num_1D_total = bloco_geometria->Get_N_Max_Com_Abs();
    num_1D_total = MAXGEO;//bloco_geometria.size()*2; // *NMA*: � necess�rio verificar outra n�mero para dimensionamento
    ////////////////////////////////////////////////////////////////////////////////////////////
    num_elementos = new long [num_1D_total];
    if(!num_elementos) {
        Informa_Erro("ERRO : Memoria insuficiente na alocacao do vetor de numero de elementos. Na sera possivel verificar geometria!",1);
        return (1);
    }

    do {
        memset(num_elementos, 0, MAXGEO); // *NMA*: para consist�ncia com dimensionamento de num_elemento acima

        num_2D_total = bloco_elem_2D->Get_Total_Componentes();

        num_1D_total = bloco_geometria.size();

        for(i=0, num_el_1D_zero = num_1D_total; i<num_2D_total && num_el_1D_zero; i++) {

            bloco_elem_2D->Get_Componente(i, el2D);
            if(el2D.Get_Validade()) {
                e[0]=el2D.Get_V1(); e[1]=el2D.Get_V2();
                e[2]=el2D.Get_V3();
                for(j=0; j<num_1D_total && num_el_1D_zero; j++) {
                    //
                    // este teste pretende agilizar o processo
                    //
                    if (!num_elementos[j]) {
                        v1_pertence = v2_pertence = lev_enum::NO;

                        el1D = bloco_geometria[j];

                        E1 = el1D.g1; E2 = el1D.g2; prim = el1D.prim;
                        for(k=0; k<n_vertices; k++)
                        {
                            if(E1 == e[k])
                                v1_pertence = lev_enum::YES;
                            else if (E2 == e[k])
                                v2_pertence = lev_enum::YES;
                            if (v1_pertence && v2_pertence) {
                                num_elementos[j]++;
                                num_el_1D_zero--;
                                break;
                            }
                        }
                    }
                }
            }
        }

        //
        // proximo passo : se existem elementos 1d que nao sao vertices de elementos 3D
        //                 (indicado por num_el_1D_zero > 0) sao incluidos pontos na
        //                 lista de pontos que correspondem ao ponto central de cada
        //                 segmento que representa o elemento 1D.
        //
        //#define CHECA_RESTAURACAO
        //#ifdef CHECA_RESTAURACAO
        std::cout << "\n num_el_1D_zero = " << num_el_1D_zero;
        std::cout << "\n iniciando verificacao e introducao de pontos";
        //#endif
        if(num_el_1D_zero) {
            for(j=0; j<num_1D_total; j++) {
                if(!num_elementos[j]) {
                    el1D = bloco_geometria[j];

                    el1D.Get_Central_Point(bloco_pts,ptnovo);

                    bloco_pts.push_back(ptnovo);

                    //*GMM* trecho ideal para monitorar a inclus�o de pontos na malha
                    // Aqui ser� feita a grava��o em arquivo de cada ponto incluido na malha
                    // APENAS PARA DEBUG
                    {
                        //
                        // bloco de divisao do elemento 1D
                        //
                        vertice_final   = el1D.g2;
                        vertice_inicial = el1D.g1;
                        vertice_novo    = bloco_pts.size()-1;
                        el1D.g2=vertice_novo;

                        bloco_geometria.push_back(el1D);

                        el1D.g1=vertice_novo;
                        el1D.g2=vertice_final;

                        bloco_geometria[j].g1 = el1D.g1;
                        bloco_geometria[j].g2 = el1D.g2;
                        bloco_geometria[j].prim = el1D.prim;
                        bloco_geometria[j].primType = el1D.primType;
                    }
                    {
                        //
                        // bloco de inclusao do ponto na malha de Delaunay
                        //
                        retorno = Inclui_1_Ponto(vertice_novo);// aqui inclui o novo ponto na malha e aplica Delaunay
                        if (retorno == 108) {
                            Informa_Erro("Erro 108: Dados Inconsistentes. Execucao interrompida...",retorno);
                        } else if (retorno == 109) {
                            Informa_Erro("Erro 109: Dados Inconsistentes. Execucao interrompida...",retorno);
                        } else if (retorno == 120) {
                            Erro(this, bloco_pts, bloco_elem_2D,bloco_pts.size()-1);     // *AP*
                            Informa_Erro("Erro 120 (Delaunay2D :: Restaura_Geometria): Base de dados corrompida. Execucao interrompida...",retorno);
                        } else if (retorno == 14) {
                            Informa_Erro("Erro 14: Memoria extendida inexistente. Execucao interrompida...", retorno);
                        } else if (retorno == 15) {
                            Informa_Erro("Erro 15: Atingido o limite maximo de elementos do sistema. Tente aumentar a quantidade de memoria extendida disponivel!", retorno);
                        }
                    }
                } // fim de if (!num_elementos[j])
            } // fim do laco for (j = 0; j < num_1D_total; j++)
        }      // fim de if (num_el_1D_zero)  certificar
    } while(num_el_1D_zero); // talvez possa ser eliminada busca pelo laco (for (i)) se, de fato ficar constatado que nao sa afetados os tetraedros associados aos demais elementos 1D
    if(num_elementos)
        delete num_elementos;
    bloco_elem_2D->Compacta();
    return retorno;
}


void Erro(Delaunay2D *dg, vCLevPtMalha & lp,Lista_Elementos_2D_D *le, long numero)
{


    Pt_Malha      pt;
    Elemento_2D el;
    FILE  *ERRO;
    double raio;
    double xc,yc,zc;
    long i;
    ERRO = fopen("erro120.err","w");


    fprintf(ERRO, "\nParalelepipedo envolvente");
    for (i = 0 ; i < 8; i++)
    {
//        lp->Get_Componente(i,pt);
        pt = lp[i];
        fprintf(ERRO,"\n coordenadas ponto %3ld : %15.7g, %15.7g, %15.7g",
                i, pt.X(), pt.Y(), pt.Z());
    }


    fprintf(ERRO, "\nnumero do ponto no programa: %ld", numero);
    fprintf(ERRO, "\nnumero do ponto na lista (comecando de 1)  : %ld", numero-7);
//    lp->Get_Componente(numero,pt);
    pt = lp[numero];
    fprintf(ERRO, "\ncoordenadas do ponto(x,y,z) (%15.7g,%15.7g,%15.7g)", pt.X(),pt.Y(), pt.Z());

    fprintf (ERRO,"\n ********* Lista de Elementos LEV_VALIDs ***************");
    fprintf (ERRO,"\nnumero  vert1  vert2  vert3  vert4       volume  ");
    fprintf (ERRO,"    xc                 yc               zc                 raio ");
    for (i = 0; i < le->Get_Total_Componentes(); i++)
    {
        le->Get_Componente(i,el);
        if (el.Get_Validade())
        {
            fprintf(ERRO, "\n %7ld %6ld %6ld %6ld %6ld %15.7g", i,el.Get_V1(), el.Get_V2(), el.Get_V3(), el.Get_Volume());
            xc = -el.Get_C0()*.5; yc = -el.Get_C1() *.5; zc = -el.Get_C2()*.5;
            raio = .25*(el.Get_C0()*el.Get_C0() + el.Get_C1()*el.Get_C1()) - el.Get_C2();
            fprintf(ERRO, " %15.7g %15.7g %15.7g %15.7g",xc, yc, zc, sqrt(raio));

// verifica se as coordenadas do ponto estao no interior de alguma das circunsferas apresentadas
            if ( pt.X() < (xc+raio) && pt.X() > (xc -raio))
            {
                if (pt.Y() <(yc+raio) && pt.Y() > (yc -raio))
                {
                    if (pt.Z() <(zc+raio) && pt.Z() > (zc - raio))
                    {
                        fprintf (ERRO, "\n <---------------- possivel circunsfera .Distancia ao centro :%15.7g",
                                sqrt((pt.X() - xc)*(pt.X() - xc) +
                                         (pt.Y() - yc)*(pt.Y() - yc) +
                                         (pt.Z() - zc)*(pt.Z() - zc)));
                    }
                }
            }
        }
    }

    fclose (ERRO);

//    dg->grava_elementos("erro120.gde",30,50,30,50);


}



lev_math::USINT Delaunay2D :: Inclui_1_Ponto(long ii)
{
    Pt_Malha        pt_atual;
    Elemento_2D        el_atual;
    lev_math::USINT status = 0;
    lev_math::Uchar continua;
    long            num_elemento; // indice do elemento de busca
    lev_math::USINT retorno=0; // valor de retorno de altera malha
    double            interior, raio2,dquadrado ;  // x2+y2+z2
    double delta=0.0;

    pt_atual = bloco_pts[ii];

    if (status) {
        status =  108;
        return(status);
    }

    do {
        dquadrado = pt_atual.X() * pt_atual.X() + pt_atual.Y() * pt_atual.Y();

        num_elemento = bloco_elem_2D->Get_Total_Componentes() - 1;
        continua = lev_enum::YES;
        //do {
            while (continua && (num_elemento >= 0)) {
                status = bloco_elem_2D->Get_Componente(num_elemento, el_atual);
                if (status) {
                    status =  109;
                    return (status);
                }

                if(el_atual.Get_Validade()) {
                    interior = dquadrado + el_atual.Get_C0() * pt_atual.X() + el_atual.Get_C1() * pt_atual.Y() + el_atual.Get_C2();
                    raio2 = (el_atual.Get_C0()*el_atual.Get_C0() + el_atual.Get_C1()*el_atual.Get_C1() ) * .25 - el_atual.Get_C2();
                    if(interior < (SIGMA+delta) * raio2)
                        continua = lev_enum::NO;
                    else num_elemento--;
                } else
                    num_elemento--;
            }
            //if(continua == lev_enum::YES) {
            //    delta += 1e-10;
            //    num_elemento = bloco_elem_2D->Get_Total_Componentes() - 1;
            //}
        //} while(continua == lev_enum::YES);

        if (!continua) {
            retorno = Altera_Malha(num_elemento, ii);
            // se caso degenerado
            if (retorno == 30 || retorno == 20) {

                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //   Toma o elemento que gerou a degenerescencia para o calculo  do
                //   deslocamento controlado
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                coordx[controle_degenerados] = pt_atual.X(); //*AP* para recuperar coordenada ao final da discretizacao
                coordy[controle_degenerados] = pt_atual.Y(); //*AP* para recuperar coordenada ao final da discretizacao

                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //  TESTE PARA DESLOCAMENTO CONTROLADO CALCULADO A PARTIR DE SIGMA*RAIO2
                //  *AP*   26/09/94
                // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                double SIG = -1e-7;
                deslcontrol = sqrt(-SIG*raio2*.5);
                pt_atual.Set_Coord(pt_atual.X()+deslcontrol, pt_atual.Y()+deslcontrol);
                bloco_pts[ii] = pt_atual;

                deslocamentos[controle_degenerados] = ii;    // para garantir a repeticao do mesmo indice no laco

                controle_degenerados++;

                #if 10
                if (controle_degenerados > (MAXDESLOCAMENTOS-20))
                    Informa_Erro("numero de degenerados > 4980",0);
                #endif
            }
        } else {
            retorno = 120;
        }
    } while (retorno == 20 || retorno == 30 || retorno == 31);   // enquanto retorno de altera malha eh 20 ou 30
    return retorno;
}




void Delaunay2D :: DBG_Salva_Lista(long npt, long *Vizinhos_Distantes, long num_vizinhos_distantes)
{
    long int i;  // usado somente como contador
    long int *Viz = NULL;   // recebe a copia da lista de vizinhos de cada elemento
    double raio2,rinsc;
    Elemento_2D elem;
    FILE *arq;
    arq = fopen("Erro306.dbg","w+");
    if (!arq) { std::cout << "\n arquivo de debug nao pode ser aberto...."; }
    else
    {
        fprintf (arq, "\n ARQUIVO DE DEBUG COM A LISTA DE ELEMENTOS 3D GERADOS");
        fprintf (arq, "\n normalmente este arquivo e gerado quando da ocorrencia do erro 306");

        fprintf (arq, "\n\n numero do ponto para o qual foi identificada a inconsistencia : %ld", npt);
        fprintf(arq, "\n\n     elem      V1       V2       V3       V4            volume    num_vizinhos     viz1     viz2     viz3     viz4      Rext      (Rint/Rext)^2");
        for (i = 0; i < bloco_elem_2D->Get_Total_Componentes(); i++)
        {
            bloco_elem_2D->Get_Componente(i,elem);
            if (elem.Get_Validade())
            {
                raio2 = (elem.Get_C0()*elem.Get_C0() +
                                    elem.Get_C1()*elem.Get_C1()
                                 ) * .25 - elem.Get_C2();
                rinsc = elem.Calc_R_Insc(bloco_pts);

                Viz = elem.Get_Vizinhos();
                fprintf(arq,"\n %9ld%9ld%9ld%9ld%9ld%15lg%14d%9ld%9ld%9ld%9ld%15.8lg%15.8lg",
                            i, elem.Get_V1(),elem.Get_V2(),elem.Get_V3(),
                            elem.Get_Volume(),elem.Get_N_Vizinhos(),
                            Viz[0],Viz[1],Viz[2],sqrt(raio2), rinsc*rinsc/raio2);
            }
        }
        fprintf (arq, "\n\n elementos distantes ( deverao ter sua vizinhanca alterada!)\n");
        for (i = 0; i < num_vizinhos_distantes; i++)    fprintf(arq, "\n %10ld", Vizinhos_Distantes[i]);
        fclose(arq);

    }
}

void Delaunay2D :: Informa_Erro( char * mens, long retorno)
{
//    #if defined INTERFACE_GRAFICA
//      Msg.Display (mens,BRANCO,AZUL);
//    #else
    std::cout << "\n\a "<< mens;
    std::cout.flush();

    throw mens;
//    #endif
//    exit (retorno);
}

//-----------------------------------------------------------------------
//                 clsass Delaunay2D :: Lista_Elementos_Contiguos
//-----------------------------------------------------------------------
//
//  Descricao          : Funcao nao recursiva para armazenar em uma lista, um 
//                         conjunto de elementos que sejam contiguos e estejam
//                         contidos dentro de um mesmo compartimento de geometria.
//                         Uma geometria fechada � um conjunto de elementos
//                         contiguos limitados pelas arestas de geometria.
//                         Toda geometria tem pelo menos um conjunto de elementos (nC)
//                         limitados por arestas de gemetria, podendo ter mais,
//                          (nC >= 1)
//
//                         A Lista de elementos onde sera feita a procura �
//                         bloco_elem_2D.
//
//
//    Parametros           : * list_cont - lista de elementos contiguos armazenados
//                         * list_index- lista contendo o �ndice dos elementos
//                         na lista original
//
//                         primeiro - indice do primeiro componente da lista, a partir
//                         do primeiro elemento ser� realizada uma busca em profundidade,
//                         vizinho a vizinho, capturando todos elementos contiguos
//                         sem ultrapassar arestas de geometria durante o processo.
//
//                         exclui - define a exlcus�o dos itens listados
//                         1 - exclui elementos listados
//
//  versao             : 2.4
//  programador        : Guilherme Moreira Magalh�es
//  ultima modificacao : 14/02/2001
//
//-----------------------------------------------------------------------

void Delaunay2D::Lista_Elementos_Contiguos
(Lista_Elementos_2D_D * list_cont,long * list_index,long primeiro, const int exclui)
{
    long total_elementos = bloco_elem_2D->Get_Total_Componentes();
    long * p_aberto  = NULL;
    char * p_fechado = NULL;//long
    p_aberto  = new long[total_elementos + 1];
    p_fechado = new char[total_elementos + 1];

    if (!(p_aberto && p_fechado))
    {
        if ( p_aberto) delete [] p_aberto;
        if (p_fechado) delete [] p_fechado;
        exit(0);
    }


    long percorrido = 0, aux_percorrido = 0, indice_aberto = 0, indice_fechado = 0, ii;

    long  * viz  = NULL, n_vizinhos, aux;
    lev_math::USINT status;
    Elemento_2D el, ell, elemento;
    int i,j, k, mark = 0;
    char mark2 = 0;
    FACE  faces_elemento[3], faces_el[3];
    Aresta_Geometria geometria;

    p_aberto[++indice_aberto]= primeiro;

    while (indice_aberto != 0)
    {
        status = bloco_elem_2D->Get_Componente(p_aberto[indice_aberto] ,elemento);
        if (status)
        {
            status =  512;
            std::cout << "\a\a\n\n\nErro "<< status <<": Dados Inconsistentes. Execucao interrompida...";
            Informa_Erro("Erro 512: Dados Inconsistentes. Execucao interrompida...",status);
            delete[] p_aberto;    delete[] p_fechado;
            exit(1);
        }
        n_vizinhos = elemento.Get_N_Vizinhos();
        viz = elemento.Get_Vizinhos();
        aux_percorrido = p_aberto[indice_aberto--];
        list_index[indice_fechado++]= aux_percorrido;
        p_fechado[aux_percorrido]=1;
        if (exclui == 1)
            bloco_elem_2D->Elimina(aux_percorrido);

        for (i=0; i < n_vizinhos; i++)
        //i=0; ll=0;  *GMM*  modifica��o no la�o para trabalhar com vizinhos ordenados
        //while (ll - <n_vizinhos)
        {
            //if (viz[i] >= 0){
            //ll++;

            status = bloco_elem_2D->Get_Componente(viz[i],el);
            if (status)
            {
                status =  512;
                std::cout << "\a\a\n\n\nErro "<< status <<": Dados Inconsistentes. Execucao interrompida...";
                Informa_Erro("Erro 512: Dados Inconsistentes. Execucao interrompida...",status);
                delete[] p_aberto;    delete[] p_fechado;
                exit(1);
            }
            if (viz[i] != percorrido)
            {
                elemento.Monta_Faces(faces_elemento);//monta as faces de elemento e
                el.Monta_Faces(faces_el);// as faces de el
                for (j=0; j<n_vertices; j++)
                    for(k=0; k<n_vertices; k++)
                    {//procura a face compartilhada
                        if((faces_elemento[j].V1 == faces_el[k].V1)||(faces_elemento[j].V1 == faces_el[k].V2))
                            if((faces_elemento[j].V2 == faces_el[k].V2)||(faces_elemento[j].V2 == faces_el[k].V1))
                                aux = k;
                    }
                //procura a face compartilhada na lista de geometrias
                mark=0;

/////////////////////////////////// STL //////////////////////////////////////////////
//                for (j=0; j<bloco_geometria->Get_Total_Componentes(); j++)
//                {
//                    status = bloco_geometria->Get_Componente(j,geometria);
//                    if ((faces_el[aux].V1 == geometria.g1) && (faces_el[aux].V2 == geometria.g2) ||
//                        (faces_el[aux].V1 == geometria.g2) && (faces_el[aux].V2 == geometria.g1))
//                    {
//                        mark=1;
//                        break;
//                    }
//                }

                for (j=0; j<bloco_geometria.size(); j++)
                {
                    geometria = bloco_geometria[j];
                    if ((faces_el[aux].V1 == geometria.g1) && (faces_el[aux].V2 == geometria.g2) ||
                        (faces_el[aux].V1 == geometria.g2) && (faces_el[aux].V2 == geometria.g1))
                    {
                        mark=1;
                        break;
                    }
                }
////////////////////////////////////////////////////////////////////////////////////

                //se a face nao for encontrada na lista de geometrias
                if (mark == 0)
                {
//                    list_cont->Inclui(el);
//                    Lista_Elementos_Contiguos(list_cont, ind_out, viz[i], exclui);
                                                        //ind_in, ind_out

                    //devo checar se o elemento ja nao consta em p_aberto
                    //ou p_fechado, caso nao conste a inclus�o pode ser realizada
                    mark2=0;
                    for(ii=1; ii<=indice_aberto; ii++)
                        if (viz[i]==p_aberto[ii])
                            mark2=1;
                    if (p_fechado[viz[i]] == 1)
                        mark2=1;

                    if(mark2 == 0)
                    {
                        p_aberto[++indice_aberto] = viz[i];
                        list_cont->Inclui(el);
                        //adiciona o indice do elemento no vetor aberto
                    }
                }
            }//fim if (viz[i] != ind_in)
            //i++;}
        }//fim la�o
        percorrido = aux_percorrido;
        viz  = NULL;
    }// fim while
    delete[] p_aberto;    delete[] p_fechado;
}
//-----------------------------------------------------------------------

//lev_math::USINT Delaunay2D::Refina(MESHTYPE metodo)
lev_math::USINT Delaunay2D::Refina(int metodo)
{
lev_math::USINT  status = 0, retorno;
    if (metodo == MED_VIZ) status = Refina_Media();
    else
    {
//        if (bloco_pts && bloco_elem_2D)
        if (bloco_pts.size() && bloco_elem_2D)
        {
        long mark = 1, teste = 0;
        long elementos_total; //n�mero total, atual, da lista de elementos 2D
        long i, j; //contador dos lacos de varredura de elementos
        double peso, comprimento, peso1, peso2, peso3;
        double x1, x2, x3, y1, y2, y3;
        double x, y;
        Pt_Malha p1, p2, p3;
        lev_enum::LEV_BOOL continua = lev_enum::YES;
        Elemento_2D el_atual;
        Aresta_Geometria geometria;


        //FACE faces_el_atual[3];//armazena faces de um simplexo

        //calcula o peso dos pontos da geometria
///////////////////////////    STL ////////////////////////////////////////
//        for (j=0; j<bloco_geometria->Get_Total_Componentes(); j++)
//        {
//            bloco_geometria->Get_Componente(j,geometria);
//            comprimento = geometria.Calc_Comprimento(bloco_pts);
//            bloco_pts->Get_Componente(geometria.g1, p1);
//            bloco_pts->Get_Componente(geometria.g2, p2);
//            p1.Build_Peso(1, comprimento);
//            p2.Build_Peso(1, comprimento);
//            bloco_pts->Modifica(geometria.g1, &p1);
//            bloco_pts->Modifica(geometria.g2, &p2);
//            p1.Set_Num_Geometrias(0);
//            p2.Set_Num_Geometrias(0);
//        }
        for (j=0; j<bloco_geometria.size(); j++)
        {
            geometria = bloco_geometria[j];
            comprimento = geometria.Calc_Comprimento(bloco_pts);
            bloco_pts[geometria.g1].Build_Peso(1, comprimento);
            bloco_pts[geometria.g2].Build_Peso(1, comprimento);
            p1.Set_Num_Geometrias(0);
            p2.Set_Num_Geometrias(0);
        }
/////////////////////////////////////////////////////////////////////////


//        for (j = 4; j < bloco_pts->Get_Total_Componentes(); j++)
        for (j = 4; j < bloco_pts.size(); j++)
        {
////////////////////////////  STL ////////////////////////////////////////////
//                bloco_pts->Get_Componente(j, p1);
//                p1.Build_Peso('*');
//                bloco_pts->Modifica(j, &p1);
                bloco_pts[j].Build_Peso('*');
///////////////////////////////////////////////////////////////////////////////
        }

        while (mark != 0)
        //while (j < 3)
        {
            j++;
            mark = 0;
            elementos_total = bloco_elem_2D->Get_Total_Componentes();
            for (i = 0; i < elementos_total; i++)
            {
                status = bloco_elem_2D->Get_Componente(i, el_atual);
                if (el_atual.Get_Validade())
                {
//                    status = bloco_pts->Get_Componente(el_atual.Get_V1(), p1);
//                    status = bloco_pts->Get_Componente(el_atual.Get_V2(), p2);
//                    status = bloco_pts->Get_Componente(el_atual.Get_V3(), p3);
                    p1 = bloco_pts[el_atual.Get_V1()];
                    p2 = bloco_pts[el_atual.Get_V2()];
                    p3 = bloco_pts[el_atual.Get_V3()];

    //                x = el_atual.Get_Volume();
                    x1 = p1.X();
                    y1 = p1.Y();
                    x2 = p2.X() - x1;
                    y2 = p2.Y() - y1;
                    x3 = p3.X() - x1;
                    y3 = p3.Y() - y1;
                    x  = (x2 * y3 - x3 * y2);
                    (x > 0)? x : x = -x;


                    peso1 = p1.Get_Peso();
                    peso2 = p2.Get_Peso();
                    peso3 = p3.Get_Peso();
                    //switch(*media_refinamento)
                    switch(metodo)
                    {
                        case ARITHMETIC: peso = (peso1 + peso2 + peso3)/3; break;
                        case GEOMETRIC : peso = pow(peso1 * peso2 * peso3, 1./3.); break;
                        case HARMONIC  : peso = 3.* peso1 * peso2 * peso3 /
                                         (peso1 * peso2 + peso2 * peso3 + peso3* peso1);
                                         break;
                        case SQUARED_HARMONIC: y1 = peso1*peso1;
                                y2 = peso2*peso2;
                                y3 = peso3*peso3;
                                peso = sqrt(3.) * peso1 * peso2 * peso3 /
                                       sqrt(peso1 * peso2 + peso2 * peso3 + peso3* peso1);
                                break;
                        default: peso = 0; break;
                    }

                    y = peso *peso;
                    if (y < x)
                    {
                        mark++;
                        x = (peso1 + peso2 + peso3)*2.;
                        y1 = (peso2 + peso3)/x;
                        y2 = (peso1 + peso3)/x;
                        y3 = (peso1 + peso2)/x;

                        x = p1.X()*y1 + p2.X()*y2 + p3.X()*y3;
                        y = p1.Y()*y1 + p2.Y()*y2 + p3.Y()*y3;
    //--------------------------------------------------------------
                        p1.Set_Num_Geometrias(0);
                        p1.Set_Coord(x, y);
                        p1.Set_Peso(peso);
//                    status = bloco_pts->Inclui(p1);
                    bloco_pts.push_back(p1);
                    //status = bloco_pts->Inclui(x,y,0,(lev_math::Uchar)REF);
                    if (status){
                         status = 115;
                         return status;
                    }
                    else
                    {   // ponto incluido normalmente
                        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        // Nesta situacao, qualquer ponto incluido garantidamente nao gera nenhuma
                        // degenerescencia ou volume igual a zero
                        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                        if (!(bloco_pts->Get_Total_Componentes()%50))
                        //if (!(bloco_pts.size()%50))
                        //{
                        //    char aux [15] = " , ";
//                            std::cout <<bloco_pts->Get_Total_Componentes() <<aux <<10;
                        //    std::cout <<bloco_pts.size() <<aux <<10;
                        //}
//                        retorno = Altera_Malha(i, bloco_pts->Get_Total_Componentes() - 1);
                        retorno = Altera_Malha(i, bloco_pts.size()- 1);

                        if (retorno == 20 || retorno == 30)
                        {                                  // ocorrencia de caso degenerado
//                            bloco_pts->Elimina_Ultimo(); // elimina o componente recem gerado
                            bloco_pts.pop_back(); // elimina o componente recem gerado
                        }
                        else if (retorno == 15)
                        {
//                            bloco_pts->Elimina_Ultimo(); // elimina o componente recem gerado
                            bloco_pts.pop_back(); // elimina o componente recem gerado
                            Informa_Erro("Erro 15: Atingido o limite maximo de elementos do sistema. Tente aumentar a quantidade de memoria extendida disponivel!",
                                                         retorno);
                        }
                        if(i >= bloco_elem_2D->Get_Total_Componentes()) // interrompe laco i (houve compactacao da lista) // *AP* teste somente
                        {
                            // este teste visa evitar um possivel erro ocasionado pela compactacao do
                            // vetor de elementos em Altera_Malha. Se a compactacao ocorrer, o indice do
                            // proximo elememento podera ser menor que o numero de eleemntos existente
                            // anteriormente na lista (elementos_total. Nestas condicoes optou-se por
                            // interromper o processo, pura e simplesmente.
                            break;
                        }
                    } // fim do teste de inclusao de pontos
    //--------------------------------------------------------------
                    }

                }//fim do teste de validade do el_atual
            }//fim do laco for i
            //if (mark == 0) continua = FALSO;
        }//fim do laco while

        }//fim do teste de associa��o de bloco_pontos e Bloco_Elementos_2D
    } // tratamento dos metodos de refinamento
    return status;
}// fim do servi�o refina_p --------------------------------------------

void Delaunay2D::Limpa_Concavidades()
{
    lev_math::USINT  status;
    long   i, j, k, l, n_vizinhos, aux = 1;
    long * elementos_vizinhos = NULL;
    int mark = 1;//, cond = 1;
    FACE faces_el_atual[3], faces_el_auxiliar[3];
    Elemento_2D el_atual;
    Elemento_2D el_auxiliar[2];
    Lista_Elementos_2D_D lista_elem_contiguos;
    Aresta_Geometria geometria;
    long * plist_index = NULL;
    plist_index = new long [bloco_elem_2D->Get_Total_Componentes()];


    for (i = 0; i < bloco_elem_2D->Get_Total_Componentes(); i++)
    {//procura por elemento v�lido
        status = bloco_elem_2D->Get_Componente(i, el_atual);
        if (status)
        {
            status =  110;
            Informa_Erro("Erro 110(2): Dados Inconsistentes. Execucao interrompida...",status);
        }
        if (el_atual.Get_Validade())
        {//se elemento � v�lido recupera os o n�mero de vizinhos
            n_vizinhos = el_atual.Get_N_Vizinhos();
            if (int(el_atual.Get_N_Vizinhos()) < n_vertices)
            {//se o elemento possui menos que 3 vizinhos
            //recupera os vizinhos e monta a lista de faces
                elementos_vizinhos = el_atual.Get_Vizinhos();
                el_atual.Monta_Faces(faces_el_atual);

                for (j=0; j < n_vizinhos; j++)//captura vizinhos v�lidos
                {
                    status = bloco_elem_2D->Get_Componente(elementos_vizinhos[j], el_auxiliar[j]);
                    if (status)
                    {
                        status =  110;
                        Informa_Erro("Erro 110(4): Dados Inconsistentes. Execucao interrompida...",status);
                        exit(1);
                    }
                }
                for (j=0; j < n_vertices; j++)//captura face n�o conpartilhada com os vizinhos LEV_VALIDs
                {
                    mark = 0;
                    for(k=0; k < n_vizinhos; k++)
                    {
                        el_auxiliar[k].Monta_Faces(faces_el_auxiliar);
                        for(l = 0; l < n_vertices; l++)
                            if ( ((faces_el_atual[j].V1 == faces_el_auxiliar[l].V1)&&
                                  (faces_el_atual[j].V2 == faces_el_auxiliar[l].V2))|| (
                                (faces_el_atual[j].V1 == faces_el_auxiliar[l].V2)&&
                                (faces_el_atual[j].V2 == faces_el_auxiliar[l].V1)))
                                mark = 1;
                    }
                    if (mark == 0)
                    {
                        aux = j;
                        break;
                    }
                }
                //checa se a aresta compartilhada nao � de geometria
                mark=0;    //se a aresta nao for encontrada o valor de mark ser� 0

///////////////////////////// STL ////////////////////////////////////////////////////
//                for (j=0; j<bloco_geometria->Get_Total_Componentes(); j++)
//                {
//                    status = bloco_geometria->Get_Componente(j,geometria);
//                    if ((faces_el_atual[aux].V1 == geometria.g1) && (faces_el_atual[aux].V2 == geometria.g2) ||
//                        (faces_el_atual[aux].V1 == geometria.g2) && (faces_el_atual[aux].V2 == geometria.g1))
//                    {
//                        mark=1;//se a aresta for encontrada mark == 1
//                        break;// e para a procura
//                    }
//                }
                for (j=0; j<bloco_geometria.size(); j++)
                {
                    geometria = bloco_geometria[j];
                    if ((faces_el_atual[aux].V1 == geometria.g1) && (faces_el_atual[aux].V2 == geometria.g2) ||
                        (faces_el_atual[aux].V1 == geometria.g2) && (faces_el_atual[aux].V2 == geometria.g1))
                    {
                        mark=1;//se a aresta for encontrada mark == 1
                        break;// e para a procura
                    }
                }

//////////////////////////////////////////////////////////////////////////////////////

                if (mark == 0)//se mark == 0 chama a fun��o reccursiva Lista_Elementos_Contiguos
                {
                    printf("Eis uma face que n�o � de geometria e n�o tem vizinhos.\n");
                    printf("%i %i\n", faces_el_atual[aux].V1, faces_el_atual[aux].V2);
                //    getch();
                    Lista_Elementos_Contiguos(&lista_elem_contiguos, plist_index, i, 1);

                }
            }//if ((el_atual.Get_N_Vizinhos()) < 3)
        }//if(el_atual.Get_Validade())
    }//for i
}



//============================================================================
//class Delaunay2D :: Mantem_Geometria(long  *ind_elem_elim,
//                    lev_math::USINT * num_elem_elim, Pt_Malha * ponto
//                      long * ind_ponto)
//============================================================================
//
//    Descri��o: Chamada quando uma quebra de geometria ocorre,
//               procura pela aresta de geometria nos elementos
//               a serem eliminados. Uma vez encontrada a aresta
//               as coordenadas do ponto s�o alteradas para o centro
//               da aresta. Uma aresta de geometria nova e cirada e a antiga 
//               � ajustada eliminar a quebra de geometria.
//
//      parametros: indi_elem_elim - vetor contendo os �ndices
//                dos elementos eliminados
//
//                  num_elem_elim - n�mero de elementos eliminados
//
//                ponto - ponto para altera��o das coordenadas
//
//                  ind_ponto - �ndice do ponto em bloco_pts
//
//                  vizinhos_distantes
//
//                  num_vizinhos_distantes
//
//                  ind_ponto
//
//   retorno :         0 N�O HOUVE QUEBRA
//                     1 UMA QUEBRA COM GEOMETRIA RESTAURADA
//                    -1;  ponto deve ser ignorado (MAIS DE UMA QUEBRA)
//
//
//    vers�o : 1.0
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 28/03/2001
//
//============================================================================
//lev_math::USINT Delaunay2D::Mantem_Geometria(long  *ind_elem_elim, lev_math::USINT * num_elem_elim,
//                                  Pt_Malha * ponto, long  * vizinhos_distantes,
//                                  lev_math::USINT * num_vizinhos_distantes,
//                                  long  * ind_ponto)
int Delaunay2D::Mantem_Geometria(long  *ind_elem_elim, lev_math::USINT num_elem_elim,
                                 long  ind_ponto)
{
    Elemento_2D elemento;
    lev_math::USINT status,elementos_introduzidos = 0;//, n_faces_total
    long i, j, k, l, m = 0;
    long n,ind_geometria = -1;// o,
    long vert_elem[60][3]; // *AP* rever o dimensionamento deste array
    Aresta_Geometria facesl[20];//, arestas_el[3];
    Aresta_Geometria geometria;
    Pt_Malha pt_aux, pt_aux1;
    int retorno = 0;//, retorno2;
    Elemento_2D el;
    char flag;
//    short int tipoCC;
    bool achou_aresta_possivel = false;
//    long vertices[3];
    int conta_quebras = 0;
    //recupera��o dos pontos vertices de cada um dos elementos que formam
    // o poligono de insercao
    for (i = 0; i < num_elem_elim; i++)
    {
        status = bloco_elem_2D->Get_Componente(ind_elem_elim[i], elemento);
        if (status)
        {
            status = 6;
            std::cout << "Erro 6: Dados inconsistentes. bloco_elem_2D em Mantem_Geometria... ";
            exit(status);
        }
        elemento.Get_Vertices(vert_elem[i]);
    }

    //procura pelas arestas compartilhadas pelos elementos que
    // formam o poligono englobante
    //m=0;
    for (j = 0, m = 0; j < num_elem_elim-1; j++)//linha para comparacao
        for(k = j+1; k < num_elem_elim; k++)// linha a ser comparada
        {
            flag=0;
            for(l = 0; l < elemento.Get_N_Vertices(); l++)// procura pelo elemento em uma posicao da linah
            {
                if ( vert_elem[j][l]==vert_elem[k][0] ||
                     vert_elem[j][l]==vert_elem[k][1] ||
                     vert_elem[j][l]==vert_elem[k][2])
                     if (flag == 0)//armazenamento das arestas comuns
                     {
                        facesl[m].g1=vert_elem[j][l];
                        flag++;
                     }
                     else
                     {
                        facesl[m].g2=vert_elem[j][l];
                        m++;
                        break;
                     }
            }
        }


    //procura por uma aresta de geometria ou pre-discretizacao, ou seja,
    // com ambos vertices sendo do tipo zero ou 1.
    // mantem seu indice no vetor faces (k) guardado
    ind_geometria = -1;

    for(j = 0; j < m /*&& (ind_geometria == -1)*/ ; j++)
    {
        achou_aresta_possivel    = false;

//        status = bloco_pts->Get_Componente(facesl[j].g1,pt_aux);
        pt_aux = bloco_pts[facesl[j].g1];
        if (status)
        {
            status = 6;
            std::cout << "Erro 6: Dados inconsistentes. bloco_pts em Mantem_Geometria... ";
            exit(status);
        }

        if (pt_aux.Get_Tipo() != REF)
        {
//            status = bloco_pts->Get_Componente(facesl[j].g2,pt_aux);
            pt_aux = bloco_pts[facesl[j].g2];
            if (status)
            {
                status = 6;
                std::cout << "Erro 6: Dados inconsistentes. bloco_pts em Mantem_Geometria... ";
                exit(status);
            }

            if (pt_aux.Get_Tipo() != REF)
            {
                k=j;//armazena o indice da face no vetor faces
                achou_aresta_possivel = true;
//                break;
            }
        }

        //procura a face[k] na lista de geometrias
        //e armazena seu indice em ind_geometria
        if(achou_aresta_possivel)
        {

/////////////////////////// STL //////////////////////////////////////////////////////
//            for (n=0; n<bloco_geometria->Get_Total_Componentes() ; n++)
//            {
//                status = bloco_geometria->Get_Componente(n,geometria);
//                if (status)
//                {
//                    status = 6;
//                    std::cout << "Erro 6: Dados inconsistentes. bloco_geometria em Mantem_Geometria... ";
//                    exit(status);
//                }
            for (n=0; n<bloco_geometria.size() ; n++)
            {
                geometria = bloco_geometria[n];
///////////////////////////////////////////////////////////////////////////////////////

                if ((facesl[k].g1 == geometria.g1) && (facesl[k].g2 == geometria.g2) ||
                (facesl[k].g1 == geometria.g2) && (facesl[k].g2 == geometria.g1))
                {
                    ind_geometria=n;
                    //atribuicoes para garantir que a sequencia de vertices
                    // de faces[k] seja a mesma da geometria na lista
                    conta_quebras++;
//                    break;
                }
            }
        }
    }

    if (conta_quebras > 1) retorno = -1;   // ponto deve ser ignorado
    else if (conta_quebras == 1)
    {
        Pt_Malha o_ponto, ponto;
//        bloco_pts->Get_Componente(ind_ponto, o_ponto);
        o_ponto = bloco_pts[ind_ponto];

//////////////////////////////////////  STL //////////////////////////////////////
//        bloco_geometria->Get_Componente(ind_geometria,geometria);
        geometria = bloco_geometria[ind_geometria];
//////////////////////////////////////////////////////////////////////////////////

        facesl[k].g1=geometria.g1;
        facesl[k].g2=geometria.g2;

//        if(ind_geometria>= 0 && ind_geometria < bloco_geometria->Get_Total_Componentes() )
        {

            //obtem as coordenadas do centro da aresta de geometria e armazena em ponto
            geometria.Get_Central_Point(bloco_pts,ponto);


            // S� restaura geometria se o ponto j� estiver no ponto central da geometria
            if (o_ponto == ponto)
            {
                // ponto foi incluido em aresta pelo processo de refinamento
                retorno = 0;

                geometria.g2= ind_ponto;

////////////////////////////////////////  STL ///////////////////////////////////////////
//                status = bloco_geometria->IncluiVerifica(geometria); // inclusao com sucesso retorna 0
                {
                //    using namespace std;
                std::vector<Aresta_Geometria>::iterator where;
                where = std::find(bloco_geometria.begin(),bloco_geometria.end(), geometria);
                if(*where == geometria)
                    status = 1;
                else status = 0;
                }
/////////////////////////////////////////////////////////////////////////////////////////

                if (status==0)
                {

                    facesl[k].g1=  ind_ponto;
                    facesl[k].prim = geometria.prim;

////////////////////////////////////////  STL ///////////////////////////////////////////
//                    bloco_geometria->Modifica(ind_geometria, facesl[k]);
                    bloco_geometria[ind_geometria] = facesl[k];
/////////////////////////////////////////////////////////////////////////////////////////

                }
                else //erro na inclusao ou aresta j� existente na lista
                {
                    retorno = 0;
                }
            }
            else
            {
//                bloco_pts->Modifica(ind_ponto, ponto);
                bloco_pts[ind_ponto] = ponto;
                retorno = 1;
            }

        }
    }


    return (retorno);
}

//============================================================================
//class Delaunay2D :: Recupera_Propriedades(Elemento_2D *elemento)
//============================================================================
//
//    Descri��o: Dado um elemento recupera o material associado ao
//                 ao elemento por meio dos ind�ces de materiais
//               e tipos de pontos que comp�e o elemento
//
//
//      parametros: elemento - elemento em que o material deve ser
//                recuperado e associado
//
//    retorno: 0 se o material foi recuperado com sucesso
//               1 se houve quebra de geometria
//               2 n�o h� material associado aos pontos
//
//    vers�o : 1.1
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 28/03/2001
//
//============================================================================

char Delaunay2D::Recupera_Propriedades(long  *ind_elem_elim, lev_math::USINT * num_elem_elim,Elemento_2D &elemento)
{
    Pt_Malha pt[3];
    long vertices[3];
    long materiais[3], aux_material=0, material_el_eliminado = 0;
    double fontes[3], aux_fontes = 0, fontes_el_eliminado = 0.0;
    lev_math::Uchar tipos[3];
    lev_math::USINT i;
    char retorno = 0;
    char pts_mat = 0;//pontos com materiais
    char pts_fontes = 0;
    Elemento_2D o_elemento_eliminado;    //*NMA*
    Pt_Malha    o_ponto_baricentro;        //*NMA*
    bool        is_inside = false;        //*NMA*
    int            status;                    //*NMA*
    long int li_zero = 0;
    short int si_zero = 0;

    elemento.Get_Vertices(vertices);

    for(i=0; i<elemento.Get_N_Vertices(); i++)//recupera materiais, tipos e geometrias
    {
//        bloco_pts->Get_Componente(vertices[i], pt[i]);
        pt[i] = bloco_pts[vertices[i]];
        tipos[i]=pt[i].Get_Tipo();
        materiais[i] = pt[i].Get_Material(li_zero);

        if(pt[i].Get_Fonte()) fontes[i] = pt[i].Get_Fonte(si_zero);
        else fontes[i] = 0.0;

        (tipos[i] == REF) ? pts_mat++ : pts_mat;
    }


    // varre a lista de elementos eliminados e verifica se o baricentro
    // do elemento atual est� contido em algum elemento eliminado
    // se sim, atribui material , se n�o: ???????????
    elemento.Get_Baricentro(bloco_pts,o_ponto_baricentro);

    is_inside = false;
    for (i = 0; i < *num_elem_elim && !is_inside; i++)
    {
        status = bloco_elem_2D->Get_Componente(ind_elem_elim[i], o_elemento_eliminado);
        if (status)
        {
            status = 6;
            std::cout << "Erro 6: Dados inconsistentes. bloco_elem_2D em Mantem_Geometria... ";
            exit(status);
        }

        is_inside = o_elemento_eliminado.Is_Point_Inside(bloco_pts, o_ponto_baricentro);
    }

    if(is_inside)
    {
        material_el_eliminado    = o_elemento_eliminado.Get_Material();
        fontes_el_eliminado        = o_elemento_eliminado.Get_Fonte();
    }
    else
    {
        if(pts_mat == 0)
        {
            std::cout << "\n AVISO: Nao encontrou elemento eliminado que cont�m "
                        << " baricentro de elemento novo ";
//            exit(i);
        }
        //retorno = 1;
        //std::cout << "\n ERRO: Nao encontrou elemento eliminado que cont�m baricentro de elemento novo";
//        std::cout << "\n pts_mat = " << pts_mat;
//        getchar();
//        exit(i);
    }

    switch( pts_mat )
    {
        case 0: //nenhum ponto � de refinamento //*NMA*
            aux_material    = material_el_eliminado;
            aux_fontes        = fontes_el_eliminado;
        break;

        case 1:
            if(tipos[0] == REF)
            {
                aux_material    =    materiais[0];
                aux_fontes        =    fontes[0];
            }
            else if (tipos[1] == REF)
            {
                aux_material    =    materiais[1];
                aux_fontes        =    fontes[1];
            }
            else if (tipos[2] == REF)
            {
                aux_material    =    materiais[2];
                aux_fontes        =    fontes[2];
            }
            else
                retorno=1;

        break;

        case 2:
            if((tipos[0]<2)&&(materiais[1]==materiais[2]) && (fontes[1]==fontes[2]))
            {
                aux_material    =    materiais[1];
                aux_fontes        =    fontes[1];
            }
            else if((tipos[1]<2)&&(materiais[0]==materiais[2]) && (fontes[0]==fontes[2]))
            {
                aux_material    =    materiais[0];
                aux_fontes        =    fontes[0];
            }
            else if((tipos[2]<2)&&(materiais[0]==materiais[1]) && (fontes[0]==fontes[1]))
            {
                aux_material    =    materiais[0];
                aux_fontes        =    fontes[0];
            }
            else
                retorno=1;
        break;

        case 3:
            if((materiais[0]==materiais[1])&&(materiais[1]==materiais[2]) &&
               (fontes[0]==fontes[1]) && (fontes[1]==fontes[2]) )
            {
                aux_material    =    materiais[0];
                aux_fontes        =    fontes[0];
            }
            else
                retorno=1;
        break;
    }

    if (retorno==0)
    {
        elemento.Set_Material(aux_material);
        elemento.Set_Fonte(aux_fontes);
    }

return(retorno);
}


//============================================================================
//void Delaunay2D::Pre_Discret(int passos)
//============================================================================
//
//    Descri��o: A partir da lista de geometrias e da nuvem inicial de pontos
//               cria novas geometrias utilizando as coordenadas do centro de 
//                 cada segmento de geometria e armazena o novo ponto na nuvem
//                 de pontos.
//                 O processo pode ser repetido de acordo com o n�mero de
//                 passos desejados.
//
//    Par�metro: passos - opcional, se n�o preenchido na chamada da fun��o
//               assume valor 1, a divis�o das arestas pode ser feita
//                 com um n�mero maior de passos pode ser feita passando este
//               par�metro na chamada da fun��o.
//
//    retorno: 
//
//    vers�o : 1.0
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 30/04/2001
//
//============================================================================
void Delaunay2D::Pre_Discret(int passos)
{
    long n_pontos;//n�mero de pontos original da malha
                  // + os pontos do retantulo englobante
    long n_gorig; //n�mero de geometrias original
    int i;
    long j;//contador
    long v_final, v_inicial;

    const long i_pontos_retangulo_englobante = 0; // *AP* esta fun��o serve somente para o caso de malha automatica.
                                            // Neste caso a contagem de indice comeca a partir de 4, devido ao
                                            // retangulo englobante.
    Aresta_Geometria geometria;
    Pt_Malha ponto;


//    n_pontos=nuvem_pontos->Get_Total_Componentes() + i_pontos_retangulo_englobante;
//    n_pontos=bloco_pts->Get_Total_Componentes();// + i_pontos_retangulo_englobante;
    n_pontos=bloco_pts.size();// + i_pontos_retangulo_englobante;

    for(i=0; i<passos; i++)
    {

///////////////////////  STL //////////////////////////////////////
//        n_gorig=bloco_geometria->Get_Total_Componentes();
        n_gorig=bloco_geometria.size();
////////////////////////////////////////////////////////////////

        for (j=0;j < n_gorig ;j++)
        {
///////////////////////  STL //////////////////////////////////////
//            bloco_geometria->Get_Componente(j, geometria);
            geometria = bloco_geometria[j];
////////////////////////////////////////////////////////////////
            geometria.g1 -= n_pontos;
            geometria.g2 -= n_pontos;
//            geometria.Get_Central_Point(bloco_pts, ponto);
            geometria.Get_Central_Point(nuvem_pontos, ponto);
            ponto.Set_Tipo(PRE);
///////////////////////  STL //////////////////////////////////////
//            if (nuvem_pontos->Inclui(ponto.X(), ponto.Y(),
//                ponto.Z(), ponto.Get_Tipo()));//return 1;
            nuvem_pontos.push_back(ponto);//return 1;
v_final=geometria.g2;
            v_inicial=geometria.g1;

            geometria.g1= n_pontos + v_inicial;
//            geometria.g2= n_pontos + nuvem_pontos->Get_Total_Componentes() -1;
            geometria.g2= n_pontos + nuvem_pontos.size() -1;

///////////////////////  STL //////////////////////////////////////
//            if (bloco_geometria->Inclui(geometria));//return 1;
            bloco_geometria.push_back(geometria);
//////////////////////////////////////////////////////////////////

//            geometria.g1=n_pontos + nuvem_pontos->Get_Total_Componentes() -1;
            geometria.g1=n_pontos + nuvem_pontos.size() -1;
            geometria.g2=n_pontos + v_final;

///////////////////////  STL //////////////////////////////////////
//            bloco_geometria->Modifica(j,&geometria);
            bloco_geometria[j] = geometria;
//////////////////////////////////////////////////////////////////

        }// fim do la�o j
    }//fim do la�o i
}

//===============================================================

//Delaunay_2D :: Verifica_Quebra_Geometria
//===============================================================

// descri��o : verifica se ocorre uma quebra de geometria para o 
//             novo ponto incluido. 
// procedimento: varre a lista de faces dos triangulos identificados 
//             para elimina��o, em busca de pelo menos uma face de 
//             triangulo que seja de geometria.
//             encontrando, desloca o ponto novo para o centro da 
//             face.
    //VARRE FACES REPETIDAS (INVALIDAS) EM BUSCA DE PELO MENOS UMA
    //    QUE SEJA DE GEOMETRIA (ARMAZENA EM QUEBRA)

    // SE QUEBRA > 0
        // SE QUEBRA = 1
                // INDICA MUDAN�A DE POSI��O DO PONTO PARA O CENTRO DA FACE
        // SE QUEBRA > 1
                // INCLUI UM PONTO EM CADA FACE QUEBRADA OU IGNORA O PONTO?

    // RETORNA QUEBRA.

// ATEN��O : VErificar o operador [] de lev_math::Lo_Templ \
//           Ele deve devolver uma c�pia do objeto e n�o uma refer�ncia.

//======================================================================
int Delaunay2D :: Verifica_Quebra_Geometria(FACE * faces, lev_math::USINT n_faces_total)
{
    int i, j;
    int quebra = 0;
    int * indices_quebra = NULL;
    indices_quebra = new int[n_faces_total];
    if (indices_quebra)
    {
        for (i = 0; i < n_faces_total; i++)
        {
            if (faces[i].Valido == LEV_INVALID)
            {
                // VERIFICA SE � FACE DE GEOMETRIA

//////////////////////////////////// STL /////////////////////////////////////////
//                for (j = 0; j < bloco_geometria[0].Get_Total_Componentes(); j++)
//                {
//                    if( faces[i] == bloco_geometria[0][j] )
//                    {
//                        indices_quebra[quebra++] = j;
//                        break;
//                    }
//                }
                for (j = 0; j < bloco_geometria.size(); j++)
                {
                    if( faces[i] == bloco_geometria[j] )
                    {
                        indices_quebra[quebra++] = j;
                        break;
                    }
                }

//////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }
    else
    {
        quebra = -1; // indica problema de mem�ria
    }

    if (indices_quebra) delete [] indices_quebra;
    return quebra;
}

void Delaunay2D :: Apaga(void)
{
    if(bloco_elem_2D)                            bloco_elem_2D->Apaga();
//    bloco_elem_2D    = NULL;

//////////////////////////////////// STL //////////////////////////////////////
//    if(bloco_geometria)                            bloco_geometria->Apaga();
//    bloco_geometria  = NULL;
    if(bloco_geometria.size())                    bloco_geometria.clear();
    if(bloco_pts.size())                        bloco_pts.clear();
    if(nuvem_pontos.size())                        nuvem_pontos.clear();
//////////////////////////////////////////////////////////////////////////////

    if(bloco_nos_elm.Get_Total_Componentes())    bloco_nos_elm.Apaga();

    Delaunay2D_Ok    = lev_enum::YES ;
    n_vertices = 3;
    if(faces) delete [] faces;
    faces            = NULL;
    faces = new FACE[MAXIMOFACES];
    if (!faces){std::cout << "\n incapaz de gerar vetor de faces . Execucao interrompida ..."; exit(0);}

}// end of Apaga



#if 0

//===========================================================================
//               class DGround  :: DGround
//===========================================================================
//
DGround :: DGround(void) : Delaunay2D() {
     DGround_Ok = SIM ;
//     Regioes   = NULL;
//     RegiaoI   = NULL;
//     RegiaoII  = NULL;
//     RegiaoIII = NULL;
//     RegiaoIV  = NULL;
//     RegiaoI   = new Regiao;
//     RegiaoII  = new Regiao;
//     RegiaoIII = new Regiao;
//     RegiaoIV  = new Regiao;
//     if (!RegiaoI || !RegiaoII || !RegiaoIII || !RegiaoIV) {
//     std::cout << "\a\a\n\n\nERRO 102 : Execucao interrompida...";
//         Informa_Erro("Erro 102: Execucao interrompida...",,102);
//         exit (102);
//     }

}

DGround :: ~DGround (void) {
     DGround_Ok      = NAO;
//     if (RegiaoI)   delete RegiaoI  ;
//     if (RegiaoII)  delete RegiaoII ;
//     if (RegiaoIII) delete RegiaoIII;
//     if (RegiaoIV)  delete RegiaoIV ;

}


//===========================================================================
//               class DGround :: Dimensiona_Englobante
//===========================================================================
//
//  Descricao          : Dimensiona o paralelepipedo envolvente em funcao
//                       dos limites maximo e minimo da nuvem de pontos
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 22/04/94
//
//===========================================================================

void DGround :: Dimensiona_Englobante(void)  {

    if (bloco_pts && bloco_elem_2D)
    {
     #if 10
     Delaunay2D :: Dimensiona_Englobante();

     #else

    switch (simetria) { //=======================================
        case  ASSIMETRICA   :
            {
            double meioL  =  RegiaoII->Get_MaxR() * fator_englobante;
            double H1     = -RegiaoIV->Get_MaxZ() * 0.3;
            double H2     =  RegiaoIV->Get_MaxZ() * 1.3;
            bloco_pts->Inclui(-meioL,-meioL,H1,0);
            bloco_pts->Inclui(-meioL, meioL,H1,1);
            bloco_pts->Inclui( meioL, meioL,H1,2);
            bloco_pts->Inclui( meioL,-meioL,H1,3);
            bloco_pts->Inclui(-meioL,-meioL,H2,4);
            bloco_pts->Inclui(-meioL, meioL,H2,5);
            bloco_pts->Inclui( meioL, meioL,H2,6);
            bloco_pts->Inclui( meioL,-meioL,H2,7);
            }
            break;

        case  EIXO         :
            {
             double meioL  =  RegiaoII->Get_MaxR() * fator_englobante;
             flo�....................�>Get_MaxZ() * 0.3;
             double H2     =  RegiaoIV->Get_MaxZ() * 1.3;
            double p0     =  RegiaoII->Get_MaxR()*.3;
            bloco_pts->Inclui(-meioL,   -p0,H1,0);
            bloco_pts->Inclui(-meioL, meioL,H1,1);
            bloco_pts->Inclui( meioL, meioL,H1,2);
            bloco_pts->Inclui( meioL,   -p0,H1,3);
            bloco_pts->Inclui(-meioL,   -p0,H2,4);
            bloco_pts->Inclui(-meioL, meioL,H2,5);
            bloco_pts->Inclui( meioL, meioL,H2,6);
            bloco_pts->Inclui( meioL,   -p0,H2,7);
            }
            break;

        case  QUADRANTE    :
            {
            double meioL  =  RegiaoII->Get_MaxR() * fator_englobante;
            double H1     = -RegiaoIV->Get_MaxZ() * 0.3;
            double H2     =  RegiaoIV->Get_MaxZ() * 1.3;
            double p0     =  RegiaoII->Get_MaxR()*.3;
            bloco_pts->Inclui(   -p0,   -p0,H1,0);
            bloco_pts->Inclui(   -p0, meioL,H1,1);
            bloco_pts->Inclui( meioL, meioL,H1,2);
            bloco_pts->Inclui( meioL,   -p0,H1,3);
            bloco_pts->Inclui(   -p0,   -p0,H2,4);
            bloco_pts->Inclui(   -p0, meioL,H2,5);
            bloco_pts->Inclui( meioL, meioL,H2,6);
            bloco_pts->Inclui( meioL,   -p0,H2,7);
            }
            break;

    } // fim do switch===========================================
     #endif
    } // fim do teste de associacao de objetos
} // fim do metodo Dimensiona_Englobante ====================================


//void DGround ::  Associa(Indicador_Movel *obj)      { Delaunay :: Associa(obj);}
void DGround ::  Associa(Lista_Elementos_2D_D *obj) { bloco_elem_2D = obj;}
//void DGround ::  Associa(Lista_Elementos_1D *obj)   { bloco_elem_1D = obj;}
void DGround ::  Associa(Lo_Pt_Malha *obj)          { bloco_pts     = obj;}


//============================================================================
//          CLASS DGROUND  ::  GRAVA_ELEMENTOS
//============================================================================
//
// Descricao        : grava os elementos gerados pelo servico discretiza conforme
//                    documento de arquivos de comunicacao de dados (set/94)
//
// parametro        : o path completo do arquivo
//
// retorno          : codigo de status de execucao:
//                                 0  :  execucao de leitura normal
//                               112  :  dados inconsistentes. Componente nao
//                                       localizado na lista de elementos
//                               121  :  arquivo nao pode ser aberto para
//                                       gravacao de elementos. Deve-se aumentar
//                                       o numero de arquivos que pode ser aberto
//                                       concomitantemente
//                               138  :  dados inconsistentes na lista de pontos
//
//============================================================================


int DGround :: grava_elementos(char *arquivo,double Rint, double Rext, double Zint, double Zext)
{
    Elemento_2D   el;
    Pt_Malha       pt;
    FILE          *arq_temp1 = NULL;
    FILE          *arq_temp2 = NULL;
    FILE          *arq_temp3 = NULL;
    FILE          *arq_temp4 = NULL;
//    FILE          *arq_temp6 = NULL;
//    FILE          *arq_temp5 = NULL;
    FILE          *arq_saida = NULL;
    FILE          *intersec  = NULL;
    FILE          *catastr   = NULL;
    double         r2min, zmin, r2max, zmax;       // referente ao ponto
    double         r2inter, zinter;                // posicao (em r2 e z) do ponto
    double         rrint, rrext;                   // referem-se ao dominio
    double         fator = sqrt(fabs(SIGMA));            // primeira avalizacao de erro no calculo da posicao radial do ponto
    double         xcm, ycm, zcm;                  // coordenadas do centro de massa
    long           cont_elem_regiao[7] = {0,0,0,0,0,0,0}; // numero de elementos localizados em cada regiao
    long           catastrofe = 0;
    long           p[4];
    long           k ;        // contador do laco de elementos
//    long num_val_total=0;   // contador de elementos LEV_VALIDs da lista
    int            status = 0;
    int            j;
    lev_math::Uchar          regiao;

    rrint     = Rint*Rint;
    rrext     = Rext*Rext;
    arq_temp1 = fopen("reg1.tmp","w");
    arq_temp2 = fopen("reg2.tmp","w");
    arq_temp3 = fopen("reg3.tmp","w");
    arq_temp4 = fopen("reg4.tmp","w");
//    arq_temp6 = fopen("temp6.tp6","w");
//    arq_temp5 = fopen("temp5.tp5","w");
    intersec  = fopen("intersec.tmp","w");
    catastr   = fopen("catastr.tmp","w");
//    if (arq_temp1 && arq_temp2 && arq_temp3 && arq_temp4 && arq_temp6 && arq_temp5) {
    if (arq_temp1 && arq_temp2 && arq_temp3 && arq_temp4) {

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    Conta o numero de componentes LEV_VALIDs em toda a lista, por regiao
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        k = 0;
        while(k<bloco_elem_2D->Get_Total_Componentes()) {
            status = bloco_elem_2D->Get_Componente(k,el);
            if (status) {
             status =  112;
                if (arq_temp1) fclose(arq_temp1);
                if (arq_temp2) fclose(arq_temp2);
                if (arq_temp3) fclose(arq_temp3);
                if (arq_temp4) fclose(arq_temp4);
//                if (arq_temp6) fclose(arq_temp6);
//                if (arq_temp5) fclose(arq_temp5);
             return (status);                       // <---------- ponto de retorno
            }

            if(el.Get_Validade())
            {
             p[0] = el.Get_V1();
             p[1] = el.Get_V2();
             p[2] = el.Get_V3();


             status = bloco_pts->Get_Componente(p[0],pt);
             if (status)
             {
                if (arq_temp1) fclose(arq_temp1);
                if (arq_temp2) fclose(arq_temp2);
                if (arq_temp3) fclose(arq_temp3);
                if (arq_temp4) fclose(arq_temp4);
//                if (arq_temp6) fclose(arq_temp6);
//                if (arq_temp5) fclose(arq_temp5);
                return 138;                  // <--------- ponto de retorno
             }

             r2min = r2max = pt.X()*pt.X() + pt.Y()*pt.Y();
             zmin  = zmax  = pt.Z();
             xcm = pt.X(); ycm = pt.Y(); zcm = pt.Z();

             for(j=1;j<num_vertices;j++)
             {
                status = bloco_pts->Get_Componente(p[j],pt);
                if (status)
                {
                    if (arq_temp1) fclose(arq_temp1);
                    if (arq_temp2) fclose(arq_temp2);
                    if (arq_temp3) fclose(arq_temp3);
                    if (arq_temp4) fclose(arq_temp4);
//                    if (arq_temp6) fclose(arq_temp6);
//                    if (arq_temp5) fclose(arq_temp5);
                 return 138;                // <--------- ponto de retorno
                }

                xcm += pt.X(); ycm += pt.Y(); zcm += pt.Z();
                r2inter = pt.X()*pt.X() + pt.Y()*pt.Y();
                zinter = pt.Z();
                if      (r2inter > r2max)    r2max = r2inter;
                else if (r2inter < r2min)    r2min = r2inter;


                if     (zinter > zmax)       zmax = zinter;
                else if(zinter < zmin)       zmin = zinter;
             } // fecha for j =1 ate 3

             if(r2max < (rrint+fator*rrint) && zmax <= Zint)                       // regiao I
                regiao = 1;
             else if (r2max < (rrint+fator*rrint) &&    /*zmax <= Zext &&*/ zmin >= Zint) // regiao IV
                regiao = 4;
             else if (r2min > (rrint+fator*rrint) && /*r2max <= rrext &&*/
                                zmax <= Zint)                                   // regiao II
                regiao = 2;
             else if (r2min > (rrint+fator*rrint) && /*r2max <= rrext &&*/
                             /*    zmax <= Zext &&*/ zmin >= Zint)                   // regiao III
                regiao = 3;

             else /*if ((r2min < (rrint-fator*rrint) && r2max > (rrint+fator*rrint)) || (zmin < Zint && zmax > Zint))*/
             {      // houve interseccao
                    // .................................................................
                    //     calcula o centro de massa o elemento para identificacao da regiao
                    // .................................................................
                    xcm /=4; ycm /=4; zcm /=4;
                    r2inter = xcm*xcm + ycm*ycm;
                    if      (r2inter <= (rrint/*+fator*rrint*/) && zcm <= Zint) regiao = 1;
                    else if (r2inter <= (rrint/*+fator*rrint*/) && zcm >= Zint) regiao = 4;
                    else if (r2inter >= (rrint/*+fator*rrint*/) && zcm <= Zint) regiao = 2;
                    else if (r2inter >= (rrint/*+fator*rrint*/) && zcm >= Zint) regiao = 3;

             }
//             else { regiao = 5; catastrofe++;}

//             if (regiao == 6)
//             {
//                    if(intersec) fprintf(intersec,"\nelemento n. %6ld : (raio max,raio min) = (%15.7g,%15.7g)  : (z max,z min) = (%15.7g,%15.7g) ",
//                                             k, sqrt(r2max), sqrt(r2min),zmax, zmin);
//             }
//             else if (regiao == 5)
//             {
//                    if(catastr ) fprintf(catastr ,"\nelemento n. %6ld : (raio max,raio min) = (%15.7g,%15.7g)  : (z max,z min) = (%15.7g,%15.7g) ",
//                                             k, sqrt(r2max), sqrt(r2min),zmax, zmin);
//             }
             cont_elem_regiao[regiao]++;
             el.Set_Validade(regiao);
             bloco_elem_2D->Modifica(k,&el);
            }  // fim do teste de validade do elemento
            k++;
        }
// __________________________________________________________________________

#if 10
{
    long somatoria = bloco_elem_2D->Get_N_Comp_LEV_VALIDs_Absoluto();
    char aux[400] = "Organiza_3D\n";
    strcat(aux,"\n total de elementos 3D : ");
    strcat(aux,ltoa(somatoria,NULL,10));
    strcat(aux,"\n elementos 3D da regiao I  : ");
    strcat(aux,ltoa(cont_elem_regiao[1],NULL,10));
    strcat(aux,"\n elementos 3D da regiao II : ");
    strcat(aux,ltoa(cont_elem_regiao[2],NULL,10));
    strcat(aux,"\n elementos 3D da regiao III: ");
    strcat(aux,ltoa(cont_elem_regiao[3],NULL,10));
    strcat(aux,"\n elementos 3D da regiao IV : ");
    strcat(aux,ltoa(cont_elem_regiao[4],NULL,10));
    strcat(aux,"\n elementos 3D interceptados: ");
    strcat(aux,ltoa(cont_elem_regiao[6],NULL,10));
    strcat(aux,"\n catastrofe                : ");
    strcat(aux,ltoa(catastrofe         ,NULL,10));
//#if defined INTERFACE_GRAFICA
//    Mensagem Debug;
//    Debug.Display(aux,VERMELHO,AMARELO,7,2);
//#else
    std::cout <<"\n\a"<<aux;
//#endif
}
#endif
        int tudo_certo;
        for (k = 0; k < bloco_elem_2D->Get_Total_Componentes() && !status; k++)
        {
            status = bloco_elem_2D->Get_Componente(k,el);
            if (!status)
            {
                tudo_certo = YES;
                switch(el.Get_Validade())
                {
                    case   1  : arq_saida = arq_temp1; break;
                    case   2  : arq_saida = arq_temp2; break;
                    case   3  : arq_saida = arq_temp3; break;
                    case   4  : arq_saida = arq_temp4; break;
//                    case   6  : arq_saida = arq_temp6; break;
//                    case   5  : arq_saida = arq_temp5; break;
                    default   : tudo_certo = NO; break;
                }
                if (tudo_certo)
                {
                    if (bloco_elem_2D->Get_Volume(k) < 0)
                     fprintf(arq_saida,"\n%10ld%10ld%10ld%10ld  %16.10g", el.Get_V2()-7, el.Get_V1()-7, el.Get_V3()-7, -el.Get_Volume());
                    else if (bloco_elem_2D->Get_Volume(k) > 0)
                     fprintf(arq_saida,"\n%10ld%10ld%10ld%10ld %16.10g", el.Get_V1()-7, el.Get_V2()-7, el.Get_V3()-7, el.Get_Volume());

                }
            }
            else
            {
                status = 112;
                if (arq_temp1) fclose(arq_temp1);
                if (arq_temp2) fclose(arq_temp2);
                if (arq_temp3) fclose(arq_temp3);
                if (arq_temp4) fclose(arq_temp4);
//                if (arq_temp6) fclose(arq_temp6);
//                if (arq_temp5) fclose(arq_temp5);
                return status;                  // <--------------- ponto de retorno
            }
        }

        if (arq_temp1) fclose(arq_temp1);
        if (arq_temp2) fclose(arq_temp2);
        if (arq_temp3) fclose(arq_temp3);
        if (arq_temp4) fclose(arq_temp4);

        // bloco de leitura de elementos 1d vulgo arquivo de pre_discretizacao





// __________________________________________________________________________

#if 1
////////////////////////////////////////////////////////////////////
        arq_saida  = fopen(arquivo,"w");
        long num_val_total = bloco_elem_2D->Get_N_Comp_LEV_VALIDs_Absoluto();
        fprintf(arq_saida,"%6d%6d%6d",0,0,1);
        fprintf(arq_saida,"\n%7ld%6d%6d",    num_val_total,1,1);
        fprintf(arq_saida,"\n%7d%6d%6d%6d", num_val_total,0,0,0);
        k = 0;
        while(k<bloco_elem_2D->Get_Total_Componentes()) {
            status = bloco_elem_2D->Get_Componente(k,el);
            if (status) {
             status =  112;
             return (status);
            }

            if(el.Get_Validade()) {
                if (bloco_elem_2D->Get_Volume(k) < 0)
                 fprintf(arq_saida,"\n%10ld%10ld%10ld%10ld  %16.10g", el.Get_V2()-7, el.Get_V1()-7, el.Get_V3()-7, -el.Get_Volume());
                else if (bloco_elem_2D->Get_Volume(k) > 0)
                 fprintf(arq_saida,"\n%10ld%10ld%10ld%10ld %16.10g", el.Get_V1()-7, el.Get_V2()-7, el.Get_V3()-7, el.Get_Volume());

            } // fim do teste de validade
            k++;
        }   // fim do loop
        fclose(arq_saida);

#endif
    } // fim do teste de arquivo aberto
    else    status = 121;
    if (arq_temp1) fclose(arq_temp1);
    if (arq_temp2) fclose(arq_temp2);
    if (arq_temp3) fclose(arq_temp3);
    if (arq_temp4) fclose(arq_temp4);
//    if (arq_temp6) fclose(arq_temp6);
//    if (arq_temp5) fclose(arq_temp5);
    return status;


} // fim de grava_elementos



// ======= fim do arquivo DGround.cpp    versao 1.00 =============================

#endif
