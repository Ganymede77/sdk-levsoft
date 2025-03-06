#ifndef _LO_PT_TP_
#define _LO_PT_TP_


// ============================================================================
// Arquivo : Lo_Pt.hpp
// Descrição : Neste arquivo é declarada a template de lista ordenada de
//             pontos utilizada especificamente no sistema de projeto  de
//             malha de aterramento.
// Implementação :
//                  Esta template é derivada da template de lista ordenada
//                  encontrada em lo_templ.hpp.
//                  As especializações aqui incluidas mantêm a compatibilidade
//                  do modelo com as listas de pontos utilizadas na versão anterior
//                  da biblioteca
// Versão : 3.0
// programador : Angelo Passaro
// ultima revisão em : 16/06/96
// ============================================================================


#include "lo_templ.hpp"
#include "ponto.hpp"


template<class T, class Q> class Lo_Pt : public lev_math::Lo_Templ<T,Q>
{
	public:
		Lo_Pt()    ;
		Lo_Pt(const Q);
		virtual ~Lo_Pt();
		inline lev_math::Uchar        Inclui (T &);
		inline lev_math::Uchar        Inclui (T&, const Q pos_list);
		lev_math::Uchar        Inclui (double x, double y, double z);
		lev_math::Uchar        Inclui (double x, double y, double z, Q posicao);
		inline lev_math::USINT        Modifica(const Q  ind, T *);
		inline lev_math::USINT        Modifica(const Q  ind, T &numero);
		lev_math::USINT        Modifica(Q indice, double x, double y, double z);   // incluir declaracao e definicao das funcoes modifica da classe base.
		inline int   Resize_Conv (lev_math::USINT tam){return 0;} // mantida por compatibilidade com a lista versão 2.0
															  // deveria ser eliminada
}  ;


template<class T, class Q>
Lo_Pt<T,Q> :: Lo_Pt () : lev_math::Lo_Templ<T,Q>(){}

template<class T, class Q>
Lo_Pt<T,Q> :: Lo_Pt (const Q num_comp) : lev_math::Lo_Templ<T,Q> (num_comp) {}

template<class T, class Q>
Lo_Pt<T,Q> :: ~Lo_Pt(){};

template<class T, class Q>
lev_math::Uchar Lo_Pt<T,Q> :: Inclui (T & p1) { return lev_math::Lo_Templ<T,Q>:: Inclui(p1);}

template<class T, class Q>
lev_math::Uchar Lo_Pt<T,Q> :: Inclui (T &p1, const Q pos_list) {return lev_math::Lo_Templ<T,Q>:: Inclui(p1,pos_list);}

template<class T, class Q>
lev_math::Uchar Lo_Pt<T,Q> :: Inclui (double x, double y, double z)
{
	T p1; p1.Set_Coord(x,y);
	return Inclui(p1);
}

template<class T, class Q>
lev_math::Uchar Lo_Pt<T,Q> :: Inclui(double x, double y, double z, Q posicao)
{
	T p1; p1.Set_Coord(x,y,z);
	return Inclui(p1,posicao);
}

template<class T, class Q>
lev_math::USINT Lo_Pt<T,Q> :: Modifica(const Q indice, T * p1) {return lev_math::Lo_Templ<T, Q>::Modifica(indice, p1);}
template<class T, class Q>
lev_math::USINT Lo_Pt<T,Q> :: Modifica(const Q indice, T & p1) {return lev_math::Lo_Templ<T, Q>::Modifica(indice, p1);}

template<class T, class Q>
lev_math::USINT Lo_Pt<T,Q> :: Modifica(Q indice, double x, double y, double z)
{
	T p1; p1.Set_Coord(x,y,z);
	return lev_math::Lo_Templ<T,Q>::Modifica(indice, p1);
}


#endif   // #if !defined (_LO_PT_TP_)

