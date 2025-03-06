#ifndef _LISTAO_H
#define _LISTAO_H


/**
 @ingroup listas
 @class Lista_Ordenada
*/

namespace lev_math {

template <class Q>
class Lista_Ordenada
{
	protected :
		Q  num_max_componentes_absoluto;
		Q  num_componentes_absoluto;
	public    :
		Lista_Ordenada()
		{
			num_max_componentes_absoluto = 0;
			num_componentes_absoluto = 0;
		} ;

		virtual ~Lista_Ordenada()
		{
			num_max_componentes_absoluto = 0;
			num_componentes_absoluto = 0;
		};

		virtual inline void Apaga(void) = 0;  // virtual  // apaga toda a lista

		inline  Q Get_N_Max_Com_Abs(void) const { return num_max_componentes_absoluto; };
		inline  Q Get_Total_Componentes(void) const { return num_componentes_absoluto; };  // numero atual de elementos da lista
		virtual inline void Set_N_Max_Com_Abs(const Q) = 0;
};


} // end namespace listas


#endif
