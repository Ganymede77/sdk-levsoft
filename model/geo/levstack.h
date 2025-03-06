// Stack.h: interface for the CStack class.
//////////////////////////////////////////////////////////////////////

//=============================================================================
//    Arquivo:            Stack.h
//.............................................................................
//    Descricao:          Realiza as tarefas de uma pilha.
//.............................................................................
//    Autores:            Diogo Hideo Odan
//                        Angelo Passaro
//                        Nancy Mieko Abe
//-----------------------------------------------------------------------------
//    Classe CStack
//-----------------------------------------------------------------------------
//    Dados membros:
//
//    Associacoes:
//
//    Observacoes:
//.............................................................................
//    Data:                Dez/2002
//    Modificado em:
//    Modificado por:
//    Motivo:
//=============================================================================

#include "worldwindow.h"

#if !defined(AFX_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_)
#define AFX_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWWStack
{
public:
    CWWStack  ();
    CWWStack  (int stacksize);
    virtual ~CWWStack  ();
protected:
    //pilha
    CWorld_Window    *array;
    //informa qual a posi��o em que se encontra o ponteiro da pilha
    int                index;
    //informa quantos elementos j� foram alocados
    int                allocated;
    int                stack_Size;
public:
    void            Push(CWorld_Window worldwnd);
    CWorld_Window    Pop();
    bool            IsEmpty();
    void            Empty_Stack();
};

#endif // !defined(AFX_STACK_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_)
