#ifndef __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER__
#define __LEV_INCOMPLETE_FACTORIZATION_PRECONDITIONER__


#include "sparse_matrix.hpp"
#include "sparse_triangular_matrix.hpp"
/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
* Arquivo: lev_inc_fac_pre.hpp                                              *
*---------------------------------------------------------------------------*
* Conteúdo: template <class M, class Q> CInc_Fac_Preconditioner                      *
*---------------------------------------------------------------------------*
* Descrição: Classe base das classes que implementam                        *
*               a fatorização das matrizes (pré-condicionamento)            *
*---------------------------------------------------------------------------*
*                                                                           *
* string para verificacao de erros ou modificacoes futuras ou observações: *
*                                                                           *
*                         *NMA*     *AP*     *LAA*                          *
*                                                                           *
****************************************************************************
*/

namespace lev_math {

template <class M,class T,class Q>
class CInc_Fac_Preconditioner
{
  private:

   typedef typename number_traits<T>::number_type REALTYPE;

    void init(void);

  protected:


    void forward_substitution(Sparse_Triangular_Matrix<T,Q>&,
    			 Ve_Templ<T,Q>&, Ve_Templ<T,Q>&);
    void back_substitution(Sparse_Triangular_Matrix<T,Q>&,
    			 Ve_Templ<T,Q>&, Ve_Templ<T,Q>&);
    virtual int validate_matrix(M &) = 0;

  public:
    CInc_Fac_Preconditioner(void);
    virtual ~CInc_Fac_Preconditioner();

    virtual void build(M&) = 0;
    virtual void apply(Ve_Templ<T,Q>&, Ve_Templ<T,Q>&) = 0;
    virtual void libera(void) = 0;

};





/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        CInc_Fac_Preconditioner                           *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:                                                               *
*                                                                          *
*            construtor                                                    *
*                                                                          *
****************************************************************************
*/
template <class M, class T,class Q>
CInc_Fac_Preconditioner<M, T ,Q>::CInc_Fac_Preconditioner(void)
{
  //init();
}
/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        ~CInc_Fac_Preconditioner                          *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
****************************************************************************
* Descricao:                                                               *
*                                                                          *
*            destrutor                                                     *
*                                                                          *
*                                                                          *
****************************************************************************
*/
template <class M, class T,class Q>
CInc_Fac_Preconditioner<M,T,Q>::~CInc_Fac_Preconditioner()
{
}
/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome do Arquivo:       LEV_pre.cpp                                       *
* Nome da Funcao:        init                                              *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:                                                               *
*                                                                          *
*            função para inicialização de variáveis                        *
*                                                                          *
****************************************************************************
*/
template <class M, class T,class Q>
void CInc_Fac_Preconditioner<M,T,Q>::init(void)
{
  std::cout << std::endl << "Initializing Preconditioner" << std::endl;
}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome do Arquivo:       LEV_pre.cpp                                       *
* Nome da Funcao:        forward_substitution                              *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12102003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
*                                                                          *
* Descricao:                                                               *
*                                                                          *
*            forward_substitution: Lz=b                                    *
*                                                                          *
* Parametros:   *NMA*                                                      *
*                                                                          *
*  Sparse_Triangular_Matrix<Q>& L: matriz esparsa triangular inferior *
*  Ve_Templ<DTYPE, Q>& z : vetor da soluçãoo                            *
*  Ve_Templ<Q,long>& b: vetor das ações (righ hand side)              *
*                                                                          *
* Valor de retorno:                                                        *
*                void                                                      *
*                                                                          *
*                                                                          *
****************************************************************************
*/

template <class M, class T,class Q>
void CInc_Fac_Preconditioner<M,T,Q>::forward_substitution(Sparse_Triangular_Matrix<T,Q>& L, Ve_Templ<T,Q>& z, Ve_Templ<T,Q>& b)
{
  L.Orientation(LINE);
  z = b;

  long tamanho = L.prim.Get_Total_Componentes();
  long total_elementos = L.position.Get_Total_Componentes();
  long i, j;

  for(i = 0; i < tamanho; i++)
  {
    z[i] = 0;

    // se for �ltima linha pega o �ltimo elmento + 1
    long fim = ((i==tamanho-1)?total_elementos:L.prim[i+1]);

    for(j = L.prim[i]; j < fim - 1; j++)
    {
      z[i] = z[i] + (L.value[j] * z[L.position[j]]);
    }

    z[i] = (b[i] - z[i]) / L.value[fim-1];
  }
}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        back_substitution                                 *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:                                                               *
*                                                                          *
*            back_substitution: Ux=b                                       *
*                                                                          *
* Parametros:   *NMA*                                                      *
*                                                                          *
*  Sparse_Triangular_Matrix<Q>& U: matriz esparsa triangular superior *
*  Ve_Templ<DTYPE, Q>& x : vetor da solução                                *
*  Ve_Templ<Q,long>& b: vetor das ações (righ hand side)              *
*                                                                          *
* Valor de retorno:                                                        *
*                void                                                      *
*                                                                          *
*                                                                          *
****************************************************************************
*/
template <class M, class T,class Q>
void CInc_Fac_Preconditioner<M,T,Q>::back_substitution(Sparse_Triangular_Matrix<T,Q>& U, Ve_Templ<T,Q>& x, Ve_Templ<T,Q>& b)
{
  U.Orientation(LINE);

  x = b;

  long tamanho = U.prim.Get_Total_Componentes();
  long total_elementos = U.position.Get_Total_Componentes();
  long i, j;

  for(i = tamanho - 1; i >= 0; i--)
  {
    x[i] = 0;

    // se for �ltima linha pega o �ltimo elmento + 1
    long fim = ((i==tamanho-1)?total_elementos:U.prim[i+1]);

    for(j = U.prim[i] + 1; j < fim; j++)
    {
      x[i] = x[i] + (U.value[j] * x[U.position[j]]);
    }

    x[i] = (b[i] - x[i]) / U.value[U.prim[i]];
  }
}

} // end lev_math
#endif
