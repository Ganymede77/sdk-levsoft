//***************************************************************************
//***************************************************************************
//
//                    C L A S S    C C O M P O S E D F U N C T I O N
//
//***************************************************************************
//***************************************************************************

#include <cstdio>
#include "composedfunctions.hpp"


CComposedFunction :: CComposedFunction()
{
	num_functions = 0;
} // fim do construtor de CComposedFunction


CComposedFunction :: ~CComposedFunction()
{
	functions.clear();
	num_functions = 0;
} // fim do destruidor de CComposedFunction


//===========================================================================
//               class CComposedFunction :: Associate
//===========================================================================
// Descricao:
//		Associa o vetor de ponteiros de funcoes
//
//-----------------------------------------------------------------------------
void CComposedFunction :: Associate(CFunction *f)
{
	functions.push_back(f);
	num_functions++;
} // fim de Associate


//===========================================================================
//               class CComposedFunction :: GetValue
//===========================================================================
// Descricao:
//		Retorna o produto entre as funcoes.
//
//-----------------------------------------------------------------------------
double CComposedFunction :: Get_Value(double k)
{
	double valor	= 1.0;
	int i;

	for(i=0;i<num_functions;i++) {
		valor = valor * (functions[i]->Get_Value(k));
	}
	return valor;
} // fim de GetValue
