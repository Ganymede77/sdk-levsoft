/* =============================================================================

	File:  region.cpp

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

      Parallelepipedal_Region : represents the eighteen regions defined by
                                coordinate transformations from an open boundary
                                to a parallelepipedal domain.

============================================================================= */


#include <cstdlib>
#include <iostream>
#include <region.hpp>


namespace _region {


double   * Region :: limits;    // domain limits
short int  Region :: n_limits;  // number of components of limits
short int  Region :: numberof;  // total number of defined objects


// =============================================================================
// Region :: Error
// =============================================================================
char * Region :: Error(short int err, char * mess)
{
   char message[300] = "";
   sprintf(message, "%s Error %d", mess, err);
   switch (err) {
   	   case 20000:     // constructor
   		   strcat(message, " Region : unable to allocate the limits of the region or to alloc finite_element lists");
   		   break;
   	   case 20001:     // Mat_Jacobian
   		   strcat(message, " Region : inconsistent region number");
   		   break;
   	   case 20002:     // Jacobian
   		   strcat(message, " Region : inconsistent region number");
   		   break;
   	   default:
   		   strcat(message, " Region : unknown error number");
   }
   strcpy(mess, message);
   std::cout << mess;
   return mess;
}


// =============================================================================
// Region :: Jacobian
// =============================================================================
short int Region::Jacobian(Point &point, double *jacobian)
{
	short int err = 0;
	switch (WhoAmI) {
   		case 1:
   			*jacobian = Jacobian(point);
   			break;
   		default :
   			err = 20002;
	}
	return err;
}


// =============================================================================
// constructor Cylindric_Region
// =============================================================================
Cylindric_Region :: Cylindric_Region()  : Region()
{
	char messag[200];
	n_limits = 4;
	if(!limits)
		limits = new double[n_limits]; // vector limits is only generated once
	if(!limits)
		Error(20000, &messag[0]);
	WhoAmI = ++numberof;    // numberof is initialized (equal zero)
}


// =============================================================================
// destructor Cylindric_Region
// =============================================================================
Cylindric_Region :: ~Cylindric_Region()
{
	n_limits = 0;
	if(limits) {
		delete [] limits; limits = NULL;
	}
	numberof = 0;
}


// =============================================================================
// Cylindric_Region :: Mat_Jacobian
// =============================================================================
short int Cylindric_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat)
{  // mat must be initialized with zeros
	static lev_math::Te_Matrix<double> ident(3,3,1.,0.,0.,0.,1.,0.,0.,0.,1.);
	short int err = 0;
	double D2, D, KRIE, REmD, cos2theta, sin2theta, cossin, K1, K2;
	*mat = ident;
	switch(WhoAmI) {
		case 1:
         	break;
		case 2:
			D2 = point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y() ;
			D  = sqrt (D2);
			KRIE = (limits[2] - limits[3]) * limits[3];
			REmD = limits[2] - D;
			cos2theta = point.Get_X() * point.Get_X() / D2;
			sin2theta = point.Get_Y() * point.Get_Y() / D2;
			cossin    = point.Get_X() * point.Get_Y() / D2;
			K1 = REmD * REmD/ KRIE;
			K2 = D * REmD/ KRIE;
			mat -> SetValue(1,1,K1 * cos2theta + K2 * sin2theta);
			mat -> SetValue(2,2,K2 * cos2theta + K1 * sin2theta);
			mat -> SetValue(2,1,(K1 - K2) * cossin);
			mat -> SetValue(1,2,(K1 - K2) * cossin);
			break;
		case 4:
			mat -> SetValue(3,3,(point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0])/
                                ((limits[0] - limits[1]) * limits[1]));
			break;
		case 3:
			D2 = point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y() ;
			D  = sqrt (D2);
			KRIE = (limits[2] - limits[3]) * limits[3];
			REmD = limits[2] - D;
			cos2theta = point.Get_X() * point.Get_X() / D2;
			sin2theta = point.Get_Y() * point.Get_Y() / D2;
			cossin    = point.Get_X() * point.Get_Y() / D2;
			K1 = REmD * REmD/ KRIE;
			K2 = D * REmD/ KRIE;
			mat -> SetValue(1,1,K1 * cos2theta + K2 * sin2theta);
			mat -> SetValue(2,2,K2 * cos2theta + K1 * sin2theta);
			mat -> SetValue(3,3,(point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0])/
                                	((limits[0] - limits[1]) * limits[1]));
			mat -> SetValue(2,1,(K1 - K2) * cossin);
			mat -> SetValue(1,2,(K1 - K2) * cossin);
			break;
		default:
			err = 20001;
	}
	return err;
}


// =============================================================================
// Cylindric_Region :: Mat_Jacobian
// =============================================================================
short int Cylindric_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jacobian)
{
	// mat must be initialized with zeros
	static lev_math::Te_Matrix<double> ident(3,3,1.,0.,0.,
                              0.,1.,0.,
                              0.,0.,1.);
	short int err = 0;
	double D2, D, KRIE, REmD, cos2theta, sin2theta, cossin, K1, K2;
	*mat = ident;

	switch(WhoAmI) {
      case 1 :
         *jacobian = 1.;
         break;
      case 2 :
         D2 = point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y() ;
         D  = sqrt (D2);
         KRIE = (limits[2] - limits[3]) * limits[3];
         REmD = limits[2] - D;
         cos2theta = point.Get_X() * point.Get_X() / D2;
         sin2theta = point.Get_Y() * point.Get_Y() / D2;
         cossin    = point.Get_X() * point.Get_Y() / D2;
         K1 = REmD * REmD/ KRIE;
         K2 = D * REmD/ KRIE;
         mat -> SetValue(1,1,K1 * cos2theta + K2 * sin2theta);
         mat -> SetValue(2,2,K2 * cos2theta + K1 * sin2theta);
         mat -> SetValue(2,1,(K1 - K2) * cossin);
         mat -> SetValue(1,2,(K1 - K2) * cossin);
         *jacobian = (K1 * K2);
         break;
      case 4 :
         mat -> SetValue(3,3,(point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0])/
                                ((limits[0] - limits[1]) * limits[1]));
         *jacobian = (point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0])/((limits[0] - limits[1]) * limits[1]) ;
         break;
      case 3 :
         D2 = point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y() ;
         D  = sqrt (D2);
         KRIE = (limits[2] - limits[3]) * limits[3];
         REmD = limits[2] - D;
         cos2theta = point.Get_X() * point.Get_X() / D2;
         sin2theta = point.Get_Y() * point.Get_Y() / D2;
         cossin    = point.Get_X() * point.Get_Y() / D2;
         K1 = REmD * REmD/ KRIE;
         K2 = D * REmD/ KRIE;
         mat -> SetValue(1,1,K1 * cos2theta + K2 * sin2theta);
         mat -> SetValue(2,2,K2 * cos2theta + K1 * sin2theta);
         mat -> SetValue(3,3,(point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0])/
                                ((limits[0] - limits[1]) * limits[1]));
         mat -> SetValue(2,1,(K1 - K2) * cossin);
         mat -> SetValue(1,2,(K1 - K2) * cossin);
         *jacobian = ((point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0]))/((limits[0] - limits[1]) * limits[1]) ;
         *jacobian *= (K1 * K2);
         break;
      default :
         err = 20001;
   } //end of switch(WhoAmI)
   return err;
}


// =============================================================================
// Cylindric_Region :: Jacobian
// =============================================================================
double Cylindric_Region :: Jacobian(Point &point)
{
	double jacob = 0.0;
	double D, REmD, KRIE;

	switch (WhoAmI) {
		case 1:
			jacob = 1.0;
			break;
		case 2:   // ( K1 * K2)
			D = sqrt( point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y());
			REmD = limits[2] - D;
			KRIE = (limits[2] - limits[3]) * limits[3];
			jacob = KRIE * KRIE / (D * REmD * REmD * REmD);
			break;
		case 4: // (t - Ze)^2 / Zi(Ze - Zi)
			jacob =  ((point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0]))/((limits[0] - limits[1]) * limits[1]);
			break;
		case 3: // (K1 * K2) *(t + Ze)^2 / Zi(Ze - Zi)
			D = sqrt( point.Get_X() * point.Get_X() + point.Get_Y() * point.Get_Y());
			REmD = limits[2] - D;
			KRIE = (limits[2] - limits[3]) * limits[3];
			jacob = (D * REmD * REmD * REmD)/(KRIE * KRIE)  ;
			jacob *= ((point.Get_Z() - limits[0]) * (point.Get_Z() - limits[0]))/((limits[0] - limits[1]) * limits[1]);
			break;
	}
	return jacob;
}


//================================================================================================
// =============================================================================
// Cylindric_Region :: Jacobian
// =============================================================================
short int Cylindric_Region :: Jacobian(Point &point, double *jacobian)
{
	short int err = 0;
   switch (WhoAmI)
   {
   	case 1:
      case 2:
      case 3:
      case 4:
        *jacobian = Jacobian(point); break;
      default :
      	err = 20002;
   } // end of switch (WhoAmI)
   return err;
} // end of Cylindric_Region :: Jacobian(Point &point, double jacobian)

#if 10

// =============================================================================
// constructor Parallelepipedal_Region
// =============================================================================
	Parallelepipedal_Region :: Parallelepipedal_Region()
   {
      char messag[200];
   	n_limits = 6;
      if(!limits) limits = new double[n_limits]; // limits is only dimensioned once
      if (!limits) Error(20000, &messag[0]);
      WhoAmI = ++numberof;    // numberof is initialized (equal zero)
   }
// =============================================================================
// destructor Parallelepipedal_Region
// =============================================================================
	Parallelepipedal_Region :: ~Parallelepipedal_Region()
   {
   	n_limits = 0;
      if (limits) delete[] limits;
		numberof = 0;
   }


// =============================================================================
// Parallelepipedal_Region :: Mat_Jacobian
// =============================================================================
#define transform(sup, inf) sup*sup/inf
short int Parallelepipedal_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat)
{  // mat must be initialized with zeros
	static double XEI = (limits[0] - limits[1]) * limits[1];
	static double YEI = (limits[2] - limits[3]) * limits[3];
	static double ZEI = (limits[4] - limits[5]) * limits[5];
   static lev_math::Te_Matrix<double> ident(3,3,1.,0.,0.,
                              0.,1.,0.,
                              0.,0.,1.);
   short int err = 0;
   switch(WhoAmI)
   {
      case 18 :
		  //*NMA*: acho que esta inicializa��o dever ser feita para todas as regi�es
		  //       para acertar as dimens�es da matriz
			*mat = ident;
         break;
      case 17 :
			mat->SetValue(1,1,1);
			mat->SetValue(2,2,1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 16 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 15 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 14 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 13 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 12 :
      	mat->SetValue(1,1,1);
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 11 :
      	mat->SetValue(1,1,1);
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 10 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 9 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         break;
      case 8 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 7 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 6 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 5 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 4 :
      	mat->SetValue(1,1, 1);
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 3 :
      	mat->SetValue(1,1, 1);
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         break;
      case 2 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, 1);
         break;
      case 1 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, 1);
         break;
      default :
         err = 20001;
   } //end of switch(WhoAmI)
   return err;
}   // end of function : Mat_Jacobian

// =============================================================================
// Parallelepipedal_Region :: Mat_Jacobian
// =============================================================================
short int Parallelepipedal_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jacob)
{  // mat must be initialized with zeros
	static double XEI = (limits[0] - limits[1]) * limits[1];
	static double YEI = (limits[2] - limits[3]) * limits[3];
	static double ZEI = (limits[4] - limits[5]) * limits[5];
   static lev_math::Te_Matrix<double> ident(3,3,1.,0.,0.,
                              0.,1.,0.,
                              0.,0.,1.);
   short int err = 0;
   switch(WhoAmI)
   {
      case 18 :
			*mat = ident; *jacob = 1.;
			break;
      case 17 :
			mat->SetValue(1,1,1);
			mat->SetValue(2,2,1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob = transform((point.Get_Z() - limits[4]),ZEI);
         break;
      case 16 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() - limits[2]),YEI);
         *jacob *= transform((point.Get_X() + limits[0]),XEI);
          break;
      case 15 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() + limits[2]),YEI);
         *jacob *= transform((point.Get_X() + limits[0]),XEI);
         break;
      case 14 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() - limits[2]),YEI);
         *jacob *= transform((point.Get_X() - limits[0]),XEI);
         break;
      case 13 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() + limits[2]),YEI);
         *jacob *= transform((point.Get_X() - limits[0]),XEI);
         break;
      case 12 :
      	mat->SetValue(1,1,1);
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() + limits[2]),YEI);
         break;
      case 11 :
      	mat->SetValue(1,1,1);
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_Y() - limits[2]),YEI);
         break;
      case 10 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_X() + limits[0]),XEI);
         break;
      case 9 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, transform((point.Get_Z() - limits[4]),ZEI));
         *jacob  = transform((point.Get_Z() - limits[4]),ZEI);
         *jacob *= transform((point.Get_X() - limits[0]),XEI);
         break;
      case 8 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() - limits[2]),YEI);
         *jacob *= transform((point.Get_X() + limits[0]),XEI);
         break;
      case 7 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() + limits[2]),YEI);
         *jacob *= transform((point.Get_X() + limits[0]),XEI);
         break;
      case 6 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() - limits[2]),YEI);
         *jacob *= transform((point.Get_X() - limits[0]),XEI);
         break;
      case 5 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() + limits[2]),YEI);
         *jacob *= transform((point.Get_X() - limits[0]),XEI);
         break;
      case 4 :
      	mat->SetValue(1,1, 1);
      	mat->SetValue(2,2, transform((point.Get_Y() + limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() + limits[2]),YEI);
         break;
      case 3 :
      	mat->SetValue(1,1, 1);
      	mat->SetValue(2,2, transform((point.Get_Y() - limits[2]),YEI));
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_Y() - limits[2]),YEI);
         break;
      case 2 :
      	mat->SetValue(1,1, transform((point.Get_X() + limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_X() + limits[0]),XEI);
         break;
      case 1 :
      	mat->SetValue(1,1, transform((point.Get_X() - limits[0]),XEI));
      	mat->SetValue(2,2, 1);
      	mat->SetValue(3,3, 1);
         *jacob  = transform((point.Get_X() - limits[0]),XEI);
         break;
      default :
         err = 20001;
   } //end of switch(WhoAmI)
   return err;
}   // end of function : Mat_Jacobian

// =============================================================================
// Parallelepipedal_Region :: Jacobian
// =============================================================================
double Parallelepipedal_Region :: Jacobian(Point &point)
{
	static double XEI = (limits[0] - limits[1]) * limits[1];
	static double YEI = (limits[2] - limits[3]) * limits[3];
	static double ZEI = (limits[4] - limits[5]) * limits[5];
	double jacob = 0.0;

	switch (WhoAmI) {
		case 18:
			jacob = 1.;
			break;
		case 17:
			jacob = transform((point.Get_Z() - limits[4]),ZEI);
			break;
		case 16:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() - limits[2]),YEI);
			jacob *= transform((point.Get_X() + limits[0]),XEI);
			break;
		case 15:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() + limits[2]),YEI);
			jacob *= transform((point.Get_X() + limits[0]),XEI);
			break;
		case 14:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() - limits[2]),YEI);
			jacob *= transform((point.Get_X() - limits[0]),XEI);
			break;
		case 13:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() + limits[2]),YEI);
			jacob *= transform((point.Get_X() - limits[0]),XEI);
			break;
		case 12:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() + limits[2]),YEI);
			break;
		case 11:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_Y() - limits[2]),YEI);
			break;
		case 10:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_X() + limits[0]),XEI);
			break;
		case 9:
			jacob  = transform((point.Get_Z() - limits[4]),ZEI);
			jacob *= transform((point.Get_X() - limits[0]),XEI);
			break;
		case 8:
			jacob  = transform((point.Get_Y() - limits[2]),YEI);
			jacob *= transform((point.Get_X() + limits[0]),XEI);
			break;
		case 7:
			jacob  = transform((point.Get_Y() + limits[2]),YEI);
			jacob *= transform((point.Get_X() + limits[0]),XEI);
			break;
		case 6:
			jacob  = transform((point.Get_Y() - limits[2]),YEI);
			jacob *= transform((point.Get_X() - limits[0]),XEI);
			break;
		case 5:
			jacob  = transform((point.Get_Y() + limits[2]),YEI);
			jacob *= transform((point.Get_X() - limits[0]),XEI);
			break;
		case 4:
			jacob  = transform((point.Get_Y() + limits[2]),YEI);
			break;
		case 3:
			jacob  = transform((point.Get_Y() - limits[2]),YEI);
			break;
		case 2:
			jacob = transform((point.Get_X() + limits[0]),XEI);
			break;
		case 1 :
			jacob  = transform((point.Get_X() - limits[0]),XEI);
			break;
   }

   return jacob;
}


#undef transform


// =============================================================================
// Parallelepipedal_Region :: Jacobian
// =============================================================================
short int Parallelepipedal_Region :: Jacobian(Point &point, double *jacobian)
{
	short int err = 0;
	switch (WhoAmI) {
		case 18:
		case 17:
		case 16:
		case 15:
		case 14:
		case 13:
		case 12:
		case 11:
		case 10:
		case 9:
		case 8:
		case 7:
		case 6:
		case 5:
		case 4:
		case 3:
		case 2:
		case 1:
			*jacobian = Jacobian(point);
			break;
		default :
			err = 20002;
	}
	return err;
}


#endif


// =============================================================================
// constructor Ordinary2D_Region
// =============================================================================
Ordinary2D_Region :: Ordinary2D_Region()
{
	char messag[200];
	n_limits = 1;
	if(!limits)
		limits = new double [n_limits]; // limits is only dimensioned once
	if(!limits)
		Error(20000, &messag[0]);
	WhoAmI = ++numberof;    // numberof is initialized (equal zero)
}


// =============================================================================
// destructor Ordinary2D_Region
// =============================================================================
Ordinary2D_Region :: ~Ordinary2D_Region()
{
	n_limits = 0;
	if(limits) {
		delete [] limits; limits = NULL;
	}
	numberof = 0;
}


// =============================================================================
// Ordinary2D_Region :: Mat_Jacobian
// =============================================================================

short int Ordinary2D_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat)
{
	// mat must be initialized with zeros
	static lev_math::Te_Matrix<double> ident(2,2, 1.,0., 0.,1.);
	short int err = 0;
	switch(WhoAmI) {
      	  case 1:
      		  *mat = ident;
      		  break;
      	  default:
      		  err = 20001;
	}
	return err;
} // End of function : Mat_Jacobian


// =============================================================================
// Ordinary2D :: Mat_Jacobian
// =============================================================================
short int Ordinary2D_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jacob)
{  // mat must be initialized with zeros
	static lev_math::Te_Matrix<double> ident(2,2,1.,0.,0.,1.);
	short int err = 0;
	switch(WhoAmI) {
		case 1:
			*mat = ident; *jacob = 1.; break;
		default:
			err = 20001;
	}
	return err;
}


// =============================================================================
// Ordinary2D_Region :: Jacobian
// =============================================================================
double Ordinary2D_Region :: Jacobian(Point &point)
{
   double jacob;
   switch (WhoAmI)
   {
     case 1 :
       jacob = 1.; break;
   } // end of switch (WhoAmI)

   return jacob;
} // end of Ordinary2D_Region :: Jacobian
#undef transform

// =============================================================================
// Ordinary2D_Region :: Jacobian
// =============================================================================
short int Ordinary2D_Region :: Jacobian(Point &point, double *jacobian)
{
	short int err = 0;
   switch (WhoAmI)
   {
      case 1 :
        *jacobian = Jacobian(point); break;
      default :
      	err = 20002;
   } // end of switch (WhoAmI)
   return err;
} // end of Ordinary2D_Region :: Jacobian(Point &point, double jacobian)

bool Ordinary2D_Region:: Is_Inside(Point & point)
{
	bool retorno = false;
	if(point.X() >= 0 && point.X() <= 10)
		if( point.Y() >= 0 && point.Y() <= 5 )
			retorno = true;
	return retorno;
}



// =============================================================================
// constructor Ordinary_Region
// =============================================================================
Ordinary_Region :: Ordinary_Region()
{
	char messag[200];
	n_limits = 1;
	if(!limits)
		limits = new double[n_limits]; // limits is only dimensioned once
	if (!limits)
		Error(20000, &messag[0]);
	WhoAmI = ++numberof;    // numberof is initialized (equal zero)
}


// =============================================================================
// destructor Ordinary_Region
// =============================================================================
Ordinary_Region :: ~Ordinary_Region()
{
	n_limits = 0;
	if(limits) {
		delete [] limits; limits = NULL;
	}
	numberof = 0;
}


// =============================================================================
// Ordinary_Region :: Mat_Jacobian
// =============================================================================

short int Ordinary_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat)
{  // mat must be initialized with zeros

   short int err = 0;

	if (ptr_lo_elements)
	{
		lev_math::Te_Matrix<double> ident(ptr_lo_elements[0][0]->Get_Dimension(),ptr_lo_elements[0][0]->Get_Dimension());
		for (int i = 0; i < ptr_lo_elements[0][0]->Get_Dimension(); i++) ident[i][i] = 1.;


	   switch(WhoAmI)
	   {
		  case 1 :
				*mat = ident;
			 break;
		  default :
			 err = 20001;
	   } //end of switch(WhoAmI)
	} else {
		lev_math::Te_Matrix<double> ident(1,1,1.);
		*mat = ident;
		err = 20001;
	}
	return err;
}

// =============================================================================
// Ordinary2D :: Mat_Jacobian
// =============================================================================
short int Ordinary_Region :: Mat_Jacobian(Point &point, lev_math::Te_Matrix<double> *mat, double *jacob)
{  // mat must be initialized with zeros


   short int err = 0;
	if (ptr_lo_elements)
	{
		lev_math::Te_Matrix<double> ident(ptr_lo_elements[0][0]->Get_Dimension(),ptr_lo_elements[0][0]->Get_Dimension());
		for (int i = 0; i < ptr_lo_elements[0][0]->Get_Dimension(); i++) ident[i][i] = 1.;

	   switch(WhoAmI)
	   {
		  case 1 :
				*mat = ident; *jacob = 1.; break;
		  default :
			 err = 20001;
	   } //end of switch(WhoAmI)
	}
	else
	{
		lev_math::Te_Matrix<double> ident(1,1,1.);
		*mat =  ident; *jacob = 1.;
		 err = 20001;
	}
   return err;
}   // end of function : Mat_Jacobian


// =============================================================================
// Ordinary_Region :: Jacobian
// =============================================================================
double Ordinary_Region :: Jacobian(Point &point)
{
   double jacob;
   switch (WhoAmI)
   {
     case 1 :
       jacob = 1.; break;
   } // end of switch (WhoAmI)

   return jacob;
} // end of Ordinary_Region :: Jacobian
#undef transform


// =============================================================================
// Ordinary_Region :: Jacobian
// =============================================================================
short int Ordinary_Region :: Jacobian(Point &point, double *jacobian)
{
	short int err = 0;
   switch (WhoAmI)
   {
      case 1 :
        *jacobian = Jacobian(point); break;
      default :
      	err = 20002;
   } // end of switch (WhoAmI)
   return err;
} // end of Ordinary_Region :: Jacobian(Point &point, double jacobian)


} // end namespace _region
