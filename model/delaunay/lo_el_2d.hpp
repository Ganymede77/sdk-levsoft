#ifndef _LO_EL_2D_
#define _LO_EL_2D_


// ****************************************************************************
//    CLASSE Lista_Elementos_2D
//
//    Arquivo   : LO_EL_3D.HPP
//
//    Descricao : Classe de elementos 3D ordenados. Esta classe eh derivada da
//                Template Lo_Templ e aqui foram implementados alguns metodos
//                especificos.
//
//    Implementacao: Esta template eh derivada da template de lista ordenada
//                   encontrada em lo_templ.hpp.
//                   As especializacoes aqui incluidas mantem a compatibilidade
//                   do modelo com as listas de pontos utilizadas na versao
//                   anterior da biblioteca
//    Versao : 3.0
//    programador : Angelo Passaro
//    ultima revisao em : 27/06/96
//
// ****************************************************************************


#include "lo_templ.hpp"     // diretorio deve ser especificado no compilador
#include "lo_long.hpp"
#include "elemento_2d.hpp"


class Lista_Elementos_2D_Basico : public lev_math::Lo_Templ<Elemento_2D,long>
{
    public:
                          Lista_Elementos_2D_Basico() : lev_math::Lo_Templ<Elemento_2D,long> () {};
                          Lista_Elementos_2D_Basico(const long Q) : lev_math::Lo_Templ<Elemento_2D,long> (Q) {};
                          Lista_Elementos_2D_Basico(Lista_Elementos_2D_Basico &L) : lev_math::Lo_Templ<Elemento_2D,long> (L) {};

        double        Get_Volume    (long indice)
                            {return ptr_pontos[indice].Get_Volume();} ;
        lev_math::Uchar         Inclui(double vol, long p1, long p2, long p3)//, long p4)
                            {
                                lev_math::Uchar retorno = 0;
                                if ((num_componentes_absoluto) < num_max_componentes_absoluto)
                                {
                                    long vert[3]; vert[0]=p1; vert[1] = p2; vert[2]=p3;
                                    ptr_pontos[num_componentes_absoluto].Set_Vertices(vert);
                                    ptr_pontos[num_componentes_absoluto].Set_Volume(vol);
                                    num_componentes_absoluto++;
                                }
                                else retorno = 15;
                                return retorno;
                            };

        lev_math::Uchar         Inclui(Elemento_2D & el)
                            { return lev_math::Lo_Templ<Elemento_2D,long>::Inclui(el);} ;
} ; //fim da especifica��o da classe Lista_Elementos_2D_Basico


class Lista_Elementos_2D : public lev_math::Lo_Templ<Elemento_2D,long>

{
    public:
                          Lista_Elementos_2D() : lev_math::Lo_Templ<Elemento_2D,long> () {};
                          Lista_Elementos_2D(const long Q) : lev_math::Lo_Templ<Elemento_2D,long> (Q) {};
                          Lista_Elementos_2D(Lista_Elementos_2D &L) : lev_math::Lo_Templ<Elemento_2D,long> (L) {};

        lev_math::Uchar         Alivia_Lista  (void) { return 0;};      // mantida por compatibilidade da
                                                                            // interface com biblioteca anterior
        double        Get_Volume    (long indice)
                            {return ptr_pontos[indice].Get_Volume();} ;

        lev_math::Uchar         Inclui(double vol, long p1, long p2, long p3)
                            {
                                lev_math::Uchar retorno = 0;
                                if ((num_componentes_absoluto) < num_max_componentes_absoluto)
                                {
                                    long vert[3];vert[0]=p1; vert[1] = p2; vert[2]=p3;
                                    ptr_pontos[num_componentes_absoluto].Set_Vertices(vert);
                                    ptr_pontos[num_componentes_absoluto].Set_N_Vizinhos(0);
                                    ptr_pontos[num_componentes_absoluto].Set_Volume(vol);
                                    num_componentes_absoluto++;
                                }
                                else retorno = 15;
                                return retorno;
                            };

        lev_math::Uchar         Inclui(Elemento_2D & el)
                            { return lev_math::Lo_Templ<Elemento_2D,long>::Inclui(el);} ;
} ; //fim da especifica��o da classe Lista_Elementos_2D


class Lista_Elementos_2D_D : public lev_math::Lo_Templ<Elemento_2D,long>
{
    protected:
     long             num_comp_validos_absoluto;

    public:

                          Lista_Elementos_2D_D() : lev_math::Lo_Templ<Elemento_2D,long> () {num_comp_validos_absoluto = 0;};
                          Lista_Elementos_2D_D(const long Q) : lev_math::Lo_Templ<Elemento_2D,long> (Q) {num_comp_validos_absoluto = 0;};
                          Lista_Elementos_2D_D(Lista_Elementos_2D_D &L) : lev_math::Lo_Templ<Elemento_2D,long> (L) {};
        lev_math::Uchar         Alivia_Lista  (void) { return 0;};      // mantida por compatibilidade da
                                                                            // interface com biblioteca anterior
        lev_math::USINT         Atribui_Vizinhanca(long  *, lev_math::USINT, lev_math::USINT)           ;
//        Uchar         Atribui_Vizinhanca(Lo_Long &, long, USINT)           ;
        void          Compacta          (void)  {Compacta_Lista();}       ; // mantida por compatibilidade com a biblioteca anterior
        void          Compacta_Lista    (int forca = 0)                   ;
        void          Elimina           (long)                            ;
        long          Get_N_Comp_Validos(void){ return num_comp_validos_absoluto;} ;      // localizar seu uso no gerador de malha
        long          Get_N_Comp_Validos_Absoluto(void){ return num_comp_validos_absoluto;};
        double        Get_Volume    (long indice)
                            {return ptr_pontos[indice].volume;} ;
        lev_math::Uchar         Inclui (Elemento_2D &el)
                            { lev_math::Uchar ret = lev_math::Lo_Templ<Elemento_2D,long>::Inclui(el);if (!ret) num_comp_validos_absoluto++;return ret;} ;
//        Uchar         Inclui            (Lo_Pt_Malha  *, long , long , long );
        lev_math::Uchar         Inclui            (vCLevPtMalha &, long , long , long );
        lev_math::USINT         Localiza_Vizinhos (long  *, lev_math::USINT *, long  *, lev_math::USINT *, Pt_Malha *, double SIGMA);
//                USINT         Localiza_Vizinhos (Lo_Long &, Lo_Long  &, Pt_Malha *);

        void            Apaga(void) //*NMA*
        {
            lev_math::Lo_Templ<Elemento_2D,long> ::Apaga();
            num_comp_validos_absoluto = 0;
        }

}  ;

#endif   // fim do teste da palavra reservada _LO_EL_2D_
