/* 
 * File:   CProject_Solver_Temporario.h
 * Author: nancy
 *
 * Created on 14 de Agosto de 2012, 15:55
 */

#ifndef CPROJECT_SOLVER_TEMPORARIO_H
#define	CPROJECT_SOLVER_TEMPORARIO_H

//#include "project_solver.h"

#include "phenspec.h"

class CProject_Spec
{
    
public:
    
    lev::CPhenSpecBase o_phen_specbase;
	std::string	sProjectDimension;				///< string que armazena a dimensďż˝o adotada para a simulaďż˝ďż˝o (one_dimensional, two_dimensional, three_dimensional)
	int			iProjectDimension;
    
    
    CProject_Spec();
    CProject_Spec(const CProject_Spec& orig);
    virtual ~CProject_Spec();

    std::vector<std::string> ExtractPathAndNameForWindowsOS(std::string namewithpath);
    
    std::string BuildName(std::string,std::string);
 
    std::string GetProjectNameOnly(void){return sProjectNameOnly;}// *NMA* como os projetos tem sido usados localmente, não sei se essas funções são necessárias
    std::string GetProjectPath(void){return sProjectPath;}
    
    int ReadSpecDataFile(std::string filename);
    
    
    inline void			Dimension(std::string dim) 	///< Operaďż˝ďż˝o que atribui dimensďż˝o adotada para o projeto
						{
							sProjectDimension = dim;
							if (sProjectDimension == "one_dimensional")       iProjectDimension = 1;
							else if (sProjectDimension == "two_dimensional")  iProjectDimension = 2;
							else if (sProjectDimension == "three_dimensional")iProjectDimension = 3;
						}
	inline std::string	Dimension(void){return sProjectDimension;}	///< Operaďż˝ďż˝o que retorna dimensďż˝o adotada para o projeto
	inline int			IntDimension(void){ return iProjectDimension;}    
    
    
private:

protected:
        std::string sProjectNameOnly, sProjectPath;
};

#endif	/* CPROJECT_SOLVER_TEMPORARIO_H */

