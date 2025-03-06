#ifndef ARN_EIG_BASE_H
#define ARN_EIG_BASE_H


#include <iostream>
#include <string>
#include <cassert>
#include <arnoldenum.h>
#include <ve_templ.hpp>
#include <utilclass.h>


namespace lev_math {


template <class T,class Q >
class ArnEigBase {

public:

	typedef typename number_traits<T>::number_type REALTYPE;

    ArnEigBase();
    ArnEigBase( ArnEigBase &);
    ArnEigBase(Q nordem, Q neigval,ArnoldEnum::OP_MODE opmode,
                ArnoldEnum::WHICH which);
    ArnEigBase(Q nordem, Q neigval,ArnoldEnum::OP_MODE opmode,
                ArnoldEnum::WHICH which, T sgm);

    /**
     * Construtor
     * @param nordem  Dimensão da Matrix
     * @param neigval Quantidade de Autovalores desejados  \f$ neigval \leq ncval +1 \f$ .
     * @param ncval   Comprimento da Fatorização de Arnoldi: Número máximo de vetores de Arnoldi usado no processo de cálculo  \f$ ncv \geq nev+1 \f$.
     * @param opmode  Metodo de Resolusão empregado na solução do problema
     * @param which Quais são os autovalores de interesse.
     * @param sgm  Deslocamento usado quando uma busca espectral for empregada.
     * @param precisao Critéririo de de convergência. Valor padrão é 0.0;
     * @param information Se information=0, então o vetor inicial será gerado aleatoriamente, caso contrário  um vetor de residuo inicial deverá ser fornecido.
     * @param iscaleigvec parâmetro lógico. Se Verdadeiro Cálcula os Autovetores.
     * @return
     */
    ArnEigBase(Q nordem, Q neigval,ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH which,
                    T sgm, T precisao, int information, bool iscaleigvec);


    /**
     * Retorna um strig com a informação  do tipo de Autovalor desejado.
     * Ex: Autovalores de Maior ou Menor Valor absoluto, Autovalores extremos e etc.
     * @return
     */
    std::string GetWhich() const;

    /**
     * Retorna a natureza do problema.
     * pode ser:
     * Generalizado \f$ A*x=\lambda*B*x \f$
     * Não-Generalizado \f$ A*x=\lambda*x \f$
     * @return
     */
    std::string GetBmat() const;

    /**
     * Dependendo da natureza do problema alguns dados membros são iniciados de forma
     * atuomática, ou seja sem intervenção do usuário.
     */
    void SetOpMode();

    /**
     * Retorna um indice especifico do vetor dado membro iparam.
     * @param indice desejado.
     * @return
     */
    int GetIparam(int indice) const;

    /**
     * Retorna um  indice especifico  do dado membro ipntr.
     * @param indice desejado.
     * @return indice do Iparam desejado.
     */
    int GetIpntr(int indice) const;

    virtual ~ArnEigBase();

    /**
     * Retorna os todos auto valores
     * @return Retorna um Ve_Templ com os autovalores calculados.
     */
    Ve_Templ<T,Q>  GetEigVal()const ;

    /**
     * Retorna os autovetores calculados.
     * @param nevl indice do autovalor associado.
     * @return  Retorna um Ve_Templ de comprimento  \f$ ndim*nev \f$  contendo todos autovetores.
     */
    Ve_Templ<T,Q>  GetEigVec(Q nevl=0)const;

    Ve_Templ<T,Q>  GetAllEigVec()  const;

    /**
     * Cálculo do erro associado para problemas não-generalizados.
     * Para o cálculo do erro foi usada a  seguinte métrica  \f$ erro = \Vert A*x -\lambda*x \Vert_{\infty} \f$.
     * @param Amat Matrix Original.
     * @return Retorna um Ve_Templ contendo as normas associada
     *  a cada conjunto autovalor/autovetor.
     */
    template <typename MATRIX>
    Ve_Templ<T,Q>  GetCheck(const MATRIX &Amat);

    /**
     * Cálculo do erro associado para problemas não-generalizados.
     * Para o cálculo do erro foi usada a  seguinte métrica  \f$ erro = \Vert A*x -\lambda*B*x \Vert_{\infty} \f$.
     * @param Amat Sparse_Matrix.
     * @param Bmat Sparse_Matrix.
     * @return
     */
    template <typename MATRIX>
    Ve_Templ<T,Q>  GetCheck(const MATRIX &Amat,const MATRIX &Bmat);

    void SetOPHowmny(ArnoldEnum::HOWMNY op_hmowmny_value = ArnoldEnum::ALL_RITZ_VEC )
    {
    	op_howmny = op_hmowmny_value;
    	SetHowMny();
    }

    /**
     * \brief  Return error code from lanczos Base calculation from arpack.
     *  if info = 0 operation was well done.
     */

    int  GetInfoAupd(){ return info_aupd;}

    /**
     * \brief  Return error code from eigvalues and eigvectors operation from Arpack.
     *    if info = 0 operation was well done.
     */
    int  GetInfoEupd(){ return info_eupd;}

    void setSigma(T s){ sigma = s;}

    /**
     * \brief  Incial configuration for iparam vector.
     *  @param  val  values to be inserted in vector val.
     *  @param  indices where val values will be inserted.
     */

    T getSigma()const { return sigma; }

    void SetIparam(int *val,int *indices);

    virtual void setNCV(Q _ncv) ;

    void setArnoldInter(int num_inter) {iparam[3] = num_inter;}
   
    void setInfoAupd(int infoAupd);
    void setInfoEupd(int infoEupd) { info_eupd = infoEupd;}
   
    void setNEV(Q _nev);
    void setIDO(Q _ido= 0) ;

    /**
     * \brief Inicia dados Membros.
     * @param neigval Número de autovalores
     * @param ncval   Número de vetores de Base de Lanczos
     * @param precisao Precisão do cálculo
     * @param information Usuário deve forncer o vetor de resíduo se information \f$ \neq \f$ 0
     * @param iscaleigvec  Se Verdeiro Calcula  todos autovetores.
     */
    void SetValues(Q neigval,  T precisao=0.0, int information=0, bool iscaleigvec=true);

protected:

    /**
     * Alocação de Vetores necessários no cálculo do problema.
     * É virtual, pois o tamanho dos vetores podem variar de acordo
     * com a natureza do problema
     * Ex.: double, complex e etc.
     */
    virtual void Alocate_Vectors();
	
	/**
     * Destroy todos os vetores alocados
     * Ex.: double, complex e etc.
     */
	void clearAll_Vectors();

    /**
     * Desaloca os  Vetores que foram alocados para o cálculo do problema.
     */
    virtual void Dealocate() = 0;

    /** op_mode indica Modo de operação ver include \file arnoldenum.h */
    ArnoldEnum::OP_MODE op_mode;

    /** op_bmat identifica a natureza do problema não-generalizado  ou generalizado.
     *  Consulte ArnoldEnum::BMAT ou o manual da arpack.
     */
    ArnoldEnum::BMAT op_bmat;

    /**
     *   op_which Seleciona no Vetor de String which qual é o tipo de de autovalor de interesse.
     * O manual da arpack ou ArnoldEnum::WHICH devem ser consultados para escolha correta
     * do parâmetro.
     */
    ArnoldEnum::WHICH op_which;

    /*!
      iparam array of lenght 11. \n
      (input, output)

      <TABLE>
	<TR>
		<TD>iparam[1] </TD><TD> The shifting strategy used during the implicity restarted portion of an IRLM. <TD>
	</TR>
	<TR>
		<TD> iparam[3]</TD><TD>Maximum number of IRLM iterations allowed.</TD>
	</TR>
	<TR>
		<TD>iparam[7]</TD><TD> Indicates the algorithmic mode used with ARPACK.</TD>

	</TR>

     </TABLE>
     */
    int iparam[12];

    /*!
      ipntr array of lenght 14. (output)
      <TABLE>
      	<TR>
		<TD> ipntr[01] </TD> <TD> pointer to the current operand vector X in workd.</TD>
	</TR>
	<TR>
		<TD> ipntr[02] </TD><TD> pointer to the current result vector Y in workd.</TD>
     </TR>
     <TR>
		<TD> ipntr[03] </TD> <TD> pointer to the vector B*X in workd when used in the shif-and-invert mode.</TD>
     </TR>
     <TR>
		<TD>  ipntr[04]</TD> <TD> pointer to the next available location in workl that is untouched by the program.</TD>
     </TR>
     <TR>
		<TD> ipntr[05]</TD> <TD> pointer to the ncv by ncv upper Hesenberg matrix H in workl.</TD>
     </TR>
     <TR>
		<TD>  ipntr[06] </TD> <TD> pointer to the ritz value array RITZ. </TD>
     </TR>
     <TR>
		<TD> ipntr[07]</TD> <TD> pointer to the (projected) ritz
      vector array Q.</TD>
      </TR>
      <TR>
		<TD> ipntr[08] </TD><TD> pointer to the error BOUNDS array in workl. </TD>
     </TR>
     <TR>
		<TD> ipntr[09] </TD><TD> pointer to the ncv ritz values of the original
      system. </TD>
     </TR>
     <TR>
		<TD> ipntr[10]</TD> <TD> Not used. </TD>
     </TR>
     <TR>
		<TD>  ipntr[11] </TD><TD> pointer to the ncv corresponding error bounds.</TD>
     </TR>
     <TR>
		<TD>  ipntr[12] </TD><TD> pointer to the np shifts in workl.</TD>
     </TR>
     */
    int ipntr[15];


    /*! \brief  Número de autovalores desejados. */
    Q nev;

    /*!
       \brief Número de vetores de base de  Lanczos.
         Esses vectors são usados no cáculo computacional, deve ser no mínimo
	  \link ::nev \f$ ncv = nev+1 \f$ \endlink.
     */
    Q ncv;

    /**
     * lworkl -> The size of the work array workl.
     * \li No mínimo \f$ 3*ncv*(ncv+6) \f$  para  problemas reais e não simétrico.
     * \li No mínim  \f$ 3*ncv*(ncv+5) \f$  para problemas complexos.
     */
    Q lworkl;

    /*!
      \brief  Comprimento do vetor de trabalho.
      \li No mínimo \f$ 3*ncv*(ncv+6)\f$  para  problemas reais e não simétrico.
      \li No mínimo  \f$ 3*ncv*(ncv+5) \f$ para problemas complexos.
     */
    // T *workl;

    /*!
     \brief  Critério de convergência.
       Se o valor padrão for  zero, então será usada a precisão interna da máquina eps. \n
       Ele pode ser alterado dependendo da precisão desejada. Normalmente quanto menor o valor maior será o custo computacional para satisfazer o critério de parada. Se o valor ::tol for muito grande e os autovalores estiverem aninhados eles podem não ser encontrados.
     */
    T tol;

    /*!  \brief  Dimensão do problema. */
    Q ndim;

    /*! \brief  "flag" de comunicação reversa.
       Deve ser zero  na primeira chamada de Aupd() função  de resolução dos autovalores.
     */
    int ido;



    /**
     * info  (input,output) Set to zero for a randomly generate starting vector.
     * If the user decides to use another starting vector, this value should be set to 1,
     * and the starting vector shoud be provided in the array resid.
     * Error flag on output

     <TABLE>
 	<TR>
     		<TD> 0 </TD>  <TD>normal exit. </TD>
     	</TR>
 	<TR>
     		<TD> 1 </TD>  <TD>Maximum number of iterations taken. iparam(5) -> return the number of wanted converged Ritz values.</TD>
     	</TR>
     	<TR>
     		<TD> 2 </TD> <TD>No Qer an informational error. </TD>
     	<TR>
     		<TD>3  <TD>No shifts could be applied during a cycle of the implicity restarted Arnoldi  iteration. </TD>
     	</TR>
     	<TR>
     		<TD>-1 </TD><TD> ndim must be positive. </TD>
     	</TR>
     	<TR>
     		<TD>-2</TD> <TD> nev must be positive. </TD>
     	</TR>
     	<TR>
     		<TD>-3</TD> <TD> ncv -nev < 2 and less than or equal to ndim.</TD>
     	</TR>
     	<TR>
     		<TD> -4</TD> <TD> The maximum number of Arnoldi update interation must be greater then zero.</TD>
     	</TR>
     	<TR>
     		<TD> -5 </TD><TD> wich must be one of LM,SM,LR,SR,LI,SI </TD>
     	</TR>
     	<TR>
     		<TD>-6</TD> <TD> bmat must be one of I or G</TD>
     	</TR>
     	<TR>
     		<TD> -7 </TD><TD> length of private work array is not sufficient. </TD>
     	</TR>
     	<TR>
     		<TD>-8</TD> <TD> Error return from lapack eigenvalue calculation.</TD>
     	</TR>
     	<TR>
     		<TD>-9 </TD><TD> starting vector is zero. </TD>
     	</TR>
     	<TR>
     		<TD>-10 </TD><TD> iparam(7) must be 1,2,3.</TD>
     	</TR>
     	<TR>
     		<TD>-11</TD> <TD> iparam(7)=1 and bmat = 'G'.</TD>
     	</TR>
     	<TR>
     		<TD> -12</TD> <TD> iparam(1) must be equal to 0 or 1.</TD>
     	</TR>
     	<TR>
     		<TD>-9999</TD> <TD> Could not build an Arnoldi factorization. ipram(5) returns the size of
      the currente arnoldi.</TD>
     	</TR>
     </TABLE>
     */
     int info_aupd;
	 
	 
     /// Comportamento similar a info_aupd.


     /**
     * info  (output) Error flag on output
     *
     *
     *
     *=  1: Maximum number of iterations taken.
                All possible eigenvalues of OP has been found. IPARAM(5)
                returns the number of wanted converged Ritz values.
          =  2: No longer an informational error. Deprecated starting
                with release 2 of ARPACK.
          =  3: No shifts could be applied during a cycle of the
                Implicitly restarted Arnoldi iteration. One possibility
                is to increase the size of NCV relative to NEV.
                See remark 4 below.

     <TABLE>
 	<TR>
     		<TD> 0 </TD>  <TD>normal exit. </TD>
     	</TR>
 	<TR>
        	<TD>-1 </TD><TD> ndim must be positive. </TD>
     	</TR>
     	<TR>
     		<TD>-2</TD> <TD> nev must be positive. </TD>
     	</TR>
     	<TR>
     		<TD>-3</TD> <TD> ncv must be greater than nev and less than or equal to ndim.</TD>
     	</TR>
     	<TR>
     		<TD> -5 </TD><TD> wich must be one of LM,SM,LR,SR,LI,SI </TD>
     	</TR>
     	<TR>
     		<TD>-6</TD> <TD> bmat must be one of I or G</TD>
     	</TR>
     	<TR>
     		<TD> -7 </TD><TD> length of private workl array is not sufficient. </TD>
     	</TR>
     	<TR>
     		<TD>-8</TD> <TD> Error return from lapack eigenvalue calculation.</TD>
     	</TR>
     	<TR>
     		<TD>-9 </TD><TD> starting vector is zero. </TD>
     	</TR>
     	<TR>
     		<TD>-10 </TD><TD> iparam(7) must be 1,2,3.</TD>
     	</TR>
     	<TR>
     		<TD>-11</TD> <TD> iparam(7)=1 and bmat = 'G'.</TD>
     	</TR>
     	<TR>
     		<TD> -12</TD> <TD>  NEV and WHICH = 'BE' are incompatible.</TD>
     	</TR>
     	<TR>
     	<TR>
     		<TD> -14</TD> <TD>   DSAUPD did not find any eigenvalues to sufficient accuracy.</TD>
     	</TR>
     	<TR>
        <TR>
     		<TD> -15</TD> <TD>   HOWMNY must be one of 'A' or 'S' if RVEC = .true.</TD>
     	</TR>
     	<TR>

     	<TR>
     		<TD> -16</TD> <TD>   HOWMNY = 'S' not yet implemented.</TD>
     	</TR>
     	<TR>


     		<TD>-9999</TD> <TD> Could not build an Arnoldi factorization. ipram(5) returns the size of
      the currente arnoldi.</TD>
     	</TR>
     </TABLE>
     */


     int info_eupd;

     /** \brief  Idica se os autovetores são necessários. */
     bool irvec;


     /*! \brief  Matriz de dimensão ::ndim X ::ncv.
           Armazena o conjunto de bases de Arnoldi.
      */

     std::vector<T> vecbasis;

     /*!
      \brief  vetor de resíduo.
       (input, output)
      \li  info  =  0  vetor  será preenchido de forma aleatória.
      \li  info !=  0  um vetor de resíduo deve ser fornecido.
      \li  No retorno o vetor contém o resíduo final.
      */
     std::vector<T> resid;

     /*!
      \brief  Vetor de trabalho tem comprimento de \f$ 3*ndim \f$.
      private (replicated) array on each PE or array allocated on for reverse communication.
      O usuário não deve utilizar esse vetor como vetor de trabalho temporário durante o processo de cáculo.
      */
     std::vector<T> workd;

     /**
      * \brief vetor de comprimento  ncv.
      * (Workspace) Private (replicated) array on each PE or array allocated on the front end.
      * only complex problems.
      */
     std::vector<T> rworkd;

     /* Dados menbros relacionados com a chamada da fução arpack dseupd */

     /**
      *  dritz_val   tipo T of dimension NEV.  \n
      *(OUTPUT) On exit, dritz_val contains the Ritz value approximations to the
      * eigenvalues of  \f$  A*z = \lambda*B*z \f$ . \n
      * The values are returned in ascending order.
      * \li IPARAM(7) = 3,4,5 then dritz_val represents the Ritz values of OP computed by dsaupd transformed to those of the original eigensystem \f$ A*z = \lambda*B*z \f$ .
      * \li  IPARAM(7) = 1,2 then the Ritz values of OP are the same as the those of \f$ A*z = \lambda*B*z \f$
      */
     std::vector<T> dritz_val;

     /**
      *	     zritz_val       tipo T  N by NEV array if HOWMNY = 'A'.  (OUTPUT)
      * On exit, Z contains the B-orthonormal Ritz vectors of the eigensystem \f$ A*z = \lambda*B*z \f$ corresponding to the Ritz
      * value approximations. If  RVEC = .FALSE. then Z is not referenced.
      * NOTE: The array Z may be set equal to first NEV columns of the Arnoldi/Lanczos basis array V computed by DSAUPD.
      */

     /*!
       \brief sigma   tipo T   (INPUT)
       \li IPARAM(7) = 3,4,5 deslocamento ::sigma deve ser fornecido.
       \li IPARAM(7) = 1 or 2. não necessita do deslocamento ::sigma.
      */
     T sigma;

     /*!
     	\brief SELECT  Logical array of dimension NCV.
        (INPUT)
		\li  HOWMNY = 'S', SELECT  especifica quais vetores de  Ritz devem ser calculados.
          Para selecionar os vetores de  Ritz vector correspondentes dritz_val(j), SELECT(j) deve ser configurado como  .TRUE..
      	\li HOWMNY = 'A' , SELECT não será referenciado.
      */
     bool *select;

     /*! op_howmny obtem em GetHowMny() os autovalores/autovetores de interesse
      */
     ArnoldEnum::HOWMNY op_howmny;

     std::string GetHowMny() const;

private:

    /**
     * Vetor de String que indica a natureza do problema. \n
     * A natureza da problema pode ser: \n
     *\li não-generalizado \f$ A*x = \lambda*x \f$
     *\li generalizado \f$ A*x = \lambda*M*x \f$.
     */
    std::string bmat;

    /**
     *  Vetor de String que indicarão quais são os autovalores de interesse. \n
     * Apesar de todas possibilidades estarem definidas, deve-se consultar
     * o manual da arpack para selecionar os valores adequados a cada tipo de problema.
     * os tipos podem ser simétricos, não-simetricos e complexos.
     */
    std::string which;

    /**
     * HOWMNY  Character*1  (INPUT)\n
     * Specifies how many Ritz vectors are wanted and the form of Z
     * the matrix of Ritz vectors.\n
     * See remark 1 below. \n
     * HOWMNY = 'A': compute NEV Ritz vectors; \n
     * HOWMNY = 'S': compute some of the Ritz vectors, specified by the logical array SELECT.
     */
    std::string howmny;

    void SetBmat();
    void SetWhich();
    void SetHowMny();
};


template < typename T,typename Q>
ArnEigBase <T,Q>::ArnEigBase()
{
	std::cout << "Construtor padrao ArnEigBase\n";
}


template < typename T,typename Q>
ArnEigBase <T,Q>::ArnEigBase(Q nordem,Q neigval, ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH set_which)
{
	// TODO (onofre#1#): Implementar um servico para configurar o dado membro iparam que eh um vetor
	ndim = nordem;
	SetValues(neigval);
	Alocate_Vectors();
	ido = 0;
	iparam[1] = 1;
	iparam[3] = 300;
	op_which = set_which;
	op_mode = opmode;
	SetOpMode();
	SetBmat();
	SetWhich();
	SetHowMny();
}


template < typename T,typename Q>
ArnEigBase <T,Q>::ArnEigBase(Q nordem, Q neigval,
    ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH set_which, T sgm)
{
	ndim = nordem;
	SetValues(neigval);
	setIDO();
	sigma = sgm;
	iparam[1] = 1;
	iparam[3] = 300;
	op_which = set_which;
	op_mode = opmode;
	SetOpMode();
	SetBmat();
	SetWhich();
	SetHowMny();
}


// TODO: Realizar atualizacao dos demais construtores.
template < typename T,typename Q>
ArnEigBase <T,Q>::ArnEigBase(Q nordem, Q neigval, ArnoldEnum::OP_MODE opmode, ArnoldEnum::WHICH set_which,
   T sgm, T precisao, int information, bool iscaleigvec)
{
	ndim = nordem;
	sigma = sgm;
	op_which = set_which;
	op_mode = opmode;
	SetValues(neigval, precisao, information, iscaleigvec);
	Alocate_Vectors();
}


template < typename T,typename Q>
ArnEigBase <T,Q>::~ArnEigBase()
{ }


template < typename T,typename Q>
void ArnEigBase <T,Q>::clearAll_Vectors() 
{
	workd.clear();
	resid.clear();
	vecbasis.clear();
    dritz_val.clear();
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::Alocate_Vectors()
{
	workd.resize(3 * ndim + 1);
	resid.resize(ndim + 1);
    vecbasis.resize(ndim * ncv + 1);
    dritz_val.resize(nev + 1);
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetValues(Q neigval, T precisao, int information, bool iscaleigvec)
{
	setNEV(neigval);
    tol = precisao;
    info_aupd = information;
    info_eupd = information;
    op_howmny = ArnoldEnum::ALL_RITZ_VEC;
    ido = 0;
   	iparam[1] = 1;
    iparam[3] = 300;
    SetOpMode();
    SetBmat();
    SetWhich();
    SetHowMny();
    irvec = iscaleigvec;
}


template < typename T,typename Q>
std::string ArnEigBase <T,Q>::GetWhich() const
{
	return (which);
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetWhich()
{
	const std::string which_values[]={"LA","SA","LM","SM","LR","SR","LI","SI","BE"};
    which = which_values[op_which];
}


template < typename T,typename Q>
std::string ArnEigBase <T,Q>::GetBmat() const
{
	return bmat;
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetBmat()
{
	const std::string bmat_values[]={"I","G"};
	bmat = bmat_values[op_bmat];
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetIparam(int *value,int *indices)
{
	for (int i = 0; i < sizeof(indices)/sizeof(int);i++)
		iparam[indices[i]+1] = value[i];
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetOpMode()
{
	switch (op_mode) {
		case ArnoldEnum::REGULAR_MODE_I:
			op_bmat = ArnoldEnum::STANDARD;
            iparam[7] = 1;
            break;
        case ArnoldEnum::SHIFT_INVERT_MODE_I:
            op_bmat = ArnoldEnum::STANDARD;
            iparam[7] = 3;
            break;
        case ArnoldEnum::REGULAR_INVERSE_MODE_G:
            op_bmat = ArnoldEnum::GENERALIZED;
            iparam[7] = 2;
            break;
        case ArnoldEnum::SHIFT_INVERT_MODE_G:
            op_bmat = ArnoldEnum::GENERALIZED;
            iparam[7] = 3;
            break;
        default:
            std::cout << "Modo nao Implentado\n";
            exit(0);
    }
}


template < typename T,typename Q>
int ArnEigBase <T,Q>::GetIparam(int indice) const
{
	return (iparam[indice]);
}


template < typename T,typename Q>
int ArnEigBase <T,Q>::GetIpntr(int indice) const
{
	return (ipntr[indice]);
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::SetHowMny()
{
	const std::string howmny_values[]={"A","S"};
	howmny = howmny_values[op_howmny];
}


template < typename T,typename Q>
std::string ArnEigBase <T,Q>::GetHowMny() const
{
	return howmny;
}


template < typename T,typename Q>
Ve_Templ<T,Q>  ArnEigBase <T,Q>::GetEigVal() const
{
	Ve_Templ<T,Q> eigval;
	eigval.Set_Data(&dritz_val[1],nev);
    return eigval;
}


template < typename T,typename Q>
Ve_Templ<T,Q>  ArnEigBase <T,Q>::GetEigVec(Q nevl)const
{
	Ve_Templ<T,Q> eigvec;
	assert(nevl >= 0 && nevl < nev);
	eigvec.Set_Data(&vecbasis[(nevl)*ndim+1],ndim);
	return eigvec;
}


template < typename T,typename Q>
Ve_Templ<T,Q>  ArnEigBase <T,Q>::GetAllEigVec()const
{
	Ve_Templ<T,Q> eigvec;
	eigvec.Set_Data(&vecbasis[1],vecbasis.size()-1);
    return eigvec;
}


template < typename T,typename Q>
template <typename MATRIX>
Ve_Templ<T,Q>  ArnEigBase <T,Q>::GetCheck(const MATRIX &Amat)
{
	Ve_Templ<T,Q> eigvec,calc_norm,rhs;
	calc_norm.Set_N_Max_Com_Abs(nev);
	T dd;

	for (int i=0; i<nev;i++) {
		eigvec.Set_Data(&vecbasis[i*ndim+1],ndim);
		rhs = (Amat)*(eigvec) -dritz_val[i+1]*(eigvec);
		dd=rhs.Norma(NORM_INFINITE);
		calc_norm.Inclui(dd);
		eigvec.Libera();
	}
	return calc_norm;
}


template < typename T,typename Q>
template <typename MATRIX>
Ve_Templ<T,Q>  ArnEigBase <T,Q>::GetCheck(const MATRIX &Amat,const MATRIX &Bmat)
{
	Ve_Templ<T,Q> eigvec,calc_norm,rhs;
	calc_norm.Set_N_Max_Com_Abs(nev);
	T dd;

	for (int i=0; i<nev;i++) {
		eigvec.Set_Data(&vecbasis[i*ndim+1],ndim);
		rhs = (Amat)*(eigvec) -dritz_val[i+1]*(Bmat)*(eigvec);
		dd=rhs.Norma(NORM_INFINITE);
		calc_norm.Inclui(dd);
		eigvec.Libera();
	}
	return calc_norm;
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::setInfoAupd(int infoAupd)
{
	if(infoAupd == 0) {
		clearAll_Vectors();
		Alocate_Vectors();
	}
	info_aupd = infoAupd;
}


template < typename T,typename Q>
void ArnEigBase <T,Q>:: setNEV(Q _nev)
{
	nev = _nev;
	setNCV(nev);
}


template < typename T,typename Q>
void ArnEigBase <T,Q>::  setIDO(Q _ido)
{
	ido = _ido;
	iparam[1] = 1;
	iparam[3] = 300;
	SetOpMode();
	SetBmat();
	SetWhich();
	SetHowMny();
	clearAll_Vectors();
}


template < typename T,typename Q>
void ArnEigBase <T,Q>:: setNCV(Q _ncv)
{
	ncv = (2*nev+1 > ndim-1) ?(ndim-1):(2*nev+1) ;
	Alocate_Vectors();
}


} // end namespace lev_math


#endif //SOLVEEIGENVALUE_H
