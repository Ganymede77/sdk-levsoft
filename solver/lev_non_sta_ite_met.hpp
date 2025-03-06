#ifndef __LEV_NONSTATIONARY_ITERATIVE_METHOD__
#define __LEV_NONSTATIONARY_ITERATIVE_METHOD__

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_non_sta_ite_met.hpp                                          */
/*---------------------------------------------------------------------------*/
/* Conteúdo: template <class MATRIX, class Q>  CNon_Sta_Ite_Method                         */
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

#include "sparse_symm_matrix.hpp"
#include "lev_input_exception.hpp"
#include "lev_inc_fac_pre_null.hpp"
#include "lev_inc_fac_pre_ic.hpp"
#include "lev_inc_fac_pre_ilu.hpp"
#include "utility.hpp"
#include "utilclass.h"

#include <string>



/****************************************************************************/
/* Alterações para compilação em linux (gcc) e windows (visual C++)         */
/*                                                                          */
/* Ultima modificacao:    18/02/2004                                        */
/*                                                                          */
/* Modificado por:        Nancy Mieko Abe e Roberto Yuji Tanaka             */
/*                                                                          */
/* Modificacao:	                                                            */
/*                                                                          */
/* 1. Parametro da sobrecarga da funcao solve que  recebe o tipo de         */
/*    pré-condicionador:                                                    */
/*                        original: char *&                                 */
/*                        alterado: char *                                  */
/*     Obs: Usando a versão original, o gcc acusava que não encontrava      */
/*          uma função com aqueles parâmetros (no matching).                */
/*          A alteração foi realizada  em todos as classes derivadas.       */
/*                                                                          */
/* 2. Sobrecarga da função                                                  */
/*    Ve_Templ<Q,long> solve(M &, Ve_Templ<Q,long> &)                       */
/*                                                                          */
/*    original:problema com compilador gcc (no matching construtor Ve_templ)*/
/*            return solve(A, b, null_preconditioner);                      */
/*                                                                          */
/*	  alterado:                                                         */
/*		    Ve_Templ<Q,long> result;                                */
/*			result = solve(A, b, null_preconditioner);          */
/*			return result;                                      */
/*                                                                          */
/*          A alteração foi realizada  em todos as classes derivadas.       */
/*                                                                          */
/****************************************************************************/

namespace lev_math{

template <class M,class T,class Q>
class CNon_Sta_Ite_Method
{
  private:




    typedef typename number_traits<T>::number_type REALTYPE;

    REALTYPE precisao_user;
    unsigned int itc_user;


  protected:

    REALTYPE precisao;
    Q itc;

	//*NMA*:01/08/2006: para gravar dados de precisão atingida em cada iteração
	bool save_iterations_data;
	std::string file_iterations_data;

    CInc_Fac_Preconditioner<M,T,Q>* get_preconditioner_instance(const std::string &);

    virtual Q validate_matrix(M &) = 0;

  public:
    CNon_Sta_Ite_Method(void);
    virtual ~CNon_Sta_Ite_Method();


    virtual Ve_Templ<T,Q> solve(M &, Ve_Templ<T,Q> &) = 0;

    virtual Ve_Templ<T,Q> solve(M &, Ve_Templ<T,Q> &, const std::string &) = 0;


    Q get_solver_iteractions (void) { return itc; }
    Q get_iteractions (void) { return itc_user; }
    void set_iteractions ( Q n) { itc_user = n; }

    REALTYPE get_solver_precision (void) { return precisao; }
    REALTYPE get_precision (void) { return precisao_user; }
    void set_precision (REALTYPE prec) { precisao_user = prec; }

	void set_save_iterations_data(bool b) {save_iterations_data = b;}
	void set_file_iterations_data(std::string name) {file_iterations_data = name;}


	REALTYPE  Relative_Error(const M &matrix,const Ve_Templ<T,Q> &solution,
            const Ve_Templ<T, Q> &rhs, NORM_TYPE norm);
};

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Nome da Funcao:        CNon_Sta_Ite_Method                               */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    13/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/*            construtor                                                    */
/*                                                                          */
/****************************************************************************/
template <class M ,class T, class Q>
CNon_Sta_Ite_Method< M ,T, Q>::CNon_Sta_Ite_Method(void)
{
	itc_user = 5000;
	precisao_user = 1e-10;
	save_iterations_data = true;
	file_iterations_data = "iteration.dat";
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        ~CNon_Sta_Ite_Method                              */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/*            destrutor                                                     */
/*                                                                          */
/****************************************************************************/
template <class M ,class T, class Q>
CNon_Sta_Ite_Method< M ,T, Q>::~CNon_Sta_Ite_Method()
{
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        get_preconditioner_instance                       */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:  *NMA*                                                        */
/*                                                                          */
/*            Aloca dinamicamente um pré-condicionador                      */
/*                                                                          */
/* Parametros:                                                              */
/*            const char* &pre: tipo do pré-condicionador  desejado         */
/*                                                                          */
/* Retorno:                                                                 */
/*            CInc_Fac_Preconditioner<M>*: ponteiro para o pré-condicionador*/
/*                                                                          */
/****************************************************************************/
template <class M ,class T, class Q>
CInc_Fac_Preconditioner<M,T,Q>* CNon_Sta_Ite_Method<M,T, Q>::get_preconditioner_instance(const std::string &pre)
{
	CInc_Fac_Preconditioner<M,T,Q> * Selected_Preconditioner = NULL;

	if (pre =="CInc_Fac_Preconditioner_Null") {
        Selected_Preconditioner = new CInc_Fac_Preconditioner_Null<M,T,Q>();
	} else if (pre=="CInc_Fac_Preconditioner_IC") {
		Selected_Preconditioner = new CInc_Fac_Preconditioner_IC<M,T,Q>();
	} else if (pre=="CInc_Fac_Preconditioner_ILU") {
		Selected_Preconditioner = new CInc_Fac_Preconditioner_ILU<M,T,Q>();
	}
	return Selected_Preconditioner;
}


template <class M,class T, class Q>
typename number_traits<T>::number_type  CNon_Sta_Ite_Method<M,T,Q>::Relative_Error(const M &matrix,const Ve_Templ<T,Q> &solution,const Ve_Templ<T,Q> &rhs, NORM_TYPE norm)
{
    Ve_Templ<T, Q>   MatXmRhs  = matrix*solution-rhs;
    REALTYPE             mat_norm  = matrix.Norm(norm);
    REALTYPE             sol_norm  = solution.Norma(norm);
    REALTYPE             mat_order = (REALTYPE) matrix.prim.Get_Total_Componentes();

      return MatXmRhs.Norma(norm)/
      (mat_norm*sol_norm* mat_order*utility::MachEps());


}

} //end lev_math namespace
#endif

