#ifndef __STRUCTURED_MESH_OBSTACLES__
#define __STRUCTURED_MESH_OBSTACLES__

//===================================================================================================
//	Arquivo:			obstacles.h
//...................................................................................................
//	Descri��o:			Interface da classe CStructuredMeshObstacles
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshObstacles
//---------------------------------------------------------------------------------------------------
//
//	Descri��o: Armazenamento das informa��es dos pontos inicial e final de obst�culos definidos
//				para uma malha estruturada
//
//...................................................................................................
//	Data:				02/03/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

class CStructuredMeshObstacle
{
protected:
	unsigned int	i_start;		// indice do ponto inicial do obst�culo
	unsigned int	j_start;		// indice do ponto inicial do obst�culo
	unsigned int	i_end;		// indice do ponto inicial do obst�culo
	unsigned int	j_end;		// indice do ponto inicial do obst�culo



public:

	CStructuredMeshObstacle();
	~CStructuredMeshObstacle();

			void	IndexIstart(unsigned int index);
	unsigned int	IndexIstart(void); 

			void	IndexJstart(unsigned int index);
	unsigned int	IndexJstart(void); 

			void	IndexIend(unsigned int index);
	unsigned int	IndexIend(void); 

			void	IndexJend(unsigned int index);
	unsigned int	IndexJend(void); 


//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
	CStructuredMeshObstacle & operator = (const CStructuredMeshObstacle &);
	bool operator ==	(const CStructuredMeshObstacle &a);

};

#endif