#ifndef __FINITE_ELEMENT__
#define __FINITE_ELEMENT__


/* =============================================================================

	File:  fin_elem.hpp

   Description : Finite element classes

   written by: Angelo Passaro

   written at: september / 1997

   Modified by:

   Modified at:


   Included Classes :
        Finite_Element (Abstract)
				Nodal_Elements (Abstract)
				Simplex_Nodal_Element_1D
				Simplex_Nodal_Element_3D ( to mantain compatibility with the old
					  class architecture the Elemento_3D_Basico class will be derived
					  from it)

	Error Codes : from 23000 to 23999
===============================================================================
Class Finite_Element:
	Data:
		 short int n_vertices: number of element vertices

	Associations:
		long int * nodes : At the present time (09/09/97), this is a vector containing the
								 index of the node points into the general node point
								 list;

								 In the future, this must be substituted for a list of
								 pointers to the node points (Pt_Malha ** nodes), to increase
								 the finite element calculation efficiency;

		Material * material : association with the base class of material.

===============================================================================
Class Nodal_Element:
	Derived from Finite_Element

	Data:
		static unsigned char order   : approximation order of the finite element
												 (ATTENTION: It is a FLAG to define the
												 behaviour of the Build_higher_order and
												 Base_function methods.)

===============================================================================
Class Simplex_Nodal_Element_3D
	Derived from Nodal_Element

	Data:
		static short int n_nodes    : number of nodes of the element for the
												defined order;
		double Volume               : derived data  used to increase the finite
												element calculation efficiency

============================================================================= */


#include "lo_templ.hpp"
#include "point.hpp"
#include "material.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sources.hpp"
#include <iostream>
#include <fstream>
#include <memory>


#define Gauss_Pt   Te_Nodal_Point<double>


class Finite_Element
{
	protected:
			short int	dimension;			// dimension of the element
			short int	n_vertices;			// number of vertices of the element
			long  int*	nodes;				// list of the defining vertices of the element
			short int	num_vizinhos;		// number  of associated face neighbor
			long *		vizinhos;			// face neighbor indices

			short int	num_max_vizinhos;   // maximum number of face neighborhood
			Material*	material;
			CSource *	source;				//ponteiro para objeto de fonte: inclus�o de tratamento de fontes em 24/11/2005 (*NMA*)

	virtual short int	Build_Higher_Order(void) = 0;
			void		Message (short int err, char * = NULL);

	public :

			static	lev_math::Lo_Templ<Point *,long> *lo_Points;

	Finite_Element();
    Finite_Element(short int );
	virtual ~Finite_Element();

	virtual double Get_Vol(void) { return 0.0; };

			short int Vertices(long int *ptr_nodes = NULL)
			{
				if (ptr_nodes)
					memcpy(ptr_nodes, nodes, sizeof(long int) * n_vertices);
				return n_vertices;
			};

			long  int*		Vertices(short int & nv) { nv = n_vertices; return nodes;}
			short int		Set_Vertices( const long int *);

	inline	void			Set_Dimension(short int d) {dimension = d;}
	inline	short int		Get_Dimension(void) {return dimension ;}

	static	void			Associate(lev_math::Lo_Templ<Point *,long> *lp) { lo_Points = lp;} ;

	virtual short int		Nodes(long int * = NULL )    = 0;   // direct acess to nodes
	virtual short int		Get_Nodes(long int * = NULL) = 0;   // encapsulate nodes
	virtual void			Set_Nodes(long int *)        = 0;
	virtual short int		Get_N_Nodes()				 = 0;         // computes the number of nodes for the given ord
	virtual double			Get_Size(void)               = 0;
	virtual void			Set_Size (double=0.0)        = 0;
	virtual int				Get_Centroid(Point &)		 = 0;
	virtual Material *		Get_Material (void) {return material;}
	virtual void			Set_Material (Material * mat) { material = mat;};
	virtual CSource *		Get_Source (void) {return source;} //inclus�o de tratamento de fontes em 24/11/2005 (*NMA*)
	virtual void			Set_Source (CSource * fonte) { source = fonte;};
				long int    Get_Max_Neighbors() { return num_max_vizinhos; }
				long int    Get_Neighbors() { return num_vizinhos; }
				long int*	Get_Neighbors(short int & nv){nv = num_vizinhos; return vizinhos;}
	virtual unsigned int	Set_Neighbor(long);
    virtual short int		Gauss_Points  (int, int &, Gauss_Pt *) = 0;
	virtual double			Get_Biggest_Edge() = 0;
	virtual void			Save(char* nome_arquivo,int op=0)=0;//*NMA*:23/01/2007: op=0-> "w" (write) op=1->"a" (append) ex: Save("elemento.dat","a")
}; // end of the Finite_Element Class


class Nodal_Element : public Finite_Element
{
	protected:
		short int order;				//*AMJ* - deixando 'order' nao estatica 30-06-2006
		short int n_nodes;				//*AMJ* - deixando 'order' nao estatica 30-06-2006
		double size;     // size depends of the element dimension
		double jacHom;					// determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo

		virtual short int Build_Higher_Order(void)=0;

	public :
		lev_math::Te_Matrix<double> JHom; // matriz jacobiana da transformacao de coordenadas reais para sistema homogeneo
		lev_math::Te_Matrix<double> Jac, jac; // matriz jacobiana da transformacao de sistema homogeneo para coordenadas reais
		double JacDet, jacDet;
		int integrationOrder;

		Nodal_Element();
		Nodal_Element(int); //*AP* define a ordem do elemento
		virtual ~Nodal_Element()
		{
			if(nodes) {
				delete [] nodes;
				nodes = NULL;
			}
			if(vizinhos) {
				delete [] vizinhos;
				vizinhos = NULL;
			}
			if(source) {
				delete [] source;
				source = NULL;
			}
			if(material) {
				delete material;
				material = NULL;
			}
		}

		virtual double Get_Vol(void) { return 0.0; };

		short int Order(void) { return order; }  //AMJ - por causa da 'order' tb mudam estas
		void Set_Order(short int ord)
		{
			order = ord;
			n_nodes = Get_N_Nodes();
			if(n_nodes) {
				if(nodes) {
					delete [] nodes;
					nodes = NULL;
				}
				nodes = new long int [n_nodes];
				if(!nodes) {
					Message(23004, (char*)("Nodal_Element default constructor"));
					exit(23004);
				}
			}
		}

		short int Get_Nodes(long int *no = NULL)     // avoid direct access to the nodes
		{
			if (no)
				memcpy(no, nodes, sizeof(long int) * n_nodes);
			return n_nodes;
		};
		void Set_Nodes(long int *no)
		{
			if (no)
				memcpy(nodes, no, sizeof(long int) * n_nodes);
		};

		virtual short int Get_N_Nodes() = 0;

		virtual void      Base_Function(Point *point, lev_math::Te_Vector<double> &vetorN) = 0;
		virtual double    Base_Function(Point *point, short int qual) = 0;
		virtual void      GradN (Point *point, lev_math::Te_Matrix<double> &DN) = 0;
		virtual void      DivN  (Point *point, lev_math::Te_Matrix<double> &divN) = 0;
		virtual void      RotN  (Point *point, lev_math::Te_Matrix<double> &rotN) = 0;

		// coordinate transformation
		virtual void      MatJacobHom (void) = 0;
		        void      MatJacobHom (lev_math::Te_Matrix<double> &Jhom);               //*AP* 11/10/2007 - implementa��o generalizada para Simplex 1D,2D e 3D
		        void      MatJacobHom (lev_math::Te_Matrix<double> &Jhom, double &jac);  //*AP* 11/10/2007 - implementa��o generalizada para Simplex 1D,2D e 3D
		        double	  MatJacobHomDet(void);								   //*AP* 11/10/2007 - implementa��o generalizada para Simplex 1D,2D e 3D

		virtual void      CartesianToHom(Point &point, Point &phom ) = 0;
		virtual void      HomToCartesian(Point &phom , Point &point) = 0;

		virtual double    Get_Size(void) {if (!size) Set_Size(0); return size;}
		virtual void      Set_Size (double vol=0.0) {size = vol;};

		virtual bool	  Is_Inside(Point &) = 0;
		virtual double	  Get_Biggest_Edge() = 0;

		virtual void	 Save(char* nome_arquivo,int op=0);//*NMA*:23/01/2007: op=0-> "w" (write) op=1->"a" (append) ex: Save("elemento.dat","a")
}; // end of the Nodal_Element class


class Simplex_Nodal_Element_1D : public Nodal_Element
{
	protected:
		//static short int n_nodes;

		virtual short int  Build_Higher_Order(void);

		virtual short int Resize_Nodes( void)
		{
			short int ret = 23000;
			Message ( ret, (char*)( "Resize_Nodes"));
			return ret;
		};

	public :
      Simplex_Nodal_Element_1D();
	  Simplex_Nodal_Element_1D(int);  //*AP* 30/06/2006

	  virtual ~Simplex_Nodal_Element_1D() {};

	  virtual short int Nodes(long int * no = NULL)
	  {
		  if (no)
			  no = nodes;
		  return n_nodes;
	  }    // direct access to the nodes

      short int Get_N_Nodes()   //* *AP* 30/06/2006
      {
    	  return ((order+1));
      }

      double Get_Vol(void)
      {
    	  return JacDet*2.0;
      }

      virtual void Set_Size (double vol=0.0)
      {
    	  if (!vol) {
    		  if(nodes) {
    			  Point *p0, *p1;
    			  p0 = lo_Points[0][nodes[0]];
    			  p1 = lo_Points[0][nodes[1]];
    			  double x12;
    			  x12 = (p0->Get_X() - p1->Get_X())/2;
    			  if(x12 < 0)
    				  vol = -x12;
    			  else
    				  vol = x12;
    		  }
    	  }
    	  size = vol;
      } // end of Set_Size

      virtual int		Get_Centroid(Point &);
      void      MatJacobHom (void);
      void      CartesianToHom(Point &point, Point &phom);
      void      HomToCartesian(Point &phom, Point &point);
      void      Transformation_Coefficients(double *, double *);

      Simplex_Nodal_Element_1D & operator = (Simplex_Nodal_Element_1D &);
      short int Gauss_Points  (int, int &, Gauss_Pt *);

      bool		Is_Inside(Point &);
      unsigned int Set_Vizinho(long);
      double		Get_Biggest_Edge()
      {
    	  return Get_Size();
      }
}; // end of Simplex_Nodal_Element_1D class


class Lagrange_Element_1D : public Simplex_Nodal_Element_1D
{
	protected:

	public :
		Lagrange_Element_1D ();
		Lagrange_Element_1D (int);

		virtual ~Lagrange_Element_1D () { };

		short int Get_N_Nodes() { return ((order+1)); }

		void   Base_Function(Point  *point, lev_math::Te_Vector<double> &vetorN);
		double Base_Function(Point  *point, short int qual);
		void   DivN(Point *point, lev_math::Te_Matrix<double> &divN);
		void   GradN(Point *point, lev_math::Te_Matrix<double> &DN);
		void   RotN(Point *point, lev_math::Te_Matrix<double> &rotN);

		Lagrange_Element_1D & operator = (Lagrange_Element_1D &);
}; // end of Lagrange_Element_1D class


class Hermite_Element_1D : public Simplex_Nodal_Element_1D
{
	protected:

	public :
		Hermite_Element_1D ();
		Hermite_Element_1D (int);

		virtual ~Hermite_Element_1D () { };

		short int Get_N_Nodes() { return (n_vertices); }

		void   Base_Function(Point  *point, lev_math::Te_Vector<double> &vetorN);
		double Base_Function(Point  *point, short int qual);
		void   DivN(Point *point, lev_math::Te_Matrix<double> &divN);
		void   GradN(Point *point, lev_math::Te_Matrix<double> &DN);
		void   RotN(Point *point, lev_math::Te_Matrix<double> &rotN);

		Hermite_Element_1D & operator = (Hermite_Element_1D &);
}; // end of Hermite_Element_1D class


//=============================================================================


class Simplex_Nodal_Element_2D : public Nodal_Element
{
	protected:
//		static short int n_nodes; //AMJ - deixando 'order' nao estatica 30-06-2006

		virtual short int  Build_Higher_Order(void);

    virtual short int Resize_Nodes( void)
    { short int ret = 23000; Message ( ret, (char*)("Resize_Nodes")); return ret;};

	public :

      Simplex_Nodal_Element_2D ();
		Simplex_Nodal_Element_2D (int);  //*AP* 30/06/2006
      virtual ~Simplex_Nodal_Element_2D () {};
      virtual short int Nodes(long int * no = NULL)
					{	if (no) no = nodes; return n_nodes;	}    // direct access to the nodes

    short int         Get_N_Nodes()   //* *AP* 30/06/2006
          { return ((order+1) * (order+2)/2); }

    double Get_Vol(void) { return JacDet/2.0; }

	virtual void      Set_Size (double vol=0.0);
	virtual int		Get_Centroid(Point &);

	void      MatJacobHom (void);

//	void      MatJacobHom (lev_math::Te_Matrix<double> &Jhom);
//	void      MatJacobHom (lev_math::Te_Matrix<double> &Jhom, double &jac);
//	double	  MatJacobHomDet(void);

    void      CartesianToHom(Point &point, Point &phom);
    void      HomToCartesian(Point &phom, Point &point);

    void      Transformation_Coefficients(double *, double *, double *);

    Simplex_Nodal_Element_2D & operator = (Simplex_Nodal_Element_2D &);
	Simplex_Nodal_Element_2D & operator == (Simplex_Nodal_Element_2D &);
    short int Gauss_Points  (int, int &, Gauss_Pt *);


	bool		Is_Inside(Point &);  // *AP* aten��o: Esta busca tem de ser otimizada
		// Teremos que considerar algum crit�rio para ordenar o vetor de elementos ........
	unsigned int Set_Vizinho(long);

	double		Get_Biggest_Edge();


}; // end of Simplex_Nodal_Element_2D class


class Lagrange_Element_2D : public Simplex_Nodal_Element_2D
{
	protected:
	public :
		Lagrange_Element_2D();
		Lagrange_Element_2D(int);

		virtual ~Lagrange_Element_2D() { };

		short int Get_N_Nodes() { return ((order+1) * (order+2)/2); }

		void   Base_Function(Point *point, lev_math::Te_Vector<double> &vetorN);
		double Base_Function(Point *point, short int qual);

		void   GradN(Point *point, lev_math::Te_Matrix<double> &DN);
		void   DivN(Point *point, lev_math::Te_Matrix<double> &divN);
		void   RotN(Point *point, lev_math::Te_Matrix<double> &rotN);

		Lagrange_Element_2D & operator = (Lagrange_Element_2D &);
}; // end of Lagrange_Element_2D class


class Hermite_Element_2D : public Simplex_Nodal_Element_2D
{
	protected:

	public :
		Hermite_Element_2D();
		Hermite_Element_2D(int);

		virtual ~Hermite_Element_2D() { };

		short int Get_N_Nodes() { return (n_vertices); }

		void   Base_Function(Point  *point, lev_math::Te_Vector<double> &vetorN);
		double Base_Function(Point  *point, short int qual);
		void   DivN(Point *point, lev_math::Te_Matrix<double> &divN);
		void   GradN(Point *point, lev_math::Te_Matrix<double> &DN);
		void   RotN(Point *point, lev_math::Te_Matrix<double> &rotN);

		Hermite_Element_2D & operator = (Hermite_Element_2D &);
}; // end of Hermite_Element_2D class


class Simplex_Nodal_Element_3D : public Nodal_Element
{
	protected:
		virtual short int  Build_Higher_Order(void);

		virtual short int Resize_Nodes( void)
		{
			short int ret = 23000;
			Message ( ret, (char*)( "Resize_Nodes"));
			return ret;
		};

	public:
		Simplex_Nodal_Element_3D();
		Simplex_Nodal_Element_3D(int);  //*AP* 30/06/2006
		virtual ~Simplex_Nodal_Element_3D() {};
		virtual short int Nodes(long int * no = NULL)
		{
			if (no)
				no = nodes;
			return n_nodes;
		}
		double Get_Vol(void) { return Get_Size(); } // Nao verificado
		virtual void    Set_Size (double vol=0.0) ;
		virtual int		Get_Centroid(Point &);
		void      MatJacobHom (void);
		void      MatJacobHom (lev_math::Te_Matrix<double> &Jhom);
		void      CartesianToHom(Point &point, Point &phom);
		void      HomToCartesian(Point &phom, Point &point);
		void      Transformation_Coefficients(double *, double *, double *, double *);

		Simplex_Nodal_Element_3D & operator = (Simplex_Nodal_Element_3D &);
		short int Gauss_Points  (int, int &, Gauss_Pt *);
		bool		Is_Inside(Point &);
		unsigned int Set_Vizinho(long);
		double		Get_Biggest_Edge();
}; // end of Simplex_Nodal_Element_3D class


class Lagrange_Element_3D : public Simplex_Nodal_Element_3D
{
	protected:
	public:
		Lagrange_Element_3D();
		Lagrange_Element_3D(int);
		virtual ~Lagrange_Element_3D() {};
		short int Get_N_Nodes() { return ((order+1) * (order+2) * (order+3)/6); };

		void      Base_Function(Point  *point, lev_math::Te_Vector<double> &vetorN);
		double    Base_Function(Point  *point, short int qual);
		void      GradN (Point  *point, lev_math::Te_Matrix<double> &DN);
		void      DivN  (Point  *point, lev_math::Te_Matrix<double> &divN);
		void      RotN  (Point  *point, lev_math::Te_Matrix<double> &rotN);

		Lagrange_Element_3D & operator = (Lagrange_Element_3D &);
}; // end of Lagrange_Element_3D class


class Hermite_Element_3D : public Simplex_Nodal_Element_3D
{
	protected:
	public:
		Hermite_Element_3D();
		Hermite_Element_3D(int);
		virtual ~Hermite_Element_3D() {};
		short int Get_N_Nodes() { return ((order+1) * (order+2) * (order+3)/6); };

		void      Base_Function(Point  *point, lev_math::Te_Vector<double> &vetorN);
		double    Base_Function(Point  *point, short int qual);
		void      GradN (Point  *point, lev_math::Te_Matrix<double> &DN);
		void      DivN  (Point  *point, lev_math::Te_Matrix<double> &divN);
		void      RotN  (Point  *point, lev_math::Te_Matrix<double> &rotN);

		Hermite_Element_3D & operator = (Hermite_Element_3D &);
}; // end of Hermite_Element_3D class

#endif  // end of __FINITE_ELEMENT__
