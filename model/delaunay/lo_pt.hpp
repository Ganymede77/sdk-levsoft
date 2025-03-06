#ifndef _LO_PT_
#define _LO_PT_


// ============================================================================
// Arquivo : Lo_Pt.hpp
// Descrição : Neste arquivo são declaradas as classes de lista ordenada de
//             pontos utiliadas especificamente no sistema de projeto  de
//             malha de aterramento.
// Implementação :  A definição das funções membro pode ser encontrada no
//                  arquivo lo_pt.cpp.
//                  Estas classe é  derivada da template de lista ordenada
//                  de pontos encontrada em lo_pt_tp.hpp.
// Versão : 3.0
// programador : Angelo Passaro
// ultima revisão em : 14/01/97
// ============================================================================


#include "lo_pt_tp.hpp"


typedef Lo_Pt<Ponto,long> Lista_Pontos;   // reproduz Lista_Pontos da biblioteca original


class Lo_Pt_El : public Lo_Pt<Pt_Malha,long>
{
	friend class Elemento_2D;
	public:
	 inline	Lo_Pt_El(lev_math::USINT = 20000);
					  ~Lo_Pt_El(){};

			  void   Elimina_Ultimo(void){if (num_componentes_absoluto > 0) num_componentes_absoluto--;}

	 inline double Get_Peso(long indice)
					{	return ptr_pontos[indice].Get_Peso(); } ;

			  lev_math::Uchar  Inclui ( Pt_Malha &T )
					{  return Lo_Pt<Pt_Malha,long>::Inclui(T);	};

			  lev_math::Uchar  Inclui ( Pt_Malha &T, const long Q)
					{	return Lo_Pt<Pt_Malha,long>::Inclui(T,Q);	};

			  lev_math::Uchar  Inclui (double x, double y, double z, lev_math::Uchar t=0) // usada FUTURAMENTE por refina
					{
						lev_math::Uchar ret = Lo_Pt<Pt_Malha,long>::Inclui(x,y,z);
						if (!ret) { ptr_pontos[num_componentes_absoluto-1].Set_Tipo(t);
										ptr_pontos[num_componentes_absoluto-1].Set_TipoCC(0);}
						return ret;
					} ;

			  lev_math::Uchar Inclui (double x, double y, double z,long  posicao_na_lista, lev_math::Uchar t = 0) // sobrepoe inclui de Lista
					{
						Pt_Malha p1; p1.Set_Coord(x,y); p1.Set_Tipo(t); p1.Set_TipoCC(0);
						return Inclui(p1,(const long) posicao_na_lista);
					} ;
	lev_math::USINT Set_Peso(long indice, double Peso)
					{	ptr_pontos[indice].Set_Peso(Peso); return 0;} ;    // função mantida por compatibilidade com versao anterior da biblioteca

};
Lo_Pt_El :: Lo_Pt_El(lev_math::USINT i) : Lo_Pt<Pt_Malha,long>(){};

typedef Lo_Pt_El Lo_Pt_Malha;   // escondido para testar implementacao com relacao de amizade no corpo de dgr15_10.hpp


#endif
