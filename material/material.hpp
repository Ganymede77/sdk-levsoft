#if !defined __MATERIAL__
#define __MATERIAL__

//#include <stdio.h>
//#include <stdlib.h>
////#include <iostream.h>
//#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "point.hpp"

const int TAM_PHYS = 10;

class Material
{
  protected :
	double Physical_Property [TAM_PHYS];
	char   Phys_Prop_Name[TAM_PHYS][30];
	int    n_properties;
    static double vacuum_permeability;
    static double vacuum_permittivity;

  public:
   static short int numberof;
   char name[81];
   short int code;
   double	angle;
   short int ident;
   double conductivity,
          permeability,
          permittivity;
   bool   effectiveMass_Anisotropy;
   double effectiveMass_Electron, effectiveMass_HeavyHole, effectiveMass_LightHole, effectiveMass_SplitOff,
          potential_Electron, potential_HeavyHole, potential_LightHole, potential_SplitOff,
          V_Electron, V_HeavyHole, V_LightHole, V_SplitOff,
          gap_Gamma, gamma,
          doping,
          impurity_electron_bind_energy,
          slope;
   int    type; // Barrier = 0, Well = 1

   Material();
   virtual short int Recover(FILE *, short int, short int = 0)= 0;
   char * Message (short int, char * = NULL);
	bool Property_Value(Point &, char * , double *);
	double Property_Value(Point &);
	bool Set_Property(double, char *);

	virtual ~Material(){};

}; // end of class Material


class Material_Ground : public Material
{
	protected:
   double * physical_dimensions;
   short int num_dimensions;

   public:
   Material_Ground();
	~Material_Ground();
   virtual short int Relevant_Sizes(double * );
   virtual short int Recover(FILE *, short int, short int = 0)= 0;
}; // end of abstract class Material_Ground


class Mat_Conductor : public Material_Ground
{
public:
	Mat_Conductor();
	~Mat_Conductor()
	{
		if(physical_dimensions) {
			delete [] physical_dimensions;
			physical_dimensions = NULL;
		}
	};
	double Area(void) { return physical_dimensions[0];};
	short int Recover(FILE *, short int, short int = 0);
};


class Mat_Soil : public Material_Ground
{
public:
	Mat_Soil();
	~Mat_Soil()
	{
		if(physical_dimensions) {
			delete [] physical_dimensions;
			physical_dimensions = NULL;
		}
	};

	short int Recover(FILE *, short int, short int = 0);
};


//===============================================================================
// classe de material isotr�pico: somente para a primeira implementa��o da leitura pela classe Cproject_Solver

class CIsotropicMaterial : public Material
{
public:
	CIsotropicMaterial(){n_properties = 0;}
	~CIsotropicMaterial(){};


	short int Recover(FILE *fp, short int a, short int = 0) { a = 0; return 0; } // somente para poder instanciar um objeto
};


#endif  // !defined __MATERIAL__


//===================================================
/*class potential
{





}*/

