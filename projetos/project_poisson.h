#ifndef CPROJECT_POISSON_H
#define	CPROJECT_POISSON_H


/* 
 * File:   CProject_Poisson.h
 * Author: nancy
 *
 * Created on 29 de Agosto de 2012, 10:30
 */


#include <vector>
#include <string>
#include "project_solver.h"
#include "material_tensor.h"


/**
 * Projeto: SDK-LEVSOFT
 *
 * A classe CLev_FEM_Poisson_Tensor implementa a construção das matrizes locais dos elementos finitos considerando as propriedades dos materiais como tensores,
 * permitindo, desta forma, o tratamento de materiais com propriedades anisotrópicas.
 * 
 *
*/
class CProject_Poisson : public CProject_Solver
{
protected:
    
	virtual Material            *Cria_Material(void); ///< Devolve ponteiro para material. Cria um ponteiro para objeto da classe CMaterial_Tensor.

public:
    CProject_Poisson();
    virtual ~CProject_Poisson();
    
    virtual int GetMaterialProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK, Point &p, std::string str_phys_property_name );
  	virtual short ReadMaterials(std::string str_propfis); ///< Leitura do arquivo de materiais associados aos elementos. Trata materiais isotrópicos e anisotrópicos.
	virtual short ReadBDMaterials(std::string); ///< Leitura do arquivo de materiais, verifica qual a versão do arquivo .mei e chama a função de leitura adequada.
    virtual short SavePropFis(std::string); ///< Grava arquivo de materiais nome_projeto.mei, caso não exista (compatibilidade com projetos antigos)
	virtual short SaveSolution(); ///< Grava solu��o, corrige problema de formata��o de arquivo em CProject_Solver::SaveSolution, que foi fixado para n�o gravar em nota��o cient�fica!
};


#endif	/* CPROJECT_POISSON_H */
