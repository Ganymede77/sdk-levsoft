#ifndef __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_IC__
#define __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_IC__

// ********************************************************************** //
// Projeto: LEVSoft
// ********************************************************************** //
// Arquivo: lev_inc_fac_pre_ic.hpp
// ---------------------------------------------------------------------- //
// Conteudo: template <class M> CInc_Fac_Preconditioner_IC
// ---------------------------------------------------------------------- //
// Descricao: Classe que implementa a fatorizacao de matrizes pelo metodo
//               Cholesky Incompleto (IC)
// ---------------------------------------------------------------------- //
//
// string para verificacao de erros ou modificacoes futuras ou observacoes:
//
//                         *NMA*     *AP*     *LAA*
//
// ********************************************************************** //
#include <cmath>
#include "lev_inc_fac_pre.hpp"

namespace lev_math {

 class CInput_Exception;


 template <class M,class T, class Q>
class CInc_Fac_Preconditioner_IC : public CInc_Fac_Preconditioner<M,T,Q>
{
  private:


    Sparse_Triangular_Matrix<T,Q> U;
    Sparse_Triangular_Matrix<T,Q> UT;

    int validate_matrix(M &);

  public:

    CInc_Fac_Preconditioner_IC(void);
    virtual ~CInc_Fac_Preconditioner_IC();

    void build(M&);
    void apply(Ve_Templ<T, Q>&, Ve_Templ<T, Q>&);

    Q getp(Q indice, Q inicio, Q fim);

	void libera(void);

};


template <class M,class T, class Q>
void CInc_Fac_Preconditioner_IC<M,T,Q>::libera(void)
{
 U.libera();
 UT.libera();

}


/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        CInc_Fac_Preconditioner_IC                        */
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
CInc_Fac_Preconditioner_IC<M,T,Q>::CInc_Fac_Preconditioner_IC(void)
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome do Arquivo:       LEV_inc_fac_pre.cpp                               */
/* Nome da Funcao:        ~CInc_Fac_Preconditioner_IC                       */
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
CInc_Fac_Preconditioner_IC<M,T,Q>::~CInc_Fac_Preconditioner_IC()
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
/* Descricao: *NMA*                                                         */
/*                                                                          */
/*    O metodo IC pode ser aplicado para a fatorizacao    de                */
/* matrizes simétricas. A função verifica se a matriz fornecida é simétrica */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa                                            */
/*                                                                          */
/* Valor de retorno:                                                        */
/*             0: se não for simétrica                                      */
/*             1: se for simétrica                                          */
/****************************************************************************/
template <class M,class T, class Q>
int CInc_Fac_Preconditioner_IC<M,T,Q>::validate_matrix(M &A)
{
  if ((A.Format() != SYMMETRIC_SUPERIOR) &&
      (A.Format() != SYMMETRIC_INFERIOR)) {
    return 0;
  }

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
/*          Constroi a matriz triangular, resultante da decomposição da     */
/*   matriz fornecida, pelo metodo Cholesky Incompleto.                     */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa simétrica                                  */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_IC<M,T,Q>::build(M& A)
{

  if (validate_matrix(A) == 0) {
    throw CInput_Exception("Matriz inválida para este Preconditioner.");
  }

  U.Format(TRIANGULAR_SUPERIOR);
  U = A;

  U.value.Zera();

  Q tamanho = U.prim.Get_Total_Componentes();
  Q total_elementos = U.position.Get_Total_Componentes();
  Q posicao;
  Q fim2, ultimaposicao;
  Q i, j, k;


  for(i = 0; i < tamanho; i++)
  {
    Q fim = ((i==tamanho-1)?total_elementos:U.prim[i+1]); // se for �ltima linha pega o �ltimo elmento + 1
    Q inicio = U.prim[i];

    for(j = inicio; j < fim; j++)
    {
      // contribui��o de cada linha para cada elemento uik
	  ultimaposicao = U.prim[U.position[j]];
	  fim2 = (U.position[j]==tamanho-1)?total_elementos:U.prim[U.position[j]+1];
      for(k = j; k < fim; k++)
      {
        posicao = getp(U.position[k], ultimaposicao, fim2);
		//posicao = U.Get_Position(U.position[j], U.position[k]);
        if (posicao != -1) {
          if ((posicao >= inicio) && (posicao < fim)) {
            if (i == U.position[k])
			{
			  //*NMA: verificacao se a diagonal � negativa. Este e'o teste utilizado para
				// verifica��o se a matriz � definida positiva

			  if(A.value[posicao] < U.value[posicao])
			  {
			    throw CInput_Exception("A matriz não é definida positiva.");
			  }

              U.value[posicao] = sqrt(A.value[posicao] - U.value[posicao]);
            } else {
              // verificar se coloca um get_position pois n�o se sabe se a matriz ser� simetrica tringular ou n�o
              U.value[posicao] = (A.value[posicao] - U.value[posicao]) / U.value[U.prim[i]];
            }
          } else {
            U.value[posicao] = U.value[posicao] + U.value[j] * U.value[k];
          }
		  ultimaposicao = posicao;
        }
      }
    }
  }

  UT = U.Transposet();
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
/*     Função que aplica o pré-condicionamento                              */
/*     Substitui  a resolução de Hx=b por UTz=b e Ux = b                    */
/*                                                                          */
/* Parametros:                                                              */
/*             Ve_Templ<Q, Q>& x: vetor com a solução do sistema    */
/*             Ve_Templ<Q,Q> b: vetor das ações (righ hand side)    */
/*                                                                          */
/* Valor de retorno:                                                        */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_IC<M,T,Q>::apply(Ve_Templ<T, Q>& b, Ve_Templ<T,Q>& x)
{
  Ve_Templ<T,Q> z;

  CInc_Fac_Preconditioner<M,T,Q>::forward_substitution(UT, z, b);
  CInc_Fac_Preconditioner<M,T,Q>::back_substitution(U, x, z);

  z.Libera();
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome do Arquivo:       LEV_pre_ic.cpp                                    */
/* Nome da Funcao:        CPreconditioner                                   */
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
/*       Obs: A descrição a seguir considera uma matriz armazenada por      */
/*    linha. Se o armazenamento for por coluna, a descriçãoo é análoga.     */
/*                                                                          */
/*      Retorna posição,no vetor value da matriz esparsa, do elemento da    */
/*   coluna "indice" da linha que começaa em "inicio" e termina em "fim"     */
/*                                                                          */
/*                                                                          */
/* Parametros:                                                              */
/*                                                                          */
/*    Q indice: indice da coluna na matriz esparsa                       */
/*    Q indice: inicio da linha no vetor position                        */
/*    Q indice: fim da linha no vetor position                           */
/*                                                                          */
/* Valor de retorno:                                                        */
/*                                                                          */
/*    Q retorno: indice da coluna no vetor position                      */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
Q CInc_Fac_Preconditioner_IC<M,T,Q>::getp(Q indice, Q inicio, Q fim) {
  Q retorno = -1;
  Q i;
  for (i = inicio; i < fim; i++)
  {
    if (indice < U.position[i]) break;
    else if (indice == U.position[i]) { retorno = i; break;}
  }
  return retorno;
}

}  //end namespace
#endif

