#ifndef __STRUCTURED_MESH_ATRACTION__
#define __STRUCTURED_MESH_ATRACTION__

//===================================================================================================
//	Arquivo:			atraction.h
//...................................................................................................
//	Descri��o:			Interface da classe CStructuredMeshAtraction
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshAtraction
//---------------------------------------------------------------------------------------------------
//
//	Descri��o: Armazenamento das informa��es de pontos e linhas de atra��o definidos
//				para uma malha estruturada
//
//...................................................................................................
//	Data:				02/03/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

class CStructuredMeshAtraction
{
protected:
	unsigned int	line;			// indice de linha de atra��o
	unsigned int	column;			// indice de coluna de atra��o
	unsigned int	indexI;			// indice I de ponto de atra��o
	unsigned int	indexJ;			// indice J de ponto de atra��o

	unsigned int	subregion;		// indice da subregi�o em que o ponto ou linha � ponto de atra��o

	//coeficientes de atra��o
	double	coefA;
	double	coefB;
	double	coefC;
	double	coefD;



public:

	CStructuredMeshAtraction();
	~CStructuredMeshAtraction();

	void			Line(unsigned int index);
	unsigned int	Line(void); 

	void			Column(unsigned int index);
	unsigned int	Column(void);

	void			IndexI(unsigned int index);
	unsigned int	IndexI(void); 

	void			IndexJ(unsigned int index);
	unsigned int	IndexJ(void); 

	void			IndexSubRegion(unsigned int index);
	unsigned int	IndexSubRegion(void); 

	void			CoefA(double a);
	double			CoefA(void);

	void			CoefB(double b);
	double			CoefB(void);

	void			CoefC(double c);
	double			CoefC(void);
	
	void			CoefD(double d);
	double			CoefD(void);

//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
	CStructuredMeshAtraction & operator = (const CStructuredMeshAtraction &);
	bool operator ==	(const CStructuredMeshAtraction &a);

};

#endif