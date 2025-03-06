#ifndef __POINT_HPP__
#define __POINT_HPP__


/* =============================================================================

	File:  Point.cpp

   Description : Point and Nodal_Point classes

   written by: Angelo Passaro

   written at: january / 1998

   Modified by:

   Modified at:

   Modification:

   Included Classes :
            Point
            	Error Codes : from 26001 to 26020
				Template Te_Nodal_Point
            	Error Codes : from 26021 to 26040



===============================================================================
Class Point:
	Data:
    short int dimension   : geometrical dimension (default 3D)
    double    *coordinates : the geometrical coordinates. It is dimensioned
                               using the dimension datum.
                               The data are stored from zero to dimension-1.
                               For the Coordinate function the parameter for the
                               index varies from 1 to dimension.
    short int origin      : type of point (geometry, pre-discretization, etc.)


   // constructors
    //==========================================================================
    Point ()
    Point (short int dim)
    Point (double * c, short int dim=3)
    // construtor copiador
    Point (const Point &p1)

    // destructor
    //==========================================================================
    ~Point()

    // SERVICES
    //==========================================================================
    short int Dimension(void)
    void      Dimension (short int dim)

    short int Coordinate(short int i, double &f)

    short int Coordinates(double *f)

    short int Set_Coordinates(double *f)

    double Get_X(void)
    double Get_Y(void)
    double Get_Z(void)

    short int Origin()

    void      Set_Origin(short int si)
    //==========================================================================
    void Error(short int err, char *msg) ;

    // operators
    //==========================================================================
    Point & operator = (const Point & p1)


	Associations:

===============================================================================
Class Te_Nodal_Point    (TEMPLATE):
	Derived from Point

	Data:
    T * state_var;    // vector for the state variables
    short int n_state_var;    // number of state variables
    short int n_bc       ;    // number of boundary conditions
    short int * type;   // boundary condition type.

    Te_Nodal_Point()

    Te_Nodal_Point(short int nsv)

    Te_Nodal_Point(short int nsv, short int dim)

    Te_Nodal_Point(char, short int nsv, short int nf)

    Te_Nodal_Point(short int nsv, short int nf, short int dim)

    // copiador
    //==========================================================================
    Te_Nodal_Point(const Te_Nodal_Point &p1)

    //==========================================================================
    // destructor
    ~Te_Nodal_Point()

    // SERVICES
    //==========================================================================
    short int Degrees_Of_Freedom(void)

    void      Boundary_Condition(short int tipo, T &valor, short int qual)

    short int Boundary_Condition (short int qual)

    short int N_Boundary_Condition(void)

    Te_Nodal_Point & operator = (const Te_Nodal_Point &p1)

    short int Set_Variables (T *vars, short int nvar)

    void      Set_Variables (short int pos, T &var)

    T         Get_Variables (short int pos)

    short int Get_Variables (T * outter)

    short int Get_N_Variables(void)

    // ERRORS DESCRIPTION
    //==========================================================================
    void Error(short int err, char *msg);
================================================================================
*/


#include "utility.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>


class Point
{
	protected:
    	short int dimension;
    	short int origin;
    	double *   coordinates;

    	void Initialize(const short int dim) ;

    	void Initialize_Copy(const short int dim, const double *c)
        {
    		Point :: Initialize(dim);
    		memcpy(&coordinates[0], &c[0], dim*sizeof(double));
        }

    	void Deinitialize (void);

	public :

		//==========================================================================
		// constructors
		Point ()              { Point::Initialize(3);}
		Point (const short int dim) { Point::Initialize(dim);}
		Point (double * c, const short int dim=3) { Point::Initialize_Copy(dim, c);}
		// construtor copiador
		Point (const Point &p1)  { Point::Initialize_Copy(p1.dimension, p1.coordinates);}

		//==========================================================================
		// destructor
		virtual ~Point() { Point::Deinitialize();}

		// SERVICES
		//==========================================================================
		short int Dimension(void) { return dimension; }
		void      Dimension (short int dim) {Point::Deinitialize(); Point::Initialize(dim);}

		//==========================================================================
		short int Coordinate(short int i, double &f)
		{ short int err = 0;  // error free
		  if ( i > 0 && i <= dimension) f = coordinates[i-1];
		  else {f = lev_math::utility::BIG_FLOAT; err = 26002; }
		  return err;
		} //====================================================== end of Coordinate

		short int Coordinates(double *f)
		{ short int err = 0;  // error free
		  if (f) memcpy( &f[0], &coordinates[0], dimension*sizeof(double));
		  else err = 26003;
		  return err;
		} //===================================================== end of Coordinates

		short int Set_Coordinates(double *f)
		{ short int err = 0;  // error free
		  if (f) memcpy( &coordinates[0], &f[0], dimension*sizeof(double));
		  else err = 26004;
		  return err;
		} //===================================================== end of Coordinates

		double Get_X(void) {return X();}  // compatibility with previous version
		double Get_Y(void) {return Y();}  // compatibility with previous version
		double Get_Z(void) {return Z();}  // compatibility with previous version

		double X(void) { return coordinates[0]; }
		double Y(void) { return coordinates[1]; }
		double Z(void)
		{
			if(dimension == 3)
				return coordinates[2];
			else
				return 0.;
		}

		void X(double x) { coordinates[0] = x; }
		void Y(double y) { coordinates[1] = y; }
		void Z(double z)
		{
			if(dimension == 3)
				coordinates[2] = z;
		}


		//==========================================================================

		short int Origin (void) { return origin;}
		void      Set_Origin (short int ori) { origin = ori;}

		//==========================================================================

		void Error(short int err, char *msg) ;

		// operators
		//==========================================================================
		Point & operator = (const Point & p1)
		{ if (p1.dimension != dimension){ Point :: Deinitialize(); Point :: Initialize(p1.dimension); }
		  memcpy(&coordinates[0], &p1.coordinates[0], dimension*sizeof(double));
		  return *this;
		}

		virtual void Save(char* nome_arquivo,int op=0);//*NMA*:23/01/2007: op=0-> "w" (write) op=1->"a" (append) ex: Save("arquivo.dat",1)
}; // end of Point Class


template <class T> class Te_Nodal_Point : public Point
{
	protected:
		T * state_var;         // vector for the state variables
		short *type_bc;        // vector for the boundary condition type
		short int n_state_var; // number of degrees of freedom
		short int n_bc;        // number of boundary conditions associated to this nodal point
		short int n_source;    // dimension of source data
		//    short int type;     // boundary condition type. It is assumed that all the state
                        // variables present the same boundary condition type.
                        // type : 0 - state variable
                        //        1 to 6  - (The BC types are listed in the document
                        //  "Arquivos de comunica��o de cados do sistema ATERDIST"
		short int vertice_of_elements;   // models the links with 3D finite elements
                                     // (number of elements that use this nodal point

		void Initialize (short int nsv, short int ns=0);
		void Deinitialize (void);   // apos os testes passar para dentro da declaracao  de classe

	public :
    long *DLNC; // SEE DHATT PAG 309

    Te_Nodal_Point(): Point()
    { Initialize(1); }

    Te_Nodal_Point(short int nsv): Point()
    { Initialize(nsv); }

    Te_Nodal_Point(char, short int nsv, short int nf)
    { Initialize(nsv,nf); }

    Te_Nodal_Point(short int nsv, short int dim): Point(dim)
    { Initialize(nsv); }

    Te_Nodal_Point(short int nsv, short int nf, short int dim): Point(dim)
    { Initialize(nsv,nf); }

    // copiador
    //==========================================================================
    Te_Nodal_Point(const Te_Nodal_Point &p1)
    { Point:: Initialize_Copy (p1.dimension, &p1.coordinates[0]);
      Initialize(p1.n_state_var, p1.n_source);
      memcpy(&state_var[0], &p1.state_var[0], (n_state_var+n_source)*sizeof(T));
      memcpy(&type_bc[0], &p1.type_bc[0], n_state_var*sizeof(short));
    }

    //==========================================================================
    // destructor
    ~Te_Nodal_Point()  { Deinitialize(); }

    // SERVICES
    //==========================================================================
//    void      Degrees_Of_Freedom(short int ndof)
//    { if (state_var) delete [] state_var;  Initialize(ndof,n_source);  }

    //==========================================================================
    short int Degrees_Of_Freedom(void){return (n_state_var - n_bc);}

     //==========================================================================
    void      Sources(short int nf)
    { if (state_var) delete [] state_var;  Initialize(n_state_var,nf);  }

    //==========================================================================
    short int Sources(void){return n_source;}

    //==========================================================================
    void      Boundary_Condition(short int tipo, T &valor, short int qual = 1)
              {
                if (qual > n_state_var || qual < 1)
                { char mens[128]; Error(26035,&mens[0]); std::cout << "\n "<< mens; qual = n_state_var;}

                type_bc[qual-1] = tipo; state_var[qual-1] = valor; n_bc++;
              }

    //==========================================================================
    short int Boundary_Condition (short int qual = 1)
              {
                if (qual > n_state_var || qual < 1)
                {
					char mens[128]; Error(26034,&mens[0]); std::cout << "\n "<< mens; qual = n_state_var;
				}

                return type_bc[qual-1];
              }

    //==========================================================================
    short int N_Boundary_Conditions (void) { return n_bc;}

    //==========================================================================
    Te_Nodal_Point & operator = (const Te_Nodal_Point &p1)
    { if (p1.dimension != dimension) { Point :: Deinitialize(); Point :: Initialize(p1.dimension); }

      if ((p1.n_state_var != n_state_var) || (p1.n_source != n_source)) {Deinitialize(); Initialize (p1.n_state_var, p1.n_source);}
      n_bc = p1.n_bc;
      vertice_of_elements = p1.vertice_of_elements;
      memcpy(&coordinates[0], &p1.coordinates[0], dimension*sizeof(double));
      memcpy(&state_var[0], &p1.state_var[0], (n_state_var+n_source)*sizeof(T));
      memcpy(&type_bc[0], &p1.type_bc[0], n_state_var*sizeof(short));
      return *this;
    }

    //==========================================================================
    short int Set_Variables (T *vars, short int nvar)
    { short int err = 0;  // no errors
      if (vars)
      {
        if       (nvar == n_state_var)    memcpy(&state_var[0], &vars[0], n_state_var *sizeof(T));
        else if  (nvar < n_state_var )
        { err = 26022;
          memcpy(&state_var[0], &vars[0], nvar *sizeof(T));
        }
        else
        { err = 26023;
          memcpy(&state_var[0], &vars[0], n_state_var *sizeof(T));
        }
      }
      else err = 26024;
      if (err) { char mens[128]; Error(err,&mens[0]); std::cout << "\n "<< mens;}
      return err;
    }

    //==========================================================================
    void      Set_Variables (short int pos, T var)
    {
      if (pos > n_state_var)
      { char mens[128]; Error(26027,&mens[0]); std::cout << "\n "<< mens; state_var [n_state_var-1] = var;}
     else state_var[pos-1] = var;

    }

    //==========================================================================
    T         Get_Variables (short int pos)
    {
      if (pos > n_state_var)
      { char mens[128]; Error(26026,&mens[0]); std::cout << "\n "<< mens; return state_var [n_state_var-1];}
      else return state_var [pos-1];

    }

    //==========================================================================
    short int Get_Variables (T * outter)
    { short int err = 0 ; // no errors
      if (outter)  memcpy(&outter[0], &state_var[0], n_state_var * sizeof(T));
      else { err = 26025;}
      return err;
    }
    //==========================================================================
    short int Get_N_Variables (void) { return n_state_var; }

    //==========================================================================
    short int Set_Sources (T *vars, short int nvar)
    { short int err = 0;  // no errors
      if (vars)
      {
        if       (nvar == n_source)    memcpy(&state_var[n_state_var], &vars[0], n_source *sizeof(T));
        else if  (nvar < n_source )
        { err = 26028;
          memcpy(&state_var[n_state_var], &vars[0], nvar *sizeof(T));
        }
        else
        { err = 26029;
          memcpy(&state_var[n_state_var], &vars[0], n_source *sizeof(T));
        }
      }
      else err = 26030;
      return err;
    }

    //==========================================================================
    void      Set_Sources (short int pos, T var)
    {
      if (pos > n_source)//*NMA*:verificar!!!!!!!!!!!
      { char mens[128]; Error(26033,&mens[0]); std::cout << "\n "<< mens; state_var [n_state_var+n_source-1] = var;}
      else state_var[n_state_var+pos] = var;
    }

    //==========================================================================
    T         Get_Sources (short int pos)
    {
      if (pos >= n_source)//*NMA*:verificar!!!!!!!!!!!
      { char mens[128]; Error(26032,&mens[0]); std::cout << "\n "<< mens; return state_var [n_state_var+n_source-1];}
      else return state_var [n_state_var+pos];
    }

    //==========================================================================
    short int Get_Sources (T * outter)
    { short int err = 0 ; // no errors
      if (outter)  memcpy(&outter[0], &state_var[n_state_var], n_source * sizeof(T));
      else { err = 26031;}
      return err;
    }

    //==========================================================================
    void  Increase_Elements(void) {vertice_of_elements++;}

    short Get_N_Elements(void)    {return vertice_of_elements;}

    // ERRORS DESCRIPTION
    //==========================================================================
    void Error(short int err, char *msg)
{
      if (!msg) return;        // msg pointer NULL
      sprintf(msg, "Error %d", err);

      switch (err)
      {
        case 26021:
          strcat(msg, ": unable to alloc memory!");
        break;
        case 26022:
          strcat(msg, ": number of variables less than the degree of freedom. The data were copied!");
        break;
        case 26023:
          strcat(msg, ": number of variables greater than the degree of freedom. Only the data associated with the number of degrees of freedom were copied!");
        break;
        case 26024:
          strcat(msg, ": NULL pointer!");
        break;
        case 26025:
          strcat(msg, ": NULL pointer!");
        break;
        case 26026:
          strcat(msg, ": number of the variable asked is greater than the degree of freedom!");
        break;
        case 26027:
          strcat(msg, ": number of the variable asked is greater than the degree of freedom!\n The datum was stored on the last position.");
        break;
        case 26028:
          strcat(msg, ": number of data less than the number of sources. The data were copied!");
        break;
        case 26029:
          strcat(msg, ": number of data greater than the number of sources. Only the data associated with the number of sources were copied!");
        break;
        case 26030:
          strcat(msg, ": NULL pointer!");
        break;
        case 26031:
          strcat(msg, ": NULL pointer!");
        break;
        case 26032:
          strcat(msg, ": number of the variable asked is greater than the number of sources!");
        break;
        case 26033:
          strcat(msg, ": number of the variable asked is greater than the number of sources!\n The datum was stored on the last position.");
        break;
        case 26034:
          strcat(msg, ": boundary condition requested for a variable number greater than the number of variables!");
        break;
        case 26035:
          strcat(msg, ": boundary condition for a variable number greater than the number of variables!\n The datum was stored on the last position.");
        break;
        default:
          strcat(msg, ": Unknown...");
      }
}

}; // end of template TE_Nodal_Point


template <class T>
void Te_Nodal_Point<T> :: Deinitialize (void)
{
    if(state_var) {
        delete [] state_var;
        state_var = NULL;
    }
    if(type_bc) {
        delete [] type_bc;
        type_bc = NULL;
    }
    if(DLNC) {
        delete [] DLNC;
        DLNC = NULL;
    }
}


template <class T>
void Te_Nodal_Point<T> :: Initialize (short int nsv, short nf)
{
	vertice_of_elements = 0;
	n_bc = 0;
	n_state_var = nsv;
	n_source = nf;
	type_bc = NULL;
	state_var = NULL;
	state_var = new T[n_state_var+n_source];
	type_bc = new short[n_state_var];
	DLNC = new long[n_state_var];

	if(!state_var || !type_bc || !DLNC) {
		char msg[512] = "";
		Error(26021, &msg[0]); std::cout << msg;
		exit(26021);
	}
	if((n_state_var+n_source) > 0)
		memset(state_var, 0,(n_state_var+n_source)*sizeof(T));
	if(n_state_var > 0) {
		memset(type_bc, 0, (n_state_var)*sizeof(short));
		memset(DLNC, 0, (n_state_var)*sizeof(long));
	}
}


#endif
