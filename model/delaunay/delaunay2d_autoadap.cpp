#include <delaunay2d_autoadap.hpp>
#include <algorithm>


extern "C"
{
    void build_fill_and_solve_generic_system(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
             double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
             int *NO_CC,double *POT_CC, int *NO_CA_REF, int *NO_CA_POT,int *TIPO,
             int *NO_CF_REF, int *NO_CF_POT,int *FONTE_EL,double *FONTE_VAL,
             int *mat_lin, int *itn_max, int *itc_max, Delaunay2D_AutoAdaptativo::materiais_struct *mat, int *tipo_estudo,
             double *MC, int *COL, int *DIAGONAL, double *RES, int *nos_alocados, double *precisao,
             double *A);
}
// ========================================================
// CONSTRUTORES E DESTRUIDOR
// ========================================================


Delaunay2D_AutoAdaptativo :: Delaunay2D_AutoAdaptativo(void)
{
    Erro_Global_Malha.Media = 1e308;
    Erro_Global_Malha.Desvio_Padrao = 0;
    Erro_Global_Malha.Limite = 1.e-5;
}


Delaunay2D_AutoAdaptativo :: ~Delaunay2D_AutoAdaptativo (void)
{ }



// ============================================================================
//       USINT Delaunay2D_AutoAdaptativo :: Refina(void)
// ========================================================
//
//  Descricao          : Refina a malha a partir do crit�rio de  gradiente  de
//                       campo
//
//
//  parametros         : nunhum
//
//  valor de retorno   : Erro de execu��o
//                       0 - execu��o normal
//                       13001 a 1300??? aloca��o de mem�ria
//
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalh�es, Nancy Mieko Abe e Angelo Passaro
//  ultima modificacao : 09/05/2001
//
//=============================================================================
// Algoritmo Simplificado
    // Enquanto o Limite � menor que o erro m�dio da malha

        // monta os arrays de comunica��o com o Solver
        // aciona a montagem e resolu��o do sistema de equa��es
        // Calcula campo el�trico nos elementos -> atribui a elementos
        // Calcula campo el�trico nos n�s -> atribui a pontos
        // Calcula a derivada do campo el�trico nos elementos
        //        (o mesmo que o calculo de campo nos elementos)- Derivada segunda
        // Calcula erro em cada elemento
        // Calcula Erro M�dio, e desvio padr�o, da Malha
        // Se  o Limite � menor que o erro m�dio da malha
            // Define a nova nuvem de pontos ( compara o erro em cada elemento com
            //              o erro m�dio da malha )
            // Inclui na malha os novos pontos (Discretiza)
        // senao : Encerra execu��o

//=============================================================================
#define _AP_DEBUG_


lev_math::USINT Delaunay2D_AutoAdaptativo :: Refina(int metodo)
{
    lev_math::USINT err = 0; // sem erro

    if(metodo != SELF_ADAPTIVE)
        err = Delaunay2D::Refina(metodo);
    else {

        const double FATOR_QUALIDADE = 0.1; // para verificar qualidade do elemento

        FILE  *p_iteracoes = NULL;// ponteiro para arquivo que conter� informacoes em cada itera�ao
        p_iteracoes = fopen("iteracoes.txt", "w+");
        fprintf(p_iteracoes,"fator de qualidade: %g", FATOR_QUALIDADE);
        fprintf(p_iteracoes,"\nerro malha limite: %g",     Erro_Global_Malha.Limite);
        fclose(p_iteracoes);

        //======================================================
        // *AP*    so para efeito de DEBUG de pontos e elementos
        //======================================================
        #ifdef _AP_DEBUG_
        FILE *p_pontos = NULL; // ponteiro para arquivo que conter� os pontos
        FILE *p_elem = NULL;   // ponteiro para arquivo que contem dados de elementos
        p_pontos = fopen("DEBUG_potenciais_pts.txt", "w+");
        p_elem = fopen("DEBUG_elementos.txt", "w+");
        fclose(p_pontos);
        fclose(p_elem);
        long * viz;
        #endif // _AP_DEBUG

        Elemento_2D    triangulo;
        Pt_Malha o_ponto, ponto_aux;

        lev_math::USINT err = 0; // sem erro
		int     continua         = true,
				dividiu_aresta   = false;
		double  area             = 0.0;
        long int vertices[10];
        int      i, j;
        int         nos_alocados;

        double *x            = NULL;
        double *y            = NULL;
        int    *E1          = NULL;
        int    *E2          = NULL;
        int    *E3          = NULL;
        int    *MEIO        = NULL;
        int    *FONTE_EL    = NULL;
        double *FONTE_VAL   = NULL;
        int    *NO_CC       = NULL;
        double *POT_CC      = NULL;
        int    *NO_CA_REF   = NULL;
        int    *NO_CA_POT   = NULL;
        int    *TIPO        = NULL;
        int    *NO_CF_REF   = NULL;
        int    *NO_CF_POT   = NULL;
        double *angulo      = NULL;
        double *MC            = NULL;
        int    *COL            = NULL;
        int    *DIAGONAL    = NULL;
        double *RES            = NULL;
        double *A            = NULL;
        double *DA          = NULL;
        int     NE            = 0;
        int     NN            = 0;
        int     NF            = 0;
        int     NCA            = 0;
        int     NCF            = 0;
        int     NC            = 0;
        double    b[4];
        double    c[4];
        int        E[4];
        double    delta        =    0.0;
        double    Field        =    0.0;
        double    Field1        =    0.0;
        double    Field2        =    0.0;
        double    Ex, Ey, Bx, By;

        /*GMM*/
        Aresta_Geometria aresta, aresta1;
        erro erro_iteracao;    // calcula os dados relacionados ao erro de cada iteracao
        Pt_Malha         vert_trn[3];
        long             ind_aresta;
        double            rinsc = 0.0;
        double            rcirc = 0.0;
        double            qualidade = 0.0;
        vCLevPtMalha    nuvem_pontos_ref;

        // ============================= dados obtidos dos arquivos ===============================
        int        tipo_estudo = 1;    // indica tipo de estudo 1-eletrostatica 2-eletrocinetica 3-magnetostica
        int     mat_lin     = 0;    // material linear = 0; material n_linear  > 0
        //=========================================================================================

        // ============================= dados para resolu��o do sistema ==========================
        int        itc_max = 100;
        int        itn_max = 100;
        double    precisao_NR = 1e-5;
        //=========================================================================================

        // a malha deve chegar compactada neste ponto
        bloco_elem_2D->Compacta_Lista(1);

        // verifica a existencia de material n�o linear  *AP*  por enquanto independente do tipo de estudo

        for(i=0; i<bloco_elem_2D->Get_Total_Componentes() && continua; i++) {
            bloco_elem_2D->Get_Componente(i,triangulo);
            if(triangulo.Get_Material() > 200) {  // verifica se material magnetico
                for(j=0; j<mat->NM; j++) {
                    if(mat->codigos[j] == triangulo.Get_Material()) {
                        if(mat->materiais[j][2]!=0.0 && mat->materiais[j][3]!=0.0) {
                            mat_lin =1;
                            continua = false;
                        }
                        break;
                    }
                }
            }
        }
        continua = true;

        const int MAX_MALHAS = 9; // *AP* limita o processo iterativo

        int contador=0; // contador para limitacao do laco auto-adaptativo deve ser removido apos os testes

        while(continua) {
            contador++;

            NE = bloco_elem_2D->Get_Total_Componentes();
            NN = bloco_pts.size();
            NF    = 0;
            NCA    = 0;
            NCF    = 0;
            NC    = 0;

            nos_alocados = (5*NN+2);

            //==================================alocacao de memoria ====================================
            //==========================================================================================
            //================================== libera��o de memoria ====================================
            //==========================================================================================
            if(x)            {free(x);            x = NULL;}
            if(y)            {free(y);            y = NULL;}
            if(E1)            {free(E1);            E1 = NULL;}
            if(E2)            {free(E2);            E2 = NULL;}
            if(E3)            {free(E3);            E3 = NULL;}
            if(MEIO)        {free(MEIO);        MEIO = NULL;}
            if(FONTE_EL)    {free(FONTE_EL);    FONTE_EL = NULL;}
            if(FONTE_VAL)   {free(FONTE_VAL);    FONTE_VAL  = NULL;}
            if(NO_CC)       {free(NO_CC);        NO_CC = NULL;}
            if(POT_CC)      {free(POT_CC);        POT_CC = NULL;}
            if(NO_CA_REF)   {free(NO_CA_REF);    NO_CA_REF = NULL;} //guarda os nos referencia dos nos com cc peri�dica
            if(NO_CA_POT)   {free(NO_CA_POT);    NO_CA_POT = NULL;} //guarda os nos com cc peri�dica
            if(TIPO)        {free(TIPO);        TIPO = NULL;}
            if(NO_CF_REF)   {free(NO_CF_REF);    NO_CF_REF = NULL;} //guarda os nos referencia dos nos com cc float
            if(NO_CF_POT)   {free(NO_CF_POT);    NO_CF_POT = NULL;} //guarda os nos com cc float
            if(angulo)      {free(angulo);        angulo = NULL;} //angulo para imas permanentes
            if(MC)            {free(MC);            MC = NULL;}
            if(COL)            {free(COL);            COL = NULL;}
            if(DIAGONAL)    {free(DIAGONAL);    DIAGONAL = NULL;}
            if(RES)            {free(RES);            RES = NULL;}
            if(A)            {free(A);            A = NULL;}
            if(DA)            {free(DA);            DA = NULL;}
            //==========================================================================================
            x = (double *)malloc( (NN+2) * sizeof(double) );
            if(!x) {
                printf("\nerro na alocacao de memoria - vetor de coordenadas x");
                err = 13001;
            }

            y = (double *)malloc( (NN+2) * sizeof(double) );

            if(!y) {
                printf("\nerro na alocacao de memoria - vetor de coordenadas y");
                err = 13002;
            }

            E1 = (int *) malloc( (NE+3) * sizeof(int) );

            if(!E1) {
                printf("\nerro na alocacao de memoria - vetor de vertice E1");
                err = 13003;
            }

            E2 = (int *) malloc( (NE+3) * sizeof(int) );

            if(!E2) {
                printf("\nerro na alocacao de memoria - vetor de vertice E2");
                err = 13004;
            }

            E3 = (int *) malloc( (NE+3) * sizeof(int) );

            if(!E3) {
                printf("\nerro na alocacao de memoria - vetor de vertice E3");
                err = 13005;
            }

            MEIO = (int *) malloc( (NE+3) * sizeof(int) );

            if(!MEIO) {
                printf("\nerro na alocacao de memoria - vetor de vertice MEIO");
                err = 13006;
            }

            NO_CA_REF = (int *) malloc( (NN+2) * sizeof(int) );

            if(!NO_CA_REF) {
                printf("\nerro na alocacao de memoria - vetor  NO_CA_REF");
                err = 13007;
            }

            NO_CA_POT = (int *) malloc( (NN+2) * sizeof(int) );

            if(!NO_CA_POT) {
                printf("\nerro na alocacao de memoria - vetor  NO_CA_POT");
                err = 13008;
            }

            TIPO = (int *)malloc( (NN+2) * sizeof(int) );

            if(!TIPO) {
                printf("\nerro na alocacao de memoria - vetor TIPO");
                err = 13009;
            }

            NO_CF_REF = (int *) malloc( (NN+2) * sizeof(int) );

            if(!NO_CF_REF) {
                printf("\nerro na alocacao de memoria - vetor  NO_CF_REF");
                err = 13010;
            }

            NO_CF_POT = (int *) malloc( (NN+2) * sizeof(int) );

            if(!NO_CF_POT) {
                printf("\nerro na alocacao de memoria - vetor  NO_CF_POT");
                err = 13011;
            }

            NO_CC = (int *) malloc( (NN+2) * sizeof(int) );

            if(!NO_CC) {
                printf("\nerro na alocacao de memoria - vetor  NO_CC");
                err = 13012;
            }

            POT_CC = (double *) malloc( (NN+2) * sizeof(double) );

            if(!POT_CC) {
                printf("\nerro na alocacao de memoria - vetor  POT_CC");
                err = 13013;
            }

            FONTE_EL = (int *) malloc( (NE+3) * sizeof(int) );

            if(!FONTE_EL) {
                printf("\nerro na alocacao de memoria - vetor FONTE_EL");
                err = 13014;
            }

            FONTE_VAL = (double *) malloc( (NE+3) * sizeof(double) );

            if(!FONTE_VAL) {
                printf("\nerro na alocacao de memoria - vetor  FONTE_VAL");
                err = 13015;
            }

            angulo = (double *) malloc( (NE+3) * sizeof(double) );

            if(!angulo) {
                printf("\nerro na alocacao de memoria - vetor angulo");
                err = 13016;
            }

            MC = (double *)malloc( (5*NN+2) * sizeof(double) );

            if(!MC) {
                printf("\nerro na alocacao de memoria - vetor MC");
                err = 13017;
            }

            COL = (int *)malloc( (5*NN+2) * sizeof(int) );

            if(!COL) {
                printf("\nerro na alocacao de memoria - vetor COL");
                err = 13018;
            }

            DIAGONAL = (int *)malloc( (NN+2) * sizeof(int) );

            if(!DIAGONAL) {
                printf("\nerro na alocacao de memoria - vetor DIAGONAL");
                err = 13019;
            }

            RES = (double *)malloc( (NN+2) * sizeof(double) );

            if(!RES) {
                printf("\nerro na alocacao de memoria - vetor de a��es");
                err = 13020;
            }

            A = (double *)calloc( (NN+2) , sizeof(double) );

            if(!A) {
                printf("\nerro na alocacao de memoria - vetor A");
                err = 13021;
            }

            // alocar  o vetor DA (derivada do potencial no ponto) e o vetor DE_field
            DA = (double *)calloc( (NN+2) , sizeof(double) );

            if(!DA) {
                printf("\nerro na alocacao de memoria - vetor DA");
                err = 13022;
            }

            //==============================================================
            // =============== monta arrays de comunica��o com o Solver=====
            //==============================================================
            // preenche vetores
            //    E1 : vertice 1 do elemento
            //      E2 : vertice 2 do elemento
            //      E3 : vertice 3 do elemento

            for(i=1; i<=NE; i++) {
                bloco_elem_2D->Get_Componente(i-1, triangulo);

                // ==============  monta arrays de vertices de triangulos ======
                triangulo.Get_Vertices(vertices);
                E1[i] = vertices[0]+1; E2[i] = vertices[1]+1; E3[i] = vertices[2]+1;

                // ==============  monta arrays de fontes e materiais ==========
                MEIO[i]   = triangulo.Get_Material();
                angulo[i] = triangulo.Get_Angulo();

                if (triangulo.Get_Fonte()) {
                    FONTE_EL[++NF] = i;
                    FONTE_VAL[NF]  = triangulo.Get_Fonte();
                }
            }

            for(i=1; i<=NN; i++) {
                o_ponto = bloco_pts[i-1];
                x[i] = o_ponto.X(); y[i] = o_ponto. Y();
                if(o_ponto.Get_TipoCC()) {
                    switch(o_ponto.Get_TipoCC()) {
                        case 1:
                            break;
                        case 2:
                            NO_CC[++NC]      = i;
                            POT_CC[NC]       = o_ponto.Get_ValorCC();
                            break;
                        case 3:
                            NO_CF_POT[++NCF] = i;
                            NO_CF_REF[NCF]   = (int) o_ponto.Get_ValorCC();
                            break;
                        case 4:
                        case 5:
                            NO_CA_POT[++NCA] = i;
                            NO_CA_REF[NCA] = (int) o_ponto.Get_ValorCC();
                            TIPO[NCA] = o_ponto.Get_TipoCC();
                            break;
                    }
                }
            }

            //==================================================================
            // ===============      Monta e resolve sistema de equa��es    =====
            //==================================================================
            build_fill_and_solve_generic_system(&NN,&NE,&NCA,&NCF,&NC,&NF,
                    x, y, E1,E2,E3,MEIO, angulo,
                    NO_CC, POT_CC, NO_CA_REF, NO_CA_POT,TIPO,
                    NO_CF_REF, NO_CF_POT,FONTE_EL, FONTE_VAL,
                    &mat_lin, &itn_max, &itc_max, mat, &tipo_estudo,
                    MC, COL, DIAGONAL,  RES, &nos_alocados, &precisao_NR, A);

            //==================================================================
            // ================ atribui potencial calculado aos nos ============
            // =================================================================
            for(i=1; i<=NN; i++)
                bloco_pts[i-1].Set_Potencial(A[i]);

            //==================================================================
            // ===============      Calcula campo el�trico ou magn�tico    =====
            //==================================================================
            for(i=1; i<=NE; i++) {
                bloco_elem_2D->Get_Componente(i-1, triangulo);

                E[1] = E1[i];
                E[2] = E2[i];
                E[3] = E3[i];

                b[1] = y[ E2[i] ] - y[ E3[i] ];
                b[2] = y[ E3[i] ] - y[ E1[i] ];
                b[3] = y[ E1[i] ] - y[ E2[i] ];

                c[1] = x[ E3[i] ] - x[ E2[i] ];
                c[2] = x[ E1[i] ] - x[ E3[i] ];
                c[3] = x[ E2[i] ] - x[ E1[i] ];

                delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );

                // aten��o aos sinais e componentes (vide switch do tipo de estudo
                Field1 = - (b[1]*A[E[1]] + b[2]*A[E[2]] + b[3]*A[E[3]])/(2*delta);
                Field2 = - (c[1]*A[E[1]] + c[2]*A[E[2]] + c[3]*A[E[3]])/(2*delta);

                switch(tipo_estudo) {
                    case 1: // eletrost�tico
                        Ex= Field1; Ey = Field2;
                        Field = sqrt(Ex*Ex + Ey*Ey);
                        break;
                    case 2:
                        Ex= Field1; Ey = Field2;
                        Field = sqrt(Ex*Ex + Ey*Ey);
                        break;
                    case 3:
                        Bx = - Field2; By = Field1;
                        Field = sqrt(Bx*Bx + By*By);
                        break;
                    default:
                        break;
                }

                // Preenche os objetos com os dados calculados
                triangulo.Set_Campo(Field);
                bloco_elem_2D->Modifica(i-1, triangulo);
            }

            //==================================================================
            // ===============      Calcula campo nos nos    ===================
            //==================================================================

            Calc_CampoNos();

            //==================================================================
            // ===============      Calcula primeira derivada do campo    ======
            //==================================================================
            // construir o vetor DA (derivada do potencial no ponto) e o vetoor DE_field
            //compute_electric_field_2d_internal (NE, prop_fis, E1, E2, E3, x, y, DA, DE_field);

            // alterar para considerar somente a derivada segunda ( o que independe do estudo)
            // e escolher o maior entre Field1 e Field2 para economizar contas.

            Pt_Malha o_pv;

            for(i=1; i<=NE; i++) {
                bloco_elem_2D->Get_Componente(i-1, triangulo);

                E[1] = E1[i];
                E[2] = E2[i];
                E[3] = E3[i];

                b[1] = y[ E2[i] ] - y[ E3[i] ];
                b[2] = y[ E3[i] ] - y[ E1[i] ];
                b[3] = y[ E1[i] ] - y[ E2[i] ];

                c[1] = x[ E3[i] ] - x[ E2[i] ];
                c[2] = x[ E1[i] ] - x[ E3[i] ];
                c[3] = x[ E2[i] ] - x[ E1[i] ];

                delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );

                short j, posicao_array;

                for(j=1, Field1 = Field2 = 0; j<=triangulo.Get_N_Vertices(); j++) {
                    o_pv = bloco_pts[E[j]-1];
                    posicao_array = o_pv.Esta_Associado(triangulo.Get_Material());
                    if (posicao_array >= 0) {
                        Field1 +=  b[j] * o_pv.Get_DerivadaPrim(posicao_array);
                        Field2 +=  c[j] * o_pv.Get_DerivadaPrim(posicao_array);
                    } else {
                        std::cout << "Erro: Material do elemento n�o esta associado a um dos pontos vertices do elemento (Refina)";
                        exit (i);
                    }
                }
                Field1 /= (2*delta);
                Field2 /= (2*delta);

                if (Field1 >= Field2)
                    Field = Field1;
                else
                    Field = Field2;

                triangulo.Set_DerivadaPrim(Field);
                bloco_elem_2D->Modifica(i-1, triangulo);
            }

            //==================================================================
            // ========================      Calcula erros    ==================
            //==================================================================
            Calc_Erro_Elementos(erro_iteracao); // Calcula erro em cada elemento

            if(contador == 1) {
                Erro_Global_Malha.Maior = erro_iteracao.Maior;
                Erro_Global_Malha.Menor = erro_iteracao.Menor; //*NMA*
            }

            Erro_Global_Malha.Media         = erro_iteracao.Media         ;
            Erro_Global_Malha.Desvio_Padrao = erro_iteracao.Desvio_Padrao ;

            printf("\n\n\nIteracao: %d\n", contador);
            printf("\nO erro medio da malha foi: %g  ", Erro_Global_Malha.Media );
            printf("\nO erro medio normalizado da malha foi: %g ", Erro_Global_Malha.Media/ Erro_Global_Malha.Maior);
            printf("\nO erro maximo da malha foi: %g",Erro_Global_Malha.Maior);
            printf("\nO desvio padrao da malha foi: %g",Erro_Global_Malha.Desvio_Padrao);
            printf("\nO erro limite da malha foi: %g",Erro_Global_Malha.Limite);
            printf("\nO erro minimo da malha foi: %g",Erro_Global_Malha.Menor); //*NMA*

            // ponteiro para arquivo que conter� informacoes sobre iteracoes
            p_iteracoes = NULL;
            p_iteracoes = fopen("iteracoes.txt", "a");
            fprintf(p_iteracoes,"\n\n\nIteracao: %d\n", contador);
            fprintf(p_iteracoes,"\nO erro medio da malha foi: %g  ", Erro_Global_Malha.Media );
            fprintf(p_iteracoes,"\nO erro medio normalizado da malha foi: %g ", Erro_Global_Malha.Media/ Erro_Global_Malha.Maior);
            fprintf(p_iteracoes,"\nO erro maximo da malha foi: %g",Erro_Global_Malha.Maior);
            fprintf(p_iteracoes,"\nO desvio padrao da malha foi: %g",Erro_Global_Malha.Desvio_Padrao);
            fprintf(p_iteracoes,"\nO erro limite da malha foi: %g",Erro_Global_Malha.Limite);
            fprintf(p_iteracoes,"\nO erro minimo da malha foi: %g",Erro_Global_Malha.Menor); //*NMA*
            fclose(p_iteracoes);

            #ifdef _AP_DEBUG_

            //*GMM* trecho ideal para os valores de potencais em pontos
            // e a derivada primeira
            // Ser� feita a grava��o em arquivo de cada ponto incluido na malha
            // declara��o dos ponteiros e primeira abertura dos arquivos no inicio da rotina
            // APENAS PARA DEBUG
            p_pontos = NULL;// ponteiro para arquivo que conter� os pontos
            p_elem   = NULL;// ponteiro para arquivo que contem dados de elementos
            p_pontos = fopen("DEBUG_potenciais_pts.txt", "a");
            p_elem   = fopen("DEBUG_elementos.txt", "a");

            long q, r;
            int iii;
            Pt_Malha pt_aux;
            Nos_Elementos n_elm;

            if ((unsigned)bloco_nos_elm.Get_Total_Componentes() != bloco_pts.size())
                printf ("\n \a\n inconsistencia nos vetores de pontos e associacao com elementos\n");

            fprintf(p_pontos, "\n\n\nN_ponto             x                y              pot            deriv           N_elementos  elementos \n");

            for(i=0; i<(signed)bloco_pts.size(); i++) {
                bloco_nos_elm.Get_Componente(i, n_elm);
                pt_aux = bloco_pts[i];

                fprintf (p_pontos,"\n%6d  %15.8lg   %15.8lg   %15.8lg  %15.8lg  %6d",
                        i+1,
                        pt_aux.X(),
                        pt_aux.Y(),
                        pt_aux.Get_Potencial(),
                        pt_aux.Get_DerivadaPrim((short int) 0),
                        n_elm.Get_NElementos() );
                for(iii=0; iii<n_elm.Get_NElementos(); iii++)
                    r = fprintf(p_pontos," %6ld ", n_elm.Get_IElementos(iii)+1);
            }
            fclose(p_pontos);

            fprintf(p_elem, "\n\n\nN_elemento  P1     P2    P3     V1     V2     V3        Campo           deriv           Erro \n");

            for(i=0; i<bloco_elem_2D->Get_Total_Componentes(); i++) {
                bloco_elem_2D->Get_Componente(i, triangulo);
                viz = triangulo.Get_Vizinhos();

                q = fprintf(p_elem,"\n%6d %6ld %6ld %6ld %6ld %6ld %6ld %15.8lg %15.8lg  %15.8lg  %15.8lg",
                        i+1,
                        triangulo.Get_Vertices(0) + 1,
                        triangulo.Get_Vertices(1) + 1,
                        triangulo.Get_Vertices(2) + 1,
                        viz[0] + 1, viz[1] + 1, viz[2] + 1,
                        triangulo.Get_Campo(),
                        triangulo.Get_DerivadaPrim(),
                        triangulo.Get_Maior_Comp(bloco_pts),
                        triangulo.Get_Erro() );
                if (q==EOF) {
                    printf("Houve erro na grava�ao do arquivo bloco_pontos");
                    getchar();
                    break;
                }
            }
            fclose(p_elem);

            #endif // _AP_DEBUG_

            // limpa lista de pontos de refinamento
            nuvem_pontos_ref.clear();
            nuvem_pontos_ref.reserve(bloco_pts.size()+50000);

            long num_Comp_Malha_Original = bloco_elem_2D->Get_Total_Componentes();
            double limite_erro;

            if (Erro_Global_Malha.Limite < Erro_Global_Malha.Media/Erro_Global_Malha.Maior && contador <= MAX_MALHAS) { //*GMM*//
                limite_erro = Erro_Global_Malha.Media;

                for (i = 0; i < num_Comp_Malha_Original; i++) {

                    bloco_elem_2D->Get_Componente(i,triangulo);

                    if(triangulo.Get_Validade()) {
                        if ( triangulo.Get_Erro() >= limite_erro) {
                            Pt_Malha p1, p2;
                            vert_trn[0] = bloco_pts[triangulo.Get_Vertices(0)];
                            vert_trn[1] = bloco_pts[triangulo.Get_Vertices(1)];
                            vert_trn[2] = bloco_pts[triangulo.Get_Vertices(2)];

                            lev_math::Uchar nao_incluido; // para verificar se um ponto de geometria foi incluido em nuvem_pontos
                            //*NMA*
                            lev_math::Uchar aresta_de_geometria;    //para verificar se a�hou a aresta na lista de arestas de geometria
                                                            //apesar dos dois v�rtices n�o serem de refinamento, a aresta pode n�o
                                                            // ser de geometria, neste caso o ponto inserido deve ser de refinamento
                            // Verifica qualidade do triangulo
                            rinsc        = triangulo.Calc_R_Insc(bloco_pts); // imp
                            rcirc        = triangulo.Get_Raio_Circ(); //get raio_circ retorna o raio circunscrito
                            qualidade    = rinsc / rcirc;

                            nao_incluido = true;

                            if(qualidade < FATOR_QUALIDADE) { // imp
                                long v1, v2;
                                // Quebra maior aresta do elemento achatado
                                triangulo.Get_Maior_Comp(bloco_pts, aresta); // imp

                                aresta_de_geometria = false;

                                //procura a aresta na lista de geometrias
                                for(ind_aresta=0; ind_aresta<(signed)bloco_geometria.size(); ind_aresta++) {
                                    aresta1 = bloco_geometria[ind_aresta];
                                    if(aresta == aresta1) { // *AP* aten�ao: o operador de compara��o implementado n�o verifica primitiva
                                        aresta = aresta1;
                                        aresta_de_geometria = true;
                                        break;
                                    }
                                }

                                //obtem as coordenadas do centro da aresta de geometria e
                                //armazena em ponto e transfere para nuvem pontos
                                aresta.Get_Central_Point(bloco_pts, o_ponto);

                                if(aresta_de_geometria) {
                                    v1 = aresta.g1;
                                    v2 = aresta.g2;

                                    std::vector<Pt_Malha>::iterator where;
                                    where = std::find(nuvem_pontos.begin(),nuvem_pontos.end(), o_ponto);
                                    if(*where == o_ponto)
                                        nao_incluido = false;
                                    else {
                                        nuvem_pontos.push_back(o_ponto);
                                        nao_incluido = true;
                                    }
                                } else {                    //*NMA*: se a aresta n�o � geometria, atribui tipo REF ao ponto
                                                            //         e associa ao ponto o material do elemento
                                    o_ponto.Set_Tipo(REF);

                                    long mat[COISAS_POR_NO];
                                    mat[0] = triangulo.Get_Material();
                                    o_ponto.Set_Material(&mat[0],1);

                                    double fonte[COISAS_POR_NO];
                                    fonte[0] = triangulo.Get_Fonte();
                                    o_ponto.Set_Fonte(&fonte[0],1);

                                    o_ponto.Set_TipoCC(0);

                                    std::vector<Pt_Malha>::iterator where;
                                    where = std::find(nuvem_pontos_ref.begin(),nuvem_pontos_ref.end(), o_ponto);
                                    if(*where == o_ponto)
                                        nao_incluido = false;
                                    else {
                                        nuvem_pontos_ref.push_back(o_ponto);
                                        nao_incluido = true;
                                    }
                                }
                            } else {
                                Pt_Malha o_ponto;
                                triangulo.Get_Baricentro(bloco_pts, o_ponto);
                                std::vector<Pt_Malha>::iterator where;
                                where = std::find(nuvem_pontos_ref.begin(),nuvem_pontos_ref.end(), o_ponto);
                                if(*where == o_ponto)
                                    nao_incluido = false;
                                else {
                                    nuvem_pontos_ref.push_back(o_ponto);
                                    nao_incluido = true;
                                }
                            }
                        } // fecha teste erro do triangulo
                    } // fecha teste validade do triangulo
                } // fecha la�o for para todos os elementos

                // inclui os pontos de refinamento
                for(i=0; i<(signed)nuvem_pontos_ref.size(); i++) {
                    Pt_Malha    o_ponto;
                    lev_math::Uchar        nao_incluido;

                    o_ponto = nuvem_pontos_ref[i];

                    std::vector<Pt_Malha>::iterator where;
                    where = std::find(nuvem_pontos.begin(),nuvem_pontos.end(), o_ponto);
                    if(*where == o_ponto)
                        nao_incluido = false;
                    else {
                        nuvem_pontos.push_back(o_ponto);
                        nao_incluido = true;
                    }
                }

                Discretiza();
                bloco_elem_2D->Compacta_Lista(1);
            } else {
                continua = false;
                //================================== libera��o de memoria ======
                //==============================================================
                if(x)            free(x);
                if(y)            free(y); //n�o � usado mas � passado para monta_mc
                if(E1)          free(E1);
                if(E2)          free(E2);
                if(E3)          free(E3);
                if(MEIO)        free(MEIO);
                if(FONTE_EL)    free(FONTE_EL);
                if(FONTE_VAL)   free(FONTE_VAL);
                if(NO_CC)       free(NO_CC);
                if(POT_CC)      free(POT_CC);
                if(NO_CA_REF)   free(NO_CA_REF); //guarda os nos referencia dos nos com cc peri�dica
                if(NO_CA_POT)   free(NO_CA_POT); //guarda os nos com cc peri�dica
                if(TIPO)        free(TIPO);
                if(NO_CF_REF)   free(NO_CF_REF); //guarda os nos referencia dos nos com cc float
                if(NO_CF_POT)   free(NO_CF_POT); //guarda os nos com cc float
                if(angulo)      free(angulo); //angulo para imas permanentes
                if(MC)            free(MC);
                if(COL)            free(COL);
                if(DIAGONAL)    free(DIAGONAL);
                if(RES)            free(RES);
                if(A)            free(A);
                if(DA)            free(DA);
                //==============================================================
            }
        } // fim de while (continua)
    } // m�todo autoadaptativo
    return err;
}


//============================================================================
// int Delaunay2D_Adaptativo::Le_Bd_Materiais(char * nome)
//============================================================================
//
//    Descri��o: Le o arquivo de banco de dados de materiais - vers�o LMAGDOS
//
//    Parametro : Path completo do arquivo de banco de dados
//
//    Retorno:   Erro de leitura
//               0 - Leitura normal
//               13001 - nome n�o fornecido
//
//    vers�o : 1.0
//    programador: Nancy Mieko Abe
//    �ltima modifica��o : 09/05/2001
//
//============================================================================
int Delaunay2D_AutoAdaptativo::Le_BD_Materiais(char * nome_propfis)
{
    int         err    = 13101;
    FILE    *fp        = NULL;
    int         i;
    int         *codigos = new int [10000];
    char     *nome_lixo = new char [256];
    double     **materiais;

    materiais = new double* [10000];
    if(!materiais) {
        std::cout << "\nDelaunay2D_AutoAdaptativo::Le_BD_Materiais(): memory allocation error for materiais\n";
        exit(0);
    }
    for(int i=0; i<10000; i++) {
        materiais[i] = new double [7];
        if(!materiais[i]) {
            std::cout << "\nDelaunay2D_AutoAdaptativo::Le_BD_Materiais(): memory allocation error for materiais[i]\n";
            exit(0);
        }
        memset(&materiais[i][0], 0, sizeof(double)*7);
    }

    if (nome_propfis)
    {
         mat = (struct materiais_struct *) malloc( 1 * sizeof(struct materiais_struct) );
         if(!mat)
         {
          printf("\nerro na alocacao de memoria - struct de materiais");
          exit(1);
         }


        fp = fopen(nome_propfis,"r");
        if (fp == NULL)
        {
            printf("\n erro na abertura do arquivo propfis.mei");
            printf("\n o programa sera encerrado. pressione qualquer tecla.");
            getchar();
            exit(0);
        }

        int NM,NELT, NCCOR, NMAG;

        fscanf(fp, "%d %d %d %d", &NM, &NELT, &NCCOR, &NMAG);

        for(i=0;i<NM;i++) fscanf(fp,"\n%d %s %lf %lf %lf %lf %lf %lf %lf",
                                             &codigos[i],nome_lixo,
                                             &materiais[i][0],
                                             &materiais[i][1],
                                             &materiais[i][2],
                                             &materiais[i][3],
                                             &materiais[i][4],
                                             &materiais[i][5],
                                             &materiais[i][6]);
        fclose(fp);
        err = 0;

        mat->NM = NM;
        for(i=0;i<NM;i++)
        {
            mat->codigos[i] = codigos[i];
            mat->materiais[i][0] = materiais[i][0];
            mat->materiais[i][1] = materiais[i][1];
            mat->materiais[i][2] = materiais[i][2];
            mat->materiais[i][3] = materiais[i][3];
            mat->materiais[i][4] = materiais[i][4];
            mat->materiais[i][5] = materiais[i][5];
            mat->materiais[i][6] = materiais[i][6];
        }

    }

    if(nome_lixo) {
        delete [] nome_lixo; nome_lixo = NULL;
    }
    if(codigos) {
        delete [] codigos; codigos = NULL;
    }
    for(int i=0; i<10000; i++) {
        if(materiais[i]) {
            delete [] materiais[i]; materiais[i] = NULL;
        }
    }
    if(materiais) {
        delete [] materiais; materiais = NULL;
    }

    return err;
}

//============================================================================
//void Delaunay2D_Adaptativo::Elimina_EFront(int passos)
//============================================================================
//
//    Descri��o: Varre a lista de elementos bidimensionais procurando por
//               elementos fronteiri�os, ou seja, que possuem tr�s v�rtices
//                 que fazem parta da geometria. Quando um elemento com estas
//               caracteristicas for encontrao um ponto deve ser inserido em
//               seu inteior.
//
//
//    vers�o : 1.0
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 30/04/2001
//
//============================================================================
bool Delaunay2D_AutoAdaptativo::Elimina_EFront()
{
    unsigned int status;
    long i, total_componentes;
    Elemento_2D elemento;
    int n_vert = elemento.Get_N_Vertices();
    long * vertices = new long [n_vert];
    Pt_Malha ponto;
    long j;
    double xb, yb;
    bool achou_elemento_fronteira = false;

    total_componentes = bloco_elem_2D->Get_Total_Componentes();

    for (i=0; i < total_componentes && !achou_elemento_fronteira;i++) {
        status = bloco_elem_2D->Get_Componente(i,elemento);
        if (status) {
            status = 110;
            Informa_Erro((char*)"Erro 110(3): Dados inconsistetes. Execu��o interrompida...",status);
        }
        if(elemento.Get_Validade()) {
            xb = yb = 0;
            elemento.Get_Vertices(vertices);//captura indices dos v�rtices
            for (j = 0; j < n_vert; j++) {
                ponto = bloco_pts[vertices[j]];
                xb += ponto.X();
                yb += ponto.Y();
                if (ponto.Get_Tipo()==REF)
                    break;
            }
            if (j == n_vert)//se todos os vertices forem de geometria ent�o
            {               // deve ser inserido um ponto no elemento.
                ponto.Set_Coord(xb/(double)n_vert, yb/(double)n_vert);
                ponto.Set_Tipo(REF);//por n�o ser um ponto de geometria
/*NMA*/            ponto.Set_Material(elemento.Get_Material());
/*NMA*/            ponto.Set_Fonte(elemento.Get_Fonte());
                //deve ser obrigatoriamente de refinamento *GMM*
//                nuvem_pontos->Inclui(ponto);
                nuvem_pontos.push_back(ponto);
                // o ponto � inlcuido em nuvem_pontos e ser� inserido
                // na malha por meio de discretiza
                achou_elemento_fronteira = true;
            }
        }
    }// fim do la�o for i

    delete [] vertices;
    vertices = NULL;

    return achou_elemento_fronteira;
}


//============================================================================
//void Delaunay2D_Adaptativo::Associa_ElmNos()
//============================================================================
//
//    Descri��o: Associa em uma lista elementos a n�s. Explicitando, associa
//               a uma posi��o em bloco_nos_elm ,correspondente � do n� na
//               lista bloco_pts, uma lista dos elementos que possuem o n�
//                 como v�rtice. Ser� utilizado para c�lculo dos campos em n�s
//               por Calc_CampoNos();
//
//    vers�o : 1.0
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 06/05/2001
//
//============================================================================
void Delaunay2D_AutoAdaptativo::Associa_ElmNos()
{
    unsigned long i;
    long j;
    Nos_Elementos no_elm;
    Elemento_2D elemento;
    long vert[3]; //n�mero de vertices fornecido pelo elemento

    bloco_nos_elm.Set_N_Max_Com_Abs(bloco_pts.size());
    Nos_Elementos n_elm;//inclui todos os pontos da malha na lista de vetores
    for(i=0; i<bloco_pts.size(); i++)
        bloco_nos_elm.Inclui(no_elm);
    for(i=0; i<(unsigned long)bloco_elem_2D->Get_Total_Componentes(); i++) {
        bloco_elem_2D->Get_Componente(i, elemento);
        if (elemento.Get_Validade()) {
            elemento.Get_Vertices(vert);
            for (j=0; j<elemento.Get_N_Vertices(); j++)
                bloco_nos_elm[vert[j]].Ad_Elemento(i);
        }
    }
}


//============================================================================
//void Delaunay2D_Adaptativo::Calc_CampoNos()
//============================================================================
//
//    Descri��o: A partir da lista bloco_nos_elm c�lcula o valor do campo em
//               cada n� armazenado em bloco_pts e armazena este valor
//               no n� corresponde na lista bloco_pts;
//
//    vers�o : 1.0
//    programador: Guilherme Moreira Magalh�es
//    �ltima modifica��o : 06/05/2001
//
//============================================================================
void Delaunay2D_AutoAdaptativo::Calc_CampoNos()
{
long i;
Nos_Elementos no_elms;
char j;
Elemento_2D elemento;
Pt_Malha ponto;

    Associa_ElmNos();

#if 0
double media;
    for(i=0; i<bloco_nos_elm.Get_Total_Componentes(); i++)
    {
        media = 0;
        bloco_nos_elm.Get_Componente(i, no_elms);
        for (j=0; j<no_elms.Get_NElementos(); j++)
        {
            bloco_elem_2D->Get_Componente(no_elms.Get_IElementos(j),elemento);
            media += elemento.Get_Campo();
        }
        media /= (double)no_elms.Get_NElementos();
        bloco_pts->Get_Componente(i, ponto);
        ponto.Set_DerivadaPrim(media);
        bloco_pts->Modifica(i, ponto);
    }
#endif

#if 10 //para o calculo do erro considerando os varios materiais do ponto
    int k;
    double media[COISAS_POR_NO];
    int    num_elem_material[COISAS_POR_NO];

    for(i=0; i<bloco_nos_elm.Get_Total_Componentes(); i++)
    {
        memset(media,0,COISAS_POR_NO*sizeof(double));
        memset(num_elem_material,0,sizeof(num_elem_material));

//        bloco_pts->Get_Componente(i, ponto);
        ponto = bloco_pts[i];
        bloco_nos_elm.Get_Componente(i, no_elms);

        for (j=0; j<no_elms.Get_NElementos(); j++)
        {
            bloco_elem_2D->Get_Componente(no_elms.Get_IElementos(j),elemento);

            k = ponto.Esta_Associado(elemento.Get_Material());

            if(k>=0)
            {
                media[k] +=elemento.Get_Campo();
                num_elem_material[k]++;
            }
            else
            {
                std::cout << "\nERRO: material do elemento n�o est� associado a um dos pontos v�rtices";
                std::cout << "\n Ponto vertice = " << i << "\n";
                exit (i);
            }

//            for(k=0;k<ponto.Get_Material();k++)
//            {
//                if(elemento.Get_Material() == ponto.Get_Material(short int (k)))
//                {
//                    media[k] +=elemento.Get_Campo();
//                    num_elem_material[k]++;
//                }
//            }
        }
        for(k=0;k<ponto.Get_Material();k++)
        {
            media[k] /= (double)num_elem_material[k];
        }

        ponto.Set_DerivadaPrim(&media[0],ponto.Get_Material());
//        bloco_pts->Modifica(i, ponto);
        bloco_pts[i] = ponto;

    }
#endif
}

//void Delaunay2D_AutoAdaptativo::Calc_Erro_Elementos()

void Delaunay2D_AutoAdaptativo::Calc_Erro_Elementos(erro &erro_iteracao)
{
    long i;
    double  desvio = 0, maior = 0;
    double    menor = 1e308; //*NMA*
    Elemento_2D elemento;

    erro_iteracao.Media = 0.0; //*NMA*

    for(i = 0; i < bloco_elem_2D->Get_Total_Componentes(); i++)
    {
        bloco_elem_2D->Get_Componente(i, elemento);
        elemento.Calc_Erro(bloco_pts);
        if (maior < elemento.Get_Erro()) maior = elemento.Get_Erro();
        if (menor > elemento.Get_Erro() ) menor = elemento.Get_Erro();//*NMA*
        erro_iteracao.Media += elemento.Get_Erro();
        bloco_elem_2D->Modifica(i, elemento);
    }

    erro_iteracao.Media /= bloco_elem_2D->Get_Total_Componentes();

    for(i = 0; i < bloco_elem_2D->Get_Total_Componentes(); i++)
    {
        bloco_elem_2D->Get_Componente(i, elemento);
        desvio += (elemento.Get_Erro() - erro_iteracao.Media) * (elemento.Get_Erro() - erro_iteracao.Media);
    }
    erro_iteracao.Desvio_Padrao = sqrt(desvio/bloco_elem_2D->Get_Total_Componentes());
    erro_iteracao.Maior = maior;
    erro_iteracao.Menor = menor; //*NMA*

}


