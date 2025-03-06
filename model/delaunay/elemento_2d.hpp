#ifndef __Elemento_2D__
#define __Elemento_2D__


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "lo_pt.hpp"
#include "aresta_geom.hpp"
#include "material.hpp"
#include "levtypedefmalha.h"


class Elemento_2D
{
  friend class Lista_Elementos_2D_D;
  protected :
    double Erro;
    double Campo;
    double DerivadaPrim;
    double Angulo;
    double Fonte;
    long  material;
    int       N_vertices;
    double    volume;
    long int  Vertices[3];   // indice dos pontos v�rtices, segundo a lista_pontos
    double    Coef[3];       // lista de coeficientes do circuncirculo
    lev_math::Uchar     Num_Vizinhos;  // n�mero atual de vizinhos do elemento (m�x deve ser 3)
    long int  Vizinhos[3];   // lista de elementos pr�ximos
    lev_math::Uchar     Estado;        // assume os valores VALIDO ou INVALIDO declarados atraves de enum
    long int  aresta;
    double    Raio_Circ;

  public:
    void Ordena_Vizinhos(vElemento2D &,  long *);
    void Calc_Erro(vCLevPtMalha &);
     void Get_Baricentro(vCLevPtMalha & ,Pt_Malha & o_ponto);
    double Get_Maior_Comp(vCLevPtMalha &,Aresta_Geometria &);
    double Get_Maior_Comp(vCLevPtMalha &);
    double Get_Erro();
    void Set_Erro(double e);
    double Get_Campo();
    void Set_Campo(double c);
    double Get_DerivadaPrim();
    void Set_DerivadaPrim(double);
    void Set_Angulo (double &a);
    double Get_Angulo();
    double Get_Fonte();
    void Set_Fonte(double f);
    void Set_Material (long & mat);
    long Get_Material (void);
    double Get_Raio_Circ (void);

    Elemento_2D();
    ~Elemento_2D();

    Elemento_2D            operator= (const Elemento_2D & el);
    virtual char        operator== (Elemento_2D & );

    virtual long int    Get_V1(void);
    virtual long int    Get_V2(void);
    virtual long int    Get_V3(void);
    double                Get_Volume(void);
    void                Set_Vertices(long int *);
    void                Monta_Faces(FACE * faces);
    void                Set_Volume(double);
    long int *            Get_Vizinhos(void);
    lev_math::USINT                Get_N_Vizinhos(void);
    lev_math::Uchar                Set_N_Vizinhos(lev_math::Uchar n);
    lev_math::Uchar                Get_Validade(void);
    void                Set_Validade(lev_math::Uchar); //atribui validade ao elemento
    void                Calc_Circuns(vCLevPtMalha &);
    double                Calc_R_Insc(vCLevPtMalha &);
    void                CartesianToHom(vCLevPtMalha &lista, Pt_Malha &o_ponto, Pt_Malha &o_phom ) ;
    void                HomToCartesian(vCLevPtMalha &lista, Pt_Malha &o_phom , Pt_Malha &o_ponto) ;
    void                Transformation_Coefficients(vCLevPtMalha &, double *, double *, double *);
    bool                Is_Point_Inside(vCLevPtMalha &lista, Pt_Malha &point);
    lev_math::USINT                Elimina_Vizinhanca(long int);
    double                Get_C0(void);
    double                Get_C1(void);
    double                Get_C2(void);
    void                Set_C0(double);
    void                Set_C1(double);
    void                Set_C2(double);
    lev_math::USINT                Set_Vizinho(long int ind);
    long int            Get_Vertices(int indice);
    long int *          Get_Vertices (long int *vert);
    int                 Get_N_Vertices();
};


#endif
