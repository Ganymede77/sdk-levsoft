#ifndef __PROJECT_SOLVER__
#define __PROJECT_SOLVER__


#include "lo_templ.hpp"
#include "ve_templ.hpp"
#include "fin_elem.hpp"
#include "region.hpp"
#include "point.hpp"
#include "material.hpp"
#include "project_base.h"
#include "levenum.h"
#include <string>


class CLevPhenomenon;


class CProject_Solver : public lev::CProject_Base
{
protected:

	std::string project_name;
	CLevPhenomenon *ptr_phen;

	lev_math::Lo_Templ<Point *, long> *ptr_lo_points;
	_region::Region	*ptr_regions;
	lev_math::Lo_Templ<long, long> *ptr_lo_long_cc_dirichlet;

	bool            delete_ptr_lo_elements_Flag;
	long			num_first_order_points;
	int				num_boundary_conditions;
	int				NM;
	int             NELT , ///< numero de colunas para a propriedade 1 (originalmente para permitividade) ///< para manter consistencia com arquivos antigos
                    NCCOR, ///< numero de colunas para a propriedade 2 (originalmente para condutividade)///< para manter consistencia com arquivos antigos
                    NMAG;  //< numero de colunas para a propriedade 3 (originalmente para permeabilidade), usar este espaço para o caso termico tambem ///< para manter consistencia com arquivos antigos
    std::vector<std::string> vMaterialName; ///< guarda nome dos materiais do arquivo .mei
	int				codigos[800];		// para armazenar codigos do propfis
	double			materiais[800][7];	// para armazenar informacoes do propfis

	virtual Point		  *	Cria_Ponto(long &index, long totalPoints);
	virtual Nodal_Element * Cria_Elemento(short);
	virtual Material	  * Cria_Material(void);
	virtual CSource		  *	Cria_Fonte(void);
			Nodal_Element * get_Finite_Element_Instance(const  std::string type);

public:

	CProject_Solver();
	~CProject_Solver();

	virtual void Associate(lev_math::Lo_Templ<Point *, long> *lp);
	virtual void Associate(_region::Region *l);
	virtual void Associate(lev_math::Lo_Templ<long, long> *l);
	virtual void Associate(CLevPhenomenon *pt_p);

	std::string GetProjectName() { return project_name; }
	std::string GetProjectPath() {
		std::string path;
		int pos1 = project_name.rfind("/");
		int pos2 = project_name.rfind("\\");
		int pos = (pos1 > pos2) ? pos1 : pos2;
		path = project_name.substr(0, pos-1);
		return path;
	}

	virtual short Read(std::string, short int order = 1);
	virtual short ReadElements(std::string, short int order = 1);
	virtual short ReadPoints(std::string);
	virtual short ReadBoundaryConditions(std::string);
	virtual short ReadMaterials(std::string);
	virtual short ReadSources(std::string);
	virtual short ReadBDMaterials(std::string);
    virtual short ReadBDMaterialsv1(std::string filename);

	virtual short Save(std::string, short int order = 1);
	virtual short SaveBoundaryConditions(std::string, short int order = 1);
	virtual short SaveElements(std::string, short int order = 1);
	virtual short SaveMaterials(std::string);
	virtual short SavePoints(std::string, short int order = 1);
	virtual short SaveProj(PHENOMENONTYPE phen=SCALAROPTICALWAVE);
	virtual short SaveSolution(std::string);
	virtual short SaveSolution(std::string filename,lev_math::Lo_Templ<double,long> &sol);

	void  Set_First_Order_N_Points(long n) { num_first_order_points = n;}
	void  SetProjectName(std::string name) { ProjectNameWithPath(name); project_name = name; }
};

#endif
