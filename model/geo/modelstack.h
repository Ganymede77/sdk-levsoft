#ifndef AFX_MODEL_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_
#define AFX_MODEL_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_


//===================================================================================================
//	Arquivo:			Stack.h
//...................................................................................................
//	Descrição:			realiza as tarefas de uma pilha.
//...................................................................................................
//	Autores:			Diogo Hideo Odan
//						Angelo Passaro
//						Nancy Mieko Abe
//---------------------------------------------------------------------------------------------------
//	Classe CStack
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:
//
//	Observações:
//...................................................................................................
//	Data:				Dez/2002
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================


#include "levgeodata.h"


class CModelStack
{
public:
	CModelStack();
	CModelStack(int stacksize);
	virtual ~CModelStack();

	void				Include(CLevGeoData model);
	bool				IsEmpty();
	void				Empty_Stack();
	CLevGeoData			Forward();
	CLevGeoData			Backward(CLevGeoData model);
	void				First(CLevGeoData model);
	bool				IsBackOK();
	bool				IsForwardOK();
protected:
	CLevGeoData*		array;
	//informa qual a posição em que se encontra o ponteiro da pilha
	int					index;
	//informa quantos elementos já foram alocados
	int					allocated;
	int					stack_Size;
	void				AllocateStack(int size);
	int					redoLevels;
	int					undoIndex;
	void				Push(CLevGeoData model);
};


#endif // !defined(AFX_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_)
