#ifndef __REGION__
#define __REGION__

/* =============================================================================

	File:  region.hpp

   Description : classes que definem o comportamento das regi�es do problema no
               que diz respeito a transforma��es de coordenadas.

   written by: Angelo Passaro

   written at: august / 1997

   Modified by:

   Modified at:


   Included Classes :
      Region   : abstract class

      Cylindric_Region : represents the four regions defined by coordinate
                           transformations from an open boundary to a
                           cylindrical domain.

 The domain limits are supposed to be presented in the following sequence:

                       Zext, Zint, Rext, Rint.

 The knowing of the class internal data structure breaks the encapsulation
 concept. However, this classes set have to be seen as a class representation
 of an association relationship between the finite elements and the phenomenum
 classes.

      Parallelepipedal_Region : represents the eighteen regions defined by
                                coordinate transformations from an open boundary
                                to a parallelepipedal domain.

============================================================================= */


#include "point.hpp"
#include "fin_elem.hpp"
#include "lo_templ.hpp"
#include "matrix.hpp"
#include <complex>
#include <memory>


namespace _region {


class Region
{
	protected:

		//=========================================
		// domain limits. They are supposed to be
		// presented in the following sequence:
		//        Zext, Zint, Rext, Rint
		//=========================================
		static double   *limits;
		static short int n_limits;   // number of components of limits
		static short int numberof;   // total number of defined objects
		short int        WhoAmI;

	public :

		lev_math::Lo_Templ<Nodal_Element *, long> *ptr_lo_elements;

		Region()
		{
			limits = NULL;
			ptr_lo_elements = NULL;
		};
		virtual ~Region() { }

		// returns the error message for the err number
		char * Error (short int err, char * mess = NULL);

		void Associate (lev_math::Lo_Templ<Nodal_Element *, long> *ptr_lo) { ptr_lo_elements = ptr_lo; }

		// returns err number, input point, output Jacobian matrix
		virtual short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat)=0;

		// returns err number, input point, output Jacobian matrix and jacobian
		virtual short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jac)=0;

		// returns jacobian, input point
		virtual double     Jacobian    (Point &point)=0;

		// returns err number, input point, output Jacobian
		virtual short int  Jacobian    (Point &point, double *jac);

		static void        Set_Limits  (double *lim) { memcpy(limits, lim, sizeof(double) * n_limits); };

		static short int   Total_Regions(void) {return numberof;}
};


class Cylindric_Region : public Region
{
   public :

		Cylindric_Region();
		~Cylindric_Region();
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat);
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jac);
		double     Jacobian    (Point &point);
		short int  Jacobian    (Point &point, double *jac);
};


//================== a synonimon for Cylindric_Region ==========================
typedef Cylindric_Region  Cylindrical_Region;
//==============================================================================


class Parallelepipedal_Region : public Region
{
	public :
		Parallelepipedal_Region ();
		~Parallelepipedal_Region ();
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat);
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jac);
		double     Jacobian    (Point &point);
		short int  Jacobian    (Point &point, double *jac);
};


class Ordinary2D_Region : public Region
{
   public :
		Ordinary2D_Region();
		~Ordinary2D_Region();
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat);
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jac);
		double     Jacobian    (Point &point);
		short int  Jacobian    (Point &point, double *jac);
	    bool Is_Inside(Point & point);
};


/***********************************************************
 Classe Oridinary_Region

  *AP* 12/10/2007 Criada para tratar regioes ordin�rias para qualquer tipo de problema (1D,2D etc)

  Problema: A classe dimensiona a matriz de transforma��o em fun��o do primeiro elemento
            presente na lista de elementos finitos. Se o problema envolver elementos
			de diferentes dimens�es ela falhar� !
			Neste caso, uma implementa��o mais complexa seria necess�ria, para identificar
			a dimens�o do problema, e n�o do elemento. Talvez fosse o caso de solicitar
			a dimens�o do ponto ao inv�s do elemento.
*/ //***********************************************************
class  Ordinary_Region : public Region
{
	public :
		Ordinary_Region();
		~Ordinary_Region();
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat);
		short int  Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jac);
		double     Jacobian    (Point &point);
		short int  Jacobian    (Point &point, double *jac);
};


} // end namespace _region


#endif // __REGION__
