#ifndef EIGSYMMDOUBLE_H
#define EIGSYMMDOUBLE_H


#include <arnoldfunction.h>
#include <arn_eig_base.h>


//==========================================================================
//  \class  EigValSym
//  \brief  Calculo de autovalores/autovetores em problemas do tipo
//          \f$ A*x=\lambda*x\f$ e \f$ A\f$ simetrica.
//
//          EigValSym permite o calculo de autovalores e autovetores para
//          matrizes simetricas e nao-generalizada. Utiliza a tecnica de
//          Arnoldi para busca espectral de autovalores.
//
// @author Onofre Felix
//==========================================================================


namespace lev_math {


template <typename T, typename Q >
class EigValSym : public  ArnEigBase<T,Q>
{

public:

	 typedef typename number_traits<T>::number_type REALTYPE;

    /**
     *
     * @param Amatrix Matriz Esparsa Matrix.
     * @param neigval Número de Autovalores.
     * @param ncval Número de vetores da base de Lanczos.
     * @param ArnoldEnum::OP_MODE Natureza do Problema não-generalizado.
     * @param ArnoldEnum::WHICH Tipo de autovalores a serem calculados.
     * @param precisao Precisão usada no Cálculo de busca dos autovalores.
     * @param information Se inoformation=0 um vetor de resíduo deve ser fornecido.
     * @param iscaleigvec Se há necessidade de calcular os autovetores.
     *
     */
	 EigValSym(Q matrix_dimension , Q neigval, ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH w);

    /**
     *
     * @param Amatrix Matriz Esparsa Matrix.
     * @param neigval Número de Autovalores.
     * @param ncval Número de vetores da base de Lanczos.
     * @param ArnoldEnum::OP_MODE Natureza do Problema não-generalizado.
     * @param ArnoldEnum::WHICH Tipo de autovalores a serem calculados.
     * @param precisao Precisão usada no Cálculo de busca dos autovalores.
     * @param information Se inoformation=0 um vetor de resíduo deve ser fornecido.
     * @param iscaleigvec Se há necessidade de calcular os autovetores.
     *
     */
	 EigValSym(Q matrix_dimension,Q neigval,T sgm, ArnoldEnum::OP_MODE opmode,ArnoldEnum::WHICH w, REALTYPE precisao, Q information=0, bool iscaleigvec=true);

	 /**
	  * Destrutor
	  *
	  */
	 virtual ~EigValSym();

	 /**
	  * Calcula os  autovalores
	  * @return Retorna um inteiro contendo uma informação específica sobre o processo de  cálculo.
	  */

	 /**
	  * Cálculo de auto valores usando modo regular. Neste processo estão envolvidos cálculos de produtos de Matriz por vetor
	  * 	       *
	  * @param MATRIX parâmetro templete, deve ser a matriz que envolvida no cálculo de autovalores.
	  *
	  * @return Retorna um inteiro contendo informações sobre o processo de
	  * cálculo.
	  */
	 template <typename MATRIX>
	 int eigenSolver(const MATRIX &mat);


	 /**
	  * Calculo dos autovalores por busca espectral para problemas nao generalizados.
	  * Utiliza um solucionador de resolucao de siteamas de equacoes Lineares,
	  * seja ele direto ou iterativo.
	  *
	  * @param MATRIX parametro templete, deve ser a matriz que envolvida no calculo de autovalores.
	  * @param SOLVE  solucionador de resolucao de sistemas Lineares.
	  *
	  * @return Retorna um inteiro contendo informacoes sobre o processo de calculo.
	  *
	  */
	 template <typename MATRIX , typename SOLVE>
	 int eigenSolver(const MATRIX &,const SOLVE &,const std::string &);

	 /**
	  * Cálculo dos autovalores por busca espectral para problemas generalizados.
	  * Utiliza um solucionador de resolucao de sistemas de equacoes Lineares,
	  * seja ele direto ou iterativo.
	  *
	  * @param MATRIX parâmetro templete, matriz que envolvida no cálculo de autovalores.
	  * @param MATRIX parâmetro templete, segunda  matriz que envolvida no cálculo de autovalores.
	  * @param SOLVE  solucionador de resolução de sistemas Lineares.
	  *
	  * @return Retorna um inteiro contendo informações sobre o processo de cálculo.
	  *
	  */
	 template <typename MATRIX , typename SOLVE>
	 int eigenSolver(const MATRIX &, const MATRIX&, const SOLVE &, const std::string & = NULL);

	 virtual void setNCV(Q num_ncv=0);

 protected:

     /**
      * Alocação dinâmica dos dados membros que são vetores.
      */
     virtual void  Alocate_Vectors();
     virtual void  Dealocate();

     std::vector<REALTYPE> workl; /*!< Vetor de Trabalho*/

     template <typename MATRIX>
     int StandardMode(const MATRIX &mat);

     template <typename MATRIX, typename SOLVE>
     int ShiftInvertMode(const MATRIX &mat ,const SOLVE &solver, const std::string &pre= NULL);

     template <typename MATRIX, typename SOLVE>
     int ShiftInvertModeGeneral(const MATRIX &Amat ,const MATRIX &Bmat , const SOLVE &sol, const std::string &pre = NULL);

     template <typename MATRIX, typename SOLVE>
     int BuclingModeGeneral(const MATRIX &mat ,const SOLVE &sol ,const std::string &pre = NULL);
     template <typename MATRIX, typename SOLVE>
     int CaleyModeGeneral(const MATRIX &mat ,const SOLVE &sol,const std::string &pre = NULL);

     friend  void arpack_aupd(int &ido,const char *bbmat, long n,const char* wwhich, long nev,
    		 double &tol, double Resid[], long ncv, double V[],
    		 long ldv, int iparam[], int ipntr[], double Workd[],
    		 double Workl[], long lworkl, int& info,double flag);

     friend void  arpack_eupd(bool rvec, const char *hHowMny,  double D[], double Z[],
    		 long ldz, double sigma, const char *bbmat, long n,
    		 const char* wwhich, long nev, double tol, double Resid[],
    		 long ncv, double V[], long ldv, int iparam[],
    		 int ipntr[], double Workd[], double Workl[],
    		 long lworkl, int &info,double flag );

     friend void arpack_aupd(int &ido,const char *bbmat, long n,const char* wwhich, long nev,
    		 float &tol, float Resid[], long ncv, float V[],
    		 long ldv, int iparam[], int ipntr[], float Workd[],
    		 float Workl[], long lworkl, int& info, float flag);

     friend void  arpack_eupd(bool &rvec, const char *hHowMny,  float D[], float Z[],
    		 long ldz, float sigma, const char *bbmat, long n,
    		 const char* wwhich, long nev, float tol, float Resid[],
    		 long ncv, float V[], long ldv, int iparam[],
    		 int ipntr[], float Workd[], float Workl[],
    		 long lworkl, int &info,float flag);
};


template < typename T, typename Q>
EigValSym <T,Q>::EigValSym(Q matrix_dimension , Q neigval, ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH w):
	 ArnEigBase<T ,Q>( matrix_dimension , neigval, opmode, w)
{
	// TODO (onofre#1#): servico para definir o tamanho o valor de lworkl
	setNCV();
}


template < typename T,typename Q>
EigValSym <T,Q>::EigValSym(Q matrix_dimension,Q neigval,T sgm, ArnoldEnum::OP_MODE opmode,
		ArnoldEnum::WHICH w, REALTYPE precisao, Q information, bool iscaleigvec)
		: ArnEigBase< T,Q>(matrix_dimension, neigval, opmode, w,sgm, precisao, information, iscaleigvec)
{
	this->lworkl = this->ncv*(this->ncv+8);
	Alocate_Vectors();
	this->sigma = sgm;
}


template < typename T,typename Q>
EigValSym <T,Q>::~EigValSym()
{
	Dealocate();
}


template < typename T,typename Q>
void EigValSym <T,Q>:: setNCV(Q num_ncv)
{
	if (num_ncv <= this->nev) {
		this->ncv = num_ncv;
		ArnEigBase <T,Q>::setNCV(num_ncv);
		this->lworkl = this->ncv*(this->ncv+8);
		Alocate_Vectors();
		ArnEigBase<T,Q>::Alocate_Vectors();
	}
}


template < typename T,typename Q>
void EigValSym <T,Q>::Dealocate()
{
	workl.clear();
}


template < typename T,typename Q>
void EigValSym <T,Q>::Alocate_Vectors()
{
	workl.resize(this->lworkl+1);
}


template <typename T,typename Q>
template <typename MATRIX>
int EigValSym <T,Q>::eigenSolver(const MATRIX &mat)
{
	int output;
	output = StandardMode(mat);
	return output;
}


template < typename T,typename Q>
template <typename MATRIX,typename SOLVE>
int EigValSym <T,Q>::eigenSolver(const MATRIX &mat, const SOLVE &sol, const std::string &pre)
{
	int output;
	output = ShiftInvertMode(mat,sol,pre);
	return output;
}


template < typename T,typename Q>
template <typename MATRIX, typename SOLVE>
int EigValSym <T,Q>::eigenSolver(const MATRIX &mat1,const MATRIX &mat2 ,const SOLVE &sol,const std::string &pre)
{
	int output;
	output = ShiftInvertModeGeneral(mat1,mat2,sol,pre);
	return output;
}


template < typename T,typename Q>
template <typename MATRIX>
int EigValSym <T,Q>::StandardMode(const MATRIX &mat)
{
	 Ve_Templ<double,Q> res,rhs;
	 while (this->ido != 99) {

		 arpack_aupd(this->ido, this->GetBmat().c_str(), this->ndim , this->GetWhich().c_str(), this->nev,
				 this->tol, &this->resid[0], this->ncv, &this->vecbasis[0], this->ndim, this->iparam, this->ipntr,
				 &this->workd[0], &this->workl[0], this->lworkl, this->info_aupd, T(0));

		 if (this->ido == -1 || this->ido == 1) {
			 rhs.Set_Data(&this->workd[this->ipntr[1]],this->ndim);
			 res = mat*rhs;
			 res.Get_Data(&this->workd[this->ipntr[2]],this->ndim);
		 }
	 }

	 arpack_eupd(this->irvec, this->GetHowMny().c_str(), &this->dritz_val[0], 0,
			 this->ndim, this->sigma,  this->GetBmat().c_str(),  this->ndim,
			 this->GetWhich().c_str(), this->nev,  this->tol, &this->resid[0],
			 this->ncv, &this->vecbasis[0], this->ndim, this->iparam,
			 this->ipntr, &this->workd[0], &this->workl[0],
			 this->lworkl, this->info_eupd,T(0));
	 return this->info_eupd;
}


template < typename T,typename Q>
template <typename MATRIX, typename SOLVE>
int EigValSym <T,Q>::ShiftInvertMode(const MATRIX &mat ,const SOLVE &solver, const std::string &pre)
{
	Ve_Templ<T,Q> rhs,res,vetmp;
	MATRIX Amat = mat;

	Amat.DiagAddVal(-this->sigma);

	while (this->ido != 99) {

		arpack_aupd(this->ido, this->GetBmat().c_str(), this->ndim , this->GetWhich().c_str(), this->nev,
				this->tol, &this->resid[0], this->ncv, &this->vecbasis[0], this->ndim, this->iparam, this->ipntr,
				&this->workd[0], &this->workl[0], this->lworkl, this->info_aupd, T(0));

		if (this->ido == -1 || this->ido == 1) {
			rhs.Set_Data(&this->workd[this->ipntr[1]],this->ndim);
			res=solver->solve(Amat,rhs,pre);
			res.Get_Data(&this->workd[this->ipntr[2]],this->ndim);
		}
 	}
 	if (!this->info_aupd) {

 		arpack_eupd(this->irvec, this->GetHowMny().c_str(), &this->dritz_val[0], 0,
 				this->ndim, this->sigma,  this->GetBmat().c_str(),  this->ndim,
 				this->GetWhich().c_str(), this->nev,  this->tol, &this->resid[0],
 				this->ncv, &this->vecbasis[0], this->ndim, this->iparam,
 				this->ipntr, &this->workd[0], &this->workl[0],
 				this->lworkl, this->info_eupd,T(0));
 	}
	return this->info_eupd;
}


template < typename T,typename Q>
template <typename MATRIX, typename SOLVE>
int EigValSym <T,Q>::ShiftInvertModeGeneral(const MATRIX &Amat ,const MATRIX &Bmat , const SOLVE &sol, const std::string &pre)
{
	Ve_Templ<T,Q> rhs,res,vetmp;
    MATRIX AMat_SigBMat,AMatrix;

    AMat_SigBMat = const_cast<MATRIX &>(Bmat);
    AMat_SigBMat = this->sigma*AMat_SigBMat;
    AMatrix = const_cast<MATRIX&>(Amat);
    AMat_SigBMat = AMatrix-AMat_SigBMat;

	while (this->ido != 99) {

		arpack_aupd(this->ido, this->GetBmat().c_str(), this->ndim , this->GetWhich().c_str(), this->nev,
				this->tol, &this->resid[0], this->ncv, &this->vecbasis[0], this->ndim, this->iparam, this->ipntr,
				&this->workd[0], &this->workl[0], this->lworkl, this->info_aupd, T(0));

		if (this->ido == -1) {
			rhs.Set_Data(&this->workd[this->ipntr[1]],this->ndim);
			rhs = Bmat*rhs;
			res=sol->solve(AMat_SigBMat,rhs,pre);
			res.Get_Data(&this->workd[this->ipntr[2]],this->ndim);
		} else if (this->ido == 1) {
			rhs.Set_Data(&this->workd[this->ipntr[3]],this->ndim);
			res=sol->solve(AMat_SigBMat,rhs,pre);
			res.Get_Data(&this->workd[this->ipntr[2]],this->ndim);
		} else if (this->ido == 2) {
			rhs.Set_Data( &this->workd[this->ipntr[1]],this->ndim);
			/**
			*           %-----------------------------------------%
			*           |         Perform  y <--- B*x.            |
			*           | Need the matrix vector multiplication   |
			*           | routine here that takes workd(ipntr(1)) |
			*           | as the input and returns the result to  |
			*           | workd(ipntr(2)).                        |
			*           %-----------------------------------------%
			*/
		    res = Bmat*rhs;
            res.Get_Data(&this->workd[this->ipntr[2]],this->ndim);
		}
	}
 	if (!this->info_aupd) {

 		arpack_eupd(this->irvec, this->GetHowMny().c_str(), &this->dritz_val[0], 0,
 				this->ndim, this->sigma,  this->GetBmat().c_str(),  this->ndim,
 				this->GetWhich().c_str(), this->nev,  this->tol, &this->resid[0],
 				this->ncv, &this->vecbasis[0], this->ndim, this->iparam,
 				this->ipntr, &this->workd[0], &this->workl[0],
 				this->lworkl, this->info_eupd,T(0));
 	}
	return this->info_eupd;
}


} // End namespace lev_math


#endif
