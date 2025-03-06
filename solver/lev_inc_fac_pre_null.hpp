#ifndef __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_NULL__
#define __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_NULL__

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_inc_fac_pre_null.hpp                                         */
/*---------------------------------------------------------------------------*/
/* Conteúdo: template <class M, class Q>  CInc_Fac_Preconditioner_Null                */
/*---------------------------------------------------------------------------*/
/* Descrição: Classe que implementa um pré-condicionador nulo, para resolução*/
/*             do sistema de equações sem pré-condicionador                  */
/*-------------------------------------------------------------------------*/
/*                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observações: */
/*                                                                           */
/*                         *NMA*     *AP*     *LAA*                          */
/*                                                                           */
/****************************************************************************/
#include "lev_inc_fac_pre.hpp"
#include "lev_inc_fac_pre_null.hpp"


namespace lev_math {

template <class M,class T, class Q>
class CInc_Fac_Preconditioner_Null : public CInc_Fac_Preconditioner<M,T,Q>
{
  private:
    int validate_matrix(M &);

  public:

    CInc_Fac_Preconditioner_Null(void);
    ~CInc_Fac_Preconditioner_Null();

    void build(M&);
    void apply(Ve_Templ<T,Q>&, Ve_Templ<T,Q>&);
	void libera(void){};
};

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        CInc_Fac_Preconditioner_Null                      */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/*            construtor                                                    */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
CInc_Fac_Preconditioner_Null<M,T,Q>::CInc_Fac_Preconditioner_Null(void)
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome do Arquivo:       LEV_inc_fac_pre.cpp                               */
/* Nome da Funcao:        ~CInc_Fac_Preconditioner_Null                     */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/*            ~CInc_Fac_Preconditioner: destrutor                           */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
CInc_Fac_Preconditioner_Null<M,T,Q>::~CInc_Fac_Preconditioner_Null()
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        validate_matrix                                   */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/*                                                                          */
/* Descricao:                                                               */
/*                                                                          */
/*            Fun��o implementada para manter a interface das classes de    */
/*     pr�-condicionamento                                                 */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
int CInc_Fac_Preconditioner_Null<M,T,Q>::validate_matrix(M &A)
{
  return 1;
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        build                                             */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/* Descricao:                                                               */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa sim�trica                                  */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_Null<M,T,Q>::build(M& A)
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        apply                                             */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_Null<M,T,Q>::apply(Ve_Templ<T,Q>& b, Ve_Templ<T,Q>& x)
{
  x = b;
}

} //end namespace
#endif
