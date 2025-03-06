#ifndef _NOS_ELM_
#define _NOS_ELM_


// ============================================================================
// Arquivo : vetor.hpp
// Descrição :	Este arquivo contem a struct para armazenar um vetor
//              de índices de elementos.
//
// Implementação :Versão : 1.0
// programador : Guilherme Moreira Magalhaes
// ultima revisão em : 02/05/2000
// ============================================================================


#include "lo_templ.hpp"


class Nos_Elementos
{
public:

	long  Get_IElementos(long  elemtos);
	void Set_NElementos(char);
	char Get_NElementos();
	char Ad_Elemento(long);
	Nos_Elementos();
	~Nos_Elementos();
	Nos_Elementos & operator =(Nos_Elementos &v);
	char operator ==(Nos_Elementos &v);

protected:
	long lista_elem[64];
	char n_elementos;
};

typedef lev_math::Lo_Templ<Nos_Elementos, long> Lo_Nos_Elementos;


#endif
