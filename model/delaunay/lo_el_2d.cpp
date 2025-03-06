
#include "lo_el_2d.hpp"
#include "lo_long.hpp" //*WPG* 18-11-2010: Adicionei essa classe pois nao havia nenhuma declaracao de Lo_Long.
//typedef Lo_Templ<long,long> Lo_Long;

//*WPG* 18-11-2010: Adicionei o escopo lev_enum:: em todos os enums que nao eram reconhecidos.

//===========================================================================
//               class Lista_Elementos_2D_D :: Atribui_Vizinhanca
//===========================================================================
//
//  Descricao          : recebe uma mensagem de DGround para acessar os ultimos
//                       elem_incl elementos incluidos e atualizar a  lista
//                       de vizinhos de cada elemento consistentemente. Ou seja,
//                       a) localiza para cada uma dos tetraedros novos todos os
//                          seus vizinhos, a partir do seguinte procedimento:
//                          a.1) vasculha vizinhos distantes a procura de um que
//                               compartilhe de uma face com um dos novos
//                               elementos incluidos.
//                               Se compartilhar, o inclui, desde que este ainda
//                               nao tenha sido indicado anteriormente (ja pertence
//                               a lista);
//                          a.2) para cada novo elemento incluido vasculha
//                               na lista dos outros tetraedros incluidos
//                               o(s) que eh (sao) vizinhos_proximos.
//                       b) atualiza os vizinhos dos elementos que pertencem a
//                          lista de vizinhos_distantes, com a inclusao do novo.
//
//  parametros         : vizin_dist - ponteiro para a lista de elementos que
//                                    envolvem o poliedro envolvente;
//                       num_vizin_dist - numero de elementos da lista de vizinhos
//                                    distantes;
//                       elem_incl  - numero de elementos que foram incluidos
//
//  retorno            : status da inclusao - 0 --> inclusao normal;
//
//  versao             : 3.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/07/96
//
//===========================================================================

lev_math::USINT Lista_Elementos_2D_D :: Atribui_Vizinhanca(long  *vizin_dist, lev_math::USINT num_vizin_dist, lev_math::USINT  elem_incl)
{
    long  el,ell;             // contador do laco de varredura de componentes
    long  el_novos,el_novoss; // contador para o numero de componentes novos
    lev_math::USINT el_viz   ;          // contador para o nuemro de componentes vizinhos
    lev_math::USINT status =0;          // status de saida
    lev_math::Uchar face, facen, facev; // contadores de faces
    lev_enum::LEV_BOOL elemento_completo ; // indica que o numero de vizinhos proximo eh igual ao numero de faces
    lev_enum::LEV_BOOL achou_vizinho     ; // permite interromper os lacos mais profundos da procura de faces comuns
    FACE  Face[3], FaceN[3], FaceV[3];
    Elemento_2D * Dist = NULL;

    if(num_vizin_dist) Dist = new Elemento_2D[num_vizin_dist];
    else               Dist = new Elemento_2D[1];
    if (!Dist) {
        status = 3;
        Erro(status,"Memoria disponivel insuficiente para criar objeto temporario");
    } else {
        long i;
        Elemento_2D EEE ;

        for (i = 0; i <num_vizin_dist; i++) {
            if (!Get_Componente(vizin_dist[i], EEE))
                Dist[i] = EEE;
            else {
                status = 10;
                Erro(status,"Dados corrompidos.");
            }
        }
        for (ell = num_componentes_absoluto; ell>(num_componentes_absoluto - elem_incl); ell--) {
            // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            // constroi as faces do elemento de indice el FACE[]
            // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            el = ell - 1;     // devido ao uso de unsigned int (-1)
            Face[0].V1 = ptr_pontos[el].Get_V1();
            Face[0].V2 = ptr_pontos[el].Get_V2();

            Face[1].V1 = ptr_pontos[el].Get_V2();
            Face[1].V2 = ptr_pontos[el].Get_V3();

            Face[2].V1 = ptr_pontos[el].Get_V1();
            Face[2].V2 = ptr_pontos[el].Get_V3();

            elemento_completo = lev_enum::NO;

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            // este laco realiza a busca entre os elementos proximos gerados.
            // Como, forcosamente, cada novo elemento faz fronteira com pelo
            // menos 1 dos outros novos elementos gerados, eh possivel reduzir
            // o numero de buscas na lista de elementos vizinhos_distantes.
            // esta reducao 'e feita com a verificacao do indicador
            // elemento_completo.
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            for (el_novos = el; el_novos>(num_componentes_absoluto - elem_incl); el_novos--) {
                achou_vizinho = lev_enum::NO;
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // constroi as faces do elemento de indice el_novos   FaceN[]
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                el_novoss = el_novos -1;  // devido ou uso de unsigned int

                FaceN[0].V1 = ptr_pontos[el_novoss].Get_V1();
                FaceN[0].V2 = ptr_pontos[el_novoss].Get_V2();

                FaceN[1].V1 = ptr_pontos[el_novoss].Get_V2();
                FaceN[1].V2 = ptr_pontos[el_novoss].Get_V3();

                FaceN[2].V1 = ptr_pontos[el_novoss].Get_V1();
                FaceN[2].V2 = ptr_pontos[el_novoss].Get_V3();

                for (face = 0; face<num_faces &&!achou_vizinho; face++)
                {
                 for (facen = 0; facen<num_faces &&!achou_vizinho; facen++)
                 {
                    if ((Face[face].V1 == FaceN[facen].V1) || (Face[face].V1 == FaceN[facen].V2))
                      if ((Face[face].V2 == FaceN[facen].V2) || (Face[face].V2 == FaceN[facen].V1))
                        {
                         if((status = ptr_pontos[el].Set_Vizinho(el_novoss))!=0)
                            return (status);
                         if ((status = ptr_pontos[el_novoss].Set_Vizinho(el))!=0) {
                            Erro_Continuacao(status, "Malha de elementos finitos inconsistente.segundo");
                            return (status);
                         }
                         achou_vizinho = lev_enum::YES; // visa evitar a continuacao dos lacos
                        } //elemento eh vizinho proximo
                 } // fim do laco de face dos demais novos elementos
                }   // fim do laco de face do novo elemento
            }     // fim do laco el_novos

            {
                for (el_viz = 0; el_viz < num_vizin_dist && !elemento_completo; el_viz++)
                {
                    achou_vizinho = lev_enum::NO;
                    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    // constroi as faces do elemento de indice vizin_dist[el_viz] FaceV[]
                    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                    FaceV[0].V1 = Dist[el_viz].Get_V1();
                    FaceV[0].V2 = Dist[el_viz].Get_V2();

                    FaceV[1].V1 = Dist[el_viz].Get_V2();
                    FaceV[1].V2 = Dist[el_viz].Get_V3();

                    FaceV[2].V1 = Dist[el_viz].Get_V1();
                    FaceV[2].V2 = Dist[el_viz].Get_V3();

                    for (face = 0; face<num_faces &&!achou_vizinho; face++)
                    {
                        for (facev = 0; facev<num_faces &&!achou_vizinho; facev++)
                        {
                            if ((Face[face].V1 == FaceV[facev].V1) || (Face[face].V1 == FaceV[facev].V2))
                                    if ((Face[face].V2 == FaceV[facev].V2)||(Face[face].V2 == FaceV[facev].V1))
                                    {
                                        if ((status = ptr_pontos[el].Set_Vizinho(vizin_dist[el_viz]))!=0)
                                        {
                                            Erro_Continuacao(status, "Malha de elementos finitos inconsistente.terceiro");
                                            return (status);
                                        }
                                        if ((status = Dist[el_viz].Set_Vizinho(el))!=0)
                                        {
                                            Erro_Continuacao(status, "Malha de elementos finitos inconsistente.quarto");
                                            return (status);
                                        }
                                        achou_vizinho = lev_enum::YES; // visa evitar a continuacao dos lacos
                                    } //elemento eh vizinho proximo
                        } // fim do laco de face dos vizin_dist
                    }   // fim do laco de face do novo elemento
                }     // fim do laco el_viz
            }       // fim do teste de ponteiro de vizin_dist
        }         // fim do laco ell
        for (i = 0; i <num_vizin_dist; i++) Modifica(vizin_dist[i],&Dist[i]);
        delete [] Dist; Dist = NULL;
    }
    return status;
} // fim da atribui_vizinhanca
//____________________________________________________________________________


//
//===========================================================================
//               class Lista_Elementos_2D_D :: Compacta_Lista
//===========================================================================
//
//  Descricao          : Este servico permite o reaproveitamento dos espacos
//                       vagos deixados na lista pela atribuicao de status =
//                       INVALIDO durante a geracao de elementos tetraedricos.
//                       Para tanto, faz uso de dois vetores auxiliares que
//                       armazenam a nova posicao dos elementos VALIDOS uma vez
//                       completada a compactacao da lista (auxiliar2X) e
//                       a antiga posicao dos elementos antes da compactacao
//                       da lista
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 23/08/94
//
//===========================================================================

void Lista_Elementos_2D_D :: Compacta_Lista(int forca)  {

        if (((long)(1.1*num_componentes_absoluto)) >= num_max_componentes_absoluto || forca)
        {   // a lista da memoria extendida deve ser compactada
            Elemento_2D  ELMODIF;
            Lo_Long auxiliar, auxiliar2;  // listas auxiliares de longs
            lev_math::USINT retorno1, retorno2, status;
            long  i, auxl, auxl2;
            long  novo_num;       // numero de elementos validos da lista de elementos
                                                        // deve coincidir com num_comp_validos_absoluto
            long  menos_um = -1;
            long  *viz  = NULL;   // indice dos vizinhos do componente que esta sendo deslocado
            lev_math::USINT n_viz;          // numero de vizinhos de componente que esta sendo deslocado
            lev_math::USINT j;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// dimensiona os blocos de memoria extendida que contem os vetores
// auxiliares de compactacao
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            auxiliar .Set_N_Max_Com_Abs(num_comp_validos_absoluto);
            auxiliar2.Set_N_Max_Com_Abs(num_max_componentes_absoluto);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//   MONTA OS VETORES AUXILIARES PARA COMPACTACAO DA LISTA
//  auxiliar  : armazena o indice do elemento i, se este for valido. Nada armazena se o elemento for invalido;
//  auxiliar2 :armazena -1 se o elemento � n�o valido e a nova posi��o na lista se o elemento for v�lido;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            for (i = 0, novo_num = 0; i < num_componentes_absoluto; i++) {
                 if (ptr_pontos[i].Get_Validade()) {
                     retorno1 = auxiliar.Inclui(i);
                     if (retorno1){}  // *AP*
                     retorno2 = auxiliar2.Inclui(novo_num);
                     if (retorno2) {} // *AP*
                     novo_num++;
                 }
                 else {
                     retorno2 = auxiliar2.Inclui(menos_um);
                     if (retorno2) {} //*AP*
                 }
            } // fim do primeiro laco i

#if 10

            if (novo_num != num_comp_validos_absoluto) {
                status = 23;
                Erro(status,"Dados Inconsistentes.");
            }
#endif
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  COPIA UM COMPONENTE PARA SUA NOVA POSICAO NA LISTA
//  auxl - indice velho, antes da compactacao, que vai para a nova posicao
//  auxl2- indice novo, assumido apos a compactacao
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            for (i = 0; i < novo_num; i++) {
                auxl = auxiliar[i];
                ELMODIF = ptr_pontos[auxl];
                if (i != auxl) {
                    ptr_pontos[auxl].Set_Validade(LEV_INVALID);
                    ptr_pontos[auxl].Num_Vizinhos = 0;
                }
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  MODIFICA OS INDICES DOS VIZINHOS COM BASE EM AUXILIAR2
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                viz   = ELMODIF.Get_Vizinhos();
                n_viz = ELMODIF.Get_N_Vizinhos();
                ELMODIF.Num_Vizinhos = 0; // necessario para redefinicao de vizinhos em SET_VIZINHO
                for (j = 0; j < n_viz; j++) {
                    auxl2 = auxiliar2[viz[j]];
                    if ((status = ELMODIF.Set_Vizinho(auxl2))!=0)
                    {
                        Erro(status, "Malha de elementos finitos inconsistente.");
                    }
                }  // fim do laco j
                Modifica(i,&ELMODIF);
            } // FIM DO LACO i

            num_componentes_absoluto = novo_num;

        } // fim do teste de disponibilidade na memoria extendida

}   // fim do servico compacta_lista
//____________________________________________________________________________


//===========================================================================
//               class Lista_Elementos_2D_D :: Elimina
//===========================================================================
//
//  Descricao          : Marca um elemento como nao valido e elimina
//                       tambem o numero do elemento da vizinhanca de elementos
//                       ainda validos. Atualiza o numero de componentes validos.
//                       (A eliminacao dos dados  do objeto elemento sao
//                       feitas diretamente, utilizando a caracteristica de
//                       friendship que esta classe tem com a de elementos.
//
//
//  parametros         : indice do Elemento_2D a ser eliminado
//  versao             : 2.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/07/96
//
//===========================================================================
void Lista_Elementos_2D_D :: Elimina(long  indice)
{
     lev_math::USINT i, status;                           // contador
     long  *ptr_vizinhos = NULL ;
     Elemento_2D  eee   ;        // auxiliar
     Elemento_2D  el, el_aux[3]; // el=copia do elemento a ser eliminado
     if (!Get_Componente(indice, el))
     {
        if (el.Get_Validade())
        {   // elemento valido
            ptr_vizinhos = el.Get_Vizinhos();
            if (ptr_vizinhos)
            {
                for (i = 0; i < el.Get_N_Vizinhos(); i++)
                {
                 if (!Get_Componente(ptr_vizinhos[i],el_aux[i]))
                 {
#if 1 // *AP* debug
                     if (!el_aux[i].Get_Validade())
                     {
                            status = 21;
#    if defined INTERFACE_GRAFICA
                            Msg2.Display("Erro 21: Vizinhos inconsistentes. Execucao interrompida...",BRANCO,AZUL);
                            exit (status);
#    else
                            Erro(status,"Erro 21: Vizinhos inconsistentes. Execucao interrompida...");
#    endif
                     }
#endif
                     status = el_aux[i].Elimina_Vizinhanca(indice);
                     if (status)
                     {
                         char mens[50];
                         sprintf(mens,"ERRO %d : Dados Inconsistentes. Execucao interrompida...",status);
                         Erro(status,mens);
                     }
                 }
                 else
                 {
                     status = 11;
#    if defined INTERFACE_GRAFICA
                     Msg2.Display("Erro 11: Dados Inconsistentes. Execucao interrompida...",BRANCO,AZUL);
                     exit(status);
#    else
                    Erro(status,"Erro 11: Dados Inconsistentes. Execucao interrompida...");
#    endif
                 }
                } // fim do laco i

                num_comp_validos_absoluto --;

                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // MODIFICA CADA UM DOS ELEMENTOS ENVOLVIDOS, APOS DEFINICAO DA VIZINHANCA
                // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                for (i = 0; i <el.Get_N_Vizinhos(); i++)
                    Modifica(ptr_vizinhos[i],&(el_aux[i]));

                el.Set_Validade(LEV_INVALID);
                el.Num_Vizinhos = 0;
                Modifica(indice, &el);

            } // fim do teste de ptr_vizinhos
            else
            {
                status = 11;
#    if defined INTERFACE_GRAFICA
                Msg2.Display("Erro 11: Dados inconsistentes. Execucao interrompida...",BRANCO,AZUL);
                exit (status);
#    else
                Erro(status,"Erro 11: Dados inconsistentes. Execucao interrompida...");
#    endif
            }
        } // fim do teste de validade de el
        else
        {
            status = 18;
#    if defined INTERFACE_GRAFICA
            Msg2.Display("Erro 18: Dados inconsistentes. Execucao interrompida...",BRANCO,AZUL);
            exit (status);
#    else
            Erro(status, "Erro 18: Dados inconsistentes. Execucao interrompida...");
#    endif
        }
     } // fim do teste de el encontrado
     else
     {
            status = 11;
#    if defined INTERFACE_GRAFICA
            Msg2.Display("Erro 11: Dados inconsistentes. Execucao interrompida...",BRANCO,AZUL);
            exit (status);
#    else
            Erro(status, "Erro 11: Dados inconsistentes. Execucao interrompida...");
#    endif
     }
} // fim do metodo Elimina ==================================================
//____________________________________________________________________________

//===========================================================================
//               class Lista_Elementos_2D_D :: Inclui
//===========================================================================
//
//  Descricao          : Registra os dados de um novo elemento 3D na lista
//                       (faz a chamada aa funcao que calcula os parametros
//                       do elemento);
//
//  Observacao         : Encarrega-se de Verificar o preenchimento da lista.
//                       Como medida de seguranca a transferencia para XMS eh
//                       efetuada quando o numero de componentes aproxima-se
//                       de menos de 10 elementos do limite maximo.
//                       Aconselha-se o uso do servico de compactacao do
//                       buffer de memoria convencional antes de chamar
//                       este servico quando esse buffer tiver um numero de
//                       elementos validos muito menor que o numero atual
//                       de componentes.
//
//  par�metros         : ponteiro para a lista de pontos atual (para calculo
//                       da circunsfera) e o �ndice dos quatro v�rtices do
//                       tetraedro (p1,p2,p3,p4).
//
//  retorno            : codigo de erro.
//                             0  inclusao normal
//                            15  limite de elementos alcancado
//                            20  elemento nao incluido (volume igual a zero)
//  versao             : 3.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/07/96
//
//===========================================================================

//Uchar Lista_Elementos_2D_D :: Inclui (Lo_Pt_Malha  *pt,long  p1, long  p2, long  p3)//, long  p4)
lev_math::Uchar Lista_Elementos_2D_D :: Inclui (vCLevPtMalha &pt,long  p1, long  p2, long  p3)//, long  p4)
{
    lev_math::Uchar retorno = 0;
    if ((num_componentes_absoluto) >= num_max_componentes_absoluto)
        retorno = Change_N_Max_Comp_Abs(num_componentes_absoluto+10000); //*NMA*:01/07/2004

    if (!retorno)
    {
        long  vert[3] = {p1,p2,p3};//,p4;
        ptr_pontos[num_componentes_absoluto].Set_Vertices(vert);
        ptr_pontos[num_componentes_absoluto].Set_Validade(LEV_VALID);
        ptr_pontos[num_componentes_absoluto].Num_Vizinhos = 0;
        ptr_pontos[num_componentes_absoluto].Calc_Circuns(pt);

        if(ptr_pontos[num_componentes_absoluto].Get_Volume())
        {
            num_componentes_absoluto++; num_comp_validos_absoluto++;
//------------------------------------------------------------------------------------
#if 0

        //*GMM* trecho ideal para monitorar a inclus�o de elementos na malha
        // Aqui ser� feita a grava��o em arquivo de cada elemento incluido na malha
        // APENAS PARA DEBUG
        FILE  *p_trn = NULL;// ponteiro para arquivo que conter� os pontos
        p_trn = fopen("bloco_elem_2D.txt", "a");
        long q;
        q = fprintf(p_trn,"%d %d %d %d %15.8lg\n", num_comp_validos_absoluto,
            p1, p2, p3,    ptr_pontos[num_componentes_absoluto-1].Get_Volume());
        if (q==EOF) {printf("Houve erro na grava�ao do arquivo bloco_elem_2D"); getch();}
        fclose(p_trn);
#endif
//------------------------------------------------------------------------------------
        }
        else { retorno = 20;}
    }
    else retorno = 15;//*NMA*: 01/07/2004
    return retorno;
}   // fim do servi�o Inclui
//____________________________________________________________________________

//===========================================================================
//               class Lista_Elementos_2D_D :: Localiza_Vizinhos
//===========================================================================
//
//  Descricao          : Dado um elemento da lista localiza todos os demais
//                       elementos que sao seus vizinhos.
//
//  parametros         : ind_elem_elim - ponteiro para um vetor contendo o
//                                       indice dos elementos a serem
//                                       eliminados
//                       num_elem_elim - ponteiro para o numero de elementos
//                                       do vetor ind_elem_elim  (necessaria-
//                                       mente < MAXELEMELIM)
//                       vizinhos_distantes - ponteiro para a lista de indices
//                                       de elementos nao eliminados mas vizinhos
//                                       do poliedro envolvente
//                       num_vizinhos_distantes - numero de elementos da lista
//                                       vizinhos distantes
//                       atual         - ponteiro para o objeto Pt_Malha que esta
//                                       sendo incluido
//
//  retorno            : status de termino de localizacao:
//                              0   OK
//                             30   localizado um caso degenerado
//                       Existem tres pontos de retorno
//
//  versao             : 3.0
//  programador        : Angelo Passaro
//  ultima modificacao : 01/07/96
//
//===========================================================================
// *RYT* 2015-09-03
// SIGMA was global in dgr15_10_2d
lev_math::USINT Lista_Elementos_2D_D :: Localiza_Vizinhos (long  *ind_elem_elim,
                 lev_math::USINT * num_elem_elim,
                 long  * vizinhos_distantes,
                 lev_math::USINT * num_vizinhos_distantes,
                 Pt_Malha * atual, double SIGMA)
/*
USINT Lista_Elementos_2D_D :: Localiza_Vizinhos (Lo_Long &ind_elem_elim,
//                 USINT * num_elem_elim,
                 Lo_Long & vizinhos_distantes,
//                 USINT * num_vizinhos_distantes,
                 Pt_Malha * atual)
*/
{
 double aux, raio2,faixa, dquadrado;  // x2+y2
 lev_math::USINT   status, n;
 lev_math::USINT k, kk, retorno = 0, indice_inicial, indice_final;
// Uchar continua, continua_procura;
 lev_enum::LEV_BOOL continua, continua_procura;
 long  indice = 0;
 Elemento_2D elem_atual , elem_viz;//<<aqui os vizinhos s�o 0 e 0 <<-----********
 continua_procura = lev_enum::YES;
 indice_inicial = indice_final = 0;

 while(continua_procura)
 {
    for (n = indice_inicial; n <= indice_final; n++)
    {
        status = Get_Componente(ind_elem_elim[n], elem_atual); // copia do elemento
        if (status)
        {
            lev_math::USINT status = 6;
            Erro(status,"Erro 6: Dados inconsistentes. Execucao interrompida...");
        }
        if (elem_atual.Estado == LEV_VALID)
        {
            dquadrado = atual->X() * atual->X() +
                                    atual->Y() * atual->Y();

            for (k = 0; k < elem_atual.Get_N_Vizinhos(); k++)
            {
                indice = elem_atual.Vizinhos[k];
                //___________________________________________________________________
                // primeiramente, verifica se o vizinho [k] ja esta incluido na lista
                // de elementos eliminados...
                //___________________________________________________________________
                continua = lev_enum::YES;
                for (kk = 0; kk <*num_elem_elim; kk++)
//                for (kk = 0; kk <ind_elem_elim.Get_Total_Componentes(); kk++)
                {
                    if (indice == ind_elem_elim[kk])
                    {
                        continua= lev_enum::NO;
                        break;  // quebra o loop kk (teste de repeticao do elemento eliminado
                    }
                } // fim do laco kk
                if (continua)
                {
                    status = Get_Componente(indice, elem_viz);
                    if (status)
                    {
                        lev_math::USINT status = 6;
                        Erro(status,"Erro 6: Dados inconsistentes. Execucao interrompida...");
                    }

                    if (elem_viz.Estado == LEV_VALID)
                    {
                        aux = dquadrado + elem_viz.Get_C0()*atual->X()+
                                                            elem_viz.Get_C1()*atual->Y()+
                                                            elem_viz.Get_C2();  // eq. c�culo

                        raio2= (elem_viz.Get_C0()*elem_viz.Get_C0()  +
                                elem_viz.Get_C1()*elem_viz.Get_C1())*.25  - elem_viz.Get_C2();//idem

                        faixa = SIGMA*raio2;

                        if (aux > faixa && aux < -faixa)
                        { retorno = 30;
                            ind_elem_elim[(*num_elem_elim)++] = indice;
//                                ind_elem_elim.Inclui(indice);
                            return retorno;                              // <------------------------------
                        } //sinaliza caso degenerado

                        if (aux < faixa )
                            ind_elem_elim[(*num_elem_elim)++] = indice;
//                                ind_elem_elim.Inclui(indice);

                        else
                        {
                            continua = lev_enum::YES;
                            for (kk = 0; kk <*num_vizinhos_distantes; kk++)
//                            for (kk = 0; kk <vizinhos_distantes.Get_Total_Componentes(); kk++)
                            {
                                if (indice == vizinhos_distantes[kk])
                                {
                                    continua= lev_enum::NO;
                                    break;  // quebra o loop kk (teste de repeticao do elemento eliminado
                                }
                            }
                            if (continua)
                                vizinhos_distantes[(*num_vizinhos_distantes)++] = indice;
//                                    vizinhos_distantes.Inclui(indice);
                        } // fim do else de teste da circunsfera

                    } // fim do teste de validade do elemento de elem_viz ----------

                    else
                    {
                        status = 17;
                        Erro(status,"Erro 17: Dados corrompidos. Execucao interrompida...");
                    }  // fim do else de elemento vizinho nao valido

                } //fim do teste de elemento ja incluido


            } // fim do for k ---------------------------------------------------
        }   // fim do teste de validade do elemento de entrada elem_atual
        else
        {
            status =  19;
            Erro(status,"Erro 19: Dados inconsistentes. Execucao interrompida...");
        }
    } // fim do laco for (n)
    if (indice_final == (*num_elem_elim-1))        continua_procura = lev_enum::NO;
//    if (indice_final == (ind_elem_elim.Get_Total_Componentes()-1))        continua_procura = NO;
    else
    {
        indice_inicial =  indice_final  + 1;
        indice_final   = *num_elem_elim - 1;
//        indice_final   = ind_elem_elim.Get_Total_Componentes() - 1;
    }

 } // fim do laco while(continua_procura)

 return retorno;
} // fim do metodo Localiza_Vizinhos ========================================
//____________________________________________________________________________



