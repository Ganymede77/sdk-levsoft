#ifndef __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_ILU__
#define __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER_ILU__

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_inc_fac_pre_ilu.hpp                                          */
/*---------------------------------------------------------------------------*/
/* Conteúdo: template <class M, class Q>  CInc_Fac_Preconditioner_ILU                 */
/*---------------------------------------------------------------------------*/
/* Descrição: Classe base das classes que implementam                        */
/*               a  resolução de sistemas de equações lineares               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observações: */
/*                                                                           */
/*                         *NMA*     *AP*     *LAA*                          */
/*                                                                           */
/****************************************************************************/
#include "lev_inc_fac_pre.hpp"
#include "lev_inc_fac_pre_ilu.hpp"

namespace lev_math {
template <class M, class T,class Q>
class CInc_Fac_Preconditioner_ILU : public CInc_Fac_Preconditioner<M,T,Q>
{
  private:




    Sparse_Triangular_Matrix<T,Q> L;
    Sparse_Triangular_Matrix<T,Q> U;

    int validate_matrix(M &);

  public:

    CInc_Fac_Preconditioner_ILU(void);
    ~CInc_Fac_Preconditioner_ILU();

    void build(M&);
    void apply(Ve_Templ<T,Q>&, Ve_Templ<T,Q>&);

	void libera(void){};
};


/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        CInc_Fac_Preconditioner_ILU                       */
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
CInc_Fac_Preconditioner_ILU<M,T,Q>::CInc_Fac_Preconditioner_ILU(void)
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome do Arquivo:       LEV_inc_fac_pre.cpp                               */
/* Nome da Funcao:        ~CInc_Fac_Preconditioner_ILU                      */
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
CInc_Fac_Preconditioner_ILU<M,T,Q>::~CInc_Fac_Preconditioner_ILU()
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
int CInc_Fac_Preconditioner_ILU<M,T,Q>::validate_matrix(M &A)
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
/*          Constroi a matriz triangular, resultante da decomposi�ao da     */
/*   matriz fornecida, pelo metodo LU Incompleto (ILU).                     */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa                                            */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_ILU<M,T,Q>::build(M& A)
{
  A.Orientation(LINE);

  U.Format(TRIANGULAR_SUPERIOR);
  U = A;
  U.value.Zera();


  L.Format(TRIANGULAR_INFERIOR);
  L = A;
  L.value.Zera();


  long tamanho = A.prim.Get_Total_Componentes();
  long total_elementosU = U.position.Get_Total_Componentes();
  long total_elementosL = L.position.Get_Total_Componentes();
  long posicao, posicaoA;
  long z, i, j, k;

  for(z = 0; z < tamanho; z++)
  {
    long fimL = ((z==tamanho-1)?total_elementosL:L.prim[z+1]); // se for última linha pega o último elmento + 1
    L.value[fimL - 1] = 1;
  }

  L.Orientation(COLUMN);

  for(i = 0; i < tamanho; i++)
  {
    long inicioU = U.prim[i];
    long fimU = ((i==tamanho-1)?total_elementosU:U.prim[i+1]); // se for última linha pega o último elmento + 1

    long inicioL = L.prim[i];
    long fimL = ((i==tamanho-1)?total_elementosL:L.prim[i+1]); // se for última linha pega o último elmento + 1

    for(j = inicioU; j < fimU; j++)
    {
      for(k = inicioL; k < fimL; k++)
      {
        if (U.position[j] >= L.position[k])
        { // Upper
          posicao = U.Get_Position(L.position[k], U.position[j]);
          if (posicao != -1) {
            if ((posicao >= inicioU) && (posicao < fimU)) {
              posicaoA = A.Get_Position(L.position[k], U.position[j]);
              U.value[posicao] = A.value[posicaoA] - U.value[posicao];
            } else {
              U.value[posicao] = U.value[posicao] + U.value[j] * L.value[k];
            }
          }
        } else { //Lower
          posicao = L.Get_Position(L.position[k], U.position[j]);
          if (posicao != -1) {
            if ((posicao >= inicioL) && (posicao < fimL)) {
              posicaoA = A.Get_Position(L.position[k], U.position[j]);
              L.value[posicao] = (A.value[posicaoA] - L.value[posicao]) / U.value[U.prim[U.position[j]]];
            } else {
              L.value[posicao] = L.value[posicao] + U.value[j] * L.value[k];
            }
          }
        }
      }
    }
  }

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
/*     Funçãoo que aplica o pré-condicionamento                              */
/*     Substitui  a resolução de Hx=b por UTz=b e Ux = b                    */
/*                                                                          */
/* Parametros:                                                              */
/*             Ve_Templ<Q, long>& x: vetor com a solução do sistema    */
/*             Ve_Templ<Q,long> b: vetor das ações (righ hand side)    */
/*                                                                          */
/* Valor de retorno:                                                        */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
void CInc_Fac_Preconditioner_ILU<M,T,Q>::apply(Ve_Templ<T,Q>& b, Ve_Templ<T,Q>& x)
{
  Ve_Templ<T,Q> z;

  CInc_Fac_Preconditioner<M,T,Q>::forward_substitution(L, z, b);
  CInc_Fac_Preconditioner<M,T,Q>::back_substitution(U, x, z);
}

} //end namespace

#endif
