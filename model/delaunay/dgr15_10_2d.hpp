#if !defined (__DELAUNAY__)
#define __DELAUNAY__


//.............. *NMA*: includes do sistema LEV
#include "fin_elem.hpp"
#include "levdef.h"
#include "levenum.h"
#include "levgeodata.h"
#include "nos_elm.hpp"
#include "lo_long.hpp"
#include "lo_doubl.hpp"
#include "lo_el_2d.hpp"
#include "material.hpp" 
#include "aresta_geom.hpp"//*Guilherme* - aqui esta definida a classe e a lista de geometrias
#include "levtypedefmalha.h"


class Delaunay 
{
public:
	vCLevPtMalha		bloco_pts, nuvem_pontos;       //conexao de mensagem  (Associacao)

	// *RYT* 2015-09-03
	// It was global before
	double SIGMA;

protected :

	 Lo_Nos_Elementos	bloco_nos_elm;	//lista respons�vel pela associa��o de vetores e
										// n�s
	 lev_math::Uchar				Delaunay_Ok;
	 SYMMETRYTYPE		simetria;		// tipo de simetria do problema, conforme
										// definido em LEVenum.H (enum SYMMETRYTYPE)
														
	 long				limite_superior_pontos; // delimita o numero maximo de pontos aser gerado na malha
														// pode ser delimitado pelo usuario  e modificado pela malha na fase de restauracao de geometria
	 double				fator_englobante;			// fator de definicao do paralelepipedo englobante
														// (inicializado com valor 1.3 no construtor
//	 Lo_Pt_Malha		*bloco_pts, * nuvem_pontos;       //conexao de mensagem  (Associacao)
	 CLevGeoData		*geoData;

	 virtual	lev_math::USINT	Altera_Malha           (long,long) = 0 ;  //virtual
	 virtual	lev_math::USINT	Define_P_Insercao      (FACE *, long *ind_elem_elim, lev_math::USINT num_el_elim) = 0;  //virtual
	 virtual	void	Dimensiona_Englobante  (void)     ;  //virtual
	 virtual	void	Elimina_Englobante     (void)     ;  //virtual
	 virtual	void	Gera_Elementos_Partida (void)     ;  //virtual
	
	
public :
	 Delaunay              (void)     ;
	~Delaunay              (void)     ;

//	 virtual	void     Associa                (Indicador_Movel *);

//	virtual	void    Associa                (Lo_Pt_Malha *)    ;
	virtual	void    Associa                (vCLevPtMalha &)    ;

	virtual	long    Discretiza             (void) = 0  ;  //virtual
	long			Get_Limite_Pontos      (void)     ;
	virtual	lev_math::USINT   Inclui_1_Ponto         (long) = 0 ;  //virtual
//	virtual	lev_math::USINT   Refina                 (MESHTYPE metodo) = 0 ;  //virtual
	virtual	lev_math::USINT   Refina                 (int metodo) = 0 ;  //virtual
	virtual	lev_math::USINT   Restaura_Geometria     (void) = 0 ;  // virtual
	long			Set_Limite_Pontos      (long)     ;
	virtual	void    Set_Simetria           (SYMMETRYTYPE tipo)       ;
	virtual	void	Associa_ElmNos			(void)=0;// virtual

	virtual void	Apaga					(void)=0;//
};

class Delaunay2D : virtual public Delaunay 
{
public :
   	void  DBG_Salva_Lista(long, long *, long);
	
	Lista_Elementos_2D_D	*bloco_elem_2D; // *NMA*: mantido como ponteiro para evitar altera��es no c�digo,
	//========  STL =========================
//	Lo_Aresta_Geometria		*bloco_geometria;
	vArestaGeometria		bloco_geometria;
	//======================================

protected :

	lev_math::Uchar					Delaunay2D_Ok;
											//	ver oberva��es em .cpp

	char	Recupera_Propriedades (long  *ind_elem_elim, lev_math::USINT * num_elem_elim,Elemento_2D &elemento);
	int		Mantem_Geometria(long  *ind_elem_elim, lev_math::USINT  num_elem_elim, long ind_ponto);
	int		Verifica_Quebra_Geometria(FACE * faces, lev_math::USINT n_faces_total);
						
	lev_math::USINT	Altera_Malha(long  indice_elemento, long  indice_ponto);
	lev_math::USINT	Define_P_Insercao     (FACE *, long  *ind_elem_elim, lev_math::USINT num_el_elim) ;  //virtual
//	lev_math::USINT	Define_P_Insercao     (FACE *, Lo_Long &ind_elem_elim, long num_el_elim) ;  //virtual
	void	Dimensiona_Englobante (void);
	void	Estatistica           (double &med, double &desv);
	//int   Elimina_Achatado_TipoI(long, int, long *, long *, long *, long *);
	lev_math::USINT	Inclui_1_Ponto        (long);
	virtual void Informa_Erro     (char *, long);
	lev_math::USINT	Refina_Media          (void)  ; // *AP*  problema

	short int Verifica_Tipo_Achatado(Elemento_2D &el, long *vertmarc, long *elemmarc);

	virtual void	Associa_ElmNos			(void){};

public:

	int n_vertices;     // *AP* aten��o : aqui ocorre uma duplica��o de informa��es. O n�mero de v�rtices 
		                    // deve estar associado ao elemento e n�o � malha.

	Delaunay2D(void);
	~Delaunay2D(void);
	
	void	Pre_Discret(int passos = 1);
	void	Limpa_Concavidades();
//	void	Associa      (Indicador_Movel *)     ;
	void	Associa      (Lista_Elementos_2D_D *);

//	void	Associa      (Lo_Pt_Malha *, Lo_Pt_Malha *);
//	void	Associa      (vCLevPtMalha &, vCLevPtMalha &);

//	void	Associa      (Lo_Aresta_Geometria *);
//	void	Associa      (vArestaGeometria &);

	void	Set_Nuvem_Pontos(lev_math::Lo_Templ<Point*,long> &);
//	void	Set_Arestas_Geometria(lev_math::Lo_Templ<Aresta,long> &);

	void	grava_elem_elim(Pt_Malha *, lev_math::USINT *, lev_math::USINT);
	void	grava_faces(FACE *, lev_math::USINT);
	int		grava_pontos(char *);
	void	Def_Nuvem_Pontos                  (void)  ;
	void	Acessa_Capacidade                 (unsigned &maior_bloco, unsigned &total, long &max_pts, long &max_elem);
	void	Dimensiona_Capacidade             (void)  ;
	long	Discretiza                      (void)  ;
	void	Elimina_Achatados			    (void);
 	inline long  Get_N_Elementos_2D         (void)  ;
	inline long  Get_N_Pontos               (void)  ;
//	virtual lev_math::USINT Refina                    (MESHTYPE metodo);
	virtual lev_math::USINT Refina                    (int metodo);
	void	Reinicializa                    (void)  ;
	virtual lev_math::USINT    Restaura_Geometria     (void) /*{return lev_math::USINT (0);}*/ ;
	void	Restitui_Degenerados            (void)  ;
	void	Gera_Elementos_Partida          (void)  ;
	void	Elimina_Englobante              (void)  ;  //virtual
	void	Lista_Elementos_Contiguos		(Lista_Elementos_2D_D * list_cont, long * list_index,long primeiro, const int exclui);

// *NMA*: Fun��es criadas para gera��o de malha integrada no Sistema LEV
	void	  			Apaga			(void);
	void				Associa			(CLevGeoData *);
	int					BuildInitialData(int );

	void				Set_N_Max_Mesh_Elements(long);		//*NMA*: Inicializa numero maximo de bloco_elem_2D

	// devolve dados usando estrutura de dados stl
	vCLevPtMalha	 &	GetMeshPoints	(void);				//*NMA*: devolve os pontos ap�s a gera��o da malha
	vArestaGeometria &	GetGeometryEdges	(void);			//*NMA*: devolve as arestas de geometria ap�s a gera��o da malha
	void				GetMeshElements	(vElemento2D  & );			//*NMA*: devolve os elementos ap�s a gera��o da malha

};


#endif
