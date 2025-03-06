#ifndef __STRUCTURED_MESH_OBSTACLES__
#define __STRUCTURED_MESH_OBSTACLES__

//===================================================================================================
//	Arquivo:			obstacles.h
//...................................................................................................
//	Descrição:			Interface da classe CStructuredMeshObstacles
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshObstacles
//---------------------------------------------------------------------------------------------------
//
//	Descrição: Armazenamento das informações dos pontos inicial e final de obstáculos definidos
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
	unsigned int	i_start;		// indice do ponto inicial do obstáculo
	unsigned int	j_start;		// indice do ponto inicial do obstáculo
	unsigned int	i_end;		// indice do ponto inicial do obstáculo
	unsigned int	j_end;		// indice do ponto inicial do obstáculo



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