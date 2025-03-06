#ifndef _PONTO_
#define _PONTO_

#include "gam.hpp"
#include "levpoint.h"


typedef CLevPoint Ponto;

//class Ponto_Base : public Ponto
class Ponto_Base : public CLevPoint
{
//	friend class Lista_Pontos;

protected  :
	lev_math::Uchar  tipo;       // indica se o ponto � de geometria, pre_discr ou refinamento
						// 0 - geom
						// 1 - pre-discr
						// 2 - refinam.

    char   dummy   ;   // apenas para fazer n_bytes par e facilitar transferencia de bloco para XMS


	public     :
				Ponto_Base()     ;
			 ~Ponto_Base()     ;
			 lev_math::Uchar  		Get_Tipo(void);
	        void   		Set_Tipo (lev_math::Uchar tip);
			  Ponto_Base & operator= (Ponto_Base &);
	        char        operator == (Ponto_Base &);

} ;

//----------------------------------------------------------------------------------------
const int COISAS_POR_NO = 4;

class Pt_Malha : public Ponto_Base  {
//	friend class Lista_Pontos;
	protected  :

		short int num_materiais_derivadas, num_potenciais, num_cc, num_fontes;

		int			Num_Geometrias;
		long		material    [COISAS_POR_NO];
		double		DerivadaPrim[COISAS_POR_NO];
//		double		Potencial   [COISAS_POR_NO];
		double		Potencial   ;
//		double		ValorCC     [COISAS_POR_NO];
		double		ValorCC     ;
//		short int	TipoCC      [COISAS_POR_NO];
		short int	TipoCC      ;
		double		Fontes	    [COISAS_POR_NO];
		double		Peso      ; // Peso dado a cada ponto (necessario somente para GAM)

	public     :
			Pt_Malha()     ;
			~Pt_Malha()     ;

		int             Esta_Associado(long);			// verifica se o material est� associada ao ponto
		int             Esta_Associado(double);			// verifica se a fonte est� associada ao ponto

		short int		Get_Material(void);			// retorna o numero de materiais associados ao no
		long			Get_Material(short int i);	// retorna o indice do material i
		long *			Get_Material(long *);		// retorna array de materiais
		void			Set_Material(long mat);
		void			Set_Material(long * mat, short i);

		short int		Get_Fonte(void);			// retorna o numero de fontes associados ao no
		double			Get_Fonte(short int i);	// retorna o valor da fonte i associada ao no
		double *		Get_Fonte(double *);		// retorna array de fontes
		void			Set_Fonte(double f);
		void			Set_Fonte(double * f, short i);

		short int		Get_DerivadaPrim(void);		// retorna o numero de deriv.prim. associados ao no
		double			Get_DerivadaPrim(short i);	// retorna o valor da Deriv.Prim. i associada ao no
		double *		Get_DerivadaPrim(double *);	// retorna o array de Deriv.Prim.
		void			Set_DerivadaPrim(double dp, long = 0);
		void			Set_DerivadaPrim(double * dp, short i);

		double			Get_Potencial();
		void			Set_Potencial(double p);

		double			Get_ValorCC();
		void			Set_ValorCC(double vcc);

		double			Get_Num_Geometrias (void);
		void			Set_Num_Geometrias(int n);

		short int   	Get_TipoCC    (void);
	    void			Set_TipoCC    (short int);

		double			Get_Peso      (void);
	    void			Set_Peso      (double);
		void            Build_Peso      (char  tipo, double Pes=0);

//		Pt_Malha &	operator =  (Pt_Malha const &);
//		virtual char operator == (Pt_Malha &);
		Pt_Malha &	operator =  (Pt_Malha const &);
		virtual char operator == (Pt_Malha const &);

} ;

//----------------------------------------------------------------------------------------
class Pt_El : public Ponto_Base  {
//	friend class Lista_Pontos;
	protected  :
	 double Potencial ;


	public     :
				Pt_El()     ;
			 ~Pt_El()     ;
	        double			Get_Potencial(void);
	        void		   Set_Potencial(double);
			  Pt_El & 		operator= (Pt_El &);
	 virtual char        operator == (Pt_El &);

} ;


#endif
