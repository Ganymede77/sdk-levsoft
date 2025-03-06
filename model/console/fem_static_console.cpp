// *
// * *WPG*: Arquivo a ser implementado utilizando as classes atuais em C++
// *


#include "lo_pt.hpp"
#include "dgr15_10_2d.hpp"
#include "elemento_2d.hpp"
#include "aresta_geom.hpp"
#include "lo_pt_tp.hpp"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"
#include "sys/types.h"
#include "sys/timeb.h"
#include "string.h"


#ifndef __STRUCT_MATERIAIS__
    #define __STRUCT_MATERIAIS__
    struct materiais_struct
    {
        int NM;
        int codigos[40];
        double materiais[40][7];
    };

#endif

//#define ____PIC____
//...........................................................................................
//...........................................................................................
// prot�tipos das fun��es internas deste arquivo

void initialization_1d_electrostatic(int num_mesh_points,
                                     double dx,double *x, int *E1, int *E2, int *NO_CA_REF, int *NO_CA_POT,
                                     int *TIPO,int *NO_CF_REF,int *NO_CF_POT,
                                     int NN,int NE,int NCA,int NCF,int NC);

void initialization_2d_by_files(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, char* nome_problema, int *mat_lin,
                                materiais_struct *mat);

void monta_col(int NN,int NE,int NCA,int NCF,int num_elem_nodes, int *E1,int *E2,int *E3,
               int *NO_CA_REF,int *NO_CA_POT,int *TIPO,int *NO_CF_REF,int *NO_CF_POT, int nos_alocados,
               int *COL,int *DIAGONAL,int *ind_col_aux);

void monta_mc(int NN,int NE,int NF,int NCA,int NCF,int NC,double *x,double *y,
              int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
              int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo, int tipo_estudo,
              double *MC,int *COL,int *DIAGONAL, double *RES, double *A, int ind_col,int mat_lin,int itn,
              materiais_struct *mat);

#if defined ____PIC____
void monta_mc_pic(int NN,int NE,int NF,int NCA,int NCF,int NC,double *x,double *y,
              int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
              int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo, int tipo_estudo,
              double *MC,int *COL,int *DIAGONAL, double *RES, double *A, int ind_col,int mat_lin,int itn,
              double permittivity);
#endif

void linear(double prop_fis,double *b,double *c,double delta,int *cond,int mat_lin,int *E,
            int *no_global,double *Ifonte,double *MC,double *A,double *RES,int *DIAGONAL, int *COL,int ind_col);


void nao_linear(double Bx,double By,double k1f,double k2f,double k3f,double *b,double *c,double delta,
                int *cond,int mat_lin,int itn,double *A,int *E,double *MC,int *no_global,double *RES,
                int *DIAGONAL, int *COL,int ind_col);


void anisotropico(int l,double Bx,double By,double *angulo,double Hc,double ni2,double k1i,double k2i,double k3i,double itn,double delta,
                  double *b,double *c,int *cond,int mat_lin,double *A,int *no_global,int *E,double *MC,double *RES,
                  int *DIAGONAL, int *COL,int ind_col);


void elem_1d_matrix(int NN,int NE,int NCA,int NF,int NCF,int NC,int num_elem_nodes,
                    int *E1,int *E2,int *E3, int *NO_CA_REF,int *NO_CA_POT,int *TIPO,int *NO_CF_REF,int *NO_CF_POT,
                    double *MC,int *COL,int *DIAGONAL,double *RES,int ind_col, double *x, double *y,
                    int *FONTE_EL,double *FONTE_VAL,int *MEIO,double permittivity_1d);

void cond_cont(int NN,int NCA,int NCF,int NC,int *NO_CA_POT,int *NO_CF_POT,int *NO_CC,double* POT_CC,
               double *MC,int *COL,int *DIAGONAL,double *RES,int ind_col,int *linha1);

int busca(int linha,int coluna, int *DIAGONAL, int *COL, int ind_col);

void iccg(int NN,double *MC,int *COL,int *DIAGONAL,double *RES,double* DA,int ind_col,int linha1, int mat_lin,
          int itn, int itc_max);

void monta_matriz_U(int NN,double *U,double *MC,int *DIAGONAL,int *COL,int ind_col,int linha1);

void calcula_y(int NN,double *U,int *DIAGONAL,int *COL,double *yi,double *gt);

void calcula_h(int NN,double *U,int *DIAGONAL,int *COL,double *yi,double *h);


void compute_electric_field_1d_internal(int NE,int *E1,int *E2,double *x,double *A,double *E_field);

double compute_electric_field_2d_internal(int NE,double prop_fis,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *E_field);

void compute_electric_field_2d_internal_in_node(int NE,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex_node,double *Ey_node);

void solve_non_linear_system_internal(int NN,int NE,int NCA,int NCF,int NC,int NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, int mat_lin,int itn_max, int itc_max,int tipo_estudo,
                                materiais_struct *mat,double *MC, int *COL, int *DIAGONAL, double *RES,
                                int nos_alocados, int ind_col,double precisao, double *A);

#if defined ____PIC____
void mesh_generation(int NN,int NF,int NCA,int NCF,int NC,double *x,double *y,
                     int *FONTE_EL,double *FONTE_VAL,
                     int *NO_CC,double *POT_CC,
                     int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,int *MEIO,
                     int *E1,int *E2,int *E3, int *NE);
#endif //____PIC____

//...........................................................................................
//...........................................................................................
// prot�tipos das fun��es de interface para pic e autoadaptatividade

extern "C" // *NMA* para ser chamado por Fortran
{
#if defined ____PIC____
// ---------------------------   1d pic ----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------
    void build_matrix_system_1d_skeleton(int *num_mesh_points, double *dx, int *nos_alocados,
                               int *NN,int *NE,int *NCA,int *NCF,int *NC,double *x,double *y, int *E1,int *E2,int *E3,
                                int *MEIO,int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *COL,int *DIAGONAL, int *ind_col);

    void fill_matrix_system_1d(double *permittivity, double *charge_density,int *NN,int *NE,int *NF,int *NCA,int *NCF,
                            int *NC,double *x,double *y,
                            int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
                            int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo,
                            int *tipo_estudo, double *MC,int *COL,int *DIAGONAL, double *RES, int *ind_col, int *linha1);

    void compute_electric_field_1d(int *NE,int *E1,int *E2,double *x,double *A, double *Ex);

// ---------------------------   2d pic  ------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

    void build_data_2d_vectors_by_pic_data(int *nx, int *ny, double *deltax, double *deltay,
                                        int *NN,int *NE,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                                        int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,
                                        int *NO_CC,double *POT_CC,
                                        int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT);


    void build_matrix_system_2d_skeleton(int *nos_alocados,int *NN,int *NE,int *NCA,int *NCF,int *NC,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *COL,int *DIAGONAL, int *ind_col);

    void fill_matrix_system_2d(double *permittivity, double *charge_density, int *NN,int *NE,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                            int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
                            int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo,
                            int *tipo_estudo, double *MC,int *COL,int *DIAGONAL, double *RES, double *A,
                            int *mat_lin,int *itn, int *ind_col, int *linha1);

    double compute_electric_field_2d(int *NE,double *prop_fis,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex);
    void compute_electric_field_2d_in_node(int *NE, int *NN,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex_node,double *Ey_node);

#endif    // ____PIC____
// ---------------------------   2d autoadapatividade  ------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------
    void build_data_2d_vectors_by_files(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, char* nome_problema, int *mat_lin,
                                materiais_struct *mat);

    void build_mesh(int *NN,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                     int *FONTE_EL,double *FONTE_VAL,
                     int *NO_CC,double *POT_CC,
                     int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,int *MEIO,
                     int *E1,int *E2,int *E3, int *NE);

    void build_fill_and_solve_generic_system(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC, int *NO_CA_REF, int *NO_CA_POT,int *TIPO,
                                int *NO_CF_REF, int *NO_CF_POT,int *FONTE_EL,double *FONTE_VAL,
                                int *mat_lin, int *itn_max, int *itc_max, materiais_struct *mat, int *tipo_estudo,
                                double *MC, int *COL, int *DIAGONAL, double *RES, int *nos_alocados, double *precisao,
                                double *A);

// ---------------------------   commom  ------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------

    void solve_static_linear_system(int *NN,double *MC,int *COL,int *DIAGONAL,double *RES,double *A,
                                int *ind_col,int *linha1, int *mat_lin, int *itn, int *itc_max,
                                int *NC, int *NCA, int *NCF,int *NO_CC, double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT);


}

//...........................................................................................

//...........................................................................................
//...........................................................................................
extern "C" // *NMA* para ser chamado por Fortran
{
//===============================================================================================================================
// buid_matrix_system_1d_skeleton
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:    - Monta a estrutura para o armazenamento da matriz esparsa sim�trica, considerando
//                    as condi��es de contorno, para elementos unidimensionais de 1a. ordem
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//                int*    num_mesh_points: n�mero de pontos da malha (grade)
//                double*    dx:                 espa�amento da grade (tamanho do elemento)
//              int*    nos_alocados:    tamanho dos vetores COL e MC                                                usado para verificar se o
//                                                                                                                    tamanho alocado (estaticamente ou dinamicamente ) � suficiente
//
// SA�DA:
//              int     *NN           :  numero de pontos da malha de EF
//              int     *NE           :  numero de elementos da malha de EF
//              int     *NF           :  numero de elementos com fonte
//              int     *NCA          :  numero de nos com cc periodica
//              int     *NCF          :  numero de nos com cc float
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//              int     tipo_estudo   :  flag que indica qual tipo de estudo
//                                          1 - eletrost�tico  2 - eletrocin�tico  3 - magnetostatico
//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com  determinada ap�s
//                                       elementos n�o nulos                                    a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais     num_mesh_points+1
//              int     *ind_col       :  dimens�o de MC  e COL
//
//===============================================================================================================================

#if defined ____PIC____

void build_matrix_system_1d_skeleton(int *num_mesh_points, double *dx, int *nos_alocados,
                               int *NN,int *NE,int *NCA,int *NCF,int *NC,double *x,double *y, int *E1,int *E2,int *E3,
                                int *MEIO,int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *COL,int *DIAGONAL, int *ind_col)
{

        int        num_elem_nodes = 2; //numero de nos do elemento


        *NN  = *num_mesh_points;
        *NE  = *num_mesh_points-1;
        *NCA = 1;
        *NCF = 0;
        *NC  = 0;


        initialization_1d_electrostatic(*num_mesh_points,*dx,x,E1,E2,NO_CA_REF,NO_CA_POT,
                                        TIPO,NO_CF_REF,NO_CF_POT, *NN, *NE, *NCA, *NCF, *NC);

        monta_col(*NN,*NE,*NCA,*NCF,num_elem_nodes,E1,E2,E3,NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF,NO_CF_POT,*nos_alocados,
                  COL,DIAGONAL,ind_col);



}//end of buid_matrix_system_1d_skeleton


//===============================================================================================================================
// fill_matrix_system_1d
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:
//                - Calcula os elementos da matriz e preenche a estrutura
//                - Introduz as condi��es de contorno
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//                double* permittivity  :  valor da permissividade do meio
//    0-based        double*    charge density:  ponteiro para vetor com fonte (densidade de carga)   num_mesh_points        do fortran, definir normalmente (1-based)
//              int*     NN            :  numero de pontos da malha de EF
//              int*     NE            :  numero de elementos da malha de EF
//              int*     NF            :  numero de elementos com fonte
//              int*     NCA           :  numero de nos com cc periodica
//              int*     NCF           :  numero de nos com cc float
//              int*     NC            :  numero de nos com cc dirichlet
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    FONTE_EL      :  ponteiro para vetor com os �ndices de elementos com fonte    NF+1
//  1-based     double* FONTE_VAL     :  ponteiro para vetor com os valores de fonte                  NF+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//              int*     tipo_estudo   :  flag que indica qual tipo de estudo
//                                          1 - eletrost�tico  2 - eletrocin�tico  3 - magnetostatico
//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com           determinada ap�s
//                                       elementos n�o nulos                                             a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais             NN+1
//              int*     ind_col       :  dimens�o de MC  e COL
//
// SA�DA:
//  0-based     double* MC              :     ponteiro para vetor com os elementos n�o nulos da            determinada ap�s
//                                       matriz  (PREENCHIDA)                                          a montagem
//  1-based     double* RES            :  ponteiro para vetor do 2. membro do sistema (PREENCHID0)     NN+1
//              int*     linha1        :  indice da primeira linha sem condi��o de contorno
//
//===============================================================================================================================
void fill_matrix_system_1d(double *permittivity, double *charge_density,int *NN,int *NE,int *NF,int *NCA,int *NCF,
                            int *NC,double *x,double *y,
                            int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
                            int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo,
                            int *tipo_estudo, double *MC,int *COL,int *DIAGONAL, double *RES, int *ind_col, int *linha1)
{
        int num_elem_nodes = 2;
        int i;

        for(i=0;i<=*ind_col;i++) MC[i] = 0.0;
        for(i=0;i<=*NN;i++) RES[i] = 0.0;

        *NF = *NE;

        for(i=0;i<*NE;i++)
        {
         FONTE_EL[i+1]   = i+1;
         FONTE_VAL[i+1]  = -charge_density[i];
        }


        //n�o foi usado monta_mc porque os c�lculos para 1d e 2d s�o muito diferentes, foi mais f�cil aproveita a elem_1d_local_matrix
        elem_1d_matrix(*NN,*NE,*NCA,*NF,*NCF,*NC,num_elem_nodes,E1,E2,E3,NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF,NO_CF_POT,
                       MC,COL,DIAGONAL,RES,*ind_col,x,y,FONTE_EL,FONTE_VAL,MEIO, *permittivity);


        cond_cont(*NN,*NCA,*NCF,*NC,NO_CA_POT,NO_CF_POT,NO_CC,POT_CC,MC,COL,DIAGONAL,RES,*ind_col,linha1);

}// termina fill_matrix_system_1d



//===============================================================================================================================
// buid_matrix_system_2d_skeleton
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:    - Monta a estrutura para o armazenamento da matriz esparsa sim�trica, considerando
//                    as condi��es de contorno, para elementos bidimensionais de 1a. ordem
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int     *NN           :  numero de pontos da malha de EF
//              int     *NE           :  numero de elementos da malha de EF
//              int     *NF           :  numero de elementos com fonte
//              int     *NCA          :  numero de nos com cc periodica
//              int     *NCF          :  numero de nos com cc float
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//              int     tipo_estudo   :  flag que indica qual tipo de estudo
//                                          1 - eletrost�tico  2 - eletrocin�tico  3 - magnetostatico                                                                                                                tamanho alocado (estaticamente ou dinamicamente ) � suficiente
//
// SA�DA:

//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com  determinada ap�s
//                                       elementos n�o nulos                                    a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais     num_mesh_points+1
//              int     *ind_col       :  dimens�o de MC  e COL
//
//===============================================================================================================================

void build_matrix_system_2d_skeleton(int *nos_alocados,int *NN,int *NE,int *NCA,int *NCF,int *NC,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *COL,int *DIAGONAL, int *ind_col)
{

        int        num_elem_nodes = 3; //numero de nos do elemento


        monta_col(*NN,*NE,*NCA,*NCF,num_elem_nodes,E1,E2,E3,NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF,NO_CF_POT,*nos_alocados,
                  COL,DIAGONAL,ind_col);



}//end of buid_matrix_system_2d_skeleton

#endif // ____PIC____

//===============================================================================================================================
// build_data_2d_vectors_by_files
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:    - le arquivos de dados
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:
//                char    *nome_problema

//                                                                                dimens�o utilizada        Obs:
// SA�DA:
//              int     *NN           :  numero de pontos da malha de EF
//              int     *NE           :  numero de elementos da malha de EF
//              int     *NF           :  numero de elementos com fonte
//              int     *NCA          :  numero de nos com cc periodica
//              int     *NCF          :  numero de nos com cc float
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//                int         mat_lin;
//===============================================================================================================================
void build_data_2d_vectors_by_files(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, char* nome_problema, int *mat_lin,
                                materiais_struct *mat)
{

    initialization_2d_by_files(NN,NE,NCA,NCF,NC,NF,
                                x,y,E1,E2,E3,MEIO,angulo,
                                NO_CC,POT_CC,
                                NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF,NO_CF_POT,
                                FONTE_EL,FONTE_VAL,nome_problema,mat_lin,mat);

}// end of build_data_2d_vectors_by_files

//===============================================================================================================================
// void build_data_2d_vectors_by_pic_data
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:
//                - A partir dos dados de pic 2d, gera a malha de elementos finitos e
//                  preenche os vetores utilizado pelo solver
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:
//                double *Lx              : deltax
//                double *Ly              : deltay
//                double *Lxt              : comprimento total em x
//                double *Lyt              : comprimento total em y
//                                                                                 dimens�o utilizada        Obs:
// SA�DA:
//              int    *NN            :  numero de pontos da malha de EF
//              int    *NE            :  numero de elementos da malha de EF
//              int    *NF            :  numero de elementos com fonte
//              int    *NCA           :  numero de nos com cc periodica
//              int    *NCF           :  numero de nos com cc float
//              int    *NC            :  numero de nos com cc dirichlet
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    FONTE_EL      :  ponteiro para vetor com os �ndices de elementos com fonte    NF+1
//  1-based     double* FONTE_VAL     :  ponteiro para vetor com os valores de fonte                  NF+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1

//
//===============================================================================================================================

#if defined ____PIC____
void build_data_2d_vectors_by_pic_data(int *nx, int *ny, double *deltax, double *deltay,
                                        int *NN,int *NE,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                                        int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,
                                        int *NO_CC,double *POT_CC,
                                        int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT)
{
    int        i,j;
    int        cont_cc = 0;
    int        posicao = 1;
    int        np_x = 0;
    int        np_y = 0;
    FILE *fp = NULL;


//calcula NN
//    *NN = *nx * *ny;

    np_x = (*nx);
    np_y = (*ny);

    *NN = np_x * np_y;

//monta vetores x e y e  vetores de cc c�clica e refer�ncia  para nos em x = 0 (horizontal)
//    for(i=1;i<=*nx;i++)
    for(i=1;i<=np_x;i++)
    {
//        if(i!=1 && i!= *nx)
        if(i!= *nx)
//        if(i!=1 && i!= np_x)
        {
//            NO_CA_POT[++cont_cc] = posicao;
            NO_CA_REF[++cont_cc] = posicao;
        }

//        for(j=1;j<=*ny;j++)
        for(j=1;j<=np_y;j++)
        {

            x[posicao] = (i-1)* *deltax;
            y[posicao] = (j-1)* *deltay;
            posicao++;

        }//end of for j

//        if(i!=1 && i!=np_x)
//        if(i!=1 && i!=*nx)
        if(i!= *nx)
        {
//            NO_CA_REF[cont_cc] = posicao-1;
            NO_CA_POT[cont_cc] = posicao-1;
            TIPO[cont_cc] = 4;//ciclica
        }


    }//end for i

// monta cc c�clica e refer�ncia para n�s em y = Lyt (vertical a direita)

//#if 0
    for(j=1;j<=*ny;j++)
//    for(j=1;j<=*ny-1;j++)
//    for(j=2;j<=*ny-1;j++)
//    for(j=2;j<=np_y-1;j++)
    {
        NO_CA_REF[++cont_cc] = j;
        NO_CA_POT[cont_cc] = *NN-np_y+j;
//        NO_CA_POT[cont_cc] = *NN-*ny+j;
        TIPO[cont_cc] = 4; //ciclica
    }
//#endif

// atualiza NCA : numero de pontos com condi��o de contorno
    *NCA =cont_cc;


//#if 0

// *NMA* // AQUI S� PARA TESTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// considerando o 3 pontos de fronteira float do ponto(1,1)
#if 0
    *NCF=3;

    NO_CF_REF[1] = 1;
    NO_CF_REF[2] = 1;
    NO_CF_REF[3] = 1;

//    NO_CF_POT[1] = *ny;
    NO_CF_POT[1] = np_y;
    NO_CF_POT[2] = *NN;
    NO_CF_POT[3] = *NN-np_y+1;
//    NO_CF_POT[3] = *NN-*ny+1;

#endif
// considerando os 4 pontos de fronteira com potencial conhecido = 0
//    *NC    = 4;
//
//    NO_CC[1] = 1;
//    NO_CC[2] = *ny;
//    NO_CC[3] = *NN;
//    NO_CC[4] = *NN-*ny+1;
//
//    POT_CC[1] = 0.0;
//    POT_CC[2] = 0.0;
//    POT_CC[3] = 0.0;
//    POT_CC[4] = 0.0;

//#endif
// gera malha
//monta vetores E1, E2 ,E3 a partir da saida do gerador de malha

#if 0
    fp = fopen("cc.m","w");
    if(fp!=NULL)
    {
        fprintf(fp,"NCA = %d;\n",*NCA);
        for(i=1;i<=*NCA;i++)
        {
            fprintf(fp,"NO_CA_REF(%d) = %d;\n",i,NO_CA_REF[i]);
            fprintf(fp,"NO_CA_POT(%d) = %d;\n",i,NO_CA_POT[i]);
        }

        fprintf(fp,"NCF = %d",*NCF);
        for(i=1;i<=*NCF;i++)
        {
            fprintf(fp,"NO_CF_REF(%d) = %d;\n",i,NO_CF_REF[i]);
            fprintf(fp,"NO_CF_POT(%d) = %d;\n",i,NO_CF_POT[i]);
        }

        fclose(fp);

    }
#endif

    mesh_generation( *NN, *NF, *NCA, *NCF, *NC, x, y, FONTE_EL, FONTE_VAL, NO_CC, POT_CC,
                         NO_CA_REF, NO_CA_POT, TIPO, NO_CF_REF,  NO_CF_POT, MEIO,
                         E1, E2, E3, NE);


}//end of build_data_2d_vectors_by_pic_data

//===============================================================================================================================
// void build_mesh
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:
//                - A partir dos dados lidos, gera a malha de elementos finitos e
//                  preenche os vetores utilizado pelo solver
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:
//                                                                                 dimens�o utilizada        Obs:
//              int    *NN            :  numero de pontos da malha de EF
//              int    *NF            :  numero de elementos com fonte
//              int    *NCA           :  numero de nos com cc periodica
//              int    *NCF           :  numero de nos com cc float
//              int    *NC            :  numero de nos com cc dirichlet
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    FONTE_EL      :  ponteiro para vetor com os �ndices de elementos com fonte    NF+1
//  1-based     double* FONTE_VAL     :  ponteiro para vetor com os valores de fonte                  NF+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//
// SA�DA:
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//              int    *NE            :  numero de elementos da malha de EF
//
//
//===============================================================================================================================
void build_mesh(int *NN,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                     int *FONTE_EL,double *FONTE_VAL,
                     int *NO_CC,double *POT_CC,
                     int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,int *MEIO,
                     int *E1,int *E2,int *E3, int *NE)
{

    mesh_generation( *NN, *NF, *NCA, *NCF, *NC, x, y, FONTE_EL, FONTE_VAL, NO_CC, POT_CC,
                         NO_CA_REF, NO_CA_POT, TIPO, NO_CF_REF,  NO_CF_POT, MEIO,
                         E1, E2, E3, NE);

}


//===============================================================================================================================
// fill_matrix_system_2d
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:    Especifica para pic 2d
//                - Calcula os elementos da matriz e preenche a estrutura
//                - Introduz as condi��es de contorno
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int*     NN            :  numero de pontos da malha de EF
//              int*     NE            :  numero de elementos da malha de EF
//              int*     NF            :  numero de elementos com fonte
//              int*     NCA           :  numero de nos com cc periodica
//              int*     NCF           :  numero de nos com cc float
//              int*     NC            :  numero de nos com cc dirichlet
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    FONTE_EL      :  ponteiro para vetor com os �ndices de elementos com fonte    NF+1
//  1-based     double* FONTE_VAL     :  ponteiro para vetor com os valores de fonte                  NF+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//              int*     tipo_estudo   :  flag que indica qual tipo de estudo
//                                          1 - eletrost�tico  2 - eletrocin�tico  3 - magnetostatico
//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com           determinada ap�s
//                                       elementos n�o nulos                                             a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais             NN+1
//              int*     ind_col       :  dimens�o de MC  e COL
//
// SA�DA:
//  0-based     double* MC              :     ponteiro para vetor com os elementos n�o nulos da            determinada ap�s
//                                       matriz  (PREENCHIDA)                                          a montagem
//  1-based     double* RES            :  ponteiro para vetor do 2. membro do sistema (PREENCHID0)     NN+1
//              int*     linha1        :  indice da primeira linha sem condi��o de contorno
//
//===============================================================================================================================

void fill_matrix_system_2d(double *permittivity,double *charge_density, int *NN,int *NE,int *NF,int *NCA,int *NCF,int *NC,double *x,double *y,
                            int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
                            int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo,
                            int *tipo_estudo, double *MC,int *COL,int *DIAGONAL, double *RES, double *A,
                            int *mat_lin,int *itn, int *ind_col, int *linha1)
{
        int num_elem_nodes = 3;
        int i,j,k;
        FILE *fp = NULL;

        for(i=0;i<=*ind_col;i++)
        {
            MC[i] = 0.0;
            if(COL[i]<0) COL[i] = -COL[i]; //para retornar a situa��o original
        }
        for(i=0;i<=*NN;i++) RES[i] = 0.0;

        monta_mc_pic(*NN,*NE,*NF,*NCA,*NCF,*NC,x,y,E1,E2,E3,MEIO,FONTE_EL,FONTE_VAL,NO_CC,POT_CC,NO_CA_REF,NO_CA_POT,TIPO,
             NO_CF_REF,NO_CF_POT,angulo,*tipo_estudo,MC,COL,DIAGONAL,RES,A,*ind_col,*mat_lin,*itn,*permittivity);

//*************************************************************************
// ESPECIFICO PARA PIC 2D : fonte dada no ponto e condi��o c�clica
//*************************************************************************
        for(i=0;i<*NN;i++)
        {
//         FONTE_EL[i+1]   = i+1;
//         FONTE_VAL[i+1]  = -charge_density[i];
         RES[i+1]  = charge_density[i];
        }

        *NF = *NN;
// soma contribui��o dos nos com CC float para o n� de referencia
        for(i=1;i<=*NCF;i++)
        {
         RES[NO_CF_REF[i]]  += RES[NO_CF_POT[i]];
        }

// soma contribui��o dos nos com CC ciclica para o n� de referencia
        for(i=1;i<=*NCA;i++)
        {
         RES[NO_CA_REF[i]]  += RES[NO_CA_POT[i]];
        }

        cond_cont(*NN,*NCA,*NCF,*NC,NO_CA_POT,NO_CF_POT,NO_CC,POT_CC,MC,COL,DIAGONAL,RES,*ind_col,linha1);

#if 0
//grava��o da matriz em arquivo para matlab
        fp = fopen("matrizes.m","w");
        if(fp!= NULL)
        {
            fprintf(fp,"MC = sparse(%d,%d);\n",*NN,*NN);

            for(i=1;i<=*NN;i++)
                for(j=i;j<=*NN;j++)
                {
                    k = busca(i,j,DIAGONAL,COL,*ind_col);

                    if(k!=-1)
                    {
                        if(MC[k]) // s� imprime os n�o nulos
                        {
                            fprintf(fp,"MC(%d,%d) = %g;\n",i,j,MC[k]);
                            if(j!=i) fprintf(fp,"MC(%d,%d) = %g;\n",j,i,MC[k]);
                        }
                    }

                }

            // nos com cc c�clica MC(i,i) = 1 (originalmente s�o zero)

            for(i=1;i<=*NCA;i++)
            {
                fprintf(fp,"MC(%d,%d) = 1;\n",NO_CA_POT[i],NO_CA_POT[i]);
            }

            for(i=1;i<=*NCF;i++)
            {
                fprintf(fp,"MC(%d,%d) = 1;\n",NO_CF_POT[i],NO_CF_POT[i]);
            }

            for(i=1;i<=*NN;i++) fprintf(fp,"RES(%d) = %g;\n",i,RES[i]);


            if(*NC)
            {
                for(i=1;i<=*NC;i++)
                {
                    fprintf(fp,"MC(%d,%d) = 1;\n",NO_CC[i],NO_CC[i]);
                    fprintf(fp,"RES(%d) = %g;\n",NO_CC[i],POT_CC[i]);
                }
            }

            fclose(fp);
        }
#endif

}// termina fill_matrix_system_2d

#endif // ____PIC____

//===============================================================================================================================
// solve_static_linear_system
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:    - Resolve sistema de equa��es lineares pelo m�todo iccg, considerando:
//                 - matrizes sim�tricas armazenadas pelo m�todo de armazenamento compacto por linha
//
//                -
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int*    NN            :  numero de pontos da malha de EF
//  0-based     double* MC              :     ponteiro para vetor com os elementos n�o nulos da   determinada ap�s
//                                       matriz                                               a montagem
//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com  determinada ap�s
//                                       elementos n�o nulos                                    a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais     NN+1
//  1-based     double* RES            :  ponteiro para vetor do 2. membro do sistema          NN+1
//              int*     ind_col       :  dimens�o de MC  e COL
//              int*     linha1        :  indice da primeira linha sem condi��o de contorno
//              int*     mat_lin       :  flag para indicar a existencia de material n�o linear
//                                        0 - material linear   1 - material n�o linear
//              int*        itn              :  numero da itera��o de NR, no caso de estudo linear
//                                        itn = 1
//              int*     itc_max       :  numero m�ximo de itera��es do iccg
//              int*     NCA           :  numero de nos com cc periodica
//              int*     NCF           :  numero de nos com cc float
//              int*     NC            :  numero de nos com cc dirichlet
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1

//
// SA�DA:
//  1-based     double* A              :  ponteiro para vetor com a solu��o do sistema         NN+1
//
//===============================================================================================================================

void solve_static_linear_system(int *NN,double *MC,int *COL,int *DIAGONAL,double *RES,double *A,
                                int *ind_col,int *linha1, int *mat_lin, int *itn, int *itc_max,
                                int *NC, int *NCA, int *NCF,int *NO_CC, double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT)
{
    int     i;
    double *DA = NULL;

    DA = (double *)calloc( (*NN+2) , sizeof(double) );
    if(!DA)
    {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor auxiliar DA - solve_static_linear_system");
      exit(1);
    }

// atualiza valor das condicoes de contorno Dirichlet no vetor de potenciais
    for(i=1;i<=*NC;i++)
     {
      DA[NO_CC[i]] = POT_CC[i];
     }

//................................................................................
//................................................................................
    iccg(*NN,MC,COL,DIAGONAL,RES,DA,*ind_col,*linha1, *mat_lin, *itn, *itc_max);
//................................................................................
//................................................................................
    if(*NCA) /* se existe nos com cond. cont. periodicas */
     {
        for(i=1;i<=*NCA;i++)
        {
         if(TIPO[i] == 5) DA[NO_CA_POT[i]] = - DA[NO_CA_REF[i]];
         else if(TIPO[i] == 4)  DA[NO_CA_POT[i]] = DA[NO_CA_REF[i]];
        }

        for(i=1;i<=*NCA;i++)
        {
         if(TIPO[i] == 5) DA[NO_CA_POT[i]] = - DA[NO_CA_REF[i]];
         else if(TIPO[i] == 4)  DA[NO_CA_POT[i]] = DA[NO_CA_REF[i]];
        }

     }

     if(*NCF)
      {
        for(i=1;i<=*NCF;i++)
         DA[NO_CF_POT[i]] = DA[NO_CF_REF[i]];
      }


    for(i=0;i<=*NN;i++)
    {
        A[i] = DA[i]; //atualiza��o de vetor de potencial eletrico a ser devolvido
    }

//atualiza�ao dos n�s com condi��o de contorno




    if(DA) free(DA);

}// end of solve_static_linear_system

#if defined ____PIC____
//===============================================================================================================================
// compute_electric_field_1d
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:    - Calcula o campo el�trico em elementos unidimensionais
//
//
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int     NE            :  numero de elementos da malha de EF
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1     NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2     NE+1
//  1-based     double* x             :  ponteiro para vetor com as coordenadas dos pontos   NN+1
//  1-based     double* A              :  ponteiro para vetor com os potenciais calculados    NN+1
//                                        (saida do iccg)
//
// SA�DA:
//  0-based     double* Ex             :  ponteiro para vetor com os campos eletricos         NN
//
//===============================================================================================================================
//void compute_electric_field_1d(int NE,int *E1,int *E2,double *x,double *A, double *Ex)
void compute_electric_field_1d(int *NE,int *E1,int *E2,double *x,double *A, double *Ex)
{
    double *E_field = NULL;
    int     i;

    E_field = (double *)calloc( (*NE+2) , sizeof(double) );
    if(!E_field)
    {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor auxiliar E_field - compute_electric_field_1d");
      exit(1);
    }

//.......................................................................................................
//.......................................................................................................
    compute_electric_field_1d_internal(*NE,E1,E2,x,A,E_field);
//.......................................................................................................
//.......................................................................................................


    for(i=0;i<*NE;i++)
    {
        Ex[i] = E_field[i+1]; //atualiza��o de vetor de campo eletrico devolvido
    }

    if(E_field) free(E_field);

}// end of compute_electric_field_1d

//===============================================================================================================================
// compute_electric_field_2d
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:    - Calcula o campo el�trico em elementos unidimensionais
//
//
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int     NE            :  numero de elementos da malha de EF
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1     NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2     NE+1
//  1-based     int*    E23           :  ponteiro para vetor com os �ndices do vertice 2     NE+1
//  1-based     double* x             :  ponteiro para vetor com as coordenadas dos pontos   NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas dos pontos   NN+1
//  1-based     double* A              :  ponteiro para vetor com os potenciais calculados    NN+1
//                                        (saida do iccg)
//
// SA�DA:
//  0-based     double* Ex             :  ponteiro para vetor com os campos eletricos         NN
//
//===============================================================================================================================
double compute_electric_field_2d(int *NE,double *prop_fis,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex)
{
    double *E_field = NULL;
    int     i;
    double  energia_total = 0.0;

    E_field = (double *)calloc( (*NE+2) , sizeof(double) );
    if(!E_field)
    {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor auxiliar E_field - compute_electric_field_1d");
      exit(1);
    }

//.......................................................................................................
//.......................................................................................................
    energia_total = compute_electric_field_2d_internal(*NE,*prop_fis,E1,E2,E3,x,y,A,E_field);
//.......................................................................................................
//.......................................................................................................


    for(i=0;i<*NE;i++)
    {
        Ex[i] = E_field[i+1]; //atualiza��o de vetor de campo eletrico devolvido
    }

    if(E_field) free(E_field);

    return energia_total;

}// end of compute_electric_field_2d

//===============================================================================================================================
// compute_electric_field_2d_in_node
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:    - Calcula o campo el�trico em nos de elementos bidimensionais
//
//
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int     NE            :  numero de elementos da malha de EF
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1     NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2     NE+1
//  1-based     int*    E23           :  ponteiro para vetor com os �ndices do vertice 2     NE+1
//  1-based     double* x             :  ponteiro para vetor com as coordenadas dos pontos   NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas dos pontos   NN+1
//  1-based     double* A              :  ponteiro para vetor com os potenciais calculados    NN+1
//                                        (saida do iccg)
//
// SA�DA:
//  0-based     double* Ex             :  ponteiro para vetor com os campos eletricos         NN
//
//===============================================================================================================================
void compute_electric_field_2d_in_node(int *NE,int *NN,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex_node,double *Ey_node)
{

    double *Ex = NULL, *Ey = NULL;
    int     i;

    Ex = (double *)calloc( (*NN+2) , sizeof(double) );
    if(!Ex)
    {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor auxiliar Ex - compute_electric_field_2d_in_node");
      exit(1);
    }

    Ey = (double *)calloc( (*NN+2) , sizeof(double) );
    if(!Ey)
    {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor auxiliar Ey - compute_electric_field_2d_in_node");
      exit(1);
    }

  compute_electric_field_2d_internal_in_node(*NE,E1,E2,E3,x,y,A,Ex,Ey);


    for(i=0;i<*NN;i++)
    {
        Ex_node[i] = Ex[i+1]; //atualiza��o de vetor de campo eletrico devolvido
        Ey_node[i] = Ey[i+1]; //atualiza��o de vetor de campo eletrico devolvido
    }

    if(Ex) free(Ex);
    if(Ey) free(Ey);


}

#endif // ____PIC____

//===============================================================================================================================
// build_fill_and_solve_generic_system
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Fevereiro/2001
//===============================================================================================================================
// Descri��o:
//                - Monta estrutura das matrizes
//                - Calcula os elementos da matriz e preenche a estrutura
//                - Introduz as condi��es de contorno
//                - resolve sistema linear ou n�o linear
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:                                                                                 dimens�o utilizada        Obs:
//              int*     NN            :  numero de pontos da malha de EF
//              int*     NE            :  numero de elementos da malha de EF
//              int*     NF            :  numero de elementos com fonte
//              int*     NCA           :  numero de nos com cc periodica
//              int*     NCF           :  numero de nos com cc float
//              int*     NC            :  numero de nos com cc dirichlet
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    FONTE_EL      :  ponteiro para vetor com os �ndices de elementos com fonte    NF+1
//  1-based     double* FONTE_VAL     :  ponteiro para vetor com os valores de fonte                  NF+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//              int*    tipo_estudo   :  flag que indica qual tipo de estudo
//                                          1 - eletrost�tico  2 - eletrocin�tico  3 - magnetostatico
//                int *    itn_max          : numero maximo de itera�oes do NR
//                int *    itc_max          : numero maximo de itera�oes do iccg
//                materiais_struct* mat : ponteiro para struct com dados de materiais
//                int *    nos_alocados  : dimens�o dos vetores MC e COL
//                double * precisao     : precisao do NR
//
// SA�DA:
//              int*    ind_col       :  quantidade de posicoes de MC  e COL utilizadas
//  0-based        int*    COL              :  ponteiro para vetor com os �ndices das colunas com           determinada ap�s
//                                       elementos n�o nulos                                             a montagem
//  1-based     int*    DIAGONAL      :  ponteiro para vetor com os �ndices das diagonais             NN+1
//  0-based     double* MC              :     ponteiro para vetor com os elementos n�o nulos da            determinada ap�s
//                                       matriz                                                        a montagem
//  1-based     double* RES            :  ponteiro para vetor do 2. membro do sistema                  NN+1
//  1-based     double* A              :  ponteiro para vetor dde potenciais                           NN+1
//
//
//===============================================================================================================================

void build_fill_and_solve_generic_system(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC, int *NO_CA_REF, int *NO_CA_POT,int *TIPO,
                                int *NO_CF_REF, int *NO_CF_POT,int *FONTE_EL,double *FONTE_VAL,
                                int *mat_lin, int *itn_max, int *itc_max, materiais_struct *mat, int *tipo_estudo,
                                double *MC, int *COL, int *DIAGONAL, double *RES, int *nos_alocados, double *precisao,
                                double *A)
{

    int        num_elem_nodes = 3; //numero de nos do elemento
    int        ind_col;


    monta_col(*NN,*NE,*NCA,*NCF,num_elem_nodes,E1,E2,E3,NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF,NO_CF_POT,*nos_alocados,
                  COL,DIAGONAL,&ind_col);


   solve_non_linear_system_internal(*NN,*NE,*NCA,*NCF,*NC,*NF,x,y,E1,E2,E3,MEIO,angulo,
                                NO_CC,POT_CC,NO_CA_REF,NO_CA_POT,TIPO,NO_CF_REF, NO_CF_POT,
                                FONTE_EL,FONTE_VAL,*mat_lin,*itn_max,*itc_max,*tipo_estudo,
                                mat,MC,COL,DIAGONAL,RES, *nos_alocados,ind_col,*precisao, A);



}//end of build_fill_and_solve_generic_system

}//fim de extern C
//...........................................................................................

//...........................................................................................
//...........................................................................................

#if defined ____PIC____

void initialization_1d_electrostatic(int num_mesh_points,
                                     double dx,double *x, int *E1, int *E2, int *NO_CA_REF, int *NO_CA_POT,
                                     int *TIPO,int *NO_CF_REF,int *NO_CF_POT,
                                     int NN,int NE,int NCA,int NCF,int NC)
{
    int     i;

//    double *source = NULL;



#if defined __WITH_ALLOCATION_NMA__
//----------------------------------------------------------------------------------------------
//alocao de memoria
//----------------------------------------------------------------------------------------------
//  printf ("\niniciou calculo de potenciais por MEF\n");

     x = (double *)malloc( (NN+2) * sizeof(double) );
     if(!x)
     {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor de coordenadas x");
      exit(1);
     }


     E1 = (int *) malloc( (NE+3) * sizeof(int) );
     if(!E1)
     {
      printf("\nerro na alocacao de memoria - vetor de vertice E1");
      exit(1);
     }

     E2 = (int *) malloc( (NE+3) * sizeof(int) );
     if(!E2)
     {
      printf("\nerro na alocacao de memoria - vetor de vertice E2");
      exit(1);
     }

     NO_CA_REF = (int *) malloc( (NCA+2) * sizeof(int) );
     if(!NO_CA_REF)
     {
      printf("\nerro na alocacao de memoria - vetor  NO_CA_REF");
      exit(1);
     }

     NO_CA_POT = (int *) malloc( (NCA+2) * sizeof(int) );
     if(!NO_CA_POT)
     {
      printf("\nerro na alocacao de memoria - vetor  NO_CA_POT");
      exit(1);
     }

     TIPO = (int *)malloc( (NCA+2) * sizeof(int) );
     if(!TIPO)
     {
      printf("\nerro na alocacao de memoria - vetor TIPO");
      exit(1);
     }

     NO_CF_REF = (int *) malloc( (NCA+2) * sizeof(int) );
     if(!NO_CF_REF)
     {
      printf("\nerro na alocacao de memoria - vetor  NO_CF_REF");
      exit(1);
     }

     NO_CF_POT = (int *) malloc( (NCA+2) * sizeof(int) );
     if(!NO_CF_POT)
     {
      printf("\nerro na alocacao de memoria - vetor  NO_CF_POT");
      exit(1);
     }


     FONTE_EL = (int *) malloc( (NE+3) * sizeof(int) );
     if(!FONTE_EL)
     {
      printf("\nerro na alocacao de memoria - vetor FONTE_EL");
      exit(1);
     }

     FONTE_VAL = (double *) malloc( (NE+3) * sizeof(double) );
     if(!FONTE_VAL)
     {
      printf("\nerro na alocacao de memoria - vetor  FONTE_VAL");
      exit(1);
     }

     RES = (double *)malloc( (NN+2) * sizeof(double) );
     if(!RES)
     {
    //  mensagem_erro(1);
      printf("\nerro na alocacao de memoria - vetor de ações");
      exit(1);
     }
//----------------------------------------------------------------------------------------------
#endif // termina #if defined __WITH_ALLOCATION_NMA__


    // source � local
//     source = (double *) malloc( (NE+3) * sizeof(double) );
//     if(!source)
//     {
//      printf("\nerro na alocacao de memoria - vetor  source");
//      exit(1);
//     }
//----------------------------------------------------------------------------------------------
// montagem dos vetores de dados
//----------------------------------------------------------------------------------------------

// calculo e constru��o do vetor de coordenadas x
    for(i=1;i<=NN;i++)
    {
        x[i] = i*dx;
    }

// constru��o dos vetores de elementos
    for(i=1;i<=NE;i++)
    {
        E1[i] = i;
        E2[i] = i+1;
    }

// constru��o dos vetores de condi��o de contorno
    NO_CA_REF[1] =  1;
    NO_CA_POT[1] =  NN;
    TIPO[1]      =  4; // c�clica

// constru��o dos vetores de fontes
//    printf("\nem inicializacao 1d");
#if 0 //� feito em fill
    for(i=0;i<NE;i++)
    {
        source[i+1] = charge_density[i]; // so para garantir a consist�ncia dos indices
    }

//    for(i=0;i<num_mesh_points;i++)
//    {
//     printf("\ncharge[%d] = %g",i,charge_density[i]);
//    }

    for(i=0;i<=NE;i++)
    {
        FONTE_EL[i]   = i;
        FONTE_VAL[i]  = -source[i];
    }

    for(i=0;i<=NN;i++)
    {
        RES[i] = 0.0;
    }
    if (source)        free(source);
#endif


}// end of initialization_1d_electrostatic

#endif // ____PIC____


//...........................................................................................
//...........................................................................................



//===============================================================================================================================
// initialization_2d_by_files
//===============================================================================================================================
// Autor:    Nancy Mieko Abe
// Data:    Mar�o/2001
//===============================================================================================================================
// Descri��o:    - le arquivos de dados
//===============================================================================================================================
// Par�metros:
//
// ENTRADA:
//                char    *nome_problema
//                int         mat_lin;
//                                                                                dimens�o utilizada        Obs:
// SA�DA:
//              int     *NN           :  numero de pontos da malha de EF
//              int     *NE           :  numero de elementos da malha de EF
//              int     *NF           :  numero de elementos com fonte
//              int     *NCA          :  numero de nos com cc periodica
//              int     *NCF          :  numero de nos com cc float
//  1-based     double* x             :  ponteiro para vetor com as coordenadas x dos pontos          NN+1
//  1-based     double* y             :  ponteiro para vetor com as coordenadas y dos pontos          NN+1
//  1-based     int*    E1              :  ponteiro para vetor com os �ndices do vertice 1              NE+1
//  1-based     int*    E2              :  ponteiro para vetor com os �ndices do vertice 2              NE+1
//  1-based     int*    E3              :  ponteiro para vetor com os �ndices do vertice 3              NE+1
//  1-based     int*    MEIO          :  ponteiro para vetor com os �ndices dos materiais             NE+1
//  1-based     int*    NO_CC         :  ponteiro para vetor com os �ndices dos nos com cc Dirichlet  NC+1
//  1-based     double* POT_CC        :  ponteiro para vetor com os valores de cc Dirichlet           NC+1
//  1-based     int*    NO_CA_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc peri�dica   NCA+1
//  1-based     int*    NO_CA_POT     :  ponteiro para vetor com os �ndices dos nos com cc peri�dica  NCA+1
//  1-based     int*    TIPO          :  ponteiro para vetor com o tipo de cc peri�dica               NCA+1
//                                            4:c�clica  5: antic�clica
//  1-based     int*    NO_CF_REF     :  ponteiro para vetor com os �ndices dos nos referencia dos nos com cc float   NCF+1
//  1-based     int*    NO_CF_POT     :  ponteiro para vetor com os �ndices dos nos com cc float      NCF+1
//  1-based     double* angulo        :  ponteiro para vetor com os valores dos angulos de �mas perm. NE+1
//
//===============================================================================================================================

void initialization_2d_by_files(int *NN,int *NE,int *NCA,int *NCF,int *NC,int *NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, char* nome_problema, int *mat_lin,
                                materiais_struct *mat)
{
    int i,j,k,f;
    char nome_geometria[256]= "";
    char nome_elementos[256]= "";
    char nome_meios[256]= "";
    char nome_fontes[256]= "";
    char nome_cc[256]= "";
    char nome_lixo[256];
    char nome_propfis[256] = "propfis.mei";

    double materiais[40][7];
    int    codigos[40];

    FILE *fp;

    strcpy(nome_geometria,nome_problema);
    strcat(nome_geometria,".n1g");

    strcpy(nome_elementos,nome_problema);
    strcat(nome_elementos,".t1g");

    strcpy(nome_meios,nome_problema);
    strcat(nome_meios,".m1g");

    strcpy(nome_fontes,nome_problema);
    strcat(nome_fontes,".f1g");

    strcpy(nome_cc,nome_problema);
    strcat(nome_cc,".c1g");


    fp = fopen(nome_propfis,"r");
    if (fp == NULL) {
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

//    for(i=0;i<NM;i++) materiais[i][0] = materiais[i][0]/8.85e-12;

//================== le arquivos de coordenadas (pontos) ===================

    fp = fopen(nome_geometria,"r");
    if (fp == NULL) {
        printf("\n erro na abertura do arquivo %s", nome_geometria);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
    }


    fscanf(fp,"%d",NN);

    for(i=1;i<=*NN ;i++)
    {
      fscanf(fp,"\n%lg %lg", &x[i],&y[i]);
    }


#if 0
switch(unidade)
{
 case 1     : break;              //metros

 case 2     : for(i=1;i<=NN;i++)
                {
                x[i] = x[i]/100;      // centimetros
                y[i] = y[i]/100;
                }
              break;
 case 3     : for(i=1;i<=NN;i++)
                {
                x[i] = x[i]/1000;     // milimetros
                y[i] = y[i]/1000;
                }
              break;
}

#endif

//    nome_arq.Fecha_Arquivo();
  fclose(fp);



//#if 0

//printf("\n leu x e y");


//===================== le dados do arquivo de triangulos =================
#define FALSE 0
#define TRUE  1

    fp = fopen(nome_elementos,"r");
    if(fp==NULL) {
        printf("\n erro na abertura do arquivo %s", nome_elementos);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
    }

 int lixo;

 fscanf(fp,"%d %d", NE,&lixo);

 for(i=1;i<=*NE;i++) fscanf(fp,"\n%d %d %d %d %d %d",&E1[i],&E2[i],
                                 &E3[i],&lixo,&lixo,&lixo);

 fclose(fp);


//====================== le dados do arquivo de meios =====================
 for(i=0;i<=*NE;i++) angulo[i] = 0;

 fp = fopen(nome_meios,"r");
    if (fp == NULL) {
        printf("\n erro na abertura do arquivo %s", nome_meios);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
 }


 fscanf(fp,"%d", NE);


 for(i=1;i<=*NE;i++)
 {
    fscanf(fp,"\n%d %lf",&MEIO[i],&angulo[i]);
    if(!MEIO[i]) {
        printf("\n erro na leitura do arquivo %s", nome_meios);
        printf("\n MEIO[%d]= %d", i, MEIO[i]);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
    } else {
     int meio_cadastrado = FALSE;
     for(j=0;j<NM;j++) if(MEIO[i] == codigos[j]) meio_cadastrado = TRUE;
     if(MEIO[i] == -1) meio_cadastrado = TRUE;
     if(!meio_cadastrado) {
        printf("\n Atencao!\nMeio nao cadastrado no arquivo de materiais.");
        printf("\n MEIO[%d]= %d", i, MEIO[i]);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
     }
    }
  }

   fclose(fp);

//printf("\n leu triangulos");


// verifica se existe meio nao linear
 *mat_lin = 0; //se so' existirem materiais lineares mat_lin==0
 int mat_lin_aux = *mat_lin;

 for(i=1;i<=*NE;i++)
 {
  if(MEIO[i] > 200)
    {
     for(j=0;j<NM;j++)
      {
        if(codigos[j] == MEIO[i])
         {
          if(materiais[j][2]!=0.0 && materiais[j][3]!=0.0) *mat_lin =1;
          break;
         }
      }

     mat_lin_aux = *mat_lin;
     if(mat_lin_aux==1) break;
    }
 }


//printf("\n verificou se existe material nao linear");




//============= le arquivo de fontes ou densidade de carga ===============
    fp = fopen(nome_fontes,"r");
    if(fp==NULL) {
        printf("\n Atencao: arquivo de fontes nao encontrado.");
    } else {
        fscanf(fp,"%d",NF);
        for(i=1;i<=*NF;i++)
            fscanf(fp,"\n%d %lf",&FONTE_EL[i],&FONTE_VAL[i]);
        fclose(fp);
    }

//printf("\n verificou fontes");


//  if(estudo==1)
//    for(i=1;i<=NF;i++) FONTE_VAL[i] = FONTE_VAL[i]/8.85e-12;

//===================== le arquivo de condicoes de contorno ===============

 int     noc, tipo_cc,ncc;
 double  lixodouble, vcc;
 int     NCA_aux=0, NCF_aux=0, NC_aux=0;

    fp = fopen(nome_cc,"r");
    if(fp==NULL) {
        printf("\n erro na abertura do arquivo %s", nome_cc);
		printf("\n o programa sera encerrado. pressione qualquer tecla.");
		getchar();
        exit(0);
    }

    fscanf(fp,"%d",&noc);

    for(i=1;i<=noc;i++) {
        fscanf(fp,"\n%d %d %lf",&lixo,&tipo_cc,&lixodouble);
        switch(tipo_cc) {
    case 2: NC_aux++;
            break;
    case 3: NCF_aux++;
            break;
    case 4: NCA_aux++;
            break;
    case 5: NCA_aux++;
            break;
  }
 }

  fclose(fp);

  *NC  = NC_aux;
  *NCF = NCF_aux;
  *NCA = NCA_aux;

 fp = fopen(nome_cc,"r");

 fscanf(fp,"%d",&noc);

 j=1;
 k=1;
 f=1;
 for(i=1;i<=noc;i++)
 {
  fscanf(fp,"\n%d %d %lf",&ncc,&tipo_cc,&vcc);
  switch(tipo_cc)
  {
    case 2: NO_CC[j] = ncc;
            POT_CC[j] = vcc;
            j++;
            break;
    case 3: NO_CF_REF[f] = (int) vcc;
            NO_CF_POT[f] = ncc;
            f++;
            break;
    case 4: NO_CA_REF[k] = (int) vcc;
            NO_CA_POT[k] = ncc;
            TIPO[k] = tipo_cc;
            k++;
            break;
    case 5: NO_CA_REF[k] = (int) vcc;
            NO_CA_POT[k] = ncc;
            TIPO[k] = tipo_cc;
            k++;
            break;
    }
 }

 fclose(fp);


}//end of initialization_2d_by_files

//...........................................................................................
//...........................................................................................


//=========== funcao para montagem do vetor COL e DIAGONAL =================

void monta_col(int NN,int NE,int NCA,int NCF,int num_elem_nodes, int *E1,int *E2,int *E3,
               int *NO_CA_REF,int *NO_CA_POT,int *TIPO,int *NO_CF_REF,int *NO_CF_POT, int nos_alocados,
               int *COL,int *DIAGONAL,int *ind_col_aux)
 {
    int ind_col;
    int ind_diag;
    int NO;
    int ind;
    int NO_IGUAL_POT[40]; // guarda todos os nos de mesmo potencial do no de referencia
    int NO_POT;           // guarda no de mesmo potencial que pertence ao elemento

    int flag_pot,flag1,flag2,flag3,flag4,flag5;
    int LIG[1000];
    int no_aux,aux;
    int col_total;      // no. de nos necessarios para MC
    int i,j,k,f,p;
    int E[4];
//    int nos_alocados;

#if defined __WITH_ALLOCATION_NMA__

    { // bloco de allocacao de memoria
         DIAGONAL = (int *)malloc( (NN+2)* sizeof(int) );
         if(!DIAGONAL)
          {
            printf("\nerro na alocacao de memoria - vetor DIAGONAL");
            exit(1);
          }

          nos_alocados = 5*NN;

          COL = (int *)malloc( nos_alocados * sizeof(int) );
          if(!COL)
          {
            printf("\nerro na alocacao de memoria - vetor COL");
            exit(1);
          }
    }
#endif // #if defined __WITH_ALLOCATION_NMA__

//------------ monta vetor coluna  prevendo condicao PERIODICA ------------
  ind_col =0;
  ind_diag=1;
  col_total =1;
  DIAGONAL[1] = 0;
  ind_diag++;

// --------------------- para todos os nos --------------------------------
  for(NO=1;NO<=NN;NO++)
  {
   ind             =  0;
   f               =  0;
   NO_IGUAL_POT[f] = -1;
   NO_POT          = -1;
   flag_pot        =  0;

   LIG[ind]        = NO;

   if(NCA) // verifica se tem nos com condicao de contorno periodicas
    {
     for(i=1;i<=NCA;i++) // para todos os nos com cond. cont. periodicas
      {
       if(NO == NO_CA_POT[i])
        {
         flag_pot = 1; /* no de mesmo pot. */
         break;
        }
      }
     }


  if(!flag_pot) // se flag_pot==0, o no nao tem cond.cont. periodica
   if(NCF) // verifica se existem nos com condicao de contorno float
    {
     for(i=1;i<=NCF;i++) // para todos os nos com cond. cont. float
      {
       if(NO == NO_CF_POT[i])
        {
         flag_pot = 1; /* no de mesmo pot. */
         break;
        }
      }
     }


   if(flag_pot == 0)  // no' nao e' de mesmo potencial
   {
    if(NCA)
    {
     for(i=1;i<=NCA;i++)
     {
      if(NO == NO_CA_REF[i]) // se e' no de referencia
      {
       f++;
       NO_IGUAL_POT[f] = NO_CA_POT[i]; // guarda no de mesmo potencial
       break;                          // na posicao 1
      }
     }
    }

    if(!f) //se f==0 nao e' no' de referencia ciclica, portanto testa float
     {
      if(NCF)
       {
        for(i=1;i<=NCF;i++)
         if(NO == NO_CF_REF[i])
          {
           f++;
           while(NO_CF_REF[f]==NO && i<=NCF)
            {
             NO_IGUAL_POT[f] = NO_CF_POT[i];
             f++;
             i++;
            }
           break;
          }
        } // fecha if(NCF)
      }   // fecha if(!f)

    for(j=1;j<=NE;j++) // para cada elemento faca
    {
     E[1] = E1[j];
     E[2] = E2[j];
     if(num_elem_nodes==3) E[3] = E3[j];

     flag1 = 0;

     for(i=1;i<=num_elem_nodes;i++) // para cada no' do elemento
     {
      if( E[i] == NO ) flag1 = 1;
      else
       {
        for(p=1;p<=f;p++)
         if( E[i] == NO_IGUAL_POT[p])
          {
           flag1  = 1;
           break;
          }
       }
     }


     if(flag1) // NO ou NO_IGUAL_POT[]  pertence ao elemento
     {
      for(k=1;k<=num_elem_nodes;k++) // para cada no' do elemento
      {
       flag2 = 0;
       flag5 = 0;
       no_aux = 0;

       // se o no nao 'e no' de mesmo potencial do no NO, verifica se e'
       // no' de mesmo potencial de outro no', se sim pego o no'o de referencia
         for(p=1;p<=f;p++)
        {
         if( E[k] == NO_IGUAL_POT[p])
         flag5=1;  // E[k] e' no de mesmo potencial de NO
         break;
        }

       // se E[k] nao e' no' de mesmo potencial de NO, verfica se e' no de mesmo
       // potencial de outro no'
       if(!flag5)
       {
        if(NCA)
        {
         for(i=1;i<=NCA;i++)
         {
          if(E[k] == NO_CA_POT[i])
          {
           no_aux = NO_CA_REF[i];
           break;
          }
         }
        }

         if(!no_aux) // se nao e' no de mesmo potencial de ciclica, testa float
          {
           if(NCF)
           {
            for(i=1;i<=NCF;i++)
            {
             if(E[k] == NO_CF_POT[i])
             {
              no_aux = NO_CF_REF[i];
              break;
             }
            }
           }
        }

        if(no_aux == 0) no_aux = E[k];
        for(i=0;i<=ind;i++)
        if(no_aux == LIG[i])
        {
         flag2 =1;
         break;
        }

        if(flag2 == 0)
        if(no_aux > NO) LIG[++ind] = no_aux;

       } // fim do if
      } // fim do for para cada no' do elemento
     } // fim do if(flag1)
    } // fim do for para cada elemento
   } // fim do if(flag_pot == 0)


//---------------------- ordenancao do vetor LIG ------------------------
   do
   {
    flag3 = 0;
    flag4 = 0;
    for( i=0 ; i<ind ; i++)
    {
     aux=LIG[i];
     if( aux > LIG[i+1] )
     {
      LIG[i] = LIG[i+1];
      LIG[i+1]=aux;
      flag4 = 1;
     }
    }

    if(!flag4) flag3 = 1;

   }while( flag3 != 1 );


// guarda os no's de LIG no vetor COL

// numero total de elementos para realocacao

   col_total = ind + 1 + col_total; // no. total de elementos para realocacao

// sao alocados inicialmente 5*NN nos, se col_total for maior do que
// o numero alocado, realoca mais nos

#if defined __WITH_ALLOCATION_NMA__
   if( col_total >= nos_alocados )
   {
    nos_alocados = col_total + 100;
    int *COL_old = COL;
    COL = (int*) realloc( (int*) COL_old , nos_alocados * sizeof(int) );
    if(!COL)
    {
     printf("\nerro na realocacao de memoria - vetor COL");
     exit(1);
    }
   }
#else
    if( col_total >= nos_alocados ) {
	 printf("\n ATENÇÃO: A dimensão do vetor COL é maior do que o alocado.");
	 printf("\n O programa será encerrado. Pressione qualquer tecla.");
	 getchar();
        exit(1);
    }

#endif //#if defined __WITH_ALLOCATION_NMA__

    for(i=0;i<=ind;i++)
   {
    COL[ind_col] = LIG[i];
    ind_col++;
   }

// guarda no vetor DIAGONAL a posicao da diagonal

   if( NO+1 <= NN)
   {
    DIAGONAL[ind_diag] = ind_col;
    ind_diag++;
   }

  } // fim do for para todos os nos

//----------------- termina para todos os nos ------------------------------
#if defined __WITH_ALLOCATION_NMA__

  if(col_total < nos_alocados)
  {
    int *COL_old = COL;
   COL = (int*) realloc( (int*) COL , (col_total+5) * sizeof(int) );
   if(!COL)
   {
     printf("\nerro na realocacao de memoria - vetor COL");
     exit(1);
    }
  }

#endif  //#if defined __WITH_ALLOCATION_NMA__

 *ind_col_aux = ind_col;

// guarda resultado no arquivo RESP ----------------------------------------

/*  if( (fp == fopen("resp","w")) == NULL)
    {
     printf("erro na abertura do arquivo RESP");
     exit(0);
    }

  for(i=0;i<ind_col;i++) fprintf(fp,"COL[%d] = %d\n",i,COL[i]);
  for(i=0;i<ind_diag;i++) fprintf(fp,"DIAGONAL[%d] = %d\n",i,DIAGONAL[i]);
  fclose(fp);
*/

}// termina funcao de montagem dos vetores coluna e diagonal
//...........................................................................................
//...........................................................................................

//...........................................................................................
//...........................................................................................
#if defined ____PIC____

void elem_1d_matrix(int NN,int NE,int NCA,int NF,int NCF,int NC,int num_elem_nodes,
					int *E1,int *E2,int *E3, int *NO_CA_REF,int *NO_CA_POT,int *TIPO,int *NO_CF_REF,int *NO_CF_POT,
					double *MC,int *COL,int *DIAGONAL,double *RES,int ind_col, double *x, double *y,
					int *FONTE_EL,double *FONTE_VAL,int *MEIO,double permittivity_1d)
{
 int	i,j,k,l;
 int    flag_ref;
 double size=0.0;
 double Sij;
 double Ifonte[4];
 double fonte;
 int    E[4];
 int	no_global[4];
 int	cond[4];


 int   cond1;

//  printf("\n\nentrou elem_1d_local_matrix");
  //printf("\n permittivity_1d = %g ", permittivity_1d);
  //printf("\n NCA = %d", NCA);

 for(l=1;l<=NE;l++)
 {

  E[1] = E1[l];
  E[2] = E2[l];

  //calcula comprimento do elemento (genérico, apesar de ser em princípio uma malha
  //									regular)

  size = x[E[2]] - x[E[1]];

  if(!size) printf ("\n elemento com comprimento nulo");
//  printf("\n size = %g ", size);

// guarda no vetor auxiliar os no's do elemento, ja substituindo os no's
// de mesmo potencial pelos no's de referencia e guardando os tipos de
// condicao de contorno

  for(i=1;i<=num_elem_nodes;i++)
  {
	no_global[i] = 0;
	cond[i] = 0;
  }

  flag_ref=0;

  for(i=1;i<=num_elem_nodes;i++)
  {
   if(NCA)
   {
	for(k=1;k<=NCA;k++)
	{
	 if(E[i] == NO_CA_POT[k])      // se no' e' no' de mesmo
	 {                             // potencial, substitui por
	  no_global[i] = NO_CA_REF[k]; // no' de referencia e
	  cond[i] = TIPO[k];		   // guarda tipo de cond.
	  flag_ref=1;                  // de contorno
	  break;
	 }
	} // fecha for k
   } // fecha if(NCA)

   if(!flag_ref)
   {
	if(NCF)
	for(k=1;k<=NCF;k++)
	{
	 if(E[i] == NO_CF_POT[k])      // se no' e' no' de mesmo
	 {                             // potencial, substitui por
	  no_global[i] = NO_CF_REF[k]; // no' de referencia
	  break;
	 }
	} // fecha for k
   } // fecha if(NCA)


   if(no_global[i] == 0) no_global[i] = E[i];

  } // fecha for i para verificacao se e' no com cond. ciclica ou float

//  for(i=1;i<=num_elem_nodes;i++)
//  {
//	printf("\nno_global[%d] = %d",i,no_global[i]);
//	printf("\ncond[%d] = %d",i,cond[i]);
//
//  }
  // calculo dos elementos da matriz local

 for(i=1;i<=num_elem_nodes;i++)
 {
  for(j=1;j<=num_elem_nodes;j++)
  {
   if(j>=i)
   {

	if(size) Sij = permittivity_1d/(size);

//	printf("\nSij = %lg", Sij);

	if(i!=j) Sij = -Sij; //so para elemento 1d

	if(cond[i]==5 || cond[j] ==5)
	{
	 if(cond[i] == cond[j]) cond1=0;
	 else cond1 = 5;
	}
	else cond1=0;

	if(i!=j) if(cond1==5) Sij = -Sij;

	k = busca(no_global[i],no_global[j],DIAGONAL,COL,ind_col);

//	printf("\nk = %d", k);

	MC[k] = MC[k] + Sij;

//	RES[no_global[i]] = - FONTE_VAL[i]; //*NMA* verificar sinal

   }
  }
 }

  for(i=1;i<=num_elem_nodes;i++) Ifonte[i] = 0;

  if(NF) // verifica se elemento tem fonte
  {

//    printf("\n size = %g ", size);
	 fonte =( (FONTE_VAL[l] * size)/2 );

	 for(i=1;i<=num_elem_nodes;i++)
	 {
	  if(cond[i] == 5) Ifonte[i] = Ifonte[i] + fonte;
	  else Ifonte[i] = Ifonte[i] + fonte;

	  RES[no_global[i]] = RES[no_global[i]] - Ifonte[i];

	 }

  } // fecha if(NF)



//  if(NF) // verifica se elemento tem fonte
//	{
//	  if(cond[i]) RES[no_global[i]] = RES[no_global[i]] - FONTE_VAL[i];
//	  else RES[no_global[i]] = - FONTE_VAL[i];

//	  printf("\ncond[%d] = %d",i,cond[i]);

//	} // fecha if(NF)
// }//fecha for i



 } // termina for para todos os elementos



//--------------------------------------------------------------------------


} // fim de elem_1d_local_matrix
//...........................................................................................


void monta_mc_pic(int NN,int NE,int NF,int NCA,int NCF,int NC,double *x,double *y,
			  int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
			  int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo, int tipo_estudo,
			  double *MC,int *COL,int *DIAGONAL, double *RES, double *A, int ind_col,int mat_lin,int itn,
			  double permittivity)
{
 int		i,j,k,t,l;
// int		tipo_meio;
 int		flag_ref;
// int		indice;
 int		E[4];
 double		b[4], c[4];
 double		delta, rb, Bx, By;
 int        simetria = 1; //simetria plana
 int		no_global[4];
 int		cond[4];
 double     Ifonte[4];
 double     fonte;
 double		prop_fis;
// double		k1f,k2f,k3f;
// double		k1i,k2i,k3i;
// double		ni2, Hc;



// normalizacao de eps
// for(i=0;i<NM;i++) materiais[i][0] = materiais[i][0]/8.85e-12;



//*****************************************************************

//---------------- calcula elementos da matriz compacta --------------------

 for(l=1;l<=NE;l++)
 {

  E[1] = E1[l];
  E[2] = E2[l];
  E[3] = E3[l];

  b[1] = y[ E2[l] ] - y[ E3[l] ];
  b[2] = y[ E3[l] ] - y[ E1[l] ];
  b[3] = y[ E1[l] ] - y[ E2[l] ];

  c[1] = x[ E3[l] ] - x[ E2[l] ];
  c[2] = x[ E1[l] ] - x[ E3[l] ];
  c[3] = x[ E2[l] ] - x[ E1[l] ];

  delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );

  rb = ( x[ E1[l] ] + x[ E2[l] ] + x[ E3[l] ] ) / 3;

  if(simetria == 2) delta = delta*rb;

  Bx = 0;
  By = 0;

// guarda no vetor auxiliar os no's do elemento, ja substituindo os no's
// de mesmo potencial pelos no's de referencia e guardando os tipos de
// condicao de contorno

  for(i=1;i<=3;i++)
  {
	no_global[i] = 0;
   cond[i] = 0;
  }

	flag_ref=0;

  for(i=1;i<=3;i++)
  {
   if(NCA)
   {
	for(k=1;k<=NCA;k++)
	{
	 if(E[i] == NO_CA_POT[k])      // se no' e' no' de mesmo
	 {                             // potencial, substitui por
	  no_global[i] = NO_CA_REF[k]; // no' de referencia e
	  cond[i] = TIPO[k];		   // guarda tipo de cond.
	  flag_ref=1;                  // de contorno
	  break;
	 }
	} // fecha for k
   } // fecha if(NCA)

   if(!flag_ref)
   {
	if(NCF)
	for(k=1;k<=NCF;k++)
	{
	 if(E[i] == NO_CF_POT[k])      // se no' e' no' de mesmo
	 {                             // potencial, substitui por
	  no_global[i] = NO_CF_REF[k]; // no' de referencia
	  break;
	 }
	} // fecha for k
   } // fecha if(NCA)


   if(no_global[i] == 0) no_global[i] = E[i];

  } // fecha for i para verificacao se e' no com cond. ciclica ou float


  if(tipo_estudo==3) // *NMA* so' para caso magnetostatico nao linear
  {
   Bx = c[1]*A[ E[1] ] + c[2]*A[ E[2] ] + c[3]*A[ E[3] ];
   Bx = Bx/(2*delta);
	By = b[1]*A[ E[1] ] + b[2]*A[ E[2] ] + b[3]*A[ E[3] ];
   By = -By/(2*delta);
  }


  for(t=1;t<=3;t++) Ifonte[t] = 0;

//  if(NF) // verifica se elemento tem fonte
//  {
//   for(i=1;i<=NF;i++)
//   {
//	if(l == FONTE_EL[i])
//	{
//	 fonte =( (FONTE_VAL[i] * delta)/3 );
//	 for(j=1;j<=3;j++)
//	 {
//	  if(cond[j] == 5) Ifonte[j] = Ifonte[j] + fonte;
//	  else Ifonte[j] = Ifonte[j] + fonte;
//	 }
//	 break;
//	}// fecha if elemento tem fonte
//   } // fecha for para verificacao se existe fonte
//  } // fecha if(NF)

//*NMA ESPECIFICO PARA PIC 2D ELETROSTATICO: fonte dado no ponto!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Ifonte[1] = FONTE_VAL[E[1]];
//	Ifonte[2] = FONTE_VAL[E[2]];
//	Ifonte[3] = FONTE_VAL[E[3]];

  prop_fis = permittivity;
  linear(prop_fis,&b[0],&c[0],delta,&cond[0],mat_lin,&E[0],&no_global[0],&Ifonte[0],MC,A,RES,DIAGONAL,COL,ind_col);



/*

// verfica qual meio e calcula ---------------------------------
if(MEIO[l]>0)
{                                             // meio == -1, buraco

 if (MEIO[l]<=100) tipo_meio = 1;             // isolante
 else if(MEIO[l]<=200) tipo_meio = 2;         // condutor
		else if(MEIO[l]<=300) tipo_meio = 3;    // ferromagnetico
			  else if(MEIO[l]>300) tipo_meio =4; // ima

 for(i=0;i<NM;i++)
  if(codigos[i] == MEIO[l])
	{
	indice = i;
	break;
	}


 switch(tipo_meio)
  {
	case 1   : switch(tipo_estudo)
				  {
					case 1 : prop_fis = materiais[indice][0];///8.85e-12;
								break;

//					case 2 : printf("\npropriedade nao coerente com tipo de estudo");
//								printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//								break;
					case 3 : prop_fis = materiais[indice][4];
								break;
				  }

//				  linear();
				  linear(prop_fis,&b[0],&c[0],delta,&cond[0],mat_lin,&E[0],&no_global[0],&Ifonte[0],MC,A,RES,DIAGONAL,COL,ind_col);
				  break;

	case 2   : switch(tipo_estudo)
				  {
//					case 1 : printf("\npropriedade nao coerente com tipo de estudo");
//								printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//								break;
					case 2 : prop_fis = materiais[indice][1];
								break;

					case 3 : prop_fis = materiais[indice][4];
								break;
				  }

//				  linear();
				  linear(prop_fis,&b[0],&c[0],delta,&cond[0],mat_lin,&E[0],&no_global[0],&Ifonte[0],MC,A,RES,DIAGONAL,COL,ind_col);
				  break;


	case 3   : switch(tipo_estudo)
				  {
//					case 1 : printf("\npropriedade nao coerente com tipo de estudo");
//								printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//								break;
					case 2 : prop_fis = materiais[indice][1]; //*NMA* 21/02/2001: Verificar isto, pois prop_fis NAO é utilizado em nao_linear
							 break;

					case 3 : k1f = materiais[indice][2];
							 k2f = materiais[indice][3];
							 k3f = materiais[indice][4];
							 break;
				  }

				  nao_linear(Bx,By,k1f,k2f,k3f,&b[0],&c[0],delta,&cond[0],mat_lin,itn,A,&E[0],MC,&no_global[0],RES,DIAGONAL,COL,ind_col);
				  break;

	case 4  : switch(tipo_estudo)
				 {
				  case 3  : k1i = materiais[indice][2];
							k2i = materiais[indice][3];
							k3i = materiais[indice][4];
							ni2 = materiais[indice][5];
							Hc  = materiais[indice][6];
							break;
				  default : printf("\npropriedade nao coerente com tipo de estudo");
							printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
								break;
				 }
				 anisotropico(l,Bx,By,angulo,Hc,ni2,k1i,k2i,k3i,itn,delta,&b[0],&c[0],&cond[0],mat_lin,A,&no_global[0],&E[0],MC,RES,DIAGONAL,COL,ind_col);
				 break;
  }

 }// fecha if(meio>0)
*/


 } // termina for para todos os elementos

//--------------------------------------------------------------------------

}// termina monta_mc_pic
#endif //____PIC____


void monta_mc(int NN,int NE,int NF,int NCA,int NCF,int NC,double *x,double *y,
              int *E1,int *E2,int *E3, int *MEIO,int *FONTE_EL,double *FONTE_VAL,int *NO_CC,double *POT_CC,
              int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,double *angulo, int tipo_estudo,
              double *MC,int *COL,int *DIAGONAL, double *RES, double *A, int ind_col,int mat_lin,int itn,
              materiais_struct *mat)
{
    int        i,j,k,t,l;
    int        tipo_meio;
    int        flag_ref;
    int        indice = 0;
    int        E[4];
    double        b[4], c[4];
    double        delta, rb, Bx, By;
    int        simetria = 1; //simetria plana
    int        no_global[4];
    int        cond[4];
    double     Ifonte[4];
    double     fonte;
    double        prop_fis = 0.0;
    double        k1f = 0.0, k2f = 0.0, k3f = 0.0;
    double        k1i = 0.0, k2i = 0.0, k3i = 0.0;
    double        ni2 = 0.0, Hc = 0.0;

    //para guardar dados do arquivo propfis.mei
    int        codigos[40];
    double        materiais[40][7];
    int        NM;    //numero de materiais no arquivo prop_fis.mei

    NM = mat->NM ;

    for(i=0;i<NM;i++) {
        codigos[i] = mat->codigos[i];
        materiais[i][0] = mat->materiais[i][0];
        materiais[i][1] = mat->materiais[i][1];
        materiais[i][2] = mat->materiais[i][2];
        materiais[i][3] = mat->materiais[i][3];
        materiais[i][4] = mat->materiais[i][4];
        materiais[i][5] = mat->materiais[i][5];
        materiais[i][6] = mat->materiais[i][6];
    }

    // Calcula elementos da matriz compacta
    for(l=1;l<=NE;l++) {

        E[1] = E1[l];
        E[2] = E2[l];
        E[3] = E3[l];

        b[1] = y[ E2[l] ] - y[ E3[l] ];
        b[2] = y[ E3[l] ] - y[ E1[l] ];
        b[3] = y[ E1[l] ] - y[ E2[l] ];

        c[1] = x[ E3[l] ] - x[ E2[l] ];
        c[2] = x[ E1[l] ] - x[ E3[l] ];
        c[3] = x[ E2[l] ] - x[ E1[l] ];

        delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );

        rb = ( x[ E1[l] ] + x[ E2[l] ] + x[ E3[l] ] ) / 3;

        if(simetria == 2) delta = delta*rb;

        Bx = 0;
        By = 0;

        // guarda no vetor auxiliar os no's do elemento, ja substituindo os no's
        // de mesmo potencial pelos no's de referencia e guardando os tipos de
        // condicao de contorno

        for(i=1;i<=3;i++) {
            no_global[i] = 0;
            cond[i] = 0;
        }

        flag_ref=0;

        for(i=1;i<=3;i++) {
            if(NCA) {
                for(k=1;k<=NCA;k++) {
                    if(E[i] == NO_CA_POT[k]) {     // se no' e' no' de mesmo potencial, substitui por
                        no_global[i] = NO_CA_REF[k]; // no' de referencia e
                        cond[i] = TIPO[k];           // guarda tipo de cond.
                        flag_ref=1;                  // de contorno
                        break;
                    }
                }
            }

            if(!flag_ref) {
                if(NCF)
                    for(k=1;k<=NCF;k++) {
                        if(E[i] == NO_CF_POT[k]) {     // se no' e' no' de mesmo potencial, substitui por
                            no_global[i] = NO_CF_REF[k]; // no' de referencia
                            break;
                        }
                    }
            }

            if(no_global[i] == 0)
                no_global[i] = E[i];

        } // fecha for i para verificacao se e' no com cond. ciclica ou float

  if(tipo_estudo==3) // *NMA* so' para caso magnetostatico nao linear
  {
   Bx = c[1]*A[ E[1] ] + c[2]*A[ E[2] ] + c[3]*A[ E[3] ];
   Bx = Bx/(2*delta);
    By = b[1]*A[ E[1] ] + b[2]*A[ E[2] ] + b[3]*A[ E[3] ];
   By = -By/(2*delta);
  }


  for(t=1;t<=3;t++) Ifonte[t] = 0;

  if(NF) // verifica se elemento tem fonte
  {
   for(i=1;i<=NF;i++)
   {
    if(l == FONTE_EL[i])
    {
     fonte =( (FONTE_VAL[i] * delta)/3 );
     for(j=1;j<=3;j++)
     {
      if(cond[j] == 5) Ifonte[j] = Ifonte[j] + fonte;
      else Ifonte[j] = Ifonte[j] + fonte;
     }
     break;
    }// fecha if elemento tem fonte
   } // fecha for para verificacao se existe fonte
  } // fecha if(NF)


// verfica qual meio e calcula ---------------------------------
if(MEIO[l]>0)
{                                             // meio == -1, buraco

 if (MEIO[l]<=100) tipo_meio = 1;             // isolante
 else if(MEIO[l]<=200) tipo_meio = 2;         // condutor
        else
            if(MEIO[l]<=300)
                tipo_meio = 3;    // ferromagnetico
              else if(MEIO[l]>300) tipo_meio =4; // ima

 for(i=0;i<NM;i++)
 {
  if(codigos[i] == MEIO[l])
    {
        indice = i;
        break;
    }
 }


 switch(tipo_meio)
  {
    case 1   : switch(tipo_estudo)
                  {
                    case 1 : prop_fis = materiais[indice][0]/8.85e-12;
                             for(t=1;t<=3;t++) Ifonte[t] = Ifonte[t]/8.85e-12;
                                break;

//                    case 2 : printf("\npropriedade nao coerente com tipo de estudo");
//                                printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//                                break;
                    case 3 : prop_fis = materiais[indice][4];
                                break;
                  }

//                  linear();
                  linear(prop_fis,&b[0],&c[0],delta,&cond[0],mat_lin,&E[0],&no_global[0],&Ifonte[0],MC,A,RES,DIAGONAL,COL,ind_col);
                  break;

    case 2   : switch(tipo_estudo)
                  {
//                    case 1 : printf("\npropriedade nao coerente com tipo de estudo");
//                                printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//                                break;
                    case 2 : prop_fis = materiais[indice][1];
                                break;

                    case 3 : prop_fis = materiais[indice][4];
                                break;
                  }

//                  linear();
                  linear(prop_fis,&b[0],&c[0],delta,&cond[0],mat_lin,&E[0],&no_global[0],&Ifonte[0],MC,A,RES,DIAGONAL,COL,ind_col);
                  break;


    case 3   : switch(tipo_estudo)
                  {
//                    case 1 : printf("\npropriedade nao coerente com tipo de estudo");
//                                printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
//                                break;
                    case 2 : prop_fis = materiais[indice][1]; //*NMA* 21/02/2001: Verificar isto, pois prop_fis NAO � utilizado em nao_linear
                             break;

                    case 3 : k1f = materiais[indice][2];
                             k2f = materiais[indice][3];
                             k3f = materiais[indice][4];
                             break;
                  }

                  nao_linear(Bx,By,k1f,k2f,k3f,&b[0],&c[0],delta,&cond[0],mat_lin,itn,A,&E[0],MC,&no_global[0],RES,DIAGONAL,COL,ind_col);
                  break;

    case 4  : switch(tipo_estudo)
                 {
                  case 3  : k1i = materiais[indice][2];
                            k2i = materiais[indice][3];
                            k3i = materiais[indice][4];
                            ni2 = materiais[indice][5];
                            Hc  = materiais[indice][6];
                            break;
                  default : printf("\npropriedade nao coerente com tipo de estudo");
                            printf("\nMEIO = %d, tipo de estudo = %d",MEIO[l],tipo_estudo);
                                break;
                 }
                 anisotropico(l,Bx,By,angulo,Hc,ni2,k1i,k2i,k3i,itn,delta,&b[0],&c[0],&cond[0],mat_lin,A,&no_global[0],&E[0],MC,RES,DIAGONAL,COL,ind_col);
                 break;
  }

 }// fecha if(meio>0)

 } // termina for para todos os elementos

//--------------------------------------------------------------------------

}// termina monta_mc

#if 0
//...........................................................................................
void solve(void)
{
  int i, itnc;
  /* variaveis para medida de tempo */
  struct  timeb  tempo;
  double  tempo_col, tempo_mc[21], tempo_cond[21], tempo_iccg[21];
  double  ta,tb;

//----------------------------------------------------------------------------------------------
//aloca��o de mem�ria
//----------------------------------------------------------------------------------------------
  MC   = (double *)calloc( (ind_col+1), sizeof(double) );
  A    = (double *)calloc( (NN+2), sizeof(double) );
  DA   = (double *)calloc( (NN+2), sizeof(double) );
  RES  = (double *)calloc( (NN+2), sizeof(double) );
  if(!MC || !A || !DA || !RES )
  {
    printf("\nerro na alocacao de memoria de vetor (MC, A, DA ou RES)");
    exit(0);
  }

  U = (double *)malloc( ind_col* sizeof(double) );
  yi = (double *)malloc( (NN+1)*sizeof(double) );
  h = (double *)malloc( (NN+1)*sizeof(double) );
  d = (double *)malloc( (NN+1)*sizeof(double) );
  gt = (double *)malloc( (NN+1)*sizeof(double) );
  if( !U || !yi || !h || !d || !gt)
  {
    printf("\nerro na alocacao de memoria de vetor (U, yi, h, d ou gt)");
    exit(0);
  }

  while( itn <= itn_max )
  {

    for(i=0;i<=ind_col;i++) if(COL[i]<0) COL[i] = -COL[i];
    for(i=0;i<=ind_col;i++) *(MC + i) = 0;
    for(i=0;i<=NN;i++) *(RES+i) =0;


    ftime(&tempo);
    ta = tempo.time+tempo.millitm*0.001;
//------------------------------------------------------------------------

    monta_mc();

//    printf("\n\nMatrizes do sistema");
//
//    for(i=0;i<ind_col;i++)
//        printf("\nCOL[%d] = %5d       MC[%d] = %15.7lg", i,COL[i],i,MC[i]);
//
//    for(i=1;i<=NN;i++)
//        printf("\nRES[%d] = %15.7lg", i,RES[i]);


//------------------------------------------------------------------------
    ftime(&tempo);
    tb = tempo.time+tempo.millitm*0.001;
//            tempo_mc[itn] = (double)(tb-ta);
    tempo_mc[1] = (double)(tb-ta);

    ftime(&tempo);
    ta = tempo.time+tempo.millitm*0.001;

//------------------------------------------------------------------------
    cond_cont();
//------------------------------------------------------------------------

//    printf("\n\nMatrizes do sistema apos imposicao de condicao de contorno");

//    for(i=0;i<ind_col;i++)
//        printf("\nCOL[%d] = %5d       MC[%d] = %15.7lg", i,COL[i],i,MC[i]);

//    for(i=1;i<=NN;i++)
//        printf("\nRES[%d] = %15.7lg", i,RES[i]);

    ftime(&tempo);
    tb = tempo.time+tempo.millitm*0.001;
//            tempo_cond[itn] = (double)(tb-ta);
    tempo_cond[1] = (double)(tb-ta);

//------------------------------------------------------------------------
// troca sinal de COL, para elementos iguais a zero em MC

    for(i=0;i<ind_col;i++)
      if(!MC[i])
         if(COL[i]>0)
          COL[i] = -COL[i];

    ftime(&tempo);
    ta = tempo.time+tempo.millitm*0.001;

//------------------------------------------------------------------------
    iccg();

//------------------------------------------------------------------------

    ftime(&tempo);
    tb = tempo.time+tempo.millitm*0.001;
//          tempo_iccg[itn] = (double)(tb-ta);
    tempo_iccg[1] = (double)(tb-ta);


    // calcula convergencia do Newton-Raphson
    DA2 = 0;
    A2 = 0;


    for(i=1;i<=NN;i++)
    {
      A[i] = A[i] + DA[i];
      A2 = A2 + (A[i] * A[i]);
      DA2 = DA2 + (DA[i] * DA[i]);
    }

    if(NCA) /* se existe nos com cond. cont. periodicas */
    {
        for(i=1;i<=NCA;i++)
        {
         if(TIPO[i] == 5) A[NO_CA_POT[i]] = - A[NO_CA_REF[i]];
         else if(TIPO[i] == 4)  A[NO_CA_POT[i]] = A[NO_CA_REF[i]];
        }
    }

    if(NCF)
    {
        for(i=1;i<=NCF;i++)
        A[NO_CF_POT[i]] = A[NO_CF_REF[i]];
    }


    if(A2) conv = sqrt(DA2)/sqrt(A2);


    itn = itn+1;
    if(conv <= precisao )  break;

  } /* fim do while */



}// fim do solve
#endif
//...........................................................................................
//...........................................................................................

//void compute_electric_field_1d(void)
void compute_electric_field_1d_internal(int NE,int *E1,int *E2,double *x,double *A,double *E_field)
{
   int    l;
   int    E[4];
   double size;
  //Calculo do campo eletrico
  for(l=1;l<=NE;l++)
  {
    E[1] = E1[l];
    E[2] = E2[l];

   //calcula comprimento do elemento (gen�rico, apesar de ser em princ�pio uma malha
   //                                    regular)

   size = x[E[2]] - x[E[1]];

   if(size) E_field[l] = ( A[E[1]] - A[E[2]] ) / size;
   else printf("\n elemento com comprimento nulo\n");
  }

}// end of compute_electric_field_1d_internal

double compute_electric_field_2d_internal(int NE,double prop_fis,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *E_field)
{
   int        l;
   int        E[4];
   double    b[4], c[4];
   double    delta;
   double    Ex, Ey, E_field_sq;
   double   energia_total = 0.0;
  //Calculo do campo eletrico

   for(l=1;l<=NE;l++)
  {
      E[1] = E1[l];
      E[2] = E2[l];
      E[3] = E3[l];

      b[1] = y[ E2[l] ] - y[ E3[l] ];
      b[2] = y[ E3[l] ] - y[ E1[l] ];
      b[3] = y[ E1[l] ] - y[ E2[l] ];

      c[1] = x[ E3[l] ] - x[ E2[l] ];
      c[2] = x[ E1[l] ] - x[ E3[l] ];
      c[3] = x[ E2[l] ] - x[ E1[l] ];

      delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );


      Ex = - (b[1]*A[E[1]] + b[2]*A[E[2]] + b[3]*A[E[3]])/(2*delta);

      Ey = - (c[1]*A[E[1]] + c[2]*A[E[2]] + c[3]*A[E[3]])/(2*delta);

      E_field_sq = Ex*Ex + Ey*Ey;

      energia_total += (prop_fis*E_field_sq*delta)/2;

      E_field[l] = sqrt(E_field_sq);

  }
   return energia_total;

}//compute_electric_field_2d_internal

#if defined ____PIC____
void compute_electric_field_2d_internal_in_node(int NE,int *E1,int *E2,int *E3,double *x,double *y,double *A,double *Ex_node,double *Ey_node)
{
   int        l,i;
   int        E[4];
   double    b[4], c[4];
   double    delta;
   double    Ex, Ey, contribuicao_x,contribuicao_y;
  //Calculo do campo eletrico

   for(l=1;l<=NE;l++)
  {
      E[1] = E1[l];
      E[2] = E2[l];
      E[3] = E3[l];

      b[1] = y[ E2[l] ] - y[ E3[l] ];
      b[2] = y[ E3[l] ] - y[ E1[l] ];
      b[3] = y[ E1[l] ] - y[ E2[l] ];

      c[1] = x[ E3[l] ] - x[ E2[l] ];
      c[2] = x[ E1[l] ] - x[ E3[l] ];
      c[3] = x[ E2[l] ] - x[ E1[l] ];

      delta = 0.5 * ( b[1]*c[2] - b[2]*c[1] );


      Ex = - (b[1]*A[E[1]] + b[2]*A[E[2]] + b[3]*A[E[3]])/(2*delta);

      Ey = - (c[1]*A[E[1]] + c[2]*A[E[2]] + c[3]*A[E[3]])/(2*delta);


      contribuicao_x = Ex * delta/3;
      contribuicao_y = Ey * delta/3;

      for(i=1;i<=3;i++)
      {
        Ex_node[E[i]] += contribuicao_x;
        Ey_node[E[i]] += contribuicao_y;
      }



//      E_field_sq = Ex*Ex + Ey*Ey;

//      E_field[l] = sqrt(E_field_sq);

  }

}//compute_electric_field_2d_internal

#endif // ____PIC____

//...........................................................................................
//...........................................................................................
//void cond_cont(void)
void cond_cont(int NN,int NCA,int NCF,int NC,int *NO_CA_POT,int *NO_CF_POT,int *NO_CC,double* POT_CC,
               double *MC,int *COL,int *DIAGONAL,double *RES,int ind_col,int *linha1_aux)
{

 int linha1;
 int NCC;
 int i,j,k;

 //if(!mat_lin) for(i=0;i<=NN;i++) RES[i]=0.0;
//-----------------  introducao das condicoes de contorno -----------------

 linha1 = 1; // linha1 aponta para a primeira linha sem
             //                 condicao de contorno   *NMA*

// DIRICHLET ------------------------------------------------------------


 for(i=1;i<=NC;i++)
 {
  NCC = NO_CC[i];
  if(NCC == linha1) linha1 = linha1+1;

  //  troca sinal da diagonal
  if(COL[ DIAGONAL[NCC] ] >0) COL[DIAGONAL[NCC]] = - COL[ DIAGONAL[NCC] ];

  for(j=1;j<=NN;j++)
  {
   if(j==NCC) RES[j] = 0; //POT_CC[i];
   else
   {
    k = busca(j,NCC,DIAGONAL,COL,ind_col);

    if(k != -1)
    {
     RES[j] = RES[j] - MC[k]*POT_CC[i];

     // troca sinal da coluna e parte da linha correspondente
     if(COL[k] > 0) COL[k] = - COL[k];

    }// fim do if k!=1
   } // fim do else
  }  // fim do for j
 }   // fim do for i


 // PERIODICA -------------------------------------------------------------

 if(NCA)
 {
  for(k=1;k<=NCA;k++)
  {
   if( COL[ DIAGONAL[ NO_CA_POT[k] ] ] > 0)
            COL[ DIAGONAL[ NO_CA_POT[k] ] ]  = - COL[ DIAGONAL[ NO_CA_POT[k] ] ];
   if(NO_CA_POT[k] == linha1) linha1 = linha1+1;
  }
 }


 if(NCF)
 {
  for(k=1;k<=NCF;k++)
  {
   if( COL[ DIAGONAL[ NO_CF_POT[k] ] ] > 0)
              COL[ DIAGONAL[ NO_CF_POT[k] ] ]  = - COL[ DIAGONAL[ NO_CF_POT[k] ] ];
   if(NO_CF_POT[k] == linha1) linha1 = linha1+1;
  }
 }

 *linha1_aux = linha1;

} // termina funcao de introducao de condicoes de contorno


//================= funcao de resolucao do sistema pelo iccg ===============
void iccg(int NN,double *MC,int *COL,int *DIAGONAL,double *RES,double* DA,int ind_col,int linha1, int mat_lin, int itn, int itc_max)
{
    int     i,j;
    int     itc;
    double  d0,d1 = 0.0,tau,prod,beta,eps = 1e-5;

    // vetores usados somente pelo iccg
    double *U        = NULL;
    double *yi    = NULL;
    double *h        = NULL;
    double *d        = NULL;
    double *gt    = NULL;

    U = (double *)malloc( ind_col* sizeof(double) );
    yi = (double *)malloc( (NN+1)*sizeof(double) );
    h = (double *)malloc( (NN+1)*sizeof(double) );
    d = (double *)malloc( (NN+1)*sizeof(double) );
    gt = (double *)malloc( (NN+1)*sizeof(double) );

    if( !U || !yi || !h || !d || !gt) {
        printf("\nerro na alocacao de memoria de vetor (U, yi, h, d ou gt)");
        exit(0);
    }

    memset((void*)U,0,ind_col*sizeof(double));
    memset((void*)yi,0,(NN+1)*sizeof(double));
    memset((void*)h,0, (NN+1)*sizeof(double));
    memset((void*)d,0, (NN+1)*sizeof(double));
    memset((void*)gt,0,(NN+1)*sizeof(double));

    if(mat_lin)
        for(i=1;i<=NN;i++)
            gt[i] = RES[i];
    else
        for(i=1;i<=NN;i++)
            gt[i] = -RES[i];

    monta_matriz_U(NN,U,MC,DIAGONAL,COL,ind_col,linha1);

    calcula_y(NN,U,DIAGONAL,COL,yi,gt);

    calcula_h(NN,U,DIAGONAL,COL,yi,h);

    for(i=1;i<=NN;i++)
        d[i] = -h[i];

    // calcula d0 = g.h
    d0=0;
    for(i=1;i<=NN;i++)
        d0 = d0 + gt[i]*h[i];

    itc = 0;

    if(itn == 1)
        if(d0<=eps)
            eps = d0/1e5;

        if(d0>eps) {
            do {
                printf ("\n %d, %g ",itc,d1);
                itc++;
                memset((void*)h,0, (NN+1)*sizeof(double));

                // calcula h = MC * d
#if 10
    int pos_inicial=0, pos_final=0;

    for(i=1;i<NN;i++)
    {
     if(COL[DIAGONAL[i]] > 0) //( so para nos sem condicao de contorno
     {
        pos_inicial = DIAGONAL[i];
        pos_final   = DIAGONAL[i+1];

        h[i] = h[i] + MC[pos_inicial] * d[i];

        for(j=pos_inicial+1;j<pos_final;j++) {
         if(COL[j] > 0) {
            h[i]      = h[i] + MC[j] * d[COL[j]];
            h[COL[j]] = h[COL[j]] + MC[j] * d[i];
         }// fecha if(COL[j] > 0)
        }// fecha for(j=pos_inicial;j<pos_final;j++)
     }//fecha if(COL[DIAGONAL[i]] > 0)
    }// fecha for(i=1;i<=NN;i++)

// para ultimo no
     i=NN;
     if(COL[DIAGONAL[i]] > 0) //( so para nos sem condicao de contorno
     {
        pos_inicial = DIAGONAL[i];
        pos_final   = ind_col;

        h[i] = h[i] + MC[pos_inicial] * d[i];

        for(j=pos_inicial+1;j<pos_final;j++)
        {
         if(COL[j] > 0)
         {
            h[i]      = h[i] + MC[j] * d[COL[j]];
            h[COL[j]] = h[COL[j]] + MC[j] * d[i];
         }// fecha if(COL[j] > 0)
        }// fecha for(j=pos_inicial;j<pos_final;j++)
     }//fecha if(COL[DIAGONAL[i]] > 0)


#endif

//_____________________________________________________________________________________________________________________



    prod = 0;

    for(i=1;i<=NN;i++) prod = prod + d[i]*h[i];
    tau = d0/prod;

    for(i=1;i<=NN;i++) DA[i] = DA[i] + tau * d[i];
    for(i=1;i<=NN;i++) gt[i] = gt[i] + tau * h[i];   // Este � o ponto de parada

//    calcula_y();
    calcula_y(NN,U,DIAGONAL,COL,yi,gt);


//    calcula_h();
   calcula_h(NN,U,DIAGONAL,COL,yi,h);


    d1 = 0;

    for(i=1;i<=NN;i++) d1 = d1+gt[i] * h[i];


    if(d1 > eps)
    {
     beta = d1/d0;
     d0 = d1;

     for(i=1;i<=NN;i++) d[i] = -h[i] + beta * d[i];

     if(itc>=itc_max) d1 = eps/10.0;
//     if(itc>=1) d1 = eps/10.0;
    }


    }while(d1>eps);

    printf ("\n %d, %g ",itc,d1);

  } // fecha if(d0>eps)

  if(U)        free(U);
  if(yi)    free(yi);
  if(h)        free(h);
  if(d)        free(d);
  if(gt)    free(gt);


}// fecha iccg

//...........................................................................................
//...........................................................................................
//------------------- rotina para montar matriz U -------------------------
//void monta_matriz_U(void)
void monta_matriz_U(int NN,double *U,double *MC,int *DIAGONAL,int *COL,int ind_col,int linha1)
{
 int posicao,i,j,k;


// calcula 1a. linha ::::::::::::::::::::::::::::::::::::::::::::::::::::::

// calcula 1o. elemento u11 = sqrt(h11) ...................................
// linha1 contem a 1a. linha da matriz MC sem condicao de contorno
// if(COL[0]>0)
 U[DIAGONAL[linha1]] = sqrt(MC[DIAGONAL[linha1]]);

// calcula 1a. linha u1j = h1j/u11 ........................................
 for(j=DIAGONAL[linha1]+1;j<DIAGONAL[linha1+1];j++)
 {
  if(COL[j] > 0)
  {
    if(!U[DIAGONAL[linha1]])
    {
        printf("\n\n Diagonal negativa - linha1");
    }

   if(COL[0]>0) U[j] = MC[j]/U[DIAGONAL[linha1]];
  }
 }


// para todas as diagonais a partir da 2a. atribui uii = hii ...............

 for(i=linha1+1;i<=NN;i++) U[ DIAGONAL[i] ] = MC[ DIAGONAL[i] ];


// calcula uii e uik .......................................................

 for(i=linha1;i<NN;i++)
 {
// subtrai uji*uji na diagonal correspondente uii
  for(j=DIAGONAL[i]+1;j<DIAGONAL[i+1];j++)
  {
   if(COL[j]>0) U[DIAGONAL[COL[j]]] = U[DIAGONAL[COL[j]]] - (U[j]*U[j]);
  }

// calcula diagonal de i+1,neste ponto temos uii=hii-somatoria de uji*uji

  if( U[DIAGONAL[i+1]] <0)
  {

    printf("\n\n Diagonal negativa %d, U[%lf] = ", DIAGONAL[i+1], U[DIAGONAL[i+1]]);

     exit(10);
  }

  U[ DIAGONAL[i+1] ] = sqrt(U[ DIAGONAL[i+1] ]);

// subtrai uji*ujk em uik e calcula uik ................................
  if(i<NN-1)
  {
   for(j=DIAGONAL[i]+1;j<DIAGONAL[i+1]-1;j++)
   {
    if(COL[j]>0)
    {
     for(k=j+1;k<DIAGONAL[i+1];k++)
     {
      if(COL[k]>0)
      {
       posicao = busca(COL[j],COL[k],DIAGONAL,COL,ind_col); // busca posicao de uik
       if(posicao != -1)
       U[posicao] = U[posicao]-(U[j]*U[k]);//subtrai uji*ujk de uik
      } // fecha if(COL[k])
     } // fecha for(k..)
    } // fecha if(COL[j])
   } // fecha for(j..)

// calcula uik
   for(k=(DIAGONAL[i+1]+1);k<DIAGONAL[i+2];k++)
   {
// soma hik a uik e divide por uii
    if(COL[k]>0)
    {
     if(U[DIAGONAL[i+1]] > 0) U[k] = (U[k]+MC[k])/U[DIAGONAL[i+1]];
    }
   }
  } // fecha if(i<NN-1)
 } // fecha for(i=linha1)

} // termina rotina de montagem da matriz U


//...........................................................................................
//...........................................................................................
//===================== funcao de calculo do vetor y =====================/
//void calcula_y(void)
void calcula_y(int NN,double *U,int *DIAGONAL,int *COL,double *yi,double *gt)
{
 int i,j;

 // calcula yi1
 if(COL[0]>0) yi[1] = gt[1]/U[0];

 // atribui gt[i] a yi[i]
 for(i=2;i<=NN;i++) yi[i] = gt[i];

 // subtrai aji*yi1 em yij e calcula yii+1
 for(i=1;i<NN;i++)
 {
  for(j=DIAGONAL[i]+1;j<DIAGONAL[i+1];j++)
  {
   if(COL[j]>0) yi[COL[j]] = yi[COL[j]] - (U[j]*yi[i]);
  }

  // calcula yii+1

  if(COL[DIAGONAL[i+1]]>0)
  {
     if(!U[ DIAGONAL[i+1] ])
     {
         printf("\n\n U[ DIAGONAL[%d] ] e' nulo!!!!\n", i+1);
         exit(0);
     }

      yi[i+1] = yi[i+1]/U[ DIAGONAL[i+1] ];
  }
 }

} // termina a rotina calcula_y


//...........................................................................................
//...........................................................................................
//================== funcao para calculo do vetor h ======================
//void calcula_h()
void calcula_h(int NN,double *U,int *DIAGONAL,int *COL,double *yi,double *h)
{
 int i,j;

 // calcula hnn
 if(COL[DIAGONAL[NN]] >0) h[NN] = yi[NN]/U[DIAGONAL[NN]];

 // calcula hi
 for(i=NN-1;i>=1;i--)
 {
  h[i] = yi[i];
  for(j=DIAGONAL[i]+1;j<DIAGONAL[i+1];j++)
  {
   if(COL[j]>0) h[i] = h[i] - (U[j]*h[COL[j]]);
  }
  if(COL[DIAGONAL[i]] >0) h[i] = h[i]/U[DIAGONAL[i]];
 }

} // termina a rotina


//**************************************************************************
//
// funcao de busca da posicao de Sij na matriz compactada
//
//**************************************************************************

//int busca(int linha,int coluna)
int busca(int linha,int coluna, int *DIAGONAL, int *COL, int ind_col)
{
 int t,k,aux;

 if(linha > coluna)
 {
  aux = linha;
  linha = coluna;
  coluna = aux;
 }

 k = -1;

 if( DIAGONAL[linha] == (ind_col-1) ) // verifica se e' ultimo no'
 {
  if( COL[ DIAGONAL[linha] ] == coluna ) k = DIAGONAL[linha];
 }
 else
 {
  // procura a coluna corresponde a partir da diagonal
  for( t=DIAGONAL[linha];t<DIAGONAL[linha+1];t++ )
  if( COL[t] == coluna )
  {
   k = t;
   break;
  }
 }


 return k;
}

//...........................................................................................
//...........................................................................................
/*
void free_memory_builder_1d(void)
{
    if(x)            free(x);
//    if(y)            free(y);
    if(E1)            free(E1);
    if(E2)            free(E2);
//    if(E3)            free(E3);
//    if(MEIO)        free(MEIO);
    if(FONTE_EL)    free(FONTE_EL);
    if(FONTE_VAL)    free(FONTE_VAL);
    if(NO_CC)        free(NO_CC);
    if(POT_CC)        free(POT_CC);
    if(NO_CA_REF)    free(NO_CA_REF);
    if(NO_CA_POT)    free(NO_CA_POT);
    if(TIPO)        free(TIPO);
    if(NO_CF_REF)    free(NO_CF_REF);
    if(NO_CF_POT)    free(NO_CF_POT);


//    if (DIAGONAL)    free(DIAGONAL);
//    if (COL)        free(COL);
//    if (MC)            free(MC);
//    if (A)            free(A);
//    if (DA)            free(DA);
//    if (RES)        free(RES);

//    if (E_field)    free(E_field);
//    if (source)        free(source);

//    if (U)            free(U);
//    if (yi)            free(yi);
//    if (h)            free(h);
//    if (d)            free(d);
//    if (gt)            free(gt);

//    reset();

}//fim de free_memory


void reset(void)
{
 x            = NULL;
// y            = NULL;
 E1          = NULL;
 E2          = NULL;
// E3          = NULL;
// MEIO        = NULL;
 FONTE_EL    = NULL;
 FONTE_VAL   = NULL;
 NO_CC       = NULL;
 POT_CC      = NULL;
// angulo      = NULL;
// codigos     = NULL;

 NO_CA_REF   = NULL;
 NO_CA_POT   = NULL;
 TIPO        = NULL;
 NO_CF_REF   = NULL;
 NO_CF_POT   = NULL;

 DIAGONAL    = NULL;
 COL          = NULL;
 MC          = NULL;
 A              = NULL;
 DA          = NULL;
 RES          = NULL;
 E_field      = NULL;
 source      = NULL;
//---------------------------------------------------
// vetores usados somente pelo iccg
 U              = NULL;
 yi          = NULL;
 h              = NULL;
 d              = NULL;
 gt          = NULL;
//----------------------------------------------------

}// fim de reset
*/

//===================== meio linear ========================================
void linear(double prop_fis,double *b,double *c,double delta,int *cond,int mat_lin,int *E,
            int *no_global,double *Ifonte,double *MC,double *A,double *RES,int *DIAGONAL, int *COL,int ind_col)
{
 double s12,s13,s23;
 double aux1=0.0;
 double Sijar;
 double res_elem[4];

 int   cond1,i,j,k;

 s12 = 0;   s13 = 0;  s23 = 0;
 for(i=0;i<4;i++) res_elem[i] = 0;

 for(i=1;i<=3;i++)
 {
  for(j=1;j<=3;j++)
  {
   if(j>=i)
   {

    Sijar = prop_fis * (b[i]*b[j] + c[i]*c[j])/(4*delta);

    if(cond[i]==5 || cond[j] ==5)
    {
     if(cond[i] == cond[j]) cond1=0;
     else cond1 = 5;
    }
    else cond1=0;

    if(mat_lin)
    {
     if(i==1)
     {
      if(j==2) s12 = Sijar * A[E[1]];
      else if(j==3) s13 = Sijar * A[E[1]];
     }
     else if(i==2 && j==3) s23 = Sijar * A[E[2]];

     res_elem[i] = res_elem[i] + Sijar * A[E[j]];
    }

    if(i!=j) if(cond1==5) Sijar = -Sijar;

//    if(!pDoc->solverMethod) //iccg
//    {
        k = busca(no_global[i],no_global[j],DIAGONAL,COL,ind_col);

    if(k!=-1) MC[k] = MC[k] + Sijar;
//    }
//    else
//    {
//        // para SuperLU, zero based
//        k = busca_CCS(no_global[i]-1,no_global[j]-1);
//
//        MC[k] = MC[k] + Sijar;
//
//        if(no_global[j] != no_global[i])
//        {
//            k = busca_CCS(no_global[j]-1,no_global[i]-1);
//
//            MC[k] = MC[k] + Sijar;
//        }
//    }

   } //  fim de if(j>=i)
  } // fim do for(j..)

  if(mat_lin)
  {
   switch(i)
   {
    case 1: aux1 = 0 + res_elem[i] ;         //- Ifonte[i];
            break;

    case 2: aux1 = s12 + res_elem[i];       // - Ifonte[i];
            break;

    case 3: aux1 = s13 + s23 + res_elem[i]; // - Ifonte[i];
            break;
    }

    if(cond[i] == 5) aux1 = -aux1;

    RES[no_global[i]] = RES[no_global[i]] + aux1;
  }


//    if(!pDoc->solverMethod) //iccg
//    {
        RES[no_global[i]] = RES[no_global[i]] - Ifonte[i];
//    }
//    else
//    {
//        RES[no_global[i]-1] = RES[no_global[i]-1] - Ifonte[i];
//    }



 }// fim do for(i..)

} // fim da rotina - meio linear


//======================== meio nao linear ==================================
void nao_linear(double Bx,double By,double k1f,double k2f,double k3f,double *b,double *c,double delta,
                int *cond,int mat_lin,int itn,double *A,int *E,double *MC,int *no_global,double *RES,int *DIAGONAL, int *COL,int ind_col)
{
 double        b2;
 int        fator_at;
 double        Sij,ni,delni,Slij;
 double        p1,p2,p3,var1,q1,q2,q3,var2;
 double        s12,s13,s23,aux_nlin=0.0;
 int        i,j,k;
 int        cond1;
 double        res_elem[5];
 double        b2max = 6.25;

 s12 =0;
 s13 =0;
 s23 =0;
 for(i=0;i<=4;i++) res_elem[i] = 0;

 b2 = (Bx*Bx) + (By*By);
 if(b2>b2max) b2 = b2max;


 /* curva do ferro por etapas (ver tese SHEN pag. 106) */

 switch(itn) /* itn - iteracao de Newton_Raphson */
 {
  case 1 : fator_at = 12000; /* 1a. iteracao */
           break;

  case 2 : fator_at =  7000; /* 2a. iteracao */
           break;

  case 3 : fator_at =  2000; /* 3a. iteracao */
           break;

  default: fator_at = 0; /* 4a. iteracao em diante */

 } /* fim do switch */

/*   printf("fator_at = %d\n",fator_at); getch(); */
  fator_at = 0;


 ni = (k1f*exp(k2f*b2) + k3f + fator_at);
 delni = (k1f * k2f * exp(k2f*b2)) ;


 for(i=1;i<=3;i++)
 {
  for(j=1;j<=3;j++)
  {
   if(j>=i)
   {
    Sij = ni * (b[i]*b[j] +c[i]*c[j])/(4*delta);


    if(cond[i]==5 || cond[j] ==5)
    {
     if(cond[i] == cond[j]) cond1=0;
     else cond1 = 5;
    }
    else cond1 = 0;

    if(mat_lin) // so faz se tiver material nao linear
    {
     if(i==1)
     {
      if(j==2) s12 = Sij * A[E[1]];
      else if(j==3) s13 = Sij * A[E[1]];
     }
     else if(i==2 && j==3) s23 = Sij * A[E[2]];

     res_elem[i] = res_elem[i] + Sij * A[E[j]];
    }

    if(i!=j) if(cond1==5) Sij = -Sij;

    p1 = A[E[1]] * (b[i]*b[1] + c[i]*c[1])/(4*delta);
    p2 = A[E[2]] * (b[i]*b[2] + c[i]*c[2])/(4*delta);
    p3 = A[E[3]] * (b[i]*b[3] + c[i]*c[3])/(4*delta);

    var1 = p1 + p2 + p3;

    q1 = A[E[1]] * (b[j]*b[1] + c[j]*c[1])/(4*delta);
    q2 = A[E[2]] * (b[j]*b[2] + c[j]*c[2])/(4*delta);
    q3 = A[E[3]] * (b[j]*b[3] + c[j]*c[3])/(4*delta);

    var2 = q1 + q2 + q3;

    Slij = (delni * var1 * var2)*(2/delta);

    if(i!=j) if(cond1==5) Slij = -Slij;

//    k = busca(no_global[i], no_global[j]);
//    MC[k] = MC[k] + Sij + Slij;

//    if(!pDoc->solverMethod) //iccg
//    {
        k = busca(no_global[i],no_global[j],DIAGONAL,COL,ind_col);

        MC[k] = MC[k] + Sij + Slij;
//    }
//    else
//    {
//        // para SuperLU, zero based
//        k = busca_CCS(no_global[i]-1,no_global[j]-1);
//
//        MC[k] = MC[k] + Sij + Slij;
//
//        if(no_global[j] != no_global[i])
//        {
//            k = busca_CCS(no_global[j]-1,no_global[i]-1);
//
//            MC[k] = MC[k] + Sij + Slij;
//        }
//    }


   } /* fim de if(j>=i)  */

  } /* fim de for(j..) */

  if(mat_lin) // so faz se tiver material nao linear
  {
   switch(i)
   {
    case 1: aux_nlin = 0 + res_elem[i]; /*- Ifonte[i];*/
            break;

    case 2: aux_nlin = s12 + res_elem[i];/* Ifonte[i];*/
            break;

    case 3: aux_nlin = s13 + s23 +res_elem[i]; /*- Ifonte[i];*/
            break;
   }

   if(cond[i] == 5) aux_nlin = -aux_nlin;

//   RES[no_global[i]] = RES[no_global[i]] + aux_nlin; /* - Ifonte[i];*/

//    if(!pDoc->solverMethod) //iccg
//    {
        RES[no_global[i]] = RES[no_global[i]] + aux_nlin;
//    }
//    else
//    {
//        RES[no_global[i]-1] = RES[no_global[i]-1] + aux_nlin;
//    }

  }

 } // fecha for(i..)


} // termina rotina


//========================= meio anisotropico ==============================
void anisotropico(int l,double Bx,double By,double *angulo,double Hc,double ni2,double k1i,double k2i,double k3i,
                  double itn,double delta, double *b,double *c,int *cond,int mat_lin,double *A,
                  int *no_global,int *E,double *MC,double *RES,int *DIAGONAL, int *COL,int ind_col)
{
 double Hcx,Hcy;
 double R[4];
 double Bxa,Bxa2,ni1,delni1,nixx,niyy,nixy,delnixx,delniyy,delnixy;
 double ni_aux, Sijima, t1, t2, t3, T, u, u1, u2, u3, U, ani_aux;
 double s12=0.0,s13=0.0,s23=0.0;
 int   i,j,k;
 int    cond1;
 double cos_a,sin_a,cos2, sin2;


/* cos_a = cos_alfa[ima];
    sin_a = sin_alfa[ima];
    cos2  = cos2_alfa[ima];
    sin2  = sin2_alfa[ima];
*/

 cos_a = cos(angulo[l]);
 sin_a = sin(angulo[l]);
 cos2  = cos_a * cos_a;
 sin2  = sin_a * sin_a;


 Hcx = (Hc * cos_a);
 Hcy = (Hc * sin_a);

 R[1] = (-Hcx * c[1] + Hcy * b[1])/2;
 R[2] = (-Hcx * c[2] + Hcy * b[2])/2;
 R[3] = (-Hcx * c[3] + Hcy * b[3])/2;

 Bxa = Bx * cos_a + By * sin_a;
 Bxa2 = Bxa * Bxa;

// ....................................................................................................................
// valor inicial de ni para imas nao lineares
//....................................................................................................................


 ni1 = (k1i * exp(k2i*Bxa2) + k3i) ;
 delni1 = (k1i * k2i * exp(k2i * Bxa2));

 if(k1i && k2i && k3i)
 {
  if(itn == 1)
  {
    Bxa2 = 1;
    ni1 = (k1i * exp(k2i*Bxa2) + k3i) ;
    delni1 = 0;
  }
 }


/* if(MEIO[l]==230) */
/*  if(itn == 1)
  {
    ni1 = 455476.32;
    delni1 = 0;
  }
*/

 nixx = ni1 * cos2 + ni2 * sin2;
 niyy = ni1 * sin2 + ni2 * cos2 ;
 nixy = (ni1 - ni2) * sin_a * cos_a;

 delnixx = delni1 * cos2;
 delniyy = delni1 * sin2;
 delnixy = delni1 * sin_a * cos_a;

 ni_aux = 0;


 for(i=1;i<=3;i++)
 {
  for(j=1;j<=3;j++)
  {
    if(j>=i)
    {
    ni_aux = 1/(4*delta);
    Sijima = ni_aux * (nixx*c[i]*c[j] + niyy*b[i]*b[j]
                        -nixy*(b[i]*c[j] + b[j]*c[i]));

    if(cond[i]==5 || cond[j] ==5)
    {
     if(cond[i] == cond[j]) cond1=0;
     else cond1 = 5;
    }
    else cond1 =0;

    if(i!=j) if(cond1==5) Sijima = -Sijima;

    if(mat_lin)
    {
     if(i==1)
     {
      if(j==2) s12 = Sijima * A[no_global[1]];
      else if(j==3) s13 = Sijima * A[no_global[1]];
     }
     else if(i==2 && j==3) s23 = Sijima * A[no_global[2]];
    }


    t1 = ( delnixx*c[i]*c[1] + delniyy*b[i]*b[1]-
            delnixy*(b[i]*c[1] + b[1]*c[i]) ) * A[E[1]];

    t2 = ( delnixx*c[i]*c[2] + delniyy*b[i]*b[2]-
            delnixy*(b[i]*c[2] + b[2]*c[i]) ) * A[E[2]];

    t3 = ( delnixx*c[i]*c[3] + delniyy*b[i]*b[3]-
            delnixy*(b[i]*c[3] + b[3]*c[i]) ) * A[E[3]];

    T = t1 + t2 +t3;

    u = (c[j]*b[1] + b[j]*c[1]) * cos_a * sin_a;
    u1 = (c[j]*c[1]*cos2 + b[j]*b[1]*sin2-u) * A[E[1]];

    u = (c[j]*b[2] + b[j]*c[2]) * cos_a * sin_a;
    u2 = (c[j]*c[2]*cos2 + b[j]*b[2]*sin2-u) * A[E[2]];

    u = (c[j]*b[3] + b[j]*c[3]) * cos_a * sin_a;
    u3 = (c[j]*c[3]*cos2 + b[j]*b[3]*sin2-u) * A[E[3]];

    U = u1 + u2 + u3;

    ani_aux = (T*U)/(8*delta*delta*delta);

    k = busca(no_global[i],no_global[j],DIAGONAL,COL,ind_col);

    if(i!=j) if(cond1==5) ani_aux = -ani_aux;

    MC[k] = MC[k] + ani_aux + Sijima;

    if(mat_lin)
    {
     RES[no_global[i]] = RES[no_global[i]] + Sijima * A[no_global[j]];
    }

    }//fim de if(j>=i)

  } // fim do for(j..)

  switch(i)
  {
    case 1: ani_aux = -R[1];
            break;

    case 2: ani_aux = s12 - R[2];
            break;

    case 3: ani_aux = s13 + s23 - R[3];
            break;
  }


  RES[no_global[i]] = RES[no_global[i]] + ani_aux;



 } // fim do for(i...)


} // fim da rotina




void solve_non_linear_system_internal(int NN,int NE,int NCA,int NCF,int NC,int NF,
                                double *x,double *y, int *E1,int *E2,int *E3,int *MEIO,double *angulo,
                                int *NO_CC,double *POT_CC,
                                int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,
                                int *FONTE_EL,double *FONTE_VAL, int mat_lin,int itn_max, int itc_max, int tipo_estudo,
                                materiais_struct *mat,double *MC, int *COL, int *DIAGONAL, double *RES,
                                int nos_alocados, int ind_col,double precisao, double *A)
{

    int itn, itnc;
    int linha1;
    int i;
    double *DA    = NULL;
    double    DA2, A2;
    double conv = 0.0;

    DA  = (double *)calloc( (NN+1), sizeof(double) );
    if(!DA)
    {
		printf("Erro na alocação do vetor DA - solve_non_linear_system_internal");
        exit(0);
    }


    if(!mat_lin)
    {
        itn_max = 1;// se n�o existirem materiais nao lineares
    }

    // atualiza valor das condicoes de contorno Dirichlet no vetor de potenciais
    for(i=1;i<=NC;i++)
    {
        A[NO_CC[i]] = POT_CC[i];
    }

    itn =1;
    itnc=1;
//=========================== NEWTON-RAPHSON ============================
    while( itn <= itn_max )
     {

        for(i=0;i<=ind_col;i++) if(COL[i]<0) COL[i] = -COL[i];
        for(i=0;i<=ind_col;i++) *(MC + i) = 0;
        for(i=0;i<=NN;i++) *(RES+i) =0;


//        ftime(&tempo);
//        ta = tempo.time+tempo.millitm*0.001;
//------------------------------------------------------------------------

        monta_mc(NN,NE,NF,NCA,NCF,NC,x,y,E1,E2,E3,MEIO,FONTE_EL,FONTE_VAL,
                  NO_CC,POT_CC,NO_CA_REF,NO_CA_POT,TIPO,
                  NO_CF_REF,NO_CF_POT,angulo,tipo_estudo,MC,COL,DIAGONAL,RES,A,ind_col,mat_lin,itn,mat);

//------------------------------------------------------------------------
//            ftime(&tempo);
//            tb = tempo.time+tempo.millitm*0.001;
//            tempo_mc[itn] = (double)(tb-ta);
//            tempo_mc[1] = (double)(tb-ta);

//            ftime(&tempo);
//            ta = tempo.time+tempo.millitm*0.001;

//------------------------------------------------------------------------
        cond_cont(NN,NCA,NCF,NC,NO_CA_POT,NO_CF_POT,NO_CC,POT_CC,MC,COL,DIAGONAL,RES,ind_col,&linha1);
//------------------------------------------------------------------------

//            ftime(&tempo);
//            tb = tempo.time+tempo.millitm*0.001;
//            tempo_cond[itn] = (double)(tb-ta);
//            tempo_cond[1] = (double)(tb-ta);

//------------------------------------------------------------------------
// troca sinal de COL, para elementos iguais a zero em MC

          for(i=0;i<ind_col;i++)
            if(!MC[i])
             if(COL[i]>0)
              COL[i] = -COL[i];
//------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
/*
  FILE *fp = NULL;

  fp = fopen("g:\\optica_integrada\\anisvc\\testes\\Iccgdat.dat","w");
    if( fp == NULL)
    {
     printf("erro na abertura do arquivo Iccgdat.dat");
    }

  if(fp)
  {
      int k, j;
      for(i=1;i<=NN;i++)
      {
          for(j=1;j<=NN;j++)
          {
              k = busca(i,j);
              if(k != -1)
                  fprintf(fp,"MC[%d,%d] = %g\n",i,j,MC[k]);
          }
      }

  }
  fclose(fp);
*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//          ftime(&tempo);
//          ta = tempo.time+tempo.millitm*0.001;

//------------------------------------------------------------------------
    iccg(NN,MC,COL,DIAGONAL,RES,DA,ind_col,linha1, mat_lin, itn, itc_max);

//------------------------------------------------------------------------

//          ftime(&tempo);
//          tb = tempo.time+tempo.millitm*0.001;
//          tempo_iccg[itn] = (double)(tb-ta);
//          tempo_iccg[1] = (double)(tb-ta);


  // calcula convergencia do Newton-Raphson
    DA2 = 0;
    A2 = 0;

    for(i=1;i<=NN;i++)
     {
            A[i] = A[i] + DA[i];
            A2 = A2 + (A[i] * A[i]);
            DA2 = DA2 + (DA[i] * DA[i]);
     }

     if(NCA) /* se existe nos com cond. cont. periodicas */
     {
            for(i=1;i<=NCA;i++)
            {
             if(TIPO[i] == 5) A[NO_CA_POT[i]] = - A[NO_CA_REF[i]];
             else if(TIPO[i] == 4)  A[NO_CA_POT[i]] = A[NO_CA_REF[i]];
            }
     }

     if(NCF)
     {
            for(i=1;i<=NCF;i++)
             A[NO_CF_POT[i]] = A[NO_CF_REF[i]];
     }


      if(A2) conv = sqrt(DA2)/sqrt(A2);


      itn = itn+1;
      if(conv <= precisao )  break;

    } /* fim do while */

//=================== FIM DO NEWTON-RAPHSON ===============================

    if(DA) free(DA);

}//end of solve_non_linear_system_internal

#if defined  ____PIC____

//---------------------------------------------------------------------------------------------------------------------
void mesh_generation(int NN,int NF,int NCA,int NCF,int NC,double *x,double *y,
                     int *FONTE_EL,double *FONTE_VAL,
                     int *NO_CC,double *POT_CC,
                     int *NO_CA_REF, int *NO_CA_POT,int *TIPO,int *NO_CF_REF, int *NO_CF_POT,int *MEIO,
                     int *E1,int *E2,int *E3, int *NE)
{
//  declara��o de objetos relacionados com a gera��o de malha.
    Lo_Pt_Malha                o_lista_pontos, o_nuvem_pontos;
    Lista_Elementos_2D_D    bloco_elem_2D;
    Elemento_2D                triangulo;
    Pt_Malha                o_ponto;
//    Lo_Aresta_Geometria bloco_geometria;
    Delaunay2D                o_malha1;
    long int                vertices[10];
    int                        i, j;

    //constroi lista de pontos a partir dos vetores x e y
    o_nuvem_pontos.Set_N_Max_Com_Abs(NN);
    o_lista_pontos.Set_N_Max_Com_Abs(10*NN);
    bloco_elem_2D.Set_N_Max_Com_Abs(100*NN);

    for ( i = 1; i <= NN; i++)
    {
        o_ponto.Set_Coord(x[i],y[i]);
        o_nuvem_pontos.Inclui(o_ponto);
    }

    // gera malha

    o_malha1.Associa(&o_lista_pontos, &o_nuvem_pontos);
    o_malha1.Associa(&bloco_elem_2D);
//    o_malha1.Associa(&bloco_geometria);

    o_malha1.Gera_Elementos_Partida();
    o_malha1.Discretiza();
    o_malha1.Elimina_Englobante();




    // atualiza NE = numero total de elementos

    bloco_elem_2D.Compacta_Lista(1);

    *NE = bloco_elem_2D.Get_Total_Componentes();

    // preenche vetores
    //    E1 : vertice 1 do elemento
    //      E2 : vertice 2 do elemento
    //      E3 : vertice 3 do elemento

    double area=0.0;

    for (i = 1 ; i <= *NE; i++)
    {
        bloco_elem_2D.Get_Componente(i-1, triangulo);
        triangulo.Get_Vertices(vertices);
        for (j = 0; j < 3; j++) vertices[j] -= 3;   // nesta vers�o do gerador de malha a lista o_lista_elementos
                                                      // apresenta �ndices v�lidos come�ando de 4;

        area = triangulo.Get_Volume();

        if(area < 0)
        {
            E1[i] = vertices[0]; E2[i] = vertices[1]; E3[i] = vertices[2];
        }
        else
        {
            E1[i] = vertices[1]; E2[i] = vertices[0]; E3[i] = vertices[2];
        }
    }



}//end of mesh_generation


#endif //____PIC____











