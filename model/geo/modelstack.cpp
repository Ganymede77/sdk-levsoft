// Stack.cpp: implementation of the CModelStack class.
//
//////////////////////////////////////////////////////////////////////

//===================================================================================================
//	Arquivo:			Stack.h
//...................................................................................................
//	Descri��o:			realiza as tarefas de uma pilha.
//...................................................................................................
//	Autores:			Diogo Hideo Odan
//						Angelo Passaro
//						Nancy Mieko Abe
//===================================================================================================


#include "modelstack.h"


#define DEFAULT_STACK_SIZE  10


#if 10
CModelStack::CModelStack()
{
	allocated = index = -1;
	redoLevels = 0;
	undoIndex = -1;

	array = NULL;

	AllocateStack(DEFAULT_STACK_SIZE);

	stack_Size = DEFAULT_STACK_SIZE;
}


CModelStack::CModelStack(int stacksize)
{
	allocated = index = -1;
	redoLevels = 0;
	undoIndex = -1;

	array = NULL;

	AllocateStack(stacksize);

	stack_Size = stacksize;
}


CModelStack::~CModelStack()
{
	if(array) {
		delete [] array;
		array = NULL;
	}
	index = allocated = -1;
}


void CModelStack::AllocateStack(int size)
{
	array = new CLevGeoData[DEFAULT_STACK_SIZE];
	if(!array)
		throw "Error: Memory allocation - CModelStack::AllocateStack()";

	stack_Size = DEFAULT_STACK_SIZE;
}


//===============================================================================================================================
// Push
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
// Modificado por: Nancy
// Data:	Novembro/2004
// Motivo:	Utiliza��o para gerenciamento de Undo no modelador
// Obs:		Baseada na classe CStack
//===============================================================================================================================
// Descri��o:	- inclui um ponteiro para o modelo na pilha
//===============================================================================================================================
// Par�metros:
//				Entrada:
//				- CLevGeoData*: ponteiro para modelo
//
//				Saida:
//						- nenhuma.
//===============================================================================================================================
void CModelStack::Push(CLevGeoData model)
{
	int i;

	++index;

//	if(index > stack_Size-1) index = 0;

	//model.BuildObjectsHistory();

	if(index < stack_Size)
		array[index] = model;
	else
	{
		for(i=1;i<stack_Size;i++)
		{
			array[i-1] = array[i];
		}

		index = stack_Size-1;
		array[index] = model;
	}

//	array[index].BuildObjectsHistory();

	if(index > allocated)
			allocated = index;
}

void CModelStack::Include(CLevGeoData model)
{
	Push(model);
    redoLevels	= 0;
	undoIndex   = index;
}

//===============================================================================================================================
// Pop
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
// Modificado por: Nancy
// Data:	Novembro/2004
// Motivo:	Utiliza��o para gerenciamento de Undo no modelador
// Obs:		Baseada na classe CStack
//===============================================================================================================================
// Descri��o:	- retorna o ultimo elemento da Pilha.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- retorna o elemento apontado por index e atualiza index-1
//===============================================================================================================================
//CLevGeoData CModelStack::Pop()
//{
//	if(index < 0)
//		return NULL;

//	return array[index--];
//}//
//===============================================================================================================================
// IsEmpty
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
//===============================================================================================================================
// Descri��o:	- Verifica a pilha est� vazia.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- retorna os limites anteriores da janela de visualiza��o.
//===============================================================================================================================
bool CModelStack::IsEmpty()
{
	bool ret = false;//FALSE;

	if(!index) ret = true;

	return ret;
}
//===============================================================================================================================
// Empty_Stack
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
//===============================================================================================================================
// Descri��o:	- Esvazia logicamente a pilha, invalidando todas a informa��es, reiniciandos as variaveis de controle.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- nenhum.
//===============================================================================================================================
void CModelStack::Empty_Stack()
{
	allocated = index = -1;
    redoLevels	= 0;

}
//===============================================================================================================================
// Forward
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
// Modificado por: Nancy
// Data:	Novembro/2004
// Motivo:	Utiliza��o para gerenciamento de Undo no modelador
// Obs:		Baseada na classe CStack
//===============================================================================================================================
// Descri��o:	-
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						-
//===============================================================================================================================
CLevGeoData CModelStack::Forward()
{
	if(redoLevels > 0)
	{
		index++;

		if (index > allocated)
		{
			index = allocated;
			redoLevels = 0;
		}
		else redoLevels--;
	}

	//array[index].RebuildPointersStatus();
//	array[index].RecoverObjectsHistory();
	return array[index];
/*
	if(redoLevels > 0)
	{
		undoIndex++;
		if (undoIndex > allocated)
		{
			undoIndex = allocated;
			redoLevels = 0;
		}
		else redoLevels--;

//		Push(array[undoIndex]);
	}

	return array[undoIndex];
	*/
}
//===============================================================================================================================
// Backward
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
// Modificado por: Nancy
// Data:	Novembro/2004
// Motivo:	Utiliza��o para gerenciamento de Undo no modelador
// Obs:		Baseada na classe CStack
//===============================================================================================================================
// Descri��o:
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- ponteiro para "�ltimo" elemento da lista
//===============================================================================================================================
CLevGeoData CModelStack::Backward(CLevGeoData model)
{
	--index;
	if(index <0) index = 0;
	else redoLevels++;

////	array[index].RebuildPointersStatus();
//	array[index].RecoverObjectsHistory();

	return array[index];
/*
	--undoIndex;

	if (undoIndex < 0)
	{
		undoIndex = 0;
//		undoIndex = allocated;
	}
	else
		redoLevels++;
*/

//	Push(array[undoIndex]);

	//array[undoIndex].RebuildPointersStatus();
//	return array[undoIndex];
}
//===============================================================================================================================
// IsBackOK
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
//===============================================================================================================================
// Descri��o:	- verifica se � possivel avan�ar no nivel de zoom.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- retorna TRUE se for poss�vel avan�ar, caso contr�rio FALSE.
//===============================================================================================================================
bool CModelStack::IsBackOK()
{
	bool status = false;//FALSE;

	if(index>0) status = true;
		else status = false;
/*
	if(allocated>0)
	{
		if(allocated < stack_Size)
		{
			if(undoIndex>0) status = TRUE;
			else status = FALSE;
		}
		else
		{
			status = TRUE;
		}
	}
	else status = FALSE;
*/
	return status;
}
//===============================================================================================================================
// IsFowardOK
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
//===============================================================================================================================
// Descri��o:	- verifica se � possivel recuar no nivel de zoom.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- nenhum.
//
//				Saida:
//						- retorna TRUE se for poss�vel recuar, caso contr�rio FALSE.
//===============================================================================================================================
bool CModelStack::IsForwardOK()
{
	bool status;

	if(redoLevels>0) status = true;
	else status = false;

	return status;
}
//===============================================================================================================================
// NewZoom
//===============================================================================================================================
// Autor:	Diogo Hideo Odan
// Data:    Dez/2002
// Projeto: Prot�tipo Explorador de Resultados
//===============================================================================================================================
// Descri��o:	- recebe as informa��es dos limites anteriores ao novo zoom realizado, salvando-os na pilha.
//===============================================================================================================================
// Par�metros:
//				Entrada:
//						- worldwnd - limites da janela a serem armazenados.
//
//				Saida:
//						- nenhum.
//===============================================================================================================================
void CModelStack::First(CLevGeoData model)
{
	array[0].FreeMemory();

	array[0] = model;

	index = 0;

	allocated = index;
}
#endif
