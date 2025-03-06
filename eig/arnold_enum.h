#ifndef ARNOLDENUM_H
#define ARNOLDENUM_H


namespace lev_math{

namespace ArnoldEnum
{
	enum  OP_MODE {REGULAR_MODE_I=1,
                SHIFT_INVERT_MODE_I,
                REGULAR_INVERSE_MODE_G,
                SHIFT_INVERT_MODE_G,
                BUCKLING_MODE_G,
                CAYLEY_MODE_G
                };

    //! Utilizado para realizar deslocamentos implicitos
	enum  ISHIFT {
  		ISHIFT_0=0, /*!< The shifts are to be provided by the user via reverse communication.*/
		ISHIFT_1, /*!< exact shifts with respect to the current Hesemberg Matix H. */
		ISHIFT_2 /*!< other choice of internal shift to be defined. */
	};

	enum   BMAT   {
		STANDARD=0, /** Standard Eigenproblem*/
		GENERALIZED	 /** Generilized Eigenproblem */
	};

	enum  WHICH {
		EIG_LARG_ALG=0, /*!< eigenvalues with largest algebraic value. symmetric problems.*/
		EIG_SMALL_ALG,  /*!< eigenvalues with smallest  algebraic value. symmetric problems.*/
		EIG_LARG_MAG,	/*!< eigenvalues with largest  value. symmetric or real nonsymmetric or complex  problems */
		EIG_SMALL_MAG,  /*!< eigenvalues with smallest   value. symmetric or real nonsymmetric or complex  problems */
		EIG_LARG_REAL,	/*!< eigenvalues with largest real part.  real nonsymmetric or complex  problems */
		EIG_SMALL_REAL, /*!< eigenvalues with smallest real part.  real nonsymmetric or complex  problems	*/
		EIG_LARG_IMG,   /*!< eigenvalues with largest imaginary part.  real nonsymmetric or complex  problems */
		EIG_SMALL_IMG,	/*!< eigenvalues with largest imaginary part.  real nonsymmetric or complex  problems */
		EIG_BOTH_END 	/*!< eigenvalues from both ends of spectrum. symmetric problems */
	};

	/*!
		Especfica se o usuário  tem interesse em todos auto vetores ou somente alguns
	 */
	enum HOWMNY {
		ALL_RITZ_VEC=0, /*!< Todos os autovetores são calculados */
		SOME_RITZ_VEC  /*!< Somente Alguns autovetores são calculados*/
	};
}


} //end namespace lev_math


#endif  // ARNOLDENUM_H
